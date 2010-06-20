#pragma once

#include "Ogre.h"

class ProceduralPrimitiveFactory
{
	Ogre::SceneManager* sceneMgr;

	void _internalPlane(Ogre::ManualObject* manual, int& offset, const Ogre::Vector3& normal, int numSeg1, int numSeg2, const Ogre::Vector3& orig, const Ogre::Vector3& delta1, const Ogre::Vector3& delta2, Ogre::Real uTile, Ogre::Real vTile);


	public:
		ProceduralPrimitiveFactory(Ogre::SceneManager* sceneManager) : sceneMgr(sceneManager) {}
		
		// "BASIC" PRIMITIVES
		Ogre::MeshPtr createBox(const Ogre::String& name, Ogre::Real sizeX, Ogre::Real sizeY, Ogre::Real sizeZ, int numSegX, int numSegY, int numSegZ, Ogre::Real uTile, Ogre::Real vTile);
		Ogre::MeshPtr createCone(const Ogre::String& name, Ogre::Real radius, Ogre::Real height, int numSegHeight, int numSegBase, Ogre::Real uTile, Ogre::Real vTile);
		Ogre::MeshPtr createSphere(const Ogre::String& name, Ogre::Real r, int nRings, int nSegments, bool normals, int numTexCoordSet, Ogre::Real uTile, Ogre::Real vTile);
		//TODO : implement
		Ogre::MeshPtr createIsoSphere(const Ogre::String& name, Ogre::Real radius, int seg);
		Ogre::MeshPtr createCylinder(const Ogre::String& name, Ogre::Real radius, Ogre::Real height, bool capped, int numSegHeight, int numSegBase, Ogre::Real uTile, Ogre::Real vTile);
		Ogre::MeshPtr createTube(const Ogre::String& name, Ogre::Real innerRadius, Ogre::Real outerRadius, Ogre::Real height, int numSegHeight, int numSegBase, Ogre::Real uTile, Ogre::Real vTile);
		Ogre::MeshPtr createTorus(const Ogre::String& name, Ogre::Real radius, Ogre::Real sectionRadius, int numSegCircle, int numSegSection, Ogre::Real uTile, Ogre::Real vTile);
		//TODO : implement
		Ogre::MeshPtr createPyramid(const Ogre::String& name, Ogre::Real size, Ogre::Real height);
		//TODO : implement
		Ogre::MeshPtr createTeapot(const Ogre::String& name, Ogre::Real size);
		//TODO : implement
		Ogre::MeshPtr createPlane(const Ogre::String& name, const Ogre::Plane& plane, int numTexCoordSet=1, int numSegX=1, int numSegY=1, Ogre::Real uTile=1.0f, Ogre::Real vTile=1.0f);
		
		// EXTENDED PRIMITIVES
		//TODO : implement
		Ogre::MeshPtr createCapsule(const Ogre::String& name, Ogre::Real height, Ogre::Real radius, int numseg);
		//TODO : implement
		Ogre::MeshPtr createChamferBox(const Ogre::String& name, Ogre::Real sizeX, Ogre::Real sizeY, Ogre::Real sizeZ, Ogre::Real sizeChamfer, int numSegX, int numSegY, int numSegZ, Ogre::Real uTile, Ogre::Real vTile);
};