#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

static int width, height;
static int score;
static int on_going;
static float anim_param;

static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_reshape(int width, int height);
static void init_game(void);

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
    glEnable(GL_MULTISAMPLE);
    
    glPointSize(1);
    glLineWidth(2); 
    
    anim_param = 0;
    on_going = 0;
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
        break;
    case 'W':
    case 'w':   
        //up
        break;
    case 'D':
    case 'd':   
        //right
        break;
    case 'S':
    case 's':   
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(50, -50, 50, 0, 0, 0, 0, 0, 1);
    
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(1000,0,0);
    glEnd();
    
    glBegin(GL_LINES);
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,1000,0);
    glEnd();
    
    glBegin(GL_LINES);
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1000);
    glEnd();
    
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
