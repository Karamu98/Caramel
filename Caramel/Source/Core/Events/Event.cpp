#include "clpch.h"
#include "Event.h"

namespace Caramel
{
    constexpr int Caramel::operator|(const EventCategory& a, const EventCategory& b)
    {
        return (int)a | (int)b;
    }
}

