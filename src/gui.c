#include "gui.h"

int GUI_Create(HINSTANCE Our_hInstance, HWND* Main_hWnd, HWND* Viewer_hWnd, HWND* Main_hStatistics, HWND* Viewer_hStatistics)
{
  Window_Width = 840;
  Window_Height = 480;
  Window_Min_Width = 400;
  Window_Min_Height = 300;
  char szClassName[] = "ClassName";
	WNDCLASSEX wincl;

	/* The Window structure */
	wincl.hInstance = Our_hInstance;
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
		   Window_Width+18,                       // The program's width
		   Window_Height+36,                      // and height in pixels
		   HWND_DESKTOP,                          // The window is a child-window to desktop
		   NULL,                                  // No menu
		   Our_hInstance,                         // Program Instance handler
		   NULL                                   // No Window Creation data
		   );

	*Main_hWnd = CreateWindowEx(
		   0,                                     // Extended possibilites for variation
		   szClassName,                           // Classname
		   "Main",   // Title Text
		   WS_CHILD | WS_VISIBLE,                 // default window
		   200,                                   // Windows decides the position
		   0,                                     // where the window ends up on the screen
		   Window_Width-200,                      // The program's width
		   Window_Height,                         // and height in pixels
		   hMain,                                 //
		   NULL,                                  // No menu
		   Our_hInstance,                         // Program Instance handler
		   NULL                                   // No Window Creation data
		   );

	hViewer = CreateWindowEx(
		   0,                                     // Extended possibilites for variation
		   szClassName,                           // Classname
		   "Viewer",                              // Title Text
		   (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU),
		   //WS_OVERLAPPEDWINDOW^WS_THICKFRAME,     // default window
		   CW_USEDEFAULT,                         // Windows decides the position
		   CW_USEDEFAULT,                         // where the window ends up on the screen
		   300+5,                                   // The program's width
		   430+25,                                   // and height in pixels
		   hMain,                                 //
		   NULL,                                  // No menu
		   Our_hInstance,                         // Program Instance handler
		   NULL                                   // No Window Creation data
		   );

	*Viewer_hWnd = CreateWindowEx(
		   0,                                     // Extended possibilites for variation
		   szClassName,                           // Classname
		   "Viewer",                              // Title Text
		   WS_CHILD | WS_VISIBLE,     // default window
		   0,                         // Windows decides the position
		   30,                         // where the window ends up on the screen
		   300,                                   // The program's width
		   300,                                   // and height in pixels
		   hViewer,                                 //
		   NULL,                                  // No menu
		   Our_hInstance,                         // Program Instance handler
		   NULL                                   // No Window Creation data
		   );

  // Start/Pause button
  hPauseButton = CreateWindow("button", BUTTON_1_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                0, 0, 200, 80, hMain, (HMENU)BUTTON_1_ID, Our_hInstance, NULL);
  // Show/Hide Viewer button
  hViewerButton = CreateWindow("button", BUTTON_2_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                0, 80, 200, 80, hMain, (HMENU)BUTTON_2_ID, Our_hInstance, NULL);
  // Delete button
  hDeleteButton = CreateWindow("button", BUTTON_3_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                0, 330, 150, 50, hViewer, (HMENU)BUTTON_3_ID, Our_hInstance, NULL);
  // Scramble button
  hScrambleButton = CreateWindow("button", BUTTON_4_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                150, 330, 150, 50, hViewer, (HMENU)BUTTON_4_ID, Our_hInstance, NULL);
  // Find button
  hFindButton = CreateWindow("button", BUTTON_5_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                0, 380, 150, 50, hViewer, (HMENU)BUTTON_5_ID, Our_hInstance, NULL);
  // Previous button
  hPreviousButton = CreateWindow("button", BUTTON_6_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                0, 0, 150, 30, hViewer, (HMENU)BUTTON_6_ID, Our_hInstance, NULL);
  // Next button
  hNextButton = CreateWindow("button", BUTTON_7_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                150, 0, 150, 30, hViewer, (HMENU)BUTTON_7_ID, Our_hInstance, NULL);
  // 'Statistics' button
  hStatisticsButton = CreateWindow("button", BUTTON_8_TEXT_1, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                150, 380, 150, 50, hViewer, (HMENU)BUTTON_8_ID, Our_hInstance, NULL);

  // Main statistics box
	*Main_hStatistics = CreateWindow("Edit", "None", WS_CHILD | WS_VISIBLE | ES_MULTILINE,
                0, 160, 200, 320, hMain, NULL, Our_hInstance, NULL);
  // Viewer statistics box
	*Viewer_hStatistics = CreateWindow("Edit", "None", WS_CHILD | WS_VISIBLE | ES_MULTILINE,
                300, 0, 300, 430, hViewer, NULL, Our_hInstance, NULL);

  return TRUE;
}
