#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //for matrices
#include <glm/gtc/type_ptr.hpp>

#define CLOTH_ROW_NUM 50
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024
#define CLOTH_STEPSIZE 0.3f
#define CLOTH_INIT_HEIGHT 5.0f
#define CAMERA_INIT_POSITION 60.0f
#define SPRING_STIFFNESS 2.0f
#define DELTA_TIME 1.0f/60.0f
#define ITERATION_NUM 1
#define GRAVITY -5.0f
#define MASS 0.001f
#define DAMPING 0.995