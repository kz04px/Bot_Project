#include "defs.h"

int world_init(s_world *world)
{
  ASSERT(world != NULL);
  
  // Normal
  world->width = 96;
  world->height = 54;
  world->frame = 0;
  world->generation = 0;
  world->seed = time(0);
  world->selected = -1;
  srand(world->seed);
  // stats
  world->bots_added = 0;
  world->bots_removed = 0;
  world->bots_most = 0;
  world->pellets_added = 0;
  world->pellets_removed = 0;
  world->pellets_most = 0;
  world->average_fitness = 0;
  world->spike_uses = 0;
  // grid
  world->grid_width = world->width/10;
  world->grid_height = world->height/10;
  //grid = (grid*)malloc(grid_width*grid_height*sizeof(grid));
  // bots
  world->num_bots = 0;
  world->max_bots = 128;
  world->num_bots_alive = 0;
  world->num_parents = 4;
  world->bots = (s_bot *)malloc(world->max_bots*sizeof(s_bot));
  world->bot_parents = (s_bot *)malloc(world->num_parents*sizeof(s_bot));
  world->bot_ranks = (int*)malloc(world->max_bots*sizeof(int));
  // pellets
  world->num_pellets = 0;
  world->max_pellets = 512;
  world->pellets = (s_pellet*)malloc(world->max_pellets*sizeof(s_pellet));

  int p;
  for(p = 0; p < world->num_parents; ++p)
  {
    bot_create(&world->bot_parents[p], RAND_BETWEEN(0, world->width), RAND_BETWEEN(0, world->height));
  }

  rand();

  return 1;
}
