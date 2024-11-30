#include "protagonista.h"
#include "potenciador.h"
#include "QKeyEvent"
#include "QGraphicsTextItem"

Protagonista::Protagonista(int _coorX, int _coorY, int _AnchoFotog, int _AltoFotog, const QString &rutaSprite, qreal _x, qreal _y, float escala, int _puntos, MainWindow *parent)
    :Personaje(_coorX, _coorY,_AnchoFotog,_AltoFotog)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    imgSprites.load(rutaSprite);
    sprite = imgSprites.copy(coorX,coorY,AnchoFotog,AltoFotog);
    setPixmap(sprite);
    setScale(escala);
    x=_x;
    y=_y;
    puntos=_puntos;


}

void Protagonista::keyPressEvent(QKeyEvent *event) {
    if (inicio) {
        switch (event->key()) {
        case Qt::Key_A:
            direccion = false;
            movimiento(-velocidad, 0);
            animacionMov(8, 100);
            break;
        case Qt::Key_D:
            direccion = true;
            movimiento(velocidad, 0);
            animacionMov(8, 200);
            break;
        case Qt::Key_W:
            if (direccion) {
                movimiento(velocidad, -velocidad);
                animacionMov(8, 300);
            } else {
                movimiento(-velocidad, -velocidad);
                animacionMov(8, 0);
            }
            break;
        case Qt::Key_S:
            if (direccion) {
                movimiento(velocidad, velocidad);
                animacionMov(8, 200);
            } else {
                movimiento(-velocidad, velocidad);
                animacionMov(8, 100);
            }
            break;
        default:
            break;
        }
        QList<QGraphicsItem*> colisiones = collidingItems();
        for (QGraphicsItem* item : colisiones) {
            Potenciador* potenciador = dynamic_cast<Potenciador*>(item);
            if (potenciador) {
                emit potenciador->recogido();
            }
        }
    }
    if (!animacionDisparo) {
        if (event->key() == Qt::Key_Space) {

            if (m_arma && m_arma->isObjetoVisible()) {
                return;
            }

            if (m_arma && primerDisparo) {
                primerDisparo = false;
                m_parabolicTimer->stop();
            } else if (m_arma) {
                primerDisparo = true;

                animacionMovContador = 0;
                animacionMovTimer = new QTimer(this);
                connect(animacionMovTimer, &QTimer::timeout, this, [this]() {
                    animacionMov(8, 0);
                    animacionMovContador++;

                    if (animacionMovContador >= 8) {
                        animacionMovTimer->stop();
                        sprite = imgSprites.copy(0,0,AnchoFotog,AltoFotog);
                        setPixmap(sprite);
                        animacionMovTimer->deleteLater();
                    }
                    if(animacionMovContador == 4) {
                        disparoTimer->start(10);
                    }
                });

                animacionMovTimer->start(150);
            }
        }
    }
}

void Protagonista::crearParabola() {
    m_arma = new Arma(scene(), x + 210, y + 95, 1200, 700);
    m_parabolicVelocidad = 91;
    m_aumentando = true;

    connect(m_arma, &Arma::objetoReachedTarget, this, &Protagonista::onObjetoReachedTarget);

    connect(m_arma, &Arma::objetoInvisible, this, [this]() {
        if (m_parabolicTimer) {
            m_parabolicTimer->start(30);
        }
    });

    connect(m_arma, &Arma::objetoPuntos, this, [this](){
        setPuntos(8);
        setCanasta();
        if(cestas>=3 && vida<=0){
            victoria=true;
        }

    });

    connect(m_arma, &Arma::objetoVida, this, [this](){
        setVida();
    });

    m_parabolicTimer = new QTimer(this);
    connect(m_parabolicTimer, &QTimer::timeout, this, [this]() {
        m_arma->actualizarTrayectoria(m_parabolicVelocidad);

        if (m_aumentando) {
            m_parabolicVelocidad += 1.0;
            if (m_parabolicVelocidad >= 91.0) {
                m_aumentando = false;
            }
        } else {
            m_parabolicVelocidad -= 1.0;
            if (m_parabolicVelocidad <= 75.0) {
                m_aumentando = true;
            }
        }
    });

    disparoTimer = new QTimer(this);
    connect(disparoTimer, &QTimer::timeout, this, [this]() {
        if (m_arma) {
            m_arma->Disparo();
        }
    });
    m_parabolicTimer->start(30);
}

void Protagonista::onObjetoReachedTarget() {
    if (disparoTimer) {
        disparoTimer->stop();
    }
}


void Protagonista::movimiento(int dirX, int dirY) {

    if (x > 1300) {
        x = 1300;
    } else if (x < -40) {
        x = -40;
    }
    if(y < 30){
        y = 30;
    } else if(y > 400){
        y = 400;
    }
    x += dirX;
    y += dirY;
    setPos(x, y);

}

void Protagonista::AnimacionInicio() {
    if (anim == nullptr) {
        anim = new QTimer(this);
        connect(anim, &QTimer::timeout, this, &Protagonista::actualizarAnimacionInicio);
        anim->start(50);
    }
}

void Protagonista::actualizarAnimacionInicio() {
    x += 10;
    y -= 15;

    setPos(x, y);
    animacionMov(8, 200);

    if (y <= 300) {
        inicio=true;
        anim->stop();
        anim->deleteLater();
        anim = nullptr;
    }
}

int Protagonista::getPosicionY()
{
    return y;
}

int Protagonista::getPosicionX()
{
    return x;
}

int Protagonista::getVelocidad() const
{
    return velocidad;
}

int Protagonista::getPuntos()
{
    return puntos;
}

int Protagonista::getVida()
{
    return vida;
}

bool Protagonista::getEstadoVD()
{
    return victoria;
}

int Protagonista::getCestas()
{
    return cestas;
}

void Protagonista::setPuntos(int _puntos)
{
    puntos += _puntos;
}

void Protagonista::setVelocidad(int _velocidad)
{
    velocidad = _velocidad;
}

void Protagonista::setVida()
{
    vida--;
}

void Protagonista::setVictoria(bool estado) {
    victoria = estado;
}

void Protagonista::setCanasta()
{
    cestas++;
}
