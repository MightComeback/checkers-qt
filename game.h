#ifndef GAME_H
#define GAME_H

#include <vector>
#include <cstdlib>
#include <QGraphicsScene>
#include <QSettings>
#include <QMessageBox>

#include "myboard.h"

class Game
{
    enum field
    {
        EMPTY,
        BLOCKED,
        WHITE,
        BLACK,
        WHITE_QUEEN,
        BLACK_QUEEN
    };

public:
    Game(QGraphicsScene *scene);
    ~Game();
    void squareClicked(int x, int y);
    bool getGameOverFlag();

private:
    MyBoard *board;
    field fields[8][8];
    std::vector<int> pawns; // зберігання індексів живих фігур
    int maxID;
    bool isChecked;
    bool whiteTurn;
    bool beatingFlag;
    std::pair<int, int> checked;

    int whitePawnsCounter;
    int blackPawnsCounter;
    bool gameOverFlag;

    bool moveIsPossible(std::pair<int, int> from);
    bool beatingIsPossible(std::pair<int, int> from);
    bool move(std::pair<int, int> from, std::pair<int, int> to);
    bool beat(std::pair<int, int> from, std::pair<int, int> to);

    void move(int x1, int y1, int x2, int y2);
    void addBlackPawn(int x, int y);
    void addWhitePawn(int x, int y);
    void changeToQueen(std::pair<int, int> p);
    void removePawn(int x, int y);

    void changeTurn();
    bool isGameOver();

    const int BOARD_SIZE = 8;

    int lastMove = 100;
    int lastBeating = 25;
};

#endif // GAME_H
