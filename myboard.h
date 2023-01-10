#ifndef MYBOARD_H
#define MYBOARD_H

#include "square.h"
#include "pawn.h"

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QThread>
#include <QParallelAnimationGroup>

#include <iostream>
#include <vector>

class MyBoard : QObject
{
    Q_OBJECT
public:
    MyBoard(QGraphicsScene *scene);
    ~MyBoard();
    void squareClicked(int x, int y);
    std::pair<int, int> getSquarePos(int x, int y);
    void uncheck(std::pair<int, int>);
    void check(std::pair<int, int>);

    void move(int x, int y, int id);
    void addBlackPawn(int x, int y, int id);
    void addWhitePawn(int x, int y, int id);
    void changeToQueen(int x, int y);
    void removePawn(int id);

    int getPawnID(int x, int y);

    void updateField(int x, int y);

private:
    int size;

    Square *squares[8][8];
    Square *getSquare(int x, int y);
    Square *choosenSquare;
    std::vector<Pawn *>::iterator iterator;
    std::vector<Pawn *> pawns;
    QGraphicsScene *scene;
    QPropertyAnimation *animation;
    int animationID;
    int newX;
    int newY;
    void changeCoordinates(int x, int y);

    int animationTime = 200;
};

#endif // MYBOARD_H
