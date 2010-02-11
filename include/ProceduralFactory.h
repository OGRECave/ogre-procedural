#pragma once

#include "Ogre.h"

class ProceduralFactory
{
	Ogre::SceneManager* sceneMgr;

	void _internalPlane(Ogre::ManualObject* manual, int& offset, const Ogre::Vector3& normal, int numSeg1, int numSeg2, const Ogre::Vector3& orig, const Ogre::Vector3& delta1, const Ogre::Vector3& delta2, float uTile, float vTile);


	public:
		ProceduralFactory(Ogre::SceneManager* sceneManager) : sceneMgr(sceneManager) {}

		Ogre::MeshPtr createBox(const Ogre::String& name, float sizeX, float sizeY, float sizeZ, int numSegX, int numSegY, int numSegZ, float uTile, float vTile);
		Ogre::MeshPtr createCone(const Ogre::String& name, float radius, float height, int numSegHeight, int numSegBase);
		Ogre::MeshPtr createSphere(const Ogre::String& name, float r, int nRings, int nSegments, bool normals, int numTexCoordSet, float uTile, float vTile);
		Ogre::MeshPtr createIsoSphere(const Ogre::String& name, float radius, int seg);
		Ogre::MeshPtr createCylinder(const Ogre::String& name, float radius, float height, bool capped, int numSegHeight, int numSegBase);
		Ogre::MeshPtr createTube(const Ogre::String& name, float innerRadius, float outerRadius, float height);
		Ogre::MeshPtr createTorus(const Ogre::String& name, float innerRadius, float outerRadius);
		Ogre::MeshPtr createPyramid(const Ogre::String& name, float size, float height);
		Ogre::MeshPtr createTeapot(const Ogre::String& name, float size);
		Ogre::MeshPtr createPlane(const Ogre::String& name, const Ogre::Plane& plane, int numTexCoordSet=1, int numSegX=1, int numSegY=1, float uTile=1.0f, float vTile=1.0f);


		//Ogre::MeshPtr createSphere(const Ogre::String& name, float r=1.0f, int nRings=8, int nSegments=8, bool normals=true, int numTexCoordSet=1, float uTile=1.0f, float vTile=1.0f);
		
		
		//Ogre::MeshPtr createBox(const Ogre::String& name, float sizeX=1.0f, float sizeY=1.0f, float sizeZ=1.0f, int numSegX=1, int numSegY=1, int numSegZ=1, float uTile=1.0f, float vTile=1.0f);
		
		
		
		Ogre::MeshPtr createCapsule(const Ogre::String& name, float height, float radius, int numseg);
		
		
		

};