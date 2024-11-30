#include "mancha.h"
#include "protagonista.h"
Mancha::Mancha(qreal _x, qreal _y, QGraphicsScene* scene, QString sprite, QObject* parent)
    : QObject(parent) {
    setPixmap(QPixmap(sprite));
    setScale(0.5);
    x=_x;
    y=_y;
    setPos(x, y);
    scene->addItem(this);

    ColTim = new QTimer(this);
    connect(ColTim, &QTimer::timeout, this, &Mancha::colision);
    ColTim->start(50);

}

void Mancha::movimiento(int velocidad) {
    x+=velocidad;
    setPos(x,y);
}

void Mancha::colision() {
    QList<QGraphicsItem *> collidingItemsList = collidingItems();
    for (QGraphicsItem *item : collidingItemsList) {
        Protagonista *protagonista = dynamic_cast<Protagonista *>(item);
        if(protagonista){
            int posX=protagonista->getPosicionX();
            int posY=protagonista->getPosicionY();
            if((x >= posX && x <= posX+292) && (y >= posY+355 && y <= posY+450)) {
                protagonista->setPuntos(1);
                eliminar();
                break;
            }
        }
    }
}

void Mancha::eliminar() {
    if (scene()) {
        scene()->removeItem(this);
    }
    emit desaparecida(this);
    QTimer::singleShot(0, this, [this]() {
        delete this;
    });
}


