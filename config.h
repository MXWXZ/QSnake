#ifndef CONFIG_H
#define CONFIG_H

#define WINDOW_WIDTH 800   //!< window width
#define WINDOW_HEIGHT 600  //!< window height
#define NODE_WIDTH 10      //!< node width
#define NODE_HEIGHT 10     //!< node height
#define FOOD_DELAY 3000    //!< food delay ms
#define FOODEX_DELAY 6000  //!< foodex delay ms
#define LEVEL1_DELAY 200   //!< level1 delay ms
#define LEVEL2_DELAY 100   //!< level2 delay ms
#define LEVEL3_DELAY 50    //!< level3 delay ms
#define LEVEL1_LEN 10      //!< level1 length
#define LEVEL2_LEN 20      //!< level2 length
#define LEVEL3_LEN 30      //!< level3 length
#define MAX_PLAYER 3       //!< max player

struct Pos {
    Pos(int x, int y) : x(x), y(y) {}
    bool operator==(const Pos& pos) const { return x == pos.x && y == pos.y; }
    int x;
    int y;
};

#endif  // CONFIG_H
