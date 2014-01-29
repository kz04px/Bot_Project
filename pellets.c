#include "defs.h"


void Draw_Pellet(world *Our_World, int p)
{
  glPushMatrix();
  glColor3f(Our_World->Pellets[p].Red, Our_World->Pellets[p].Green, Our_World->Pellets[p].Blue);
  glTranslatef(Our_World->Pellets[p].x, Our_World->Pellets[p].y, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(-0.05, -0.05);
    glVertex2f(-0.05,  0.05);
    glVertex2f( 0.05,  0.05);
    glVertex2f( 0.05, -0.05);
  glEnd();
  glPopMatrix();
}

void Draw_Pellets(world *Our_World)
{
	int p;
	for(p = 0; p < Our_World->Num_Pellets; ++p)
	{
	  Draw_Pellet(Our_World, p);
	}
}

int Pellet_Remove(world* Our_World, int p)
{
  if(Our_World->Num_Pellets < 1)
    return FALSE;

  Our_World->Pellets[p] = Our_World->Pellets[Our_World->Num_Pellets-1];
  // World stats
  Our_World->Pellets_Removed++;
  Our_World->Num_Pellets--;
  return TRUE;
}

int Pellet_Add(world* Our_World, float x, float y)
{
  if(Our_World->Num_Pellets >= Our_World->Max_Pellets)
    return FALSE;

  if(x < 0)
    Our_World->Pellets[Our_World->Num_Pellets].x = RAND_BETWEEN(0.0, Our_World->Width);
  else
    Our_World->Pellets[Our_World->Num_Pellets].x = x;

  if(y < 0)
    Our_World->Pellets[Our_World->Num_Pellets].y = RAND_BETWEEN(0.0, Our_World->Height);
  else
    Our_World->Pellets[Our_World->Num_Pellets].y = y;

  Our_World->Pellets[Our_World->Num_Pellets].Nutrition = PELLET_NUTRITION;
  //Our_World->Pellets[Our_World->Num_Pellets].Energy = PELLET_START_ENERGY;
  //Our_World->Pellets[Our_World->Num_Pellets].Energy = PELLET_ENERGY;
  //Our_World->Pellets[Our_World->Num_Pellets].Age = 0;
  Our_World->Pellets[Our_World->Num_Pellets].Red = 1.0;
  Our_World->Pellets[Our_World->Num_Pellets].Green = 1.0;
  Our_World->Pellets[Our_World->Num_Pellets].Blue = 0.0;
  // World stats
  Our_World->Pellets_Added++;
  Our_World->Num_Pellets++;

  // Boundary checks
  /*
  if(x < 0)                       Our_World->Pellets[Our_World->Num_Pellets].x = 0;
  else if(x > Our_World->Width)   Our_World->Pellets[Our_World->Num_Pellets].x = Our_World->Width;
  if(y < 0)                       Our_World->Pellets[Our_World->Num_Pellets].y = 0;
  else if(y > Our_World->Height)  Our_World->Pellets[Our_World->Num_Pellets].y = Our_World->Height;
  */

  //printf("Add_Pellet() %.4g %.4g\n", Our_World->Pellets[Our_World->Num_Pellets].x, Our_World->Pellets[Our_World->Num_Pellets].y);

  return TRUE;
}
