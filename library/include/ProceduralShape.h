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
	bool isClosed;

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

	int getSegCount()
	{
		return (points.size()-1) + (isClosed?1:0);
	}


	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector2 getDirectionAfter(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!isClosed && i == points.size()-1 && i>0)
			return (points[i] - points[i-1]).normalisedCopy();
		else
			return (getPoint(i+1) - getPoint(i)).normalisedCopy();
	}

	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector2 getDirectionBefore(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!isClosed && i == 1)
			return (points[1] - points[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}

	Ogre::Vector2 getAvgDirection(int i)
	{
	    return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();

	}

};

/**
 * Enables to build a shape from Bezier control points.
 * Tangents are automatically calculated from control points, so the curve will "touch" every point you define
 */
class _ProceduralExport BezierShape
{
	std::vector<Ogre::Vector2> points;

	int numSeg;
	bool isClosed;
public:
	BezierShape() : numSeg(4), isClosed(false) {}

	BezierShape& addPoint(const Ogre::Vector2& pt)
	{
		points.push_back(pt);
		return *this;
	}

	BezierShape& addPoint(float x, float y)
	{
	    points.push_back(Ogre::Vector2(x,y));
	    return *this;
	}

	BezierShape& reset()
	{
		points.clear();
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
		return points[i];
	}

	const Ogre::Vector2& safeGetPoint(int i)
	{
		if (isClosed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	/**
	 * Build a shape from bezier shape
	 */
	Shape realizeShape()
	{
		Shape shape;

		for (int i=0;i<points.size();i++)
		{
			const Ogre::Vector2& P0 = points[i];
			const Ogre::Vector2& P3 = safeGetPoint(i+1);

			Ogre::Vector2 P1 = P0 + 0.5 * (safeGetPoint(i+1)-safeGetPoint(i-1));
			Ogre::Vector2 P2 = P3 - 0.5 * (safeGetPoint(i+2)-P0);

			for (int j=0;j<numSeg;j++)
			{
				float t = (float)j/(float)numSeg;
				Ogre::Vector2 P = pow(1-t,3)*P0 + 3*pow(1-t,2)*t*P1 + 3*(1-t)*pow(t,2)*P2 + pow(t,3)*P3;
				shape.addPoint(P);
			}
			if (i==points.size()-1 && !isClosed)
			{
				shape.addPoint(P3);
			}
		}
		if (isClosed)
			shape.close();

		return shape;
	}
};

class _ProceduralExport RectangleShape
{
	float width,height;
	
	public:
	RectangleShape() : width(1.0), height(1.0) {}
	
	RectangleShape& setWidth(float width)
	{
		this->width = width;
		return *this;
	}
	
	RectangleShape& setHeight(float height)
	{
		this->height = height;
		return *this;
	}
	
	Shape realizeShape()
	{
		Shape s;
		s.addPoint(-.5*width,-.5*height)
		 .addPoint(.5*width,-.5*height)
		 .addPoint(.5*width,.5*height)
		 .addPoint(-.5*width,.5*height)
		 .close();		
		return shape;
	}
};

class _ProceduralExport CircleShape
{
	float radius;
	int numSeg;
	
	public:
	CircleShape() : radius(1.0), numSeg(8) {}
	
	CircleShape& setRadius(float radius)
	{
		this->radius = radius;
		return *this;
	}
	
	CircleShape& setNumSeg(int numSeg)
	{
		this->numSeg = numSeg;
		return *this;
	}
	
	Shape realizeShape()
	{
		Shape s;
		float deltaAngle = Ogre::Math::TWO_PI/(float)numSeg;
		for (int i=0;i<numSeg;i++)
		{
			s.addPoint(radius*cosf(i*deltaAngle), radius*sinf(i*deltaAngle));
		}
		s.close();
		return s;
	}
};
}

#endif
