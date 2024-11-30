#ifndef MANCHA_H
#define MANCHA_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

class Mancha : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Mancha(qreal _x, qreal _y, QGraphicsScene* scene, QString sprite, QObject* parent = nullptr);
    void colision();
    void eliminar();
    void movimiento(int velocidad);

signals:
    void desaparecida(Mancha* mancha);

private:
    QTimer *ColTim;
    qreal x;
    qreal y;
};

#endif // MANCHA_H
