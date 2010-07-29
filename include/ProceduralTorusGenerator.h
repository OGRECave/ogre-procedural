#ifndef PROCEDURAL_TORUS_GENERATOR_INCLUDED
#define PROCEDURAL_TORUS_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class TorusGenerator : public MeshGenerator
{
    int numSegSection;
    int numSegCircle;
    float radius;
    float sectionRadius;
public:
    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);
	inline TorusGenerator & setNumSegSection(int numSegSection)
	{
		this->numSegSection = numSegSection;
		return *this;
	}

	inline TorusGenerator & setNumSegCircle(int numSegCircle)
	{
		this->numSegCircle = numSegCircle;
		return *this;
	}

	inline TorusGenerator & setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}

	inline TorusGenerator & setSectionRadius(float sectionRadius)
	{
		this->sectionRadius = sectionRadius;
		return *this;
	}

};
}
#endif
