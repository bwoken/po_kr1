#include "drone.h"
#include <algorithm>

Drone::Drone(int drone_id, double initial_battery)
    : id_(drone_id), battery_(100.0), x_(0.0), y_(0.0), z_(0.0), flying_(false)
{
    set_battery(initial_battery);
}

int Drone::get_id() const { return id_; }
double Drone::get_battery() const { return battery_; }
bool Drone::is_flying() const { return flying_; }

void Drone::set_battery(double value)
{
    battery_ = std::clamp(value, 0.0, 100.0);
}

void Drone::takeoff()
{
    if (!flying_ && battery_ >= 10.0) {
        flying_ = true;
        z_ = 1.0;
        set_battery(battery_ - 1.0);
    }
}

void Drone::land()
{
    flying_ = false;
    z_ = 0.0;
}

void Drone::move_to(double x, double y, double z)
{
    if (flying_ && battery_ >= 5.0 && z >= 0.0) {
        x_ = x;
        y_ = y;
        z_ = z;
        set_battery(battery_ - 1.0);
    }
}

void Drone::emergency_land()
{
    flying_ = false;
    z_ = 0.0;
}
