#include "defs.h"

void Draw_Background(world *Our_World);
void Draw_Test(world *Our_World);
void Draw_Bots(world *Our_World);
void Draw_Pellets(world *Our_World);
void PrintText(world *Our_World, int x, int y, int w, int h, const char *String);
void EnableOpenGL(HWND, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int Main_Init(world *Our_World, int w, int h)
{
  Main.w = w;
  Main.h = h;
  Main.r = (float)Main.h/Main.w;
  Main.View_Zoom = 0.2;
  Main.World = Our_World;
  Main.View_X = 0.5*Our_World->Width;
  Main.View_Y = 0.5*Our_World->Height;
  Main.Display = TRUE;
  Main.Display_Statistics = TRUE;
  Main.Quit = FALSE;
  return TRUE;
}

int Viewer_Init(world *Our_World, int w, int h)
{
  Viewer.w = w;
  Viewer.h = h;
  Viewer.r = (float)Viewer.h/Viewer.w;
  Viewer.View_X = 0.0;
  Viewer.View_Y = 0.0;
  Viewer.View_Zoom = 0.5;
  Viewer.World = Our_World;
  Viewer.Display = FALSE;
  Viewer.Display_Statistics = FALSE;
  Viewer.Quit = FALSE;
  return TRUE;
}

void Render_Viewer(void* a)
{
  char String[256];
  char Temp[128];
	EnableOpenGL(Viewer.hWnd, &Viewer.hDC, &Viewer.hRC);
	glClearColor(0.0, 0.0, 0.0, 0.0); // Black

  while(Viewer.Quit == FALSE)
  {
    if(Viewer.Display == TRUE)
    {
      glViewport(0, 0, Viewer.w, Viewer.h);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      // Draw viewing area
      if(Viewer.World->Selected >= 0)
      {
        glOrtho(-1.0/Viewer.View_Zoom, 1.0/Viewer.View_Zoom,
            -Viewer.r/Viewer.View_Zoom, Viewer.r/Viewer.View_Zoom,
            -1, 1);
        glTranslatef(-Viewer.World->Bots[Viewer.World->Selected].x, -Viewer.World->Bots[Viewer.World->Selected].y, 1.0);

        Draw_Background(Viewer.World);
        Draw_Pellets(Viewer.World);
        Draw_Bots(Viewer.World);
        glPopMatrix();

        static int count = 0;
        if(Viewer.Display_Statistics == TRUE && count%30 == 0)
        {
          int i;
          // Display inputs
          sprintf(String, " Brain Inputs:\r\n");
          for(i = 0; i < Viewer.World->Bots[Viewer.World->Selected].NN.Layer_Sizes[0]; ++i)
          {
            sprintf(Temp, "  %i: %.4g\r\n", i, Viewer.World->Bots[Viewer.World->Selected].NN.Input[0][i]);
            strcat(String, Temp);
          }
          // Display outputs
          sprintf(Temp, " Brain Outputs:\r\n");
          strcat(String, Temp);
          for(i = 0; i < Viewer.World->Bots[Viewer.World->Selected].NN.Layer_Sizes[2]; ++i)
          {
            sprintf(Temp, "  %i: %.4g\r\n", i, Viewer.World->Bots[Viewer.World->Selected].NN.Output[2][i]);
            strcat(String, Temp);
          }
          /*
          sprintf(String, " Bot: %i\r\n", Viewer.World->Selected);
          sprintf(Temp, " Age: %i\r\n", Viewer.World->Bots[Viewer.World->Selected].Age);
          strcat(String, Temp);
          sprintf(Temp, " Energy: %i\r\n", Viewer.World->Bots[Viewer.World->Selected].Energy);
          strcat(String, Temp);
          sprintf(Temp, " Dead: %i\r\n", Viewer.World->Bots[Viewer.World->Selected].Dead);
          strcat(String, Temp);

          // Position & rotation
          sprintf(Temp, " x: %i\r\n", (int)Viewer.World->Bots[Viewer.World->Selected].x);
          strcat(String, Temp);
          sprintf(Temp, " y: %i\r\n", (int)Viewer.World->Bots[Viewer.World->Selected].y);
          strcat(String, Temp);
          sprintf(Temp, " r: %i\r\n", (int)Viewer.World->Bots[Viewer.World->Selected].r);
          strcat(String, Temp);

          // Eyes
          sprintf(Temp, " Eyes: %i\r\n", Viewer.World->Bots[Viewer.World->Selected].Num_Eyes);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", Viewer.World->Bots[Viewer.World->Selected].Eyes[0].In_Strength,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[0].In_Red,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[0].In_Green,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[0].In_Blue);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", Viewer.World->Bots[Viewer.World->Selected].Eyes[1].In_Strength,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[1].In_Red,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[1].In_Green,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[1].In_Blue);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g %.4g %.4g %.4g\r\n", Viewer.World->Bots[Viewer.World->Selected].Eyes[2].In_Strength,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[2].In_Red,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[2].In_Green,
                                                     Viewer.World->Bots[Viewer.World->Selected].Eyes[2].In_Blue);
          strcat(String, Temp);

          // Body colour
          sprintf(Temp, " Colour:\r\n");
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", Viewer.World->Bots[Viewer.World->Selected].Red);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", Viewer.World->Bots[Viewer.World->Selected].Green);
          strcat(String, Temp);
          sprintf(Temp, "  %.4g\r\n", Viewer.World->Bots[Viewer.World->Selected].Blue);
          strcat(String, Temp);
          */

          SetWindowText(Viewer.hStatistics, String);
        }
        count++;
      }
      SwapBuffers(Viewer.hDC);
    }
    Sleep(20);
  }
	DisableOpenGL(Viewer.hWnd, Viewer.hDC, Viewer.hRC);
}

void Render_Main(void* a)
{
  char String[256];
  char Temp[128];
	EnableOpenGL(Main.hWnd, &Main.hDC, &Main.hRC);
	glClearColor(0.0, 0.0, 0.0, 0.0); // Black
  glViewport(0, 0, Main.w, Main.h);

  while(Main.Quit == FALSE)
  {
    glViewport(0, 0, Main.w, Main.h);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrtho(-1.0/Main.View_Zoom, 1.0/Main.View_Zoom,
        -Main.r/Main.View_Zoom, Main.r/Main.View_Zoom,
        -1, 1);
    glTranslatef(-Main.View_X, -Main.View_Y, 1.0);

    Draw_Background(Main.World);
    Draw_Pellets(Main.World);
    Draw_Bots(Main.World);
    //Draw_Edge_Bots(Viewer.World);

    SwapBuffers(Main.hDC);

    static int count = 0;
    if(Main.Display_Statistics == TRUE && count%30 == 0)
    {
      sprintf(String, " Generation: %i\r\n", Main.World->Generation);
      sprintf(Temp, " Frame: %i\r\n", Main.World->Frame);
      strcat(String, Temp);
      sprintf(Temp, " World Width: %i\r\n", Main.World->Width);
      strcat(String, Temp);
      sprintf(Temp, " World Height: %i\r\n\r\n", Main.World->Height);
      strcat(String, Temp);

      sprintf(Temp, " Bots: %i\r\n", Main.World->Num_Bots);
      strcat(String, Temp);
      sprintf(Temp, " Most Bots: %i\r\n", Main.World->Bots_Most);
      strcat(String, Temp);
      sprintf(Temp, " Max Bots: %i\r\n", Main.World->Max_Bots);
      strcat(String, Temp);
      sprintf(Temp, " Bots added: %i\r\n", Main.World->Bots_Added);
      strcat(String, Temp);
      sprintf(Temp, " Bots removed: %i\r\n\r\n", Main.World->Bots_Removed);
      strcat(String, Temp);

      sprintf(Temp, " Pellets: %i\r\n", Main.World->Num_Pellets);
      strcat(String, Temp);
      sprintf(Temp, " Max Pellets: %i\r\n", Main.World->Max_Pellets);
      strcat(String, Temp);
      SetWindowText(Main.hStatistics, String);
    }
    count++;
    Sleep(20);
  }
	DisableOpenGL(Main.hWnd, Main.hDC, Main.hRC);
}

void Draw_Background(world* Our_World)
{
	glPushMatrix();
	glColor3f(0.0, 0.6, 0.0); // Green
	glScalef(Our_World->Width, Our_World->Height, 1.0);
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
