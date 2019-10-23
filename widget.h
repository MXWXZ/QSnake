#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include "player.h"
#include "map.h"

class Player;

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

private:
    void StopGame();

private:
    Map map_;                  //!< map
    QVector<Player*> player_;  //!< player
    QTimer food_timer;         //!< food timer
    QTimer foodex_timer;       //!< foodex timer
    int start_ = 0;            //!< game start
    bool gameover_ = false;    //!< game over
};
#endif  // WIDGET_H
