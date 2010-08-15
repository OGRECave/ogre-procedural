#include "ProceduralBoxGenerator.h"
#include "ProceduralPlaneGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void BoxGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	PlaneGenerator pg;
	pg.setUTile(uTile).setVTile(vTile);
	pg.setNumSegX(numSegX).setNumSegY(numSegY).setSizeX(sizeX).setSizeY(sizeY)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_Z)
	  .setPosition(.5*sizeZ*Ogre::Vector3::NEGATIVE_UNIT_Z)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
    pg.setNumSegX(numSegX).setNumSegY(numSegY).setSizeX(sizeX).setSizeY(sizeY)
	  .setNormal(Ogre::Vector3::UNIT_Z)
	  .setPosition(.5*sizeZ*Ogre::Vector3::UNIT_Z)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
    pg.setNumSegX(numSegX).setNumSegY(numSegZ).setSizeX(sizeX).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_Y)
	  .setPosition(.5*sizeY*Ogre::Vector3::NEGATIVE_UNIT_Y)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
    pg.setNumSegX(numSegX).setNumSegY(numSegZ).setSizeX(sizeX).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::UNIT_Y)
	  .setPosition(.5*sizeY*Ogre::Vector3::UNIT_Y)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
    pg.setNumSegX(numSegY).setNumSegY(numSegZ).setSizeX(sizeY).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_X)
	  .setPosition(.5*sizeZ*Ogre::Vector3::NEGATIVE_UNIT_X)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
    pg.setNumSegX(numSegY).setNumSegY(numSegZ).setSizeX(sizeY).setSizeY(sizeZ)
	  .setNormal(Ogre::Vector3::UNIT_X)
	  .setPosition(.5*sizeZ*Ogre::Vector3::UNIT_X)
	  .addToManualObject(manual, offset, boundingRadius, aabb);

    aabb.setExtents(-.5*sizeX, -.5*sizeY, -.5*sizeZ,.5*sizeX, .5*sizeY, .5*sizeZ);
    boundingRadius = Ogre::Math::Sqrt(sizeX*sizeX + sizeY*sizeY + sizeZ*sizeZ);
}
}
