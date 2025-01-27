#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <cmath>

#define PI 3.14159265358979323846
#define WIDTH 640
#define HEIGHT 480

class Ball {
    float x, y, vx, vy;
    const float gravity = -0.1, restitution = 0.9;
public:
    std::vector<float> circleVertices;
    Ball() : x(10.0), y(400.0), vx(3.0), vy(5.0) {}
    void setCircleVertices();
    void ball();
    void move(float LhandX, float RhandX, float handY);
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

void Ball::move(float LhandX, float RhandX, float handY){
    if(handY < y && y < handY + 5){
        if(LhandX < x && x < LhandX + 45 || RhandX < x && x < RhandX + 45){
            vy = -vy * 1.2;
        }
    }
    if(x < 0 || x > WIDTH){
        vx = -vx;
    }
    if(y > HEIGHT){
        y = HEIGHT;
        vy = -vy;
    }
    if(y < 0){
        y = 0;
        vy = -vy * restitution;
    }
    vy += gravity;
    x += vx;
    y += vy;
}

class Player{
    public:
        float positionX = 300.0;
        float moveSpeed = 10.0;
        float LhandX = 185, LhandY = 110, RhandX = 415, RhandY = 110, RarmX = 370, LarmX = 185, FootX = 266,RlegX = 330, LlegX = 270, headX = 300;
        void player();
        void move();
        void updateHandPositions();
        void drawRoundRect(float x, float y, float width, float height, float radius, int angle);
        void drawCrescent(float x, float y, float radius, float thickness, int num_segments);
        void drawCircle(float x, float y, float radius, int num_segments);
        void drawEllipse(float x, float y, float radiusX, float radiusY, int num_segments);
        void drawFaceParts(float x, float y, float radiusX, float radiusY, int num_segments);
        void drawRect(float x, float y, float width, float height);
        void drawCurvedLeg(float x, float y, float width, float height, float angle, float thickness, bool flip);
        void drawIsoscelesTriangle(float x, float y, float base, float height);
};

void Player::drawRoundRect(float x, float y, float width, float height, float radius, int angle){
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
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(x + 15, y + 12, 3, 100);
    // LeftEye
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(x - 15, y + 10, 5, 100);
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(x - 15, y + 12, 3, 100);
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
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(x - halfBase, y);
    glVertex2f(x + halfBase, y);
    glVertex2f(x + halfBase, y + height);
    glEnd();

    // 底角を黒丸で塗りつぶす
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(x - halfBase, y, 10, 100);
    drawCircle(x + halfBase, y, 7, 100);

    // 頂角を白丸で隠す
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(x, y + height, 12, 100);
    drawRect(x - base, y - halfBase, 50, 10);

    x += 70;

    // 左右反転した2等辺三角形を描画
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(x - halfBase, y);
    glVertex2f(x + halfBase, y);
    glVertex2f(x - halfBase, y + height);
    glEnd();

    // 底角を黒丸で塗りつぶす
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(x - halfBase, y, 7, 100);
    drawCircle(x + halfBase, y, 10, 100);

    // 頂角を白丸で隠す
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(x, y + height, 12, 100);
    drawRect(x - base, y - halfBase, 50, 10);
}

void Player::player(){
    // RightArm
    glColor3f(0.0, 0.0, 0.0);
    drawRoundRect(RarmX, 80, 45, 10, 10, 45);
    drawRoundRect(RarmX - 50, 60, 45, 10, 10, 0);
    // LeftArm
    glColor3f(0.0, 0.0, 0.0);
    drawRoundRect(LarmX, 80, 45, 10, 10, -45);
    drawRoundRect(LarmX + 50, 60, 45, 10, 10, 0);
    // RightHand
    glColor3f(0.0, 0.0, 0.0);
    drawCrescent(RhandX, RhandY, 15, 10, 100);
    // LeftHand
    glColor3f(0.0, 0.0, 0.0);
    drawCrescent(LhandX, LhandY, 15, 10, 100);
    // Body
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(headX, 57, 20, 50);
    // Head
    glColor3f(0.0, 0.0, 0.0);
    drawEllipse(headX, 100, 30, 18, 50);
    drawFaceParts(headX, 100, 30, 18, 50);
    // Foot
    glColor3f(0.0, 0.0, 0.0);
    drawIsoscelesTriangle(FootX, 2, 20, 30);
    // Leg
    glColor3f(0.0, 0.0, 0.0);
    drawCurvedLeg(RlegX, 12, 30, 10, -45, 5, 1); // Right
    drawCurvedLeg(LlegX, 12, 30, 10, 45, 5); // Left
}

void Player::updateHandPositions(){
    LhandX = positionX - 115;
    RhandX = positionX + 115;
    LarmX = positionX - 115;
    RarmX = positionX + 70;
    FootX = positionX - 34;
    RlegX = positionX + 30;
    LlegX = positionX - 30;
    headX = positionX;
}

void Player::move(){
    updateHandPositions();
}

Ball ball;
Player player;

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    player.player();
    ball.move(player.LhandX, player.RhandX, player.LhandY);
    ball.ball();
    glutSwapBuffers();
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 約60FPSで更新
}

void specialKeyInput(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_LEFT:
            if(player.positionX > 50) { // 左端の制限
                player.positionX -= player.moveSpeed;
            }
            break;
        case GLUT_KEY_RIGHT:
            if(player.positionX < WIDTH - 50) { // 右端の制限
                player.positionX += player.moveSpeed;
            }
            break;
    }
    player.move(); // 位置の更新を反映
    glutPostRedisplay();
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
    glutSpecialFunc(specialKeyInput); 
    glutMainLoop();
    return 0;
}