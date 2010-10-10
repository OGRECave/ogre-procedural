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
#include "ProceduralExtruder.h"

namespace Procedural
{
	void Extruder::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)	
	{
		int numSegCircle = extrusionPath->getPoints().size();
		int numSegSection = shapeToExtrude->getPoints().size();

	for (int i = 0; i <= numSegCircle;i++)
	{
		Ogre::Vector3 v0(extrusionPath->getPoints()[i]);
		Ogre::Vector3 v1(extrusionPath->getPoints()[i]);
		Ogre::Vector3 direction((v1-v0).normalisedCopy());
				
		// First, compute an approximate quaternion (everything is ok except Roll angle)
		Ogre::Quaternion quat = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
		// Then, compute a correction quaternion : we want the "up" direction to be always the same
		Ogre::Vector3 projectedY = Ogre::Vector3::UNIT_Y - Ogre::Vector3::UNIT_Y.dotProduct(direction) * direction;		
		Ogre::Vector3 tY = quat * Ogre::Vector3::UNIT_Y;
		Ogre::Quaternion quat2 = tY.getRotationTo(projectedY);
		Ogre::Quaternion q = quat2 * quat;

		for (int j =0;j<=shapeToExtrude->getPoints().size();j++)
		{
			Ogre::Vector2 vp2 = shapeToExtrude->getPoints()[j];
			Ogre::Vector3 vp(vp2.x, vp2.y, 0);

			manual->position(v0+vp);
			manual->normal(vp.normalisedCopy());
			manual->textureCoord(i/(Ogre::Real)numSegCircle*uTile, j/(Ogre::Real)numSegSection*vTile);

			if (i != numSegCircle)
			{
				manual->index(offset + numSegSection + 1);
				manual->index(offset + numSegSection);
				manual->index(offset);
				manual->index(offset + numSegSection + 1);
				manual->index(offset);
				manual->index(offset + 1);
			}
			offset ++;
		}
	}

	}
}