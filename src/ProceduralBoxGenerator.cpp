#include "ProceduralBoxGenerator.h"
#include "ProceduralPlaneGenerator.h"

namespace Procedural
{
void BoxGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax)
{
	PlaneGenerator pg;
	pg.setUTile(uTile).setVTile(vTile);
	pg.setNumSegX(numSegX).setNumSegY(numSegY).setSizeX(sizeX).setSizeY(sizeY)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_Z)
	  .setPosition(.5*sizeZ*Ogre::Vector3::NEGATIVE_UNIT_Z)
	  .addToManualObject(manual, offset, boundingRadius, AABBmin, AABBmax);
    pg.setNumSegX(numSegX).setNumSegY(numSegY).setSizeX(sizeX).setSizeY(sizeY)
	  .setNormal(Ogre::Vector3::UNIT_Z)
	  .setPosition(.5*sizeZ*Ogre::Vector3::UNIT_Z)
	  .addToManualObject(manual, offset, boundingRadius, AABBmin, AABBmax);
    pg.setNumSegX(numSegX).setNumSegY(numSegZ).setSizeX(sizeX).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_Y)
	  .setPosition(.5*sizeY*Ogre::Vector3::NEGATIVE_UNIT_Y)
	  .addToManualObject(manual, offset, boundingRadius, AABBmin, AABBmax);
    pg.setNumSegX(numSegX).setNumSegY(numSegZ).setSizeX(sizeX).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::UNIT_Y)
	  .setPosition(.5*sizeY*Ogre::Vector3::UNIT_Y)
	  .addToManualObject(manual, offset, boundingRadius, AABBmin, AABBmax);
    pg.setNumSegX(numSegY).setNumSegY(numSegZ).setSizeX(sizeY).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_X)
	  .setPosition(.5*sizeZ*Ogre::Vector3::NEGATIVE_UNIT_X)
	  .addToManualObject(manual, offset, boundingRadius, AABBmin, AABBmax);
    pg.setNumSegX(numSegY).setNumSegY(numSegZ).setSizeX(sizeY).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::UNIT_X)
	  .setPosition(.5*sizeZ*Ogre::Vector3::UNIT_X)
	  .addToManualObject(manual, offset, boundingRadius, AABBmin, AABBmax);

//	pg.setUTile(uTile).setVTile(vTile);
	//pg.setNumSeg1(numSegX).setNumSeg2(numSegY).setNormal(Ogre::Vector3::NEGATIVE_UNIT_Z).setCenter(Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ)).setSize1().setSize2()
	/*
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_Z, numSegX, numSegY, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, uTile, vTile);
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_Z,          numSegX, numSegY, Ogre::Vector3(-.5*sizeX,-.5*sizeY, .5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, uTile, vTile);
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_Y, numSegX, numSegZ, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile);
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_Y,          numSegX, numSegZ, Ogre::Vector3(-.5*sizeX, .5*sizeY,-.5*sizeZ), sizeX/(Ogre::Real)numSegX*Ogre::Vector3::UNIT_X, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile);
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_X, numSegY, numSegZ, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile);
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_X,          numSegY, numSegZ, Ogre::Vector3( .5*sizeX,-.5*sizeY,-.5*sizeZ), sizeY/(Ogre::Real)numSegY*Ogre::Vector3::UNIT_Y, sizeZ/(Ogre::Real)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile);
*/
}
/*Ogre::MeshPtr ProceduralPrimitiveFactory::createBox(const Ogre::String& name, Ogre::Real sizeX, Ogre::Real sizeY, Ogre::Real sizeZ, int numSegX, int numSegY, int numSegZ, Ogre::Real uTile, Ogre::Real vTile)
{

}*/
}
