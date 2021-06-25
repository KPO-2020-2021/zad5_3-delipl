/**
 * @file Terrain.hpp
 * @author Delicat Jakub (delicat.kuba@gmail.com)
 * @brief File Describes Terrain Class
 * @version 0.1
 * @date 2021-06-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef Terrain_HPP
#define Terrain_HPP
#include "Scene.hpp"
#include "SceneObject.hpp"

/**
 * @brief Makes flat surface on applyed position.
 */
class Terrain : public SceneObject {
  public:
  /**
   * @brief Construct a new Terrain object
   */
    Terrain() :
        SceneObject("surface.dat", {-300, -300, 0}, {4, 4, 4}) {
        Scene::api.UsunNazwePliku(this->Name());
        // PzG::InfoPlikuDoRysowania *wInfoPliku1 = &Scene::api.DodajNazwePliku((std::string(TMP_FOLDER) + this->Name()).c_str());
        // wInfoPliku1->ZmienKolor(3);
    }

    /**
     * @brief nothing
     */
    void Update() override{};

    /**
     * @brief Can land on surface.
     */
    bool CanLand() const override { return true; };
};
#endif // !Terrain_HPP