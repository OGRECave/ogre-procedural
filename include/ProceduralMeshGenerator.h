#ifndef PROCEDURAL_MESH_GENERATOR_INCLUDED
#define PROCEDURAL_MESH_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralRoot.h"

namespace Procedural
{
template <typename T>
class MeshGenerator
{
protected:
    Ogre::SceneManager* sceneMgr;
    float uTile;
    float vTile;

    bool enableNormals;
    unsigned int numTexCoordSet;
public:

    Ogre::MeshPtr realizeMesh(const std::string& name)
    {
		Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		int offset=0;
		Ogre::AxisAlignedBox aabb;
		float radius(0.f);
		addToManualObject(manual, offset, radius, aabb);

		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		mesh->_setBounds( aabb, false );
		mesh->_setBoundingSphereRadius(radius);

        unsigned short src, dest;
	    if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		{
			mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		}

		return mesh;
	}

    virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)=0;

    MeshGenerator() : uTile(1.f),
                      vTile(1.f),
                      enableNormals(true),
                      numTexCoordSet(1)
	{
	    sceneMgr = Root::getInstance()->sceneManager;
        assert(sceneMgr);
    }

    inline T& setUTile(float uTile)
	{
		this->uTile = uTile;
		return static_cast<T&>(*this);
	}

	inline T & setVTile(float vTile)
	{
		this->vTile = vTile;
		return static_cast<T&>(*this);
	}

    inline T & setEnableNormals(bool enableNormals)
	{
		this->enableNormals = enableNormals;
		return static_cast<T&>(*this);
	}

	inline T & setNumTexCoordSet(unsigned int numTexCoordSet)
	{
		this->numTexCoordSet = numTexCoordSet;
		return static_cast<T&>(*this);
	}

};
//
}
#endif
