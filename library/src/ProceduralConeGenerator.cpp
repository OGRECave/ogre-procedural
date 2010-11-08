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
#include "ProceduralConeGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void ConeGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	assert(height>0. && radius>0. && "Height and radius must be positive");
	assert(numSegBase>0 && numSegHeight>0 && "Num seg must be positive integers");
	
	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	Ogre::Vector3 refNormal = Ogre::Vector3(radius, height, 0.f).normalisedCopy();
	Ogre::Quaternion q;

	for (int i = 0; i <=numSegHeight; i++)
	{
		Ogre::Real r0 = radius * (1 - i / (Ogre::Real)numSegHeight);
		for (int j = 0; j<=numSegBase; j++)
		{
			Ogre::Real x0 = r0* cosf(j*deltaAngle);
			Ogre::Real z0 = r0 * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			q.FromAngleAxis(Ogre::Radian(-j*deltaAngle), Ogre::Vector3::UNIT_Y);
			manual->normal(q*refNormal);
			//if (i != numSegHeight)
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);
			/*else
			manual->textureCoord(0.f, i/(Ogre::Real)numSegHeight*vTile);*/

			if (i != numSegHeight&& j != numSegBase)
			{
				manual->index(offset + numSegBase + 2);
				manual->index(offset);
				manual->index(offset + numSegBase+1);
				manual->index(offset + numSegBase + +2);
				manual->index(offset + 1);
				manual->index(offset);
			}

			offset ++;
		}
	}

	//low cap
	int centerIndex = offset;
	manual->position(0,0,0);
	manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
	manual->textureCoord(0.0,vTile);
	offset++;
	for (int j=0; j<=numSegBase; j++)
	{
		Ogre::Real x0 = radius * cosf(j*deltaAngle);
		Ogre::Real z0 = radius * sinf(j*deltaAngle);

		manual->position(x0, 0.0f, z0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);
		if (j!=numSegBase)
		{
			manual->index(centerIndex);
			manual->index(offset);
			manual->index(offset+1);
		}
		offset++;
	}

/*    boundingRadius = Utils::max(radius, height);
	aabb = Ogre::AxisAlignedBox(-radius,0,-radius,radius, height, radius);*/
}
}
