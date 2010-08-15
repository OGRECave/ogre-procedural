#ifndef PROCEDURAL_TUBE_GENERATOR_INCLUDED
#define PROCEDURAL_TUBE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class TubeGenerator : public MeshGenerator<TubeGenerator>
{
    int numSegBase;
    int numSegHeight;
    float outerRadius;
    float innerRadius;
    float height;

public:
    TubeGenerator() : numSegBase(8),
        numSegHeight(1),
        outerRadius(2.f),
        innerRadius(1.f),
        height(1.f) {}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

    inline TubeGenerator & setNumSegBase(int numSegBase)
    {
        this->numSegBase = numSegBase;
        return *this;
    }

    inline TubeGenerator & setNumSegHeight(int numSegHeight)
    {
        this->numSegHeight = numSegHeight;
        return *this;
    }

    inline TubeGenerator & setOuterRadius(float outerRadius)
    {
        this->outerRadius = outerRadius;
        return *this;
    }

    inline TubeGenerator & setInnerRadius(float innerRadius)
    {
        this->innerRadius = innerRadius;
        return *this;
    }

    inline TubeGenerator & setHeight(float height)
    {
        this->height = height;
        return *this;
    }
};
}
#endif
