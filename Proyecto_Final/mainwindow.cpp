#include "mainwindow.h"
#include "potenciador.h"
#include "ui_mainwindow.h"
#include "protagonista.h"
#include "enemigo.h"
#include <QTimer>
#include <QFontDatabase>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1599,899);
    this->vista = new QGraphicsView(this);
    vista->resize(1599, 899);
    Menu();
    vista->show();
    idFont = QFontDatabase::addApplicationFont(":/fuentes/Simpsonfont.ttf");
    FuenteSimpson = QFontDatabase::applicationFontFamilies(idFont).at(0);
}


void MainWindow::Nivel1() {
    vista->setScene(nullptr);

    lvl1 = new QGraphicsScene(this);

    QImage background(":/Imagenes/sala.jpg");
    lvl1->setBackgroundBrush(QBrush(background));
    lvl1->setSceneRect(0, 0, background.width(), background.height());

    vista->setScene(lvl1);
    vista->resize(background.width() + 4, background.height() + 4);


    Protagonista* Marge = new Protagonista(0, 0, 75, 100, ":/Imagenes/marge.png", 0, 850, 5, 0);
    lvl1->addItem(Marge);
    Marge->setPos(0, 850);
    Marge->setFlag(QGraphicsItem::ItemIsFocusable);
    Marge->setFocus();

    Enemigo* Lisa = new Enemigo(0, 0, 73, 105, ":/Imagenes/lisa.png", 1300, 200, 3.5, ":/Imagenes/mancha.png", lvl1, Marge);
    lvl1->addItem(Lisa);
    Lisa->setPos(1300, 200);


    QTimer* timer = new QTimer(this);
    QTimer* MovLisa = new QTimer(this);
    QTimer* timerCambioDireccion = new QTimer(this);
    QTimer* timerManchas = new QTimer(this);
    QTimer* movCircular = new QTimer(this);
    QTimer* ManchasActual = new QTimer(this);

    connect(movCircular, &QTimer::timeout, this, [Lisa]() {
        Lisa->movCirculo();

        QTimer::singleShot(10000, [Lisa]() {
            Lisa->setAlCentro(false);
        });
    });


    connect(ManchasActual, &QTimer::timeout, this, [Lisa]() {
        Lisa->verificarManchas();
    });
    connect(MovLisa, &QTimer::timeout, Lisa, &Enemigo::movAutomatico);
    connect(timerManchas, &QTimer::timeout, this, [Lisa]() {
        Lisa->crearMancha(150,300);
    });
    connect(timerCambioDireccion, &QTimer::timeout, this, [Lisa]() {
        Lisa->cambiarDireccionVertical();
    });

    connect(timer, &QTimer::timeout, this, [Lisa, Marge]() {
        int LisaY = Lisa->getPosicionY() + 200;
        int MargeY = Marge->getPosicionY() + 300;

        if (LisaY < MargeY) {
            Lisa->setZValue(1);
            Marge->setZValue(2);
        } else {
            Marge->setZValue(1);
            Lisa->setZValue(2);
        }
    });
    ManchasActual->start(50);
    Marge->AnimacionInicio();
    MovLisa->start(50);
    timer->start(50);
    timerCambioDireccion->start(3000);
    timerManchas->start(500);
    movCircular->start(40000);

    QGraphicsPixmapItem* fondoCronometro = new QGraphicsPixmapItem(QPixmap(":/Imagenes/reloj.png"));
    fondoCronometro->setPos(290, 0);
    lvl1->addItem(fondoCronometro);
    fondoCronometro->setScale(0.29);

    QGraphicsTextItem* textoCronometro = new QGraphicsTextItem(fondoCronometro);
    textoCronometro->setDefaultTextColor(Qt::black);
    textoCronometro->setFont(QFont(FuenteSimpson, 190, QFont::Bold));
    textoCronometro->setPlainText("01:00");
    textoCronometro->setPos(300, 10);
    lvl1->addItem(textoCronometro);

    QTimer* cronometroTimer = new QTimer(this);
    int* tiempoRestante = new int(60);

    connect(cronometroTimer, &QTimer::timeout, this, [this, textoCronometro, tiempoRestante, Marge, Lisa]() {
        if (*tiempoRestante > 0) {
            (*tiempoRestante)--;
            int minutos = *tiempoRestante / 60;
            int segundos = *tiempoRestante % 60;
            textoCronometro->setPlainText(QString("%1:%2")
                                              .arg(minutos, 2, 10, QChar('0'))
                                              .arg(segundos, 2, 10, QChar('0')));

            if (*tiempoRestante == 0 || Marge->getEstadoVD()) {
                detenerTimers();
                Lisa->eliminarManchas();
                QList<QGraphicsItem*> items = lvl1->items();
                for (QGraphicsItem* item : items) {
                    lvl1->removeItem(item);
                    delete item;
                }
                if (Marge->getEstadoVD()) {
                    bloqLvl2 = false;

                    QGraphicsPixmapItem* resultadoImagen = new QGraphicsPixmapItem(
                        QPixmap(":/Imagenes/victoria.png")
                        );
                    resultadoImagen->setPos((lvl1->width() - resultadoImagen->pixmap().width()) / 2,
                                            (lvl1->height() - resultadoImagen->pixmap().height()) / 2);
                    lvl1->addItem(resultadoImagen);
                    resultadoImagen->setZValue(5);

                } else{
                    bloqLvl2 = true;
                    QGraphicsPixmapItem* resultadoImagen = new QGraphicsPixmapItem(
                        QPixmap(":/Imagenes/derrota.png")
                        );
                    resultadoImagen->setPos((lvl1->width() - resultadoImagen->pixmap().width()) / 2,
                                            (lvl1->height() - resultadoImagen->pixmap().height()) / 2);
                    lvl1->addItem(resultadoImagen);
                    resultadoImagen->setZValue(5);

                }


                QTimer::singleShot(3000, this, &MainWindow::Menu);
            }
        }
    });

    cronometroTimer->start(1000);



    QGraphicsTextItem* textoPuntos = new QGraphicsTextItem();
    textoPuntos->setDefaultTextColor(Qt::black);
    textoPuntos->setFont(QFont(FuenteSimpson, 50, QFont::Bold));
    textoPuntos->setPlainText(QString("Puntos: %1").arg(Marge->getPuntos()));
    textoPuntos->setPos(900, 10);
    lvl1->addItem(textoPuntos);


    QTimer* actualizarPuntosTimer = new QTimer(this);
    connect(actualizarPuntosTimer, &QTimer::timeout, this, [textoPuntos, Marge, this]() {
        textoPuntos->setPlainText(QString("Puntos: %1").arg(Marge->getPuntos()));
        puntosTotales=Marge->getPuntos();
    });
    actualizarPuntosTimer->start(100);

    int inicioX=0;
    int inicioY=480;
    int limiteX = background.width() - 50;
    int limiteY = background.height() - 80;

    Potenciador* potenciador = new Potenciador(lvl1, inicioX, inicioY, limiteX, limiteY);

    potenciador->setZValue(3);

    connect(potenciador, &Potenciador::recogido, this, [Marge, potenciador]() {
        Marge->setVelocidad(20);
        QTimer::singleShot(5000, [Marge]() {
            Marge->setVelocidad(6);
        });
        potenciador->activar();
        Marge->setPuntos(5);
    });


}


void MainWindow::Nivel2()
{
    puntosIniciales = puntosTotales;
    estadoImagenes.clear();
    vidasCestas.clear();

    estadoImagenes = {"normal.png", "normal.png", "normal.png", "normal.png"};

    vista->setScene(nullptr);

    lvl2 = new QGraphicsScene(this);

    QImage background(":/Imagenes/fondo.png");
    lvl2->setBackgroundBrush(QBrush(background));
    lvl2->setSceneRect(0, 0, background.width(), background.height());

    vista->setScene(lvl2);
    vista->resize(background.width() + 4, background.height() + 4);

    Protagonista* Homero = new Protagonista(0, 0, 132, 160, ":/Imagenes/homero.png", 0, 270, 4, puntosTotales);
    lvl2->addItem(Homero);
    Homero->setPos(0, 270);
    Homero->setFlag(QGraphicsItem::ItemIsFocusable);
    Homero->setFocus();

    Enemigo* Plataforma = new Enemigo(0, 0, 193, 123, ":/Imagenes/superficie.png", 1200, 650, 0.9, ":/Imagenes/circulo.png", lvl2, Homero);
    lvl2->addItem(Plataforma);
    Plataforma->setPos(1200, 650);

    QTimer* movPlataforma = new QTimer(this);
    connect(movPlataforma, &QTimer::timeout, this, [Plataforma]() {
        Plataforma->movimiento();
    });

    Plataforma->crearMancha(68,10);

    Homero->crearParabola();
    QGraphicsTextItem* textoPuntos = new QGraphicsTextItem();
    textoPuntos->setDefaultTextColor(Qt::black);
    textoPuntos->setFont(QFont(FuenteSimpson, 50, QFont::Bold));
    textoPuntos->setPlainText(QString("Puntos: %1").arg(Homero->getPuntos()));
    textoPuntos->setPos(900, 10);
    lvl2->addItem(textoPuntos);


    QTimer* actualizarPuntosTimer = new QTimer(this);
    connect(actualizarPuntosTimer, &QTimer::timeout, this, [textoPuntos, Homero, this]() {
        textoPuntos->setPlainText(QString("Puntos: %1").arg(Homero->getPuntos()));
        puntosTotales=Homero->getPuntos();
    });
    actualizarPuntosTimer->start(100);

    for (int i = 0; i < 4; ++i) {
        QGraphicsPixmapItem* imagen = new QGraphicsPixmapItem(QPixmap(":/Imagenes/normal.png"));
        imagen->setPos(50 + (i * 160), 10);
        lvl2->addItem(imagen);
        vidasCestas.push_back(imagen);
    }

    QTimer* controlVictoriaTimer = new QTimer(this);
    connect(controlVictoriaTimer, &QTimer::timeout, this, [this, Homero]() {
        int vidasActuales = Homero->getVida();
        int cestasActuales = Homero->getCestas();
        for (int i = 0; i < 4; ++i) {
            if (i < 4 - vidasActuales) {
                if (estadoImagenes[i] != "falla.png") {
                    if (cestasActuales > 0) {
                        estadoImagenes[i] = "encesta.png";
                        vidasCestas[i]->setPixmap(QPixmap(":/Imagenes/encesta.png"));
                        cestasActuales--;
                    } else {
                        estadoImagenes[i] = "falla.png";
                        vidasCestas[i]->setPixmap(QPixmap(":/Imagenes/falla.png"));
                    }
                }
            } else {
                estadoImagenes[i] = "normal.png";
                vidasCestas[i]->setPixmap(QPixmap(":/Imagenes/normal.png"));
            }
        }
        if (Homero->getVida() <= 0 || Homero->getEstadoVD()) {
            detenerTimers();
            QList<QGraphicsItem*> items = lvl2->items();
            for (QGraphicsItem* item : items) {
                lvl2->removeItem(item);
                delete item;
            }

            if (Homero->getEstadoVD()) {
                bloqLvl2 = true;
                puntosTotales+=8;
                QFile file("puntaje.txt");
                if (file.open(QIODevice::Append | QIODevice::Text)) {
                    QTextStream out(&file);
                    QDateTime currentDateTime = QDateTime::currentDateTime();
                    out << currentDateTime.toString("yyyy-MM-dd hh:mm:ss")
                        << " puntos: " << puntosTotales << "\n";
                    file.close();
                }

                QGraphicsPixmapItem* resultadoImagen = new QGraphicsPixmapItem(
                    QPixmap(":/Imagenes/victoria.png")
                    );
                resultadoImagen->setPos((lvl2->width() - resultadoImagen->pixmap().width()) / 2,
                                        (lvl2->height() - resultadoImagen->pixmap().height()) / 2);
                lvl2->addItem(resultadoImagen);
                resultadoImagen->setZValue(5);
            } else {
                QGraphicsPixmapItem* resultadoImagen = new QGraphicsPixmapItem(
                    QPixmap(":/Imagenes/derrota.png")
                    );
                resultadoImagen->setPos((lvl2->width() - resultadoImagen->pixmap().width()) / 2,
                                        (lvl2->height() - resultadoImagen->pixmap().height()) / 2);
                puntosTotales = puntosIniciales;
                lvl2->addItem(resultadoImagen);
                resultadoImagen->setZValue(5);
            }

            QTimer::singleShot(3000, this, &MainWindow::Menu);
        }
    });

    controlVictoriaTimer->start(100);

    movPlataforma->start(50);

}

void MainWindow::Menu()
{
    vista->setScene(nullptr);

    escenaMenu = new QGraphicsScene(this);

    QImage MenuF(":/Imagenes/inicio1.jpg");
    escenaMenu->setSceneRect(0, 0, MenuF.width(), MenuF.height());
    escenaMenu->setBackgroundBrush(QBrush(MenuF));

    QGraphicsPixmapItem* scoreFondo = new QGraphicsPixmapItem(QPixmap(":/Imagenes/score.png"));
    scoreFondo->setPos(850, 75);
    escenaMenu->addItem(scoreFondo);

    QFile file("puntaje.txt");
    QVector<QPair<QString, int>> scores;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" puntos: ");
            if (parts.size() == 2) {
                scores.append(qMakePair(parts[0], parts[1].toInt()));
            }
        }
        file.close();

        std::sort(scores.begin(), scores.end(),
                  [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                      return a.first > b.first;
                  });

        QGraphicsTextItem* ultimosPuntajes = new QGraphicsTextItem("Últimos Puntajes:");
        ultimosPuntajes->setDefaultTextColor(Qt::black);
        ultimosPuntajes->setFont(QFont(FuenteSimpson, 20, QFont::Bold));
        ultimosPuntajes->setPos(950, 270);
        escenaMenu->addItem(ultimosPuntajes);

        for (int i = 0; i < std::min(3, scores.size()); ++i) {
            QGraphicsTextItem* scoreText = new QGraphicsTextItem(
                QString("%1 - %2").arg(scores[i].first)
                    .arg(scores[i].second)
                );
            scoreText->setDefaultTextColor(Qt::black);
            scoreText->setFont(QFont(FuenteSimpson, 16));
            scoreText->setPos(950, 320 + i * 30);
            escenaMenu->addItem(scoreText);
        }

        std::sort(scores.begin(), scores.end(),
                  [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                      return a.second > b.second;
                  });

        QGraphicsTextItem* mejoresPuntajes = new QGraphicsTextItem("Mejores Puntajes:");
        mejoresPuntajes->setDefaultTextColor(Qt::black);
        mejoresPuntajes->setFont(QFont(FuenteSimpson, 20, QFont::Bold));
        mejoresPuntajes->setPos(950, 450);
        escenaMenu->addItem(mejoresPuntajes);

        for (int i = 0; i < std::min(3, scores.size()); ++i) {
            QGraphicsTextItem* scoreText = new QGraphicsTextItem(
                QString("%1 - %2").arg(scores[i].first)
                    .arg(scores[i].second)
                );
            scoreText->setDefaultTextColor(Qt::black);
            scoreText->setFont(QFont(FuenteSimpson, 16));
            scoreText->setPos(950, 500 + i * 30);
            escenaMenu->addItem(scoreText);
        }
    }

    botonlvl1 = new QPushButton();
    botonlvl2 = new QPushButton();

    estiloBotonlvl1 = "QPushButton {"
                      "background-image: url(:/Imagenes/1.jpg);"
                      "background-repeat: no-repeat;"
                      "background-position: center;"
                      "border: 5px solid #000000;"
                      "width: 261px;"
                      "height: 293px;"
                      "}";
    if(bloqLvl2){
        estiloBotonlvl2 = "QPushButton {"
                          "background-image: url(:/Imagenes/2bloq.jpg);"
                          "background-repeat: no-repeat;"
                          "background-position: center;"
                          "border: 5px solid #000000;"
                          "width: 261px;"
                          "height: 293px;"
                          "}";
    } else {
        estiloBotonlvl2 = "QPushButton {"
                          "background-image: url(:/Imagenes/2.jpg);"
                          "background-repeat: no-repeat;"
                          "background-position: center;"
                          "border: 5px solid #000000;"
                          "width: 261px;"
                          "height: 293px;"
                          "}";
    }
    botonlvl1->setStyleSheet(estiloBotonlvl1);
    botonlvl2->setStyleSheet(estiloBotonlvl2);

    QGraphicsProxyWidget *proxy1 = escenaMenu->addWidget(botonlvl1);
    QGraphicsProxyWidget *proxy2 = escenaMenu->addWidget(botonlvl2);
    proxy1->setPos(90, 510);
    proxy2->setPos(432, 510);

    connect(botonlvl1, &QPushButton::clicked, this, &MainWindow::Nivel1);
    if(!bloqLvl2){
        connect(botonlvl2, &QPushButton::clicked, this, &MainWindow::Nivel2);
    }

    vista->setScene(escenaMenu);
    vista->resize(MenuF.width() + 4, MenuF.height() + 4);
}


void MainWindow::detenerTimers() {
    QList<QTimer*> timers = findChildren<QTimer*>();
    for (QTimer* timer : timers) {
        if (timer->isActive()) {
            timer->stop();
            timer->deleteLater();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    if (vista->scene() != nullptr) {
        delete vista->scene();
    }
}
