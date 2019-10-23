#include "widget.h"
#include <QPainter>
#include <QKeyEvent>
#include "config.h"
#include "aiplayer.h"

Widget::Widget(QWidget* parent) : QWidget(parent) {
    // resize and disable user resize
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setStyleSheet("QWidget{background:black}");
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    map_.SetMap(WINDOW_WIDTH, WINDOW_HEIGHT);
    for (int i = 0; i < MAX_PLAYER - 1; ++i)
        player_.push_back(new Player);
    player_.push_back(new AIPlayer);
}

Widget::~Widget() {
    for (auto i : player_)
        delete i;
}

void Widget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font("YaHei Consolas Hybrid", 30, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Qt::white);

    // check snake alive
    if (start_) {
        int remain = 0;
        for (int i = 0; i < MAX_PLAYER; ++i)
            if (player_[i]->IsValid())
                ++remain;
        if (start_ == 1 && !remain) {  // single mode
            StopGame();
            painter.drawText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 50,
                             QString("GAME OVER!"));
            painter.drawText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2,
                             QString("Press R to restart!"));
            return;
        }
        if (start_ == 2 && remain <= 1) {  // 2 player mode
            bool whowin = player_[0]->IsValid();
            StopGame();
            if (remain == 0)
                painter.drawText(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50,
                                 QString("Tie!"));
            else
                painter.drawText(
                    WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50,
                    QString(whowin ? "Player 1 win!" : "Player 2 win!"));
            painter.drawText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2,
                             QString("Press R to restart!"));
            return;
        }
        if (start_ == 3 && remain <= 1) {  // computer mode
            bool whowin = player_[0]->IsValid();
            StopGame();
            if (remain == 0)
                painter.drawText(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50,
                                 QString("Tie!"));
            else
                painter.drawText(
                    WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50,
                    QString(whowin ? "Player 1 win!" : "Computer win!"));
            painter.drawText(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2,
                             QString("Press R to restart!"));
            return;
        }
        if (start_ > 10)
            painter.drawText(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50,
                             QString("PAUSE"));
    } else {
        painter.drawText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100,
                         QString("Snake Game"));
        font.setPointSize(20);
        painter.setFont(font);
        painter.drawText(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2,
                         QString("1: Single mode"));
        painter.drawText(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 50,
                         QString("2: Two-Player mode"));
        painter.drawText(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 100,
                         QString("3: Human VS Computer"));
    }

    map_.Paint(&painter);
    QWidget::paintEvent(event);
}

void Widget::keyPressEvent(QKeyEvent* event) {
    for (int i = 0; i < MAX_PLAYER; ++i)
        if (player_[i]->IsValid())
            player_[i]->KeyPress(event->key());

    switch (event->key()) {
        case Qt::Key_R:
            if (gameover_) {
                gameover_ = false;
                start_ = 0;
                map_.InitMap();
                update();
            }
            break;
        case Qt::Key_Space:
            if (start_ > 10) {
                for (int i = 0; i < MAX_PLAYER; ++i)
                    player_[i]->Pause(false);
                food_timer.start();
                foodex_timer.start();
                start_ -= 10;
            } else if (start_ && !gameover_) {
                for (int i = 0; i < MAX_PLAYER; ++i)
                    player_[i]->Pause();
                food_timer.stop();
                foodex_timer.stop();
                start_ += 10;
            }
            if (!gameover_)
                update();
            break;
        case Qt::Key_Escape:
            exit(0);
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
            if (!start_) {
                start_ = 1;

                // player1
                QList<Pos> pos;
                pos.push_back(Pos(40, 50));
                pos.push_back(Pos(40, 51));
                pos.push_back(Pos(40, 52));
                auto snake = map_.AddSnake(pos, Snake::kUp, Qt::green);
                player_[0]->SetKeys(Qt::Key_Up, Qt::Key_Down, Qt::Key_Left,
                                    Qt::Key_Right);
                player_[0]->SetSnake(snake);
                player_[0]->StartPlayer(&map_, this);

                if (event->key() == Qt::Key_2) {  // player2
                    start_ = 2;
                    QList<Pos> pos;
                    pos.push_back(Pos(40, 12));
                    pos.push_back(Pos(40, 11));
                    pos.push_back(Pos(40, 10));
                    auto snake = map_.AddSnake(pos, Snake::kDown, Qt::red);
                    player_[1]->SetKeys(Qt::Key_W, Qt::Key_S, Qt::Key_A,
                                        Qt::Key_D);
                    player_[1]->SetSnake(snake);
                    player_[1]->StartPlayer(&map_, this);
                }

                if (event->key() == Qt::Key_3) {  // computer
                    start_ = 3;
                    QList<Pos> pos;
                    pos.push_back(Pos(40, 12));
                    pos.push_back(Pos(40, 11));
                    pos.push_back(Pos(40, 10));
                    auto snake = map_.AddSnake(pos, Snake::kDown, Qt::blue);
                    player_[2]->SetSnake(snake);
                    player_[2]->StartPlayer(&map_, this);
                }

                // init map
                map_.AddWall();
                map_.AddRandom(Map::kFood, 5);
                connect(&food_timer, &QTimer::timeout, [=]() {
                    map_.AddRandom(Map::kFood, 2);
                    update();
                });
                food_timer.start(FOOD_DELAY);
                map_.AddRandom(Map::kBigFood, 3);
                connect(&foodex_timer, &QTimer::timeout, [=]() {
                    map_.AddRandom(Map::NodeType(Map::kSpecial + 1 +
                                                 qrand() % (Map::kDead -
                                                            Map::kSpecial - 1)),
                                   1);
                    map_.AddRandom(Map::NodeType(Map::kSpecial + 1 +
                                                 qrand() % (Map::kDead -
                                                            Map::kSpecial - 1)),
                                   1);
                    update();
                });
                foodex_timer.start(FOODEX_DELAY);
                map_.AddRandom(Map::kWall, 10);
            }
    }
}

void Widget::StopGame() {
    food_timer.stop();
    foodex_timer.stop();
    food_timer.disconnect();
    foodex_timer.disconnect();
    for (int i = 0; i < MAX_PLAYER; ++i)
        player_[i]->Stop(&map_);
    gameover_ = true;
}
