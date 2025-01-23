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
        void player();
        void drawRoundRect(float x, float y, float width, float height, float radius, int angle);
        void drawCrescent(float x, float y, float radius, float thickness, int num_segments);
        void drawCircle(float x, float y, float radius, int num_segments);
        void drawEllipse(float x, float y, float radiusX, float radiusY, int num_segments);
        void drawFaceParts(float x, float y, float radiusX, float radiusY, int num_segments);
        void drawRect(float x, float y, float width, float height);
        void drawCurvedLeg(float x, float y, float width, float height, float angle, float thickness, bool flip);
        void drawIsoscelesTriangle(float x, float y, float base, float height);
};

void Player::drawRoundRect(float x, float y, float width, float height, float radius, int angle) {
    // 半径が高さの半分を超えないように調整
    radius = std::min(radius, height / 2);
    radius = std::min(radius, width / 2);
    
    int num_segments = 100; // 円弧の分割数
    float theta = 2 * PI / float(num_segments);
    float tangetial_factor = tanf(theta);
    float radial_factor = cosf(theta);

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

    // 四角形の左右の長方形部分を描画
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

    // 右上の円弧
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + width - radius, y + height - radius); // 円の中心
    for (int i = 0; i <= num_segments / 4; i++) {
        float cx = radius * cosf(theta * i);
        float cy = radius * sinf(theta * i);
        glVertex2f(x + width - radius + cx, y + height - radius + cy);
    }
    glEnd();

    // 左上の円弧
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + height - radius); // 円の中心
    for (int i = num_segments / 4; i <= num_segments / 2; i++) {
        float cx = radius * cosf(theta * i);
        float cy = radius * sinf(theta * i);
        glVertex2f(x + radius + cx, y + height - radius + cy);
    }
    glEnd();

    // 左下の円弧
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius); // 円の中心
    for (int i = num_segments / 2; i <= 3 * num_segments / 4; i++) {
        float cx = radius * cosf(theta * i);
        float cy = radius * sinf(theta * i);
        glVertex2f(x + radius + cx, y + radius + cy);
    }
    glEnd();

    // 右下の円弧
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + width - radius, y + radius); // 円の中心
    for (int i = 3 * num_segments / 4; i <= num_segments; i++) {
        float cx = radius * cosf(theta * i);
        float cy = radius * sinf(theta * i);
        glVertex2f(x + width - radius + cx, y + radius + cy);
    }
    glEnd();

    // 変換行列を元に戻す
    glPopMatrix();
}

void Player::drawCrescent(float x, float y, float radius, float thickness, int num_segments) {
    float angle_step = 2.0f * M_PI / num_segments;

    // 手の形にするために黒色で円を描く
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= num_segments; i++) {
        float angle = i * angle_step;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();

    // 手の形にするために黒色を白色で塗りつぶす
    glColor3f(1.0, 1.0, 1.0); // 白色を設定
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y + thickness); // 円弧の中心
    float large_radius = radius + 0.5f;
    for (int i = 0; i <= num_segments; i++) {
        float angle = i * angle_step;
        glVertex2f(x + cos(angle) * large_radius, y + thickness + sin(angle) * large_radius);
    }
    glEnd();
}

void Player::drawCircle(float x, float y, float radius, int num_segments) {
    float angle_step = 2.0f * M_PI / num_segments;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= num_segments; i++) {
        float angle = i * angle_step;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void Player::drawEllipse(float x, float y, float radiusX, float radiusY, int num_segments) {
    float angle_step = 2.0f * M_PI / num_segments;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= num_segments; i++) {
        float angle = i * angle_step;
        glVertex2f(x + cos(angle) * radiusX, y + sin(angle) * radiusY);
    }
    glEnd();
}

void Player::drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void Player::drawFaceParts(float x, float y, float radiusX, float radiusY, int num_segments) {
    // RightEye
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(x + 15, y + 10, 5, 100);
    // LeftEye
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(x - 15, y + 10, 5, 100);
    // Nose
    glColor3f(0.0, 0.0, 0.0);
    drawEllipse(x, y + 20, 10, 20, 100);
    // Neck
    glColor3f(0.0, 0.0, 0.0);
    drawRect(x - 3, y - 40, 6, 30);
}

void Player::drawCurvedLeg(float x, float y, float width, float height, float angle, float thickness, bool flip = 0) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);

    if (flip) {
        glScalef(-1, 1, 1);
    }

    glBegin(GL_QUAD_STRIP);
    for (float t = 0; t <= 1; t += 0.01) {
        float xt = (1 - t) * (1 - t) * 0 + 2 * (1 - t) * t * (width / 2) + t * t * width;
        float yt = (1 - t) * (1 - t) * 0 + 2 * (1 - t) * t * height + t * t * height;

        float nx = -yt;
        float ny = xt;
        float length = sqrt(nx * nx + ny * ny);
        nx /= length;
        ny /= length;

        glVertex2f(xt + nx * thickness / 2, yt + ny * thickness / 2);
        glVertex2f(xt - nx * thickness / 2, yt - ny * thickness / 2);
    }
    glEnd();

    glPopMatrix();
}

void Player::drawIsoscelesTriangle(float x, float y, float base, float height) {
    float halfBase = base / 2.0f;

    // 2等辺三角形を描画
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 0.0); // 黒色
    glVertex2f(x - halfBase, y);
    glVertex2f(x + halfBase, y);
    glVertex2f(x + halfBase, y + height);
    glEnd();

    // 底角を黒丸で塗りつぶす
    glColor3f(0.0, 0.0, 0.0); // 黒色
    drawCircle(x - halfBase, y, 10, 100); // 半径10の黒丸
    drawCircle(x + halfBase, y, 7, 100); // 半径10の黒丸

    // 頂角を白丸で隠す
    glColor3f(1.0, 1.0, 1.0); // 白色
    drawCircle(x, y + height, 12, 100); // 半径10の白丸
    drawRect(x - base, y - halfBase, 50, 10);
}

void Player::player(){
    // RightArm
    glColor3f(0.0, 0.0, 0.0);
    drawRoundRect(370, 80, 45, 10, 10, 45);
    drawRoundRect(320, 60, 45, 10, 10, 0);
    // LeftArm
    glColor3f(0.0, 0.0, 0.0);
    drawRoundRect(185, 80, 45, 10, 10, -45);
    drawRoundRect(235, 60, 45, 10, 10, 0);
    // RightHand
    glColor3f(0.0, 0.0, 0.0);
    drawCrescent(415, 110, 15, 10, 100);
    // LeftHand
    glColor3f(0.0, 0.0, 0.0);
    drawCrescent(185, 110, 15, 10, 100);
    // Body
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(300, 57, 20, 50);
    // Head
    glColor3f(0.0, 0.0, 0.0);
    drawEllipse(300, 100, 30, 18, 50);
    drawFaceParts(300, 100, 30, 18, 50);
    // RightLeg
    glColor3f(0.0, 0.0, 0.0);
    drawIsoscelesTriangle(20, 60, 20, 30);
    drawCurvedLeg(270, 12, 30, 10, 45, 5);
    // LeftLeg
    glColor3f(0.0, 0.0, 0.0);
    drawCurvedLeg(330, 12, 30, 10, -45, 5, 1);
}

Ball ball;
Player player;

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    player.player();
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