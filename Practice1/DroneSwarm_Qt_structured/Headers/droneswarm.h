#ifndef DRONESWARM_H
#define DRONESWARM_H

#include "drone.h"
#include <vector>

class DroneSwarm {
private:
    std::vector<Drone> drones_;

public:
    void add_drone(const Drone& drone);
    bool contains_id(int id) const;
    int next_id() const;
    int size() const;
    const std::vector<Drone>& get_drones() const;
    double average_battery() const;
    int count_ready() const;
    int count_critical() const;
};

#endif
