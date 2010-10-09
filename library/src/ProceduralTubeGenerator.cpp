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
#include "ProceduralTubeGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural {
void TubeGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	assert(height>0. && outerRadius>0. && innerRadius>0. && "Height and radius must be positive");
	assert(innerRadius<outerRadius && "Outer radius must be bigger than inner radius");
	assert(numSegBase>0 && numSegHeight>0 && "Num seg must be positive integers");
	

	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			Ogre::Real x0 = outerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = outerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);

			if (i != numSegHeight) {
				manual->index(offset + numSegBase + 1);
				manual->index(offset);
				manual->index(offset + numSegBase);
				manual->index(offset + numSegBase + 1);
				manual->index(offset + 1);
				manual->index(offset);
				}
					offset ++;
		}

	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(-Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);

			if (i != numSegHeight) {
				manual->index(offset + numSegBase + 1);
				manual->index(offset + numSegBase);
				manual->index(offset);
				manual->index(offset + numSegBase + 1);
				manual->index(offset);
				manual->index(offset + 1);
				}
					offset ++;
		}


		//low cap
		for (int j=0;j<=numSegBase;j++)
		{
			Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,vTile);

			x0 = outerRadius * cosf(j*deltaAngle);
			z0 = outerRadius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);

			if (j!=numSegBase)
			{
				manual->index(offset);
				manual->index(offset+1);
				manual->index(offset+3);
				manual->index(offset+2);
				manual->index(offset);
				manual->index(offset+3);
			}
			offset+=2;
		}


		//high cap
		for (int j=0;j<=numSegBase;j++)
		{
			Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);

			x0 = outerRadius * cosf(j*deltaAngle);
			z0 = outerRadius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,vTile);

			if (j!=numSegBase)
			{
				manual->index(offset+1);
				manual->index(offset);
				manual->index(offset+3);
				manual->index(offset);
				manual->index(offset+2);
				manual->index(offset+3);
			}
			offset+=2;
		}


	boundingRadius = Utils::max(outerRadius, height);
	aabb = Ogre::AxisAlignedBox(-outerRadius,0,-outerRadius,outerRadius, height, outerRadius);
}
}
