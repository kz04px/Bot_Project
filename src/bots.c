#include "defs.h"

#define EYE_ACCURACY 15

void Draw_Bot(bot *Our_Bot)
{
  int e; // Current eye
  int s; // Current spike

  // Draw vision triangles
  float Accuracy_Angle;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for(e = 0; e < Our_Bot->Num_Eyes; ++e)
  {
    Accuracy_Angle = (float)Our_Bot->Eyes[e].View_Angle*2/EYE_ACCURACY;
    glPushMatrix();
    glColor3f(Our_Bot->Eyes[e].In_Red, Our_Bot->Eyes[e].In_Green, Our_Bot->Eyes[e].In_Blue);

    glRotatef(Our_Bot->r+Our_Bot->Eyes[e].Position-Our_Bot->Eyes[e].View_Angle, 0.0, 0.0, -1.0);
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    int i;
    for(i = 0; i <= EYE_ACCURACY; ++i)
      {
      glVertex2f((float)Our_Bot->Eyes[e].View_Distance*sin(DEG_TO_RAD(Accuracy_Angle*i)),
                 (float)Our_Bot->Eyes[e].View_Distance*cos(DEG_TO_RAD(Accuracy_Angle*i)));
      }
    glEnd();
    glPopMatrix();
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Draw spikes
  for(s = 0; s < Our_Bot->Num_Spikes; ++s)
  {
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glRotatef(Our_Bot->r, 0.0, 0.0, -1.0);
    glBegin(GL_TRIANGLES);
      glVertex2f( 0.00, 0.20 + Our_Bot->Spikes[s].Length);
      glVertex2f( 0.05, 0.15);
      glVertex2f(-0.05, 0.15);
    glEnd();
    glPopMatrix();
  }

  // Draw body
  glPushMatrix();
  glColor3f(Our_Bot->Red, Our_Bot->Green, Our_Bot->Blue); // Red
  glRotatef(Our_Bot->r, 0,0,-1);
  glBegin(GL_POLYGON);
    glVertex2f( 0.00,  0.20);
    glVertex2f( 0.19,  0.06);
    glVertex2f( 0.12, -0.16);
    glVertex2f(-0.12, -0.16);
    glVertex2f(-0.19,  0.06);
  glEnd();
  glPopMatrix();

  // Draw eyes
  for(e = 0; e < Our_Bot->Num_Eyes; ++e)
  {
    glPushMatrix();
    glColor3f(Our_Bot->Eyes[e].In_Red, Our_Bot->Eyes[e].In_Green, Our_Bot->Eyes[e].In_Blue);
    glRotatef(Our_Bot->r, 0.0, 0.0, -1.0);
    float yPos = 0.25 * cos(DEG_TO_RAD(Our_Bot->Eyes[e].Position)) - 0.05;
    float xPos = 0.25 * sin(DEG_TO_RAD(Our_Bot->Eyes[e].Position)) - 0.05;
    glBegin(GL_QUADS);
      glVertex2f(xPos    , yPos    );
      glVertex2f(xPos    , yPos+0.1);
      glVertex2f(xPos+0.1, yPos+0.1);
      glVertex2f(xPos+0.1, yPos    );
    glEnd();
    glPopMatrix();
  }
}

void Draw_Edge_Bots(world *Our_World)
{
	int b;
	for(b = 0; b < Our_World->Num_Bots; ++b)
	{
	  // -w
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x-Our_World->Width, Our_World->Bots[b].y, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
    // +w
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x+Our_World->Width, Our_World->Bots[b].y, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
    // -h
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x, Our_World->Bots[b].y-Our_World->Height, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
    // +h
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x, Our_World->Bots[b].y+Our_World->Height, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
	  // -w -h
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x-Our_World->Width, Our_World->Bots[b].y-Our_World->Height, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
	  // -w +h
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x-Our_World->Width, Our_World->Bots[b].y+Our_World->Height, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
	  // +w -h
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x+Our_World->Width, Our_World->Bots[b].y-Our_World->Height, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
	  // +w +h
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x+Our_World->Width, Our_World->Bots[b].y+Our_World->Height, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
	}
}

void Draw_Bots(world *Our_World)
{
	int b;
	for(b = 0; b < Our_World->Num_Bots; ++b)
	{
	  glPushMatrix();
    glTranslatef(Our_World->Bots[b].x, Our_World->Bots[b].y, 0.0);
	  Draw_Bot(&Our_World->Bots[b]);
    glPopMatrix();
	}
}

int Bot_Find_Closest(world* Our_World, float x, float y)
{
  int Closest_Bot = -1;
  float Max_Range = 0.5;
  float Closest_Range = 999999;

  int b;
  for(b = 0; b < Our_World->Num_Bots; ++b)
  {
    if(DIST(Our_World->Bots[b].x, Our_World->Bots[b].y, x, y) < Closest_Range)
    {
      Closest_Range = DIST(Our_World->Bots[b].x, Our_World->Bots[b].y, x, y);
      Closest_Bot = b;
    }
  }

  if(Closest_Range < Max_Range)
  {
    printf("Bot selected - Bot: %i, Range: %.4g\n", Closest_Bot, Closest_Range);
    return Closest_Bot;
  }
  else
  {
    printf("No bot selected - Closest: %i, %.4g\n", Closest_Bot, Closest_Range);
    return -1;
  }
}

int Bot_Copy(bot* Dest, bot* Source)
{
  if(Dest == NULL || Source == NULL)
    return FALSE;

  // Manual copy
  Dest->Energy = Source->Energy;
  Dest->Age = Source->Age;
  Dest->Dead = Source->Dead;
  Dest->x = Source->x;
  Dest->y = Source->y;
  Dest->r = Source->r;
  Dest->Size = Source->Size;
  Dest->Turn_Rate = Source->Turn_Rate;
  Dest->Red = Source->Red;
  Dest->Green = Source->Green;
  Dest->Blue = Source->Blue;
  Dest->Num_Eyes = Source->Num_Eyes;
  Dest->Num_Spikes = Source->Num_Spikes;

  // Pointer stuff (Eyes)
  int e;
  for(e = 0; e < Source->Num_Eyes; ++e)
  {
    Dest->Eyes[e].Position      = Source->Eyes[e].Position;
    Dest->Eyes[e].View_Distance = Source->Eyes[e].View_Distance;
    Dest->Eyes[e].View_Angle    = Source->Eyes[e].View_Angle;
    Dest->Eyes[e].Red           = Source->Eyes[e].Red;
    Dest->Eyes[e].Green         = Source->Eyes[e].Green;
    Dest->Eyes[e].Blue          = Source->Eyes[e].Blue;
    Dest->Eyes[e].In_Strength   = Source->Eyes[e].In_Strength;
    Dest->Eyes[e].In_Red        = Source->Eyes[e].In_Red;
    Dest->Eyes[e].In_Green      = Source->Eyes[e].In_Green;
    Dest->Eyes[e].In_Blue       = Source->Eyes[e].In_Blue;
  }

  // Pointer stuff (Spikes)
  int s;
  for(s = 0; s < Dest->Num_Spikes; ++s)
  {
    Dest->Spikes[s].Retracted = Source->Spikes[s].Retracted;
    Dest->Spikes[s].Position  = Source->Spikes[s].Position;
    Dest->Spikes[s].Length    = Source->Spikes[s].Length;
  }

  // Pointer stuff (Neural network)
  Dest->NN.Num_Layers = Source->NN.Num_Layers;
  Dest->NN.Num_Inputs = Source->NN.Num_Inputs;
  Dest->NN.Num_Outputs = Source->NN.Num_Outputs;
  Dest->NN.Layer_Sizes = Source->NN.Layer_Sizes;
  int i, j, k;
  for(i = 1; i < Dest->NN.Num_Layers; ++i)
  {
    for(j = 0; j < Dest->NN.Layer_Sizes[i]; ++j)
    {
      for(k = 0; k < Dest->NN.Layer_Sizes[i-1]; ++k)
      {
        Dest->NN.Weights[i][j][k] = Source->NN.Weights[i][j][k];
      }
    }
  }

  return TRUE;
}

int Bot_Refresh(bot* Our_Bot)
{
  Our_Bot->Energy = BOT_START_ENERGY;
  Our_Bot->Age = 0;
  Our_Bot->Dead = FALSE;
  return TRUE;
}

int Bot_Breed(bot* Child, bot* Parent1, bot* Parent2)
{
  if(Child == NULL || Parent1 == NULL || Parent2 == NULL)
    return FALSE;

  // Basic version
  if(rand()%2 == 0)
    Bot_Copy(Child, Parent1);
  else
    Bot_Copy(Child, Parent2);

  // Crossover version
  /*
  Bot_Copy(Child, Parent1);
  if(rand()%2 == 0)
    Child->Size = Parent2->Size;
  if(rand()%2 == 0)
    Child->Turn_Rate = Parent2->Turn_Rate;
  */

  return TRUE;
}

int Bot_Mutate(bot* Our_Bot)
{
  Our_Bot->Red += RAND_BETWEEN(-0.1, 0.1);
  Our_Bot->Green += RAND_BETWEEN(-0.1, 0.1);
  Our_Bot->Blue += RAND_BETWEEN(-0.1, 0.1);
  Our_Bot->Turn_Rate += RAND_BETWEEN(-0.1, 0.1);

  // Neural network
  NN_Mutate(&Our_Bot->NN);

  // Eyes
  int e;
  for(e = 0; e < Our_Bot->Num_Eyes; ++e)
  {
    //Our_Bot->Eyes[e].Position += RAND_BETWEEN(-1.0, 1.0);
    //Our_Bot->Eyes[e].View_Angle += RAND_BETWEEN(-1.0, 1.0);
    //Our_Bot->Eyes[e].View_Distance += RAND_BETWEEN(-0.1, 0.1);
  }

  // Spikes
  int s;
  for(s = 0; e < Our_Bot->Num_Eyes; ++s)
  {
    //Our_Bot->Spikes[s].Position += RAND_BETWEEN(-0.1, 0.1);
    //Our_Bot->Spikes[s].Length += RAND_BETWEEN(-0.1, 0.1);
  }

  return TRUE;
}

void Bots_Breed_New_Generation(world* Our_World)
{
  // Create parent bots
  int p;
  for(p = 0; p < Our_World->Num_Parents; ++p)
  {
    Bot_Copy(&Our_World->Bot_Parents[p], &Our_World->Bots[Our_World->Bot_Ranks[p]]);
  }

  // Breed new generation from parents
  int b;
  for(b = 0; b < Our_World->Num_Bots; ++b)
  {
    Bot_Breed(&Our_World->Bots[b],
              &Our_World->Bot_Parents[rand()%Our_World->Num_Parents],
              &Our_World->Bot_Parents[rand()%Our_World->Num_Parents]);

    // Refresh bot
    Our_World->Bots[b].x = RAND_BETWEEN(0, Our_World->Width);
    Our_World->Bots[b].y = RAND_BETWEEN(0, Our_World->Height);
    Our_World->Bots[b].Energy = BOT_START_ENERGY;
    Our_World->Bots[b].Age = 0;
    Our_World->Bots[b].Dead = FALSE;

    // Mutate bot
    Bot_Mutate(&Our_World->Bots[b]);
  }
}

void Bot_Ranks(world* Our_World)
{
  #ifndef BOT_SCORE
  #define BOT_SCORE (Our_World->Bots[b].Age + Our_World->Bots[b].Energy)
  #endif

  int n;
  //for(n = Our_World->Num_Bots-1; n >= 0; --n)
  for(n = 0; n < Our_World->Num_Bots; ++n)
  {
    int Best_Bot_Score = -1;
    int Best_Bot = -1;

    int b;
    for(b = 0; b < Our_World->Num_Bots; ++b)
    {
      if(BOT_SCORE > Best_Bot_Score)
      {
        Best_Bot_Score = BOT_SCORE;
        Best_Bot = b;
      }
    }

    Our_World->Bot_Ranks[n] = Best_Bot;
    Our_World->Bots[Best_Bot].Age = -1;
    Our_World->Bots[Best_Bot].Energy = -1;
  }

  #undef BOT_SCORE
}

int Bot_Kill(world* Our_World, int b)
{
  if(b >= Our_World->Num_Bots || b < 0 || Our_World->Bots[b].Dead == TRUE)
    return FALSE;

  Our_World->Bots[b].Dead = TRUE;
  Our_World->Num_Bots_Alive--;
  return TRUE;
}

int Bot_Scramble(world* Our_World, int b)
{
  if(b >= Our_World->Num_Bots || b < 0)
    return FALSE;

  Our_World->Bots[b].Red = RAND_BETWEEN(0.0, 1.0);
  Our_World->Bots[b].Green = RAND_BETWEEN(0.0, 1.0);
  Our_World->Bots[b].Blue = RAND_BETWEEN(0.0, 1.0);
  Our_World->Bots[b].r = RAND_BETWEEN(0.0, 360.0);
  return TRUE;
}

int Bot_Remove(world* Our_World, int n)
{
  if(n >= Our_World->Num_Bots || n < 0)
    return FALSE;

  Bot_Copy(&Our_World->Bots[n], &Our_World->Bots[Our_World->Num_Bots-1]);
  //Our_World->Bots[n] = Our_World->Bots[Our_World->Num_Bots-1];
  // World stats
  Our_World->Bots_Removed++;
  Our_World->Num_Bots--;
  return TRUE;
}

int Bot_Dump(bot* Our_Bot)
{
  FILE* Our_File = fopen("Bot_Dump.txt", "a");
  if(Our_Bot == NULL || Our_File == NULL)
    return FALSE;

  // General
  fprintf(Our_File, "Energy: %i (%p)\n", Our_Bot->Energy, (void*)&Our_Bot->Energy);
  fprintf(Our_File, "Age: %i (%p)\n", Our_Bot->Age, (void*)&Our_Bot->Age);
  fprintf(Our_File, "Dead: %i (%p)\n", Our_Bot->Dead, (void*)&Our_Bot->Dead);
  fprintf(Our_File, "x: %.4g (%p)\n", Our_Bot->x, (void*)&Our_Bot->x);
  fprintf(Our_File, "y: %.4g (%p)\n", Our_Bot->y, (void*)&Our_Bot->y);
  fprintf(Our_File, "r: %.4g (%p)\n", Our_Bot->r, (void*)&Our_Bot->r);
  fprintf(Our_File, "Size: %.4g (%p)\n", Our_Bot->Size, (void*)&Our_Bot->Size);
  fprintf(Our_File, "Turn_Rate: %.4g (%p)\n", Our_Bot->Turn_Rate, (void*)&Our_Bot->Turn_Rate);
  fprintf(Our_File, "Red: %.4g (%p)\n", Our_Bot->Red, (void*)&Our_Bot->Red);
  fprintf(Our_File, "Green: %.4g (%p)\n", Our_Bot->Green, (void*)&Our_Bot->Green);
  fprintf(Our_File, "Blue: %.4g (%p)\n", Our_Bot->Blue, (void*)&Our_Bot->Blue);

  // Brain
  fprintf(Our_File, "NN: (%p)\n", (void*)&Our_Bot->NN);
  fprintf(Our_File, " NN.Num_Layers: %i (%p)\n",  Our_Bot->NN.Num_Layers, (void*)&Our_Bot->NN.Num_Layers);
  fprintf(Our_File, " NN.Num_Inputs: %i (%p)\n",  Our_Bot->NN.Num_Inputs, (void*)&Our_Bot->NN.Num_Inputs);
  fprintf(Our_File, " NN.Num_Outputs: %i (%p)\n", Our_Bot->NN.Num_Outputs, (void*)&Our_Bot->NN.Num_Outputs);
  fprintf(Our_File, " NN.Input: %p (%p)\n", Our_Bot->NN.Input, (void*)&Our_Bot->NN.Input);
  fprintf(Our_File, " NN.Output: %p (%p)\n", Our_Bot->NN.Output, (void*)&Our_Bot->NN.Output);
  fprintf(Our_File, " NN.Weights: %p (%p)\n", Our_Bot->NN.Weights, (void*)&Our_Bot->NN.Weights);

  // Eyes
  int e;
  fprintf(Our_File, "Num_Eyes: %i (%p)\n", Our_Bot->Num_Eyes, (void*)&Our_Bot->Num_Eyes);
  fprintf(Our_File, "Eyes: %p (%p)\n", Our_Bot->Eyes, (void*)&Our_Bot->Eyes);
  for(e = 0; e < Our_Bot->Num_Eyes; ++e)
  {
    fprintf(Our_File, " Eye[%i].Position: %.4g (%p)\n",      e, Our_Bot->Eyes[e].Position,      (void*)&Our_Bot->Eyes[e].Position);
    fprintf(Our_File, " Eye[%i].View_Distance: %.4g (%p)\n", e, Our_Bot->Eyes[e].View_Distance, (void*)&Our_Bot->Eyes[e].View_Distance);
    fprintf(Our_File, " Eye[%i].View_Angle: %.4g (%p)\n",    e, Our_Bot->Eyes[e].View_Angle,    (void*)&Our_Bot->Eyes[e].View_Angle);
    fprintf(Our_File, " Eye[%i].In_Strength: %.4g (%p)\n",   e, Our_Bot->Eyes[e].In_Strength,   (void*)&Our_Bot->Eyes[e].In_Strength);
    fprintf(Our_File, " Eye[%i].In_Red: %.4g (%p)\n",        e, Our_Bot->Eyes[e].In_Red,        (void*)&Our_Bot->Eyes[e].In_Red);
    fprintf(Our_File, " Eye[%i].In_Green: %.4g (%p)\n",      e, Our_Bot->Eyes[e].In_Green,      (void*)&Our_Bot->Eyes[e].In_Green);
    fprintf(Our_File, " Eye[%i].In_Blue: %.4g (%p)\n",       e, Our_Bot->Eyes[e].In_Blue,       (void*)&Our_Bot->Eyes[e].In_Blue);
  }

  // Spikes
  fprintf(Our_File, "Spikes: (%p)\n", (void*)&Our_Bot->Spikes);

  fprintf(Our_File, "\n");
  fclose(Our_File);
  return TRUE;
}

int Bot_Create(bot* Our_Bot, float x, float y)
{
  if(Our_Bot == NULL)
    return FALSE;

  Our_Bot->x = x;
  Our_Bot->y = y;
  Our_Bot->Age = 0;
  Our_Bot->Size = 1;
  Our_Bot->Dead = FALSE;
  Our_Bot->Red   = RAND_BETWEEN(0.0, 1.0);
  Our_Bot->Green = RAND_BETWEEN(0.0, 1.0);
  Our_Bot->Blue  = RAND_BETWEEN(0.0, 1.0);
  Our_Bot->Energy = BOT_START_ENERGY;
  Our_Bot->r = RAND_BETWEEN(0.0, 360.0);
  Our_Bot->Turn_Rate = 3.0;
  // Eyes
  Our_Bot->Num_Eyes = 3;
  Our_Bot->Eyes = (eye*) malloc(Our_Bot->Num_Eyes*sizeof(eye));
  Our_Bot->Eyes[0].Position =  15;
  Our_Bot->Eyes[1].Position = 345;
  Our_Bot->Eyes[2].Position = 180;
  Our_Bot->Eyes[0].View_Angle = 30;
  Our_Bot->Eyes[1].View_Angle = 30;
  Our_Bot->Eyes[2].View_Angle = 15;
  Our_Bot->Eyes[0].View_Distance = 2.5;
  Our_Bot->Eyes[1].View_Distance = 2.5;
  Our_Bot->Eyes[2].View_Distance = 2.5;
  // Spikes
  Our_Bot->Num_Spikes = 1;
  Our_Bot->Spikes = (spike*) malloc(Our_Bot->Num_Spikes*sizeof(spike));
  Our_Bot->Spikes[0].Position = 0;
  Our_Bot->Spikes[0].Length = RAND_BETWEEN(0.0, 0.2);
  // NN
  Our_Bot->NN.Num_Layers = 3;
  Our_Bot->NN.Layer_Sizes = (int*) malloc(3*sizeof(int));
  Our_Bot->NN.Layer_Sizes[0] = 13;
  Our_Bot->NN.Layer_Sizes[1] = 7;
  Our_Bot->NN.Layer_Sizes[2] = 3;

  NN_Create(&Our_Bot->NN);
  NN_Random_Weights(&Our_Bot->NN, -1.0, 1.0);

  return TRUE;
}

int Bot_Add(world* Our_World, float x, float y)
{
  if(Our_World->Num_Bots >= Our_World->Max_Bots)
    return FALSE;

  if(x < 0) x = RAND_BETWEEN(0, Our_World->Width);
  if(y < 0) y = RAND_BETWEEN(0, Our_World->Height);

  //printf("%.4g %.4g\n", x, y);
  //getchar();

  Bot_Create(&Our_World->Bots[Our_World->Num_Bots], x, y);

  // World stats
  Our_World->Bots_Added++;
  Our_World->Num_Bots++;
  Our_World->Num_Bots_Alive++;

  //printf("Add_Bot() %.4g %.4g\n", Our_World->Bots[Our_World->Num_Bots].x, Our_World->Bots[Our_World->Num_Bots].y);

  return TRUE;
}
