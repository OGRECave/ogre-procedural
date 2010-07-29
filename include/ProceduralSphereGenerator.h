#ifndef PROCEDURAL_SPHERE_GENERATOR_INCLUDED
#define PROCEDURAL_SPHERE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{
class SphereGenerator : public MeshGenerator
{
    float radius;
    std::string name;
    unsigned int numRings;
    unsigned int numSegments;
    bool enableNormals;
    unsigned int numTexCoordSet;

public:
    SphereGenerator() : radius(1.0),
        name("default"),
        numRings(8), numSegments(8),
        enableNormals(true),
        numTexCoordSet(1)
    {}

	inline SphereGenerator & setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}

	inline SphereGenerator & setName(std::string name)
	{
		this->name = name;
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

	inline SphereGenerator & setEnableNormals(bool enableNormals)
	{
		this->enableNormals = enableNormals;
		return *this;
	}

	inline SphereGenerator & setNumTexCoordSet(unsigned int numTexCoordSet)
	{
		this->numTexCoordSet = numTexCoordSet;
		return *this;
	}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);

};
}
#endif
