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
#include "OgreMatrix4.h"
#include "ProceduralUtils.h"

namespace Procedural
{

typedef std::vector<Ogre::Vector2> PointList;

/**
 * WIP
 * Implements a Delaunay Triangulation algorithm.
 * It works on Shapes to build Triangle Buffers
 */
class _ProceduralExport Triangulator
{	
	struct Triangle;
	struct DelaunaySegment;
	typedef std::list<Triangle> DelaunayTriangleBuffer;
		
	static void delaunay(PointList& pointList, DelaunayTriangleBuffer& tbuffer);
	static void addConstraints(const Shape& shape, DelaunayTriangleBuffer& tbuffer);
	static void triangulatePolygon(const std::vector<int>& input, const DelaunaySegment& seg, DelaunayTriangleBuffer& tbuffer, const PointList& pointList);	
	
	
//-----------------------------------------------------------------------
struct DelaunaySegment
{
	int i1, i2;
	DelaunaySegment(int _i1, int _i2) : i1(_i1), i2(_i2) {}
};	

	//-----------------------------------------------------------------------
struct Triangle
{
	const PointList* pl;
	int i[3];
	DelaunayTriangleBuffer::iterator adj[3];
	DelaunayTriangleBuffer::iterator emptyIterator;
	Triangle(const PointList* pl, DelaunayTriangleBuffer::iterator zeroIterator) : emptyIterator(zeroIterator)
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
	
	bool containsSegment(int i0, int i1) const
	{		
		return ((i0==i[0] || i0==i[1] || i0==i[2])&&(i1==i[0] || i1==i[1] || i1==i[2]));
	}
};
//-----------------------------------------------------------------------
struct TouchSuperTriangle
{
	int i0,i1,i2;
	TouchSuperTriangle(int i, int j, int k) : i0(i), i1(j), i2(k) {}
	bool operator()(const Triangulator::Triangle& tri)
	{
		for (int k=0;k<3;k++) if (tri.i[k]==i0 || tri.i[k]==i1 ||tri.i[k]==i2) return true;
		return false;
	}
};

public:	
	/**
	 * WIP
	 * Executes the Constrained Delaunay Triangulation algorithm
	 * @arg shape The input Shape to triangulate
	 * @arg ouput A vector of index where is outputed the resulting triangle indexes
	 */
	static void triangulate(const Shape& shape, std::vector<int>& output);

	static void triangulateToMesh(const Shape& shape, std::string out)
	{
		TriangleBuffer buffer;
		std::vector<int> indexBuffer;		
		triangulate(shape,indexBuffer);
		for (int j =0;j<=shape.getSegCount();j++)
			{
				Ogre::Vector2 vp2 = shape.getPoint(j);
				Ogre::Vector3 vp(vp2.x, vp2.y, 0);
				Ogre::Vector3 normal = -Ogre::Vector3::UNIT_Z;				

				Ogre::Vector3 newPoint = vp;				
				buffer.position(newPoint);				
				buffer.normal(normal);
				buffer.textureCoord(vp2.x, vp2.y);
			}
			
			for (int i=0;i<indexBuffer.size()/3;i++)
			{				
				buffer.index(indexBuffer[i*3]);
				buffer.index(indexBuffer[i*3+2]);
				buffer.index(indexBuffer[i*3+1]);
			}
		buffer.transformToMesh(out);
	}
};

}
#endif
