#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "player.h"

#define INF 100000000

class AIPlayer : public Player {
public:
    AIPlayer();
    void StartPlayer(Map* map, Widget* base) override;
    void Stop(Map* map) override;
    void Pause(bool pause = true) override;

private:
    QTimer ai_timer_;  //!< ai timer
};

#endif  // AIPLAYER_H
