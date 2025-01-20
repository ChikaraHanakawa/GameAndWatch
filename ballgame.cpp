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

class Player{
    public:
        int x=360, y=60;
        void player();
        void drawRoundRect(float x, float y, float width, float height, float radius, int angle);
        void drawCrescent(float x, float y, float radius, float thickness, int num_segments);
};

void Player::drawRoundRect(float x, float y, float width, float height, float radius, int angle){
    int num_segments = 100; // 円弧の分割数
    float theta = 2 * PI / float(num_segments);
    float tangetial_factor = tanf(theta);
    float radial_factor = cosf(theta);

    float cx = radius; // 初期のx座標
    float cy = 0; // 初期のy座標

    // 変換行列を保存
    glPushMatrix();

    // 四角形を中心に回転
    glTranslatef(x + width / 2, y + height / 2, 0);
    glRotatef(angle, 0, 0, 1);
    glTranslatef(-(x + width / 2), -(y + height / 2), 0);

    // 四角形の中央部分を描画
    glBegin(GL_QUADS);
    glVertex2f(x + radius, y);
    glVertex2f(x + width - radius, y);
    glVertex2f(x + width - radius, y + height);
    glVertex2f(x + radius, y + height);
    glEnd();

    // 四角形の上下の長方形部分を描画
    glBegin(GL_QUADS);
    glVertex2f(x, y + radius);
    glVertex2f(x + radius, y + radius);
    glVertex2f(x + radius, y + height - radius);
    glVertex2f(x, y + height - radius);

    glVertex2f(x + width - radius, y + radius);
    glVertex2f(x + width, y + radius);
    glVertex2f(x + width, y + height - radius);
    glVertex2f(x + width - radius, y + height - radius);
    glEnd();

    // 四角形の角の円弧を描画
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        glVertex2f(x + width - radius + cx, y + height - radius + cy);
        float tx = -cy;
        float ty = cx;
        cx += tx * tangetial_factor;
        cy += ty * tangetial_factor;
        cx *= radial_factor;
        cy *= radial_factor;
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        glVertex2f(x + radius + cx, y + height - radius + cy);
        float tx = -cy;
        float ty = cx;
        cx += tx * tangetial_factor;
        cy += ty * tangetial_factor;
        cx *= radial_factor;
        cy *= radial_factor;
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        glVertex2f(x + radius + cx, y + radius + cy);
        float tx = -cy;
        float ty = cx;
        cx += tx * tangetial_factor;
        cy += ty * tangetial_factor;
        cx *= radial_factor;
        cy *= radial_factor;
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; i++) {
        glVertex2f(x + width - radius + cx, y + radius + cy);
        float tx = -cy;
        float ty = cx;
        cx += tx * tangetial_factor;
        cy += ty * tangetial_factor;
        cx *= radial_factor;
        cy *= radial_factor;
    }
    glEnd();

    // 変換行列を元に戻す
    glPopMatrix();
}
void Player::drawCrescent(float x, float y, float radius, float thickness, int num_segments) {
    float angle_step = 2.0f * M_PI / num_segments;

    // Draw outer circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= num_segments; i++) {
        float angle = i * angle_step;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();

    // Draw inner circle to create crescent shape
    glColor3f(1.0, 1.0, 1.0); // Set color to white to "erase" part of the circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y + thickness); // Move the inner circle up
    for (int i = 0; i <= num_segments; i++) {
        float angle = i * angle_step;
        glVertex2f(x + cos(angle) * (radius - thickness), y + thickness + sin(angle) * (radius - thickness));
    }
    glEnd();
}

void Player::player(){
    // RightArm
    glColor3f(0.0, 0.0, 0.0);
    drawRoundRect(x, y, 70, 20, 10, 45);
    // LeftArm
    glColor3f(0.0, 0.0, 0.0);
    drawRoundRect(x - 200, y, 70, 20, 10, -45);
    // RightHand
    glColor3f(0.0, 0.0, 0.0);
    drawCrescent(x + 70, y, 20, 15, 100);
    // LeftHand
    glColor3f(0.0, 0.0, 0.0);
    drawCrescent(x - 200, y, 20, 15, 100);
}

Ball ball;
Player player;

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    ball.move();
    ball.ball();
    player.player();
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