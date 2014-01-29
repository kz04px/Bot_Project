#include "defs.h"

float Angle_Difference(float x, float y)
{
  float Dif = x-y;
  NORMALISE_DEG(Dif);
  if(Dif > 180)
    return (360-Dif);
  else
    return Dif;
}

int Simulation_End(world* Our_World)
{
 	Simulation.World = NULL;
 	Simulation.Pause = TRUE;
 	Simulation.Quit = TRUE;
 	Simulation.Delay = 20;
 	Simulation.Logging = FALSE;
 	fclose(Simulation.Log_File);
  return TRUE;
}

int Simulation_Init(world* Our_World)
{
 	Simulation.World = Our_World;
 	Simulation.Pause = TRUE;
 	Simulation.Quit = FALSE;
 	Simulation.Delay = 20;
 	Simulation.Logging = TRUE;
 	Simulation.Log_File = fopen("Log.txt", "w");
  return TRUE;
}

void Update_Bots_Positions(world* Our_World)
{
  int b;
  for(b = 0; b < Our_World->Num_Bots; ++b)
  {
    if(Our_World->Bots[b].Dead == TRUE)
      continue;

    // Update position & rotation (NN)
    Our_World->Bots[b].x += 0.1 * Our_World->Bots[b].NN.Output[Our_World->Bots[b].NN.Num_Layers-1][0] * sin(DEG_TO_RAD(Our_World->Bots[b].r));
    Our_World->Bots[b].y += 0.1 * Our_World->Bots[b].NN.Output[Our_World->Bots[b].NN.Num_Layers-1][0] * cos(DEG_TO_RAD(Our_World->Bots[b].r));
    Our_World->Bots[b].r += Our_World->Bots[b].Turn_Rate*(Our_World->Bots[b].NN.Output[Our_World->Bots[b].NN.Num_Layers-1][1]-0.5);

    // Position & rotation checks
    if(Simulation.World->Bots[b].x < 0)
      Simulation.World->Bots[b].x += Simulation.World->Width;
    else if(Simulation.World->Bots[b].x > Simulation.World->Width)
      Simulation.World->Bots[b].x -= Simulation.World->Width;

    if(Simulation.World->Bots[b].y < 0)
      Simulation.World->Bots[b].y += Simulation.World->Height;
    else if(Simulation.World->Bots[b].y > Simulation.World->Height)
      Simulation.World->Bots[b].y -= Simulation.World->Height;

    if(Simulation.World->Bots[b].r < 0)
      Simulation.World->Bots[b].r += 360;
    else if(Simulation.World->Bots[b].r > 360)
      Simulation.World->Bots[b].r -= 360;
  }
}

void Update_Bots_Inputs(world* Our_World)
{
  int b, b2, e, p;
  for(b = 0; b < Simulation.World->Num_Bots; ++b)
  {
    if(Our_World->Bots[b].Dead == TRUE)
      continue;

    // Resets
    for(e = 0; e < Simulation.World->Bots[b].Num_Eyes; ++e)
    {
      Simulation.World->Bots[b].Eyes[e].In_Strength = 0.0; //Simulation.World->Bots[b].Eyes[e].View_Distance;
      Simulation.World->Bots[b].Eyes[e].In_Red = 0.0;
      Simulation.World->Bots[b].Eyes[e].In_Green = 0.0;
      Simulation.World->Bots[b].Eyes[e].In_Blue = 0.0;
    }

    // Look for pellets
    for(p = 0; p < Our_World->Num_Pellets; ++p)
    {
      float dx = Our_World->Pellets[p].x - Our_World->Bots[b].x;
      float dy = Our_World->Pellets[p].y - Our_World->Bots[b].y;
      float Dist = sqrt(dx*dx + dy*dy);
      float Angle = 0;
      if(dy == 0)
      {
        if(dx >= 0) {Angle =  90 - Our_World->Bots[b].r;}
        if(dx <  0) {Angle = 270 - Our_World->Bots[b].r;}
      }
      else
      {
        Angle = RAD_TO_DEG(atan(dx/dy)) - Our_World->Bots[b].r;
        if(dy < 0) Angle += 180;
      }
      NORMALISE_DEG(Angle);

      // Eyes
      for(e = 0; e < Our_World->Bots[b].Num_Eyes; ++e)
      {
        if(Angle_Difference(Angle, Our_World->Bots[b].Eyes[e].Position) <= Our_World->Bots[b].Eyes[e].View_Angle &&
          (Our_World->Bots[b].Eyes[e].View_Distance-Dist)/Our_World->Bots[b].Eyes[e].View_Distance > Our_World->Bots[b].Eyes[e].In_Strength)
          //Dist < Our_World->Bots[b].Eyes[e].In_Strength)
        {
        //Our_World->Bots[b].Eyes[e].In_Strength = Dist;
        Our_World->Bots[b].Eyes[e].In_Strength = (Our_World->Bots[b].Eyes[e].View_Distance-Dist)/Our_World->Bots[b].Eyes[e].View_Distance;
        Our_World->Bots[b].Eyes[e].In_Red   = Our_World->Pellets[p].Red;
        Our_World->Bots[b].Eyes[e].In_Green = Our_World->Pellets[p].Green;
        Our_World->Bots[b].Eyes[e].In_Blue  = Our_World->Pellets[p].Blue;
        }
      }
    }

    // Look for bots
    /*
    for(b2 = 0; b2 < Our_World->Num_Bots; ++b2)
    {
      if(b2 == b) continue; // We can't see ourself

      float dx = Our_World->Bots[b2].x - Our_World->Bots[b].x;
      float dy = Our_World->Bots[b2].y - Our_World->Bots[b].y;
      float Dist = sqrt(dx*dx + dy*dy);
      float Angle = 0;
      if(dy == 0)
      {
        if(dx >= 0) {Angle =  90 - Our_World->Bots[b].r;}
        if(dx <  0) {Angle = 270 - Our_World->Bots[b].r;}
      }
      else
      {
        Angle = RAD_TO_DEG(atan(dx/dy)) - Our_World->Bots[b].r;
        if(dy < 0) Angle += 180;
      }
      NORMALISE_DEG(Angle);

      // Eyes
      for(e = 0; e < Our_World->Bots[b].Num_Eyes; ++e)
      {
        if(Angle_Difference(Angle, Our_World->Bots[b].Eyes[e].Position) <= Our_World->Bots[b].Eyes[e].View_Angle &&
          Dist < Our_World->Bots[b].Eyes[e].In_Strength)
        {
        Our_World->Bots[b].Eyes[e].In_Strength = Dist;
        Our_World->Bots[b].Eyes[e].In_Red   = Our_World->Bots[b2].Red;
        Our_World->Bots[b].Eyes[e].In_Green = Our_World->Bots[b2].Green;
        Our_World->Bots[b].Eyes[e].In_Blue  = Our_World->Bots[b2].Blue;
        }
      }
    }
    */
  }
}

void Update_Pellets(world* Our_World)
{
  int p, b;
  for(p = 0; p < Our_World->Num_Pellets; ++p)
  {
    for(b = 0; b < Our_World->Num_Bots; ++b)
    {
      if(DIST(Our_World->Pellets[p].x, Our_World->Pellets[p].y,
              Our_World->Bots[b].x, Our_World->Bots[b].y) < 0.15)
      {
        Our_World->Bots[b].Energy += Our_World->Pellets[p].Nutrition;

        Pellet_Remove(Our_World, p);
        Pellet_Add(Our_World, -1, -1);
        p--;
        break;
      }
    }
  }
}

void Simulate_World(void* a)
{
  while(Simulation.Quit == FALSE)
  {
    if(Simulation.Pause == TRUE)
    {
      Update_Bots_Inputs(Simulation.World);
      Sleep(100);
      continue;
    }

    Update_Bots_Inputs(Simulation.World);
    Update_Bots_NNs(Simulation.World);
    Update_Bots_Positions(Simulation.World);
    Update_Pellets(Simulation.World);

    int b;
    // Energy
    for(b = 0; b < Simulation.World->Num_Bots; ++b)
    {
      if(Simulation.World->Bots[b].Dead == TRUE)
        continue;

      if(Simulation.World->Bots[b].Energy <= 0)
        Bot_Kill(Simulation.World, b);
      else
        Simulation.World->Bots[b].Energy--;
    }

    // Age
    for(b = 0; b < Simulation.World->Num_Bots; ++b)
    {
      if(Simulation.World->Bots[b].Dead == TRUE)
        continue;

      Simulation.World->Bots[b].Age++;
    }

    // Next generation check
    if(Simulation.World->Num_Bots_Alive <= Simulation.World->Num_Parents ||
       Simulation.World->Frame >= 2000)
    {
      // Rank the bots
      Bot_Ranks(Simulation.World);
      printf("Best: ");
      for(b = 0; b < Simulation.World->Num_Bots; ++b)
        printf("%i ", Simulation.World->Bot_Ranks[b]);
      printf("\n");

      // Breed new generation
      Bots_Breed_New_Generation(Simulation.World);
      Simulation.World->Num_Bots_Alive = Simulation.World->Num_Bots;

      // Logging
      fprintf(Simulation.Log_File, "%i\t %i\t\n", Simulation.World->Generation, Simulation.World->Pellets_Removed);

      // Reset logging
      Simulation.World->Pellets_Removed = 0;

      // Next generation
      Simulation.World->Generation++;
      Simulation.World->Frame = 0;
    }
    else
    {
      Simulation.World->Frame++;
    }

    Sleep(Simulation.Delay);
  }
}
