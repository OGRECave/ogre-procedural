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
#include "ProceduralExtruder.h"
#include "ProceduralTriangulator.h"
#include "ProceduralGeometryHelpers.h"

namespace Procedural
{
	void Extruder::addToTriangleBuffer(TriangleBuffer& buffer) const
	{
		assert(mExtrusionPath && mShapeToExtrude && "Shape and Path must not be null!");
		int numSegPath = mExtrusionPath->getSegCount();
		int numSegShape = mShapeToExtrude->getSegCount();
		assert(numSegPath>0 && numSegShape>0 && "Shape and path must contain at least two points");
				
		// Triangulate the begin and end caps
		std::vector<int> indexBuffer;
		if (!mExtrusionPath->isClosed() && mCapped)
			Triangulator::triangulate(*mShapeToExtrude, indexBuffer);

		// Estimate vertex and index count
		buffer.rebaseOffset();
		if (!mExtrusionPath->isClosed() && mCapped)
		{
			buffer.estimateIndexCount(numSegShape*numSegPath*6 + 2*indexBuffer.size());
			buffer.estimateVertexCount((numSegShape+1)*(numSegPath+1) + 2*(numSegShape+1));
		} else {
			buffer.estimateIndexCount(numSegShape*numSegPath*6);
			buffer.estimateVertexCount((numSegShape+1)*(numSegPath+1));
		}

		Ogre::Quaternion qBegin, qEnd/*, lastQ*/;		
		//Ogre::Vector3 lastV0;

	for (int i = 0; i <= numSegPath;i++)
	{		
		Ogre::Vector3 v0 = mExtrusionPath->getPoint(i);

		Ogre::Vector3 direction = mExtrusionPath->getAvgDirection(i);

		// First, compute an approximate quaternion (everything is ok except Roll angle)
		Ogre::Quaternion quat = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
		// Then, compute a correction quaternion : we want the "up" direction to be always the same
		Ogre::Vector3 projectedY = Ogre::Vector3::UNIT_Y - Ogre::Vector3::UNIT_Y.dotProduct(direction) * direction;
		Ogre::Vector3 tY = quat * Ogre::Vector3::UNIT_Y;
		Ogre::Quaternion quat2 = tY.getRotationTo(projectedY);
		Ogre::Quaternion q = quat2 * quat;
		if (i == 0) qBegin = q;
		if (i == numSegPath) qEnd = q;

		/*if (mFixSharpAngles && i>0)
		{
			Plane plane1(lastQ * Ogre::Vector3::UNIT_Z, lastV0);
			Plane plane2(q * Ogre::Vector3::UNIT_Z, v0);
			Line inter;
			if (plane1.intersect(plane2, inter))
			{
				Ogre::Vector3 v = inter.shortestPathToPoint(v0);
				if (v.length() < 2.0) // TODO : shape.boundingCircle
				{
					v0 = v0 + (2.0-v.length()) * v.normalisedCopy();
				}
			}
		}
		lastQ = q;
		lastV0 = v0;*/

		for (int j =0;j<=numSegShape;j++)
		{
			Ogre::Vector2 vp2 = mShapeToExtrude->getPoint(j);
			Ogre::Vector2 vp2direction = mShapeToExtrude->getAvgDirection(j);
			Ogre::Vector2 vp2normal = mShapeToExtrude->getAvgNormal(j);
			Ogre::Vector3 vp(vp2.x, vp2.y, 0);
			Ogre::Vector3 normal(vp2normal.x, vp2normal.y, 0);							
			buffer.rebaseOffset();
			Ogre::Vector3 newPoint = v0+q*vp;
			buffer.position(newPoint);
			buffer.normal(q*normal);
			buffer.textureCoord(i/(Ogre::Real)numSegPath*uTile, j/(Ogre::Real)numSegShape*vTile);

			if (j <numSegShape && i <numSegPath)
			{		
				if (mShapeToExtrude->getOutSide() == SIDE_LEFT)
				{
					buffer.triangle(numSegShape + 1, numSegShape + 2, 0);
					buffer.triangle(0, numSegShape + 2, 1);
				}
				else 
				{
					buffer.triangle(numSegShape + 2, numSegShape + 1, 0);
					buffer.triangle(numSegShape + 2, 0, 1);
				}
			}			
		}
	}
	if (!mExtrusionPath->isClosed() && mCapped)
	{
		//begin cap
			buffer.rebaseOffset();
			for (int j =0;j<=numSegShape;j++)
			{
				Ogre::Vector2 vp2 = mShapeToExtrude->getPoint(j);
				Ogre::Vector2 vp2direction = mShapeToExtrude->getAvgDirection(j);
				Ogre::Vector2 vp2normal = mShapeToExtrude->getAvgNormal(j);
				Ogre::Vector3 vp(vp2.x, vp2.y, 0);
				Ogre::Vector3 normal = -Ogre::Vector3::UNIT_Z;				

				Ogre::Vector3 newPoint = mExtrusionPath->getPoint(0)+qBegin*vp;				
				buffer.position(newPoint);				
				buffer.normal(qBegin*normal);
				buffer.textureCoord(vp2.x, vp2.y);
			}
			
			for (int i=0;i<indexBuffer.size()/3;i++)
			{				
				buffer.index(indexBuffer[i*3]);
				buffer.index(indexBuffer[i*3+2]);
				buffer.index(indexBuffer[i*3+1]);
			}
		// end cap
			buffer.rebaseOffset();
			for (int j =0;j<=numSegShape;j++)
			{
				Ogre::Vector2 vp2 = mShapeToExtrude->getPoint(j);
				Ogre::Vector2 vp2direction = mShapeToExtrude->getAvgDirection(j);
				Ogre::Vector2 vp2normal = mShapeToExtrude->getAvgNormal(j);
				Ogre::Vector3 vp(vp2.x, vp2.y, 0);
				Ogre::Vector3 normal = Ogre::Vector3::UNIT_Z;				

				Ogre::Vector3 newPoint = mExtrusionPath->getPoint(numSegPath)+qEnd*vp;				
				buffer.position(newPoint);				
				buffer.normal(qEnd*normal);
				buffer.textureCoord(vp2.x, vp2.y);
			}
			
			for (int i=0;i<indexBuffer.size()/3;i++)
			{				
				buffer.index(indexBuffer[i*3]);
				buffer.index(indexBuffer[i*3+1]);
				buffer.index(indexBuffer[i*3+2]);
			}
	}		
}
}
