#ifndef PROCEDURAL_CAPSULE_GENERATOR_INCLUDED
#define PROCEDURAL_CAPSULE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{
class CapsuleGenerator : public MeshGenerator<CapsuleGenerator>
{
    float radius;
    unsigned int numRings;
    unsigned int numSegments;
    unsigned int numSegHeight;
    float height;

public:
    CapsuleGenerator() : radius(1.0),
        numRings(8), numSegments(16), height(1.0), numSegHeight(1)

    {}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);


    inline CapsuleGenerator & setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}

	inline CapsuleGenerator & setNumRings(unsigned int numRings)
	{
		this->numRings = numRings;
		return *this;
	}

	inline CapsuleGenerator & setNumSegments(unsigned int numSegments)
	{
		this->numSegments = numSegments;
		return *this;
	}

	inline CapsuleGenerator & setNumSegHeight(unsigned int numSegHeight)
	{
		this->numSegHeight = numSegHeight;
		return *this;
	}

	inline CapsuleGenerator & setHeight(float height)
	{
		this->height = height;
		return *this;
	}


};
}
#endif
