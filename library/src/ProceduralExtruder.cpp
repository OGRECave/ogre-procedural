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

using namespace Ogre;

namespace Procedural
{
	//-----------------------------------------------------------------------
	Quaternion Extruder::_computeQuaternion(Ogre::Vector3 direction)
	{
		// First, compute an approximate quaternion (everything is ok except Roll angle)
		Quaternion quat = Vector3::UNIT_Z.getRotationTo(direction);
		// Then, compute a correction quaternion : we want the "up" direction to be always the same
		Vector3 projectedY = Vector3::UNIT_Y - Vector3::UNIT_Y.dotProduct(direction) * direction;
		Vector3 tY = quat * Vector3::UNIT_Y;
		Quaternion quat2 = tY.getRotationTo(projectedY);
		Quaternion q = quat2 * quat;
		return q;
	}
	//-----------------------------------------------------------------------
	void Extruder::_extrudeBodyImpl(TriangleBuffer& buffer, const Shape* shapeToExtrude) const
	{
		assert(mExtrusionPath && shapeToExtrude && "Shape and Path must not be null!");
		int numSegPath = mExtrusionPath->getSegCount();
		int numSegShape = shapeToExtrude->getSegCount();
		assert(numSegPath>0 && numSegShape>0 && "Shape and path must contain at least two points");
	
		// Estimate vertex and index count
		buffer.rebaseOffset();
		buffer.estimateIndexCount(numSegShape*numSegPath*6);
		buffer.estimateVertexCount((numSegShape+1)*(numSegPath+1));
				
	for (int i = 0; i <= numSegPath;i++)
	{		
		Vector3 v0 = mExtrusionPath->getPoint(i);
		Vector3 direction = mExtrusionPath->getAvgDirection(i);

		Quaternion q = _computeQuaternion(direction);
				
		/*if (mFixSharpAngles && i>0)
		{
			Plane plane1(lastQ * Vector3::UNIT_Z, lastV0);
			Plane plane2(q * Vector3::UNIT_Z, v0);
			Line inter;
			if (plane1.intersect(plane2, inter))
			{
				Vector3 v = inter.shortestPathToPoint(v0);
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
			Vector2 vp2 = shapeToExtrude->getPoint(j);
			Vector2 vp2direction = shapeToExtrude->getAvgDirection(j);
			Vector2 vp2normal = shapeToExtrude->getAvgNormal(j);
			Vector3 vp(vp2.x, vp2.y, 0);
			Vector3 normal(vp2normal.x, vp2normal.y, 0);							
			buffer.rebaseOffset();
			Vector3 newPoint = v0+q*vp;
			buffer.position(newPoint);
			buffer.normal(q*normal);
			buffer.textureCoord(i/(Real)numSegPath*uTile, j/(Real)numSegShape*vTile);

			if (j <numSegShape && i <numSegPath)
			{		
				if (shapeToExtrude->getOutSide() == SIDE_LEFT)
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
}
	//-----------------------------------------------------------------------
	void Extruder::_extrudeCapImpl(TriangleBuffer& buffer) const
	{
			std::vector<int> indexBuffer;
			PointList pointList;

			buffer.rebaseOffset();

			if (mShapeToExtrude)
			{
				Triangulator::triangulate(*mShapeToExtrude, indexBuffer);
				pointList = mShapeToExtrude->getPoints();
			} else
			{
				Triangulator::triangulate(*mMultiShapeToExtrude, indexBuffer, pointList);
			}
			buffer.estimateIndexCount(2*indexBuffer.size());
			buffer.estimateVertexCount(2*pointList.size());

			Quaternion qBegin = _computeQuaternion(mExtrusionPath->getDirectionAfter(0));
			Quaternion qEnd = _computeQuaternion(mExtrusionPath->getDirectionBefore(mExtrusionPath->getSegCount()));
			
			//begin cap
			buffer.rebaseOffset();
			for (int j =0;j<pointList.size();j++)
			{
				Vector2 vp2 = pointList[j];
				Vector3 vp(vp2.x, vp2.y, 0);
				Vector3 normal = -Vector3::UNIT_Z;				

				Vector3 newPoint = mExtrusionPath->getPoint(0)+qBegin*vp;				
				buffer.position(newPoint);				
				buffer.normal(qBegin*normal);
				buffer.textureCoord(vp2.x, vp2.y);
			}
			
			for (unsigned short i=0;i<indexBuffer.size()/3;i++)
			{				
				buffer.index(indexBuffer[i*3]);
				buffer.index(indexBuffer[i*3+2]);
				buffer.index(indexBuffer[i*3+1]);
			}
		// end cap
			buffer.rebaseOffset();
			for (unsigned short j =0;j<pointList.size();j++)
			{
				Vector2 vp2 = pointList[j];
				Vector3 vp(vp2.x, vp2.y, 0);
				Vector3 normal = Vector3::UNIT_Z;				

				Vector3 newPoint = mExtrusionPath->getPoint(mExtrusionPath->getSegCount())+qEnd*vp;				
				buffer.position(newPoint);				
				buffer.normal(qEnd*normal);
				buffer.textureCoord(vp2.x, vp2.y);
			}
			
			for (unsigned short i=0;i<indexBuffer.size()/3;i++)
			{				
				buffer.index(indexBuffer[i*3]);
				buffer.index(indexBuffer[i*3+1]);
				buffer.index(indexBuffer[i*3+2]);
			}
	
	}
	//-----------------------------------------------------------------------
	void Extruder::addToTriangleBuffer(TriangleBuffer& buffer) const
	{
		assert((mShapeToExtrude || mMultiShapeToExtrude) && "Either shape or multishape must be defined!");

		// Triangulate the begin and end caps
	
		if (!mExtrusionPath->isClosed() && mCapped)
			_extrudeCapImpl(buffer);

		if (mShapeToExtrude)
			_extrudeBodyImpl(buffer, mShapeToExtrude);
		else 
		{
			for (int i=0; i<mMultiShapeToExtrude->getShapeCount();i++)			
				_extrudeBodyImpl(buffer, &mMultiShapeToExtrude->getShape(i));
		}
	}
}
