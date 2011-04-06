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
#include "ProceduralTorusGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void TorusGenerator::addToTriangleBuffer(TriangleBuffer& buffer) const
{
	assert(mNumSegSection>0 && mNumSegCircle>0 && "Num seg must be positive");
	assert(mRadius>0. && mSectionRadius>0. && "Radius must be positive");

	buffer.rebaseOffset();
	buffer.estimateVertexCount((mNumSegCircle+1)*(mNumSegSection+1));
	buffer.estimateIndexCount((mNumSegCircle)*(mNumSegSection+1)*6);

	Ogre::Real deltaSection = (Ogre::Math::TWO_PI / mNumSegSection);
	Ogre::Real deltaCircle = (Ogre::Math::TWO_PI / mNumSegCircle);
	int offset = 0;

	for (int i = 0; i <=mNumSegCircle; i++)
		for (int j = 0; j<=mNumSegSection; j++)
		{
			Ogre::Vector3 c0(mRadius, 0.0, 0.0);
			Ogre::Vector3 v0(mRadius+mSectionRadius * cosf(j*deltaSection),mSectionRadius * sinf(j*deltaSection),0.0);
			Ogre::Quaternion q;
			q.FromAngleAxis(Ogre::Radian(i*deltaCircle),Ogre::Vector3::UNIT_Y);
			Ogre::Vector3 v = q * v0;
			Ogre::Vector3 c = q * c0;
			buffer.position(v);
			buffer.normal((v-c).normalisedCopy());
			buffer.textureCoord(i/(Ogre::Real)mNumSegCircle*uTile, j/(Ogre::Real)mNumSegSection*vTile);

			if (i != mNumSegCircle)
			{
				buffer.index(offset + mNumSegSection + 1);
				buffer.index(offset);
				buffer.index(offset + mNumSegSection);
				buffer.index(offset + mNumSegSection + 1);
				buffer.index(offset + 1);
				buffer.index(offset);
			}
			offset ++;
		}
}
}
