#include "config.hpp"
#include "Object.hpp"

#include <fstream>


#ifndef TMP_FOLDER
#define TMP_FOLDER "tmp/"
#endif  // !1


Object::Object(const std::string name, const Vector3 &centerPosition,
               const Vector3 &scale, Transform *const &pin) :
    Transform(pin), id{Object::HMO}, name{std::to_string(Object::aHMO) + "_" + name} {
    this->lenPointsPack = 0;
    this->scale[0][0] = scale[0];
    this->scale[1][1] = scale[1];
    this->scale[2][2] = scale[2];

    this->position = centerPosition;
    this->orientation = MatrixRot();

    std::ifstream readFile(DATA_FOLDER + name);
    if (readFile.good())
        readFile >> *this;
    else {
        system("pwd");
        throw std::logic_error("Cannot read object file! " + std::string(DATA_FOLDER) + name );
    }
    readFile.close();

    this->originPoints = std::vector<Vector3>(this->actualPoints.size(), Vector3());
    double minZ = 0;
    for (auto &point : this->actualPoints)
    {
        if(point[2] < minZ)
            minZ = point[2];
    }
    for (auto &point : this->actualPoints)
    {

        point -= VectorZ * minZ;
    }
    this->originPoints = this->actualPoints;

    ++Object::HMO;
    ++Object::aHMO;
    // this->name = std::to_string(this->id) + "_" + name;
    this->UpdatePoints();
}

// Object::Object(const Object &obj) :
//     Transform(obj.pinnedTransform),
//     name{obj.name},
//     id{Object::HMO} {
//     this->actualPoints.clear();
//     this->originPoints.clear();
//     this->actualPoints = std::vector<Vector3>(obj.CountPoints(), Vector3());
//     this->originPoints = std::vector<Vector3>(obj.CountPoints(), Vector3());

//     this->lenPointsPack = obj.LengthOfPointPack();
//     this->orientation = obj.orientation;
//     this->scale = obj.scale;
//     this->position = obj.position;

//     this->originPoints = obj.OriginPoints();
//     for (std::size_t i = 0; i < obj.CountPoints(); ++i) {
//         this->actualPoints[i] = obj[i];
//     }

//     this->originPoints = this->actualPoints;
//     ++Object::HMO;
// }
Object::~Object() {
    system((std::string("rm ") + std::string(TMP_FOLDER) + this->Name()).c_str());
    this->originPoints.clear();
    this->actualPoints.clear();
    --Object::HMO;
}

Vector3 &Object::operator[](const std::size_t &index) {
    if (index >= this->actualPoints.size())
        throw std::overflow_error("Vector overflow!");
    return this->actualPoints[index];
}

Vector3 Object::operator[](const std::size_t &index) const {
    if (index >= this->actualPoints.size())
        throw std::overflow_error("Vector overflow!");
    return this->actualPoints[index];
}

void Object::Save() {
    std::ofstream tmpFile(TMP_FOLDER + this->Name());
    if (tmpFile)
        tmpFile << *this;
    tmpFile.close();
}

std::vector<Vector3> Object::OriginPoints() const {
    return this->originPoints;
}

void Object::UpdatePoints() {
    this->actualPoints = this->originPoints;

    for (auto &point : this->actualPoints) {
        point = this->ConvertToGlobal(point);
    }

    this->Save();
}

std::istream &operator>>(std::istream &strm, Object &object) {
    object.actualPoints.clear();
    std::size_t onePack = 0;
    while (!strm.eof()) {
        if (strm.peek() == '#') {
            strm.get();
        } else if (strm.peek() == '\n') {
            strm.get();
            if (strm.peek() == '\n') {
                object.lenPointsPack = onePack;
            }
        }

        Vector3 point;
        try {
            strm >> point;
        } catch (std::logic_error &e) {
            // point.~Vector3();
            strm.clear();
            return strm;
        }

        if (!strm)
            throw std::logic_error("Cannot read Object!");

        object.actualPoints.push_back(point);

        if (object.lenPointsPack == 0)
            ++onePack;
    }
    return strm;
}

std::ostream &operator<<(std::ostream &strm, const Object &object) {
    std::size_t k = 1;
    for (std::size_t i = 0; i < object.CountPoints(); ++i) {
        strm << object[i] << std::endl;
        if (k == object.LengthOfPointPack()) {
            strm << "#\n\n";
            k = 0;
        }
        if (!strm)
            throw std::logic_error("Cannot read Object!");
        ++k;
    }
    return strm;
}