#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "droneswarm.h"

class QSpinBox;
class QDoubleSpinBox;
class QLabel;
class QTableWidget;
class ChargeHistogram;

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    DroneSwarm swarm_;
    QSpinBox* idInput_;
    QDoubleSpinBox* batteryInput_;
    QLabel* averageLabel_;
    QLabel* readyLabel_;
    QLabel* criticalLabel_;
    QLabel* totalLabel_;
    QTableWidget* table_;
    ChargeHistogram* histogram_;

    void addDroneFromInputs();
    void refreshView();

public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif
