#include "droneswarm.h"
#include <algorithm>

void DroneSwarm::add_drone(const Drone& drone)
{
    drones_.push_back(drone);
}

bool DroneSwarm::contains_id(int id) const
{
    return std::any_of(drones_.begin(), drones_.end(),
                       [id](const Drone& drone) { return drone.get_id() == id; });
}

int DroneSwarm::next_id() const
{
    int candidate = 1;
    while (contains_id(candidate))
        ++candidate;
    return candidate;
}

int DroneSwarm::size() const { return static_cast<int>(drones_.size()); }

const std::vector<Drone>& DroneSwarm::get_drones() const { return drones_; }

double DroneSwarm::average_battery() const
{
    if (drones_.empty())
        return 0.0;

    double total = 0.0;
    for (const Drone& drone : drones_)
        total += drone.get_battery();

    return total / static_cast<double>(drones_.size());
}

int DroneSwarm::count_ready() const
{
    int count = 0;
    for (const Drone& drone : drones_) {
        if (drone.get_battery() >= 20.0)
            ++count;
    }
    return count;
}

int DroneSwarm::count_critical() const
{
    int count = 0;
    for (const Drone& drone : drones_) {
        if (drone.get_battery() < 10.0)
            ++count;
    }
    return count;
}
