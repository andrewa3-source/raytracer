#include <OpenGl/gl.h>
#include <OpenGl/glu.h>
// #include <OpenGl/glut.h>

#define NUM_VERTICES 8
#define NUM_FACES 6

GLint vertices[NUM_VERTICES][3] = {
    {  0,  0,  0 },
    {  0,  0,  1 },
    {  0,  1,  0 },
    {  0,  1,  1 },
    {  1,  0,  0 },
    {  1,  0,  1 },
    {  1,  1,  0 },
    {  1,  1,  1 }
};

GLint faces[NUM_FACES][4] = {
    {1, 5, 7, 3},
    {5, 4, 6, 7},
    {4, 0, 2, 6},
    {3, 7, 6, 2},
    {0, 1, 3, 2},
    {0, 4, 5, 1} 
};

GLfloat colors[NUM_VERTICES][3] = {
    { 0.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 1.0, 1.0 },
    { 1.0, 0.0, 0.0 },
    { 1.0, 0.0, 1.0 },
    { 1.0, 1.0, 0.0 },
    { 1.0, 1.0, 1.0 }
};



