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
    IcoSphereGenerator() : radius(1.),
        numIterations(2)
    {}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline IcoSphereGenerator & setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}

	inline IcoSphereGenerator & setNumIterations(unsigned int numIterations)
	{
		this->numIterations = numIterations;
		return *this;
	}

};
}
#endif
