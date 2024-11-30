#ifndef ARMA_H
#define ARMA_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsPixmapItem>
#include <QPainterPath>

class Arma : public QObject
{
    Q_OBJECT

public:
    explicit Arma(QGraphicsScene* scene, qreal startX, qreal startY, qreal targetX, qreal targetY);

    void actualizarTrayectoria(qreal velocidad);
    void recalcularTrayectoria();
    void Disparo();
    bool isObjetoVisible() const;

signals:
    void objetoReachedTarget();
    void objetoInvisible();
    void objetoPuntos();
    void objetoVida();

private:
    QGraphicsScene* m_scene;
    QGraphicsPathItem* m_pathItem;
    QGraphicsPixmapItem* m_objeto;
    QPainterPath m_parabolicPath;

    qreal m_startX;
    qreal m_startY;
    qreal m_targetX;
    qreal m_targetY;
    qreal m_velocidad = 91.0;
    qreal tiempoDisparo = 0;

    const qreal gravedad = 9.8;

};

#endif // ARMA_H
