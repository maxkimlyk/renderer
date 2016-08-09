#include "geometry.h"
#include <iostream>

template <typename type>
std::ostream& operator<< (std::ostream &stream, vec2<type> vec)
{
    return stream << "(" << vec.x << ", " << vec.y << ")";
}

template <typename type>
std::ostream& operator<< (std::ostream &stream, vec3<type> vec)
{
    return stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

int main()
{
    vec2f v1 (10, 32);
    vec2f v2 (1, 2);

    std::cout << "+: " << v1+v2 << std::endl;
    std::cout << "-: " << v1-v2 << std::endl;
    std::cout << "*: " << v1*v2 << std::endl;
    std::cout << "*3: " << v1*3 << std::endl;
    std::cout << "3*: " << 3*v1 << std::endl;
    std::cout << "Norm: " << Normalize(v1) << std::endl;
    std::cout << "Norm len: " << Length( Normalize(v1) ) << std::endl;

    vec3f v3 (1, 0, 0);
    vec3f v4 (0, 1, 0);

    std::cout << "+: " << v3+v4 << std::endl;
    std::cout << "-: " << v3-v4 << std::endl;
    std::cout << "*: " << v3*v4 << std::endl;
    std::cout << "*3: " << v3*3 << std::endl;
    std::cout << "3*: " << 3*v3 << std::endl;
    std::cout << "Norm: " << Normalize(v3) << std::endl;
    std::cout << "Norm len: " << Length( Normalize(v3) ) << std::endl;
    std::cout << "Cross: " << Cross(v3, v4) << std::endl;

    return 0;
}
