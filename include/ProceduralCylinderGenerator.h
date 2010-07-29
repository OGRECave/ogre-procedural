#ifndef PROCEDURAL_CYLINDER_GENERATOR_INCLUDED
#define PROCEDURAL_CYLINDER_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class CylinderGenerator : public MeshGenerator
{
    int numSegBase;
    int numSegHeight;
    bool capped;
    float radius;
    float height;

public:
    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);

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
