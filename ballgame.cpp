#include <GL/glut.h>
#include <vector>
#include <cmath>

#define PI 3.14159265358979323846
#define WIDTH 640
#define HEIGHT 480

class Ball {
public:
    float x, y, vx, vy;
    const float gravity = -0.1, restitution = 0.8;
    std::vector<float> circleVertices;
    Ball() : x(10.0), y(10.0), vx(3.0), vy(10.0) {}
    void setCircleVertices();
    void ball();
    void move();
};

void Ball::setCircleVertices(){
    float angle = 0.0;
    float radius = 10.0;
    float x = 0.0;
    float y = 0.0;
    for(int i = 0; i < 360; i++){
        angle = i * PI / 180;
        x = radius * cos(angle);
        y = radius * sin(angle);
        circleVertices.push_back(x);
        circleVertices.push_back(y);
    }
}

void Ball::ball(){
    glColor3f(0.0, 0.0, 0.0); // ボールの色を黒に設定
    glBegin(GL_POLYGON);
    for(int i = 0; i < circleVertices.size(); i += 2){
        glVertex2f(circleVertices[i] + x, circleVertices[i + 1] + y);
    }
    glEnd();
}

void Ball::move(){
    vy += gravity;
    x += vx;
    y += vy;
    if(x < 0 || x > WIDTH){
        vx = -vx;
    }
    if(y < 0){
        y = 0;
        vy = -vy * restitution;
    }
    if(y > HEIGHT){
        y = HEIGHT;
        vy = -vy;
    }
}

Ball ball;

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    ball.move();
    ball.ball();
    glutSwapBuffers();
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 約60FPSで更新
}

void initOpenGL(){
    glClearColor(1.0, 1.0, 1.0, 1.0); // 背景色を白に設定
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT); // 2Dの正射影を設定
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Ball Game");

    initOpenGL();

    ball.setCircleVertices();

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}