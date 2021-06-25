/**
 * @file Plateau.hpp
 * @author Delicat Jakub (delicat.kuba@gmail.com)
 * @brief File describes obstacle like Plateau class
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef PLATEAU_HPP
#define PLATEAU_HPP
#include "Cuboid.hpp"
/**
 * @brief Plateau obstacle
 * 
 */
class Plateau : public Cuboid
{
public:
    /**
     * @brief Construct a new Plateau object
     * 
     * @param position of Plateau
     * @param width of Plateau
     * @param lenght of Plateau
     */
    Plateau(const Vector3 &position, const double &width, const double &length) : Cuboid(Vector3({width, length, width}) / 10, position){};

    virtual ~Plateau() {}

    bool CanLand() const override { return true; }

};

#endif