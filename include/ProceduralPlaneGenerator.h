#ifndef PROCEDURAL_PLANE_GENERATOR_INCLUDED
#define PROCEDURAL_PLANE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"


// TODO : either use a "transform" and baisc parameters, or more complex parameters...

namespace Procedural
{

class PlaneGenerator : public MeshGenerator<PlaneGenerator>
{
    int numSegX;
    int numSegY;
    Ogre::Vector3 normal;
    float sizeX;
    float sizeY;
public:

    PlaneGenerator(): numSegX(1), numSegY(1),
        normal(Ogre::Vector3::UNIT_Y),
        sizeX(1), sizeY(1)
    {}


    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

    inline PlaneGenerator & setNumSegX(int numSegX)
    {
        this->numSegX = numSegX;
        return *this;
    }

    inline PlaneGenerator & setNumSegY(int numSegY)
    {
        this->numSegY = numSegY;
        return *this;
    }

    inline PlaneGenerator & setNormal(Ogre::Vector3 normal)
    {
        this->normal = normal;
        return *this;
    }

    inline PlaneGenerator & setSizeX(float sizeX)
    {
        this->sizeX = sizeX;
        return *this;
    }

    inline PlaneGenerator & setSizeY(float sizeY)
    {
        this->sizeY = sizeY;
        return *this;
    }

};
}
#endif
