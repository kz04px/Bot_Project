#include "defs.h"

void draw_pellet(s_pellet* p)
{
  ASSERT(p != NULL);
  
  glPushMatrix();
  glColor3f(p->red, p->green, p->blue);
  glTranslatef(p->x, p->y, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(-0.05, -0.05);
    glVertex2f(-0.05,  0.05);
    glVertex2f( 0.05,  0.05);
    glVertex2f( 0.05, -0.05);
  glEnd();
  glPopMatrix();
}

void draw_pellets(s_world *world)
{
  ASSERT(world != NULL);
  
  int p;
  for(p = 0; p < world->num_pellets; ++p)
  {
    draw_pellet(&world->pellets[p]);
  }
}

int pellet_remove(s_world *world, int p)
{
  ASSERT(world != NULL);
  
  if(world->num_pellets < 1)
  {
    return 0;
  }

  world->pellets[p] = world->pellets[world->num_pellets-1];
  // world stats
  world->pellets_removed++;
  world->num_pellets--;
  return 1;
}

int pellet_add(s_world *world, float x, float y)
{
  ASSERT(world != NULL);
  
  if(world->num_pellets >= world->max_pellets)
  {
    return 0;
  }

  if(x < 0)
  {
    world->pellets[world->num_pellets].x = RAND_BETWEEN(0.0, world->width);
  }
  else
  {
    world->pellets[world->num_pellets].x = x;
  }

  if(y < 0)
  {
    world->pellets[world->num_pellets].y = RAND_BETWEEN(0.0, world->height);
  }
  else
  {
    world->pellets[world->num_pellets].y = y;
  }

  world->pellets[world->num_pellets].nutrition = PELLET_NUTRITION;
  //world->pellets[world->num_pellets].energy = PELLET_START_ENERGY;
  //world->pellets[world->num_pellets].energy = PELLET_ENERGY;
  //world->pellets[world->num_pellets].age = 0;
  world->pellets[world->num_pellets].red = 1.0;
  world->pellets[world->num_pellets].green = 1.0;
  world->pellets[world->num_pellets].blue = 0.0;
  // world stats
  world->pellets_added++;
  world->num_pellets++;

  //printf("add_pellet() %.4g %.4g\n", world->pellets[world->num_pellets].x, world->pellets[world->num_pellets].y);

  return 1;
}
