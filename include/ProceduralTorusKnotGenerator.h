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
    TorusKnotGenerator() : numSegSection(8),
                        numSegCircle(16),
                        radius(1.f),
                        sectionRadius(.2f), p(2),q(3) {}


    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);


};
}
#endif
