#include "widget.h"

void Widget::newInit()
{
    this->timer = new QTimer(this);
    this->gradually = new QTimer(this);
    this->food_update = new QTimer(this);
    this->gameover = new QLabel(this);
    this->snake = new Snake();
    this->snake->Head = new QLabel(this);
    this->head_direction = UP;

    //初始化蛇头属性
    snake->Head->setPixmap(QPixmap(":/Snake/Snake_Block_Head.png"));
    snake->Head->resize(SNAKE_BLOCK_SIDE + 2, SNAKE_BLOCK_SIDE + 2);

    //初始化蛇身属性
    for (int i = 0; i < MAX_SNAKE_LEN; i++)
    {
        snake->snake_Body[i].Body= new QLabel(this);
        snake->snake_Body[i].Body->setPixmap(QPixmap(":/Snake/Snake_Block_Body.png"));
        snake->snake_Body[i].Body->resize(SNAKE_BLOCK_SIDE, SNAKE_BLOCK_SIDE);
    }

    //初始化蛇身位置
#define SET_LOCATION_FOR_SNAKE    \
    for (int i = 0; i < MAX_SNAKE_LEN; i++)                               \
    {                                                                     \
        snake->snake_Body[i].x = current_head_position[0];                \
        snake->snake_Body[i].y = current_head_position[1] + SNAKE_BLOCK_SIDE * i;   \
        snake->snake_Body[i].update();                                    \
    }

    SET_LOCATION_FOR_SNAKE

    //初始化食物
    srand((unsigned)time(NULL));
    for (int i = 0; i < FOOD_NUM; i++)
    {
        food[i].Foodstyle = new QLabel(this);
        food[i].Foodstyle->setPixmap(QPixmap(":/Snake/Food.png"));
        int food_size = rand() % 1 + 10;  //[10, 11)
        food[i].Foodstyle->resize(food_size, food_size);
        food[i].x = rand() % WIDTH;
        food[i].y = rand() % HEIGHT;
        food[i].Foodstyle->move(food[i].x, food[i].y);
    }

    //重新开始按钮初始化
    this->restart = new QPushButton(this);
    restart->setText("开始游戏");
    restart->setFont(QFont("微软雅黑", 11));
    restart->resize(110, 60);
    restart->move(WIDTH/2 - 53, HEIGHT/2 + 25);
    //重新开始
    connect(restart, &QPushButton::clicked, this, [=](){
        current_head_position[0] = (WIDTH/2)-(SNAKE_BLOCK_SIDE/2);
        current_head_position[1] = (HEIGHT/2)-20;
        snake->Head->move(current_head_position[0], current_head_position[1]);
        this->head_direction = UP;
        snake->len = DEFAULT_LEN;
        for (int i = snake->len; i < MAX_SNAKE_LEN; i++)
        {
            snake->snake_Body[i].IsExist = 0;
        }
        SET_LOCATION_FOR_SNAKE
        gameover->hide();
        this->is_gameover = 0;
        food_update->start();
        for (int i = 0; i < FOOD_NUM; i++)
        {
            int food_size = rand() % 1 + 10;
            food[i].Foodstyle->resize(food_size, food_size);
            food[i].x = rand() % WIDTH;
            food[i].y = rand() % HEIGHT;
            food[i].Foodstyle->move(food[i].x, food[i].y);
            food[i].Foodstyle->show();
        }
        restart->hide();
        timer->stop();
        timer->setInterval(RATE);
        restart->setText("重新开始");
        timer->start();
    });


    //初始化游戏结束提示
    gameover->setText("游戏结束");
    gameover->resize(200, 100);
    gameover->move(WIDTH/2 - 75, HEIGHT/2 - 120);
    gameover->setFont(QFont("微软雅黑", 18));
    gameover->hide();
}

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    INIT
    resize(WIDTH, HEIGHT);

    //默认蛇的长度
    for (int i = 0; i < DEFAULT_LEN; i++)
    {
        snake->snake_Body[i].IsExist = 1;
    }

    //游戏帧
    timer->setInterval(RATE);
    connect(timer, &QTimer::timeout, this, [=](){
        update(1);
    });

    //食物更新间隔
    food_update->setInterval(10*1000);
    connect(food_update, &QTimer::timeout, this, [=](){
        //更新的食物会更大
        srand((unsigned)time(NULL));
        for (int i = 0; i < FOOD_NUM; i++)
        {
            if (food[i].Foodstyle->isHidden())
            {
                int food_new_size = rand() % 5 + 10;  //[10, 15)
                food[i].Foodstyle->resize(food_new_size, food_new_size);
                food[i].x = rand() % WIDTH;
                food[i].y = rand() % HEIGHT;
                food[i].Foodstyle->move(food[i].x, food[i].y);
                food[i].Foodstyle->show();
            }
        }
    });
    food_update->start();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up :
    case Qt::Key_W :
    {
        if (head_direction != DOWN && head_direction != UP)
        {
            current_head_position[1] -= SNAKE_BLOCK_SIDE;
            head_direction = UP;
            update();
        }
        break;
    }
    case Qt::Key_Down :
    case Qt::Key_S :
    {
        if (head_direction != UP && head_direction != DOWN)
        {
            current_head_position[1] += SNAKE_BLOCK_SIDE;
            head_direction = DOWN;
            update();
        }
        break;
    }
    case Qt::Key_Left :
    case Qt::Key_A :
    {
        if (head_direction != RIGHT && head_direction != LEFT)
        {
            current_head_position[0] -= SNAKE_BLOCK_SIDE;
            head_direction = LEFT;
            update();
        }

        break;
    }
    case Qt::Key_Right :
    case Qt::Key_D :
    {
        if (head_direction != LEFT && head_direction != RIGHT)
        {
            current_head_position[0] += SNAKE_BLOCK_SIDE;
            head_direction = RIGHT;
            update();
        }
        break;
    }
    //回车键加速
    case Qt::Key_Enter :
    case Qt::Key_Return :
    {
        if (snake->len > DEFAULT_LEN + 5)
        {
            if (head_direction == UP || head_direction == DOWN)
            {
                current_head_position[1] += head_direction * SNAKE_BLOCK_SIDE;
            }
            else if (head_direction == LEFT || head_direction == RIGHT)
            {
                current_head_position[0] += head_direction * SNAKE_BLOCK_SIDE / 2;
            }
            if ((++flag) % 4 == 0)
            {
                if (snake->len > DEFAULT_LEN) snake->snake_Body[--snake->len].IsExist = 0;
            }
            update();
        }
        break;
    }
    }
}


void Widget::update(int isTimeout)
{
    if (this->is_gameover)
    {
        return;
    }
//begin游戏结束判定
    //边界判定
    if ((current_head_position[0] < -10 || current_head_position[0] > WIDTH)
            || (current_head_position[1] < 0 || current_head_position[1] > HEIGHT - 10))
    {
        gameOver();
    }

    //自杀判定
    for (int i = 2; i < snake->len; i++)
    {
        if (snake->snake_Body[i].x == current_head_position[0] && snake->snake_Body[i].y == current_head_position[1])
        {
            gameOver();
        }
    }
//end游戏结束判定

    if (isTimeout)
    {
        switch (head_direction)  //根据头方向自然移动
        {
        case UP :
            current_head_position[1] -= SNAKE_BLOCK_SIDE;
            break;
        case DOWN :
            current_head_position[1] += SNAKE_BLOCK_SIDE;
            break;
        case LEFT :
            current_head_position[0] -= SNAKE_BLOCK_SIDE;
            break;
        case RIGHT :
            current_head_position[0] += SNAKE_BLOCK_SIDE;
            break;
        }
    }
    snake->Head->raise();
    snake->Head->move(current_head_position[0] - 1, current_head_position[1] - 1);

    if (snake->len < 30)
    {
        timer->setInterval(RATE);
    }
    switch (snake->len)
    {
    case 30:
        timer->setInterval(RATE * 0.9);
        break;
    case 40:
        timer->setInterval(RATE * 0.8);
        break;
    case 50:
        timer->setInterval(RATE * 0.7);
        break;
    case 75:
        timer->setInterval(RATE * 0.6);
        break;
    case 100:
        timer->setInterval(RATE * 0.49);
        break;
    case 125:
        timer->setInterval(RATE * 0.41);
        break;
    case 150:
        timer->setInterval(RATE * 0.34);
        break;
    }

    //更新坐标
    followHead();

    //吃食物
    eatFood();

    for (int i = 0; i < MAX_SNAKE_LEN; i++)
    {
        if (snake->snake_Body[i].IsExist)
        {
            snake->snake_Body[i].Body->show();
        }else snake->snake_Body[i].Body->hide();
    }
}

void Widget::followHead()
{
    snake->snake_Body[0].x = current_head_position[0];
    snake->snake_Body[0].y = current_head_position[1];
    for (int i = MAX_SNAKE_LEN - 1; i > 0; i--)
    {
        snake->snake_Body[i].x = snake->snake_Body[i - 1].x;
        snake->snake_Body[i].y = snake->snake_Body[i - 1].y;
        snake->snake_Body[i].update();
    }
    snake->snake_Body[0].update();
}

void Widget::eatFood()
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < FOOD_NUM; i++)
    {
        auto caneat = [=]()->int{
            if (sqrt((current_head_position[0] - food[i].x)*(current_head_position[0] - food[i].x)
                     +(current_head_position[1] - food[i].y)*(current_head_position[1] - food[i].y)
                ) < SNAKE_BLOCK_SIDE)
            {
                return 1;
            }
            return 0;
        };
        if (caneat() == 1)
        {
            food[i].Foodstyle->hide();

            if (snake->len < MAX_SNAKE_LEN) snake->snake_Body[snake->len++].IsExist = 1;
        }
    }
}

void Widget::gameOver()
{
    gameover->show();
    restart->show();
    this->is_gameover = 1;
    food_update->stop();
    //文字背景清空
    for (int i = 0; i < FOOD_NUM; i++)
    {
        if ((food[i].x > (WIDTH/2 - 200) && food[i].x < (WIDTH/2 + 200))
                && (food[i].y > (HEIGHT/2 - 200) && food[i].y < (HEIGHT/2 + 200)))
        {
            food[i].Foodstyle->hide();
        }
    }
}

Widget::~Widget()
{
    delete snake;
    delete restart;
    delete gameover;
    delete food_update;
    delete gradually;
    delete timer;
}
