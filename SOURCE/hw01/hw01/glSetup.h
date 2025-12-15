#ifndef __GL_SETUP_H_
#define __GL_SETUP_H_

#include <Windows.h>
#include <gl/glu.h>

#include <GLFW/glfw3.h>

extern float screenScale;		// 전체 모니터에서 차지할 비율
extern int screenW, screenH;	// 모니터 해상도
extern int windowW, windowH;	// 프레임 버퍼 사이즈
extern float aspect;			// windowW/windowH
extern float dpiScaling;

extern int vsync;				// 수직 동기화 (그래픽 카드의 프레임 생성과 모니터의 프레임 출력 타이밍을 맞추도록 하는 설정)

extern bool perspectiveView;
extern float fovy;				// Field of view in the y direction
extern float nearDist;
extern float farDist;

GLFWwindow* initializeOpenGL(int argc, char* argv[], GLfloat bf[4]);
void reshape(GLFWwindow* window, int w, int h);
void setupProjectionMatrix();

void drawAxes(float l, float w);

#endif // __GL_SETUP_H_
