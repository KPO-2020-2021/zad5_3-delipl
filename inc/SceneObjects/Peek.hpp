/**
 * @file Peek.hpp
 * @author Delicat Jakub (delicat.kuba@gmail.com)
 * @brief File describes obstacle like Peek class
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef PEEK_HPP
#define PEEK_HPP
#include "Cuboid.hpp"
/**
 * @brief Peek obstacle
 * 
 */
class Peek : public Cuboid
{
public:
    /**
     * @brief Construct a new Peek object
     * 
     * @param position of Peek
     * @param width of Peek
     * @param lenght of Peek
     */
    Peek(const Vector3 &position, const double &width, const double &lenght);

    virtual ~Peek() {}

    bool CanLand() const override{return false;}
};

#endif