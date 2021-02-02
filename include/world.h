#pragma once

#include "define.h"
#include "space.h"

namespace meophys
{
    class World : public Space
    {
    public:
        struct Boundary
        {
            int top = 1;
            int left = 0;
            int bottom = 0;
            int right = 1;
        };

    public:
        World() = default;
        virtual ~World() = default;

        void set_boundary(int top, int left, int bottom, int right)
        {
            _boundary.top = top;
            _boundary.left = left;
            _boundary.bottom = bottom;
            _boundary.right = right;
        }
        Boundary &boundary() { return _boundary; }

    protected:
        virtual void on_tick(double ticked_time) override;

    private:
        Boundary _boundary;
    };
} // namespace meophys