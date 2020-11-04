#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#include "image.h"

#define PI 3.14
#define EPS 0.3
#define TIMER_INTERVAL 40
#define TIMER_ID 0

static char* FILENAME1 = "pacman.bmp"; 
static char* FILENAME2 = "floor1.bmp"; 
static float matrix[16]; 
static GLuint names[3];

static int map_mat[67][67];

static int width, height;
static int score;
static int on_going;
static int position[2];
static int ready = 1;
static int next_position[2];
static int wanted_direction[2];
static int ghosts_color[3] = {1,2,3};
static int ghosts_look[3] = {90,180,-90};
static int ghosts_position[6];
static int ghosts_dir[6] = {0,1,1,0,0,-1};
static int game_timer;
static int rot_ply = 0;
static int view_param;
static float anim_param;

static void on_keyboard(unsigned char key, int x, int y);
static void on_display(void);
static void on_timer(int);
static void on_reshape(int width, int height);
static void init_game(void);
static void init_map();
void draw_map();
void draw_player(int);
void draw_wall_y();
void draw_wall_x();
void draw_pillar( int);
void light_and_material(void);
void draw_ghost(int,int,int);
void move_ghost(int ghost);
float min_distance_to_another(int ghost, int x, int y);
void text(char* t, int h, int w);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(30, 15);
    glutCreateWindow(argv[0]);
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    
    glClearColor(0.1, 0.1, 0.1, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    light_and_material();
    
    init_game();
    
    glutMainLoop();
    
    return 0;
}

static void init_game(void)
{
  
    glPointSize(1);
    glLineWidth(2); 
    
    init_map();
    
    game_timer = 0;
    anim_param = 0;
    on_going = 0;
    view_param = 14;
    score = 0;
    
    width = 1920;
    height = 1080;
    
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
    
    
        
    Image * image;
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    image = image_init(0, 0);

    glGenTextures(3,names);
    image_read(image, FILENAME1);
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    image_read(image, FILENAME2);
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 
                    0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    image_done(image);
}

static void on_timer(int v)
{
    if (v != TIMER_ID) return;
	game_timer++;
    glutPostRedisplay();
    
    ready = 0;
    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);   
}

static void on_keyboard(unsigned char key, int x, int y)
{   
    switch (key) {
    case 27:   
        exit(0);
        break;
    case 13:
        if (on_going == 0){ // Game intro
            on_going = 1;
            game_timer = 0;
        }
        if (on_going == -1 || on_going == 2){
            on_going = 1; // Start new game
            init_game();
        }
        break;
    case 'Q':
    case 'q':
        if (view_param < 20 && on_going == 1)
            view_param += 2; // Move camera up
        break;
    case 'E':
    case 'e':
        if (view_param > 0 && on_going == 1)
            view_param -= 2; // Move camera down
        break;
        
    }
    
    
    if (on_going == 1 && game_timer > 80){
        switch (key) {
        case 'A':
        case 'a':  
            // Go left
            wanted_direction[0] = 0;
            wanted_direction[1] = -1;
            break;
        case 'W':
        case 'w':   
            wanted_direction[0] = -1;
            wanted_direction[1] = 0;        
            // Go up
            break;
        case 'D':
        case 'd':   
            wanted_direction[0] = 0;
            wanted_direction[1] = 1;        
            // Go right
            break;
        case 'S':
        case 's':   
            wanted_direction[0] = 1;
            wanted_direction[1] = 0;        
            //Go down
            break;
        
        }
    }
    
    glutPostRedisplay();
}

static void on_display(void)
{
    game_timer++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (on_going == 1 && game_timer < 80)
        anim_param = game_timer;
    
    // Seting camera and view parametars	
    float x0 = on_going != 0 ? 84 - anim_param/3 + view_param : 115;
    float y0 = on_going != 0 ? 33 : 0;
    float z0 = on_going != 0 ? 26 + anim_param/4: 0;
    float x1 = on_going != 0 ? 35 : 0;
    float y1 = on_going != 0 ? 33 : 0;
    float z1 = on_going != 0 ? 14 - anim_param/4 + view_param/2 : 0;
    
    gluLookAt(x0, y0, z0, x1, y1, z1, 0, 0, 1);
        
    // Set Intro picture on the plane
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glBegin(GL_QUADS);
        glNormal3f(1,0,0);
        glTexCoord2f(0,0);
            glVertex3f(100,-16,-9);
        glTexCoord2f(1,0); 
            glVertex3f(100,16,-9);
        glTexCoord2f(1,1); 
            glVertex3f(100,16,9);
        glTexCoord2f(0,1); 
            glVertex3f(100,-16,9);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Set floor texture
    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glTexCoord2f(0,0);
            glVertex3f(0,0,0);
        glTexCoord2f(11,0); 
            glVertex3f(0,66,0);;
        glTexCoord2f(11,11); 
            glVertex3f(62,66,0);
        glTexCoord2f(0,11); 
           glVertex3f(62,0,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    

   
    if (on_going == 1){
        // Moving player 
        if (map_mat[position[0] + wanted_direction[0]][position[1] + wanted_direction[1]] > 0){
            next_position[0] = wanted_direction[0];
            next_position[1] = wanted_direction[1];
            rot_ply = -wanted_direction[0] * 90;
            rot_ply +=  wanted_direction[1] == -1 ? 180 : 0;
        }
        // Set current position and calculate score
        if (ready == 0 && map_mat[position[0] + next_position[0]][position[1] + next_position[1]] > 0){
            position[0] += next_position[0];
            position[1] += next_position[1];
            if (map_mat[position[0]][position[1]] == 2) score += 10;
            if (map_mat[position[0]][position[1]] == 3) score += 100;
            map_mat[position[0]][position[1]] = 1;
        }
        
	// Ability to go to other side 
        if ( position[1] < 1 ) 
            position[1] = 65;
        else if (position[1] > 65)
            position[1] = 1;
        
    
        // After some time, ghosts can walk away 
        if (game_timer > 150 && ready == 0)
            move_ghost(2);

        if (game_timer > 350 && ready == 0)
            move_ghost(1);
        
        if (game_timer > 500 && ready == 0)
            move_ghost(3);
    }
    
    
    draw_map();
    draw_player(rot_ply);

    
    
    draw_ghost(ghosts_position[0],ghosts_position[1],1);
    draw_ghost(ghosts_position[2],ghosts_position[3],2); 
    draw_ghost(ghosts_position[4],ghosts_position[5],3);
    
    int i = 0;
    
    for (; i < 3; i++) {
	// Check player and ghosts collide 
        if ( sqrt(pow(position[0]-ghosts_position[i*2],2) + pow(position[1]-ghosts_position[i*2+1],2)) < 3){
            position[0] = ghosts_position[i*2];
            position[1] = ghosts_position[i*2+1];
            on_going = -1;
        }
	// Turn ghost in walking way
        ghosts_look[i] = ghosts_dir[2*i+1] * 90;
        ghosts_look[i] = ghosts_dir[2*i] == -1 ? 180 : ghosts_look[i];
    }
    
    char t1[30] = " ";
    char t2[60] = " ";
    
    if (on_going != 0)
        sprintf(t1, "Score: %d ", score);
    if (on_going == 2)
        sprintf(t2, "Congrats, it was nice!    Press ENTER to play again");
    if (on_going == -1)
        sprintf(t2, "Game Over!   Press ENTER to play again");
    
    text(t2, 50, height-50);
    text(t1, width-150, height-50);

    // If player has score 3870, game is done
    if (score == 3870)
        on_going = 2;
    
    ready = 1;
    
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
    gluPerspective(60, (float) w / h, 1, 100);
}

void draw_map()
{
    int i, j;
    // Read map matrix 
    for (i = 0; i < 67; i++){
        for (j = 0; j < 67; j++){
            
            float wave = sin(game_timer/5 + i + j)/5+1;
            
            switch (map_mat[i][j]){
            case 2 : // small ball
                glTranslatef(i,j,wave);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(.35,4,3);
                glTranslatef(-i,-j,-wave);
                break;
                
            case 3 : // big ball
                glTranslatef(i,j,2);
                    glColor3f(.7, .6, 0);
                    glutSolidSphere(.5,14,14);
                glTranslatef(-i,-j,-2);
                break;
                
            case 9 : // wall
                glTranslatef(i,j,0);
                    glColor3f(0.1, .8, 0.9);
                    draw_wall_y();
                glTranslatef(-i,-j,0);
                break;
            case 8 : // wall
                glTranslatef(i,j,0);
                    glColor3f(0.1, .8, 0.9);
                    draw_wall_x();
                glTranslatef(-i,-j,0);
                break;
            
            case 7 : // round wall
                glTranslatef(i-1,j+1,0);
                   glColor3f(0.1, .9, 1);
                    glColor3f(0.1, .8, 0.9);
                    draw_pillar(0);
                glTranslatef(-i+1,-j-1,0);
                break;
            case 6 : //
                glTranslatef(i-1,j-1,0);
                    glColor3f(0.1, .8, 0.9);
                    draw_pillar(1);
                glTranslatef(-i+1,-j+1,0);
                break;
            case 5 : //
                glTranslatef(i+1,j-1,0);
                    glColor3f(0.1, .8, 0.9);
                    draw_pillar(2);
                glTranslatef(-i-1,-j+1,0);
                break;    
            case 4 : //
                glTranslatef(i+1,j+1,0); 
                    glColor3f(0.1, .8, 0.9);
                    draw_pillar(3);
                glTranslatef(-i-1,-j-1,0);
                break;
                
            }
        }
    }
}

void draw_player(int rot)
{   
    int mouth_angle = on_going == 1 ? (game_timer % 20)*2 : 0 ;

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
            glutSolidSphere(.35,16,16);
            glTranslatef(0.2,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.18,8,8);
            glTranslatef(-0.2,0,-0.1);
        glTranslatef(0,-0.8,0);
            glColor3f(1, 1, 1);
            glutSolidSphere(0.35,16,16);
            glTranslatef(0.2,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.18,8,8);
            glTranslatef(-0.2,0,-0.1);
        glTranslatef(-1.1,0.4,-0.5);

    glPopMatrix();
}

void draw_wall_y()
{
    int wall_high = 2.2;
    // Wall plane
    glBegin(GL_POLYGON);
        glNormal3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(0,0,wall_high);
        glVertex3f(0,1,wall_high);
        glVertex3f(0,1,0);
    glEnd();
                  
    
}
void draw_wall_x()
{
    int wall_high = 2.2;
    // Wall plane
    glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,0,wall_high);
        glVertex3f(1,0,wall_high);
        glVertex3f(1,0,0);
    glEnd();
    
}

void draw_pillar( int k)
{
    // Draw 1/4 cylinder on the end of wall
    float l = 2;
    float r = 1;
    float u, v;
    glPushMatrix();
    glRotatef(k*90,0,0,1);
    GLUquadric *quad = gluNewQuadric();
    GLdouble plane0[] = {0,-1,0,0};
    GLdouble plane1[] = {1,0,0,0};
        glEnable(GL_CLIP_PLANE0);
        glClipPlane(GL_CLIP_PLANE0, plane0);
        glEnable(GL_CLIP_PLANE0+1);
        glClipPlane(GL_CLIP_PLANE0+1, plane1);
        gluCylinder(quad, r,  r,  l,  12,  1);
        glDisable(GL_CLIP_PLANE0+1);
        glDisable(GL_CLIP_PLANE0);
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
        glRotatef(ghosts_look[c-1],0,0,1);

	// Ghost Head
        glPushMatrix();
            GLdouble plane0[] = {0,0,1,0};
            glEnable(GL_CLIP_PLANE0);
            glClipPlane(GL_CLIP_PLANE0, plane0);
                
                glutSolidSphere(1.7,20,20); 
            glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
        
	// Ghosts body
        float l = 1.6;
        float r = 1.7;
        float u, v;
        glPushMatrix();
        glTranslatef(0,0,-1.6);
        glRotatef(game_timer/2,0,0,1); 
        
            GLUquadric *quad = gluNewQuadric();  
            gluCylinder(quad, r,  r,  l,  20,  20);
        
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
                glutSolidSphere(.15,8,8);
            glTranslatef(-0.3,0,-0.1);
        glTranslatef(0,-1,0);
            glColor3f(1, 1, 1);
            glutSolidSphere(0.4,16,16);
            glTranslatef(0.3,0,0.1);
                glColor3f(0,0,0);
                glutSolidSphere(.15,6,6);
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
            glTranslatef(1.8,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(1,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        //
        glPushMatrix();
            glTranslatef(1.8,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(1,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        glPopMatrix();
        //
        glPushMatrix();
            glTranslatef(1.8,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(1,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        //
        glPushMatrix();
            glTranslatef(1.8,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(1,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        //
        glPushMatrix();
        glRotatef(-22,0,0,1);
        glPushMatrix();
            glTranslatef(1.8,-0.2,mouth_height); 
            glRotatef(50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(1,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        //
        glPushMatrix();
            glTranslatef(1.8,0.2,mouth_height); 
            glRotatef(-50,1,0,0);
            glColor3f(0.3,0,0.2);
            glScalef(1,7,1);
            glutSolidCube(0.1);
        glPopMatrix();
        glPopMatrix();
        
    glPopMatrix();
    
}

void move_ghost(int ghost)
{
    int x = ghosts_position[ghost*2-2];
    int y = ghosts_position[ghost*2-1];
   
    int count = 0;
    
    int new_x[4] = {0,0,0,0};
    int new_y[4] = {0,0,0,0};
    int new_pos_x, new_pos_y;
    
    // Check adjacent positions and ghosts  
    if (map_mat[x + 1][y] > 0 && min_distance_to_another(ghost, 1, 0) > 3){
        new_x[count]++;
        count++;
    } 
    //   
    if (map_mat[x - 1][y] > 0 && min_distance_to_another(ghost, -1, 0) > 3){
        new_x[count]--;
        count++;
    }
    //
    if (map_mat[x][y + 1] > 0 && min_distance_to_another(ghost, 0, 1) > 3){
        new_y[count]++;
        count++;
    }
    //
    if (map_mat[x][y - 1] > 0 && min_distance_to_another(ghost, 0, -1) > 3 ){
        new_y[count]--;
        count++;
    }

    // If can not move, stay on current position  
    if (count == 0) {
        new_pos_x = x;
        new_pos_y = y;
    }
    // Random new positon
    else {
        clock_t cl = clock();
        double r = cl;
        srand( cl );
    
        int rand_coef = rand() % count;
        new_pos_x = x + new_x[rand_coef];
        new_pos_y = y + new_y[rand_coef];
    }
    
    // If there is no any barrier, go ahead.
    if (count < 3 && map_mat[x + ghosts_dir[ghost*2-2]][y + ghosts_dir[ghost*2-1]] > 0 
    && min_distance_to_another(ghost, ghosts_dir[ghost*2-2], ghosts_dir[ghost*2-1]) > 3 ){
        new_pos_x = x + ghosts_dir[ghost*2-2];
        new_pos_y = y + ghosts_dir[ghost*2-1];   
    }
    // If position has not changed, do not change direction and position
    else if (new_pos_x == x && new_pos_y == y) {
        return;
    }
    else {
    // New direction 
        ghosts_dir[ghost*2-2] = new_pos_x - x;
        ghosts_dir[ghost*2-1] = new_pos_y - y;
    }
    
    // New position
    ghosts_position[ghost*2-2] = new_pos_x;
    ghosts_position[ghost*2-1] = new_pos_y;
}

float min_distance_to_another(int ghost, int x, int y)
{
    int g_x = ghost - 1;
    int g1_x = (g_x + 1) % 3;
    int g2_x = (g_x + 2) % 3;
    
    // Calculate distances to another ghosts (Manhattan)
    float dist1 = abs(ghosts_position[g_x*2]-ghosts_position[g1_x*2]+x) + abs(ghosts_position[g_x*2+1]-ghosts_position[g1_x*2+1]+y);
    float dist2 = abs(ghosts_position[g_x*2]-ghosts_position[g2_x*2]+x) + abs(ghosts_position[g_x*2+1]-ghosts_position[g2_x*2+1]+y);
    
    return dist1 < dist2 ? dist1 : dist2;
}

void light_and_material(void)
{
    // Set brightnes 
    GLfloat light_position[] = {330, 40, 100, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat diffuse[] = { 1, 1, 1, 1 };
    GLfloat specular[] = { 0, 0, 0, 1 };
    GLfloat ambient_coeff[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat diffuse_coeff[] = { 0, 0, 0, 1 };
    GLfloat specular_coeff[] = { 1, 1, 1, 1 };
    GLfloat shininess = 0;
       
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeff);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void text(char* t, int h, int w)
{   
    // Write text t on display position (h,w)
    glDisable(GL_LIGHTING); 
    glColor3f(0, 0.8, 0.9);
    glPushMatrix();
        glMatrixMode(GL_PROJECTION); 
        
        GLdouble matrix[16];
        glGetDoublev(GL_PROJECTION_MATRIX, matrix);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -3, 3);
        
        glMatrixMode(GL_MODELVIEW);
        
        glLoadIdentity();
        glRasterPos2f(h,w); 
        
        int i;
        for( i = 0; t[i]; i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int) t[i]); 
        }
        
        glMatrixMode(GL_PROJECTION);
        
        glLoadMatrixd(matrix); 
        
        glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
}

void init_map() {
   
    int mat[67][67] = {
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
                {1,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,8,0,1,1,1,1,1,1,1,1,1,1,1,0,8,0,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,1},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,8,0,1,0,0,0,0,1,0,0,0,0,1,0,8,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,5,0,0,2,0,4,0,5,0,2,0,8,0,1,0,0,0,0,1,0,0,0,0,1,0,8,0,2,0,4,0,5,0,2,0,4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,2,0,8,0,8,0,2,0,7,9,9,9,0,6,0,1,0,7,9,9,9,0,6,0,2,0,8,0,8,0,2,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,8,0,8,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,8,0,8,0,1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
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
             
         
        int i, j;    
        for (i = 0; i < 67; i++){
            for (j = 0; j < 67; j++){
                map_mat[i][j] = mat[i][j];
            }
        }
}
