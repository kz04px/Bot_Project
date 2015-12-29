#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <math.h>

#define VERSION "1.00"

// Tools
#define NO_TOOL      0
#define PELLET_ADDER 1
#define BOT_ADDER    2
#define BOT_SELECTOR 3

int num_threads = 0;
HANDLE hThreads[3];
int current_tool = NO_TOOL;
int bquit = 0;

#endif // MAIN_H_INCLUDED
