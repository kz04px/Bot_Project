#include "main.h"
#include "gui.h"
#include "defs.h"

world *Our_World = NULL;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{

  // GUI Stuff
	GUI_Create(hThisInstance, &Main.hWnd, &Viewer.hWnd, &Main.hStatistics, &Viewer.hStatistics);

	// Show the main window
	ShowWindow(hMain, nCmdShow);

  // Controls
  printf("Keyboard:\n");
  printf(" 1   - No tool\n");
  printf(" 2   - Pellet Adder tool\n");
  printf(" 3   - Bot Adder tool\n");
  printf(" 4   - Bot Selector tool\n");
  printf(" v   - Open/Close viewer\n");
  printf(" sb  - Start/Pause\n");
  printf(" esc - Close\n\n");

  printf("Mouse:\n");
  printf(" lb  - Use tool\n");
  printf(" rb  - Drag\n");
  printf(" sw  - Zoom\n\n");

  // Init stuff
  Our_World = (world*) malloc(1*sizeof(world));
  World_Init(Our_World);
  Main_Init(Our_World, Window_Width-200, Window_Height);
  Viewer_Init(Our_World, 300, 300);
 	Simulation_Init(Our_World);

 	// Create scenario
 	int i;
 	for(i = 0; i < 20; ++i)
    Bot_Add(Our_World, -1, -1);
 	for(i = 0; i < 150; ++i)
    Pellet_Add(Our_World, -1, -1);

 	// Threads
 	hThreads[0] = (HANDLE)_beginthread(Render_Main, 0, &Main);
	hThreads[1] = (HANDLE)_beginthread(Render_Viewer, 0, &Viewer);
 	hThreads[2] = (HANDLE)_beginthread(Simulate_World, 0, &Simulation);
 	Num_Threads = 3;

  MSG messages;
  while(GetMessage(&messages, NULL, 0, 0) && bQuit == FALSE)
  {
    TranslateMessage(&messages);
    DispatchMessage(&messages);
  }

	// Close stuff
	Main.Quit = TRUE;
	Viewer.Quit = TRUE;
	Simulation.Quit = TRUE;
  WaitForMultipleObjects(Num_Threads, hThreads, TRUE, INFINITE);
  Simulation_End(Our_World);
	return 0;
}

int Move_Camera = FALSE;
int xPos, yPos;
float aaa, bbb;
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	  case WM_CLOSE:
      if(hwnd == hViewer)
      {
        ShowWindow(hViewer, SW_HIDE);
        Viewer.Display = FALSE;
      }
      else
      {
        bQuit = TRUE;
        PostQuitMessage(0);
      }
      break;
	  case WM_SIZE:
      if(hwnd == hMain)
      {
        Window_Width = LOWORD(lParam);
        Window_Height = HIWORD(lParam);
        if(Window_Width < Window_Min_Width) {Window_Width = Window_Min_Width;}
        if(Window_Height < Window_Min_Height) {Window_Height = Window_Min_Height;}
        Main.w = Window_Width-200;
        Main.h = Window_Height;
        Main.r = (float)Main.h/Main.w;
        MoveWindow(Main.hWnd, 200,0, Window_Width,Window_Height, TRUE);
      }
      break;
	  case WM_COMMAND:
      SetFocus(hMain);
      switch(wParam)
      {
        case BUTTON_1_ID:
          Simulation.Pause = 1-Simulation.Pause;
          if(Simulation.Pause == TRUE)
            SetWindowText(hPauseButton, BUTTON_1_TEXT_1);
          else
            SetWindowText(hPauseButton, BUTTON_1_TEXT_2);
          break;
        case BUTTON_2_ID:
          Viewer.Display = 1-Viewer.Display;
          if(Viewer.Display == TRUE)
            ShowWindow(hViewer, SW_SHOW);
          else
            ShowWindow(hViewer, SW_HIDE);
          break;
        case BUTTON_3_ID:
          //printf("Remove bot: %i\n", Our_World->Selected);
          //Bot_Remove(Our_World, Our_World->Selected);

          printf("Kill bot: %i\n", Our_World->Selected);
          Bot_Kill(Our_World, Our_World->Selected);

          //Our_World->Selected = -1;
          if(Our_World->Selected >= Our_World->Num_Bots)
            Our_World->Selected--;

          break;
        case BUTTON_4_ID:
          printf("Scramble bot: %i\n", Our_World->Selected);
          Bot_Scramble(Our_World, Our_World->Selected);
          Bot_Mutate(&Our_World->Bots[Our_World->Selected]);
          break;
        case BUTTON_5_ID:
          if(Our_World->Selected < 0) break;
          printf("Find bot: %i\n", Our_World->Selected);
          Main.View_X = Our_World->Bots[Our_World->Selected].x;
          Main.View_Y = Our_World->Bots[Our_World->Selected].y;
          break;
        case BUTTON_6_ID: // 'Previous' button
          if(Our_World->Selected > 0) Our_World->Selected--;
          break;
        case BUTTON_7_ID: // 'Next' button
          if(Our_World->Selected < Our_World->Num_Bots-1) Our_World->Selected++;
          break;
        case BUTTON_8_ID: // 'Statistics' button
          Viewer.Display_Statistics = 1-Viewer.Display_Statistics;
          if(Viewer.Display_Statistics == TRUE)
            SetWindowPos(hViewer,0,0,0,600+5,430+25,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
          else
            SetWindowPos(hViewer,0,0,0,300+5,430+25,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
          break;
        default:
          break;
      }
      break;
	  case WM_LBUTTONDOWN:
      Move_Camera = FALSE;
      if(hwnd == Main.hWnd)
      {
        aaa = Main.View_X + 2.0*((float)LOWORD(lParam)/Main.w-0.5)/Main.View_Zoom;
        bbb = Main.View_Y - 2.0*((float)HIWORD(lParam)/Main.h-0.5)*Main.r/Main.View_Zoom;
      }
      else if(hwnd == Viewer.hWnd)
      {
        aaa = Our_World->Bots[Our_World->Selected].x + 2.0*((float)LOWORD(lParam)/Viewer.w-0.5)/Viewer.View_Zoom;
        bbb = Our_World->Bots[Our_World->Selected].y - 2.0*((float)HIWORD(lParam)/Viewer.h-0.5)*Viewer.r/Viewer.View_Zoom;
      }
      switch(Current_Tool)
      {
        case NO_TOOL:
          break;
        case PELLET_ADDER:
          Pellet_Add(Our_World, aaa, bbb);
          break;
        case BOT_ADDER:
          Bot_Add(Our_World, aaa, bbb);
          break;
        case BOT_SELECTOR:
          Our_World->Selected = Bot_Find_Closest(Our_World, aaa, bbb);
          break;
        default:
          printf("Unknown tool selected (%i)\n", Current_Tool);
          Current_Tool = NO_TOOL;
          break;
      }
      break;
	  case WM_RBUTTONDOWN:
      if(hwnd == Main.hWnd)
      {
        Move_Camera = TRUE;
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
      }
      break;
	  case WM_RBUTTONUP:
      Move_Camera = FALSE;
      break;
	  case WM_MOUSEMOVE:
      if(Move_Camera)
      {
        Main.View_X +=        2.5*(xPos - LOWORD(lParam))/Window_Width  /Main.View_Zoom;
        Main.View_Y -= Main.r*2.5*(yPos - HIWORD(lParam))/Window_Height /Main.View_Zoom;

        // Boundary check
        if(Main.View_X < 0)                       Main.View_X = 0;
        else if(Main.View_X > Our_World->Width)   Main.View_X = Our_World->Width;
        if(Main.View_Y < 0)                       Main.View_Y = 0;
        else if(Main.View_Y > Our_World->Height)  Main.View_Y = Our_World->Height;

        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
      }
      break;
	  case WM_MOUSEWHEEL:
      if((short)HIWORD(wParam) > 0)
        Main.View_Zoom *= 1.1;
      else
        Main.View_Zoom /= 1.1;
      break;
	  case WM_KEYDOWN:
      switch(wParam)
      {
        case VK_ESCAPE:
          SendMessage(hMain, WM_CLOSE, 0, 0);
          break;
        case VK_SPACE:
          SendMessage(hMain, WM_COMMAND, BUTTON_1_ID, 0);
          break;
        case 107: // +
          SendMessage(hMain, WM_MOUSEWHEEL, 120<<16, 0);
          break;
        case 109: // -
          SendMessage(hMain, WM_MOUSEWHEEL, (-120)<<16, 0);
          break;
        case 'V':
          SendMessage(hMain, WM_COMMAND, BUTTON_2_ID, 0);
          break;
        case 'Z':
          if(Bot_Dump(&Our_World->Bots[0]) == TRUE) printf("Bot 0 dumped\n");
          break;
        case 'T':
          Simulation.Delay = 0;
          break;
        case 'F':
          Simulation.Delay = 20;
          break;
        case 'S':
          Simulation.Delay = 200;
          break;
        case '1':
          Current_Tool = NO_TOOL;
          printf("Tool: NO_TOOL (%i)\n", Current_Tool);
          break;
        case '2':
          Current_Tool = PELLET_ADDER;
          printf("Tool: PELLET_ADDER (%i)\n", Current_Tool);
          break;
        case '3':
          Current_Tool = BOT_ADDER;
          printf("Tool: BOT_ADDER (%i)\n", Current_Tool);
          break;
        case '4':
          Current_Tool = BOT_SELECTOR;
          printf("Tool: BOT_SELECTOR (%i)\n", Current_Tool);
          break;
        default:
          printf("Key: %i\n", wParam);
          break;
      }
      break;
	  default:
      return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
