#include <windows.h>
#include <gl/gl.h>
#include <string.h>
#include <stdbool.h>
#include "menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "texture.h"


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int textureId;
char anim=0;
float TypeOfAnim=0;
float tick=0;
int draw=0;
int move=1;
bool isEnterPressed = false;

typedef struct Hero
{
  float x, y; // Character's position
  float dx, dy; // Character's movement speed
  float size; // Character's size (e.g., width or height)
} Hero;

Hero hero;


void InitButton()
{
    ChooseBTN(0,3);
    Menu_AddButton("start",10,10,100,30,2);
    Menu_AddButton("test",10,50,100,30,2);
    Menu_AddButton("quit",10,90,100,30,2);
    Menu_AddButton("continue",10,10,100,30,2);
    Menu_AddButton("anim1",10,10,100,30,2);
    Menu_AddButton("anim2",10,50,100,30,2);
    Menu_AddButton("anim3",10,90,100,30,2);
    Menu_AddButton("back",10,130,100,30,2);
    Menu_AddButton("exit",10,420,100,30,2);
}


//typedef struct
//{
 //float x, y, //координаты нахождения в пространстве
 //dx, dy; // первая производная по координате (скорость)
//} Hero;
//void Cheek_Collision(Hero *obj){ // первое – наш
//подвижный персонаж, переменная directional – для разделения проверки по
//Х и У

   // if (obj->y<0) //если координата y в результате смещения меньше 0
    //{
     //   obj->y=0; //поменять координату y на 0
    //}
//}

void InitMain()
{
    ChooseBTN(0,3);
    anim=0;
    TypeOfAnim=0;
    tick=0;
}

InitContinue()
{
    ChooseBTN(1,4);
    anim=0;
    TypeOfAnim=0;
    tick=0;
}


// New function to handle character movement
void MoveHero(Hero *hero)
{
    if (draw && move){
  // Update character position based on speed
  hero->x += hero->dx;
  hero->y += hero->dy;

  // Implement collision detection and handling here (optional)
  //Cheek_Collision(hero);
    }
}

void Cheek_Collision(Hero *hero) {
  const float worldWidth = 1024;
  const float worldHeight = 512;

  // Horizontal collisions
  if (hero->x < 0) {
    hero->x = 0;
    hero->dx = 0.0f; // Stop movement on collision
  } else if (hero->x + hero->size > worldWidth) {
    hero->x = worldWidth - hero->size;
    hero->dx = 0.0f;
  }

  // Vertical collisions
  if (hero->y < 0) {
    hero->y = 0;
    hero->dy = 0.0f;
  } else if (hero->y + hero->size > worldHeight) {
    hero->y = worldHeight - hero->size;
    hero->dy = 0.0f;
  }
}

void DrawHero(Hero *hero, int *fframe, int totalFrames, float spriteXsize, float spriteYsize, float charsizex, float charsizey) {
    // Вычисление координат кадра
static int i;
    if (draw){
        if (isEnterPressed){
            spriteXsize=1080; //переменные с размерами текстуры и отдельного кадра
            spriteYsize=80;
            charsizey=80;
            charsizex=120;
    *fframe = (*fframe + 1) % totalFrames;
    move=0;
        }
        else if ((hero->dx == 0) &&  (hero->dy == 0)){
                move=1;
            spriteXsize=400; //переменные с размерами текстуры и отдельного кадра
            spriteYsize=80;
            charsizey=80;
            charsizex=80;
            if (i % 3 == 0) {
                    *fframe = (*fframe + 1) % 4; // Переключение кадра через раз
                }
            }
            //Sleep(500);

        else{
                move=1;
            *fframe = (*fframe + 1) % totalFrames;
        }
    int frame=*fframe;
    i++;
    float left = (charsizex * frame) / spriteXsize; // Координаты для левой границы кадра
    float right = left + (charsizex / spriteXsize); // Координаты для правой границы
    float top = 0.0f; // Верхняя граница (если спрайт-лист однорядковый)
    float bottom = 1.0f; // Нижняя граница (если спрайт-лист однорядковый)

    // Координаты вершин персонажа
    float vertex[] = {
        hero->x, hero->y, // Левый верхний угол
        hero->x + charsizex, hero->y, // Правый верхний угол
        hero->x + charsizex, hero->y + charsizey, // Правый нижний угол
        hero->x, hero->y + charsizey // Левый нижний угол
    };

    // Координаты текстуры для данного кадра
    float texCoords[] = {
        left, top,
        right, top,
        right, bottom,
        left, bottom
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (isEnterPressed){
        Texture_LoadTexture("pigeon eat.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);
    }
    else if ((hero->dx == 0) && (hero->dy == 0)){
        Texture_LoadTexture("pigeon idle.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);
        //Sleep(500);
    }
    else {
        Texture_LoadTexture("pigeon walk.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);
    }

    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);

    glColor3f(1,1,1);
    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

            glDrawArrays(GL_TRIANGLE_FAN,0,4);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    glDisable(GL_BLEND);
    }
}


void InitTest()
{
    ChooseBTN(4,8);
    tick=0;
    TypeOfAnim=0;
    anim=1;
}
void InitGame()
{
    //ResetTexture();
    ChooseBTN(8,9);
    draw=1;
    hero.size=1;
  }

void MouseDown()
{
    static int save=0;
    int buttonId = Menu_MouseDown();
    if (buttonId<0) return;
    char *name = Menu_GetButtonName(buttonId);
    if (strcmp(name,"quit")==0) PostQuitMessage(0);
    if (strcmp(name,"test")==0) InitTest();
    if (strcmp(name,"start")==0){
    InitGame();
    save=1;}
    if (strcmp(name,"continue")==0){
    InitGame();}
    if (strcmp(name,"back")==0)
    {
        ResetTexture();
        if (save==1){
        InitContinue();
        }
        else{
        InitMain();
        }
            }
    if (strcmp(name,"exit")==0)
    {
        ResetTexture();
        InitContinue();
    }


    if (strcmp(name,"anim1")==0)
    {
        TypeOfAnim=0;
        tick=0;
    }
    if (strcmp(name,"anim2")==0)
    {
        TypeOfAnim=1;
        tick=0;
    }
    if (strcmp(name,"anim3")==0)
    {
        TypeOfAnim=2;
        tick=0;
    }
}


void ResetTexture() {
    draw=0;
    // Сбрасывание матриц
    //glLoadIdentity(); // Возвращение матрицы к изначальному состоянию

    // Сбрасывание режима смешивания
    glDisable(GL_BLEND);

    // Сбрасывание привязанных текстур
    glBindTexture(GL_TEXTURE_2D, 0); // Привязка к нулевой текстуре

    // Сбрасывание других параметров, если нужно
}

void ShowTexture(float NumberOfSprite, float TypeOfAnim)
{
    if (TypeOfAnim==0)
    {
 glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Texture_LoadTexture("pigeon walk.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);
        float TextureVertex[] = {150,10,230,10,230,130,150,130};
        float TextureCord[] = {0,0,1,0,1,1,0,1};

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textureId);

        glColor3f(1,1,1);
        glPushMatrix();
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            static float spriteXsize=800; //переменные с размерами текстуры и отдельного кадра
            static float spriteYsize=80;
            static float charsizey=80;
            static float charsizex=80;
            float left=(charsizex*NumberOfSprite)/spriteXsize; //вычисление координат кадра на изображении от
            float right=left+(charsizex/spriteXsize); //номера кадра
            float top=(charsizey*TypeOfAnim)/spriteYsize;
            float bottom=top+(charsizey/spriteYsize);

            TextureCord[5]=TextureCord[7]=bottom;
            TextureCord[1]=TextureCord[3]=top;
            TextureCord[0]=TextureCord[6]=left;
            TextureCord[2]=TextureCord[4]=right;
            glVertexPointer(2,GL_FLOAT,0,TextureVertex);
            glTexCoordPointer(2,GL_FLOAT,0,TextureCord);
            glDrawArrays(GL_TRIANGLE_FAN,0,4);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glPopMatrix();
        glDisable(GL_BLEND);
    }
    if (TypeOfAnim==1)
    {
 glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Texture_LoadTexture("pigeon idle.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);
        float TextureVertex[] = {150,10,230,10,230,130,150,130};
        float TextureCord[] = {0,0,1,0,1,1,0,1};

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textureId);

        glColor3f(1,1,1);
        glPushMatrix();
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            static float spriteXsize=400; //переменные с размерами текстуры и отдельного кадра
            static float spriteYsize=80;
            static float charsizey=80;
            static float charsizex=80;
            float left=(charsizex*NumberOfSprite)/spriteXsize; //вычисление координат кадра на изображении от
            float right=left+(charsizex/spriteXsize); //номера кадра
            float top=(charsizey*TypeOfAnim)/spriteYsize;
            float bottom=top+(charsizey/spriteYsize);

            TextureCord[5]=TextureCord[7]=bottom;
            TextureCord[1]=TextureCord[3]=top;
            TextureCord[0]=TextureCord[6]=left;
            TextureCord[2]=TextureCord[4]=right;
            glVertexPointer(2,GL_FLOAT,0,TextureVertex);
            glTexCoordPointer(2,GL_FLOAT,0,TextureCord);
            glDrawArrays(GL_TRIANGLE_FAN,0,4);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glPopMatrix();
        glDisable(GL_BLEND);
    }
      if (TypeOfAnim==2)
    {
 glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Texture_LoadTexture("pigeon eat.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);
        float TextureVertex[] = {150,10,310,10,310,130,150,130};
        float TextureCord[] = {0,0,1,0,1,1,0,1};

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,textureId);

        glColor3f(1,1,1);
        glPushMatrix();
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            static float spriteXsize=1080; //переменные с размерами текстуры и отдельного кадра
            static float spriteYsize=80;
            static float charsizey=80;
            static float charsizex=120;
            float left=(charsizex*NumberOfSprite)/spriteXsize; //вычисление координат кадра на изображении от
            float right=left+(charsizex/spriteXsize); //номера кадра
            float top=(charsizey*TypeOfAnim)/spriteYsize;
            float bottom=top+(charsizey/spriteYsize);

            TextureCord[5]=TextureCord[7]=bottom;
            TextureCord[1]=TextureCord[3]=top;
            TextureCord[0]=TextureCord[6]=left;
            TextureCord[2]=TextureCord[4]=right;
            glVertexPointer(2,GL_FLOAT,0,TextureVertex);
            glTexCoordPointer(2,GL_FLOAT,0,TextureCord);
            glDrawArrays(GL_TRIANGLE_FAN,0,4);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glPopMatrix();
        glDisable(GL_BLEND);
    }
}



int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1024,
                          512,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    //Texture_LoadTexture("spriteList.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);

    RECT rct;
    GetClientRect(hwnd, &rct);
    glOrtho(0, rct.right, rct.bottom, 0, 1,-1);

    InitButton();


    float spriteXsize = 800.0f;
    float spriteYsize = 80.0f;
    float charsizex = 80.0f;
    float charsizey = 80.0f;
    int fframe = 0; // Текущий кадр анимации
    int totalFrames = 7; // Общее количество кадров на спрайт-листе


    /* program main loop */
    while (!bQuit) {
  // Handle messages
  if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      bQuit = TRUE;
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  } else {
    // Game loop logic

    // Handle user input (assuming this happens in MouseDown)
    //MouseDown();

    // Update game state (including character movement)
    MoveHero(&hero); // Call MoveHero to update character position

    // Clear screen
    glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

     DrawHero(&hero, &fframe, totalFrames, spriteXsize, spriteYsize, charsizex, charsizey);

    //frame = (frame + 1) % totalFrames; // Переключение на следующий кадр
    // Draw menu
    Menu_ShowMenu();

    // Draw character (assuming ShowTexture handles character rendering)
    if (anim) {
      ShowTexture(tick, TypeOfAnim);
    }
    // Swap buffers
    SwapBuffers(hDC);

    // Update animation tick
    tick++;
    if (tick > 8) {
      tick = 0;
    }

    // Short delay (consider alternative timing mechanisms)
    Sleep(100);
  }
}
    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_MOUSEMOVE:
            Menu_MouseMove(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_LBUTTONDOWN:
            MouseDown();
            break;

        case WM_LBUTTONUP:
            Menu_MouseUp();
            break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
                case VK_LEFT:
                    hero.dx = -10.0f; // Движение влево
                    break;
                case VK_RIGHT:
                    hero.dx = 10.0f; // Движение вправо
                    break;
                case VK_UP:
                    hero.dy = -10.0f; // Движение вверх
                    break;
                case VK_DOWN:
                    hero.dy = 10.0f; // Движение вниз
                    break;
                case VK_RETURN:
                     isEnterPressed = true;
                    break;
                default:
                    break;
            }
            break;
        }

        case WM_KEYUP: {
             switch (wParam) {
                case VK_RETURN: // Клавиша Enter отпущена
                    isEnterPressed = false;
                    break;}

            if (wParam == VK_LEFT || wParam == VK_RIGHT) {
                hero.dx = 0.0f;
            }
            if (wParam == VK_UP || wParam == VK_DOWN) {
                hero.dy = 0.0f;
            }
            break;

        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

