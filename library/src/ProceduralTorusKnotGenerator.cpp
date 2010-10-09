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
#include "ProceduralTorusKnotGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void TorusKnotGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	assert(numSegSection>0 && numSegCircle>0 && "Num seg must be positive");
	assert(radius>0. && sectionRadius>0. && "Radius must be positive");
	assert(p>0 && q>0 && "p and q must be positive");

	for (int i = 0; i <= numSegCircle * p;i++)
	{
		float phi = Ogre::Math::TWO_PI * i/(float)numSegCircle;
		float x0 = radius*(2 + cos(q*phi/(float)p)) * cos(phi) / 3.f;
		float y0 = radius*sin(q*phi/(float)p) / 3.f;
		float z0 = radius*(2 + cos(q*phi/(float)p)) * sin(phi) / 3.f;

		float phi1 = Ogre::Math::TWO_PI * (i+1)/(float)numSegCircle;
		float x1 = radius*(2 + cos(q*phi1/p)) * cos(phi1) / 3.f;
		float y1 = radius*sin(q*phi1/p) / 3.f;
		float z1 = radius*(2 + cos(q*phi1/p)) * sin(phi1) / 3.f;

		Ogre::Vector3 v0(x0,y0,z0);
		Ogre::Vector3 v1(x1,y1,z1);
		Ogre::Vector3 direction((v1-v0).normalisedCopy());
				
		// First, compute an approximate quaternion (everything is ok except Roll angle)
		Ogre::Quaternion quat = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
		// Then, compute a correction quaternion : we want the "up" direction to be always the same
		Ogre::Vector3 projectedY = Ogre::Vector3::UNIT_Y - Ogre::Vector3::UNIT_Y.dotProduct(direction) * direction;		
		Ogre::Vector3 tY = quat * Ogre::Vector3::UNIT_Y;
		Ogre::Quaternion quat2 = tY.getRotationTo(projectedY);
		Ogre::Quaternion q = quat2 * quat;

		for (int j =0;j<=numSegSection;j++)
		{
			float alpha = Ogre::Math::TWO_PI *j/numSegSection;
			Ogre::Vector3 vp = sectionRadius*(q * Ogre::Vector3(cos(alpha), sin(alpha),0));

			manual->position(v0+vp);
			manual->normal(vp.normalisedCopy());
			manual->textureCoord(i/(Ogre::Real)numSegCircle*uTile, j/(Ogre::Real)numSegSection*vTile);

			if (i != numSegCircle * p)
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

	boundingRadius = Ogre::Math::Sqrt(2*Ogre::Math::Sqr(radius + sectionRadius)+Ogre::Math::Sqr(sectionRadius));
	aabb = Ogre::AxisAlignedBox(-radius-sectionRadius,-sectionRadius,-radius-sectionRadius, radius+sectionRadius, sectionRadius, radius+sectionRadius);
}
}
