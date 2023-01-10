#ifndef SQUARE_H
#define SQUARE_H

#include <QColor>
#include <QGraphicsItem>
#include <QPainter>

#include <iostream>

class Square : public QGraphicsRectItem
{
public:
    Square(QColor color, int x, int y, int size = 20);
    void uncheck();
    void check();
    QColor getColor();
    bool isAvaliable();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    QGraphicsEllipseItem *pawn;
    QGraphicsEllipseItem *getPawn();
    void removePawn();

private:
    bool toUse;
    QColor color;

    int x;
    int y;
    int size;
};

#endif // SQUARE_H
