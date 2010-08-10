#ifndef PROCEDURAL_TORUS_GENERATOR_INCLUDED
#define PROCEDURAL_TORUS_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class TorusGenerator : public MeshGenerator<TorusGenerator>
{
    int numSegSection;
    int numSegCircle;
    float radius;
    float sectionRadius;
public:
    TorusGenerator() : numSegSection(8),
                        numSegCircle(8),
                        radius(5.f),
                        sectionRadius(1.f) {}


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
