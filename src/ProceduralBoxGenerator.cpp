#include "ProceduralBoxGenerator.h"

namespace Procedural
{
Ogre::MeshPtr BoxGenerator::realizeMesh()
{
Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	
	int offset=0;	
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_Z, numSegX, numSegY, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_Z,          numSegX, numSegY, Ogre::Vector3(-.5*sizeX,-.5*sizeY, .5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_Y, numSegX, numSegZ, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_Y,          numSegX, numSegZ, Ogre::Vector3(-.5*sizeX, .5*sizeY,-.5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_X, numSegY, numSegZ, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_X,          numSegY, numSegZ, Ogre::Vector3( .5*sizeX,-.5*sizeY,-.5*sizeZ), sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 

	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	Ogre::Real r = sizeX;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}
/*Ogre::MeshPtr ProceduralPrimitiveFactory::createBox(const Ogre::String& name, Ogre::Real sizeX, Ogre::Real sizeY, Ogre::Real sizeZ, int numSegX, int numSegY, int numSegZ, Ogre::Real uTile, Ogre::Real vTile)
{
	
}*/
}