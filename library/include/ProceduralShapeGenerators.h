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
#ifndef PROCEDURAL_SHAPE_GENERATORS_INCLUDED
#define PROCEDURAL_SHAPE_GENERATORS_INCLUDED

#include "ProceduralShape.h"

namespace Procedural
{
//-----------------------------------------------------------------------
template<class T>
class BaseSpline2
{
protected:
	int numSeg;
	bool closed;
	Side outSide;
public:
	BaseSpline2() : numSeg(4), closed(false), outSide(SIDE_RIGHT) {}
	
	T& setOutSide(Side side)
	{
		outSide = side;
		return (T&)*this;
	}

	Side getOutSide() const
	{
		return outSide;
	}

	T& setNumSeg(int numSeg)
	{
		assert(numSeg>=1);
		this->numSeg = numSeg;
		return (T&)*this;
	}
		
	T& close()
	{
		closed = true;
		return (T&)*this;
	}
};

//-----------------------------------------------------------------------
/**
 * Produces a shape from Cubic Hermite control points
 */
class _ProceduralExport CubicHermiteSpline2 : public BaseSpline2<CubicHermiteSpline2>
{	
	struct ControlPoint
	{
		Ogre::Vector2 position;
		Ogre::Vector2 tangentBefore;
		Ogre::Vector2 tangentAfter;	
		
		ControlPoint(Ogre::Vector2 p, Ogre::Vector2 before, Ogre::Vector2 after) : position(p), tangentBefore(before), tangentAfter(after) {}
	};

	std::vector<ControlPoint> points;	
	
public:
	void addPoint(Ogre::Vector2 p, Ogre::Vector2 before, Ogre::Vector2 after)
	{
		points.push_back(ControlPoint(p, before, after));
	}
	
	const ControlPoint& safeGetPoint(int i) const
	{
		if (closed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	/**
	 * Builds a shape from control points
	 */
	Shape realizeShape();
};

//-----------------------------------------------------------------------
/**
 * Builds a shape from a Catmull-Rom Spline.
 */
class _ProceduralExport CatmullRomSpline2 : public BaseSpline2<CatmullRomSpline2>
{	
	std::vector<Ogre::Vector2> points;
	public:	
	CatmullRomSpline2& addPoint(const Ogre::Vector2& pt)
	{
		points.push_back(pt);
		return *this;
	}

	CatmullRomSpline2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		points.push_back(Ogre::Vector2(x,y));
		return *this;
	}
	
	const Ogre::Vector2& safeGetPoint(int i) const
	{
		if (closed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}
	
	/**
	 * Build a shape from bezier control points
	 */
	Shape realizeShape();
};

//-----------------------------------------------------------------------
/**
 * Builds a shape from a Kochanek Bartels spline.
 *
 * More details here : http://en.wikipedia.org/wiki/Kochanek%E2%80%93Bartels_spline
 */
class _ProceduralExport KochanekBartelsSpline2 : public BaseSpline2<KochanekBartelsSpline2>
{	
	struct ControlPoint
	{
		Ogre::Vector2 position;
		Ogre::Real tension;
		Ogre::Real bias;
		Ogre::Real continuity;
		
		ControlPoint(Ogre::Vector2 p, Ogre::Real t, Ogre::Real b, Ogre::Real c) : position(p), tension(t), bias(b), continuity(c) {}
		ControlPoint(Ogre::Vector2 p) : position(p), tension(0.), bias(0.), continuity(0.) {}
	};

	std::vector<ControlPoint> points;
	
public:
	KochanekBartelsSpline2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		points.push_back(ControlPoint(Ogre::Vector2(x,y)));
		return *this;
	}

	KochanekBartelsSpline2& addPoint(Ogre::Vector2 p)
	{
		points.push_back(ControlPoint(p));
		return *this;
	}

	const ControlPoint& safeGetPoint(int i) const
	{
		if (closed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	/**
	 * Adds a control point to the spline
	 * @arg p Point position
	 * @arg t Tension    +1 = Tight            -1 = Round
	 * @arg b Bias       +1 = Post-shoot       -1 = Pre-shoot
	 * @arg c Continuity +1 = Inverted Corners -1 = Box Corners
	 */
	KochanekBartelsSpline2& addPoint(Ogre::Vector2 p, Ogre::Real t, Ogre::Real b, Ogre::Real c)
	{
		points.push_back(ControlPoint(p,t,b,c));
		return *this;
	}

	/**
	 * Builds a shape from control points
	 */
	Shape realizeShape();
	
};

//-----------------------------------------------------------------------
/**
 * Builds a rectangular shape
 */
class _ProceduralExport RectangleShape
{
	Ogre::Real width,height;

	public:
	RectangleShape() : width(1.0), height(1.0) {}

	RectangleShape& setWidth(Ogre::Real width)
	{
		this->width = width;
		return *this;
	}

	RectangleShape& setHeight(Ogre::Real height)
	{
		this->height = height;
		return *this;
	}

	Shape realizeShape()
	{
		Shape s;
		s.addPoint(-.5f*width,-.5f*height)
		 .addPoint(.5f*width,-.5f*height)
		 .addPoint(.5f*width,.5f*height)
		 .addPoint(-.5f*width,.5f*height)
		 .close();
		return s;
	}
};

//-----------------------------------------------------------------------
/**
 * Builds a circular shape
 */
class _ProceduralExport CircleShape
{
	Ogre::Real radius;
	int numSeg;

	public:
	CircleShape() : radius(1.0), numSeg(8) {}

	CircleShape& setRadius(Ogre::Real radius)
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
		Ogre::Real deltaAngle = Ogre::Math::TWO_PI/(Ogre::Real)numSeg;
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