// This program creates a simple OpenGL window and draws a blue circle in the center of the window.
// It uses the Windows API for window management and OpenGL for rendering.
// Compile with: g++ -o circle.exe circle.cpp -lopengl32 -lgdi32
// Note: Make sure you have the OpenGL and GDI32 libraries available in your compiler's library path.
#include <windows.h>
#include <GL/gl.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define SEGMENTS 100

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_CLOSE) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void drawCircle(float cx, float cy, float r, int segments) {
    glColor3f(0.0f, 0.0f, 1.0f); // blue
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // center
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = cx + cosf(angle) * r;
        float y = cy + sinf(angle) * r;
        glVertex2f(x, y);
    }
    glEnd();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BareOpenGLWindowClass";

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, "Blue Circle", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
        NULL, NULL, hInstance, NULL);

    HDC hDC = GetDC(hWnd);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 32,
        0, 0, 0, 0, 0, 0,
        0, 0,
        0, 0, 0, 0, 0,
        24, 8, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pf = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pf, &pfd);

    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hWnd, nCmdShow);

    // OpenGL setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white background
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircle(0.0f, 0.0f, 0.5f, SEGMENTS);

        SwapBuffers(hDC);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);

    return 0;
}