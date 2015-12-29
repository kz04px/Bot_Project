#include "defs.h"

#define EYE_ACCURACY 15

void draw_bot(bot *our_bot)
{
  int e; // Current eye
  int s; // Current spike

  // draw vision triangles
  float Accuracy_Angle;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for(e = 0; e < our_bot->num_eyes; ++e)
  {
    Accuracy_Angle = (float)our_bot->eyes[e].view_angle*2/EYE_ACCURACY;
    glPushMatrix();
    glColor3f(our_bot->eyes[e].in_red, our_bot->eyes[e].in_green, our_bot->eyes[e].in_blue);

    glRotatef(our_bot->r+our_bot->eyes[e].position-our_bot->eyes[e].view_angle, 0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    int i;
    for(i = 0; i <= EYE_ACCURACY; ++i)
    {
      glVertex2f((float)our_bot->eyes[e].view_distance*sin(DEG_TO_RAD(Accuracy_Angle*i)),
                 (float)our_bot->eyes[e].view_distance*cos(DEG_TO_RAD(Accuracy_Angle*i)));
    }
    glEnd();
    glPopMatrix();
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // draw spikes
  for(s = 0; s < our_bot->num_spikes; ++s)
  {
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glRotatef(our_bot->r, 0.0, 0.0, -1.0);
    glBegin(GL_TRIANGLES);
      glVertex2f( 0.00, 0.20 + our_bot->spikes[s].length);
      glVertex2f( 0.05, 0.15);
      glVertex2f(-0.05, 0.15);
    glEnd();
    glPopMatrix();
  }

  // draw body
  glPushMatrix();
  glColor3f(our_bot->red, our_bot->green, our_bot->blue); // red
  glRotatef(our_bot->r, 0,0,-1);
  glBegin(GL_POLYGON);
    glVertex2f( 0.00,  0.20);
    glVertex2f( 0.19,  0.06);
    glVertex2f( 0.12, -0.16);
    glVertex2f(-0.12, -0.16);
    glVertex2f(-0.19,  0.06);
  glEnd();
  glPopMatrix();

  // draw eyes
  for(e = 0; e < our_bot->num_eyes; ++e)
  {
    glPushMatrix();
    glColor3f(our_bot->eyes[e].in_red, our_bot->eyes[e].in_green, our_bot->eyes[e].in_blue);
    glRotatef(our_bot->r, 0.0, 0.0, -1.0);
    float yPos = 0.25 * cos(DEG_TO_RAD(our_bot->eyes[e].position)) - 0.05;
    float xPos = 0.25 * sin(DEG_TO_RAD(our_bot->eyes[e].position)) - 0.05;
    glBegin(GL_QUADS);
      glVertex2f(xPos    , yPos    );
      glVertex2f(xPos    , yPos+0.1);
      glVertex2f(xPos+0.1, yPos+0.1);
      glVertex2f(xPos+0.1, yPos    );
    glEnd();
    glPopMatrix();
  }
}

void draw_edge_bots(world *our_world)
{
  int b;
  for(b = 0; b < our_world->num_bots; ++b)
  {
    // -w
    glPushMatrix();
    glTranslatef(our_world->bots[b].x-our_world->width, our_world->bots[b].y, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
    // +w
    glPushMatrix();
    glTranslatef(our_world->bots[b].x+our_world->width, our_world->bots[b].y, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
    // -h
    glPushMatrix();
    glTranslatef(our_world->bots[b].x, our_world->bots[b].y-our_world->height, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
    // +h
    glPushMatrix();
    glTranslatef(our_world->bots[b].x, our_world->bots[b].y+our_world->height, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
    // -w -h
    glPushMatrix();
    glTranslatef(our_world->bots[b].x-our_world->width, our_world->bots[b].y-our_world->height, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
    // -w +h
    glPushMatrix();
    glTranslatef(our_world->bots[b].x-our_world->width, our_world->bots[b].y+our_world->height, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
    // +w -h
    glPushMatrix();
    glTranslatef(our_world->bots[b].x+our_world->width, our_world->bots[b].y-our_world->height, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
    // +w +h
    glPushMatrix();
    glTranslatef(our_world->bots[b].x+our_world->width, our_world->bots[b].y+our_world->height, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
  }
}

void draw_bots(world *our_world)
{
  int b;
  for(b = 0; b < our_world->num_bots; ++b)
  {
    glPushMatrix();
    glTranslatef(our_world->bots[b].x, our_world->bots[b].y, 0.0);
    draw_bot(&our_world->bots[b]);
    glPopMatrix();
  }
}

int bot_find_closest(world* our_world, float x, float y)
{
  int closest_bot = -1;
  float max_Range = 0.5;
  float closest_Range = 999999;

  int b;
  for(b = 0; b < our_world->num_bots; ++b)
  {
    if(DIST(our_world->bots[b].x, our_world->bots[b].y, x, y) < closest_Range)
    {
      closest_Range = DIST(our_world->bots[b].x, our_world->bots[b].y, x, y);
      closest_bot = b;
    }
  }

  if(closest_Range < max_Range)
  {
    printf("bot selected - bot: %i, Range: %.4g\n", closest_bot, closest_Range);
    return closest_bot;
  }
  else
  {
    printf("No bot selected - closest: %i, %.4g\n", closest_bot, closest_Range);
    return -1;
  }
}

int bot_copy(bot* dest, bot* source)
{
  if(dest == NULL || source == NULL)
    return 0;

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

int bot_Refresh(bot* our_bot)
{
  our_bot->energy = BOT_START_ENERGY;
  our_bot->age = 0;
  our_bot->dead = 0;
  return 0;
}

int bot_breed(bot* Child, bot* parent1, bot* parent2)
{
  if(Child == NULL || parent1 == NULL || parent2 == NULL)
    return 0;

  // Basic version
  if(rand()%2 == 0)
    bot_copy(Child, parent1);
  else
    bot_copy(Child, parent2);

  // Crossover version
  /*
  bot_copy(Child, parent1);
  if(rand()%2 == 0)
    Child->size = parent2->size;
  if(rand()%2 == 0)
    Child->turn_rate = parent2->turn_rate;
  */

  return 0;
}

int bot_mutate(bot* our_bot)
{
  our_bot->red += RAND_BETWEEN(-0.1, 0.1);
  our_bot->green += RAND_BETWEEN(-0.1, 0.1);
  our_bot->blue += RAND_BETWEEN(-0.1, 0.1);
  our_bot->turn_rate += RAND_BETWEEN(-0.1, 0.1);

  // Neural network
  nn_mutate(&our_bot->nn);

  // eyes
  int e;
  for(e = 0; e < our_bot->num_eyes; ++e)
  {
    //our_bot->eyes[e].position += RAND_BETWEEN(-1.0, 1.0);
    //our_bot->eyes[e].view_angle += RAND_BETWEEN(-1.0, 1.0);
    //our_bot->eyes[e].view_distance += RAND_BETWEEN(-0.1, 0.1);
  }

  // spikes
  int s;
  for(s = 0; e < our_bot->num_eyes; ++s)
  {
    //our_bot->spikes[s].position += RAND_BETWEEN(-0.1, 0.1);
    //our_bot->spikes[s].length += RAND_BETWEEN(-0.1, 0.1);
  }

  return 0;
}

void bots_breed_new_generation(world* our_world)
{
  // create parent bots
  int p;
  for(p = 0; p < our_world->num_parents; ++p)
  {
    bot_copy(&our_world->bot_parents[p], &our_world->bots[our_world->bot_ranks[p]]);
  }

  // breed new generation from parents
  int b;
  for(b = 0; b < our_world->num_bots; ++b)
  {
    bot_breed(&our_world->bots[b],
              &our_world->bot_parents[rand()%our_world->num_parents],
              &our_world->bot_parents[rand()%our_world->num_parents]);

    // Refresh bot
    our_world->bots[b].x = RAND_BETWEEN(0, our_world->width);
    our_world->bots[b].y = RAND_BETWEEN(0, our_world->height);
    our_world->bots[b].energy = BOT_START_ENERGY;
    our_world->bots[b].age = 0;
    our_world->bots[b].dead = 0;

    // mutate bot
    bot_mutate(&our_world->bots[b]);
  }
}

void bot_ranks(world* our_world)
{
  #ifndef BOT_SCORE
  #define BOT_SCORE (our_world->bots[b].age + our_world->bots[b].energy)
  #endif

  int n;
  //for(n = our_world->num_bots-1; n >= 0; --n)
  for(n = 0; n < our_world->num_bots; ++n)
  {
    int Best_bot_Score = -1;
    int Best_bot = -1;

    int b;
    for(b = 0; b < our_world->num_bots; ++b)
    {
      if(BOT_SCORE > Best_bot_Score)
      {
        Best_bot_Score = BOT_SCORE;
        Best_bot = b;
      }
    }

    our_world->bot_ranks[n] = Best_bot;
    our_world->bots[Best_bot].age = -1;
    our_world->bots[Best_bot].energy = -1;
  }

  #undef BOT_SCORE
}

int bot_kill(world* our_world, int b)
{
  if(b >= our_world->num_bots || b < 0 || our_world->bots[b].dead)
    return 0;

  our_world->bots[b].dead = 0;
  our_world->num_bots_alive--;
  return 0;
}

int bot_scramble(world* our_world, int b)
{
  if(b >= our_world->num_bots || b < 0)
    return 0;

  our_world->bots[b].red = RAND_BETWEEN(0.0, 1.0);
  our_world->bots[b].green = RAND_BETWEEN(0.0, 1.0);
  our_world->bots[b].blue = RAND_BETWEEN(0.0, 1.0);
  our_world->bots[b].r = RAND_BETWEEN(0.0, 360.0);
  return 0;
}

int bot_remove(world* our_world, int n)
{
  if(n >= our_world->num_bots || n < 0)
    return 0;

  bot_copy(&our_world->bots[n], &our_world->bots[our_world->num_bots-1]);
  //our_world->bots[n] = our_world->bots[our_world->num_bots-1];
  // world stats
  our_world->bots_removed++;
  our_world->num_bots--;
  return 0;
}

int bot_dump(bot* our_bot)
{
  FILE* our_File = fopen("bot_dump.txt", "a");
  if(our_bot == NULL || our_File == NULL)
    return 0;

  // General
  fprintf(our_File, "energy: %i (%p)\n", our_bot->energy, (void*)&our_bot->energy);
  fprintf(our_File, "age: %i (%p)\n", our_bot->age, (void*)&our_bot->age);
  fprintf(our_File, "dead: %i (%p)\n", our_bot->dead, (void*)&our_bot->dead);
  fprintf(our_File, "x: %.4g (%p)\n", our_bot->x, (void*)&our_bot->x);
  fprintf(our_File, "y: %.4g (%p)\n", our_bot->y, (void*)&our_bot->y);
  fprintf(our_File, "r: %.4g (%p)\n", our_bot->r, (void*)&our_bot->r);
  fprintf(our_File, "size: %.4g (%p)\n", our_bot->size, (void*)&our_bot->size);
  fprintf(our_File, "turn_rate: %.4g (%p)\n", our_bot->turn_rate, (void*)&our_bot->turn_rate);
  fprintf(our_File, "red: %.4g (%p)\n", our_bot->red, (void*)&our_bot->red);
  fprintf(our_File, "green: %.4g (%p)\n", our_bot->green, (void*)&our_bot->green);
  fprintf(our_File, "blue: %.4g (%p)\n", our_bot->blue, (void*)&our_bot->blue);

  // Brain
  fprintf(our_File, "nn: (%p)\n", (void*)&our_bot->nn);
  fprintf(our_File, " nn.num_layers: %i (%p)\n",  our_bot->nn.num_layers, (void*)&our_bot->nn.num_layers);
  fprintf(our_File, " nn.num_inputs: %i (%p)\n",  our_bot->nn.num_inputs, (void*)&our_bot->nn.num_inputs);
  fprintf(our_File, " nn.num_outputs: %i (%p)\n", our_bot->nn.num_outputs, (void*)&our_bot->nn.num_outputs);
  fprintf(our_File, " nn.input: %p (%p)\n", our_bot->nn.input, (void*)&our_bot->nn.input);
  fprintf(our_File, " nn.output: %p (%p)\n", our_bot->nn.output, (void*)&our_bot->nn.output);
  fprintf(our_File, " nn.weights: %p (%p)\n", our_bot->nn.weights, (void*)&our_bot->nn.weights);

  // eyes
  int e;
  fprintf(our_File, "num_eyes: %i (%p)\n", our_bot->num_eyes, (void*)&our_bot->num_eyes);
  fprintf(our_File, "eyes: %p (%p)\n", our_bot->eyes, (void*)&our_bot->eyes);
  for(e = 0; e < our_bot->num_eyes; ++e)
  {
    fprintf(our_File, " Eye[%i].position: %.4g (%p)\n",      e, our_bot->eyes[e].position,      (void*)&our_bot->eyes[e].position);
    fprintf(our_File, " Eye[%i].view_distance: %.4g (%p)\n", e, our_bot->eyes[e].view_distance, (void*)&our_bot->eyes[e].view_distance);
    fprintf(our_File, " Eye[%i].view_angle: %.4g (%p)\n",    e, our_bot->eyes[e].view_angle,    (void*)&our_bot->eyes[e].view_angle);
    fprintf(our_File, " Eye[%i].in_strength: %.4g (%p)\n",   e, our_bot->eyes[e].in_strength,   (void*)&our_bot->eyes[e].in_strength);
    fprintf(our_File, " Eye[%i].in_red: %.4g (%p)\n",        e, our_bot->eyes[e].in_red,        (void*)&our_bot->eyes[e].in_red);
    fprintf(our_File, " Eye[%i].in_green: %.4g (%p)\n",      e, our_bot->eyes[e].in_green,      (void*)&our_bot->eyes[e].in_green);
    fprintf(our_File, " Eye[%i].in_blue: %.4g (%p)\n",       e, our_bot->eyes[e].in_blue,       (void*)&our_bot->eyes[e].in_blue);
  }

  // spikes
  fprintf(our_File, "spikes: (%p)\n", (void*)&our_bot->spikes);

  fprintf(our_File, "\n");
  fclose(our_File);
  return 0;
}

int bot_create(bot* our_bot, float x, float y)
{
  if(our_bot == NULL)
    return 0;

  our_bot->x = x;
  our_bot->y = y;
  our_bot->age = 0;
  our_bot->size = 1;
  our_bot->dead = 0;
  our_bot->red   = RAND_BETWEEN(0.0, 1.0);
  our_bot->green = RAND_BETWEEN(0.0, 1.0);
  our_bot->blue  = RAND_BETWEEN(0.0, 1.0);
  our_bot->energy = BOT_START_ENERGY;
  our_bot->r = RAND_BETWEEN(0.0, 360.0);
  our_bot->turn_rate = 3.0;
  // eyes
  our_bot->num_eyes = 3;
  our_bot->eyes = (eye*) malloc(our_bot->num_eyes*sizeof(eye));
  our_bot->eyes[0].position =  15;
  our_bot->eyes[1].position = 345;
  our_bot->eyes[2].position = 180;
  our_bot->eyes[0].view_angle = 30;
  our_bot->eyes[1].view_angle = 30;
  our_bot->eyes[2].view_angle = 15;
  our_bot->eyes[0].view_distance = 2.5;
  our_bot->eyes[1].view_distance = 2.5;
  our_bot->eyes[2].view_distance = 2.5;
  // spikes
  our_bot->num_spikes = 1;
  our_bot->spikes = (spike*) malloc(our_bot->num_spikes*sizeof(spike));
  our_bot->spikes[0].position = 0;
  our_bot->spikes[0].length = RAND_BETWEEN(0.0, 0.2);
  // nn
  our_bot->nn.num_layers = 3;
  our_bot->nn.layer_sizes = (int*) malloc(3*sizeof(int));
  our_bot->nn.layer_sizes[0] = 13;
  our_bot->nn.layer_sizes[1] = 7;
  our_bot->nn.layer_sizes[2] = 3;

  nn_create(&our_bot->nn);
  nn_random_weights(&our_bot->nn, -1.0, 1.0);

  return 0;
}

int bot_add(world* our_world, float x, float y)
{
  if(our_world->num_bots >= our_world->max_bots)
    return 0;

  if(x < 0) x = RAND_BETWEEN(0, our_world->width);
  if(y < 0) y = RAND_BETWEEN(0, our_world->height);

  //printf("%.4g %.4g\n", x, y);
  //getchar();

  bot_create(&our_world->bots[our_world->num_bots], x, y);

  // world stats
  our_world->bots_added++;
  our_world->num_bots++;
  our_world->num_bots_alive++;

  //printf("add_bot() %.4g %.4g\n", our_world->bots[our_world->num_bots].x, our_world->bots[our_world->num_bots].y);

  return 0;
}
