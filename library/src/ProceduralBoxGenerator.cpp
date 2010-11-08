/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010 Michael Broutin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "ProceduralBoxGenerator.h"
#include "ProceduralPlaneGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void BoxGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	assert(numSegX>0 && numSegY>0 && numSegZ>0 && "Num seg must be positive integers");
	assert(sizeX>0. && sizeY>0. && sizeZ>0. && "Sizes must be positive");

	PlaneGenerator pg;
	pg.setUTile(uTile).setVTile(vTile);
	pg.setNumSegX(numSegY).setNumSegY(numSegX).setSizeX(sizeY).setSizeY(sizeX)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_Z)
	  .setPosition(.5*sizeZ*Ogre::Vector3::NEGATIVE_UNIT_Z)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegY).setNumSegY(numSegX).setSizeX(sizeY).setSizeY(sizeX)
	  .setNormal(Ogre::Vector3::UNIT_Z)
	  .setPosition(.5*sizeZ*Ogre::Vector3::UNIT_Z)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegX).setSizeX(sizeZ).setSizeY(sizeX)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_Y)
	  .setPosition(.5*sizeY*Ogre::Vector3::NEGATIVE_UNIT_Y)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegX).setSizeX(sizeZ).setSizeY(sizeX)
	  .setNormal(Ogre::Vector3::UNIT_Y)
	  .setPosition(.5*sizeY*Ogre::Vector3::UNIT_Y)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegY).setSizeX(sizeZ).setSizeY(sizeY)
	  .setNormal(Ogre::Vector3::NEGATIVE_UNIT_X)
	  .setPosition(.5*sizeX*Ogre::Vector3::NEGATIVE_UNIT_X)
	  .addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegY).setSizeX(sizeZ).setSizeY(sizeY)
	  .setNormal(Ogre::Vector3::UNIT_X)
	  .setPosition(.5*sizeX*Ogre::Vector3::UNIT_X)
	  .addToManualObject(manual, offset, boundingRadius, aabb);

	aabb.setExtents(-.5*sizeX, -.5*sizeY, -.5*sizeZ,.5*sizeX, .5*sizeY, .5*sizeZ);
	boundingRadius = Ogre::Math::Sqrt(sizeX*sizeX + sizeY*sizeY + sizeZ*sizeZ);
}
}
