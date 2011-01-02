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
#ifndef PROCEDURAL_TRIANGULATOR_INCLUDED
#define PROCEDURAL_TRIANGULATOR_INCLUDED

#include "ProceduralTriangleBuffer.h"
#include "ProceduralShape.h"
#include "OgreVector3.h"
#include "ProceduralUtils.h"

namespace Procedural
{

typedef std::vector<Ogre::Vector2> PointList;


class _ProceduralExport Triangulator
{	
	struct Triangle;
	typedef std::list<Triangle> DelaunayTriangleBuffer;

	static void delaunay(PointList& pointList, DelaunayTriangleBuffer& tbuffer);
public:
	
	static TriangleBuffer triangulate(const Shape& shape);


struct Circle
{
	Ogre::Vector2 center;
	float radius;
	static Circle from3Points(Ogre::Vector2 p1, Ogre::Vector2 p2, Ogre::Vector2 p3)
	{
		Circle c;
		Ogre::Vector2 c1 = .5*(p1+p2);
		Ogre::Vector2 d1 = (p2-p1).perpendicular();
		Ogre::Vector2 c2 = c1+d1;

		Ogre::Vector2 c3 = .5*(p2+p3);
		Ogre::Vector2 d3 = (p3-p2).perpendicular();
		Ogre::Vector2 c4 = c3+d3;

		Ogre::Vector2 intersect;
		float denom = (c1.x-c2.x)*(c3.y-c4.y)-(c1.y-c2.y)*(c3.x-c4.x);
		float intersectx = ((c1.x*c2.y-c1.y*c2.x)*(c3.x-c4.x)-(c3.x*c4.y-c4.y*c3.x)*(c1.y-c2.y))/denom;
		float intersecty = ((c1.x*c2.y-c1.y*c2.x)*(c3.y-c4.y)-(c3.x*c4.y-c4.y*c3.x)*(c1.y-c2.y))/denom;

		intersect = Ogre::Vector2(intersectx, intersecty);

		c.center = intersect;
		c.radius = (intersect-c1).length();
		return c;
	}

	bool isPointInside(Ogre::Vector2 p)
	{
		return (p-center).length()<radius;
	}
};

struct Triangle
{
	PointList* pl;
	int i[3];
	DelaunayTriangleBuffer::iterator adj[3];
	DelaunayTriangleBuffer::iterator emptyIterator;
	Triangle(PointList* pl, DelaunayTriangleBuffer::iterator zeroIterator) : emptyIterator(zeroIterator)
	{
		adj[0]=emptyIterator;adj[1]=emptyIterator;adj[2]=emptyIterator;
		this->pl = pl;
	}

	Ogre::Vector2 p(int k) const
	{
		return (*pl)[i[k]];
	}

	bool operator==(const Triangle& other) const
	{
		return i[0]==other.i[0] && i[1]==other.i[1] && i[2]==other.i[2];
	}

	void detach();

	void setVertices(int i0, int i1, int i2);
	void setAdj(DelaunayTriangleBuffer::iterator t0, DelaunayTriangleBuffer::iterator t1,DelaunayTriangleBuffer::iterator t2,DelaunayTriangleBuffer::iterator myIterator);

	int findSegNumber(int i0, int i1) const;

	bool isPointInside(Ogre::Vector2 point);
};
};

}
#endif
