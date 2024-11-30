#ifndef POTENCIADOR_H
#define POTENCIADOR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

class Potenciador : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Potenciador(QGraphicsScene* escena, int iniacioX, int inicioY, int limiteX, int limiteY);
    void activar();
    void desactivar();

signals:
    void recogido();

private:
    QGraphicsScene* escena;
    QTimer* reaparicionTimer;
    int limiteX, limiteY, inicioX, inicioY;

public slots:
    void reaparecer();
};

#endif // POTENCIADOR_H
