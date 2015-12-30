#include "defs.h"

void draw_Background(s_world *world);
void draw_Test(s_world *world);
void draw_bots(s_world *world);
void draw_pellets(s_world *world);
void PrintText(s_world *world, int x, int y, int w, int h, const char *String);
void EnableOpenGL(HWND, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int main_init(s_world *world, int w, int h)
{
  main_display.w = w;
  main_display.h = h;
  main_display.r = (float)main_display.h/main_display.w;
  main_display.view_zoom = 0.2;
  main_display.world = world;
  main_display.view_X = 0.5*world->width;
  main_display.view_Y = 0.5*world->height;
  main_display.display = 1;
  main_display.display_statistics = 1;
  main_display.quit = 0;
  return 0;
}

int viewer_init(s_world *world, int w, int h)
{
  ASSERT(world != NULL);
  
  viewer_display.w = w;
  viewer_display.h = h;
  viewer_display.r = (float)viewer_display.h/viewer_display.w;
  viewer_display.view_X = 0.0;
  viewer_display.view_Y = 0.0;
  viewer_display.view_zoom = 0.5;
  viewer_display.world = world;
  viewer_display.display = 0;
  viewer_display.display_statistics = 0;
  viewer_display.quit = 0;
  return 0;
}

void render_viewer(void* a)
{
  char String[256];
  char Temp[128];
  EnableOpenGL(viewer_display.hWnd, &viewer_display.hDC, &viewer_display.hRC);
  glClearColor(0.0, 0.0, 0.0, 0.0); // Black

  while(!viewer_display.quit)
  {
    if(viewer_display.display)
    {
      glViewport(0, 0, viewer_display.w, viewer_display.h);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      // draw viewing area
      if(viewer_display.world->selected >= 0)
      {
        glOrtho(-1.0/viewer_display.view_zoom, 1.0/viewer_display.view_zoom,
            -viewer_display.r/viewer_display.view_zoom, viewer_display.r/viewer_display.view_zoom,
            -1, 1);
        glTranslatef(-viewer_display.world->bots[viewer_display.world->selected].x, -viewer_display.world->bots[viewer_display.world->selected].y, 1.0);

        draw_Background(viewer_display.world);
        draw_pellets(viewer_display.world);
        draw_bots(viewer_display.world);
        glPopMatrix();

        static int count = 0;
        if(viewer_display.display_statistics && count%30 == 0)
        {
          int i;
          // display inputs
          sprintf(String, " Brain inputs:\r\n");
          for(i = 0; i < viewer_display.world->bots[viewer_display.world->selected].nn.layer_sizes[0]; ++i)
          {
            sprintf(Temp, "  %i: %.4g\r\n", i, viewer_display.world->bots[viewer_display.world->selected].nn.input[0][i]);
            strcat(String, Temp);
          }
          // display outputs
          sprintf(Temp, " Brain outputs:\r\n");
          strcat(String, Temp);
          for(i = 0; i < viewer_display.world->bots[viewer_display.world->selected].nn.layer_sizes[2]; ++i)
          {
            sprintf(Temp, "  %i: %.4g\r\n", i, viewer_display.world->bots[viewer_display.world->selected].nn.output[2][i]);
            strcat(String, Temp);
          }
          /*
          sprintf(String, " s_bot: %i\r\n", viewer_display.world->selected);
          sprintf(Temp, " age: %i\r\n", viewer_display.world->bots[viewer_display.world->selected].age);
          strcat(String, Temp);
          sprintf(Temp, " energy: %i\r\n", viewer_display.world->bots[viewer_display.world->selected].energy);
          strcat(String, Temp);
          sprintf(Temp, " dead: %i\r\n", viewer_display.world->bots[viewer_display.world->selected].dead);
          strcat(String, Temp);

          // position & rotation
          sprintf(Temp, " x: %i\r\n", (int)viewer_display.world->bots[viewer_display.world->selected].x);
          strcat(String, Temp);
          sprintf(Temp, " y: %i\r\n", (int)viewer_display.world->bots[viewer_display.world->selected].y);
          strcat(String, Temp);
          sprintf(Temp, " r: %i\r\n", (int)viewer_display.world->bots[viewer_display.world->selected].r);
          strcat(String, Temp);

          // eyes
          sprintf(Temp, " eyes: %i\r\n", viewer_display.world->bots[viewer_display.world->selected].num_eyes);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", viewer_display.world->bots[viewer_display.world->selected].eyes[0].in_strength,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[0].in_red,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[0].in_green,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[0].in_blue);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", viewer_display.world->bots[viewer_display.world->selected].eyes[1].in_strength,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[1].in_red,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[1].in_green,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[1].in_blue);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", viewer_display.world->bots[viewer_display.world->selected].eyes[2].in_strength,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[2].in_red,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[2].in_green,
                                                     viewer_display.world->bots[viewer_display.world->selected].eyes[2].in_blue);
          strcat(String, Temp);

          // Body colour
          sprintf(Temp, " Colour:\r\n");
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", viewer_display.world->bots[viewer_display.world->selected].red);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", viewer_display.world->bots[viewer_display.world->selected].green);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", viewer_display.world->bots[viewer_display.world->selected].blue);
          strcat(String, Temp);
          */

          SetWindowText(viewer_display.hstatistics, String);
        }
        count++;
      }
      SwapBuffers(viewer_display.hDC);
    }
    Sleep(20);
  }
  DisableOpenGL(viewer_display.hWnd, viewer_display.hDC, viewer_display.hRC);
}

void render_main(void* a)
{
  char String[256];
  char Temp[128];
  EnableOpenGL(main_display.hWnd, &main_display.hDC, &main_display.hRC);
  glClearColor(0.0, 0.0, 0.0, 0.0); // Black
  glViewport(0, 0, main_display.w, main_display.h);

  while(!main_display.quit)
  {
    glViewport(0, 0, main_display.w, main_display.h);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrtho(-1.0/main_display.view_zoom, 1.0/main_display.view_zoom,
        -main_display.r/main_display.view_zoom, main_display.r/main_display.view_zoom,
        -1, 1);
    glTranslatef(-main_display.view_X, -main_display.view_Y, 1.0);

    draw_Background(main_display.world);
    draw_pellets(main_display.world);
    draw_bots(main_display.world);
    draw_edge_bots(viewer_display.world);

    SwapBuffers(main_display.hDC);

    static int count = 0;
    if(main_display.display_statistics && count%30 == 0)
    {
      sprintf(String, " Generation: %i\r\n", main_display.world->generation);
      sprintf(Temp, " Frame: %i\r\n", main_display.world->frame);
      strcat(String, Temp);
      sprintf(Temp, " World width: %i\r\n", main_display.world->width);
      strcat(String, Temp);
      sprintf(Temp, " World height: %i\r\n", main_display.world->height);
      strcat(String, Temp);
      sprintf(Temp, " Average fitness: %i\r\n\r\n", main_display.world->average_fitness);
      strcat(String, Temp);

      sprintf(Temp, " Bots: %i\r\n", main_display.world->num_bots);
      strcat(String, Temp);
      sprintf(Temp, " Most bots: %i\r\n", main_display.world->bots_most);
      strcat(String, Temp);
      sprintf(Temp, " Max bots: %i\r\n", main_display.world->max_bots);
      strcat(String, Temp);
      sprintf(Temp, " Bots added: %i\r\n", main_display.world->bots_added);
      strcat(String, Temp);
      sprintf(Temp, " Bots removed: %i\r\n\r\n", main_display.world->bots_removed);
      strcat(String, Temp);

      sprintf(Temp, " Pellets: %i\r\n", main_display.world->num_pellets);
      strcat(String, Temp);
      sprintf(Temp, " Max pellets: %i\r\n", main_display.world->max_pellets);
      strcat(String, Temp);
      SetWindowText(main_display.hstatistics, String);
    }
    count++;
    Sleep(20);
  }
  DisableOpenGL(main_display.hWnd, main_display.hDC, main_display.hRC);
}

void draw_Background(s_world *world)
{
  ASSERT(world != NULL);
  
  glPushMatrix();
  glColor3f(0.0, 0.6, 0.0); // green
  glScalef(world->width, world->height, 1.0);
  glBegin(GL_QUADS);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, 1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, 0.0);
  glEnd();
  glPopMatrix();
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
  ASSERT(hDC != NULL);
  ASSERT(hRC != NULL);
  
  PIXELFORMATDESCRIPTOR pfd;
  int iFormat;
  *hDC = GetDC(hwnd);
  ZeroMemory(&pfd, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;
  iFormat = ChoosePixelFormat(*hDC, &pfd);
  SetPixelFormat(*hDC, iFormat, &pfd);
  *hRC = wglCreateContext(*hDC);
  wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hwnd, hDC);
}
