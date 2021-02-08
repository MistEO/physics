#include "interstellar.h"

#include <cmath>
#include "object.hpp"

#ifdef ENABLE_CACHE
#include <map>
#endif

using namespace meophys;

void Interstellar::on_tick(double ticked_time)
{
    std::shared_lock<std::shared_mutex> rdlock(_objs_mutex);
    auto temp_objects = _objects;
    const auto ano_objects = _objects;
    rdlock.unlock();

    // TODO: 缓存性能优化
#ifdef ENABLE_CACHE
    using objptr_pair = std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>>;
    // std::unordered_map cannot be constructed;
    std::map<objptr_pair, Force> cache;
#endif

    for (auto &&[obj, status] : temp_objects)
    {
        Force gravitation(0, 0);
        auto &&[c_x, c_y] = status.coordinate();
        for (auto &&[ano_obj, ano_status] : ano_objects)
        {
            if (obj == ano_obj)
            {
                continue;
            }
#ifdef ENABLE_CACHE
            auto cache_iter = cache.find(std::make_pair(ano_obj, obj));
            if (cache_iter != cache.cend())
            {
                gravitation += cache_iter->second;
                continue;
            }
#endif
            auto &&[ano_c_x, ano_c_y] = ano_status.coordinate();

            // The universal gravitation, F = GMm/r²
            double r2 = std::pow(c_x - ano_c_x, 2) + std::pow(c_y - ano_c_y, 2);
            double fg_value = GravitationConstant * obj->mass() * ano_obj->mass() / r2;

            double r = std::sqrt(r2);
            double fg_x = (ano_c_x - c_x) * fg_value / r;
            double fg_y = (ano_c_y - c_y) * fg_value / r;
            Force fg(fg_x, fg_y);
#ifdef ENABLE_CACHE
            // cache[std::make_pair(ano_obj, obj)] = fg;
            cache[std::make_pair(obj, ano_obj)] = -fg;
#endif

            gravitation += fg;
        }

        // a = F / m
        Acceleration acc = (status.sum_of_forces() + gravitation) / obj->mass();

        // x = Vt + ½at²
        Displacement d = status.velocity() * ticked_time + 0.5 * acc * std::pow(ticked_time, 2);
        status.coordinate() += d;

        // Vt = V0 + at
        status.velocity() += acc * ticked_time;
    }
    std::unique_lock<std::shared_mutex> wrlock(_objs_mutex);
    _objects = std::move(temp_objects);
}