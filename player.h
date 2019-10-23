#ifndef PLAYER_H
#define PLAYER_H

#include "snake.h"
#include "map.h"
#include "widget.h"
#include <QTimer>

class Widget;

class Player : public QObject {
    Q_OBJECT

public:
    Player();

    void SetSnake(Snake* snake) { snake_ = snake; }
    void SetKeys(int up, int down, int left, int right);
    virtual void StartPlayer(Map* map, Widget* base);
    void KeyPress(int key);
    bool IsValid() const { return valid_; }
    virtual void Stop(Map* map);
    virtual void Pause(bool pause = true);

protected:
    bool valid_ = false;
    Snake* snake_ = nullptr;  //!< player snake
    QTimer timer_;            //!< player timer
    int up_key_ = 0;          //!< up key
    int down_key_ = 0;        //!< down key
    int left_key_ = 0;        //!< left key
    int right_key_ = 0;       //!< right key
};

#endif  // PLAYER_H
