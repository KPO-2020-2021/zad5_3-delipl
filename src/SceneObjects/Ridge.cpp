#include "Ridge.hpp"

Ridge::Ridge(const Vector3 &position, const double &width, const double &length) : Cuboid(Vector3({width, width, length}) / 10, position)
{

    *(this->vertexes[6]) = *(this->vertexes[7]);
    *(this->vertexes[0]) = *(this->vertexes[1]);
    *(this->vertexes[8]) = *(this->vertexes[1]);
    
    for(auto &center: this->centers){
        *center -= VectorZ * 10;
   }
        this->UpdatePoints();
}