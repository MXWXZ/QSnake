#ifndef MAP_H
#define MAP_H

#include <snake.h>

#include <QPainter>

class Map {
public:
    enum NodeType {
        kEmpty = 0,
        kFood,
        kSpecial,  // special food
        kBigFood,
        kBoomFood,
        kDead,  // after this dead
        kSnake,
        kWall
    };

public:
    Map();
    ~Map();
    void SetMap(int width, int height);
    Snake* AddSnake(const QList<Pos>& initpos, Snake::Direction direction,
                    Qt::GlobalColor color);
    void DeleteSnake(Snake* snake);
    bool CheckSnake(Snake* snake) const { return snakes_.contains(snake); }
    void AddRandom(NodeType type, int cnt);
    void AddWall();
    void InitMap();
    NodeType** GetMap() const { return map_; }
    int GetWidth() const { return max_width_; }
    int GetHeight() const { return max_height_; }

    void Paint(QPainter* painter);
    int CheckAndRefresh(Snake* snake);

private:
    void RefreshSnake(Snake* snake);

private:
    NodeType** map_ = nullptr;  //!< map
    QList<Snake*> snakes_;      //!< snake list
    int max_width_;             //!< max map width
    int max_height_;            //!< max map height
};

#endif  // MAP_H
