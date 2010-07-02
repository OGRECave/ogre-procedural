#include "Ogre.h"

class SphereGenerator
{
	float radius;
	std::string name;
	unsigned int numRings;
	unsigned int numSegments;
	bool enableNormals;
	unsigned int numTexCoordSet;
	float uTile;
	float vTile;

public:
	SphereGenerator(Ogre::SceneManager* sceneManager) : radius(1.0), 
														name("default"), 
														numRings(8), numSegments(8), 
														enableNormals(true), 
														numTexCoordSet(1), 
														uTile(1.f), vTile(1.f), 
														sceneMgr(sceneManager)
	{}

	inline SphereGenerator& setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}

	inline SphereGenerator& setNumRings(unsigned int numRings)
	{
		this->numRings = numRings;
		return *this;
	}

	inline SphereGenerator& setNumSegments(unsigned int numSegments)
	{
		this->numSegments = numSegments;
		return *this;
	}


	inline SphereGenerator& setName(const std::string& name)
	{
		this->name = name;
		return *this;
	}

	Ogre::MeshPtr realize();

	Ogre::SceneManager* sceneMgr;

};