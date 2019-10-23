#include "aiplayer.h"

AIPlayer::AIPlayer() {}

/**
 * Start player.
 * @param map map
 * @param base window base
 */
void AIPlayer::StartPlayer(Map* map, Widget* base) {
    Player::StartPlayer(map, base);
    connect(&ai_timer_, &QTimer::timeout, [=]() {
        if (!map->CheckSnake(snake_)) {
            Stop(map);
            return;
        }
        auto head = snake_->GetPos().first();
        auto node = map->GetMap();
        Pos minpos(0, 0);
        int min = INF;
        Snake::Direction dir = snake_->GetDirection();

        // find nearest food
        for (int i = 0; i < map->GetWidth(); ++i)
            for (int j = 0; j < map->GetHeight(); ++j)
                if (node[i][j] < Map::kDead && node[i][j] > Map::kEmpty)
                    if (qAbs(i - head.x) + qAbs(j - head.y) < min) {
                        min = qAbs(i - head.x) + qAbs(j - head.y);
                        minpos.x = i;
                        minpos.y = j;
                    }

        // find food
        if (min != INF) {
            if (minpos.x < head.x) {
                if (dir == Snake::kRight)
                    dir = Snake::kDown;
                else
                    dir = Snake::kLeft;
            } else if (minpos.x > head.x) {
                if (dir == Snake::kLeft)
                    dir = Snake::kUp;
                else
                    dir = Snake::kRight;
            } else if (minpos.y < head.y) {
                if (dir == Snake::kDown)
                    dir = Snake::kLeft;
                else
                    dir = Snake::kUp;
            } else if (minpos.y > head.y) {
                if (dir == Snake::kUp)
                    dir = Snake::kRight;
                else
                    dir = Snake::kDown;
            }
        }
        snake_->ChangeDirection(dir);

        // check dead
        auto next = snake_->GetNextPos();
        if (node[next.x][next.y] > Map::kDead) {
            snake_->ChangeDirection(Snake::kUp);
            next = snake_->GetNextPos();
            if (node[next.x][next.y] < Map::kDead)
                return;
            snake_->ChangeDirection(Snake::kRight);
            next = snake_->GetNextPos();
            if (node[next.x][next.y] < Map::kDead)
                return;
            snake_->ChangeDirection(Snake::kDown);
            next = snake_->GetNextPos();
            if (node[next.x][next.y] < Map::kDead)
                return;
            snake_->ChangeDirection(Snake::kLeft);
            next = snake_->GetNextPos();
            if (node[next.x][next.y] < Map::kDead)
                return;
        }
    });
    ai_timer_.start(30);
}

/**
 * Stop player.
 * @param map map
 */
void AIPlayer::Stop(Map* map) {
    ai_timer_.stop();
    ai_timer_.disconnect();

    Player::Stop(map);
}

/**
 * Pause player.
 * @param pause true for pause
 */
void AIPlayer::Pause(bool pause /* = true*/) {
    if (pause)
        ai_timer_.stop();
    else
        ai_timer_.start();
    Player::Pause(pause);
}
