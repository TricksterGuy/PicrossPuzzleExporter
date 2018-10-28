#include "color.hpp"

#include <functional>

#include "cpercep.hpp"

Color::Color(unsigned short color_data)
{
    Set(color_data);
}

bool Color::operator<(const Color& right) const
{
    bool less;
    less = x < right.x;
    if (x == right.x) less = y < right.y;
    if (x == right.x && y == right.y) less = z < right.z;
    return less;
}

void Color::Set(int a, int b, int c)
{
    x = a;
    y = b;
    z = c;
}

void Color::Set(unsigned short color_data)
{
    x = color_data & 0x1f;
    y = (color_data >> 5) & 0x1f;
    z = (color_data >> 10) & 0x1f;
}

void Color::Get(int& a, int& b, int& c) const
{
    a = x;
    b = y;
    c = z;
}

/** Distance in perception.
  */
double Color::Distance(const Color& other) const
{
    double ox, oy, oz;
    double l, a, b, ol, oa, ob;

    ox = other.x;
    oy = other.y;
    oz = other.z;

    cpercep_rgb_to_space(x * 255.0 / 31, y * 255.0 / 31, z * 255.0 / 31, &l, &a, &b);
    cpercep_rgb_to_space(ox * 255.0 / 31, oy * 255.0 / 31, oz * 255.0 / 31, &ol, &oa, &ob);

    return cpercep_distance_space(l, a, b, ol, oa, ob);
    //return sqrt((x - ox) * (x - ox) + (y - oy) * (y - oy) + (z - oz) *(z - oz));

}
