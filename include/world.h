#pragma once

#include "define.h"
#include "space.h"

namespace meophys
{
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

    protected:
        virtual void callback_when_ticktime() override;

    private:
    };
} // namespace meophys