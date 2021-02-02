#pragma once

#include "define.h"
#include "space.h"

namespace meophys
{
    struct Boundary
    {
        int top = 1;
        int left = 0;
        int bottom = 0;
        int right = 1;
    };

    class World : public Space
    {
    public:
        World();
        virtual ~World() = default;

        virtual void push_object(std::shared_ptr<Object> object, Coordinate coor) override
        {
            return Space::push_object(object, coor);
        }
        virtual Time &time() override { return Space::time(); }

        void set_boundary(int top, int left, int bottom, int right)
        {
            _boundary.top = top;
            _boundary.left = left;
            _boundary.bottom = bottom;
            _boundary.right = right;
        }
        Boundary &boundary() { return _boundary; }

    protected:
        virtual void on_tick() override;

    private:
        Boundary _boundary;
    };
} // namespace meophys