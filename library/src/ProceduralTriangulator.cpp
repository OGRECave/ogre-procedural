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
void Triangle::setVertices(int i0, int i1, int i2)
{
	i[0] = i0;
	i[1] = i1;
	i[2] = i2;
}

void Triangle::setAdj(DelaunayTriangleBuffer::iterator t0, DelaunayTriangleBuffer::iterator t1,DelaunayTriangleBuffer::iterator t2,DelaunayTriangleBuffer::iterator myIterator)
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
DelaunayTriangleBuffer Triangulator::delaunay2(PointList pointList)
{
	DelaunayTriangleBuffer tbuffer;
	// Compute super triangle
	float maxTriangleSize = 0.f;
	for (PointList::iterator it = pointList.begin(); it!=pointList.end();it++)
	{
		maxTriangleSize = std::max<float>(maxTriangleSize,fabs(it->x));
		maxTriangleSize = std::max<float>(maxTriangleSize,fabs(it->y));
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
		DelaunayTriangleBuffer::iterator triangle;
		for (DelaunayTriangleBuffer::iterator it = tbuffer.begin();it!=tbuffer.end();it++)
		{
			if (it->isPointInside(p))
				triangle = it;
		}
		// Build 3 triangles and suppress old triangle
		//Triangle* subTri[3];
		DelaunayTriangleBuffer::iterator subTri[3];
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
			DelaunayTriangleBuffer::iterator tri0 = subTri[k];
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
				DelaunayTriangleBuffer::iterator tri1 = tri0->adj[2];
				tbuffer.push_back(Triangle(&pointList, tbuffer.end()));
				DelaunayTriangleBuffer::iterator newt0 = --tbuffer.end();
				tbuffer.push_back(Triangle(&pointList, tbuffer.end()));
				DelaunayTriangleBuffer::iterator newt1 = --tbuffer.end();
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
    DelaunayTriangleBuffer dtb = delaunay2(shape.getPoints());
    TriangleBuffer tb;
    PointList& pl = *(dtb.begin()->pl);
    for (PointList::iterator it = pl.begin(); it!=pl.end();it++)
    {
        tb.position(Ogre::Vector3(it->x,it->y,0.));
        tb.normal(Ogre::Vector3::UNIT_Z);
        tb.textureCoord(Ogre::Vector2(it->x,it->y));
    }
    for (DelaunayTriangleBuffer::iterator it = dtb.begin(); it!=dtb.end();it++)
    {
        tb.index(it->i[0]);
        tb.index(it->i[1]);
        tb.index(it->i[2]);
    }
    return tb;
}

}
