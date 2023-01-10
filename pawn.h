#ifndef PAWN_H
#define PAWN_H

#include <QColor>
#include <QGraphicsItem>
#include <QPainter>
#include <QSettings>

#include <iostream>

Q_GLOBAL_STATIC(QColor, blackPawnColor);
Q_GLOBAL_STATIC(QColor, whitePawnColor);

class Pawn : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QPoint pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    Pawn(int x, int y, int size, QColor color, int id);
    Pawn(Pawn &pawn);
    ~Pawn();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    int getID();
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    void setQueen();

    QPoint pos();
    void changeOpacity(qreal opacity);
    void setPos(QPoint p);
signals:
    void updateOpacity();

private:
    QPolygonF crown;

    QColor color;
    int x;
    int y;
    int size;
    int id;

    bool isQueen;
};

#endif // PAWN_H
