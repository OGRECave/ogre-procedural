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
    int p;
    int q;
public:
    TorusKnotGenerator() : numSegSection(16),
                        numSegCircle(64),
                        radius(5.f),
                        sectionRadius(1.f), p(2),q(3) {}


    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);


};
}
#endif
