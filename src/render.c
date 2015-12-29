#include "defs.h"

void draw_Background(world *our_world);
void draw_Test(world *our_world);
void draw_bots(world *our_world);
void draw_pellets(world *our_world);
void PrintText(world *our_world, int x, int y, int w, int h, const char *String);
void EnableOpenGL(HWND, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int main_init(world *our_world, int w, int h)
{
  Main.w = w;
  Main.h = h;
  Main.r = (float)Main.h/Main.w;
  Main.view_zoom = 0.2;
  Main.world = our_world;
  Main.view_X = 0.5*our_world->width;
  Main.view_Y = 0.5*our_world->height;
  Main.display = 1;
  Main.display_statistics = 1;
  Main.quit = 0;
  return 0;
}

int viewer_init(world *our_world, int w, int h)
{
  viewer.w = w;
  viewer.h = h;
  viewer.r = (float)viewer.h/viewer.w;
  viewer.view_X = 0.0;
  viewer.view_Y = 0.0;
  viewer.view_zoom = 0.5;
  viewer.world = our_world;
  viewer.display = 0;
  viewer.display_statistics = 0;
  viewer.quit = 0;
  return 0;
}

void render_viewer(void* a)
{
  char String[256];
  char Temp[128];
  EnableOpenGL(viewer.hWnd, &viewer.hDC, &viewer.hRC);
  glClearColor(0.0, 0.0, 0.0, 0.0); // Black

  while(!viewer.quit)
  {
    if(viewer.display)
    {
      glViewport(0, 0, viewer.w, viewer.h);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      // draw viewing area
      if(viewer.world->selected >= 0)
      {
        glOrtho(-1.0/viewer.view_zoom, 1.0/viewer.view_zoom,
            -viewer.r/viewer.view_zoom, viewer.r/viewer.view_zoom,
            -1, 1);
        glTranslatef(-viewer.world->bots[viewer.world->selected].x, -viewer.world->bots[viewer.world->selected].y, 1.0);

        draw_Background(viewer.world);
        draw_pellets(viewer.world);
        draw_bots(viewer.world);
        glPopMatrix();

        static int count = 0;
        if(viewer.display_statistics && count%30 == 0)
        {
          int i;
          // display inputs
          sprintf(String, " Brain inputs:\r\n");
          for(i = 0; i < viewer.world->bots[viewer.world->selected].nn.layer_sizes[0]; ++i)
          {
            sprintf(Temp, "  %i: %.4g\r\n", i, viewer.world->bots[viewer.world->selected].nn.input[0][i]);
            strcat(String, Temp);
          }
          // display outputs
          sprintf(Temp, " Brain outputs:\r\n");
          strcat(String, Temp);
          for(i = 0; i < viewer.world->bots[viewer.world->selected].nn.layer_sizes[2]; ++i)
          {
            sprintf(Temp, "  %i: %.4g\r\n", i, viewer.world->bots[viewer.world->selected].nn.output[2][i]);
            strcat(String, Temp);
          }
          /*
          sprintf(String, " bot: %i\r\n", viewer.world->selected);
          sprintf(Temp, " age: %i\r\n", viewer.world->bots[viewer.world->selected].age);
          strcat(String, Temp);
          sprintf(Temp, " energy: %i\r\n", viewer.world->bots[viewer.world->selected].energy);
          strcat(String, Temp);
          sprintf(Temp, " dead: %i\r\n", viewer.world->bots[viewer.world->selected].dead);
          strcat(String, Temp);

          // position & rotation
          sprintf(Temp, " x: %i\r\n", (int)viewer.world->bots[viewer.world->selected].x);
          strcat(String, Temp);
          sprintf(Temp, " y: %i\r\n", (int)viewer.world->bots[viewer.world->selected].y);
          strcat(String, Temp);
          sprintf(Temp, " r: %i\r\n", (int)viewer.world->bots[viewer.world->selected].r);
          strcat(String, Temp);

          // eyes
          sprintf(Temp, " eyes: %i\r\n", viewer.world->bots[viewer.world->selected].num_eyes);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", viewer.world->bots[viewer.world->selected].eyes[0].in_strength,
                                                     viewer.world->bots[viewer.world->selected].eyes[0].in_red,
                                                     viewer.world->bots[viewer.world->selected].eyes[0].in_green,
                                                     viewer.world->bots[viewer.world->selected].eyes[0].in_blue);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", viewer.world->bots[viewer.world->selected].eyes[1].in_strength,
                                                     viewer.world->bots[viewer.world->selected].eyes[1].in_red,
                                                     viewer.world->bots[viewer.world->selected].eyes[1].in_green,
                                                     viewer.world->bots[viewer.world->selected].eyes[1].in_blue);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", viewer.world->bots[viewer.world->selected].eyes[2].in_strength,
                                                     viewer.world->bots[viewer.world->selected].eyes[2].in_red,
                                                     viewer.world->bots[viewer.world->selected].eyes[2].in_green,
                                                     viewer.world->bots[viewer.world->selected].eyes[2].in_blue);
          strcat(String, Temp);

          // Body colour
          sprintf(Temp, " Colour:\r\n");
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", viewer.world->bots[viewer.world->selected].red);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", viewer.world->bots[viewer.world->selected].green);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", viewer.world->bots[viewer.world->selected].blue);
          strcat(String, Temp);
          */

          SetWindowText(viewer.hstatistics, String);
        }
        count++;
      }
      SwapBuffers(viewer.hDC);
    }
    Sleep(20);
  }
  DisableOpenGL(viewer.hWnd, viewer.hDC, viewer.hRC);
}

void render_Main(void* a)
{
  char String[256];
  char Temp[128];
  EnableOpenGL(Main.hWnd, &Main.hDC, &Main.hRC);
  glClearColor(0.0, 0.0, 0.0, 0.0); // Black
  glViewport(0, 0, Main.w, Main.h);

  while(!Main.quit)
  {
    glViewport(0, 0, Main.w, Main.h);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrtho(-1.0/Main.view_zoom, 1.0/Main.view_zoom,
        -Main.r/Main.view_zoom, Main.r/Main.view_zoom,
        -1, 1);
    glTranslatef(-Main.view_X, -Main.view_Y, 1.0);

    draw_Background(Main.world);
    draw_pellets(Main.world);
    draw_bots(Main.world);
    //draw_edge_bots(viewer.world);

    SwapBuffers(Main.hDC);

    static int count = 0;
    if(Main.display_statistics && count%30 == 0)
    {
      sprintf(String, " generation: %i\r\n", Main.world->generation);
      sprintf(Temp, " frame: %i\r\n", Main.world->frame);
      strcat(String, Temp);
      sprintf(Temp, " world width: %i\r\n", Main.world->width);
      strcat(String, Temp);
      sprintf(Temp, " world height: %i\r\n\r\n", Main.world->height);
      strcat(String, Temp);

      sprintf(Temp, " bots: %i\r\n", Main.world->num_bots);
      strcat(String, Temp);
      sprintf(Temp, " Most bots: %i\r\n", Main.world->bots_most);
      strcat(String, Temp);
      sprintf(Temp, " max bots: %i\r\n", Main.world->max_bots);
      strcat(String, Temp);
      sprintf(Temp, " bots added: %i\r\n", Main.world->bots_added);
      strcat(String, Temp);
      sprintf(Temp, " bots removed: %i\r\n\r\n", Main.world->bots_removed);
      strcat(String, Temp);

      sprintf(Temp, " pellets: %i\r\n", Main.world->num_pellets);
      strcat(String, Temp);
      sprintf(Temp, " max pellets: %i\r\n", Main.world->max_pellets);
      strcat(String, Temp);
      SetWindowText(Main.hstatistics, String);
    }
    count++;
    Sleep(20);
  }
  DisableOpenGL(Main.hWnd, Main.hDC, Main.hRC);
}

void draw_Background(world* our_world)
{
  glPushMatrix();
  glColor3f(0.0, 0.6, 0.0); // green
  glScalef(our_world->width, our_world->height, 1.0);
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
