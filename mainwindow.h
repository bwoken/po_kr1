#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QTimer>

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
    void tick();

    void on_btnLostLink_clicked();
    void on_btnLowBattery_clicked();
    void on_btnRestore_clicked();
    void on_btnReset_clicked();

private:
    Ui::MainWindow *ui;

    // Состояния конечного автомата
    enum class State
    {
        FLIGHT,
        LOST_LINK,
        LOW_BATTERY,
        LANDING
    };

    State currentState;

    // Графическая сцена
    QGraphicsScene *scene;

    // Объекты на сцене
    QGraphicsEllipseItem *uav;
    QGraphicsRectItem *base;

    // Маленькие иконки
    QGraphicsTextItem *uavIcon;
    QGraphicsTextItem *baseIcon;

    // Линия возврата
    QGraphicsLineItem *rtlLine;

    // Таймер
    QTimer *timer;

    // Данные системы
    int battery;
    int batteryCounter;
    bool linkActive;

    // Координаты БПЛА
    double uavX;
    double uavY;

    // Координаты базы
    double baseX;
    double baseY;

    // Направление обычного полёта
    double flightDx;
    double flightDy;

    // Функции
    void moveTowards(double targetX, double targetY);

    double distance(double x1, double y1,
                    double x2, double y2);

    void log(const QString &message);

    void updateState();

    void resetSimulation();
};

#endif // MAINWINDOW_H