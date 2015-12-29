#ifndef gui_H_INCLUDED
#define gui_H_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define WINDOW_TITLE "bots Project"
// Start/pause button
#define BUTTON_1_ID 1001
#define BUTTON_1_TEXT_1 "Start"
#define BUTTON_1_TEXT_2 "pause"
// viewer button
#define BUTTON_2_ID 1002
#define BUTTON_2_TEXT_1 "viewer"
// Delete button
#define BUTTON_3_ID 1003
#define BUTTON_3_TEXT_1 "Delete"
// scramble button
#define BUTTON_4_ID 1004
#define BUTTON_4_TEXT_1 "scramble"
// find button
#define BUTTON_5_ID 1005
#define BUTTON_5_TEXT_1 "find"
// Previous button
#define BUTTON_6_ID 1006
#define BUTTON_6_TEXT_1 "Previous"
// Next button
#define BUTTON_7_ID 1007
#define BUTTON_7_TEXT_1 "Next"
// statistics button
#define BUTTON_8_ID 1008
#define BUTTON_8_TEXT_1 "statistics"

int gui_create(HINSTANCE our_hInstance, HWND* main_hWnd, HWND* viewer_hWnd, HWND* main_hstatistics, HWND* viewer_hstatistics);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HWND hMain;
HWND hviewer;
HWND hpauseButton;
HWND hviewerButton;
HWND hDeleteButton;
HWND hscrambleButton;
HWND hfindButton;
HWND hPreviousButton;
HWND hNextButton;
HWND hstatisticsButton;
int windowwidth;
int windowheight;
int windowmin_width;
int windowmin_height;

#endif // gui_H_INCLUDED
