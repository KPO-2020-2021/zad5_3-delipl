#include "Scene.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
Scene::Scene(){
    api.ZmienTrybRys(PzG::TR_3D);
    api.Inicjalizuj();
}

Scene::~Scene(){
    // system("pwd");
    // for (auto &obj : this->activeObjects)
    //     obj->~SceneObject();
    system("killall gnuplot  /usr/lib/gnuplot/gnuplot_qt");
    // system("rm ./tmp/*");
}

void Scene::Add(const std::shared_ptr<SceneObject> &obj) {
    this->activeObjects.push_back(obj);
    api.DodajNazwePliku(std::string(TMP_FOLDER + obj->Name()).c_str());
}

void Scene::Remove(const std::size_t &ID){
    // std::cout << ID << "  " << this->activeObjects.size() << " Ma nazwe" <<  activeObjects[ID]->Name() << std::endl;

    if (ID >= this->activeObjects.size())
        throw std::overflow_error("There is no more objects");

    // this->api.UsunNazwePliku(std::string(TMP_FOLDER + this->activeObjects[ID]->Name()).c_str());
    // if(ID == 0)
    //     this->activeObjects.erase(this->activeObjects.begin());
    // else
    // this->activeObjects[ID]->~SceneObject();
    this->activeObjects.erase(this->activeObjects.begin() + ID);
}

std::shared_ptr<SceneObject> &Scene::operator[](const std::size_t &i) {
    if(i >= this->activeObjects.size())
        throw std::overflow_error("There is no more objects");
    return this->activeObjects[i];
}

std::shared_ptr<SceneObject> Scene::operator[](const std::size_t &i) const {
    if(i >= this->activeObjects.size())
        throw std::overflow_error("There is no more objects");
    return this->activeObjects[i];
}

void Scene::Update(){

    for (std::size_t i = 0; i < this->activeObjects.size(); ++i)
    {
        std::shared_ptr<SceneObject> obj;
        try
        {
         obj = this->SelectDrone(i);
        }
        catch(...){
            continue;
        }
        auto drone = std::dynamic_pointer_cast<Drone>(obj);
        if (drone != nullptr)
        {
            if(drone->position[2] > 0){
                if(this->CollisionDetector(drone) && drone->position[2] == drone->animation.goalPosition[2]){
                    std::cout << "!!![EMERYGENCY LANDING]!!!" << std::endl;
                    for (std::size_t i = 0; i < drone->moves.size(); ++i){
                        drone->moves.pop();
                    }
                }
            }
        }
    }

    for (auto &obj : this->activeObjects)
        obj->Update();
    api.Rysuj();
}

std::size_t Scene::CountObjects() const{
    return this->activeObjects.size();
}

void Scene::Draw(SceneObject const *obj) {
    api.DodajNazwePliku((std::string(TMP_FOLDER + obj->Name()).c_str()));
    api.ZmienTrybRys(PzG::TR_3D);
    api.Rysuj();
    api.UsunNazwePliku(std::string(TMP_FOLDER + obj->Name()).c_str());
}

void Scene::AddToDrawable(SceneObject const *obj) {
    api.DodajNazwePliku((std::string(TMP_FOLDER + obj->Name()).c_str()));
    api.ZmienTrybRys(PzG::TR_3D);
}

void Scene::RemoveFromDrawable(SceneObject const *obj)
{
    // std::cout << "usuwam... ->   '" << (std::string(TMP_FOLDER + obj->Name()).c_str()) <<"'"<< std::endl;
    api.UsunNazwePliku((std::string(TMP_FOLDER + obj->Name()).c_str()));
    // usleep(2000000);
    // api.Rysuj();
}

void Scene::ClearGNUPlot() {
    api.UsunWszystkieNazwyPlikow();
}

std::shared_ptr<SceneObject> &Scene::SelectDrone(const std::size_t &index)
{
    std::size_t n = 0;
    for (auto &obj : this->activeObjects)
    {
        if (std::dynamic_pointer_cast<Drone>(obj) != nullptr)
        {
            ++n;
            if (n == index)
            {
                return obj;
            }
        }
    }
    throw std::logic_error("There is no drones");
    return activeObjects[0];
}

bool Scene::CollisionDetector(const std::shared_ptr<SceneObject> &obj) const{
    for(auto &sceneObject: this->activeObjects){
        if (sceneObject == this->activeObjects[0])
            continue;
        if (obj->IsOverlapping(sceneObject.get()))
        {

            return true;

        }
    }
    return false;
}