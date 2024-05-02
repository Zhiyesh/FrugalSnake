#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <random>
#include <ctime>
#include <cmath>
#include <QFont>
#include <QPushButton>

#define HEIGHT 978
#define WIDTH 1244

#define RATE 150  //初始速度
#define FOOD_NUM 64  //食物数量

#define UP -1
#define DOWN 1
#define LEFT -2
#define RIGHT 2

#define INIT this->newInit();

#include "snake.h"
#include "food.h"


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void newInit();
    void update(int isTimeout = 0);
    void followHead();
    void eatFood();
    void gameOver();
    ~Widget();

private:
    void keyPressEvent(QKeyEvent* event);
    QTimer *timer;
    QTimer *gradually;
    QTimer *food_update;
    Snake *snake;
    Food food[FOOD_NUM];
    QLabel *gameover;
    QPushButton *restart;
    int current_head_position[2] = {/*(WIDTH/2)-(SNAKE_BLOCK_SIDE/2), (HEIGHT/2)-20*/ WIDTH/2, HEIGHT/2};  //当前位置
    int head_direction;  // UP RIGHT DOWN LEFT
    int is_gameover = 0;
    int flag = 0;
};

#endif // WIDGET_H
