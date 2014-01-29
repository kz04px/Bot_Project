#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>  // _beginthread()
#include <time.h>     // time(0)
#include <math.h>     // sqrt()

#define TRUE  1
#define FALSE 0
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
  int Eye_Offset;
  int Spike_Offset;
  int Num_Layers;
  int Num_Inputs;
  int Num_Outputs;
  int *Layer_Sizes;
  double **Input;
  double **Output;
  double ***Weights;
} neural_network;

typedef struct
{
  float x;
  float y;
  //int Energy;
  //int Age;
  int Nutrition;
  float Size;
  float Red;
  float Green;
  float Blue;
} pellet;

typedef struct
{
  float Position;
  float View_Distance;
  float View_Angle;
  float Red;
  float Green;
  float Blue;
  // What it sees
  float In_Strength;
  float In_Red;
  float In_Green;
  float In_Blue;
} eye;

typedef struct
{
  int Retracted;
  float Position;
  float Length;
  //float Red;
  //float Green;
  //float Blue;
} spike;

typedef struct
{
  // General
  int Energy;
  int Age;
  int Dead;
  // Position
  float x;
  float y;
  float r;
  float Size;
  float Turn_Rate;
  // Colour
  float Red;
  float Green;
  float Blue;
  // brain
  neural_network NN;
  // eyes
  int Num_Eyes;
  eye *Eyes;
  // spikes
  int Num_Spikes;
  spike *Spikes;
} bot;

typedef struct
{
	// Normal
	int Time_Seed;
	int Width;
	int Height;
	int Selected;
	// Stats
	int Generation;
	int Frame;
	int Bots_Added;
	int Bots_Removed;
	int Bots_Most;
	int Pellets_Added;
	int Pellets_Removed;
	int Pellets_Most;
	// Bots
	int Num_Bots;
	int Max_Bots;
	int Num_Bots_Alive;
	int Num_Parents;
	bot* Bots;
	bot* Bot_Parents;
	int* Bot_Ranks;
	// Pellets
	int Num_Pellets;
	int Max_Pellets;
	pellet* Pellets;
	// Grid
	int Grid_Width;
	int Grid_Height;
	//grid** Grid;
} world;

typedef struct
{
  HWND hWnd;
  HWND hStatistics;
  HDC hDC;
  HGLRC hRC;
  world *World;
	int Display;
	int Display_Statistics;
	int w;
	int h;
  float r;
	float View_X;
	float View_Y;
	float View_Zoom;
	int Quit;
} render_parameters;

typedef struct
{
  world *World;
  int Pause;
  int Quit;
  float Delay;
  int Logging;
  FILE* Log_File;
} simulation_parameters;

render_parameters Main;
render_parameters Viewer;
simulation_parameters Simulation;

float Angle_Difference(float x, float y);
// world.c
int World_Init(world* Our_World);
// simulation.c
int Simulation_Init(world *Our_World);
int Simulation_End(world* Our_World);
void Simulate_World(void*);
// bots.c
void Bot_Ranks(world* Our_World);
void Bots_Breed_New_Generation(world* Our_World);
int Bot_Create(bot* Our_Bot, float x, float y);
int Bot_Dump(bot* Our_Bot);
int Bot_Mutate(bot *Our_Bot);
int Bot_Kill(world* Our_World, int b);
void Draw_Edge_Bots(world *Our_World);
int Bot_Scramble(world* Our_World, int n);
int Bot_Remove(world* Our_World, int n);
int Bot_Add(world* Our_World, float x, float y);
int Bot_Find_Closest(world* Our_World, float x, float y);
// pellets.c
int Pellet_Add(world* Our_World, float x, float y);
int Pellet_Remove(world* Our_World, int p);
// render.c
int Main_Init(world *Our_World, int w, int h);
int Viewer_Init(world *Our_World, int w, int h);
void Render_Main(void* a);
void Render_Viewer(void* a);
// neural network.c
int NN_Mutate(neural_network* Network);
void NN_Create(neural_network* NN);
void Update_Bots_NNs(world* Our_World);
void NN_Random_Weights(neural_network* Our_NN, double Min, double Max);

#endif // DEFS_H_INCLUDED
