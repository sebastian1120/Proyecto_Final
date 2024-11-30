#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "protagonista.h"
#include "QGraphicsTextItem"
#include <QTimer>
#include "mancha.h"

class MainWindow;

class Enemigo : public Personaje {
    Q_OBJECT
public:
    Enemigo(int _coorX, int _coorY, int _AnchoFotog, int _AltoFotog, const QString &rutaSprite,
            qreal _x, qreal _y, float escala, const QString &_rutaSpriteMancha,
            QGraphicsScene* escena, Protagonista* protagonista = nullptr, QObject* parent = nullptr);

    void movimiento(int dirX, int dirY, bool esVertical);
    void movimiento();
    void movAutomatico();
    void movCirculo();
    void MOVCircular();
    void cambiarDireccionVertical();
    int getPosicionY();
    QList<Mancha*> getListaManchas() const;
    void setAlCentro(bool centro);
    void cambiarDireccion();
    void crearMancha(int desfaseX, int desfaseY);
    void eliminarManchas();
    QList<Mancha*> manchas;
    void verificarManchas();


signals:

private slots:
    void eliminarManchaDeLista(Mancha* mancha);
private:
    MainWindow *mainWindow;
    qreal x;
    qreal y;
    int velocidad;
    QString rutaSpriteMancha;
    bool alCentro=false;
    short int direccionActual;
    QGraphicsScene* escena;
    Protagonista* protagonista;
    int start=0;
    bool verificacionFinalizada = false; // Indica si ya se verificaron las manchas después de las 3 primeras.


};

#endif // ENEMIGO_H
