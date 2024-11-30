#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void Nivel1();
    void Nivel2();
    void Menu();
    void detenerTimers();

private:
    Ui::MainWindow *ui;
    QGraphicsView *vista;
    QGraphicsScene *escenaMenu;
    QGraphicsScene *lvl1;
    QGraphicsScene *lvl2;
    QPushButton *botonlvl1;
    QPushButton *botonlvl2;
    QString estiloBotonlvl1;
    QString estiloBotonlvl2;
    QString FuenteSimpson;
    int idFont;
    int puntosTotales;
    int puntosIniciales;
    bool bloqLvl2=true;
    QVector<QString> estadoImagenes;
    QVector<QGraphicsPixmapItem*> vidasCestas;
};
#endif // MAINWINDOW_H
