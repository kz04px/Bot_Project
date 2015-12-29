#include "main.h"
#include "gui.h"
#include "defs.h"

world *our_world = NULL;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{

  // gui Stuff
  gui_create(hThisInstance, &Main.hWnd, &viewer.hWnd, &Main.hstatistics, &viewer.hstatistics);

  // Show the main window
  ShowWindow(hMain, nCmdShow);

  // Controls
  printf("Keyboard:\n");
  printf(" 1   - No tool\n");
  printf(" 2   - pellet adder tool\n");
  printf(" 3   - bot adder tool\n");
  printf(" 4   - bot Selector tool\n");
  printf(" v   - Open/close viewer\n");
  printf(" sb  - Start/pause\n");
  printf(" esc - close\n\n");

  printf("Mouse:\n");
  printf(" lb  - Use tool\n");
  printf(" rb  - Drag\n");
  printf(" sw  - zoom\n\n");

  // init stuff
  our_world = (world*) malloc(1*sizeof(world));
  world_init(our_world);
  main_init(our_world, windowwidth-200, windowheight);
  viewer_init(our_world, 300, 300);
  simulation_init(our_world);
  
  // create scenario
  int i;
  for(i = 0; i < 20; ++i)
    bot_add(our_world, -1, -1);
  for(i = 0; i < 150; ++i)
    pellet_add(our_world, -1, -1);

  // Threads
  hThreads[0] = (HANDLE)_beginthread(render_Main, 0, &Main);
  hThreads[1] = (HANDLE)_beginthread(render_viewer, 0, &viewer);
  hThreads[2] = (HANDLE)_beginthread(simulate_world, 0, &simulation);
  num_threads = 3;

  MSG messages;
  while(GetMessage(&messages, NULL, 0, 0) && !bquit)
  {
    TranslateMessage(&messages);
    DispatchMessage(&messages);
  }

  // close stuff
  Main.quit = 1;
  viewer.quit = 1;
  simulation.quit = 1;
  WaitForMultipleObjects(num_threads, hThreads, 1, INFINITE);
  simulation_end(our_world);
  return 0;
}

int Move_Camera = 0;
int xPos, yPos;
float aaa, bbb;
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message)
  {
    case WM_CLOSE:
      if(hwnd == hviewer)
      {
        ShowWindow(hviewer, SW_HIDE);
        viewer.display = 0;
      }
      else
      {
        bquit = 1;
        PostQuitMessage(0);
      }
      break;
    case WM_SIZE:
      if(hwnd == hMain)
      {
        windowwidth = LOWORD(lParam);
        windowheight = HIWORD(lParam);
        if(windowwidth < windowmin_width) {windowwidth = windowmin_width;}
        if(windowheight < windowmin_height) {windowheight = windowmin_height;}
        Main.w = windowwidth-200;
        Main.h = windowheight;
        Main.r = (float)Main.h/Main.w;
        MoveWindow(Main.hWnd, 200,0, windowwidth,windowheight, 1);
      }
      break;
    case WM_COMMAND:
      SetFocus(hMain);
      switch(wParam)
      {
        case BUTTON_1_ID:
          simulation.pause = 1-simulation.pause;
          if(simulation.pause)
            SetWindowText(hpauseButton, BUTTON_1_TEXT_1);
          else
            SetWindowText(hpauseButton, BUTTON_1_TEXT_2);
          break;
        case BUTTON_2_ID:
          viewer.display = 1-viewer.display;
          if(viewer.display)
            ShowWindow(hviewer, SW_SHOW);
          else
            ShowWindow(hviewer, SW_HIDE);
          break;
        case BUTTON_3_ID:
          //printf("remove bot: %i\n", our_world->selected);
          //bot_remove(our_world, our_world->selected);

          printf("kill bot: %i\n", our_world->selected);
          bot_kill(our_world, our_world->selected);

          //our_world->selected = -1;
          if(our_world->selected >= our_world->num_bots)
            our_world->selected--;

          break;
        case BUTTON_4_ID:
          printf("scramble bot: %i\n", our_world->selected);
          bot_scramble(our_world, our_world->selected);
          bot_mutate(&our_world->bots[our_world->selected]);
          break;
        case BUTTON_5_ID:
          if(our_world->selected < 0) break;
          printf("find bot: %i\n", our_world->selected);
          Main.view_X = our_world->bots[our_world->selected].x;
          Main.view_Y = our_world->bots[our_world->selected].y;
          break;
        case BUTTON_6_ID: // 'Previous' button
          if(our_world->selected > 0) our_world->selected--;
          break;
        case BUTTON_7_ID: // 'Next' button
          if(our_world->selected < our_world->num_bots-1) our_world->selected++;
          break;
        case BUTTON_8_ID: // 'statistics' button
          viewer.display_statistics = 1-viewer.display_statistics;
          if(viewer.display_statistics)
            SetWindowPos(hviewer,0,0,0,600+5,430+25,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
          else
            SetWindowPos(hviewer,0,0,0,300+5,430+25,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
          break;
        default:
          break;
      }
      break;
    case WM_LBUTTONDOWN:
      Move_Camera = 0;
      if(hwnd == Main.hWnd)
      {
        aaa = Main.view_X + 2.0*((float)LOWORD(lParam)/Main.w-0.5)/Main.view_zoom;
        bbb = Main.view_Y - 2.0*((float)HIWORD(lParam)/Main.h-0.5)*Main.r/Main.view_zoom;
      }
      else if(hwnd == viewer.hWnd)
      {
        aaa = our_world->bots[our_world->selected].x + 2.0*((float)LOWORD(lParam)/viewer.w-0.5)/viewer.view_zoom;
        bbb = our_world->bots[our_world->selected].y - 2.0*((float)HIWORD(lParam)/viewer.h-0.5)*viewer.r/viewer.view_zoom;
      }
      switch(current_tool)
      {
        case NO_TOOL:
          break;
        case PELLET_ADDER:
          pellet_add(our_world, aaa, bbb);
          break;
        case BOT_ADDER:
          bot_add(our_world, aaa, bbb);
          break;
        case BOT_SELECTOR:
          our_world->selected = bot_find_closest(our_world, aaa, bbb);
          break;
        default:
          printf("Unknown tool selected (%i)\n", current_tool);
          current_tool = NO_TOOL;
          break;
      }
      break;
    case WM_RBUTTONDOWN:
      if(hwnd == Main.hWnd)
      {
        Move_Camera = 1;
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
      }
      break;
    case WM_RBUTTONUP:
      Move_Camera = 0;
      break;
    case WM_MOUSEMOVE:
      if(Move_Camera)
      {
        Main.view_X +=        2.5*(xPos - LOWORD(lParam))/windowwidth  /Main.view_zoom;
        Main.view_Y -= Main.r*2.5*(yPos - HIWORD(lParam))/windowheight /Main.view_zoom;

        // Boundary check
        if(Main.view_X < 0)                       Main.view_X = 0;
        else if(Main.view_X > our_world->width)   Main.view_X = our_world->width;
        if(Main.view_Y < 0)                       Main.view_Y = 0;
        else if(Main.view_Y > our_world->height)  Main.view_Y = our_world->height;

        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
      }
      break;
    case WM_MOUSEWHEEL:
      if((short)HIWORD(wParam) > 0)
        Main.view_zoom *= 1.1;
      else
        Main.view_zoom /= 1.1;
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
          if(bot_dump(&our_world->bots[0])) printf("bot 0 dumped\n");
          break;
        case 'T':
          simulation.delay = 0;
          break;
        case 'F':
          simulation.delay = 20;
          break;
        case 'S':
          simulation.delay = 200;
          break;
        case '1':
          current_tool = NO_TOOL;
          printf("Tool: NO_TOOL (%i)\n", current_tool);
          break;
        case '2':
          current_tool = PELLET_ADDER;
          printf("Tool: PELLET_ADDER (%i)\n", current_tool);
          break;
        case '3':
          current_tool = BOT_ADDER;
          printf("Tool: BOT_ADDER (%i)\n", current_tool);
          break;
        case '4':
          current_tool = BOT_SELECTOR;
          printf("Tool: BOT_SELECTOR (%i)\n", current_tool);
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
