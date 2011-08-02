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
#include "ProceduralMultiShape.h"
#include "ProceduralShape.h"

using namespace Ogre;

namespace Procedural
{	
//-----------------------------------------------------------------------
	MeshPtr MultiShape::realizeMesh(const std::string& name, const Ogre::String& group)
	{
		Ogre::SceneManager *smgr = Ogre::Root::getSingleton().getSceneManagerIterator().begin()->second;
		ManualObject * manual = smgr->createManualObject(name);
				
		for (std::vector<Shape>::iterator it = mShapes.begin(); it!=mShapes.end(); it++)
		{
			manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_STRIP);
			it->_appendToManualObject(manual);
			manual->end();
		}		
		
		MeshPtr mesh;
		if (name=="")
			mesh = manual->convertToMesh(Utils::getName(), group);
		else
			mesh = manual->convertToMesh(name, group);
		smgr->destroyManualObject(manual);
		return mesh;
	}	
//-----------------------------------------------------------------------
	std::vector<Vector2> MultiShape::getPoints() const
	{
		std::vector<Vector2> result;		
		for (size_t i = 0;i<mShapes.size(); i++)
		{
			std::vector<Vector2> points = mShapes[i].getPoints();
			result.insert(result.end(), points.begin(), points.end());
		}
		return result;
	}	
//-----------------------------------------------------------------------
	bool MultiShape::isPointInside(const Vector2& point) const
	{
		// Draw a horizontal lines that goes through "point"
		// Using the closest intersection, find whethe the point is actually inside
		int closestSegmentIndex=-1;
		Real closestSegmentDistance = std::numeric_limits<Real>::max();
		Vector2 closestSegmentIntersection(Vector2::ZERO);
		const Shape* closestSegmentShape = 0;
		//bool isOnVertexA = false;
		//bool isOnVertexB = false;
		
		for (size_t k =0;k<mShapes.size();k++)
		{
			const Shape& shape = mShapes[k];
			for (size_t i =0;i<shape.getSegCount();i++)
			{
				Vector2 A = shape.getPoint(i);
				Vector2 B = shape.getPoint(i+1);
				if (A.y!=B.y && (A.y-point.y)*(B.y-point.y)<=0.)
				{
					Vector2 intersect(A.x+(point.y-A.y)*(B.x-A.x)/(B.y-A.y), point.y);			
					float dist = abs(point.x-intersect.x);
					if (dist<closestSegmentDistance)
					{
						closestSegmentIndex = i;
						closestSegmentDistance = dist;
						closestSegmentIntersection = intersect;
						closestSegmentShape = &shape;
					}
				}
			}
		}
		if (closestSegmentIndex!=-1)
		{
			if (closestSegmentIntersection.squaredDistance(closestSegmentShape->getPoint(closestSegmentIndex))<1e-8)
				return (closestSegmentShape->getAvgNormal(closestSegmentIndex).x * (point.x-closestSegmentIntersection.x)<0);
			if (closestSegmentIntersection.squaredDistance(closestSegmentShape->getPoint(closestSegmentIndex+1))<1e-8)
				return (closestSegmentShape->getAvgNormal(closestSegmentIndex+1).x * (point.x-closestSegmentIntersection.x)<0);
			return (closestSegmentShape->getNormalAfter(closestSegmentIndex).x * (point.x-closestSegmentIntersection.x)<0);				
		}
		// the shapes must not contradict each other about outside, so just ask the first shape
		if (mShapes[0].findRealOutSide() == mShapes[0].getOutSide())
			return false;
		else 
			return true;
	}
//-----------------------------------------------------------------------
	bool MultiShape::isClosed() const
	{
		for (std::vector<Shape>::const_iterator it = mShapes.begin(); it!=mShapes.end(); it++)
		{
			if (!it->isClosed())
				return false;
		}
		return true;
	}

}
