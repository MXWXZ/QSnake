#include "player.h"

Player::Player() {}

/**
 * Set keys.
 * @param up up key
 * @param down down key
 * @param left left key
 * @param right right key
 */
void Player::SetKeys(int up, int down, int left, int right) {
    up_key_ = up;
    down_key_ = down;
    left_key_ = left;
    right_key_ = right;
}

/**
 * Start player.
 * @param map map
 * @param base window base
 */
void Player::StartPlayer(Map* map, Widget* base) {
    connect(&timer_, &QTimer::timeout, [=]() {
        if (map->CheckSnake(snake_)) {
            snake_->Push();
            if (map->CheckAndRefresh(snake_) == 1)
                timer_.setInterval(snake_->GetRefreshDelay());
        } else {
            Stop(map);
        }
        base->update();
    });
    timer_.start(snake_->GetRefreshDelay());
    valid_ = true;
}

/**
 * Key press.
 * @param key press key
 */
void Player::KeyPress(int key) {
    if (!snake_)
        return;
    if (key == up_key_)
        snake_->ChangeDirection(Snake::kUp);
    else if (key == down_key_)
        snake_->ChangeDirection(Snake::kDown);
    else if (key == left_key_)
        snake_->ChangeDirection(Snake::kLeft);
    else if (key == right_key_)
        snake_->ChangeDirection(Snake::kRight);
}

/**
 * Stop player.
 * @param map map
 */
void Player::Stop(Map* map) {
    map->DeleteSnake(snake_);
    snake_ = nullptr;
    valid_ = false;
    timer_.stop();
    timer_.disconnect();
}

/**
 * Pause player.
 * @param pause true for pause
 */
void Player::Pause(bool pause /* = true*/) {
    if (pause)
        timer_.stop();
    else
        timer_.start();
}
