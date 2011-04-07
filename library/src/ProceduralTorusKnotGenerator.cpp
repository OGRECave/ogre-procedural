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
#include "ProceduralStableHeaders.h"
#include "ProceduralTorusKnotGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void TorusKnotGenerator::addToTriangleBuffer(TriangleBuffer& buffer) const
{
	assert(mNumSegSection>0 && mNumSegCircle>0 && "Num seg and circle must be positive");
	assert(mRadius>0. && mSectionRadius>0. && "Radius and section radius must be positive");
	assert(mP>0 && mQ>0 && "p and q must be positive");

	buffer.rebaseOffset();
	buffer.estimateVertexCount((mNumSegCircle*mP+1)*(mNumSegSection+1));
	buffer.estimateIndexCount((mNumSegCircle*mP)*(mNumSegSection+1)*6);

	int offset = 0;

	for (int i = 0; i <= mNumSegCircle * mP;i++)
	{
		Ogre::Real phi = Ogre::Math::TWO_PI * i/(Ogre::Real)mNumSegCircle;
		Ogre::Real x0 = mRadius*(2 + cos(mQ*phi/(Ogre::Real)mP)) * cos(phi) / 3.f;
		Ogre::Real y0 = mRadius*sin(mQ*phi/(Ogre::Real)mP) / 3.f;
		Ogre::Real z0 = mRadius*(2 + cos(mQ*phi/(Ogre::Real)mP)) * sin(phi) / 3.f;

		Ogre::Real phi1 = Ogre::Math::TWO_PI * (i+1)/(Ogre::Real)mNumSegCircle;
		Ogre::Real x1 = mRadius*(2 + cos(mQ*phi1/mP)) * cos(phi1) / 3.f;
		Ogre::Real y1 = mRadius*sin(mQ*phi1/mP) / 3.f;
		Ogre::Real z1 = mRadius*(2 + cos(mQ*phi1/mP)) * sin(phi1) / 3.f;

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

		for (int j =0;j<=mNumSegSection;j++)
		{
			Ogre::Real alpha = Ogre::Math::TWO_PI *j/mNumSegSection;
			Ogre::Vector3 vp = mSectionRadius*(mQ * Ogre::Vector3(cos(alpha), sin(alpha),0));

			buffer.position(v0+vp);
			buffer.normal(vp.normalisedCopy());
			buffer.textureCoord(i/(Ogre::Real)mNumSegCircle*uTile, j/(Ogre::Real)mNumSegSection*vTile);

			if (i != mNumSegCircle * mP)
			{
				buffer.index(offset + mNumSegSection + 1);
				buffer.index(offset + mNumSegSection);
				buffer.index(offset);
				buffer.index(offset + mNumSegSection + 1);
				buffer.index(offset);
				buffer.index(offset + 1);
			}
			offset ++;
		}
	}
}
}
