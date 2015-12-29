#include "defs.h"

float angle_difference(float x, float y)
{
  float Dif = x-y;
  NORMALISE_DEG(Dif);
  if(Dif > 180)
    return (360-Dif);
  else
    return Dif;
}

int simulation_end(world* our_world)
{
  simulation.world = NULL;
  simulation.pause = 1;
  simulation.quit = 1;
  simulation.delay = 20;
  simulation.logging = 0;
  fclose(simulation.log_file);
  return 0;
}

int simulation_init(world* our_world)
{
  simulation.world = our_world;
  simulation.pause = 1;
  simulation.quit = 0;
  simulation.delay = 20;
  simulation.logging = 1;
  simulation.log_file = fopen("Log.txt", "w");
  return 0;
}

void update_bots_positions(world* our_world)
{
  int b;
  for(b = 0; b < our_world->num_bots; ++b)
  {
    if(our_world->bots[b].dead)
      continue;

    // update position & rotation (nn)
    our_world->bots[b].x += 0.1 * our_world->bots[b].nn.output[our_world->bots[b].nn.num_layers-1][0] * sin(DEG_TO_RAD(our_world->bots[b].r));
    our_world->bots[b].y += 0.1 * our_world->bots[b].nn.output[our_world->bots[b].nn.num_layers-1][0] * cos(DEG_TO_RAD(our_world->bots[b].r));
    our_world->bots[b].r += our_world->bots[b].turn_rate*(our_world->bots[b].nn.output[our_world->bots[b].nn.num_layers-1][1]-0.5);

    // position & rotation checks
    if(simulation.world->bots[b].x < 0)
      simulation.world->bots[b].x += simulation.world->width;
    else if(simulation.world->bots[b].x > simulation.world->width)
      simulation.world->bots[b].x -= simulation.world->width;

    if(simulation.world->bots[b].y < 0)
      simulation.world->bots[b].y += simulation.world->height;
    else if(simulation.world->bots[b].y > simulation.world->height)
      simulation.world->bots[b].y -= simulation.world->height;

    if(simulation.world->bots[b].r < 0)
      simulation.world->bots[b].r += 360;
    else if(simulation.world->bots[b].r > 360)
      simulation.world->bots[b].r -= 360;
  }
}

void update_bots_inputs(world* our_world)
{
  int b, b2, e, p;
  for(b = 0; b < simulation.world->num_bots; ++b)
  {
    if(our_world->bots[b].dead)
      continue;

    // Resets
    for(e = 0; e < simulation.world->bots[b].num_eyes; ++e)
    {
      simulation.world->bots[b].eyes[e].in_strength = 0.0; //simulation.world->bots[b].eyes[e].view_distance;
      simulation.world->bots[b].eyes[e].in_red = 0.0;
      simulation.world->bots[b].eyes[e].in_green = 0.0;
      simulation.world->bots[b].eyes[e].in_blue = 0.0;
    }

    // Look for pellets
    for(p = 0; p < our_world->num_pellets; ++p)
    {
      float dx = our_world->pellets[p].x - our_world->bots[b].x;
      float dy = our_world->pellets[p].y - our_world->bots[b].y;
      float Dist = sqrt(dx*dx + dy*dy);
      float Angle = 0;
      if(dy == 0)
      {
        if(dx >= 0) {Angle =  90 - our_world->bots[b].r;}
        if(dx <  0) {Angle = 270 - our_world->bots[b].r;}
      }
      else
      {
        Angle = RAD_TO_DEG(atan(dx/dy)) - our_world->bots[b].r;
        if(dy < 0) Angle += 180;
      }
      NORMALISE_DEG(Angle);

      // eyes
      for(e = 0; e < our_world->bots[b].num_eyes; ++e)
      {
        if(angle_difference(Angle, our_world->bots[b].eyes[e].position) <= our_world->bots[b].eyes[e].view_angle &&
          (our_world->bots[b].eyes[e].view_distance-Dist)/our_world->bots[b].eyes[e].view_distance > our_world->bots[b].eyes[e].in_strength)
          //Dist < our_world->bots[b].eyes[e].in_strength)
        {
        //our_world->bots[b].eyes[e].in_strength = Dist;
        our_world->bots[b].eyes[e].in_strength = (our_world->bots[b].eyes[e].view_distance-Dist)/our_world->bots[b].eyes[e].view_distance;
        our_world->bots[b].eyes[e].in_red   = our_world->pellets[p].red;
        our_world->bots[b].eyes[e].in_green = our_world->pellets[p].green;
        our_world->bots[b].eyes[e].in_blue  = our_world->pellets[p].blue;
        }
      }
    }

    // Look for bots
    /*
    for(b2 = 0; b2 < our_world->num_bots; ++b2)
    {
      if(b2 == b) continue; // We can't see ourself

      float dx = our_world->bots[b2].x - our_world->bots[b].x;
      float dy = our_world->bots[b2].y - our_world->bots[b].y;
      float Dist = sqrt(dx*dx + dy*dy);
      float Angle = 0;
      if(dy == 0)
      {
        if(dx >= 0) {Angle =  90 - our_world->bots[b].r;}
        if(dx <  0) {Angle = 270 - our_world->bots[b].r;}
      }
      else
      {
        Angle = RAD_TO_DEG(atan(dx/dy)) - our_world->bots[b].r;
        if(dy < 0) Angle += 180;
      }
      NORMALISE_DEG(Angle);

      // eyes
      for(e = 0; e < our_world->bots[b].num_eyes; ++e)
      {
        if(angle_difference(Angle, our_world->bots[b].eyes[e].position) <= our_world->bots[b].eyes[e].view_angle &&
          Dist < our_world->bots[b].eyes[e].in_strength)
        {
        our_world->bots[b].eyes[e].in_strength = Dist;
        our_world->bots[b].eyes[e].in_red   = our_world->bots[b2].red;
        our_world->bots[b].eyes[e].in_green = our_world->bots[b2].green;
        our_world->bots[b].eyes[e].in_blue  = our_world->bots[b2].blue;
        }
      }
    }
    */
  }
}

void update_pellets(world* our_world)
{
  int p, b;
  for(p = 0; p < our_world->num_pellets; ++p)
  {
    for(b = 0; b < our_world->num_bots; ++b)
    {
      if(DIST(our_world->pellets[p].x, our_world->pellets[p].y,
              our_world->bots[b].x,    our_world->bots[b].y) < 0.15)
      {
        our_world->bots[b].energy += our_world->pellets[p].nutrition;

        pellet_remove(our_world, p);
        pellet_add(our_world, -1, -1);
        p--;
        break;
      }
    }
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

    update_bots_inputs(simulation.world);
    update_bots_nns(simulation.world);
    update_bots_positions(simulation.world);
    update_pellets(simulation.world);

    int b;
    // energy
    for(b = 0; b < simulation.world->num_bots; ++b)
    {
      if(simulation.world->bots[b].dead)
        continue;

      if(simulation.world->bots[b].energy <= 0)
        bot_kill(simulation.world, b);
      else
        simulation.world->bots[b].energy--;
    }

    // age
    for(b = 0; b < simulation.world->num_bots; ++b)
    {
      if(simulation.world->bots[b].dead)
        continue;

      simulation.world->bots[b].age++;
    }

    // Next generation check
    if(simulation.world->num_bots_alive <= simulation.world->num_parents ||
       simulation.world->frame >= 2000)
    {
      // rank the bots
      bot_ranks(simulation.world);
      printf("Best: ");
      for(b = 0; b < simulation.world->num_bots; ++b)
        printf("%i ", simulation.world->bot_ranks[b]);
      printf("\n");

      // breed new generation
      bots_breed_new_generation(simulation.world);
      simulation.world->num_bots_alive = simulation.world->num_bots;

      // logging
      fprintf(simulation.log_file, "%i\t %i\t\n", simulation.world->generation, simulation.world->pellets_removed);

      // Reset logging
      simulation.world->pellets_removed = 0;

      // Next generation
      simulation.world->generation++;
      simulation.world->frame = 0;
    }
    else
    {
      simulation.world->frame++;
    }

    Sleep(simulation.delay);
  }
}
