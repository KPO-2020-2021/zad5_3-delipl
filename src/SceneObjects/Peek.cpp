#include "Peek.hpp"

Peek::Peek(const Vector3 &position, const double &width, const double &length) : Cuboid(Vector3({width, length, width})/10, position)
{
    this->Rotate(90, VectorX);
    for (auto &point : this->vertexes)
        (*point)[1] = 0;

    for (auto &point : this->centers)
        (*point)[1] = length;
    this->UpdatePoints();
}