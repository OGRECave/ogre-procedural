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
	
	/**
	 * Bounds-safe method to get a point : it will allow you to go beyond the bounds
	 */
	inline const Ogre::Vector2& getPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/**
	 * Makes the shape a closed shape, ie it will automatically connect 
	 * the last point to the first point.
	 */
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

	inline size_t getSegCount() const
	{
		return (mPoints.size()-1) + (mClosed?1:0);
	}	

	inline bool isClosed() const
	{
	  return mClosed;
	}	

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

	inline Ogre::Vector2 getAvgDirection(int i) const
	{
		return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();
	}	

	inline Ogre::Vector2 getNormalAfter(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getDirectionAfter(i).perpendicular();
		return getDirectionAfter(i).perpendicular();
	}	

	inline Ogre::Vector2 getNormalBefore(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getDirectionBefore(i).perpendicular();
		return getDirectionBefore(i).perpendicular();
	}	

	inline Ogre::Vector2 getAvgNormal(int i) const
	{
		if (mOutSide==SIDE_RIGHT)
		return -getAvgDirection(i).perpendicular();
		return getAvgDirection(i).perpendicular();
	}

	/**
	 * Outputs a mesh representing the shape.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name);
	
	/**
	 * Appends the shape vertices to a manual object being edited
	 */
	void _appendToManualObject(Ogre::ManualObject* manual);

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
	MultiShape booleanIntersect(const Shape& other) const;
	 
	/**
	 * Computes the union between this shape and another one.
	 * Both shapes must be closed.
	 */
	MultiShape booleanUnion(const Shape& other) const;
	 
	/**
	 * Computes the difference between this shape and another one.
	 * Both shapes must be closed.
	 */
	MultiShape booleanDifference(const Shape& other) const;
	 
	/**
	 * On a closed shape, find if the outside is located on the right
	 * or on the left. If the outside can easily be determined, 
	 * you'd rather use setOutside(), which doesn't need any computation.
	 */
	Side findRealOutSide() const;	

	/**
	 * Applies the given translation to all the points already defined.
	 * Has strictly no effect on the points defined after that
	 * @param translation the translation vector
     */
	Shape& translate(const Ogre::Vector2& translation)
	{
		for (std::vector<Ogre::Vector2>::iterator it = mPoints.begin(); it!=mPoints.end(); it++)
			*it+=translation;
		return *this;
	}

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
		
	bool _findWhereToGo(const Shape* inputShapes[], BooleanOperationType opType, IntersectionInShape intersection, Ogre::uint8& shapeSelector, char& isIncreasing, int& currentSegment) const;
	
	void _findAllIntersections(const Shape& other, std::vector<IntersectionInShape>& intersections) const;
};
}

#endif
