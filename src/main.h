#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <math.h>

#define VERSION "v1.00"
#define AUTHOR "Twipply"
#define TRUE 1
#define FALSE 0

// Tools
#define NO_TOOL      0
#define PELLET_ADDER 1
#define BOT_ADDER    2
#define BOT_SELECTOR 3

int Num_Threads = 0;
HANDLE hThreads[3];
int Current_Tool = NO_TOOL;
int bQuit = FALSE;

#endif // MAIN_H_INCLUDED
