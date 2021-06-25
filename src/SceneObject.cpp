#include "SceneObject.hpp"



#include <fstream>

#include "Scene.hpp"
SceneObject::SceneObject(const std::string name, const Vector3 &centerPosition,
                         const Vector3 &scale, Transform *const pin) :
    Object(name, centerPosition, scale, pin) {
    this->shadowRadius = 0;
    for (auto &point1 : this->actualPoints)
    {
        for (auto &point2 : this->actualPoints)
        {
            if ((point1 - point2).Length() > this->shadowRadius)
                this->shadowRadius = (point1 - point2).Length();
        }
    }
    this->shadowRadius = this->shadowRadius / 2;
}

 SceneObject ::~SceneObject(){
    //  std::cout << "Dupa" << std::endl;
    //  Scene::api.UsunNazwePliku((std::string(TMP_FOLDER) + this->Name()).c_str());
    // std::cout << "Destruktor ObiektuSceny" << std::endl;
    Scene::RemoveFromDrawable(this);
 }
void SceneObject::Draw() {
    Scene::Draw(this);
}

bool SceneObject ::IsOverlapping(const SceneObject &obj){
    return obj.CanLand();
}