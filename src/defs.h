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

#define MAX_FRAMES_PER_GENERATION 5000

// Bots
#define MIN_VIEW_DIST     1.0
#define MAX_VIEW_DIST     5.0
#define MIN_VIEW_ANGLE    5
#define MAX_VIEW_ANGLE    45
#define MIN_SPIKE_LENGTH  1.0
#define MAX_SPIKE_LENGTH  5.0
#define BOT_START_ENERGY  500
#define BOT_NUTRITION     200

// Pellets
#define PELLET_NUTRITION  200

//#define DEBUG
#define LOGGING
//#define MUTATE_EYES
//#define MUTATE_SPIKES

#ifndef DEBUG
  #define ASSERT(n)
#else
#define ASSERT(n) \
  if(!(n)) \
  { \
    printf("\n%s - Failed\n", #n); \
    printf("On %s\n", __DATE__); \
    printf("At %s\n", __TIME__); \
    printf("In File %s\n", __FILE__); \
    printf("At Line %d\n", __LINE__); \
    getchar(); \
  }
#endif

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
} s_neural_network;

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
} s_pellet;

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
} s_eye;

typedef struct
{
  int retracted;
  float position;
  float length;
  //float red;
  //float green;
  //float blue;
} s_spike;

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
  s_neural_network nn;
  // eyes
  int num_eyes;
  s_eye *eyes;
  // spikes
  int num_spikes;
  s_spike *spikes;
} s_bot;

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
  int average_fitness;
  int spike_uses;
  // bots
  int num_bots;
  int max_bots;
  int num_bots_alive;
  int num_parents;
  s_bot *bots;
  s_bot *bot_parents;
  int* bot_ranks;
  // pellet
  int num_pellets;
  int max_pellets;
  s_pellet* pellets;
  // grid
  int grid_width;
  int grid_height;
  //grid** grid;
} s_world;

typedef struct
{
  HWND hWnd;
  HWND hstatistics;
  HDC hDC;
  HGLRC hRC;
  s_world *world;
  int display;
  int display_statistics;
  int w;
  int h;
  float r;
  float view_X;
  float view_Y;
  float view_zoom;
  int quit;
} s_render_parameters;

typedef struct
{
  s_world *world;
  int pause;
  int quit;
  float delay;
  #ifdef LOGGING
  FILE* log_file;
  #endif
} s_simulation_parameters;

s_render_parameters main_display;
s_render_parameters viewer_display;
s_simulation_parameters simulation;

float angle_difference(float x, float y);

// world.c
int world_init(s_world *world);

// simulation.c
int simulation_init(s_world *world);
int simulation_end(s_world *world);
void simulate_frame(s_world* world);
void simulate_world(void*);

// bots.c
void bot_ranks(s_world *world);
void bots_breed_new_generation(s_world *world);
int bot_create(s_bot *bot, float x, float y);
int bot_dump(s_bot *bot);
int bot_mutate(s_bot *bot);
int bot_kill(s_world *world, int b);
void draw_edge_bots(s_world *world);
int bot_scramble(s_world *world, int n);
int bot_remove(s_world *world, int n);
int bot_add(s_world *world, float x, float y);
int bot_find_closest(s_world *world, float x, float y);

// pellet.c
int pellet_add(s_world *world, float x, float y);
int pellet_remove(s_world *world, int p);

// render.c
int main_init(s_world *world, int w, int h);
int viewer_init(s_world *world, int w, int h);
void render_main(void* a);
void render_viewer(void* a);

// neural network.c
int nn_mutate(s_neural_network* network);
void nn_create(s_neural_network* nn);
void update_bots_nns(s_world *world);
void nn_random_weights(s_neural_network* nn, double min, double max);

#endif // DEFS_H_INCLUDED
