#pragma once

#include "space.h"

class World : public Space {
public:
    ~World();

    void push_object(Object* object) override;

    void set_boundary(Coordinate bottomleft, Coordinate topright);
    void set_boundary(std::pair<Coordinate, Coordinate> boundary);
    std::pair<Coordinate, Coordinate> get_boundary() const;
    void set_gravity(double gravity);

private:
    void tick_object(Object* object, double time) override;

    double gravity = 0;
    std::vector<Force*> gravity_vector;
    std::pair<Coordinate, Coordinate> boundary;
};