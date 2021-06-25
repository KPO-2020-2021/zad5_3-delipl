/**
 * @file Ridge.hpp
 * @author Delicat Jakub (delicat.kuba@gmail.com)
 * @brief File describes obstacle like Ridge class
 * @version 0.1
 * @date 2021-06-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef RIDGE_HPP
#define RIDGE_HPP
#include "Cuboid.hpp"
/**
 * @brief Ridge obstacle
 * 
 */
class Ridge : public Cuboid
{
public:
    /**
     * @brief Construct a new Ridge object
     * 
     * @param position of Ridge
     * @param width of Ridge
     * @param length of Ridge
     */
        Ridge(const Vector3 &position, const double &width, const double &length);

        bool CanLand() const override {return false;}

        virtual  ~Ridge() {}
};

#endif