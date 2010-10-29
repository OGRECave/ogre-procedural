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
#ifndef PROCEDURAL_SHAPE_INCLUDED
#define PROCEDURAL_SHAPE_INCLUDED
#include "OgreVector2.h"
#include "ProceduralPlatform.h"
#include "ProceduralUtils.h"

namespace Procedural
{
class _ProceduralExport Shape
{
	std::vector<Ogre::Vector2> points;
	boolean isClosed;

public:
	Shape() : isClosed(false) {}

	Shape& addPoint(const Ogre::Vector2& pt)
	{
		points.push_back(pt);
		return *this;
	}

	Shape& addPoint(float x, float y)
	{
		points.push_back(Ogre::Vector2(x, y));
		return *this;
	}
	
	Shape& reset()
	{
		points.clear();		
		return *this;
	}

	std::vector<Ogre::Vector2> getPoints()
	{
		return points;
	}
	
	const Ogre::Vector2& getPoint(int i)
	{
		return points[i];
	}
	
	const Ogre::Vector2& safeGetPoint(int i)
	{
		if (isClosed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	Shape& close()
	{
		assert(points.size()>0 && "Cannot close an empty shape");
		isClosed = true;
		return *this;
	}
};

class _ProceduralExport BezierShape
{
	std::vector<Ogre::Vector2> bezierPoints;
	
	int numSeg;
	boolean isClosed;
public:
	BezierShape() : numSeg(4), isClosed(false) {}

	BezierShape& addPoint(const Ogre::Vector2& pt)
	{
		bezierPoints.push_back(pt);
		return *this;
	}

	BezierShape& reset()
	{
		bezierPoints.clear();
		return *this;
	}
	
	BezierShape& close()
	{
		isClosed = true;		
		return *this;
	}
	
	BezierShape& setNumSeg(int numSeg)
	{
		assert(numSeg>=1);
		this->numSeg = numSeg;
		return *this;
	}
	
	const Ogre::Vector2& getPoint(int i)
	{
		return bezierPoints[i];
	}
	
	const Ogre::Vector2& safeGetPoint(int i)
	{
		if (isClosed)
			return bezierPoints[Utils::modulo(i,bezierPoints.size())];
		return bezierPoints[Utils::cap(i,0,bezierPoints.size()-1)];
	}
	
	Shape realizeShape()
	{
		Shape shape;
		
		for (int i=0;i<bezierPoints.size();i++)
		{
			const Ogre::Vector2& P0 = bezierPoints[i];
			const Ogre::Vector2& P3 = safeGetPoint(i+1);			
			
			Ogre::Vector2 P1 = 2 * P0 - safeGetPoint(i-1);
			Ogre::Vector2 P2 = 2 * P3 - safeGetPoint(i+1);
			
			for (int j=0;j<=numSeg;j++)
			{
				float t = (float)j/(float)numSeg;
				Ogre::Vector2 P = pow(1-t,3)*P0 + 3*sqr(1-t)*t*P1 + 3*(1-t)*sqr(t)*P2 + pow(t,3)*P3;
				shape.addPoint(P);
			}
		}
		
		return shape;
	}
};
}

#endif