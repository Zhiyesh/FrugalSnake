#ifndef SNAKE_H
#define SNAKE_H

#include <QLabel>

#define SNAKE_BLOCK_SIDE 15  //蛇的边长
#define MAX_SNAKE_LEN 64  //最大蛇长度
#define DEFAULT_LEN 4  //默认蛇长度

class Snake_Body
{
public:
    QLabel* Body;
    int x;
    int y;
    int IsExist = 0;  //不存在则不渲染，只计算已经存在的

    void update() {this->Body->move(this->x, this->y);}
};

class Snake
{
public:
    Snake();
    QLabel* Head;
    Snake_Body snake_Body[MAX_SNAKE_LEN];
    int len = DEFAULT_LEN;
};


#endif // SNAKE_H
