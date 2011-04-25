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

		/*if (mFixSharpAngles)
			mExtrusionPath->fixSharpAngles(shapeToExtrude->findBoundingRadius());*/
		Ogre::Real lineicPos=0.;
				
	for (int i = 0; i <= numSegPath;i++)
	{		
		Vector3 v0 = mExtrusionPath->getPoint(i);
		Vector3 direction = mExtrusionPath->getAvgDirection(i);

		Quaternion q = Utils::_computeQuaternion(direction);

		Real scale=1.;

		if (i>0) lineicPos+=(v0-mExtrusionPath->getPoint(i-1)).length();
		if (mRotationTrack)
		{
			Real angle = mRotationTrack->getValue(lineicPos);				
			q= q*Quaternion((Radian)angle, Vector3::UNIT_Z);
		}
		if (mScaleTrack)
			scale = mScaleTrack->getValue(lineicPos);

		for (int j =0;j<=numSegShape;j++)
		{
			Vector2 vp2 = shapeToExtrude->getPoint(j);
			Vector2 vp2direction = shapeToExtrude->getAvgDirection(j);
			Vector2 vp2normal = shapeToExtrude->getAvgNormal(j);
			Vector3 vp(vp2.x, vp2.y, 0);
			Vector3 normal(vp2normal.x, vp2normal.y, 0);							
			buffer.rebaseOffset();
			Vector3 newPoint = v0+q*(scale*vp);

			addPoint(buffer, newPoint,
							 q*normal, 
							 Vector2(i/(Real)numSegPath, j/(Real)numSegShape));

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

			Quaternion qBegin = Utils::_computeQuaternion(mExtrusionPath->getDirectionAfter(0));
			Quaternion qEnd = Utils::_computeQuaternion(mExtrusionPath->getDirectionBefore(mExtrusionPath->getSegCount()));
			
			//begin cap
			buffer.rebaseOffset();
			for (size_t j =0;j<pointList.size();j++)
			{
				Vector2 vp2 = pointList[j];
				Vector3 vp(vp2.x, vp2.y, 0);
				Vector3 normal = -Vector3::UNIT_Z;				

				Vector3 newPoint = mExtrusionPath->getPoint(0)+qBegin*vp;
				addPoint(buffer, newPoint,
								 qBegin*normal,
								 vp2);
			}
			
			for (size_t i=0;i<indexBuffer.size()/3;i++)
			{				
				buffer.index(indexBuffer[i*3]);
				buffer.index(indexBuffer[i*3+2]);
				buffer.index(indexBuffer[i*3+1]);
			}

			// end cap
			buffer.rebaseOffset();
			for (size_t j =0;j<pointList.size();j++)
			{
				Vector2 vp2 = pointList[j];
				Vector3 vp(vp2.x, vp2.y, 0);
				Vector3 normal = Vector3::UNIT_Z;				

				Vector3 newPoint = mExtrusionPath->getPoint(mExtrusionPath->getSegCount())+qEnd*vp;				
				addPoint(buffer, newPoint,
								 qEnd*normal,
								 vp2);
			}
			
			for (size_t i=0;i<indexBuffer.size()/3;i++)
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
