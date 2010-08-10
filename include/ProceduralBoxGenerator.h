#ifndef PROCEDURAL_BOX_GENERATOR_INCLUDED
#define PROCEDURAL_BOX_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{
class BoxGenerator : public MeshGenerator<BoxGenerator>
{
    float sizeX,sizeY,sizeZ;
    int numSegX,numSegY,numSegZ;
public:
    BoxGenerator() : sizeX(1.f), sizeY(1.f), sizeZ(1.f),
        numSegX(1), numSegY(1), numSegZ(1) {}


    BoxGenerator& setSizeX(float sizeX)
    {
        this->sizeX = sizeX;
        return *this;
    }

    BoxGenerator& setSizeY(float sizeY)
    {
        this->sizeY = sizeY;
        return *this;
    }

    BoxGenerator& setSizeZ(float sizeZ)
    {
        this->sizeZ = sizeZ;
        return *this;
    }

    BoxGenerator& setNumSegX(int numSegX)
    {
        this->numSegX = numSegX;
        return *this;
    }

    BoxGenerator& setNumSegY(int numSegY)
    {
        this->numSegY = numSegY;
        return *this;
    }

    BoxGenerator& setNumSegZ(int numSegZ)
    {
        this->numSegZ = numSegZ;
        return *this;
    }

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);

};


}
#endif
