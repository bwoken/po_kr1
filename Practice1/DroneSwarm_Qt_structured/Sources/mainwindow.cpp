#include "mainwindow.h"
#include "chargehistogram.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      idInput_(new QSpinBox(this)),
      batteryInput_(new QDoubleSpinBox(this)),
      averageLabel_(new QLabel(this)),
      readyLabel_(new QLabel(this)),
      criticalLabel_(new QLabel(this)),
      totalLabel_(new QLabel(this)),
      table_(new QTableWidget(this)),
      histogram_(new ChargeHistogram(this))
{
    setWindowTitle("Управление роем БПЛА");
    resize(950, 720);

    auto* central = new QWidget(this);
    auto* root = new QVBoxLayout(central);
    auto* heading = new QLabel("МОНИТОРИНГ РОЯ БЕСПИЛОТНИКОВ");
    heading->setObjectName("heading");
    root->addWidget(heading);

    auto* addGroup = new QGroupBox("Добавить дрон вручную");
    auto* addLayout = new QHBoxLayout(addGroup);
    idInput_->setRange(1, 1000000);
    idInput_->setValue(swarm_.next_id());
    idInput_->setPrefix("ID: ");
    batteryInput_->setRange(0.0, 100.0);
    batteryInput_->setDecimals(1);
    batteryInput_->setSuffix(" %");
    batteryInput_->setValue(75.0);
    batteryInput_->setPrefix("Заряд: ");
    auto* addButton = new QPushButton("Добавить в рой");
    addButton->setObjectName("addButton");
    addLayout->addWidget(idInput_);
    addLayout->addWidget(batteryInput_);
    addLayout->addWidget(addButton);
    root->addWidget(addGroup);

    auto* statsLayout = new QHBoxLayout;
    for (QLabel* label : {averageLabel_, readyLabel_, criticalLabel_, totalLabel_}) {
        label->setMinimumHeight(68);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);
        label->setObjectName("statCard");
        statsLayout->addWidget(label);
    }
    root->addLayout(statsLayout);

    auto* tableTitle = new QLabel("Состав роя");
    tableTitle->setObjectName("sectionTitle");
    root->addWidget(tableTitle);
    table_->setColumnCount(3);
    table_->setHorizontalHeaderLabels({"ID дрона", "Заряд батареи", "Состояние"});
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setAlternatingRowColors(true);
    table_->setMinimumHeight(180);
    root->addWidget(table_, 1);

    auto* chartTitle = new QLabel("Гистограмма заряда аккумуляторов");
    chartTitle->setObjectName("sectionTitle");
    root->addWidget(chartTitle);
    root->addWidget(histogram_);

    auto* bottomLayout = new QHBoxLayout;
    auto* demoButton = new QPushButton("Загрузить пример из задания");
    auto* removeButton = new QPushButton("Очистить рой");
    bottomLayout->addWidget(demoButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(removeButton);
    root->addLayout(bottomLayout);

    setCentralWidget(central);
    setStyleSheet(R"(
        QMainWindow, QWidget { background: #f4f7fb; color: #1d2939; font-family: "Segoe UI"; font-size: 10pt; }
        #heading { font-size: 19pt; font-weight: 700; color: #16324f; padding: 10px 2px; }
        #sectionTitle { font-size: 12pt; font-weight: 700; padding-top: 6px; color: #16324f; }
        QGroupBox { border: 1px solid #d6deea; border-radius: 8px; margin-top: 10px; padding: 12px; font-weight: 600; background: white; }
        QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 5px; }
        #statCard { background: white; border: 1px solid #d6deea; border-radius: 8px; font-weight: 600; padding: 8px; }
        QTableWidget { background: white; border: 1px solid #d6deea; border-radius: 6px; gridline-color: #e5eaf1; }
        QHeaderView::section { background: #e9eff7; padding: 7px; border: none; font-weight: 700; }
        QPushButton { background: #e3eaf4; border: none; border-radius: 6px; padding: 9px 14px; font-weight: 600; }
        QPushButton:hover { background: #d2deed; }
        #addButton { background: #1769aa; color: white; }
        #addButton:hover { background: #12588f; }
    )");

    connect(addButton, &QPushButton::clicked, this, [this]() { addDroneFromInputs(); });
    connect(demoButton, &QPushButton::clicked, this, [this]() {
        swarm_ = DroneSwarm();
        swarm_.add_drone(Drone(1, 95.0));
        swarm_.add_drone(Drone(2, 78.0));
        swarm_.add_drone(Drone(3, 54.0));
        swarm_.add_drone(Drone(4, 18.0));
        swarm_.add_drone(Drone(5, 7.0));
        refreshView();
        idInput_->setValue(swarm_.next_id());
    });
    connect(removeButton, &QPushButton::clicked, this, [this]() {
        swarm_ = DroneSwarm();
        refreshView();
        idInput_->setValue(swarm_.next_id());
    });

    refreshView();
}

void MainWindow::addDroneFromInputs()
{
    const int id = idInput_->value();
    if (swarm_.contains_id(id)) {
        QMessageBox::warning(this, "Повтор ID", "Дрон с таким ID уже находится в рое.");
        return;
    }

    swarm_.add_drone(Drone(id, batteryInput_->value()));
    refreshView();
    idInput_->setValue(swarm_.next_id());
}

void MainWindow::refreshView()
{
    const auto& drones = swarm_.get_drones();
    table_->setRowCount(static_cast<int>(drones.size()));

    for (int row = 0; row < static_cast<int>(drones.size()); ++row) {
        const Drone& drone = drones[static_cast<std::size_t>(row)];
        table_->setItem(row, 0, new QTableWidgetItem(QString::number(drone.get_id())));
        table_->setItem(row, 1, new QTableWidgetItem(QString::number(drone.get_battery(), 'f', 1) + "%"));

        QString state;
        QColor color;
        if (drone.get_battery() < 10.0) {
            state = "Критический заряд";
            color = QColor("#dc3545");
        } else if (drone.get_battery() < 20.0) {
            state = "Низкий заряд";
            color = QColor("#b7791f");
        } else {
            state = "Готов к полёту";
            color = QColor("#16835f");
        }
        auto* stateItem = new QTableWidgetItem(state);
        stateItem->setForeground(color);
        table_->setItem(row, 2, stateItem);
    }

    averageLabel_->setText(QString("СРЕДНИЙ ЗАРЯД\n%1%").arg(swarm_.average_battery(), 0, 'f', 1));
    readyLabel_->setText(QString("ГОТОВЫ К ПОЛЁТУ\n%1").arg(swarm_.count_ready()));
    criticalLabel_->setText(QString("КРИТИЧЕСКИЙ ЗАРЯД\n%1").arg(swarm_.count_critical()));
    totalLabel_->setText(QString("ВСЕГО ДРОНОВ\n%1").arg(swarm_.size()));
    histogram_->setDrones(drones);
}
