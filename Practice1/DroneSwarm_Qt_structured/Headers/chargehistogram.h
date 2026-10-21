#ifndef CHARGEHISTOGRAM_H
#define CHARGEHISTOGRAM_H

#include <QWidget>
#include <vector>

class Drone;

class ChargeHistogram : public QWidget {
    Q_OBJECT
private:
    std::vector<Drone> drones_;

public:
    explicit ChargeHistogram(QWidget* parent = nullptr);
    void setDrones(const std::vector<Drone>& drones);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif
