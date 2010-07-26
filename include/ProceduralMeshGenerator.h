#ifndef PROCEDURAL_MESH_GENERATOR_INCLUDED
#define PROCEDURAL_MESH_GENERATOR_INCLUDED
#include "Ogre.h"
namespace Procedural
{
class MeshGenerator
{
protected:
    Ogre::SceneManager* sceneMgr;
    std::string name;
    static int counter;
    float uTile;
    float vTile;
public:
    MeshGenerator()
    {
        /*counter++;
        name = "defaultmesh_" + counter;*/
    }
    virtual ~MeshGenerator() {}
	Ogre::MeshPtr realizeMesh();
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax)=0;
};

}
#endif
