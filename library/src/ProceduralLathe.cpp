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
#include "ProceduralLathe.h"
#include "ProceduralTriangulator.h"

using namespace Ogre;

namespace Procedural
{
//-----------------------------------------------------------------------
void Lathe::_latheBodyImpl(TriangleBuffer& buffer, const Shape* shapeToExtrude) const
{
	int numSegShape = mShapeToExtrude->getSegCount();
	assert(numSegShape>1 && "Shape must contain at least two points");
	int offset =0;

	int numSeg = mClosed?mNumSeg+1:mNumSeg;
	
	buffer.rebaseOffset();
	buffer.estimateIndexCount(numSeg*numSegShape*6);
	buffer.estimateVertexCount((numSegShape+1)*(numSeg+1));
	

	for (int i=0;i<numSeg;i++)
	{
		Real angle = i/(Real)numSeg*Math::TWO_PI;
		Quaternion q;
		q.FromAngleAxis((Radian)angle,Vector3::UNIT_Y);

		for (int j=0;j<=numSegShape;j++)
		{
			const Vector2& v0 = mShapeToExtrude->getPoint(j);
			Vector3 vp(v0.x,v0.y,0);
			const Vector2& vp2direction = mShapeToExtrude->getAvgDirection(j);
			Vector2 vp2normal = vp2direction.perpendicular();
			Vector3 normal(vp2normal.x, vp2normal.y, 0);
			normal.normalise();
			if (mShapeToExtrude->getOutSide() == SIDE_LEFT)
				normal = -normal;

			addPoint(buffer, q*vp,
							 q*normal,
							 Vector2(i/(Real)numSeg, j/(Real)numSegShape));

			if (j <numSegShape && i <numSeg)
			{
				buffer.index(offset + numSegShape + 2);
				buffer.index(offset);
				buffer.index(offset + numSegShape + 1);
				buffer.index(offset + numSegShape + 2);
				buffer.index(offset + 1);
				buffer.index(offset);
			}
			offset ++;
		}
	}
}
//-----------------------------------------------------------------------
void Lathe::_latheCapImpl(TriangleBuffer& buffer) const
{
		std::vector<int> indexBuffer;
		PointList pointList;

		buffer.rebaseOffset();

		Triangulator t;
		if (mShapeToExtrude)
			t.setShapeToTriangulate(mShapeToExtrude);
		else
			t.setMultiShapeToTriangulate(mMultiShapeToExtrude);
		t.triangulate(indexBuffer, pointList);
		buffer.estimateIndexCount(2*indexBuffer.size());
		buffer.estimateVertexCount(2*pointList.size());


		//begin cap
		buffer.rebaseOffset();
		/*Quaternion qBegin = Utils::_computeQuaternion(mExtrusionPath->getDirectionAfter(0));
		if (mRotationTrack)
		{
			Real angle = mRotationTrack->getFirstValue();
			qBegin = qBegin*Quaternion((Radian)angle, Vector3::UNIT_Z);
		}	
		Real scaleBegin=1.;
		if (mScaleTrack)
			scaleBegin = mScaleTrack->getFirstValue();
		for (size_t j =0;j<pointList.size();j++)
		{
			Vector2 vp2 = pointList[j];
			Vector3 vp(vp2.x, vp2.y, 0);
			Vector3 normal = -Vector3::UNIT_Z;				

			Vector3 newPoint = mExtrusionPath->getPoint(0)+qBegin*(scaleBegin*vp);
			addPoint(buffer, newPoint,
				qBegin*normal,
				vp2);
		}

		for (size_t i=0;i<indexBuffer.size()/3;i++)
		{				
			buffer.index(indexBuffer[i*3]);
			buffer.index(indexBuffer[i*3+2]);
			buffer.index(indexBuffer[i*3+1]);
		}*/

		// end cap
		buffer.rebaseOffset();
		/*Quaternion qEnd = Utils::_computeQuaternion(mExtrusionPath->getDirectionBefore(mExtrusionPath->getSegCount()));
		if (mRotationTrack)
		{
			Real angle = mRotationTrack->getLastValue();
			qEnd = qEnd*Quaternion((Radian)angle, Vector3::UNIT_Z);
		}			
		Real scaleEnd=1.;
		if (mScaleTrack)
			scaleEnd = mScaleTrack->getLastValue();

		for (size_t j =0;j<pointList.size();j++)
		{
			Vector2 vp2 = pointList[j];
			Vector3 vp(vp2.x, vp2.y, 0);
			Vector3 normal = Vector3::UNIT_Z;				

			Vector3 newPoint = mExtrusionPath->getPoint(mExtrusionPath->getSegCount())+qEnd*(scaleEnd*vp);
			addPoint(buffer, newPoint,
				qEnd*normal,
				vp2);
		}

		for (size_t i=0;i<indexBuffer.size()/3;i++)
		{				
			buffer.index(indexBuffer[i*3]);
			buffer.index(indexBuffer[i*3+1]);
			buffer.index(indexBuffer[i*3+2]);
		}*/
}
//-----------------------------------------------------------------------
void Lathe::addToTriangleBuffer(TriangleBuffer& buffer) const
{
	assert((mShapeToExtrude || mMultiShapeToExtrude) && "Either shape or multishape must be defined!");
	
	// Triangulate the begin and end caps
	if (!mClosed && mCapped)
		_latheCapImpl(buffer);

	// Extrudes the body
	if (mShapeToExtrude)
		_latheBodyImpl(buffer, mShapeToExtrude);
	else 
	{
		for (int i=0; i<mMultiShapeToExtrude->getShapeCount();i++)			
			_latheBodyImpl(buffer, &mMultiShapeToExtrude->getShape(i));
	}
		
	
}
}
