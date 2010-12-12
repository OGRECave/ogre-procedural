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
#include "ProceduralTriangulator.h"

using namespace Ogre;

namespace Procedural
{
/*Triangulator::SegmentBuffer Triangulator::delaunayMerge(const SegmentBuffer& leftSegmentBuffer, const SegmentBuffer& rightSegmentBuffer)
{
	// TODO Find the lowest segment beteween left and right that doesn't intersect another segment	
	std::vector<int> pl1 = leftSegmentBuffer.getPointList();
//	std::sort(pl1.begin(), pl1.end(), lowestPointSorter);
	std::vector<int> pl2 = rightSegmentBuffer.getPointList();
	//std::sort(pl2.begin(), pl2.end(), lowestPointSorter);

	Segment LR(leftSegmentBuffer.points);	
	// List of new segments
	SegmentBuffer newSegmentBuffer(leftSegmentBuffer.points);
	const PointList& points = *rightSegmentBuffer.points;
	
	bool finished = false;
	while (!finished)
	{
	// ---Find right candidate
	bool foundRightCandidate = false;
	int rightCandidateIndex = -1;
	{
	AngleSorterRight as(points, LR);
	std::vector<int> rightPointIndices = rightSegmentBuffer.getPointList();
	std::sort(rightPointIndices.begin(), rightPointIndices.end(), as);
	int i=0;
	while (!foundRightCandidate && i<rightPointIndices.size())
	{
		// check angle is less than 180 degrees
		Radian angle = as.getAngle(i);		
		if (angle > (Radian)Math::PI)
		{
			
			continue;
		}
		
		// Check that the next point is not inside the circumcircle
		Circle c = Circle::from3Points(LR.p1(), LR.p2(), points[rightPointIndices[i]]);
		if (!c.isPointInside(points[rightPointIndices[i+1]]))
		{
			foundRightCandidate = true;
			rightCandidateIndex = rightPointIndices[i];
		}
	}
	}

	// ---Find left candidate
	bool foundLeftCandidate = false;
	int leftCandidateIndex = -1;
	{
	AngleSorterLeft as(points, LR);
	std::vector<int> leftPointIndices = leftSegmentBuffer.getPointList();
	std::sort(leftPointIndices.begin(), leftPointIndices.end(), as);
	int i=0;
	while (!foundLeftCandidate && i<leftPointIndices.size())
	{
		// check angle is less than 180 degrees
		Radian angle = as.getAngle(i);		
		if (angle > (Radian)Math::PI)
			continue;
		
		// Check that the next point is not inside the circumcircle
		Circle c = Circle::from3Points(LR.p1(), LR.p2(), points[leftPointIndices[i]]);
		if (!c.isPointInside(points[leftPointIndices[i+1]]))
		{
			foundLeftCandidate = true;
			leftCandidateIndex = leftPointIndices[i];
		}
	}
	}
	
	// ---Set the new LR from the candidate points found
	if (!foundLeftCandidate && !foundRightCandidate)
	{
		finished = true;
	}
	else if (foundLeftCandidate && foundRightCandidate)
	{
		Circle c = Circle::from3Points(points[leftCandidateIndex], LR.p1(), LR.p2());
		if (!c.isPointInside(points[rightCandidateIndex]))
		{
			// Right point must not be inside left point's circumcircle, so it's ok
			LR.i2 = leftCandidateIndex;
			newSegmentBuffer.addSegment(LR);
		}
		else
		{
			LR.i1 = leftCandidateIndex;
			newSegmentBuffer.addSegment(LR);
		}
		
	}
	else if (foundLeftCandidate)
	{
		LR.i1 = leftCandidateIndex;
		newSegmentBuffer.addSegment(LR);
	}
	else if (foundRightCandidate)
	{
		LR.i2 = leftCandidateIndex;
		newSegmentBuffer.addSegment(LR);	
	}
	}

	// --- Finally copy buffers
	newSegmentBuffer.append(leftSegmentBuffer);
	newSegmentBuffer.append(rightSegmentBuffer);
	
	return newSegmentBuffer;
}


bool sortFunction(Vector2 p1, Vector2 p2) 
{ 
	if (p1.x!=p2.x)
		return p1.x<p2.x;
	return p1.y<p2.y;
}

void Triangulator::delaunay(PointList pointList)
{
	assert(pointList.size()>=3);
	// sort by X and Y
	std::sort(pointList.begin(), pointList.end(), sortFunction);
	
	// split into groups of 2 or 3
	SegmentBufferList segmentGroups;
	{
	int i=0;
	while (i<pointList.size())
	{		
		SegmentBuffer segmentBuffer(&pointList);
		int remaining = pointList.size() - i;
		if (remaining >=5 || remaining == 3)
		{	
			int A = i++;
			int B = i++;
			int C = i++;
			segmentBuffer.addSegment(A,B);			
			segmentBuffer.addSegment(B,C);
			segmentBuffer.addSegment(A,C);
		}
		else // (remaining == 2 || remaining == 4)
		{
			int A = i++;
			int B = i++;
			segmentBuffer.addSegment(A,B);			
		}
		segmentGroups.push_back(segmentBuffer);
	}
	}
	
	// Merge segment lists
	while (segmentGroups.size()>1)
	{
		SegmentBufferList nextSegmentGroups;
		
		int i=0;
		
		while (i<segmentGroups.size())
		{
			int remaining = segmentGroups.size() - i;
			if (remaining>=2)
			{
				SegmentBuffer newList = delaunayMerge(segmentGroups[i++], segmentGroups[i++]);
				nextSegmentGroups.push_back(newList);
			}
			else // remaining == 1
			{
				nextSegmentGroups.push_back(segmentGroups[i++]);
			}
		}		
		segmentGroups = nextSegmentGroups;
	}
	
	// TODO Apply constraints
}*/


void Triangle::setVertices(int i0, int i1, int i2)
{
	i[0] = i0;
	i[1] = i1;
	i[2] = i2;
}

void Triangle::setAdj(TriangleBuffer::iterator t0, TriangleBuffer::iterator t1,TriangleBuffer::iterator t2,TriangleBuffer::iterator myIterator)
{
	adj[0] = t0;
	adj[1] = t1;
	adj[2] = t2;
	// build reciprocal
	if (t0 != emptyIterator) t0->adj[t0->findSegNumber(i[1],i[2])] = myIterator;
	if (t1 != emptyIterator) t1->adj[t1->findSegNumber(i[2],i[0])] = myIterator;
	if (t2 != emptyIterator) t2->adj[t2->findSegNumber(i[0],i[1])] = myIterator;
}

void Triangle::detach()
	{
		if (adj[0] != emptyIterator) adj[0]->adj[adj[0]->findSegNumber(i[1],i[2])] = emptyIterator;
		if (adj[1] != emptyIterator) adj[1]->adj[adj[1]->findSegNumber(i[2],i[0])] = emptyIterator;
		if (adj[2] != emptyIterator) adj[2]->adj[adj[2]->findSegNumber(i[0],i[1])] = emptyIterator;
	}

int Triangle::findSegNumber(int i0, int i1) const
	{
		if ((i0==i[0] && i1==i[1])||(i0==i[1] && i1==i[0]))
			return 2;
		if ((i0==i[1] && i1==i[2])||(i0==i[2] && i1==i[1]))
			return 0;
		if ((i0==i[2] && i1==i[0])||(i0==i[0] && i1==i[2]))
			return 1;
	}

bool Triangle::isPointInside(Ogre::Vector2 point)
	{
		// Compute vectors        
		Ogre::Vector2 v0 = p(2) - p(0);
		Ogre::Vector2 v1 = p(1) - p(0);
		Ogre::Vector2 v2 = point - p(0);

		// Compute dot products
		float dot00 = v0.squaredLength();
		float dot01 = v0.dotProduct(v1);
		float dot02 = v0.dotProduct(v2);
		float dot11 = v1.squaredLength();
		float dot12 = v1.dotProduct(v2);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		return (u > 0) && (v > 0) && (u + v < 1);
	}	

struct TouchSuperTriangle
{
	int i0,i1,i2;
	TouchSuperTriangle(int i, int j, int k) : i0(i), i1(j), i2(k) {}
	bool operator()(const Triangle& tri)
	{
		for (int k=0;k<3;k++) if (tri.i[k]==i0 || tri.i[k]==i1 ||tri.i[k]==i2) return true;
		return false;
	}
};


// Triangulation by insertion
TriangleBuffer Triangulator::delaunay2(PointList pointList)
{	
	TriangleBuffer tbuffer;
	// Compute super triangle
	float maxTriangleSize = 0.f;
	for (PointList::iterator it = pointList.begin(); it!=pointList.end();it++)
	{
		maxTriangleSize = std::max(maxTriangleSize,abs(it->x));
		maxTriangleSize = std::max(maxTriangleSize,abs(it->y));
	}
	int maxTriangleIndex=pointList.size();
	pointList.push_back(Ogre::Vector2(-2*maxTriangleSize,-2*maxTriangleSize));
	pointList.push_back(Ogre::Vector2(2*maxTriangleSize,-2*maxTriangleSize));
	pointList.push_back(Ogre::Vector2(0.,2*maxTriangleSize));
	Triangle superTriangle(&pointList, tbuffer.end());
	superTriangle.i[0]= maxTriangleIndex;
	superTriangle.i[1]= maxTriangleIndex+1;
	superTriangle.i[2]= maxTriangleIndex+2;
	tbuffer.push_back(superTriangle);

	// Point insertion loop
	for (int i=0;i<pointList.size()-3;i++)
	{		
		// Insert 1 point, find triangle containing it
		Vector2& p = pointList[i];
		TriangleBuffer::iterator triangle;
		for (TriangleBuffer::iterator it = tbuffer.begin();it!=tbuffer.end();it++)
		{
			if (it->isPointInside(p))
				triangle = it;
		}		
		// Build 3 triangles and suppress old triangle	
		//Triangle* subTri[3];
		TriangleBuffer::iterator subTri[3];
		tbuffer.push_back(Triangle(&pointList, tbuffer.end()));
		subTri[0] = --tbuffer.end();
		tbuffer.push_back(Triangle(&pointList, tbuffer.end()));
		subTri[1] = --tbuffer.end();
		tbuffer.push_back(Triangle(&pointList, tbuffer.end()));
		subTri[2] = --tbuffer.end();
		
		subTri[0]->setVertices(triangle->i[0],triangle->i[1],i);
		subTri[1]->setVertices(triangle->i[1],triangle->i[2],i);
		subTri[2]->setVertices(triangle->i[2],triangle->i[0],i);
		subTri[0]->setAdj(subTri[1],subTri[2],triangle->adj[2], subTri[0]);
		subTri[1]->setAdj(subTri[2],subTri[0],triangle->adj[0], subTri[1]);
		subTri[2]->setAdj(subTri[0],subTri[1],triangle->adj[1], subTri[2]);
		triangle = tbuffer.erase(triangle);
		
		for (int k=0;k<3;k++)
		{
			TriangleBuffer::iterator tri0 = subTri[k];
			//Check if new triangle is Delaunay
			Circle c = Circle::from3Points(tri0->p(0), tri0->p(1), tri0->p(2));		
			bool isDelaunay = true;
			for (int j = 0;j<i+1;j++)
			{
				if (j!=tri0->i[0] && j!=tri0->i[1] && j!=tri0->i[2])
				if (c.isPointInside(pointList[j]))
				{
					isDelaunay = false;
					break;
				}
			}
			// Flip edges where needed
			if (!isDelaunay && tri0->adj[2]!=tbuffer.end())
			{			
				TriangleBuffer::iterator tri1 = tri0->adj[2];
				tbuffer.push_back(Triangle(&pointList, tbuffer.end()));
				TriangleBuffer::iterator newt0 = --tbuffer.end();
				tbuffer.push_back(Triangle(&pointList, tbuffer.end()));				
				TriangleBuffer::iterator newt1 = --tbuffer.end();
				int x = tri1->findSegNumber(tri0->i[0],tri0->i[1]);//opposite of common side for t1
				newt0->setVertices(tri0->i[2],tri1->i[x],tri0->i[1]);
				newt1->setVertices(tri0->i[2],tri0->i[0],tri1->i[x]);
				newt0->setAdj(tri1->adj[(x+2)%3], tri0->adj[0], newt1, newt0);
				newt1->setAdj(tri1->adj[(x+1)%3], newt0,tri0->adj[1],newt1);
				tbuffer.erase(tri0);
				tbuffer.erase(tri1);
			}
		}
	}			
		
	//Remove super triangle
	TouchSuperTriangle touchSuperTriangle(maxTriangleIndex, maxTriangleIndex+1,maxTriangleIndex+2);
	tbuffer.remove_if(touchSuperTriangle);
	pointList.pop_back();
	pointList.pop_back();
	pointList.pop_back();
	return tbuffer;
}

TriangleBuffer Triangulator::triangulate(const Shape& shape)
{
	return delaunay2(shape.getPoints());	
}

}
