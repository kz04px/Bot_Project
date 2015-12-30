#include "main.h"
#include "gui.h"
#include "defs.h"

s_world *world = NULL;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
  // gui stuff
  gui_create(hThisInstance, &main_display.hWnd, &viewer_display.hWnd, &main_display.hstatistics, &viewer_display.hstatistics);

  // Show the main window
  ShowWindow(hMain, nCmdShow);
  
  // Show the console
  AllocConsole();
  freopen("CONIN$", "r", stdin);
  freopen("CONOUT$", "w", stdout);
  freopen("CONOUT$", "w", stderr);

  // Controls
  printf("Keyboard:\n");
  printf(" 1   - No tool\n");
  printf(" 2   - Pellet adder tool\n");
  printf(" 3   - Bot adder tool\n");
  printf(" 4   - Bot selector tool\n");
  printf(" v   - Open/Close viewer\n");
  printf(" sb  - Start/Pause\n");
  printf(" esc - Close\n");
  printf("\n");

  printf("Mouse:\n");
  printf(" lb  - Use tool\n");
  printf(" rb  - Drag\n");
  printf(" sw  - Zoom\n");
  printf("\n");

  // init stuff
  world = (s_world *)malloc(1*sizeof(s_world));
  world_init(world);
  main_init(world, window_width-200, window_height);
  viewer_init(world, 300, 300);
  simulation_init(world);
  
  // create scenario
  int i;
  for(i = 0; i < 20; ++i)
  {
    bot_add(world, -1, -1);
  }
  for(i = 0; i < 150; ++i)
  {
    pellet_add(world, -1, -1);
  }

  // perft
  int r;
  int num_frames = 50000;
  double time_taken;
  time_t start = clock();
  for(r = 1; r <= num_frames; ++r)
  {
    simulate_frame(simulation.world);
  }
  time_taken = ((double)clock()-start)/CLOCKS_PER_SEC;
  printf("\n");
  printf("Results:\n");
  printf("Frames: %i\n", num_frames);
  printf("Total time: %.3fs\n", time_taken);
  printf("Time per frame: %.3fns\n", (1000000.0)*time_taken/num_frames);
  printf("\n");
  //getchar();
  //return 0;
  
  // Threads
  hThreads[0] = (HANDLE)_beginthread(render_main, 0, &main_display);
  hThreads[1] = (HANDLE)_beginthread(render_viewer, 0, &viewer_display);
  hThreads[2] = (HANDLE)_beginthread(simulate_world, 0, &simulation);
  num_threads = 3;

  MSG messages;
  while(GetMessage(&messages, NULL, 0, 0) && !bquit)
  {
    TranslateMessage(&messages);
    DispatchMessage(&messages);
  }

  // close stuff
  main_display.quit = 1;
  viewer_display.quit = 1;
  simulation.quit = 1;
  WaitForMultipleObjects(num_threads, hThreads, 1, INFINITE);
  simulation_end(world);
  return 0;
}

int move_camera = 0;
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
        viewer_display.display = 0;
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
        window_width = LOWORD(lParam);
        window_height = HIWORD(lParam);
        if(window_width < window_min_width) {window_width = window_min_width;}
        if(window_height < window_min_height) {window_height = window_min_height;}
        main_display.w = window_width-200;
        main_display.h = window_height;
        main_display.r = (float)main_display.h/main_display.w;
        MoveWindow(main_display.hWnd, 200,0, window_width,window_height, 1);
      }
      break;
    case WM_COMMAND:
      SetFocus(hMain);
      switch(wParam)
      {
        case BUTTON_1_ID:
          simulation.pause = 1-simulation.pause;
          if(simulation.pause)
          {
            SetWindowText(hpauseButton, BUTTON_1_TEXT_1);
          }
          else
          {
            SetWindowText(hpauseButton, BUTTON_1_TEXT_2);
          }
          break;
        case BUTTON_2_ID:
          viewer_display.display = 1-viewer_display.display;
          if(viewer_display.display)
          {
            ShowWindow(hViewer, SW_SHOW);
          }
          else
          {
            ShowWindow(hViewer, SW_HIDE);
          }
          break;
        case BUTTON_3_ID:
          //printf("remove s_bot: %i\n", world->selected);
          //bot_remove(world, world->selected);

          printf("kill bot: %i\n", world->selected);
          bot_kill(world, world->selected);

          //world->selected = -1;
          if(world->selected >= world->num_bots)
          {
            world->selected--;
          }

          break;
        case BUTTON_4_ID:
          printf("scramble bot: %i\n", world->selected);
          bot_scramble(world, world->selected);
          bot_mutate(&world->bots[world->selected]);
          break;
        case BUTTON_5_ID:
          if(world->selected < 0) break;
          printf("find bot: %i\n", world->selected);
          main_display.view_X = world->bots[world->selected].x;
          main_display.view_Y = world->bots[world->selected].y;
          break;
        case BUTTON_6_ID: // 'Previous' button
          if(world->selected > 0) world->selected--;
          break;
        case BUTTON_7_ID: // 'Next' button
          if(world->selected < world->num_bots-1) world->selected++;
          break;
        case BUTTON_8_ID: // 'statistics' button
          viewer_display.display_statistics = 1-viewer_display.display_statistics;
          if(viewer_display.display_statistics)
            SetWindowPos(hViewer,0,0,0,600+5,430+25,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
          else
            SetWindowPos(hViewer,0,0,0,300+5,430+25,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
          break;
        default:
          break;
      }
      break;
    case WM_LBUTTONDOWN:
      move_camera = 0;
      if(hwnd == main_display.hWnd)
      {
        aaa = main_display.view_X + 2.0*((float)LOWORD(lParam)/main_display.w-0.5)/main_display.view_zoom;
        bbb = main_display.view_Y - 2.0*((float)HIWORD(lParam)/main_display.h-0.5)*main_display.r/main_display.view_zoom;
      }
      else if(hwnd == viewer_display.hWnd)
      {
        aaa = world->bots[world->selected].x + 2.0*((float)LOWORD(lParam)/viewer_display.w-0.5)/viewer_display.view_zoom;
        bbb = world->bots[world->selected].y - 2.0*((float)HIWORD(lParam)/viewer_display.h-0.5)*viewer_display.r/viewer_display.view_zoom;
      }
      switch(current_tool)
      {
        case NO_TOOL:
          break;
        case PELLET_ADDER:
          pellet_add(world, aaa, bbb);
          break;
        case BOT_ADDER:
          bot_add(world, aaa, bbb);
          break;
        case BOT_SELECTOR:
          world->selected = bot_find_closest(world, aaa, bbb);
          break;
        default:
          printf("Unknown tool selected (%i)\n", current_tool);
          current_tool = NO_TOOL;
          break;
      }
      break;
    case WM_RBUTTONDOWN:
      if(hwnd == main_display.hWnd)
      {
        move_camera = 1;
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
      }
      break;
    case WM_RBUTTONUP:
      move_camera = 0;
      break;
    case WM_MOUSEMOVE:
      if(move_camera)
      {
        main_display.view_X +=        2.5*(xPos - LOWORD(lParam))/window_width  /main_display.view_zoom;
        main_display.view_Y -= main_display.r*2.5*(yPos - HIWORD(lParam))/window_height /main_display.view_zoom;

        // Boundary check
             if(main_display.view_X < 0)             {main_display.view_X = 0;}
        else if(main_display.view_X > world->width)  {main_display.view_X = world->width;}
             if(main_display.view_Y < 0)             {main_display.view_Y = 0;}
        else if(main_display.view_Y > world->height) {main_display.view_Y = world->height;}

        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
      }
      break;
    case WM_MOUSEWHEEL:
      if((short)HIWORD(wParam) > 0)
      {
        main_display.view_zoom *= 1.1;
      }
      else
      {
        main_display.view_zoom /= 1.1;
      }
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
          if(bot_dump(&world->bots[world->selected])) 
          {
            printf("Bot %i dumped\n", world->selected);
          }
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
          break;
      }
      break;
    default:
      return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}
