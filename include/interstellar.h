#pragma once

#include "define.h"
#include "space.hpp"

namespace meophys
{
    class Interstellar : public Space
    {
        static constexpr double GravitationConstant = 6.67259e-11;

    public:
        Interstellar() = default;
        Interstellar(const Interstellar &) = delete;
        Interstellar(Interstellar &&) = delete;
        virtual ~Interstellar() = default;

        Interstellar &operator=(const Interstellar &) = delete;
        Interstellar &operator=(Interstellar &&) = delete;

    protected:
        virtual void on_tick(double ticked_time, const ObjectAndStatusMap &pre_objs, ObjectAndStatusMap &cur_objs) override;
    };
} // namespace meophys