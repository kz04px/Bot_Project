#include "defs.h"

void draw_pellet(pellet* p)
{
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

void draw_pellets(world *our_world)
{
  int p;
  for(p = 0; p < our_world->num_pellets; ++p)
  {
    draw_pellet(&our_world->pellets[p]);
  }
}

int pellet_remove(world* our_world, int p)
{
  if(our_world->num_pellets < 1)
    return 0;

  our_world->pellets[p] = our_world->pellets[our_world->num_pellets-1];
  // world stats
  our_world->pellets_removed++;
  our_world->num_pellets--;
  return 1;
}

int pellet_add(world* our_world, float x, float y)
{
  if(our_world->num_pellets >= our_world->max_pellets)
    return 0;

  if(x < 0)
    our_world->pellets[our_world->num_pellets].x = RAND_BETWEEN(0.0, our_world->width);
  else
    our_world->pellets[our_world->num_pellets].x = x;

  if(y < 0)
    our_world->pellets[our_world->num_pellets].y = RAND_BETWEEN(0.0, our_world->height);
  else
    our_world->pellets[our_world->num_pellets].y = y;

  our_world->pellets[our_world->num_pellets].nutrition = PELLET_NUTRITION;
  //our_world->pellets[our_world->num_pellets].energy = PELLET_START_ENERGY;
  //our_world->pellets[our_world->num_pellets].energy = PELLET_ENERGY;
  //our_world->pellets[our_world->num_pellets].age = 0;
  our_world->pellets[our_world->num_pellets].red = 1.0;
  our_world->pellets[our_world->num_pellets].green = 1.0;
  our_world->pellets[our_world->num_pellets].blue = 0.0;
  // world stats
  our_world->pellets_added++;
  our_world->num_pellets++;

  //printf("add_pellet() %.4g %.4g\n", our_world->pellets[our_world->num_pellets].x, our_world->pellets[our_world->num_pellets].y);

  return 1;
}
