#ifndef PROCEDURAL_CONE_GENERATOR_INCLUDED
#define PROCEDURAL_CONE_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class ConeGenerator : public MeshGenerator<ConeGenerator>
{
    int numSegBase;
    int numSegHeight;
    float radius;
    float height;
public:
    ConeGenerator() : numSegBase(16),
        numSegHeight(1),
        radius(1.f),
        height(1.f)
    {}


    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

    inline ConeGenerator & setNumSegBase(int numSegBase)
	{
		this->numSegBase = numSegBase;
		return *this;
	}

	inline ConeGenerator & setNumSegHeight(int numSegHeight)
	{
		this->numSegHeight = numSegHeight;
		return *this;
	}

	inline ConeGenerator & setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}

	inline ConeGenerator & setHeight(float height)
	{
		this->height = height;
		return *this;
	}


};
}

#endif
