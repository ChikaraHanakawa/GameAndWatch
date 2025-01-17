#include <iostream>
#include <math.h>
#include <ctime>
#include <vector>
#include </usr/include/GL/glut.h>

#define WIDTH 640
#define HEIGHT 480
#define PI 3.14159265358979323846

class Game{
    public:
        bool gameover = false;
        std::vector<float> circleVetices = {};
        void init();
        void display();
};

void Game::init(){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2);
}

class Ball : public Game{
    private:
        float x = -60.0;
        float y = 180.0;
        float vx = 1.0;
        float vy = 1.0;
    public:
        void initCircleVertices();
        void ball();
};

void Ball::initCircleVertices(){
    for(int i = 0; i < 360; i++){
        float theta = i * PI / 180;
        float x = 0.5 * cos(theta);
        float y = 0.5 * sin(theta);
        circleVetices.push_back(x);
        circleVetices.push_back(y);
    }
}

void Ball::ball(){
    if(circleVetices.size() == 0){
        initCircleVertices();
    }
    glBegin(GL_POLYGON);
    for(int i = 0; i < circleVetices.size(); i += 2){
        glVertex2f(circleVetices[i] + x, circleVetices[i+1] + y);
    }
    glEnd();
}

class Player : public Game{
    public:
        void draw();
        void hand_move();
};

void Player::draw(){
    if(circleVetices.empty()){
        for(int i = 0; i < 360; i++){
            float theta = i * PI / 180;
            float x = 0.5 * cos(theta);
            float y = 0.5 * sin(theta);
            circleVetices.push_back(x);
            circleVetices.push_back(y);
        }
    }
    // Head
    // Body
    glBegin(GL_QUADS);
    for(int i = 0; i < circleVetices.size(); i += 2){
        glVertex2f(circleVetices[i], circleVetices[i+1] - 60.0);
    }
    glEnd();
    // LeftHand
    // RightHand
    // Leg
}

void Player::hand_move(){
    static float x = 0.0;
    static float y = 0.0;
    if(x < -WIDTH/2){
        x = -WIDTH/2;
    }
    if(x > WIDTH/2){
        x = WIDTH/2;
    }
    if(Game::gameover){
        x = 0.0;
    }
    glTranslatef(x, y, 0.0);
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Game");
    Game game;
    Ball ball;
    Player player;
    game.init();
    while(!game.gameover){
        ball.ball();
        glClear(GL_COLOR_BUFFER_BIT);
        player.hand_move();
        player.draw();
        glutSwapBuffers();
    }
    return 0;
}