#ifndef PROCEDURAL_CYLINDER_GENERATOR_INCLUDED
#define PROCEDURAL_CYLINDER_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class CylinderGenerator : public MeshGenerator<CylinderGenerator>
{
    int numSegBase;
    int numSegHeight;
    bool capped;
    float radius;
    float height;

public:
    CylinderGenerator() : numSegBase(8),
        numSegHeight(1),
        capped(true),
        radius(1.f),
        height(1.f)
    {}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

    inline CylinderGenerator & setNumSegBase(int numSegBase)
    {
        this->numSegBase = numSegBase;
        return *this;
    }

    inline CylinderGenerator & setNumSegHeight(int numSegHeight)
    {
        this->numSegHeight = numSegHeight;
        return *this;
    }

    inline CylinderGenerator & setCapped(bool capped)
    {
        this->capped = capped;
        return *this;
    }

    inline CylinderGenerator & setRadius(float radius)
    {
        this->radius = radius;
        return *this;
    }

    inline CylinderGenerator & setHeight(float height)
    {
        this->height = height;
        return *this;
    }

};
}

#endif
