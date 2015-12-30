#include "defs.h"

#define EYE_ACCURACY 15

void draw_bot(s_bot *bot)
{
  ASSERT(bot != NULL);
  
  int e; // Current eye
  int s; // Current spike

  // draw vision triangles
  float accuracy_angle;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for(e = 0; e < bot->num_eyes; ++e)
  {
    accuracy_angle = (float)bot->eyes[e].view_angle*2/EYE_ACCURACY;
    glPushMatrix();
    glColor3f(bot->eyes[e].in_red, bot->eyes[e].in_green, bot->eyes[e].in_blue);

    glRotatef(bot->r+bot->eyes[e].position-bot->eyes[e].view_angle, 0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    int i;
    for(i = 0; i <= EYE_ACCURACY; ++i)
    {
      glVertex2f((float)bot->eyes[e].view_distance*sin(DEG_TO_RAD(accuracy_angle*i)),
                 (float)bot->eyes[e].view_distance*cos(DEG_TO_RAD(accuracy_angle*i)));
    }
    glEnd();
    glPopMatrix();
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // draw spikes
  for(s = 0; s < bot->num_spikes; ++s)
  {
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glRotatef(bot->r, 0.0, 0.0, -1.0);
    glBegin(GL_TRIANGLES);
      glVertex2f( 0.00, 0.20 + bot->spikes[s].length);
      glVertex2f( 0.05, 0.15);
      glVertex2f(-0.05, 0.15);
    glEnd();
    glPopMatrix();
  }

  // draw body
  glPushMatrix();
  glColor3f(bot->red, bot->green, bot->blue);
  glRotatef(bot->r, 0,0,-1);
  glBegin(GL_POLYGON);
    glVertex2f( 0.00,  0.20);
    glVertex2f( 0.19,  0.06);
    glVertex2f( 0.12, -0.16);
    glVertex2f(-0.12, -0.16);
    glVertex2f(-0.19,  0.06);
  glEnd();
  glPopMatrix();

  // draw eyes
  for(e = 0; e < bot->num_eyes; ++e)
  {
    glPushMatrix();
    glColor3f(bot->eyes[e].in_red, bot->eyes[e].in_green, bot->eyes[e].in_blue);
    glRotatef(bot->r, 0.0, 0.0, -1.0);
    float yPos = 0.25 * cos(DEG_TO_RAD(bot->eyes[e].position)) - 0.05;
    float xPos = 0.25 * sin(DEG_TO_RAD(bot->eyes[e].position)) - 0.05;
    glBegin(GL_QUADS);
      glVertex2f(xPos    , yPos    );
      glVertex2f(xPos    , yPos+0.1);
      glVertex2f(xPos+0.1, yPos+0.1);
      glVertex2f(xPos+0.1, yPos    );
    glEnd();
    glPopMatrix();
  }
}

void draw_edge_bots(s_world *world)
{
  ASSERT(world != NULL);
  
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    // Bottom of the world
    if(world->bots[b].y < 0.25*world->height)
    {
      glPushMatrix();
      glTranslatef(world->bots[b].x, world->bots[b].y+world->height, 0.0);
      draw_bot(&world->bots[b]);
      glPopMatrix();
      
      // Bottom left
      if(world->bots[b].x < 0.25*world->width)
      {
        glPushMatrix();
        glTranslatef(world->bots[b].x+world->width, world->bots[b].y+world->height, 0.0);
        draw_bot(&world->bots[b]);
        glPopMatrix();
      }
      // Bottom right
      else if(world->bots[b].x > 0.75*world->width)
      {
        glPushMatrix();
        glTranslatef(world->bots[b].x-world->width, world->bots[b].y+world->height, 0.0);
        draw_bot(&world->bots[b]);
        glPopMatrix();
      }
    }
    // Top of the world
    if(world->bots[b].y > 0.75*world->height)
    {
      glPushMatrix();
      glTranslatef(world->bots[b].x, world->bots[b].y-world->height, 0.0);
      draw_bot(&world->bots[b]);
      glPopMatrix();
      
      // Top left
      if(world->bots[b].x < 0.25*world->width)
      {
        glPushMatrix();
        glTranslatef(world->bots[b].x+world->width, world->bots[b].y-world->height, 0.0);
        draw_bot(&world->bots[b]);
        glPopMatrix();
      }
      // Top right
      else if(world->bots[b].x > 0.75*world->width)
      {
        glPushMatrix();
        glTranslatef(world->bots[b].x-world->width, world->bots[b].y-world->height, 0.0);
        draw_bot(&world->bots[b]);
        glPopMatrix();
      }
    }
    // Left of the world
    if(world->bots[b].x < 0.25*world->width)
    {
      glPushMatrix();
      glTranslatef(world->bots[b].x+world->width, world->bots[b].y, 0.0);
      draw_bot(&world->bots[b]);
      glPopMatrix();
    }
    // Right of the world
    if(world->bots[b].x > 0.75*world->width)
    {
      glPushMatrix();
      glTranslatef(world->bots[b].x-world->width, world->bots[b].y, 0.0);
      draw_bot(&world->bots[b]);
      glPopMatrix();
    }
  }
  
  // Cover the outsides
  // Left side
  glPushMatrix();
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(-world->width, 0.0);
    glVertex2f(-world->width, world->height);
    glVertex2f(0.0, world->height);
    glVertex2f(0.0, 0.0);
  glEnd();
  glPopMatrix();
  // Right side
  glPushMatrix();
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(world->width, 0.0);
    glVertex2f(world->width, world->height);
    glVertex2f(2*world->width, world->height);
    glVertex2f(2*world->width, 0.0);
  glEnd();
  glPopMatrix();
  // Top long side
  glPushMatrix();
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(-world->width, world->height);
    glVertex2f(-world->width, 2*world->height);
    glVertex2f(2*world->width, 2*world->height);
    glVertex2f(2*world->width, world->height);
  glEnd();
  glPopMatrix();
  // Bottom long side
  glPushMatrix();
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(-world->width, -world->height);
    glVertex2f(-world->width, 0.0);
    glVertex2f(2*world->width, 0.0);
    glVertex2f(2*world->width, -world->height);
  glEnd();
  glPopMatrix();
}

void draw_bots(s_world *world)
{
  ASSERT(world != NULL);
  
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    glPushMatrix();
    glTranslatef(world->bots[b].x, world->bots[b].y, 0.0);
    draw_bot(&world->bots[b]);
    glPopMatrix();
  }
}

int bot_find_closest(s_world *world, float x, float y)
{
  ASSERT(world != NULL);
  
  int closest_bot = -1;
  float max_Range = 0.5;
  float closest_Range = 999999;

  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    if(DIST(world->bots[b].x, world->bots[b].y, x, y) < closest_Range)
    {
      closest_Range = DIST(world->bots[b].x, world->bots[b].y, x, y);
      closest_bot = b;
    }
  }

  if(closest_Range < max_Range)
  {
    printf("Bot selected - s_bot: %i, Range: %.4g\n", closest_bot, closest_Range);
    return closest_bot;
  }
  else
  {
    printf("No bot selected - closest: %i, %.4g\n", closest_bot, closest_Range);
    return -1;
  }
}

int bot_copy(s_bot *dest, s_bot *source)
{
  if(dest == NULL || source == NULL)
  {
    return 0;
  }

  // Manual copy
  dest->energy = source->energy;
  dest->age = source->age;
  dest->dead = source->dead;
  dest->x = source->x;
  dest->y = source->y;
  dest->r = source->r;
  dest->size = source->size;
  dest->turn_rate = source->turn_rate;
  dest->red = source->red;
  dest->green = source->green;
  dest->blue = source->blue;
  dest->num_eyes = source->num_eyes;
  dest->num_spikes = source->num_spikes;

  // Pointer stuff (eyes)
  int e;
  for(e = 0; e < source->num_eyes; ++e)
  {
    dest->eyes[e].position      = source->eyes[e].position;
    dest->eyes[e].view_distance = source->eyes[e].view_distance;
    dest->eyes[e].view_angle    = source->eyes[e].view_angle;
    dest->eyes[e].red           = source->eyes[e].red;
    dest->eyes[e].green         = source->eyes[e].green;
    dest->eyes[e].blue          = source->eyes[e].blue;
    dest->eyes[e].in_strength   = source->eyes[e].in_strength;
    dest->eyes[e].in_red        = source->eyes[e].in_red;
    dest->eyes[e].in_green      = source->eyes[e].in_green;
    dest->eyes[e].in_blue       = source->eyes[e].in_blue;
  }

  // Pointer stuff (spikes)
  int s;
  for(s = 0; s < dest->num_spikes; ++s)
  {
    dest->spikes[s].retracted = source->spikes[s].retracted;
    dest->spikes[s].position  = source->spikes[s].position;
    dest->spikes[s].length    = source->spikes[s].length;
  }

  // Pointer stuff (Neural network)
  dest->nn.num_layers = source->nn.num_layers;
  dest->nn.num_inputs = source->nn.num_inputs;
  dest->nn.num_outputs = source->nn.num_outputs;
  dest->nn.layer_sizes = source->nn.layer_sizes;
  int i, j, k;
  for(i = 1; i < dest->nn.num_layers; ++i)
  {
    for(j = 0; j < dest->nn.layer_sizes[i]; ++j)
    {
      for(k = 0; k < dest->nn.layer_sizes[i-1]; ++k)
      {
        dest->nn.weights[i][j][k] = source->nn.weights[i][j][k];
      }
    }
  }

  return 0;
}

int bot_refresh(s_bot *bot)
{
  ASSERT(bot != NULL);
  
  bot->energy = BOT_START_ENERGY;
  bot->age = 0;
  bot->dead = 0;
  return 0;
}

int bot_breed(s_bot *Child, s_bot *parent1, s_bot *parent2)
{
  if(Child == NULL || parent1 == NULL || parent2 == NULL)
  {
    return 0;
  }

  // Basic version
  /*
  if(rand()%2 == 0)
  {
    bot_copy(Child, parent1);
  }
  else
  {
    bot_copy(Child, parent2);
  }
  */

  // Crossover version
  bot_copy(Child, parent1);
  if(rand()%2 == 0) {Child->red = parent2->red;}
  if(rand()%2 == 0) {Child->green = parent2->green;}
  if(rand()%2 == 0) {Child->blue = parent2->blue;}
  if(rand()%2 == 0) {Child->size = parent2->size;}
  if(rand()%2 == 0) {Child->turn_rate = parent2->turn_rate;}
  /*
  if(rand()%2 == 0) {Child->xxxx = parent2->xxxx;}
  if(rand()%2 == 0) {Child->xxxx = parent2->xxxx;}
  if(rand()%2 == 0) {Child->xxxx = parent2->xxxx;}
  if(rand()%2 == 0) {Child->xxxx = parent2->xxxx;}
  if(rand()%2 == 0) {Child->xxxx = parent2->xxxx;}
  */

  return 0;
}

int bot_mutate(s_bot *bot)
{
  ASSERT(bot != NULL);
  
  // Movement
  bot->turn_rate += RAND_BETWEEN(-0.1, 0.1);
  
  // Colour
  bot->red += RAND_BETWEEN(-0.1, 0.1);
  bot->green += RAND_BETWEEN(-0.1, 0.1);
  bot->blue += RAND_BETWEEN(-0.1, 0.1);

  // Neural network
  nn_mutate(&bot->nn);

  // eyes
  #ifdef MUTATE_EYES
  int e;
  for(e = 0; e < bot->num_eyes; ++e)
  {
    bot->eyes[e].position += RAND_BETWEEN(-1.0, 1.0);
         if(bot->eyes[e].position >= 360.0) {bot->eyes[e].position -= 360.0;}
    else if(bot->eyes[e].position < 0.0)    {bot->eyes[e].position += 360.0;}
    
    bot->eyes[e].view_angle += RAND_BETWEEN(-1.0, 1.0);
         if(bot->eyes[e].view_angle > MAX_VIEW_ANGLE) {bot->eyes[e].view_angle = MAX_VIEW_ANGLE;}
    else if(bot->eyes[e].view_angle < MIN_VIEW_ANGLE) {bot->eyes[e].view_angle = MIN_VIEW_ANGLE;}
    
    bot->eyes[e].view_distance += RAND_BETWEEN(-0.1, 0.1);
         if(bot->eyes[e].view_distance > MAX_VIEW_DIST) {bot->eyes[e].view_distance = MAX_VIEW_DIST;}
    else if(bot->eyes[e].view_distance < MIN_VIEW_DIST) {bot->eyes[e].view_distance = MIN_VIEW_DIST;}
  }
  #endif

  // spikes
  #ifdef MUTATE_SPIKES
  int s;
  for(s = 0; e < bot->num_eyes; ++s)
  {
    bot->spikes[s].position += RAND_BETWEEN(-0.1, 0.1);
         if(bot->spikes[s].position >= 360.0) {bot->spikes[s].position -= 360.0;}
    else if(bot->spikes[s].position < 0.0)    {bot->spikes[s].position += 360.0;}
    
    bot->spikes[s].length += RAND_BETWEEN(-0.1, 0.1);
         if(bot->spikes[s].position > MAX_SPIKE_LENGTH) {bot->spikes[s].position = MAX_SPIKE_LENGTH;}
    else if(bot->spikes[s].position < MIN_SPIKE_LENGTH) {bot->spikes[s].position = MIN_SPIKE_LENGTH;}
  }
  #endif

  return 0;
}

void bots_breed_new_generation(s_world *world)
{
  ASSERT(world != NULL);
  
  // create parent s_bots
  int p;
  for(p = 0; p < world->num_parents; ++p)
  {
    bot_copy(&world->bot_parents[p], &world->bots[world->bot_ranks[p]]);
  }

  // breed new generation from parents
  int b;
  for(b = 0; b < world->num_bots; ++b)
  {
    bot_breed(&world->bots[b],
              &world->bot_parents[rand()%world->num_parents],
              &world->bot_parents[rand()%world->num_parents]);

    // refresh bot
    world->bots[b].x = RAND_BETWEEN(0, world->width);
    world->bots[b].y = RAND_BETWEEN(0, world->height);
    world->bots[b].energy = BOT_START_ENERGY * RAND_BETWEEN(0.8, 1.2);
    world->bots[b].age = 0;
    world->bots[b].dead = 0;

    // mutate bot
    bot_mutate(&world->bots[b]);
  }
}

void bot_ranks(s_world *world)
{
  ASSERT(world != NULL);
  
  #ifndef BOT_SCORE
  #define BOT_SCORE (world->bots[b].age + world->bots[b].energy)
  #endif

  int n;
  //for(n = world->num_bots-1; n >= 0; --n)
  for(n = 0; n < world->num_bots; ++n)
  {
    int Best_bot_Score = -1;
    int Best_bot = -1;

    int b;
    for(b = 0; b < world->num_bots; ++b)
    {
      if(BOT_SCORE > Best_bot_Score)
      {
        Best_bot_Score = BOT_SCORE;
        Best_bot = b;
      }
    }

    world->bot_ranks[n] = Best_bot;
    world->bots[Best_bot].age = -1;
    world->bots[Best_bot].energy = -1;
  }

  #undef BOT_SCORE
}

int bot_kill(s_world *world, int b)
{
  ASSERT(world != NULL);
  
  if(b >= world->num_bots || b < 0 || world->bots[b].dead)
  {
    return 0;
  }

  world->bots[b].dead = 0;
  world->num_bots_alive--;
  return 0;
}

int bot_scramble(s_world *world, int b)
{
  ASSERT(world != NULL);
  
  if(b >= world->num_bots || b < 0)
  {
    return 0;
  }

  world->bots[b].red = RAND_BETWEEN(0.0, 1.0);
  world->bots[b].green = RAND_BETWEEN(0.0, 1.0);
  world->bots[b].blue = RAND_BETWEEN(0.0, 1.0);
  world->bots[b].r = RAND_BETWEEN(0.0, 360.0);
  return 0;
}

int bot_remove(s_world *world, int n)
{
  ASSERT(world != NULL);
  
  if(n >= world->num_bots || n < 0)
  {
    return 0;
  }

  bot_copy(&world->bots[n], &world->bots[world->num_bots-1]);
  //world->bots[n] = world->bots[world->num_bots-1];
  // world stats
  world->bots_removed++;
  world->num_bots--;
  return 0;
}

int bot_dump(s_bot *bot)
{
  ASSERT(bot != NULL);
  
  FILE* File = fopen("bot_dump.txt", "a");
  if(bot == NULL || File == NULL)
  {
    return 0;
  }

  // General
  fprintf(File, "energy: %i (%p)\n", bot->energy, (void*)&bot->energy);
  fprintf(File, "age: %i (%p)\n", bot->age, (void*)&bot->age);
  fprintf(File, "dead: %i (%p)\n", bot->dead, (void*)&bot->dead);
  fprintf(File, "x: %.4g (%p)\n", bot->x, (void*)&bot->x);
  fprintf(File, "y: %.4g (%p)\n", bot->y, (void*)&bot->y);
  fprintf(File, "r: %.4g (%p)\n", bot->r, (void*)&bot->r);
  fprintf(File, "size: %.4g (%p)\n", bot->size, (void*)&bot->size);
  fprintf(File, "turn_rate: %.4g (%p)\n", bot->turn_rate, (void*)&bot->turn_rate);
  fprintf(File, "red: %.4g (%p)\n", bot->red, (void*)&bot->red);
  fprintf(File, "green: %.4g (%p)\n", bot->green, (void*)&bot->green);
  fprintf(File, "blue: %.4g (%p)\n", bot->blue, (void*)&bot->blue);

  // Brain
  fprintf(File, "nn: (%p)\n", (void*)&bot->nn);
  fprintf(File, " nn.num_layers: %i (%p)\n",  bot->nn.num_layers, (void*)&bot->nn.num_layers);
  fprintf(File, " nn.num_inputs: %i (%p)\n",  bot->nn.num_inputs, (void*)&bot->nn.num_inputs);
  fprintf(File, " nn.num_outputs: %i (%p)\n", bot->nn.num_outputs, (void*)&bot->nn.num_outputs);
  fprintf(File, " nn.input: %p (%p)\n", bot->nn.input, (void*)&bot->nn.input);
  fprintf(File, " nn.output: %p (%p)\n", bot->nn.output, (void*)&bot->nn.output);
  fprintf(File, " nn.weights: %p (%p)\n", bot->nn.weights, (void*)&bot->nn.weights);

  // eyes
  int e;
  fprintf(File, "num_eyes: %i (%p)\n", bot->num_eyes, (void*)&bot->num_eyes);
  fprintf(File, "eyes: %p (%p)\n", bot->eyes, (void*)&bot->eyes);
  for(e = 0; e < bot->num_eyes; ++e)
  {
    fprintf(File, " Eye[%i].position: %.4g (%p)\n",      e, bot->eyes[e].position,      (void*)&bot->eyes[e].position);
    fprintf(File, " Eye[%i].view_distance: %.4g (%p)\n", e, bot->eyes[e].view_distance, (void*)&bot->eyes[e].view_distance);
    fprintf(File, " Eye[%i].view_angle: %.4g (%p)\n",    e, bot->eyes[e].view_angle,    (void*)&bot->eyes[e].view_angle);
    fprintf(File, " Eye[%i].in_strength: %.4g (%p)\n",   e, bot->eyes[e].in_strength,   (void*)&bot->eyes[e].in_strength);
    fprintf(File, " Eye[%i].in_red: %.4g (%p)\n",        e, bot->eyes[e].in_red,        (void*)&bot->eyes[e].in_red);
    fprintf(File, " Eye[%i].in_green: %.4g (%p)\n",      e, bot->eyes[e].in_green,      (void*)&bot->eyes[e].in_green);
    fprintf(File, " Eye[%i].in_blue: %.4g (%p)\n",       e, bot->eyes[e].in_blue,       (void*)&bot->eyes[e].in_blue);
  }

  // spikes
  fprintf(File, "spikes: (%p)\n", (void*)&bot->spikes);

  fprintf(File, "\n");
  fclose(File);
  return 0;
}

int bot_create(s_bot *bot, float x, float y)
{
  ASSERT(bot != NULL);

  bot->x = x;
  bot->y = y;
  bot->age = 0;
  bot->size = 1;
  bot->dead = 0;
  bot->red   = RAND_BETWEEN(0.0, 1.0);
  bot->green = RAND_BETWEEN(0.0, 1.0);
  bot->blue  = RAND_BETWEEN(0.0, 1.0);
  bot->energy = BOT_START_ENERGY * RAND_BETWEEN(0.8, 1.2);
  bot->r = RAND_BETWEEN(0.0, 360.0);
  bot->turn_rate = 3.0;
  // eyes
  bot->num_eyes = 3;
  bot->eyes = (s_eye*)malloc(bot->num_eyes*sizeof(s_eye));
  bot->eyes[0].position =  15;
  bot->eyes[1].position = 345;
  bot->eyes[2].position = 180;
  bot->eyes[0].view_angle = 30;
  bot->eyes[1].view_angle = 30;
  bot->eyes[2].view_angle = 15;
  bot->eyes[0].view_distance = 2.5;
  bot->eyes[1].view_distance = 2.5;
  bot->eyes[2].view_distance = 2.5;
  // spikes
  bot->num_spikes = 1;
  bot->spikes = (s_spike*)malloc(bot->num_spikes*sizeof(s_spike));
  bot->spikes[0].position = 0;
  bot->spikes[0].length = RAND_BETWEEN(0.0, 0.2);
  // nn
  bot->nn.num_layers = 3;
  bot->nn.layer_sizes = (int*)malloc(bot->nn.num_layers*sizeof(int));
  bot->nn.layer_sizes[0] = 13;
  bot->nn.layer_sizes[1] = 7;
  bot->nn.layer_sizes[2] = 3;

  nn_create(&bot->nn);
  nn_random_weights(&bot->nn, -1.0, 1.0);

  return 0;
}

int bot_add(s_world *world, float x, float y)
{
  ASSERT(world != NULL);
  
  if(world->num_bots >= world->max_bots)
  {
    return 0;
  }

  if(x < 0) {x = RAND_BETWEEN(0, world->width);}
  if(y < 0) {y = RAND_BETWEEN(0, world->height);}

  //printf("%.4g %.4g\n", x, y);
  //getchar();

  bot_create(&world->bots[world->num_bots], x, y);

  // world stats
  world->bots_added++;
  world->num_bots++;
  world->num_bots_alive++;

  //printf("add_bot() %.4g %.4g\n", world->bots[world->num_bots].x, world->bots[world->num_bots].y);

  return 0;
}
