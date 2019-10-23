#ifndef SNAKE_H
#define SNAKE_H

#include <QPainter>
#include "config.h"

class Snake {
public:
    enum Direction {
        kLeft = 1,
        kUp,
        kDown,
        kRight,
        kInvalid,
    };

public:
    Snake(const QList<Pos>& initpos, Direction direction, int max_width,
          int max_height, Qt::GlobalColor color);

    void AddNode(int cnt);
    void Push();
    void Paint(QPainter* painter);
    void ChangeDirection(Direction direction);
    QList<Pos> GetPos() const { return pos_; }
    int GetLevel() const;
    int GetRefreshDelay() const;
    Direction GetDirection() const { return direction_; }
    Direction GetNextDirection() const { return next_direction_; }
    Pos GetNextPos() const;

private:
    QList<Pos> pos_;            //!< snake body
    int max_width_;             //!< max map width
    int max_height_;            //!< max map height
    Direction direction_;       //!< current direction
    Direction next_direction_;  //!< next direction
    int node_left_ = 0;         //!< node left to add
    int length_;                //!< snake length
    Qt::GlobalColor color_;     //!< snake color
};

#endif  // SNAKE_H
