/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010-2012 Michael Broutin

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
#include "ProceduralSplines.h"
#include "ProceduralShapeGeneratorsBase.h"

namespace Procedural
{
//-----------------------------------------------------------------------
/**
 * Produces a shape from Cubic Hermite control points
 * \image html spline_cubichermite.png
 */
class _ProceduralExport CubicHermiteSpline2 : public BaseSpline2<CubicHermiteSpline2>
{	
public:
	typedef CubicHermiteSplineControlPoint<Ogre::Vector2> ControlPoint;
private:
	std::vector<ControlPoint> mPoints;	
public:
	/// Adds a control point
	inline CubicHermiteSpline2& addPoint(const Ogre::Vector2& p, const Ogre::Vector2& before, const Ogre::Vector2& after)
	{
		mPoints.push_back(ControlPoint(p, before, after));
		return *this;
	}
	/// Adds a control point
	inline CubicHermiteSpline2& addPoint(const Ogre::Vector2& p, const Ogre::Vector2& tangent)
	{
		mPoints.push_back(ControlPoint(p, tangent, tangent));
		return *this;
	}
	/// Adds a control point
	inline CubicHermiteSpline2& addPoint(const Ogre::Vector2& p, CubicHermiteSplineAutoTangentMode autoTangentMode = AT_CATMULL)
	{
		ControlPoint cp;
		cp.position = p;
		cp.autoTangentBefore = autoTangentMode;
		cp.autoTangentAfter = autoTangentMode;
		mPoints.push_back(cp);
		return *this;
	}

		/// Adds a control point
	inline CubicHermiteSpline2& addPoint(Ogre::Real x, Ogre::Real y, CubicHermiteSplineAutoTangentMode autoTangentMode = AT_CATMULL)
	{
		ControlPoint cp;
		cp.position = Ogre::Vector2(x,y);
		cp.autoTangentBefore = autoTangentMode;
		cp.autoTangentAfter = autoTangentMode;
		mPoints.push_back(cp);
		return *this;
	}

	/// Safely gets a control point
	inline const ControlPoint& safeGetPoint(unsigned int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/**
	 * Builds a shape from control points
	 */
	Shape realizeShape();
};

//-----------------------------------------------------------------------
/**
 * Builds a shape from a Catmull-Rom Spline.
 * A catmull-rom smoothly interpolates position between control points
 * \image html spline_catmull.png
 */
class _ProceduralExport CatmullRomSpline2 : public BaseSpline2<CatmullRomSpline2>
{	
	std::vector<Ogre::Vector2> mPoints;
	public:	
	/// Adds a control point
	inline CatmullRomSpline2& addPoint(const Ogre::Vector2& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	/// Adds a control point
	inline CatmullRomSpline2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(Ogre::Vector2(x,y));
		return *this;
	}
	
	/// Safely gets a control point
	inline const Ogre::Vector2& safeGetPoint(unsigned int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/**
	 * Build a shape from bezier control points
	 */
	Shape realizeShape();
};

//-----------------------------------------------------------------------
/**
 * Builds a shape from a Kochanek Bartels spline.
 * \image html spline_kochanekbartels.png
 * More details here : http://en.wikipedia.org/wiki/Kochanek%E2%80%93Bartels_spline
 */
class _ProceduralExport KochanekBartelsSpline2 : public BaseSpline2<KochanekBartelsSpline2>
{	
public:
	typedef KochanekBartelsSplineControlPoint<Ogre::Vector2> ControlPoint;
private:
	std::vector<ControlPoint> mPoints;
	
public:
	/// Adds a control point
	inline KochanekBartelsSpline2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(ControlPoint(Ogre::Vector2(x,y)));
		return *this;
	}

	/// Adds a control point
	inline KochanekBartelsSpline2& addPoint(Ogre::Vector2 p)
	{
		mPoints.push_back(ControlPoint(p));
		return *this;
	}

	/// Safely gets a control point
	inline const ControlPoint& safeGetPoint(unsigned int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/**
	 * Adds a control point to the spline
	 * @param p Point position
	 * @param t Tension    +1 = Tight            -1 = Round
	 * @param b Bias       +1 = Post-shoot       -1 = Pre-shoot
	 * @param c Continuity +1 = Inverted Corners -1 = Box Corners
	 */
	inline KochanekBartelsSpline2& addPoint(Ogre::Vector2 p, Ogre::Real t, Ogre::Real b, Ogre::Real c)
	{
		mPoints.push_back(ControlPoint(p,t,b,c));
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
	Ogre::Real mWidth,mHeight;

	public:
	/// Default constructor
	RectangleShape() : mWidth(1.0), mHeight(1.0) {}

	/// Sets width
	inline RectangleShape& setWidth(Ogre::Real width)
	{
		mWidth = width;
		return *this;
	}

	/// Sets height
	inline RectangleShape& setHeight(Ogre::Real height)
	{
		mHeight = height;
		return *this;
	}

	/// Builds the shape
	Shape realizeShape()
	{
		Shape s;
		s.addPoint(-.5f*mWidth,-.5f*mHeight)
		 .addPoint(.5f*mWidth,-.5f*mHeight)
		 .addPoint(.5f*mWidth,.5f*mHeight)
		 .addPoint(-.5f*mWidth,.5f*mHeight)
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
	Ogre::Real mRadius;
	unsigned int mNumSeg;

	public:
	/// Default constructor
	CircleShape() : mRadius(1.0), mNumSeg(8) {}

	/// Sets radius
	inline CircleShape& setRadius(Ogre::Real radius)
	{
		mRadius = radius;
		return *this;
	}

	/// Sets number of segments
	inline CircleShape& setNumSeg(unsigned int numSeg)
	{
		mNumSeg = numSeg;
		return *this;
	}

	/// Builds the shape
	Shape realizeShape()
	{
		Shape s;
		Ogre::Real deltaAngle = Ogre::Math::TWO_PI/(Ogre::Real)mNumSeg;
		for (unsigned int i = 0; i < mNumSeg; ++i)
		{
			s.addPoint(mRadius*cosf(i*deltaAngle), mRadius*sinf(i*deltaAngle));
		}
		s.close();
		return s;
	}
};

//-----------------------------------------------------------------------
/**
 * Builds a ellipse shape
 */
class _ProceduralExport EllipseShape
{
	Ogre::Real mRadiusX, mRadiusY;
	unsigned int mNumSeg;

	public:
	/// Default constructor
	EllipseShape() : mRadiusX(1.0), mRadiusY(1.0), mNumSeg(8) {}

	/// Sets radius in x direction
	inline EllipseShape& setRadiusX(Ogre::Real radius)
	{
		mRadiusX = radius;
		return *this;
	}

	/// Sets radius in y direction
	inline EllipseShape& setRadiusY(Ogre::Real radius)
	{
		mRadiusY = radius;
		return *this;
	}

	/// Sets number of segments
	inline EllipseShape& setNumSeg(unsigned int numSeg)
	{
		mNumSeg = numSeg;
		return *this;
	}

	/// Builds the shape
	inline Shape realizeShape()
	{
		Shape s;
		Ogre::Real deltaAngle = Ogre::Math::TWO_PI/(Ogre::Real)mNumSeg;
		for (unsigned int i = 0; i < mNumSeg; ++i)
		{
			s.addPoint(mRadiusX*cosf(i*deltaAngle), mRadiusY*sinf(i*deltaAngle));
		}
		s.close();
		return s;
	}
};

//-----------------------------------------------------------------------
/**
 * Builds a triangele shape
 * \image html shape_triangle.png
 */
class _ProceduralExport TriangleShape
{
	Ogre::Real mLengthA, mLengthB, mLengthC;

	public:
	/// Default constructor
	TriangleShape() : mLengthA(1.0), mLengthB(1.0), mLengthC(1.0) {}

	/// Sets length of edge A
	inline TriangleShape& setLengthA(Ogre::Real length)
	{
		mLengthA = length;
		return *this;
	}

	/// Sets length of edge B
	inline TriangleShape& setLengthB(Ogre::Real length)
	{
		mLengthB = length;
		return *this;
	}

	/// Sets length of edge C
	inline TriangleShape& setLengthC(Ogre::Real length)
	{
		mLengthC = length;
		return *this;
	}

	/// Builds the shape
	inline Shape realizeShape()
	{
		assert(mLengthA>0 && mLengthB>0 && mLengthC>0 && "Length of triangle edge must be longer than 0!");
		assert(mLengthA< (mLengthB+mLengthC) && "Length of triangle edge A must be shorter or equal than B+C!");
		Ogre::Radian alpha = Ogre::Math::ACos((mLengthB * mLengthB + mLengthC * mLengthC - mLengthA * mLengthA) / (2 * mLengthB * mLengthC));

		Shape s;
		s.addPoint(0.0f, 0.0f);
		s.addPoint(Ogre::Math::Cos(alpha) * mLengthB, Ogre::Math::Sin(alpha) * mLengthB);
		s.addPoint(mLengthC, 0.0f);
		s.close();
		s.translate((Ogre::Math::Cos(alpha) * mLengthB + mLengthC) / -3.0f, mLengthB / -3.0f);
		
		return s;
	}
};

//-----------------------------------------------------------------------
/**
 * Produces a shape from Cubic Hermite control points
 * \image html spline_roundedcorner.png
 */
class _ProceduralExport RoundedCornerSpline2 : public BaseSpline2<RoundedCornerSpline2>
{		
	Ogre::Real mRadius;

	std::vector<Ogre::Vector2> mPoints;	
	
public:
	RoundedCornerSpline2() : mRadius(.1f) {}
	
	/// Sets the radius of the corners
	inline RoundedCornerSpline2& setRadius(Ogre::Real radius)
	{
		mRadius = radius;
		return *this;
	}

	/// Adds a control point
	inline RoundedCornerSpline2& addPoint(const Ogre::Vector2& p)
	{
		mPoints.push_back(p);
		return *this;
	}

	/// Adds a control point
	inline RoundedCornerSpline2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(Ogre::Vector2(x,y));
		return *this;
	}

	/// Safely gets a control point
	inline const Ogre::Vector2& safeGetPoint(unsigned int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/**
	 * Builds a shape from control points
	 */
	Shape realizeShape();
};

//-----------------------------------------------------------------------
/**
 * Builds a shape from a Bezier-Curve.
 * \image html spline_beziercurve.png
 */
class _ProceduralExport BezierCurve2 : public BaseSpline2<BezierCurve2>
{	
	std::vector<Ogre::Vector2> mPoints;
	unsigned int mNumSeg;

public:
	/// Default constructor
	BezierCurve2() : mNumSeg(8) {}

	/// Sets number of segments per two control points
	inline BezierCurve2& setNumSeg(unsigned int numSeg)
	{
		mNumSeg = numSeg;
		return *this;
	}

	/// Adds a control point
	inline BezierCurve2& addPoint(const Ogre::Vector2& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	/// Adds a control point
	inline BezierCurve2& addPoint(Ogre::Real x, Ogre::Real y)
	{
		mPoints.push_back(Ogre::Vector2(x,y));
		return *this;
	}
	
	/// Safely gets a control point
	inline const Ogre::Vector2& safeGetPoint(unsigned int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/**
	 * Build a shape from bezier control points
	 */
	Shape realizeShape();
};

}

#endif
