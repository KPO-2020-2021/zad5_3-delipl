#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <iostream>
#include <limits>
#include <thread>



#include "config.hpp"
#include "Plateau.hpp"
#include "Terrain.hpp"
#include "Peek.hpp"
#include "Ridge.hpp"
#include "Drone.hpp"

#include "Menu.hpp"
#include "Scene.hpp"
#include "Vectors.hpp"



bool DISPLAY = true;

enum Draw{
    goToDraw
};


int main() {
    {
        /* initialize random seed: */
        srand(time(NULL));
        bool finish = false;
        /* -------------------------------------------------------------------------- */
        /*                              INIT INFORMATIONS                             */
        /* -------------------------------------------------------------------------- */

        std::cout << "Project Rotation 3D based on C++ Boiler Plate by Jakub Delicat v"
                << PROJECT_VERSION_MAJOR /*duże zmiany, najczęściej brak kompatybilności wstecz */
                << "."
                << PROJECT_VERSION_MINOR /* istotne zmiany */
                << "."
                << PROJECT_VERSION_PATCH /* naprawianie bugów */
                << "."
                << PROJECT_VERSION_TWEAK /* zmiany estetyczne itd. */
                << std::endl;
        std::system("cat ./LICENSE");
        std::system("pwd");
        std::cout << "Press enter to start..." << std::endl
                << std::endl
                << std::endl;
        std::cin.ignore(std::numeric_limits<int>().max(), '\n');

        /* -------------------------------------------------------------------------- */
        /*                              CREATING FIGURES                              */
        /* -------------------------------------------------------------------------- */

        Scene scene;
        std::thread displaying([&scene, &finish]() {
            auto start = std::chrono::high_resolution_clock::now();
            while(!finish){
                
                auto measure = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(measure - start);
                try{
                    if(elapsed.count() >= (100)){
                        start = std::chrono::high_resolution_clock::now();
                        throw Draw::goToDraw;
                    }

                }
                catch(Draw){
                    scene.Update();
                    // std::cout << "\n\nCounting Vectors..." << std::endl;

                    // std::cout << "===========================================" << std::endl;
                    // std::cout << std::setw(35) << "Number of Vectors on Scene: " << std::setw(10) << Vector3::HowManyObjects() << std::endl;
                    // std::cout << std::setw(35) << "Number of Vectors from start: " << std::setw(10) << Vector3::AllHowManyObjects() << std::endl;
                    // std::cout << "===========================================" << std::endl
                    //           << std::endl
                    //           << std::endl;
                }
            }
        });
            

        std::shared_ptr<Drone> drone;

        std::shared_ptr<Terrain> floor = std::make_shared<Terrain>();
        scene.Add(std::move (floor));

        /* -------------------------------------------------------------------------- */
        /*                              MENU CONSTRUCTOR                              */
        /* -------------------------------------------------------------------------- */
        
        // std::vector<MatrixRot> rotationSequece;
        Menu menu({{"Print informations about selected object: ",
                    [&drone]() mutable
                    {
                        if (drone == nullptr)
                            throw std::logic_error("Did not choosed the active object.");
                        std::cout << drone->SeflID() << " " << drone->Name() << std::endl;
                        std::cout << "position: \n"
                                << drone->position
                                << "\neuler anglesRPY: \n"
                                << drone->anglesRPY
                                << "\nrotation Matrix: \n"
                                << drone->orientation;
                    }},
                   {"Add Scene Object: ",
                    [&scene]()
                    {
                        std::cout << "Choose type of object:" << std::endl;
                        Vector3 pos =  {double(rand() % 200 + 1), double(rand() % 200 + 1), 0};
                        pos[0] *= rand()%2 ==0 ? -1: 1;
                        pos[1] *= rand()%2 ==0 ? -1: 1;
                        Menu menu1 = {{"Drone", [&scene, &pos]()
                                       {
                                           std::shared_ptr<Drone> tmp = std::make_shared<Drone>(pos);
                                           scene.Add(std::move(tmp));
                                       }},
                                      {"Peek", [&scene, &pos]()
                                       {
                                           std::shared_ptr<Peek> tmp = std::make_shared<Peek>(pos, 40, 30);
                                           scene.Add(std::move(tmp));
                                       }},
                                      {"Ridge", [&scene, &pos]()
                                       {
                                           std::shared_ptr<Ridge> tmp = std::make_shared<Ridge>(pos, 30, 40);
                                           scene.Add(std::move(tmp));
                                       }},
                                      {"Plateau", [&scene, &pos]()
                                       {
                                           std::shared_ptr<Plateau> tmp = std::make_shared<Plateau>(pos, 35, 35);
                                           scene.Add(std::move(tmp));
                                       }}

                        };
                        std::cout << menu1;
                        std::cin >> menu1;
                    }},
                   {"Remove Scene Object", [&scene, &drone]()
                    {
                        if(scene.CountObjects() == 0){

                            std::cout << "There is no SceneObject on Scene" << std::endl;
                            return;
                        }
                        std::vector<std::pair<std::string, std::function<void(void)>>> vec;
                        std::vector<std::function<void(void)>> lambdas;
                        for (std::size_t i = 0; i < scene.CountObjects(); ++i)
                        {
                            lambdas.push_back([&scene, i,&drone]()
                                              { 
                                                  if(drone == std::dynamic_pointer_cast<Drone>(scene[i])){
                                                      drone.~__shared_ptr<Drone>();
                                                  }
                                                  std::cout << "Usuwam" << scene[i]->Name()<< std::endl;
                                                  scene.Remove(i);
                                              });
                            std::stringstream ss;
                            ss << std::setw(15) << scene[i]->Name() << scene[i]->position;
                            vec.push_back(std::pair<std::string, std::function<void(void)>>(ss.str(), lambdas[i]));
                        }
                        Menu menu2(vec);

                        std::cout << menu2;
                        std::cin >> menu2;
                        vec.clear();
                        
                    }},
                   {"Choose active drone: ", [&drone, &scene]()
                    {
                        // std::cout << "There are " << scene.CountObjects() << " on scene. Type number. 1 - n" << std::endl;
                        int d = 1;
                        for (std::size_t i = 0; i < scene.CountObjects(); ++i)
                        {
                            auto localPtr = std::dynamic_pointer_cast<Drone>(scene[i]);
                            if (localPtr != nullptr)
                            {
                                if (localPtr == drone)
                                    std::cout << "* ";
                                else
                                    std::cout << "  ";
                                std::cout << d++ << " Drone is on position:  " << localPtr->position << std::endl;
                            }
                        }
                        std::cout << "Type number of Drone" << std::endl;
                        std::size_t k = 1;
                        std::cin >> k;
                        drone->ChangeColor(1);
                        drone = std::dynamic_pointer_cast<Drone>(scene.SelectDrone(k));
                        drone->ChangeColor(2);
                    }},
                   {"Move drone", [&drone]()
                    {
                        if (drone == nullptr)
                            throw std::logic_error("Did not choosed the active object.");
                        std::cout << "Type height, angle and length of move." << std::endl;
                        Vector3 pos;
                        std::cin >> pos;
                        if(pos[0] != 0)
                            drone->moves.push([pos, drone]()
                                            { drone->GoVerdical(pos[0] ); });
                        if (pos[1] != 0)
                            drone->moves.push([pos, drone]()
                                            { drone->Right(pos[1]); });
                        if (pos[2] != 0)
                            drone->moves.push([pos, drone]()
                                            { drone->Forward(pos[2]); });
                        if (pos[0] != 0)
                            drone->moves.push([pos, drone]()
                                            { drone->GoVerdical(pos[0] * -1); });


                        drone->MakeRoute(pos[0], pos[1], pos[2]);
                    }},
                   {"Recognize flight", [&drone]()
                    {
                        if (drone == nullptr)
                            throw std::logic_error("Did not choosed the active object.");
                        std::cout << "Type height, angle and length of move." << std::endl;
                        drone->moves.push([&drone]()
                                        { drone->GoVerdical(150); });
                        for (int i = 0; i < 30; ++i)
                        {
                            drone->moves.push([drone]()
                                            {
                                                drone->Right(360 / 20);
                                                drone->Forward(10);
                                            });
                        }
                        drone->moves.push([drone]()
                                        { drone->GoVerdical(150 * -1); });

                        // drone->MakeRoute(pos[0], pos[1], pos[2]);
                    }},
                   {"Exit", [&finish, &scene]()
                    {
                        finish = true;
                        throw std::logic_error("Exit");
                    }}});
        {
            Vector3 pos = {double(rand() % 200 + 1), double(rand() % 200 + 1), 0};
            pos = {double(rand() % 200 + 1), double(rand() % 200 + 1), 0};
            pos[0] *= rand() % 2 == 0 ? -1 : 1;
            pos[1] *= rand() % 2 == 0 ? -1 : 1;

            std::shared_ptr<Plateau> tmp = std::make_shared<Plateau>(pos, 35, 35);
            scene.Add(std::move(tmp));

            pos = {double(rand() % 200 + 1), double(rand() % 200 + 1), 0};
            pos[0] *= rand() % 2 == 0 ? -1 : 1;
            pos[1] *= rand() % 2 == 0 ? -1 : 1;
            std::shared_ptr<Drone> tmp1 = std::make_shared<Drone>(pos);
            scene.Add(std::move(tmp1));

            pos = {double(rand() % 200 + 1), double(rand() % 200 + 1), 0};
            pos[0] *= rand() % 2 == 0 ? -1 : 1;
            pos[1] *= rand() % 2 == 0 ? -1 : 1;
            std::shared_ptr<Peek> tmp2 = std::make_shared<Peek>(pos, 40, 30);
            scene.Add(std::move(tmp2));

            pos = {double(rand() % 200 + 1), double(rand() % 200 + 1), 0};
            pos[0] *= rand() % 2 == 0 ? -1 : 1;
            pos[1] *= rand() % 2 == 0 ? -1 : 1;
            std::shared_ptr<Ridge> tmp3 = std::make_shared<Ridge>(pos, 30, 40);
            scene.Add(std::move(tmp3));
        }
        

        drone =  std::dynamic_pointer_cast<Drone>(scene.SelectDrone(1));
        drone->ChangeColor(2);

        /* -------------------------------------------------------------------------- */
        /*                                  MAIN LOOP                                 */
        /* -------------------------------------------------------------------------- */
        std::thread menuig([&menu, &drone, &finish, &scene]() {
            while (!finish) {
                std::cout << "\n\nCounting Vectors..." << std::endl;

                std::cout << "===========================================" << std::endl;
                std::cout << std::setw(35) << "Number of Vectors on Scene: " << std::setw(10) << Vector3::HowManyObjects() << std::endl;
                std::cout << std::setw(35) << "Number of Vectors from start: " << std::setw(10) << Vector3::AllHowManyObjects() << std::endl;
                std::cout << "===========================================" << std::endl;

                std::cout << menu;
                try {
                
                

                    std::cin >> menu;
                } catch (std::logic_error &e) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<int>().max(), '\n');
                    if(std::string(e.what()) == "Exit"){
                        // scene.~Scene();

                        finish = true;
                    }else{
                        std::cerr << std::endl
                                << std::endl
                                << "!!![ERROR]!!!" << std::endl;
                        std::cerr << e.what() << std::endl
                                << std::endl;
                    }
                    
                    
                }
                catch (...) {
                    std::cerr << "Fatal error, cautch ununderstable throw!!!" << std::endl;
                    scene.~Scene();
                    exit(-1);
                }
            }
            
        });

        displaying.join();
        menuig.join();

        // displaying.~thread();
        // menuig.~thread();
    }
    std::cout << "===========================================" << std::endl;
    // Cos sie ze staticami dzieje
    std::cout << std::setw(35) << "Number of Vectors on Scene: " << std::setw(10) << Vector3::HowManyObjects()-39 << std::endl;
    std::cout << std::setw(35) << "Number of Vectors from start: " << std::setw(10) << Vector3::AllHowManyObjects() << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}