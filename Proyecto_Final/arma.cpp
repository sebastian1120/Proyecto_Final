#include "arma.h"
#include <QPen>
#include <cmath>
#include <cstdlib>
#include "mancha.h"

Arma::Arma(QGraphicsScene* scene, qreal startX, qreal startY, qreal targetX, qreal targetY)
    : m_scene(scene), m_startX(startX), m_startY(startY),
    m_targetX(targetX), m_targetY(targetY)
{
    m_parabolicPath = QPainterPath();
    m_pathItem = m_scene->addPath(m_parabolicPath, QPen(Qt::red, 2, Qt::DashLine));
    recalcularTrayectoria();

    m_objeto = new QGraphicsPixmapItem(QPixmap(":/Imagenes/saco.png"));
    m_objeto->setPos(m_startX-50, m_startY-45);
    m_objeto->hide();
    m_scene->addItem(m_objeto);

    tiempoDisparo = 0;
}

void Arma::actualizarTrayectoria(qreal velocidad)
{
    m_velocidad = velocidad;
    recalcularTrayectoria();
}

void Arma::recalcularTrayectoria()
{
    m_parabolicPath = QPainterPath();
    m_parabolicPath.moveTo(m_startX, m_startY);
    qreal tiempoMax = 5 * m_velocidad * std::sin(M_PI / 4) / gravedad;
    qreal pasoTiempo = 0.1;
    for (qreal t = 0; t <= tiempoMax; t += pasoTiempo) {
        qreal x = m_startX + m_velocidad * std::cos(M_PI / 4) * t;
        qreal y = m_startY - (m_velocidad * std::sin(M_PI / 4) * t - 0.5 * gravedad * t * t);
        if (y >= 680) {
            break;
        }
        m_parabolicPath.lineTo(x, y);
    }
    m_scene->removeItem(m_pathItem);
    delete m_pathItem;
    m_pathItem = m_scene->addPath(m_parabolicPath, QPen(Qt::red, 10, Qt::DashLine));
}

bool Arma::isObjetoVisible() const {
    return m_objeto->isVisible();
}

void Arma::Disparo()
{
    if (!m_objeto->isVisible()) {
        m_objeto->show();
        tiempoDisparo = 0;
    }

    tiempoDisparo += 0.1;
    qreal x = (m_startX-50) + m_velocidad * std::cos(M_PI/4) * tiempoDisparo;
    qreal y = (m_startY-45) - (m_velocidad * std::sin(M_PI/4) * tiempoDisparo - 0.5 * gravedad * tiempoDisparo * tiempoDisparo);

    m_objeto->setPos(x, y);

    QList<QGraphicsItem*> colisiones = m_objeto->collidingItems();
    for (QGraphicsItem* item : colisiones) {
        Mancha* mancha = dynamic_cast<Mancha*>(item);
        if (mancha) {
            m_objeto->hide();
            m_objeto->setPos(m_startX-50, m_startY-45);
            tiempoDisparo = 0;
            emit objetoReachedTarget();
            emit objetoInvisible();
            emit objetoVida();
            emit objetoPuntos();
            return;
        }
    }

    if (y >= 800) {
        m_objeto->hide();
        m_objeto->setPos(m_startX-50, m_startY-45);
        tiempoDisparo = 0;

        emit objetoReachedTarget();
        emit objetoVida();
        emit objetoInvisible();
    }
}
