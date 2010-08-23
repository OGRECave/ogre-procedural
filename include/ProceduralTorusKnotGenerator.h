#ifndef PROCEDURAL_TORUS_KNOT_GENERATOR_INCLUDED
#define PROCEDURAL_TORUS_KNOT_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class TorusKnotGenerator : public MeshGenerator<TorusKnotGenerator>
{
    int numSegSection;
    int numSegCircle;
    float radius;
    float sectionRadius;
public:
    TorusKnotGenerator() : numSegSection(16),
                        numSegCircle(16),
                        radius(1.f),
                        sectionRadius(0.2f) {}


    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);


};
}
#endif
