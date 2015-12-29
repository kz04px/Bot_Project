#include "defs.h"

int world_init(world* our_world)
{
  // Normal
  our_world->width = 96;
  our_world->height = 54;
  our_world->frame = 0;
  our_world->generation = 0;
  our_world->seed = time(0);
  our_world->selected = -1;
  srand(our_world->seed);
  // stats
  our_world->bots_added = 0;
  our_world->bots_removed = 0;
  our_world->bots_most = 0;
  our_world->pellets_added = 0;
  our_world->pellets_removed = 0;
  our_world->pellets_most = 0;
  // grid
  our_world->grid_width = our_world->width/10;
  our_world->grid_height = our_world->height/10;
  //grid = (grid*) malloc(grid_width*grid_height*sizeof(grid));
  // bots
  our_world->num_bots = 0;
  our_world->max_bots = 128;
  our_world->num_bots_alive = 0;
  our_world->num_parents = 4;
  our_world->bots = (bot*) malloc(our_world->max_bots*sizeof(bot));
  our_world->bot_parents = (bot*) malloc(our_world->num_parents*sizeof(bot));
  our_world->bot_ranks = (int*) malloc(our_world->max_bots*sizeof(int));
  // pellets
  our_world->num_pellets = 0;
  our_world->max_pellets = 512;
  our_world->pellets = (pellet*) malloc(our_world->max_pellets*sizeof(pellet));

  int p;
  for(p = 0; p < our_world->num_parents; ++p)
  {
    bot_create(&our_world->bot_parents[p], RAND_BETWEEN(0, our_world->width), RAND_BETWEEN(0, our_world->height));
  }

  rand();

  return 1;
}
