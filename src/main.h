#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_TITLE "Bots Project"
#define VERSION "v1.00"
#define AUTHOR "Twipply"
#define TRUE 1
#define FALSE 0

// Start/Pause button
#define BUTTON_1_ID 1001
#define BUTTON_1_TEXT_1 "Start"
#define BUTTON_1_TEXT_2 "Pause"
// Viewer button
#define BUTTON_2_ID 1002
#define BUTTON_2_TEXT_1 "Viewer"
// Delete button
#define BUTTON_3_ID 1003
#define BUTTON_3_TEXT_1 "Delete"
// Scramble button
#define BUTTON_4_ID 1004
#define BUTTON_4_TEXT_1 "Scramble"
// Find button
#define BUTTON_5_ID 1005
#define BUTTON_5_TEXT_1 "Find"
// Previous button
#define BUTTON_6_ID 1006
#define BUTTON_6_TEXT_1 "Previous"
// Next button
#define BUTTON_7_ID 1007
#define BUTTON_7_TEXT_1 "Next"
// Statistics button
#define BUTTON_8_ID 1008
#define BUTTON_8_TEXT_1 "Statistics"

// Tools
#define NO_TOOL      0
#define PELLET_ADDER 1
#define BOT_ADDER    2
#define BOT_SELECTOR 3

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

char szClassName[] = "CodeBlocksWindowsApp";
HWND hMain;
HWND hViewer;
HWND hPauseButton;
HWND hViewerButton;
HWND hDeleteButton;
HWND hScrambleButton;
HWND hFindButton;
HWND hPreviousButton;
HWND hNextButton;
HWND hStatisticsButton;
int Num_Threads = 0;
HANDLE hThreads[3];
int Window_Width = 840;
int Window_Height = 480;
int Window_Min_Width = 400;
int Window_Min_Height = 300;
int Current_Tool = NO_TOOL;
int bQuit = FALSE;

#endif // MAIN_H_INCLUDED
