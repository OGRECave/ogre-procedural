#ifndef PROCEDURAL_MESH_GENERATOR_INCLUDED
#define PROCEDURAL_MESH_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralRoot.h"

namespace Procedural
{
class MeshGenerator
{
protected:
    Ogre::SceneManager* sceneMgr;
    //std::string name;
    static int counter;
    float uTile;
    float vTile;
public:

    Ogre::MeshPtr realizeMesh(const std::string& name);
	Ogre::Entity* realizeEntity();
    virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax)=0;

    MeshGenerator() : uTile(1.f), vTile(1.f)
	{
	    sceneMgr = Root::getInstance()->sceneManager;
        assert(sceneMgr);
    }

    inline MeshGenerator & setUTile(float uTile)
	{
		this->uTile = uTile;
		return *this;
	}

	inline MeshGenerator & setVTile(float vTile)
	{
		this->vTile = vTile;
		return *this;
	}

};
//
}
#endif
