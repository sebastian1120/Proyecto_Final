#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

#include "Personaje.h"
#include "QKeyEvent"
#include "QGraphicsTextItem"
#include <QTimer>
#include "arma.h"

class MainWindow;

class Protagonista : public Personaje
{
    Q_OBJECT
public:
    Protagonista(int _coorX, int _coorY, int _AnchoFotog, int _AltoFotog, const QString &rutaSprite, qreal _x, qreal _y, float escala, int _puntos, MainWindow *parent = nullptr);
    void keyPressEvent(QKeyEvent *event);
    void movimiento(int dirX, int dirY);
    void AnimacionInicio();
    void actualizarAnimacionInicio();
    void setPuntos(int _puntos);
    void setVelocidad(int _velocidad);
    void setVida();
    void setVictoria(bool estado);
    void setCanasta();
    int getCestas();
    int getPosicionY();
    int getPosicionX();
    int getPuntos();
    int getVelocidad() const;
    int getVida();
    bool getEstadoVD();
    void crearParabola();

signals:
    void colisionConPotenciador();

private slots:
    void onObjetoReachedTarget();
private:
    MainWindow *mainWindow;

    bool direccion; //true va a ser derecha, false izquierda
    bool inicio = false;
    bool m_aumentando = true;
    bool primerDisparo = true;
    bool animacionDisparo = false;
    bool victoria = false;

    qreal x;
    qreal y;
    qreal m_parabolicVelocidad;

    int puntos=0;
    int velocidad = 6;
    int animacionMovContador;
    int vida=4;
    int cestas=0;

    QTimer* m_parabolicTimer;
    QTimer* disparoTimer;
    QTimer* animacionMovTimer;
    QTimer* anim = nullptr;

    Arma* m_arma;



};

#endif // PROTAGONISTA_H
