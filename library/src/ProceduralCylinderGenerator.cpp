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
#include "ProceduralCylinderGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{

void CylinderGenerator::addToTriangleBuffer(TriangleBuffer& buffer) const
{
	assert(mHeight>0. && mRadius>0. && "height and radius must be positive");
	assert(mNumSegBase>0 && mNumSegHeight>0 && "Num seg must be positive integers");

	buffer.rebaseOffset();
	if (mCapped)
	{
		buffer.estimateVertexCount((mNumSegHeight+1)*(mNumSegBase+1)+2*(mNumSegBase+1)+2);
		buffer.estimateIndexCount(mNumSegHeight*(mNumSegBase+1)*6+6*mNumSegBase);		
	} else {
		buffer.estimateVertexCount((mNumSegHeight+1)*(mNumSegBase+1));
		buffer.estimateIndexCount(mNumSegHeight*(mNumSegBase+1)*6);
	}


	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / mNumSegBase);
	Ogre::Real deltaHeight = mHeight/(Ogre::Real)mNumSegHeight;
	int offset = 0;

	for (int i = 0; i <=mNumSegHeight; i++)
		for (int j = 0; j<=mNumSegBase; j++)
		{
			Ogre::Real x0 = mRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = mRadius * sinf(j*deltaAngle);
			buffer.position(x0, i*deltaHeight, z0);
			buffer.normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			buffer.textureCoord(j/(Ogre::Real)mNumSegBase*uTile, i/(Ogre::Real)mNumSegHeight*vTile);

			if (i != mNumSegHeight) 
			{
				buffer.index(offset + mNumSegBase + 1);
				buffer.index(offset);
				buffer.index(offset + mNumSegBase);
				buffer.index(offset + mNumSegBase + 1);
				buffer.index(offset + 1);
				buffer.index(offset);
			}
			offset ++;
		}
	if (mCapped)
	{
		//low cap
		int centerIndex = offset;
		buffer.position(0,0,0);
		buffer.normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		buffer.textureCoord(0.0,vTile);
		offset++;
		for (int j=0;j<=mNumSegBase;j++)
		{
			Ogre::Real x0 = mRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = mRadius * sinf(j*deltaAngle);

			buffer.position(x0, 0.0f, z0);
			buffer.normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			buffer.textureCoord(j/(Ogre::Real)mNumSegBase,0.0);
			if (j!=mNumSegBase)
			{
				buffer.index(centerIndex);
				buffer.index(offset);
				buffer.index(offset+1);
			}
			offset++;
		}
		// high cap
		centerIndex = offset;
		buffer.position(0,mHeight,0);
		buffer.normal(Ogre::Vector3::UNIT_Y);
		buffer.textureCoord(0.0,0.0);
		offset++;
		for (int j=0;j<=mNumSegBase;j++)
		{
			Ogre::Real x0 = mRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = mRadius * sinf(j*deltaAngle);

			buffer.position(x0, mHeight, z0);
			buffer.normal(Ogre::Vector3::UNIT_Y);
			buffer.textureCoord(j/(Ogre::Real)mNumSegBase*uTile,vTile);
			if (j!=mNumSegBase)
			{
				buffer.index(centerIndex);
				buffer.index(offset+1);
				buffer.index(offset);
			}
			offset++;
		}
	}
}
}
