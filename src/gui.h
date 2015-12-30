#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define WINDOW_TITLE "Bots Project"
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

int gui_create(HINSTANCE hInstance, HWND* main_hWnd, HWND* viewer_hWnd, HWND* main_hstatistics, HWND* viewer_hstatistics);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HWND hMain;
HWND hViewer;
HWND hpauseButton;
HWND hViewerButton;
HWND hDeleteButton;
HWND hscrambleButton;
HWND hfindButton;
HWND hPreviousButton;
HWND hNextButton;
HWND hStatisticsButton;
int window_width;
int window_height;
int window_min_width;
int window_min_height;

#endif // GUI_H_INCLUDED
