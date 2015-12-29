#include "gui.h"

int gui_create(HINSTANCE our_hInstance, HWND* main_hWnd, HWND* viewer_hWnd, HWND* main_hstatistics, HWND* viewer_hstatistics)
{
  windowwidth = 840;
  windowheight = 480;
  windowmin_width = 400;
  windowmin_height = 300;
  char szClassName[] = "ClassName";
  WNDCLASSEX wincl;

  /* The Window structure */
  wincl.hInstance = our_hInstance;
  wincl.lpszClassName = szClassName;
  wincl.lpfnWndProc = WindowProcedure;
  wincl.style = CS_DBLCLKS;
  wincl.cbSize = sizeof(WNDCLASSEX);

  /* Use default icon and mouse-pointer */
  wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wincl.lpszMenuName = NULL;
  wincl.cbClsExtra = 0;
  wincl.cbWndExtra = 0;
  wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

  /* Register the window class, and if it fails quit the program */
  if(!RegisterClassEx(&wincl))
    return 0;

  /* The class is registered, let's create the program*/
  hMain = CreateWindowEx(
       0,                                     // Extended possibilites for variation
       szClassName,                           // Classname
       WINDOW_TITLE,                          // Title Text
       WS_OVERLAPPEDWINDOW,                   // default window
       CW_USEDEFAULT,                         // Windows decides the position
       CW_USEDEFAULT,                         // where the window ends up on the screen
       windowwidth+18,                       // The program's width
       windowheight+36,                      // and height in pixels
       HWND_DESKTOP,                          // The window is a child-window to desktop
       NULL,                                  // No menu
       our_hInstance,                         // Program Instance handler
       NULL                                   // No Window Creation data
       );

  *main_hWnd = CreateWindowEx(
       0,                                     // Extended possibilites for variation
       szClassName,                           // Classname
       "Main",   // Title Text
       WS_CHILD | WS_VISIBLE,                 // default window
       200,                                   // Windows decides the position
       0,                                     // where the window ends up on the screen
       windowwidth-200,                      // The program's width
       windowheight,                         // and height in pixels
       hMain,                                 //
       NULL,                                  // No menu
       our_hInstance,                         // Program Instance handler
       NULL                                   // No Window Creation data
       );

  hviewer = CreateWindowEx(
       0,                                     // Extended possibilites for variation
       szClassName,                           // Classname
       "viewer",                              // Title Text
       (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU),
       //WS_OVERLAPPEDWINDOW^WS_THICKFRAME,   // default window
       CW_USEDEFAULT,                         // Windows decides the position
       CW_USEDEFAULT,                         // where the window ends up on the screen
       300+5,                                 // The program's width
       430+25,                                // and height in pixels
       hMain,                                 //
       NULL,                                  // No menu
       our_hInstance,                         // Program Instance handler
       NULL                                   // No Window Creation data
       );

  *viewer_hWnd = CreateWindowEx(
       0,                                     // Extended possibilites for variation
       szClassName,                           // Classname
       "viewer",                              // Title Text
       WS_CHILD | WS_VISIBLE,                 // default window
       0,                                     // Windows decides the position
       30,                                    // where the window ends up on the screen
       300,                                   // The program's width
       300,                                   // and height in pixels
       hviewer,                               //
       NULL,                                  // No menu
       our_hInstance,                         // Program Instance handler
       NULL                                   // No Window Creation data
       );

  // Start/pause button
  hpauseButton = CreateWindow("button", BUTTON_1_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 0, 200, 80, hMain, (HMENU)BUTTON_1_ID, our_hInstance, NULL);
  // Show/Hide viewer button
  hviewerButton = CreateWindow("button", BUTTON_2_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 80, 200, 80, hMain, (HMENU)BUTTON_2_ID, our_hInstance, NULL);
  // Delete button
  hDeleteButton = CreateWindow("button", BUTTON_3_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 330, 150, 50, hviewer, (HMENU)BUTTON_3_ID, our_hInstance, NULL);
  // scramble button
  hscrambleButton = CreateWindow("button", BUTTON_4_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 150, 330, 150, 50, hviewer, (HMENU)BUTTON_4_ID, our_hInstance, NULL);
  // find button
  hfindButton = CreateWindow("button", BUTTON_5_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 380, 150, 50, hviewer, (HMENU)BUTTON_5_ID, our_hInstance, NULL);
  // Previous button
  hPreviousButton = CreateWindow("button", BUTTON_6_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 0, 150, 30, hviewer, (HMENU)BUTTON_6_ID, our_hInstance, NULL);
  // Next button
  hNextButton = CreateWindow("button", BUTTON_7_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 150, 0, 150, 30, hviewer, (HMENU)BUTTON_7_ID, our_hInstance, NULL);
  // 'statistics' button
  hstatisticsButton = CreateWindow("button", BUTTON_8_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 150, 380, 150, 50, hviewer, (HMENU)BUTTON_8_ID, our_hInstance, NULL);

  // Main statistics box
  *main_hstatistics = CreateWindow("Edit", "None", WS_CHILD | WS_VISIBLE | ES_MULTILINE, 0, 160, 200, 320, hMain, NULL, our_hInstance, NULL);
  // viewer statistics box
  *viewer_hstatistics = CreateWindow("Edit", "None", WS_CHILD | WS_VISIBLE | ES_MULTILINE, 300, 0, 300, 430, hviewer, NULL, our_hInstance, NULL);

  return 0;
}
