#include "pawn.h"

Pawn::Pawn(int x, int y, int size, QColor color, int id)
{
    isQueen = false;
    this->x = x;
    this->y = y;
    this->size = size;
    this->color = color;
    this->id = id;
    setZValue(10);
}
Pawn::Pawn(Pawn &pawn)
{
    crown = pawn.crown;
    color = pawn.color;
    x = pawn.x;
    y = pawn.y;
    size = pawn.size;
    id = pawn.id;
    isQueen = pawn.isQueen;
}

Pawn::~Pawn() {}
int Pawn::getID()
{
    return id;
}
int Pawn::getX()
{
    return x;
}
int Pawn::getY()
{
    return y;
}
void Pawn::setX(int x)
{
    this->x = x;
}
void Pawn::setY(int y)
{
    this->y = y;
}

QPoint Pawn::pos()
{
    return QPoint(x, y);
}

void Pawn::setPos(QPoint p)
{
    setX(p.x());
    setY(p.y());
    update();
}

void Pawn::setQueen()
{
    isQueen = true;
    update();
}

void Pawn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = QRectF(x + size / 8, y + size / 8, 3 * size / 4, 3 * size / 4);

    QColor color;
    if (this->color == Qt::black)
    {
        QSettings settings("ZUNP KNT-211", "CHECKERS");

        color = settings.value("blackPawn", QColor(Qt::black)).value<QColor>();
    }
    else
    {
        QSettings settings("ZUNP KNT-211", "CHECKERS");

        color = settings.value("whitePawn", QColor(Qt::black)).value<QColor>();
    }

    QBrush brush(color);
    painter->setPen(color);
    painter->setBrush(color);
    if (color == Qt::black)
        painter->setPen(Qt::white);
    painter->drawEllipse(rec);

    if (isQueen)
    {
        crown.clear();
        painter->setPen(QPen(Qt::yellow));
        painter->drawEllipse(rec);
    }
}

QRectF Pawn::boundingRect() const
{
    return QRectF(x, y, size, size);
}

void Pawn::changeOpacity(qreal opacity)
{
    setOpacity(opacity);
    setZValue(10);
    prepareGeometryChange();
    emit updateOpacity();
}
