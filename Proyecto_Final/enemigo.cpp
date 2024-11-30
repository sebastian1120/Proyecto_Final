#include "enemigo.h"
#include "mancha.h"
#include "protagonista.h"
#include <QGraphicsTextItem>
#include <QRandomGenerator>
#include <cmath>
#include <QtMath>

Enemigo::Enemigo(int _coorX, int _coorY, int _AnchoFotog, int _AltoFotog, const QString &rutaSprite, qreal _x, qreal _y, float escala, const QString &_rutaSpriteMancha, QGraphicsScene* _escena, Protagonista* _protagonista, QObject* parent)
    : Personaje(_coorX, _coorY, _AnchoFotog, _AltoFotog), x(_x), y(_y), direccionActual(1), escena(_escena), protagonista(_protagonista) {

    imgSprites.load(rutaSprite);
    sprite = imgSprites.copy(coorX, coorY, AnchoFotog, AltoFotog);
    setPixmap(sprite);
    setScale(escala);
    rutaSpriteMancha = _rutaSpriteMancha;
}

void Enemigo::crearMancha(int desfaseX, int desfaseY) {
    if (escena) {
        Mancha* nuevaMancha = new Mancha(x + desfaseX, y + desfaseY, escena, rutaSpriteMancha, this);
        connect(nuevaMancha, &Mancha::desaparecida, this, &Enemigo::eliminarManchaDeLista);
        manchas.append(nuevaMancha);
        start++;
    }
}

void Enemigo::eliminarManchaDeLista(Mancha* mancha) {
    manchas.removeAll(mancha);
}





void Enemigo::verificarManchas() {
    QList<QGraphicsItem*> items = escena->items();
    bool hayManchas = false;

    for (QGraphicsItem* item : items) {
        if (dynamic_cast<Mancha*>(item)) {
            if(start>=3){
                hayManchas = true;
            }
            break;
        }
    }
    if(start>=3){
        if (!hayManchas && protagonista) {
            protagonista->setVictoria(true);
        }
    }
}





void Enemigo::movimiento(int dirX, int dirY, bool esVertical) {
    if (esVertical) {
        if (y < 190) {
            y = 190;
        } else if (y > 500) {
            y = 500;
        }
    } else {
        if (y < 200) {
            y = 200;
        } else if (y > 530) {
            y = 530;
        }
    }

    if (x > 1340) {
        x = 1340;
    } else if (x < -10) {
        x = -10;
    }

    x += dirX;
    y += dirY;
    setPos(x, y);
}

void Enemigo::eliminarManchas() {
    QList<Mancha*> manchasCopia = manchas;
    for (Mancha* mancha : manchasCopia) {
        mancha->eliminar();
    }
    manchas.clear();
}


void Enemigo::movimiento() {
    if (x <= 900) {
        velocidad=6;
    } else if (x >= 1200) {
        velocidad=-6;
    }
    x+=velocidad;
    setPos(x,y);
    for (Mancha* mancha : manchas) {
        mancha->movimiento(velocidad);
    }

}

void Enemigo::movCirculo()
{
    alCentro=true;
    const int objetivoX = 580;
    const int objetivoY = 350;

    QTimer* movimientoTimer = new QTimer(this);

    connect(movimientoTimer, &QTimer::timeout, this, [this, movimientoTimer]() {
        const int pasoX = 10;
        const int pasoY = 10;

        if (x < objetivoX) {
            x = (x + pasoX > objetivoX) ? objetivoX : x + pasoX;
        } else if (x > objetivoX) {
            x = (x - pasoX < objetivoX) ? objetivoX : x - pasoX;
        }

        if (y < objetivoY) {
            y = (y + pasoY > objetivoY) ? objetivoY : y + pasoY;
        } else if (y > objetivoY) {
            y = (y - pasoY < objetivoY) ? objetivoY : y - pasoY;
        }

        if(x < objetivoX && y == objetivoY) {
            animacionMov(8, 315);
        }
        else if(x > objetivoX && y == objetivoY){
            animacionMov(8, 210);
        } else if(x == objetivoX && y < objetivoY) {
            animacionMov(8, 315);
        } else if (x == objetivoX && y > objetivoY) {
            animacionMov(8, 105);
        } else if(x < objetivoX) {
            if(y > objetivoY) {
                animacionMov(8, 105);
            }
            else{
                animacionMov(8, 315);
            }
        }
        else{
            if(y > objetivoY) {
                animacionMov(8, 0);
            }
            else{
                animacionMov(8, 210);
            }
        }
        setPos(x, y);

        if (x == objetivoX && y == objetivoY) {
            movimientoTimer->stop();
            MOVCircular();
            movimientoTimer->deleteLater();
        }
    });

    movimientoTimer->start(50);
}

void Enemigo::MOVCircular() {
    const int centroX = 580;
    const int centroY = 350;
    const int radio = 100;
    const int velocidad = 5;

    QTimer* circuloTimer = new QTimer(this);
    QTimer* detencionTimer = new QTimer(this);

    static int angulo = 0;

    connect(circuloTimer, &QTimer::timeout, this, [this]() {

        double radianes = qDegreesToRadians(static_cast<double>(angulo));

        x = centroX + radio * cos(radianes);
        y = centroY + radio * sin(radianes);

        setPos(x, y);

        if (angulo > 0 && angulo <= 90) {
            animacionMov(8, 210);
        } else if (angulo > 90 && angulo <= 180) {
            animacionMov(8, 0);
        } else if (angulo > 180 && angulo <= 270) {
            animacionMov(8, 105);
        } else {
            animacionMov(8, 315);
        }
        angulo = (angulo + velocidad) % 360;
    });

    connect(detencionTimer, &QTimer::timeout, this, [circuloTimer, detencionTimer]() {
        circuloTimer->stop();
        detencionTimer->stop();
        detencionTimer->deleteLater();
    });

    circuloTimer->start(50);
    detencionTimer->start(10000);
}



void Enemigo::movAutomatico() {
    if(!alCentro){
        if (x >= 1340 || x <= -10 || y >= 530 || y <= 190) {
            cambiarDireccion();
        }

        switch (direccionActual) {
        case 1:
            animacionMov(8, 315);
            movimiento(12, 0, false);
            break;
        case 2:
            animacionMov(8, 210);
            movimiento(-12, 0, false);
            break;
        case 3:
            animacionMov(8, 105);
            movimiento(8, -12, true);
            break;
        case 4:
            animacionMov(8, 0);
            movimiento(-8, -12, true);
            break;
        case 5:
            animacionMov(8, 315);
            movimiento(8, 12, true);
            break;
        case 6:
            animacionMov(8, 210);
            movimiento(-8, 12, true);
            break;
        }
    }
}

void Enemigo::cambiarDireccionVertical() {
    if(!alCentro){
        direccionActual = QRandomGenerator::global()->bounded(3, 7);
    }
}

int Enemigo::getPosicionY() {
    return y;
}


void Enemigo::cambiarDireccion() {
    direccionActual = QRandomGenerator::global()->bounded(1, 7);
}

void Enemigo::setAlCentro(bool centro)
{
    alCentro = centro;
}

QList<Mancha*> Enemigo::getListaManchas() const
{
    return manchas;
}

