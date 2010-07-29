#include "ProceduralMeshGenerator.h"


namespace Procedural
{
    int MeshGenerator::counter=0;

	Ogre::MeshPtr MeshGenerator::realizeMesh(const std::string& name)
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

	Ogre::Entity* MeshGenerator::realizeEntity()
	{
        counter++;
        std::string meshName = "mesh_" + counter;
        realizeMesh(meshName);
        std::string entityName = "entity_" + counter;
	    Ogre::Entity* ent = sceneMgr->createEntity(entityName, meshName);
        Ogre::SceneNode* sn = sceneMgr->getRootSceneNode()->createChildSceneNode();
        sn->attachObject(ent);
        return ent;
	}
}
