#pragma once

#include "define.h"
#include "space.h"

namespace meophys
{
    class Interstellar : public Space
    {
        static constexpr double Gravitation = 6.67 * 1e-11;

    public:
        Interstellar() = default;
        virtual ~Interstellar() = default;

    protected:
        virtual void on_tick(double ticked_time) override;
    };
} // namespace meophys