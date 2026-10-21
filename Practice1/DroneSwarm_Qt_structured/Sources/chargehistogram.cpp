#include "chargehistogram.h"
#include "drone.h"
#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <algorithm>

ChargeHistogram::ChargeHistogram(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(230);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void ChargeHistogram::setDrones(const std::vector<Drone>& drones)
{
    drones_ = drones;
    update();
}

void ChargeHistogram::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#ffffff"));

    const int left = 42;
    const int right = 16;
    const int top = 22;
    const int bottom = 42;
    const int chartW = width() - left - right;
    const int chartH = height() - top - bottom;

    painter.setPen(QColor("#d8dee9"));
    painter.drawLine(left, top, left, top + chartH);
    painter.drawLine(left, top + chartH, left + chartW, top + chartH);

    for (int tick = 0; tick <= 100; tick += 20) {
        const int y = top + chartH - (chartH * tick / 100);
        painter.setPen(QColor("#e8edf3"));
        painter.drawLine(left, y, left + chartW, y);
        painter.setPen(QColor("#536273"));
        painter.drawText(5, y - 7, left - 10, 16, Qt::AlignRight, QString::number(tick) + "%");
    }

    if (drones_.empty()) {
        painter.setPen(QColor("#667085"));
        painter.drawText(rect(), Qt::AlignCenter, "Добавьте дроны, чтобы построить гистограмму");
        return;
    }

    const double slot = static_cast<double>(chartW) / drones_.size();
    const int barW = std::max(8, static_cast<int>(slot * 0.56));

    for (std::size_t i = 0; i < drones_.size(); ++i) {
        const double charge = std::clamp(drones_[i].get_battery(), 0.0, 100.0);
        const int barH = static_cast<int>(chartH * charge / 100.0);
        const int x = left + static_cast<int>(slot * i + (slot - barW) / 2);
        const int y = top + chartH - barH;

        QColor barColor = charge < 10.0 ? QColor("#dc3545")
                          : charge < 20.0 ? QColor("#f59e0b")
                          : charge < 50.0 ? QColor("#4f8df7")
                                          : QColor("#20a779");
        painter.setPen(Qt::NoPen);
        painter.setBrush(barColor);
        painter.drawRoundedRect(QRect(x, y, barW, barH), 4, 4);

        painter.setPen(QColor("#253247"));
        painter.drawText(QRect(x - 12, y - 21, barW + 24, 18),
                         Qt::AlignCenter, QString::number(charge, 'f', 0) + "%");
        painter.drawText(QRect(x - 10, top + chartH + 7, barW + 20, 24),
                         Qt::AlignCenter, "№" + QString::number(drones_[i].get_id()));
    }
}
