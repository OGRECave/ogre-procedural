#include "ProceduralMeshGenerator.h"

#include "ProceduralRoot.h"

namespace Procedural
{
    int MeshGenerator::counter=0;

	Ogre::MeshPtr MeshGenerator::realizeMesh(std::string name)
	{
		Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		int offset=0;
		Ogre::Vector3 AABBmin(Ogre::Vector3::ZERO);
		Ogre::Vector3 AABBmax(Ogre::Vector3::ZERO);
		float radius(0.f);
		addToManualObject(manual, offset, radius, AABBmin, AABBmax);

		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-radius, -radius, -radius), Ogre::Vector3(radius, radius, radius) ), false );

		mesh->_setBoundingSphereRadius(radius);
        unsigned short src, dest;
	    if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		{
			mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		}

		return mesh;

	}

	MeshGenerator::MeshGenerator()
	{
	    sceneMgr = Root::getInstance()->sceneManager;
        assert(sceneMgr);
        counter++;
       // name = "defaultmesh_" + counter;
    }


}
