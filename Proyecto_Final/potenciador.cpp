#include "potenciador.h"
#include <QRandomGenerator>

Potenciador::Potenciador(QGraphicsScene* escena, int _inicioX, int _inicioY, int _limiteX, int _limiteY)
    : escena(escena)
{
    setPixmap(QPixmap(":/Imagenes/energia.png"));
    setScale(0.2);
    inicioX = _inicioX;
    inicioY = _inicioY;
    limiteX = _limiteX;
    limiteY = _limiteY;
    int x = QRandomGenerator::global()->bounded(inicioX, limiteX);
    int y = QRandomGenerator::global()->bounded(inicioY, limiteY);

    setPos(x, y);
    escena->addItem(this);

    reaparicionTimer = new QTimer(this);
    reaparicionTimer->setSingleShot(true);
    connect(reaparicionTimer, &QTimer::timeout, this, &Potenciador::reaparecer);
}

void Potenciador::activar()
{
    hide();
    reaparicionTimer->start(15000);
}

void Potenciador::desactivar()
{
    show();
}

void Potenciador::reaparecer()
{
    int x = QRandomGenerator::global()->bounded(inicioX, limiteX);
    int y = QRandomGenerator::global()->bounded(inicioY, limiteY);
    setPos(x, y);
    desactivar();
}
