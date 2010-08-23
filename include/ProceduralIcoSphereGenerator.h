#ifndef PROCEDURAL_ICOSPHERE_GENERATOR_INCLUDED
#define PROCEDURAL_ICOSPHERE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{
class IcoSphereGenerator : public MeshGenerator<IcoSphereGenerator>
{
    float radius;
    unsigned int numIterations;

public:
    IcoSphereGenerator() : radius(1.0),
        numIterations(1)
    {}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

};
}
#endif
