#ifndef PROCEDURAL_SPHERE_GENERATOR_INCLUDED
#define PROCEDURAL_SPHERE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{
class SphereGenerator : public MeshGenerator<SphereGenerator>
{
    float radius;
    unsigned int numRings;
    unsigned int numSegments;

public:
    SphereGenerator() : radius(1.0),
        numRings(16), numSegments(16)

    {}

	inline SphereGenerator & setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}

	inline SphereGenerator & setNumRings(unsigned int numRings)
	{
		this->numRings = numRings;
		return *this;
	}

	inline SphereGenerator & setNumSegments(unsigned int numSegments)
	{
		this->numSegments = numSegments;
		return *this;
	}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

};
}
#endif
