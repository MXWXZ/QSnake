#include "map.h"

Map::Map() {}

Map::~Map() {
    for (int i = 0; i < max_width_; ++i)
        delete[] map_[i];
    delete[] map_;

    for (auto& i : snakes_)
        delete i;
}

/**
 * Set map size.
 * @param width map width, must be multiple of NODE_WIDTH
 * @param height map height, must be multiple of NODE_HEIGHT
 */
void Map::SetMap(int width, int height) {
    assert(width % NODE_WIDTH == 0);
    assert(height % NODE_HEIGHT == 0);

    max_width_ = width / NODE_WIDTH;
    max_height_ = height / NODE_HEIGHT;
    map_ = new NodeType*[max_width_];
    InitMap();
}

/**
 * Add a snake.
 * @param initpos initial pos
 * @param direction initial direction
 * @return snake pointer
 */
Snake* Map::AddSnake(const QList<Pos>& initpos, Snake::Direction direction,
                     Qt::GlobalColor color) {
    snakes_.push_back(
        new Snake(initpos, direction, max_width_, max_height_, color));
    RefreshSnake(snakes_.last());
    return snakes_.last();
}

/**
 * Delete a snake.
 * @param snake snake pointer
 */
void Map::DeleteSnake(Snake* snake) {
    if (snake && snakes_.contains(snake)) {
        for (auto i : snake->GetPos())
            map_[i.x][i.y] = kEmpty;
        snakes_.removeOne(snake);
        delete snake;
    }
}

/**
 * Add food.
 * @param type food type
 * @param cnt food count
 */
void Map::AddRandom(NodeType type, int cnt) {
    int max = max_width_ * max_height_;
    for (int i = 0; i < cnt; ++i) {
        int pos = qrand() % max;
        int tmp = pos;
        while (map_[pos % max_width_][pos / max_width_] != kEmpty) {
            pos = (pos + 1) % max;
            if (pos == tmp)
                return;
        }
        map_[pos % max_width_][pos / max_width_] = type;
    }
}

/**
 * Add random wall.
 */
void Map::AddWall() {
    for (int i = 0; i < 4; ++i) {
        int mod = i < 2 ? max_width_ : max_height_;
        int length = qrand() % (mod + 1);
        int start = qrand() % (mod + 1 - length);
        for (int j = start; j < start + length; ++j)
            map_[i < 2 ? j : (i == 2 ? 0 : max_width_ - 1)]
                [i < 2 ? (i == 0 ? 0 : max_height_ - 1) : j] = kWall;
    }
}

/**
 * Init map.
 */
void Map::InitMap() {
    for (int i = 0; i < max_width_; ++i) {
        map_[i] = new NodeType[max_height_];
        memset(map_[i], 0, sizeof(NodeType) * max_height_);
    }
}

/**
 * Paint map.
 * @param painter painter pointer
 */
void Map::Paint(QPainter* painter) {
    painter->setPen(Qt::black);

    for (auto i : snakes_)
        i->Paint(painter);

    for (int i = 0; i < max_width_; ++i)
        for (int j = 0; j < max_height_; ++j) {
            switch (map_[i][j]) {
                case kFood:
                    painter->setBrush(QBrush(Qt::yellow));
                    break;
                case kBigFood:
                    painter->setBrush(QBrush(QColor(255, 69, 0)));
                    break;
                case kBoomFood:
                    painter->setBrush(QBrush(Qt::cyan));
                    break;
                case kWall:
                    painter->setBrush(QBrush(QColor(150, 75, 0)));
                    break;
                default:
                    continue;
            }
            painter->drawRect(QRectF(i * NODE_WIDTH, j * NODE_HEIGHT,
                                     NODE_WIDTH, NODE_HEIGHT));
        }
}

/**
 * Check and refresh snake pos.
 * @param snake snake pointer
 * @return 0 for success, 1 for food, 2 for dead
 */
int Map::CheckAndRefresh(Snake* snake) {
    int ret = 0;

    // clear snake
    for (int i = 0; i < max_width_; ++i)
        for (int j = 0; j < max_height_; ++j)
            if (map_[i][j] == kSnake)
                map_[i][j] = kEmpty;

    // refresh other snakes
    for (auto i : snakes_)
        if (i != snake)
            RefreshSnake(i);

    auto pos = snake->GetPos();
    // except first node
    for (auto i = pos.begin() + 1; i < pos.end(); ++i)
        map_[i->x][i->y] = kSnake;
    auto first = pos.first();
    if (map_[first.x][first.y] > kDead) {  // dead
        // redo
        for (auto i = pos.begin() + 1; i < pos.end(); ++i)
            map_[i->x][i->y] = kEmpty;
        snakes_.removeOne(snake);
        delete snake;

        // another snake
        for (auto& i : snakes_) {
            if (i->GetPos().first() == first)
                DeleteSnake(i);
        }
        return 2;
    } else if (map_[first.x][first.y] == kFood) {
        snake->AddNode(1);
        ret = 1;
    } else if (map_[first.x][first.y] == kBigFood) {
        snake->AddNode(2);
        ret = 1;
    } else if (map_[first.x][first.y] == kBoomFood) {
        for (auto i : snakes_)
            if (i != snake)
                i->AddNode(-1);
    }
    map_[first.x][first.y] = kSnake;
    return ret;
}

/**
 * Refresh snake pos.
 * @param snake snake pointer
 */
void Map::RefreshSnake(Snake* snake) {
    for (auto& i : snake->GetPos())
        map_[i.x][i.y] = kSnake;
}
