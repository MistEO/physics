#pragma once

#include <utility>
#include <vector>

class WholeTimer;
typedef std::pair<double, double> Force;

class Object {
    friend class WholeTimer;

public:
    Object(double mass, double x_coordinate, double y_coordinate, double x_velocity = 0, double y_velocity = 0);

    std::pair<double, double> coordinate() const;
    std::pair<double, double> velocity() const;
    std::pair<double, double> acceleration() const;

    void tick(int time = 1);

private:
    const double mass;
    double x_coordinate;
    double y_coordinate;
    double x_velocity = 0;
    double y_velocity = 0;
    double x_acceleration = 0;
    double y_acceleration = 0;
    std::vector<Force> forces;
};