#ifndef DRONE_H
#define DRONE_H

class Drone {
private:
    int id_;
    double battery_;
    double x_;
    double y_;
    double z_;
    bool flying_;

public:
    Drone(int drone_id = 0, double initial_battery = 100.0);

    int get_id() const;
    double get_battery() const;
    bool is_flying() const;
    void set_battery(double value);
    void takeoff();
    void land();
    void move_to(double x, double y, double z);
    void emergency_land();
};

#endif
