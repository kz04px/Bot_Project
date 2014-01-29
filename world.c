#include "defs.h"

int World_Init(world* Our_World)
{
  // Normal
  Our_World->Width = 96; // 30
  Our_World->Height = 54; // 20
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

  // Create parents FIXME: This is awful
  int p;
  for(p = 0; p < Our_World->Num_Parents; ++p)
  {
    Bot_Create(&Our_World->Bot_Parents[p], RAND_BETWEEN(0, Our_World->Width), RAND_BETWEEN(0, Our_World->Height));
    /*
    Our_World->Bot_Parents[p].Eyes = (eye*) malloc(3*sizeof(eye));
    Our_World->Bot_Parents[p].Spikes = (spike*) malloc(1*sizeof(spike));
    Our_World->Bot_Parents[p].NN.Layer_Sizes = (int*) malloc(3*sizeof(int));
    Our_World->Bot_Parents[p].NN.Layer_Sizes[0] = 12;
    Our_World->Bot_Parents[p].NN.Layer_Sizes[1] = 7;
    Our_World->Bot_Parents[p].NN.Layer_Sizes[2] = 3;
    printf("Before: %i\n", Our_World->Bot_Parents[p].NN);
    NN_Create(&Our_World->Bot_Parents[p].NN);
    printf("After:  %i\n", Our_World->Bot_Parents[p].NN);
    */
  }

  rand();

  return TRUE;
}
