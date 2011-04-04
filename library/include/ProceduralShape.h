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
#include "ProceduralMultiShape.h"

namespace Procedural
{
enum Side {SIDE_LEFT, SIDE_RIGHT};

/**
 * Describes a succession of interconnected 2D points.
 * It can be closed or not, and there's always an outside and an inside
 */
class _ProceduralExport Shape
{
	std::vector<Ogre::Vector2> mPoints;
	bool mClosed;
	Side mOutSide;

public:
	Shape() : mClosed(false), mOutSide(SIDE_RIGHT) {}

	/* --------------------------------------------------------------------------- */
	inline Shape& addPoint(const Ogre::Vector2& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}
	
	/* --------------------------------------------------------------------------- */
	inline Shape& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(Ogre::Vector2(x, y));
		return *this;
	}
	
	/* --------------------------------------------------------------------------- */
	inline Shape& reset()
	{
		mPoints.clear();
		return *this;
	}
	
	/* --------------------------------------------------------------------------- */
	inline std::vector<Ogre::Vector2> getPoints() const
	{
		return mPoints;
	}
	
	/* --------------------------------------------------------------------------- */
	inline const Ogre::Vector2& getPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/* --------------------------------------------------------------------------- */
	inline Shape& close()
	{
		assert(mPoints.size()>0 && "Cannot close an empty shape");
		mClosed = true;
		return *this;
	}

	/**
	 * Sets which side (left or right) is on the outside of the shape.
	 * It is used for such things as normal generation
	 * Default is right, which corresponds to placing points anti-clockwise.
	 */
	inline Shape& setOutSide(Side side)
	{
		mOutSide = side;
		return *this;
	}	
	/* --------------------------------------------------------------------------- */
	inline Side getOutSide() const
	{
		return mOutSide;
	}
	/* --------------------------------------------------------------------------- */
	/// Switches the inside and the outside
	inline Shape& switchSide()
	{
		mOutSide = (mOutSide == SIDE_LEFT)? SIDE_RIGHT: SIDE_LEFT;
		return *this;
	}	
	/* --------------------------------------------------------------------------- */
	inline size_t getSegCount() const
	{
		return (mPoints.size()-1) + (mClosed?1:0);
	}	
	/* --------------------------------------------------------------------------- */
	inline bool isClosed() const
	{
	  return mClosed;
	}	
	/* --------------------------------------------------------------------------- */
	/**
	 * Returns local direction after the current point
	 */
	inline Ogre::Vector2 getDirectionAfter(int i) const
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!mClosed && i == mPoints.size()-1 && i>0)
			return (mPoints[i] - mPoints[i-1]).normalisedCopy();
		else
			return (getPoint(i+1) - getPoint(i)).normalisedCopy();
	}
	/* --------------------------------------------------------------------------- */
	/**
	 * Returns local direction after the current point
	 */
	inline Ogre::Vector2 getDirectionBefore(int i) const
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!mClosed && i == 1)
			return (mPoints[1] - mPoints[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}	
	/* --------------------------------------------------------------------------- */
	inline Ogre::Vector2 getAvgDirection(int i) const
	{
		return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();
	}	
	/* --------------------------------------------------------------------------- */
	inline Ogre::Vector2 getNormalAfter(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getDirectionAfter(i).perpendicular();
		return getDirectionAfter(i).perpendicular();
	}	
	/* --------------------------------------------------------------------------- */
	inline Ogre::Vector2 getNormalBefore(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getDirectionBefore(i).perpendicular();
		return getDirectionBefore(i).perpendicular();
	}	
	/* --------------------------------------------------------------------------- */
	inline Ogre::Vector2 getAvgNormal(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getAvgDirection(i).perpendicular();
		return getAvgDirection(i).perpendicular();
	}
	/* --------------------------------------------------------------------------- */
	/**
	 * Outputs a mesh representing the shape.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name);
	/* --------------------------------------------------------------------------- */
	void _appendToManualObject(Ogre::ManualObject* manual);
	/* --------------------------------------------------------------------------- */
	/**
	 * Tells whether a point is inside a shape or not
	 * @arg point The point to check
	 * @return true if the point is inside this shape, false otherwise
	 */
	bool isPointInside(const Ogre::Vector2& point) const;
	/* --------------------------------------------------------------------------- */
	/**
	 * Computes the intersection between this shape and another one.
	 * Both shapes must be closed.
	 * @arg other The shape against which the intersection is computed
	 * @return The intersection of two shapes, as a new shape
	 */
	MultiShape booleanIntersect(const Shape& other) const;
	/* --------------------------------------------------------------------------- */
	/**
	 * Computes the union between this shape and another one.
	 * Both shapes must be closed.
	 */
	MultiShape booleanUnion(const Shape& other) const;
	/* --------------------------------------------------------------------------- */
	/**
	 * Computes the difference between this shape and another one.
	 * Both shapes must be closed.
	 */
	MultiShape booleanDifference(const Shape& other) const;
	/* --------------------------------------------------------------------------- */
	/**
	 * On a closed shape, find if the outside is located on the right
	 * or on the left. If the outside can easily be determined, 
	 * you'd rather use setOutside(), which doesn't need any computation.
	 */
	Side findRealOutSide() const;

	private:

	enum BooleanOperationType { BOT_UNION, BOT_INTERSECTION, BOT_DIFFERENCE};

	MultiShape _booleanOperation(const Shape& other, BooleanOperationType opType) const;

	struct IntersectionInShape
	{
		int index[2];
		bool onVertex[2];
		Ogre::Vector2 position;
		IntersectionInShape(int i, int j, Ogre::Vector2 intersect) : position(intersect)
		{
			index[0] = i;
			index[1] = j;
			onVertex[0] = false;
			onVertex[1] = false;
		}		
	};

	bool Shape::_isLookingForOutside(BooleanOperationType opType, char shapeSelector) const;
	
	char _isIncreasing(Ogre::Real d, BooleanOperationType opType, char shapeSelector) const;

	//char _isIncreasing(BooleanOperationType opType, char shapeSelector) const;
	
	bool _findWhereToGo(const Shape* inputShapes[], BooleanOperationType opType, IntersectionInShape intersection, Ogre::uint8& shapeSelector, char& isIncreasing, int& currentSegment) const;
	
	void _findAllIntersections(const Shape& other, std::vector<IntersectionInShape>& intersections) const;
};

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
