#include "defs.h"

float angle_difference(float x, float y)
{
  float Dif = x-y;
  NORMALISE_DEG(Dif);
  if(Dif > 180)
  {
    return (360 - Dif);
  }
  else
  {
    return Dif;
  }
}

int simulation_end(s_world *world)
{
  ASSERT(world != NULL);
  
  simulation.world = NULL;
  simulation.pause = 1;
  simulation.quit = 1;
  simulation.delay = 20;
  #ifdef LOGGING
  fclose(simulation.log_file);
  #endif
  return 0;
}

int simulation_init(s_world *world)
{
  ASSERT(world != NULL);
  
  simulation.world = world;
  simulation.pause = 1;
  simulation.quit = 0;
  simulation.delay = 20;
  #ifdef LOGGING
  simulation.log_file = fopen("log.txt", "w");
  #endif
  return 0;
}

void update_bots_positions(s_world *world)
{
  ASSERT(world != NULL);
  
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    if(world->bots[b].dead)
    {
      continue;
    }

    // update position & rotation (nn)
    world->bots[b].x += 0.1 * world->bots[b].nn.output[world->bots[b].nn.num_layers-1][0] * sin(DEG_TO_RAD(world->bots[b].r));
    world->bots[b].y += 0.1 * world->bots[b].nn.output[world->bots[b].nn.num_layers-1][0] * cos(DEG_TO_RAD(world->bots[b].r));
    world->bots[b].r += world->bots[b].turn_rate*(world->bots[b].nn.output[world->bots[b].nn.num_layers-1][1]-0.5);

    // position & rotation checks
    if(simulation.world->bots[b].x < 0.0)
    {
      simulation.world->bots[b].x += simulation.world->width;
    }
    else if(simulation.world->bots[b].x > simulation.world->width)
    {
      simulation.world->bots[b].x -= simulation.world->width;
    }

    if(simulation.world->bots[b].y < 0.0)
    {
      simulation.world->bots[b].y += simulation.world->height;
    }
    else if(simulation.world->bots[b].y > simulation.world->height)
    {
      simulation.world->bots[b].y -= simulation.world->height;
    }

    if(simulation.world->bots[b].r < 0.0)
    {
      simulation.world->bots[b].r += 360.0;
    }
    else if(simulation.world->bots[b].r >= 360.0)
    {
      simulation.world->bots[b].r -= 360.0;
    }
  }
}

void update_bots_inputs(s_world *world)
{
  ASSERT(world != NULL);
  
  int b, b2, e, p, s;
  for(b = 0; b < simulation.world->num_bots; ++b)
  {
    if(world->bots[b].dead)
    {
      continue;
    }

    // Resets
    for(e = 0; e < simulation.world->bots[b].num_eyes; ++e)
    {
      simulation.world->bots[b].eyes[e].in_strength = 0.0; //simulation.world->bots[b].eyes[e].view_distance;
      simulation.world->bots[b].eyes[e].in_red = 0.0;
      simulation.world->bots[b].eyes[e].in_green = 0.0;
      simulation.world->bots[b].eyes[e].in_blue = 0.0;
    }

    // Look for pellets
    for(p = 0; p < world->num_pellets; ++p)
    {
      float dx = world->pellets[p].x - world->bots[b].x;
      if(dx > MAX_VIEW_DIST || dx < -MAX_VIEW_DIST) {continue;}
      
      float dy = world->pellets[p].y - world->bots[b].y;
      if(dy > MAX_VIEW_DIST || dy < -MAX_VIEW_DIST) {continue;}
      
      float dist = sqrt(dx*dx + dy*dy);
      if(dist > MAX_VIEW_DIST) {continue;}
      
      // eating
      if(dist < 0.15)
      {
        world->bots[b].energy += world->pellets[p].nutrition;
        pellet_remove(world, p);
        pellet_add(world, -1, -1);
        p--;
        continue;
      }
      
      float angle = 0;
      if(dy == 0.0)
      {
        if(dx >= 0.0) {angle =  90.0 - world->bots[b].r;}
        if(dx <  0.0) {angle = 270.0 - world->bots[b].r;}
      }
      else
      {
        angle = RAD_TO_DEG(atan(dx/dy)) - world->bots[b].r;
        if(dy < 0.0) {angle += 180.0;}
      }
      NORMALISE_DEG(angle);

      // eyes
      for(e = 0; e < world->bots[b].num_eyes; ++e)
      {
        if(angle_difference(angle, world->bots[b].eyes[e].position) <= world->bots[b].eyes[e].view_angle &&
          (world->bots[b].eyes[e].view_distance-dist)/world->bots[b].eyes[e].view_distance > world->bots[b].eyes[e].in_strength)
          //dist < world->bots[b].eyes[e].in_strength)
        {
        //world->bots[b].eyes[e].in_strength = dist;
        world->bots[b].eyes[e].in_strength = (world->bots[b].eyes[e].view_distance-dist)/world->bots[b].eyes[e].view_distance;
        world->bots[b].eyes[e].in_red   = world->pellets[p].red;
        world->bots[b].eyes[e].in_green = world->pellets[p].green;
        world->bots[b].eyes[e].in_blue  = world->pellets[p].blue;
        }
      }
    }

    // Look for bots
    for(b2 = 0; b2 < world->num_bots; ++b2)
    {
      if(b2 == b) {continue;} // We can't see ourself

      float dx = world->bots[b2].x - world->bots[b].x;
      if(dx > MAX_VIEW_DIST || dx < -MAX_VIEW_DIST) {continue;}
      
      float dy = world->bots[b2].y - world->bots[b].y;
      if(dy > MAX_VIEW_DIST || dy < -MAX_VIEW_DIST) {continue;}
      
      float dist = sqrt(dx*dx + dy*dy);
      if(dist > MAX_VIEW_DIST) {continue;}
      
      float angle = 0.0;
      if(dy == 0.0)
      {
        if(dx >= 0.0) {angle =  90.0 - world->bots[b].r;}
        if(dx <  0.0) {angle = 270.0 - world->bots[b].r;}
      }
      else
      {
        angle = RAD_TO_DEG(atan(dx/dy)) - world->bots[b].r;
        if(dy < 0.0) {angle += 180.0;}
      }
      NORMALISE_DEG(angle);

      // eyes
      for(e = 0; e < world->bots[b].num_eyes; ++e)
      {
        if(angle_difference(angle, world->bots[b].eyes[e].position) <= world->bots[b].eyes[e].view_angle &&
          (world->bots[b].eyes[e].view_distance-dist)/world->bots[b].eyes[e].view_distance > world->bots[b].eyes[e].in_strength)
          //dist < world->bots[b].eyes[e].in_strength)
        {
        world->bots[b].eyes[e].in_strength = dist;
        world->bots[b].eyes[e].in_red   = world->bots[b2].red;
        world->bots[b].eyes[e].in_green = world->bots[b2].green;
        world->bots[b].eyes[e].in_blue  = world->bots[b2].blue;
        }
      }
      
      // spikes
      for(s = 0; s < world->bots[b].num_spikes; ++s)
      {
        if(dist < world->bots[b].spikes[s].length && world->bots[b].nn.output[world->bots[b].nn.num_layers-1][2] >= 0.5)
        {
          world->bots[b2].energy -= 15;
          world->bots[b].energy += 10;
          world->spike_uses++;
        }
      }
    }
  }
}

void simulate_frame(s_world* world)
{
  ASSERT(world != NULL);
  
  update_bots_inputs(simulation.world);
  update_bots_nns(simulation.world);
  update_bots_positions(simulation.world);

  int b;
  // energy
  for(b = 0; b < simulation.world->num_bots; ++b)
  {
    if(simulation.world->bots[b].dead)
    {
      continue;
    }

    if(simulation.world->bots[b].energy <= 0)
    {
      bot_kill(simulation.world, b);
    }
    else
    {
      simulation.world->bots[b].energy--;
    }
  }

  // age
  for(b = 0; b < simulation.world->num_bots; ++b)
  {
    if(simulation.world->bots[b].dead)
    {
      continue;
    }

    simulation.world->bots[b].age++;
  }

  // Next generation check
  if(simulation.world->num_bots_alive <= simulation.world->num_parents || simulation.world->frame >= MAX_FRAMES_PER_GENERATION)
  {
    // Stats
    simulation.world->average_fitness = 0;
    for(b = 0; b < simulation.world->num_bots; ++b)
    {
      simulation.world->average_fitness += simulation.world->bots[b].age;
    }
    simulation.world->average_fitness = (int)((float)simulation.world->average_fitness / simulation.world->num_bots);
    
    // rank the bots
    bot_ranks(simulation.world);
    #ifdef DEBUG
    printf("Best: ");
    for(b = 0; b < simulation.world->num_bots; ++b)
    {
      printf("%i ", simulation.world->bot_ranks[b]);
    }
    printf("\n");
    #endif

    // breed new generation
    bots_breed_new_generation(simulation.world);
    simulation.world->num_bots_alive = simulation.world->num_bots;

    // logging
    #ifdef LOGGING
    //fprintf(simulation.log_file, "%i\t %i\t\n", simulation.world->generation, simulation.world->pellets_removed);
    fprintf(simulation.log_file, "%i", simulation.world->generation);
    fprintf(simulation.log_file, "\t%i", simulation.world->average_fitness);
    fprintf(simulation.log_file, "\t%i", simulation.world->pellets_removed);
    fprintf(simulation.log_file, "\t%i", simulation.world->spike_uses);
    fprintf(simulation.log_file, "\n");
    #endif
    
    // Reset logging
    simulation.world->pellets_removed = 0;
    simulation.world->spike_uses = 0;

    // Next generation
    simulation.world->generation++;
    simulation.world->frame = 0;
    
    // Reposition pellets
    int p;
    for(p = 0; p < world->num_pellets; ++p)
    {
      world->pellets[p].x = RAND_BETWEEN(0.0, world->width);
      world->pellets[p].y = RAND_BETWEEN(0.0, world->height);
    }
  }
  else
  {
    simulation.world->frame++;
  }
}

void simulate_world(void* a)
{
  while(!simulation.quit)
  {
    if(simulation.pause)
    {
      update_bots_inputs(simulation.world);
      Sleep(100);
      continue;
    }

    simulate_frame(simulation.world);
    
    Sleep(simulation.delay);
  }
}
