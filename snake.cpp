#include "snake.h"

Snake::Snake(const QList<Pos>& initpos, Direction direction, int max_width,
             int max_height, Qt::GlobalColor color)
    : max_width_(max_width),
      max_height_(max_height),
      direction_(direction),
      next_direction_(direction),
      color_(color) {
    pos_.append(initpos);
    length_ = initpos.size();
}

/**
 * Add snake node.
 * @param cnt node count to add to the snake
 */
void Snake::AddNode(int cnt) {
    if (length_ + cnt < 3)
        cnt = -(length_ - 3);
    length_ += cnt;
    node_left_ += cnt;
}

/**
 * Push the snake forward.
 */
void Snake::Push() {
    direction_ = next_direction_;
    pos_.push_front(GetNextPos());

    // do not delete last when node need to add
    if (node_left_ > 0) {
        --node_left_;
    } else if (node_left_ < 0) {
        do {
            pos_.pop_back();
            ++node_left_;
        } while (node_left_ < 0);
        pos_.pop_back();
    } else {
        pos_.pop_back();
    }
}

/**
 * Paint snake.
 * @param painter painter pointer
 */
void Snake::Paint(QPainter* painter) {
    painter->setBrush(QBrush(color_));
    painter->setPen(Qt::black);

    for (auto& i : pos_)
        painter->drawRect(QRectF(i.x * NODE_WIDTH, i.y * NODE_HEIGHT,
                                 NODE_WIDTH, NODE_HEIGHT));
}

/**
 * Change snake direction.
 * @param direction new direction
 * @note if direction is invalid, nothing will happen
 */
void Snake::ChangeDirection(Direction direction) {
    // invalid move
    if (direction + direction_ == kInvalid)
        return;

    next_direction_ = direction;
}

/**
 * Get snake level.
 * @return level
 */
int Snake::GetLevel() const {
    if (length_ <= LEVEL1_LEN)
        return 1;
    else if (length_ <= LEVEL2_LEN)
        return 2;
    else
        return 3;
}

/**
 * Get snake refresh delay.
 * @return refresh delay ms
 */
int Snake::GetRefreshDelay() const {
    switch (GetLevel()) {
        case 1:
            return LEVEL1_DELAY;
        case 2:
            return LEVEL2_DELAY;
        case 3:
            return LEVEL3_DELAY;
        default:
            assert(false);
    }
}

Pos Snake::GetNextPos() const {
    auto ret = pos_.first();
    switch (next_direction_) {
        case kLeft:
            --ret.x;
            if (ret.x < 0)
                ret.x = max_width_ - 1;
            break;
        case kRight:
            ++ret.x;
            if (ret.x == max_width_)
                ret.x = 0;
            break;
        case kUp:
            --ret.y;
            if (ret.y < 0)
                ret.y = max_height_ - 1;
            break;
        case kDown:
            ++ret.y;
            if (ret.y == max_height_)
                ret.y = 0;
            break;
        default:
            assert(false);
    }
    return ret;
}
