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
#include "OgreMesh.h"
#include "OgreManualObject.h"
#include "ProceduralRoot.h"

namespace Procedural
{
enum Side {SIDE_LEFT, SIDE_RIGHT};

/**
 * Describes a succession of interconnected 2D points.
 * It can be closed or not, and there's always an outside and an inside
 */
class _ProceduralExport Shape
{
	std::vector<Ogre::Vector2> points;
	bool closed;
	Side outSide;

public:
	Shape() : closed(false), outSide(SIDE_RIGHT) {}

	inline Shape& addPoint(const Ogre::Vector2& pt)
	{
		points.push_back(pt);
		return *this;
	}

	inline Shape& addPoint(Ogre::Real x, Ogre::Real y)
	{
		points.push_back(Ogre::Vector2(x, y));
		return *this;
	}

	inline Shape& reset()
	{
		points.clear();
		return *this;
	}

	inline std::vector<Ogre::Vector2> getPoints() const
	{
		return points;
	}

	inline const Ogre::Vector2& getPoint(int i) const
	{
		if (closed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	inline Shape& close()
	{
		assert(points.size()>0 && "Cannot close an empty shape");
		closed = true;
		return *this;
	}

	/**
	 * Sets which side (left or right) is on the outside of the shape.
	 * It is used for such things as normal generation
	 * Default is right, which corresponds to placing points anti-clockwise.
	 */
	inline Shape& setOutSide(Side side)
	{
		outSide = side;
		return *this;
	}

	inline Side getOutSide() const
	{
		return outSide;
	}

	inline int getSegCount() const
	{
		return (points.size()-1) + (closed?1:0);
	}

	inline bool isClosed() const
	{
	  return closed;
	}	

	/**
	 * Returns local direction after the current point
	 */
	inline Ogre::Vector2 getDirectionAfter(int i) const
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!closed && i == points.size()-1 && i>0)
			return (points[i] - points[i-1]).normalisedCopy();
		else
			return (getPoint(i+1) - getPoint(i)).normalisedCopy();
	}

	/**
	 * Returns local direction after the current point
	 */
	inline Ogre::Vector2 getDirectionBefore(int i) const
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!closed && i == 1)
			return (points[1] - points[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}

	inline Ogre::Vector2 getAvgDirection(int i) const
	{
		return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();

	}

	inline Ogre::Vector2 getNormalAfter(int i) const
	{
		if (outSide==SIDE_RIGHT)
		return -getDirectionAfter(i).perpendicular();
		return getDirectionAfter(i).perpendicular();
	}

	inline Ogre::Vector2 getNormalBefore(int i) const
	{
		if (outSide==SIDE_RIGHT)
		return -getDirectionBefore(i).perpendicular();
		return getDirectionBefore(i).perpendicular();
	}

	inline Ogre::Vector2 getAvgNormal(int i) const
	{
		if (outSide==SIDE_RIGHT)
		return -getAvgDirection(i).perpendicular();
		return getAvgDirection(i).perpendicular();
	}

	/**
	 * Outputs a mesh representing the shape.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name);

	/**
	 * Tells whether a point is inside a shape or not
	 * @arg point The point to check
	 * @return true if the point is inside this shape, false otherwise
	 */
	bool isPointInside(const Ogre::Vector2& point) const;
	
	/**
	 * Computes the intersection between this shape and another one.
	 * Both shapes must be closed.
	 * @arg other The shape against which the intersection is computed
	 * @return The intersection of two shapes, as a new shape
	 */
	Shape booleanIntersect(const Shape& other) const;
	
	/**
	 * WIP
	 * Computes the union between this shape and another one.
	 * Both shapes must be closed.
	 */
	Shape booleanUnion(const Shape& other) const;
	
	/**
	 * WIP
	 * Computes the difference between this shape and another one.
	 * Both shapes must be closed.
	 */
	Shape booleanDifference(const Shape& other) const;

	private:

	struct IntersectionInShape
	{
		int index[2];
		Ogre::Vector2 position;
		IntersectionInShape(int i, int j, Ogre::Vector2 intersect) : position(intersect)
		{
			index[0] = i;
			index[1] = j;
		}
	};

	void _findAllIntersections(const Shape& other, std::vector<IntersectionInShape>& intersections) const;
};

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
	Shape realizeShape()
	{
	Shape shape;

		int numPoints = closed?points.size():points.size()-1;		
		for (int i=0;i<numPoints;i++)
		{
			const Ogre::Vector2& p0 = points[i].position;
			const Ogre::Vector2& m0 = points[i].tangentAfter;
			const Ogre::Vector2& p1 = safeGetPoint(i+1).position;
			const Ogre::Vector2& m1 = safeGetPoint(i+1).tangentBefore;

			for (int j=0;j<numSeg;j++)
			{
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t2*t;
				Ogre::Vector2 P = (2*t3-3*t2+1)*p0+(t3-2*t2+t)*m0+(-2*t3+3*t2)*p1+(t3-t2)*m1;
				shape.addPoint(P);
			}
			if (i==points.size()-2 && !closed)
			{
				shape.addPoint(p1);
			}
		}
		if (closed)
			shape.close();
		shape.setOutSide(outSide);

		return shape;
	}
};

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
	Shape realizeShape()
	{
		Shape shape;

		int numPoints = closed?points.size():points.size()-1;		
		for (int i=0;i<numPoints;i++)
		{			
			const Ogre::Vector2& P1 = safeGetPoint(i-1);
			const Ogre::Vector2& P2 = safeGetPoint(i);
			const Ogre::Vector2& P3 = safeGetPoint(i+1);
			const Ogre::Vector2& P4 = safeGetPoint(i+2);

			for (int j=0;j<numSeg;j++)
			{				
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t*t2;
				Ogre::Vector2 P = 0.5*((-t3+2*t2-t)*P1 + (3*t3-5*t2+2.)*P2 + (-3*t3+4*t2+t)*P3 + (t3-t2)*P4);
				shape.addPoint(P);
			}
			if (i==points.size()-2 && !closed)
			{
				shape.addPoint(P3);
			}

		}
		if (closed)
			shape.close();
		shape.setOutSide(outSide);

		return shape;
	}
};

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
	Shape realizeShape()
	{
		Shape shape;
		
		int numPoints = closed?points.size():points.size()-1;		
		for (int i=0;i<numPoints;i++)
		{
			const ControlPoint& pm1 = safeGetPoint(i-1);
			const ControlPoint& p0 = safeGetPoint(i);
			const ControlPoint& p1 = safeGetPoint(i+1);
			const ControlPoint& p2 = safeGetPoint(i+2);
			
			Ogre::Vector2 m0 = (1-p0.tension)*(1+p0.bias)*(1+p0.continuity)/2.*(p0.position-pm1.position)+(1-p0.tension)*(1-p0.bias)*(1-p0.continuity)/2.*(p1.position-p0.position);
			Ogre::Vector2 m1 = (1-p1.tension)*(1+p1.bias)*(1-p1.continuity)/2.*(p1.position-p0.position)+(1-p1.tension)*(1-p1.bias)*(1+p1.continuity)/2.*(p2.position-p1.position);
			
			for (int j=0;j<numSeg;j++)
			{
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t2*t;
				Ogre::Vector2 P = (2*t3-3*t2+1)*p0.position+(t3-2*t2+t)*m0+(-2*t3+3*t2)*p1.position+(t3-t2)*m1;
				shape.addPoint(P);
			}
			if (i==points.size()-2 && !closed)
			{
				shape.addPoint(p1.position);
			}
		}
		return shape;
	}
};

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
		s.addPoint(-.5*width,-.5*height)
		 .addPoint(.5*width,-.5*height)
		 .addPoint(.5*width,.5*height)
		 .addPoint(-.5*width,.5*height)
		 .close();
		return s;
	}
};

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
