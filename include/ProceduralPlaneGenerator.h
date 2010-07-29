#ifndef PROCEDURAL_PLANE_GENERATOR_INCLUDED
#define PROCEDURAL_PLANE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"


// TODO : either use a "transform" and baisc parameters, or more complex parameters...

namespace Procedural
{

class PlaneGenerator : public MeshGenerator
{
    int numSeg1;
    int numSeg2;
    Ogre::Vector3 normal;
public:
    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);

    PlaneGenerator& setNumSeg1(int numSeg1)
    {
        this->numSeg1 = numSeg1;
        return *this;
    }

    PlaneGenerator& setNumSeg2(int numSeg2)
    {
        this->numSeg2 = numSeg2;
        return *this;
    }
};
}
#endif
