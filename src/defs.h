#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>  // _beginthread()
#include <time.h>     // time()
#include <math.h>     // sqrt()

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define RAND_BETWEEN(a,b) ((b-a)*((float)rand()/RAND_MAX)+a)
#define DIST(x1,y1, x2,y2) sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
#define DEG_TO_RAD(x) (x*0.017453)
#define RAD_TO_DEG(x) (x/0.017453)
#define NORMALISE_DEG(x) (x >= 360) ? x=x-360 : ((x < 0) ? x=x+360 : x)
#define SIGMOID(x) 1/(1 + exp(-x))

#define BOT_START_ENERGY  500
#define BOT_NUTRITION     200
#define PELLET_NUTRITION  200

typedef struct
{
  int eye_offset;
  int spike_offset;
  int num_layers;
  int num_inputs;
  int num_outputs;
  int *layer_sizes;
  double **input;
  double **output;
  double ***weights;
} neural_network;

typedef struct
{
  float x;
  float y;
  //int energy;
  //int age;
  int nutrition;
  float size;
  float red;
  float green;
  float blue;
} pellet;

typedef struct
{
  float position;
  float view_distance;
  float view_angle;
  float red;
  float green;
  float blue;
  // What it sees
  float in_strength;
  float in_red;
  float in_green;
  float in_blue;
} eye;

typedef struct
{
  int retracted;
  float position;
  float length;
  //float red;
  //float green;
  //float blue;
} spike;

typedef struct
{
  // General
  int energy;
  int age;
  int dead;
  // position
  float x;
  float y;
  float r;
  float size;
  float turn_rate;
  // Colour
  float red;
  float green;
  float blue;
  // brain
  neural_network nn;
  // eyes
  int num_eyes;
  eye *eyes;
  // spikes
  int num_spikes;
  spike *spikes;
} bot;

typedef struct
{
  // Normal
  int seed;
  int width;
  int height;
  int selected;
  // stats
  int generation;
  int frame;
  int bots_added;
  int bots_removed;
  int bots_most;
  int pellets_added;
  int pellets_removed;
  int pellets_most;
  // bots
  int num_bots;
  int max_bots;
  int num_bots_alive;
  int num_parents;
  bot* bots;
  bot* bot_parents;
  int* bot_ranks;
  // pellets
  int num_pellets;
  int max_pellets;
  pellet* pellets;
  // grid
  int grid_width;
  int grid_height;
  //grid** grid;
} world;

typedef struct
{
  HWND hWnd;
  HWND hstatistics;
  HDC hDC;
  HGLRC hRC;
  world *world;
  int display;
  int display_statistics;
  int w;
  int h;
  float r;
  float view_X;
  float view_Y;
  float view_zoom;
  int quit;
} render_parameters;

typedef struct
{
  world *world;
  int pause;
  int quit;
  float delay;
  int logging;
  FILE* log_file;
} simulation_parameters;

render_parameters Main;
render_parameters viewer;
simulation_parameters simulation;

float angle_difference(float x, float y);

// world.c
int world_init(world* our_world);

// simulation.c
int simulation_init(world *our_world);
int simulation_end(world* our_world);
void simulate_world(void*);

// bots.c
void bot_ranks(world* our_world);
void bots_breed_new_generation(world* our_world);
int bot_create(bot* our_bot, float x, float y);
int bot_dump(bot* our_bot);
int bot_mutate(bot *our_bot);
int bot_kill(world* our_world, int b);
void draw_edge_bots(world *our_world);
int bot_scramble(world* our_world, int n);
int bot_remove(world* our_world, int n);
int bot_add(world* our_world, float x, float y);
int bot_find_closest(world* our_world, float x, float y);

// pellets.c
int pellet_add(world* our_world, float x, float y);
int pellet_remove(world* our_world, int p);

// render.c
int main_init(world *our_world, int w, int h);
int viewer_init(world *our_world, int w, int h);
void render_Main(void* a);
void render_viewer(void* a);

// neural network.c
int nn_mutate(neural_network* network);
void nn_create(neural_network* nn);
void update_bots_nns(world* our_world);
void nn_random_weights(neural_network* our_nn, double min, double max);

#endif // DEFS_H_INCLUDED
