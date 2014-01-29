#include "defs.h"

int World_Init(world* Our_World)
{
  // Normal
  Our_World->Width = 96;
  Our_World->Height = 54;
  Our_World->Frame = 0;
  Our_World->Generation = 0;
  Our_World->Time_Seed = time(0);
  Our_World->Selected = -1;
  srand(Our_World->Time_Seed);
  // Stats
	Our_World->Bots_Added = 0;
	Our_World->Bots_Removed = 0;
	Our_World->Bots_Most = 0;
	Our_World->Pellets_Added = 0;
	Our_World->Pellets_Removed = 0;
	Our_World->Pellets_Most = 0;
  // Grid
  Our_World->Grid_Width = Our_World->Width/10;
  Our_World->Grid_Height = Our_World->Height/10;
  //Grid = (grid*) malloc(Grid_Width*Grid_Height*sizeof(grid));
  // Bots
  Our_World->Num_Bots = 0;
  Our_World->Max_Bots = 128;
  Our_World->Num_Bots_Alive = 0;
  Our_World->Num_Parents = 4;
  Our_World->Bots = (bot*) malloc(Our_World->Max_Bots*sizeof(bot));
  Our_World->Bot_Parents = (bot*) malloc(Our_World->Num_Parents*sizeof(bot));
  Our_World->Bot_Ranks = (int*) malloc(Our_World->Max_Bots*sizeof(int));
  // Pellets
  Our_World->Num_Pellets = 0;
  Our_World->Max_Pellets = 512;
  Our_World->Pellets = (pellet*) malloc(Our_World->Max_Pellets*sizeof(pellet));

  int p;
  for(p = 0; p < Our_World->Num_Parents; ++p)
  {
    Bot_Create(&Our_World->Bot_Parents[p], RAND_BETWEEN(0, Our_World->Width), RAND_BETWEEN(0, Our_World->Height));
  }

  rand();

  return TRUE;
}
