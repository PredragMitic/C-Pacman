#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14
#define EPS 0.3

static int map_mat[67][67]  = {
                {4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,5},
                {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
                {8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,4,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,4,9,9,9,9,9,9,0,5,0,2,0,8,0,8,0,2,0,4,9,9,9,9,9,9,0,5,0,2,0,4,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,8},
                {8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,8,0,1,0,8,0,0,0,0,0,0,0,8,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,8,0,1,0,8},
                {8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,8,0,2,0,8,0,0,0,0,0,0,0,8,0,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,8,0,2,0,8},
                {8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,7,9,9,9,9,9,9,9,9,9,9,0,6,0,2,0,7,9,9,9,9,9,9,0,6,0,2,0,7,0,6,0,2,0,7,9,9,9,9,9,9,9,6,0,2,0,7,9,9,9,9,9,9,9,9,9,9,0,6,0,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,4,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,4,0,5,0,2,0,4,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,4,0,5,0,2,0,4,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,7,9,9,9,9,9,9,9,9,9,9,0,6,0,2,0,8,0,8,0,2,0,7,9,9,9,9,0,5,0,4,9,9,9,9,0,6,0,2,0,8,0,8,0,2,0,7,9,9,9,9,9,9,9,9,9,9,0,6,0,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,8,0,8,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,3,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,3,0,8},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {7,9,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,0,2,0,8,0,7,9,9,9,9,0,5,0,2,0,8,0,8,0,2,0,4,9,9,9,9,0,6,0,8,0,2,0,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,6},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,0,8,0,4,9,9,9,9,0,6,0,2,0,7,0,6,0,2,0,7,9,9,9,9,0,5,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,8,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,0,8,0,8,0,2,0,4,9,9,9,0,5,0,1,0,4,9,9,9,0,5,0,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,8,0,0,0,0,0,0,1,0,0,0,0,0,0,8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,6,0,0,2,0,7,0,6,0,2,0,8,0,1,0,0,0,0,1,0,0,0,0,1,0,8,0,2,0,7,0,6,0,2,0,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,8,0,1,0,0,0,0,1,0,0,0,0,1,0,8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,1,1,1,1,1,1,1,1,1,1,1,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,8,0,1,0,0,0,0,1,0,0,0,0,1,0,8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,0,2,0,4,0,5,0,2,0,8,0,1,0,0,0,0,1,0,0,0,0,1,0,8,0,2,0,4,0,5,0,2,0,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,0,8,0,8,0,2,0,7,9,9,9,9,9,9,9,9,9,9,9,9,0,6,0,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,0,8,0,8,0,2,0,4,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {4,9,9,9,9,9,9,9,9,9,9,9,9,9,0,6,0,0,2,0,7,0,6,0,2,0,7,9,9,9,9,0,5,0,4,9,9,9,9,0,6,0,2,0,7,0,6,0,2,0,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,5},
                {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,8,0,8,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
                {8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,4,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,4,9,9,9,9,9,9,0,5,0,2,0,8,0,8,0,2,0,4,9,9,9,9,9,9,0,5,0,2,0,4,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,7,9,9,9,9,9,9,9,9,0,5,0,8,0,2,0,7,9,9,9,9,9,9,0,6,0,2,0,7,0,6,0,2,0,7,9,9,9,9,9,9,0,6,0,2,0,8,0,4,9,9,9,9,9,9,9,9,0,6,0,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,0,8},
                {0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
                {7,9,9,9,9,9,9,9,9,0,5,0,2,0,8,0,8,0,2,0,4,0,5,0,2,0,4,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,2,0,4,0,5,0,2,0,8,0,8,0,2,0,4,9,9,9,9,9,9,9,9,0,6},
                {0,0,0,0,0,0,0,0,0,0,8,0,1,0,8,0,8,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,1,0,8,0,8,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,8,0,2,0,8,0,8,0,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,2,0,8,0,8,0,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
                {4,9,9,9,9,9,9,9,9,0,6,0,2,0,7,0,6,0,2,0,8,0,8,0,2,0,7,9,9,9,9,0,5,0,4,9,9,9,9,0,6,0,2,0,8,0,8,0,2,0,7,0,6,0,2,0,7,9,9,9,9,9,9,9,9,0,5},
                {8,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,8,0,8,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,8},
                {8,0,3,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,0,8,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,3,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,6,0,7,9,9,9,9,0,5,0,2,0,8,0,8,0,2,0,4,9,9,9,9,0,6,0,7,9,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,0,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,0,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,6,0,2,0,7,0,6,0,2,0,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,6,0,0,2,0,8},
                {8,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,8},
                {8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,6}
             };

static int width, height;
static int score;
static int on_going;
static int position[2];
static int next_position[2];
static int wanted_direction[2];
static int ghosts_color[3] = {1,2,3};
static int ghosts_position[6];
static int game_timer;
static int rot_ply = 0;
static float anim_param;

static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int width, int height);
static void init_game(void);
void draw_map();
void draw_player(int);
void draw_wall_y();
void draw_wall_x();
void draw_pillar( float from, float to);
void light_and_material(void);
void draw_ghost(int,int,int);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(32, 16);
    glutCreateWindow(argv[0]);

    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);

    init_game();
    
    glutMainLoop();
    
    return 0;
}

static void init_game(void)
{
    glClearColor(0.5, 0.8, 0.9, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    
    glPointSize(1);
    glLineWidth(2); 
    
    game_timer = 0;
    anim_param = 0;
    on_going = 0;

    position[0] = 28;
    position[1] = 2;
    
    next_position[0] = 0;
    next_position[1] = 0;

    wanted_direction[0] = 0;
    wanted_direction[1] = 0;
    
    ghosts_position[0] = 28;
    ghosts_position[1] = 28;
    ghosts_position[2] = 28;
    ghosts_position[3] = 33;
    ghosts_position[4] = 28;
    ghosts_position[5] = 38;
    
    light_and_material();
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:   
        exit(0);
        break;
    case 'A':
    case 'a':  
        //left
        wanted_direction[0] = 0;
        wanted_direction[1] = -1;
        break;
    case 'W':
    case 'w':   
        wanted_direction[0] = -1;
        wanted_direction[1] = 0;        
        //up
        break;
    case 'D':
    case 'd':   
        wanted_direction[0] = 0;
        wanted_direction[1] = 1;        
        //right
        break;
    case 'S':
    case 's':   
        wanted_direction[0] = 1;
        wanted_direction[1] = 0;        
        //down
        break;
    case 13:
        //start game
        break;
    }
    glutPostRedisplay();
}

static void on_display(void)
{
    game_timer++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(40, 30, 7, 30, 30, 0, 0, 0, 1);
    
    glBegin(GL_LINES);
        // X Axis
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(1000,0,0);
    glEnd();
    
    glBegin(GL_LINES);
        // Y Axis
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,1000,0);
    glEnd();
    
    glBegin(GL_LINES);
        // Z Axis
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1000);
    glEnd();
    
    
    glBegin(GL_POLYGON);
        glColor3f(0.2, 0.7, 0.8); 
        glNormal3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,66,0);
        glVertex3f(62,66,0);
        glVertex3f(62,0,0);
    glEnd();
   

    if (map_mat[position[0] + wanted_direction[0]][position[1] + wanted_direction[1]] > 0){
        next_position[0] = wanted_direction[0];
        next_position[1] = wanted_direction[1];
        rot_ply = -wanted_direction[0] * 90;
        rot_ply +=  wanted_direction[1] == -1 ? 180 : 0;
    }

    if (game_timer % 2 == 0 && map_mat[position[0] + next_position[0]][position[1] + next_position[1]] > 0){
        position[0] += next_position[0];
        position[1] += next_position[1];
        map_mat[position[0]][position[1]] = 1;
    }
    
    if ( position[1] < 1 ) 
        position[1] = 65;
    else if (position[1] > 65)
        position[1] = 1;
        

    draw_map();
    draw_player(rot_ply);

    draw_ghost(ghosts_position[0],ghosts_position[1],1);
    draw_ghost(ghosts_position[2],ghosts_position[3],2); 
    draw_ghost(ghosts_position[4],ghosts_position[5],3);
    
    glutPostRedisplay();
    glutSwapBuffers();
}

static void on_reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) w / h, 0.1, 200);
}

void draw_map()
{
    
    int i, j;
    
    for (i = 0; i < 67; i++){
        for (j = 0; j < 67; j++){
            
            float wave = sin(game_timer/5 + i + j)/5+1;
            
            switch (map_mat[i][j]){
            case 2 :
                glTranslatef(i,j,wave);
                    glColor3f(1, 1, 0.3);
                    glutSolidSphere(.3,10,10);
                glTranslatef(-i,-j,-wave);
                break;
                
            case 3 :
                glTranslatef(i,j,2);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(.5,10,10);
                glTranslatef(-i,-j,-2);
                break;
                
            case 9 :
                glTranslatef(i,j,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_wall_y();
                glTranslatef(-i,-j,0);
                break;
            case 8 :
                glTranslatef(i,j,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_wall_x();
                glTranslatef(-i,-j,0);
                break;
            
            case 7 :
                glTranslatef(i-1,j+1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(PI/2,PI);
                glTranslatef(-i+1,-j-1,0);
                break;
            case 6 :
                glTranslatef(i-1,j-1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(0,PI/2);
                glTranslatef(-i+1,-j+1,0);
                break;
            case 5 :
                glTranslatef(i+1,j-1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(3*PI/2, 2*PI);
                glTranslatef(-i-1,-j+1,0);
                break;    
            case 4 :
                glTranslatef(i+1,j+1,0);
                    glColor3f(0.2, 0, 0.8);
                    draw_pillar(PI,3*PI/2);
                glTranslatef(-i-1,-j-1,0);
                break;
                
            }
        }
    }
}

void draw_player(int rot)
{   
    int mouth_angle = (game_timer % 20)*2;

    glPushMatrix();
    glTranslatef(position[0],position[1],2);
        
        glRotatef(90+rot,0,0,1);        
        glRotatef(-mouth_angle,0,1,0); 
        
        GLUquadric *newQuad = gluNewQuadric();

        // Up side of head
        glPushMatrix();
            GLdouble plane0[] = {0,0,1,0};
            glColor3f(0.7, 0.6, 0);
            gluDisk(newQuad,0,1.4,32,4);
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, plane0);
                glColor3f(0.8, 0.9, 0);
                glutSolidSphere(1.5,20,20); 
            glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
        
        // Down side of head
        glPushMatrix();
            glRotatef(2*mouth_angle,0,1,0);
            glColor3f(0.7, 0.6, 0);
            gluDisk(newQuad,0,1.4,32,4); 
            GLdouble plane1[] = {0,0,-1,0};
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, plane1);
                glColor3f(0.8, 0.9, 0);
                glutSolidSphere(1.5,20,20); 
            glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
        
        // Eyes
        glTranslatef(1.1,0.4,0.5);
            glColor3f(1, 1, 1);
            glutSolidSphere(.35,20,20);
            glTranslatef(0.2,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.18,10,10);
            glTranslatef(-0.2,0,-0.1);
        glTranslatef(0,-0.8,0);
            glColor3f(1, 1, 1);
            glutSolidSphere(0.35,20,20);
            glTranslatef(0.2,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.18,10,10);
            glTranslatef(-0.2,0,-0.1);
        glTranslatef(-1.1,0.4,-0.5);

    glPopMatrix();
}

void draw_wall_y()
{
    int wall_high = 2.2;
    
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
         glNormal3f(1,0,0);
        glVertex3f(0,0,wall_high);
        glVertex3f(0,1,wall_high);
        glVertex3f(0,1,0);
    glEnd();
                  
    
}
void draw_wall_x()
{
    int wall_high = 2.2;
    
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glNormal3f(0,1,0);
        glVertex3f(0,0,wall_high);
        glVertex3f(1,0,wall_high);
        glVertex3f(1,0,0);
    glEnd();
}

void set_v(float u, float v, float r)
{
    glNormal3f(sin(v),cos(v),0);
    glVertex3f(r*sin(v),r*cos(v),u);
}

void draw_pillar( float from, float to)
{
    float l = 2.2;
    float r = 1;
    float u, v;
    glPushMatrix();
    for (u = 0; u < l-EPS; u += 0.2) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = from; v <= to + EPS; v += 0.2) {
            set_v(u, v, r);
            set_v(u + 0.2, v,r);
        }
        glEnd();
    }
    glPopMatrix();
}

void draw_ghost(int x,int y, int c)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
    
    switch (ghosts_color[c-1]){
        case 0 :
            glColor4f(0.8,0.8,1,0.9); break;
        case 1 :
            glColor4f(0.9,0,0,0.9); break;
        case 2 :
            glColor4f(0,0.9,0,0.9); break;
        case 3 :
            glColor4f(0,0,0.9,0.9); break;
    }
    
   
    glPushMatrix();
    
    glTranslatef(x,y,2.4);
        // Up side of head
        glPushMatrix();
            GLdouble plane0[] = {0,0,1,0};
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, plane0);
                
                glutSolidSphere(1.7,40,40); 
            glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
        
        float l = 1.6;
        float r = 1.7;
        float u, v;
        glPushMatrix();
        glTranslatef(0,0,-1.6);
        glRotatef(game_timer/2,0,0,1); 
        
            GLUquadric *quad = gluNewQuadric();
            
            gluCylinder(quad, r,  r,  l,  40,  40);
            
        
            int i = 1;
            glBegin(GL_TRIANGLE_STRIP);
            for (v = 0; v <= 2*PI + EPS; v += PI/10) {
                i++;
                glNormal3f(sin(v),cos(v),0);
                glVertex3f(r*sin(v),r*cos(v),0);
                
                glNormal3f(sin(v),cos(v),0);
                glVertex3f(r*sin(v),r*cos(v),-(i%2)-0);
            }
            glEnd();
        
        glPopMatrix();
        
   
        
        // Eyes
        glTranslatef(1.2,0.5,0.5);
            glColor3f(1, 1, 1);
            glutSolidSphere(.4,20,20);
            glTranslatef(0.3,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.15,10,10);
            glTranslatef(-0.3,0,-0.1);
        glTranslatef(0,-1,0);
            glColor3f(1, 1, 1);
            glutSolidSphere(0.4,20,20);
            glTranslatef(0.3,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.15,10,10);
            glTranslatef(-0.3,0,-0.1);
        glTranslatef(-1.2,0.5,-0.5);
        
        // Eyebrows
        float eybrows_height = 0.9 + cos(game_timer/6)/20;
        glPushMatrix();
            glTranslatef(1.4,0.5,eybrows_height); 
            glRotatef(30,1,0,1);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.4,-0.5,eybrows_height); 
            glRotatef(-30,1,0,1);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
    
        // Mouth
        float mouth_height = -0.5 + sin(game_timer/6)/10;
        glPushMatrix();
        glRotatef(22,0,0,1);
        glPushMatrix();
            glTranslatef(2,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
        glRotatef(-22,0,0,1);
        glPushMatrix();
            glTranslatef(2,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(2,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(2,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(3,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        glPopMatrix();
        
    glPopMatrix();
    
}

void light_and_material(void)
{
    GLfloat light_position[] = {-100, 100, 100, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat diffuse[] = { 0.8, 0.9, 0.8, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat ambient_coeff[] = { 0.5, 0.4, 0.3, 1 };
    GLfloat diffuse_coeff[] = { 1, 1, 1, 1 };
    GLfloat specular_coeff[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat shininess = 50;
       
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

