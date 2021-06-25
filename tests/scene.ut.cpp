#include "Scene.hpp"

#include <limits>

#include "doctest.h"
extern bool DISPLAY;

TEST_CASE("1. Scene constructor") {
    CHECK_NOTHROW(Scene scene);
}

TEST_CASE("2. Draw without making scene"){
    if (DISPLAY) {
        Scene::Draw(new Rotor(SpinDirection_t::Clockwise));
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.ignore(std::numeric_limits<int>().max(), '\n');
    }
}
TEST_CASE("3. Cuboid"){
    if (DISPLAY) {
        Scene::Draw(new Cuboid({6, 10 ,9}));
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.ignore(std::numeric_limits<int>().max(), '\n');
    }
}
TEST_CASE("4. Draw by scene") {
    Scene scene;
    auto drone = std::make_shared<Drone>();

    CHECK_NOTHROW(scene.Add(drone));

    if (DISPLAY) {
        scene.Update();
        std::cout << "Press Enter to continue..." << std::endl;
        std::cin.ignore(std::numeric_limits<int>().max(), '\n');
    }
}

// // TEST_CASE("3 Pointners to objects") {
// //     Scene scene;
// //     scene.AddObject(Object("line.dat"));

// //     Object *obj = &scene[0];

// //     CHECK(obj == &scene[0]);
// //     if (DISPLAY) {
// //         scene.Update();
// //         std::cout << "Press Enter to continue..." << std::endl;
// //         std::cin.ignore(std::numeric_limits<int>().max(), '\n');
// //     }

// //     obj->Rotate(90, VectorZ);

// //     if (DISPLAY) {
// //         scene.Update();
// //         CHECK(scene[0][0] == Vector3({0, 100, 0}));
// //         std::cout << "Press Enter to continue..." << std::endl;
// //         std::cin.ignore(std::numeric_limits<int>().max(), '\n');
// //     }
// //     scene.Update();
// //     CHECK(scene[0][0] == Vector3({0, 100, 0}));

// //     CHECK_THROWS(obj = &scene[9]);

// //     scene.AddObject(Object("line.dat"));
// //     obj = &scene[1];
// //     obj->orientation = MatrixRot(90, VectorZ) * obj->orientation;
// //     if (DISPLAY) {
// //         scene.Update();
// //         std::cout << "Press Enter to continue..." << std::endl;
// //         std::cin.ignore(std::numeric_limits<int>().max(), '\n');
// //     }
// //     scene.Update();
// //     CHECK(scene[1][0] == Vector3({0, 100, 0}));
// // }
// // 
// // TEST_CASE("11. Lambda test") {
// //     Object *obj;
// //     Scene scene;
// //     scene.AddObject(Object("line.dat"));
// //     obj = &scene[0];
// //     if (DISPLAY) {
// //         scene.Update();
// //         std::cout << "Press Enter to continue..." << std::endl;
// //         std::cin.ignore(std::numeric_limits<int>().max(), '\n');
// //     }
// //     auto lambda = [&obj]() {
// //         obj->orientation = MatrixRot(90, VectorZ) * obj->orientation;
// //     };

// //     CHECK(scene[0][0] == Vector3({100, 0, 0}));
// //     if (DISPLAY) {
// //         scene.Update();
// //         std::cout << "Press Enter to continue..." << std::endl;
// //         std::cin.ignore(std::numeric_limits<int>().max(), '\n');
// //     }
// //     lambda();
// //     scene.Update();
// //     CHECK(scene[0][0] == Vector3({0, 100, 0}));
// // }