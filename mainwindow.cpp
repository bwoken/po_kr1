#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QFont>
#include <QtMath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // =========================================
    // СОЗДАЁМ СЦЕНУ
    // =========================================

    scene = new QGraphicsScene(0, 0, 600, 400, this);

    ui->graphicsView->setScene(scene);


    // =========================================
    // СОЗДАЁМ БАЗУ
    // =========================================

    baseX = 65;
    baseY = 365;

    base = scene->addRect(
        baseX - 15,
        baseY - 15,
        30,
        30,
        QPen(Qt::green),
        QBrush(Qt::green)
        );


    // Иконка базы
    baseIcon = scene->addText("⌂");

    QFont baseFont;
    baseFont.setPointSize(18);

    baseIcon->setFont(baseFont);
    baseIcon->setDefaultTextColor(Qt::darkGreen);

    baseIcon->setPos(
        baseX - 9,
        baseY - 40
        );


    // =========================================
    // СОЗДАЁМ БПЛА
    // =========================================

    uavX = 300;
    uavY = 200;

    uav = scene->addEllipse(
        0,
        0,
        20,
        20,
        QPen(Qt::green),
        QBrush(Qt::green)
        );

    uav->setPos(
        uavX - 10,
        uavY - 10
        );


    // Иконка БПЛА
    uavIcon = scene->addText("✈");

    QFont uavFont;
    uavFont.setPointSize(16);

    uavIcon->setFont(uavFont);
    uavIcon->setDefaultTextColor(Qt::darkGreen);

    uavIcon->setPos(
        uavX - 9,
        uavY - 32
        );


    // =========================================
    // НАЧАЛЬНЫЕ ЗНАЧЕНИЯ
    // =========================================

    battery = 80;

    // Счётчик нужен для расхода 1% в секунду
    batteryCounter = 0;

    linkActive = true;

    currentState = State::FLIGHT;


    // Обычное направление полёта
    flightDx = 2;
    flightDy = 1;


    // Линии возврата пока нет
    rtlLine = nullptr;


    // =========================================
    // СОЗДАЁМ ТАЙМЕР
    // =========================================

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout,
            this, &MainWindow::tick);

    timer->start(50);


    // =========================================
    // НАЧАЛЬНЫЙ ИНТЕРФЕЙС
    // =========================================

    ui->labelState->setText(
        "Состояние: ПОЛЁТ"
        );

    ui->labelBattery->setText(
        "Заряд: 80%"
        );


    log("Система запущена");
}


// =========================================
// ДЕСТРУКТОР
// =========================================

MainWindow::~MainWindow()
{
    delete ui;
}


// =========================================
// РАССТОЯНИЕ МЕЖДУ ТОЧКАМИ
// =========================================

double MainWindow::distance(double x1, double y1,
                            double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    return qSqrt(dx * dx + dy * dy);
}


// =========================================
// ЛОГ
// =========================================

void MainWindow::log(const QString &message)
{
    ui->textLog->append(message);
}


// =========================================
// ДВИЖЕНИЕ К ТОЧКЕ
// =========================================

void MainWindow::moveTowards(double targetX, double targetY)
{
    double dx = targetX - uavX;
    double dy = targetY - uavY;

    double length = qSqrt(
        dx * dx + dy * dy
        );


    // Если уже почти достигли точки
    if (length <= 3)
    {
        uavX = targetX;
        uavY = targetY;

        uav->setPos(
            uavX - 10,
            uavY - 10
            );

        uavIcon->setPos(
            uavX - 9,
            uavY - 32
            );


        // Обновляем линию
        if (rtlLine != nullptr)
        {
            rtlLine->setLine(
                uavX,
                uavY,
                baseX,
                baseY
                );
        }

        return;
    }


    // Двигаемся к цели
    uavX += dx / length * 3;
    uavY += dy / length * 3;


    // Перемещаем круг БПЛА
    uav->setPos(
        uavX - 10,
        uavY - 10
        );


    // Перемещаем иконку вместе с БПЛА
    uavIcon->setPos(
        uavX - 9,
        uavY - 32
        );


    // Обновляем линию возврата
    if (rtlLine != nullptr)
    {
        rtlLine->setLine(
            uavX,
            uavY,
            baseX,
            baseY
            );
    }
}


// =========================================
// ОБНОВЛЕНИЕ СОСТОЯНИЯ
// =========================================

void MainWindow::updateState()
{
    if (currentState == State::FLIGHT)
    {
        ui->labelState->setText(
            "Состояние: ПОЛЁТ"
            );

        uav->setBrush(
            QBrush(Qt::green)
            );

        uav->setPen(
            QPen(Qt::green)
            );

        uavIcon->setDefaultTextColor(
            Qt::darkGreen
            );
    }


    else if (currentState == State::LOST_LINK)
    {
        ui->labelState->setText(
            "Состояние: ПОТЕРЯ СВЯЗИ"
            );

        uav->setBrush(
            QBrush(Qt::yellow)
            );

        uav->setPen(
            QPen(Qt::yellow)
            );

        uavIcon->setDefaultTextColor(
            Qt::darkYellow
            );


        if (rtlLine != nullptr)
        {
            rtlLine->setPen(
                QPen(Qt::yellow, 2)
                );
        }
    }


    else if (currentState == State::LOW_BATTERY)
    {
        ui->labelState->setText(
            "Состояние: НИЗКИЙ ЗАРЯД"
            );

        QColor orange(255, 165, 0);

        uav->setBrush(
            QBrush(orange)
            );

        uav->setPen(
            QPen(orange)
            );

        uavIcon->setDefaultTextColor(
            QColor(180, 100, 0)
            );


        if (rtlLine != nullptr)
        {
            rtlLine->setPen(
                QPen(orange, 2)
                );
        }
    }


    else if (currentState == State::LANDING)
    {
        ui->labelState->setText(
            "Состояние: ПОСАДКА"
            );

        uav->setBrush(
            QBrush(Qt::red)
            );

        uav->setPen(
            QPen(Qt::red)
            );

        uavIcon->setDefaultTextColor(
            Qt::darkRed
            );
    }
}


// =========================================
// ОСНОВНОЙ ЦИКЛ
// =========================================

void MainWindow::tick()
{
    // Если посадка завершена -
    // больше ничего не делаем
    if (currentState == State::LANDING)
    {
        return;
    }


    // =========================================
    // РАСХОД БАТАРЕИ
    // =========================================

    batteryCounter++;


    // 20 * 50 мс = 1 секунда
    if (batteryCounter >= 20)
    {
        batteryCounter = 0;


        if (battery > 5)
        {
            battery--;
        }


        ui->labelBattery->setText(
            "Заряд: " +
            QString::number(battery) +
            "%"
            );
    }


    // =========================================
    // ПОЛЁТ
    // =========================================

    if (currentState == State::FLIGHT)
    {
        // Простое плавное движение
        uavX += flightDx;
        uavY += flightDy;


        // Границы движения по X
        if (uavX >= 500)
        {
            flightDx = -2;
        }

        if (uavX <= 100)
        {
            flightDx = 2;
        }


        // Границы движения по Y
        if (uavY >= 300)
        {
            flightDy = -1;
        }

        if (uavY <= 100)
        {
            flightDy = 1;
        }


        // Перемещаем круг
        uav->setPos(
            uavX - 10,
            uavY - 10
            );


        // Перемещаем иконку
        uavIcon->setPos(
            uavX - 9,
            uavY - 32
            );


        // Проверяем заряд
        if (battery <= 20)
        {
            battery = 20;


            currentState =
                State::LOW_BATTERY;


            log(
                "Низкий заряд! "
                "Возврат на базу."
                );


            // Звук аварии
            QApplication::beep();


            // Создаём линию возврата
            if (rtlLine != nullptr)
            {
                scene->removeItem(rtlLine);
            }


            rtlLine = scene->addLine(
                uavX,
                uavY,
                baseX,
                baseY,
                QPen(
                    QColor(255, 165, 0),
                    2
                    )
                );


            updateState();
        }
    }


    // =========================================
    // ПОТЕРЯ СВЯЗИ
    // =========================================

    else if (currentState ==
             State::LOST_LINK)
    {
        moveTowards(
            baseX,
            baseY
            );


        if (distance(
                uavX,
                uavY,
                baseX,
                baseY
                ) < 1)
        {
            currentState =
                State::LANDING;


            log(
                "БПЛА достиг базы."
                );

            log(
                "Посадка завершена."
                );


            // Звук посадки
            QApplication::beep();


            updateState();


            timer->stop();
        }
    }


    // =========================================
    // НИЗКИЙ ЗАРЯД
    // =========================================

    else if (currentState ==
             State::LOW_BATTERY)
    {
        moveTowards(
            baseX,
            baseY
            );


        if (distance(
                uavX,
                uavY,
                baseX,
                baseY
                ) < 1)
        {
            currentState =
                State::LANDING;


            log(
                "БПЛА достиг базы."
                );

            log(
                "Посадка завершена."
                );


            // Звук посадки
            QApplication::beep();


            updateState();


            timer->stop();
        }
    }
}


// =========================================
// ПОТЕРЯ СВЯЗИ
// =========================================

void MainWindow::on_btnLostLink_clicked()
{
    linkActive = false;


    if (currentState == State::FLIGHT)
    {
        currentState =
            State::LOST_LINK;


        log("Потеря связи!");
        log("Начат возврат на базу.");


        // Звук аварии
        QApplication::beep();


        // Создаём линию
        if (rtlLine != nullptr)
        {
            scene->removeItem(rtlLine);
        }


        rtlLine = scene->addLine(
            uavX,
            uavY,
            baseX,
            baseY,
            QPen(Qt::yellow, 2)
            );


        updateState();
    }
}


// =========================================
// РАЗРЯД БАТАРЕИ
// =========================================

void MainWindow::on_btnLowBattery_clicked()
{
    if (currentState == State::LANDING)
    {
        return;
    }


    battery = 20;


    currentState =
        State::LOW_BATTERY;


    ui->labelBattery->setText(
        "Заряд: 20%"
        );


    log("Разряд батареи!");
    log("Начат возврат на базу.");


    // Звук аварии
    QApplication::beep();


    // Создаём линию
    if (rtlLine != nullptr)
    {
        scene->removeItem(rtlLine);
    }


    rtlLine = scene->addLine(
        uavX,
        uavY,
        baseX,
        baseY,
        QPen(
            QColor(255, 165, 0),
            2
            )
        );


    updateState();
}


// =========================================
// ВОССТАНОВЛЕНИЕ СВЯЗИ
// =========================================

void MainWindow::on_btnRestore_clicked()
{
    linkActive = true;


    if (currentState == State::LOST_LINK)
    {
        if (battery > 20)
        {
            currentState =
                State::FLIGHT;


            log(
                "Связь восстановлена."
                );

            log(
                "Возвращаемся к обычному полёту."
                );


            // Удаляем линию
            if (rtlLine != nullptr)
            {
                scene->removeItem(rtlLine);
                rtlLine = nullptr;
            }


            updateState();
        }
        else
        {
            currentState =
                State::LOW_BATTERY;


            log(
                "Связь восстановлена, "
                "но заряд низкий."
                );

            log(
                "Продолжаем возврат на базу."
                );


            updateState();


            if (rtlLine != nullptr)
            {
                rtlLine->setPen(
                    QPen(
                        QColor(255, 165, 0),
                        2
                        )
                    );
            }
        }
    }
}


// =========================================
// СБРОС
// =========================================

void MainWindow::on_btnReset_clicked()
{
    resetSimulation();
}


// =========================================
// ПОЛНЫЙ СБРОС
// =========================================

void MainWindow::resetSimulation()
{
    timer->stop();


    // Начальные значения
    battery = 80;
    batteryCounter = 0;

    linkActive = true;

    currentState =
        State::FLIGHT;


    // Возвращаем БПЛА
    uavX = 300;
    uavY = 200;


    uav->setPos(
        uavX - 10,
        uavY - 10
        );


    uavIcon->setPos(
        uavX - 9,
        uavY - 32
        );


    // Начальное направление
    flightDx = 2;
    flightDy = 1;


    // Удаляем линию
    if (rtlLine != nullptr)
    {
        scene->removeItem(rtlLine);
        rtlLine = nullptr;
    }


    // Интерфейс
    ui->labelBattery->setText(
        "Заряд: 80%"
        );


    updateState();


    // Очищаем лог
    ui->textLog->clear();


    log("Система сброшена.");


    timer->start(50);
}