#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Personaje(int _coorX, int _coorY, int _AnchoFotog, int _AltoFotog);

    void animacionMov(int columna, int fila);

protected:
    int coorX;
    int coorY;
    int AnchoFotog;
    int AltoFotog;
    int cont = 0;
    QPixmap imgSprites;
    QPixmap sprite;
};

#endif // PERSONAJE_H
