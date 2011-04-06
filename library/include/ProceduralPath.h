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
#ifndef PROCEDURAL_PATH_INCLUDED
#define PROCEDURAL_PATH_INCLUDED

#include "OgreVector3.h"
#include "OgreSimpleSpline.h"
#include "ProceduralUtils.h"
#include "ProceduralPlatform.h"
#include "ProceduralRoot.h"

namespace Procedural
{
/**
 * Succession of points in 3D space.
 * Can be closed or not.
 */
class _ProceduralExport Path
{
	std::vector<Ogre::Vector3> mPoints;
	bool mClosed;
public:
	Path() : mClosed(false)	{}

	/** Adds a point to the path, as a Vector3 */
	Path& addPoint(const Ogre::Vector3& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	/** Adds a point to the path, using its 3 coordinates */
	Path& addPoint(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mPoints.push_back(Ogre::Vector3(x,y,z));
		return *this;
	}

	/** Clears the content of the Path */
	Path& reset()
	{
		mPoints.clear();
		return *this;
	}

	/** Define the path as being closed. Almost the same as adding a last point on the first point position */
	Path& close()
	{
		assert(mPoints.size()>0 && "Cannot close an empty path");
		mClosed = true;
		return *this;
	}
	
	/** Tells if the path is closed or not */
	bool isClosed()
	{
		return mClosed;
	}

	/** Gets the list of points as a vector of Vector3 */
	std::vector<Ogre::Vector3> getPoints()
	{
		return mPoints;
	}

	/** Safely gets a given point.
	 * Takes into account whether the path is closed or not.
	 * @param i the index of the point.
	 *          if it is <0 or >maxPoint, cycle through the list of points
	 */
	const Ogre::Vector3& getPoint(int i)
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/** Gets the number of segments in the path
	 * Takes into accound whether path is closed or not
	 */
	int getSegCount()
	{
		return (mPoints.size()-1) + (mClosed?1:0);
	}

	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector3 getDirectionAfter(int i)
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
	Ogre::Vector3 getDirectionBefore(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!mClosed && i == 1)
			return (mPoints[1] - mPoints[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}

	/**
	 * Returns the local direction at the current point.
	 * @param i index of the point
	 */
	Ogre::Vector3 getAvgDirection(int i)
	{
	    return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();

	}

	/**
	 * Outputs a mesh representing the path.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name)
	{
		Ogre::ManualObject * manual = Root::getInstance()->sceneManager->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
			   
		
		for (std::vector<Ogre::Vector3>::iterator itPos = mPoints.begin(); itPos != mPoints.end();itPos++)		
			manual->position(*itPos);		
		if (mClosed)
			manual->position(*(mPoints.begin()));
		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		return mesh;
	}

};
//-----------------------------------------------------------------------
template<class T>
class BaseSpline3
{
protected:
	int mNumSeg;
	bool mClosed;
public:
	BaseSpline3() : mNumSeg(4), mClosed(false) {}	

	T& setNumSeg(int numSeg)
	{
		assert(numSeg>=1);
		mNumSeg = numSeg;
		return (T&)*this;
	}
		
	T& close()
	{
		mClosed = true;
		return (T&)*this;
	}
};
//-----------------------------------------------------------------------
/**
 * Builds a path from a Catmull-Rom Spline.
 */
class _ProceduralExport CatmullRomSpline3 : public BaseSpline3<CatmullRomSpline3>
{	
	std::vector<Ogre::Vector3> mPoints;
	public:	
	CatmullRomSpline3() {}
	
	CatmullRomSpline3(const Ogre::SimpleSpline& input) 
	{
		mPoints.resize(input.getNumPoints());
		for (unsigned short i=0; i<input.getNumPoints(); i++)
			mPoints.push_back(input.getPoint(i));
	}
	
	Ogre::SimpleSpline toSimpleSpline() const 
	{
		Ogre::SimpleSpline spline;
		for (unsigned short i=0;i<mPoints.size();i++)
			spline.addPoint(mPoints[i]);
		return spline;
	}
	
	CatmullRomSpline3& addPoint(const Ogre::Vector3& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	CatmullRomSpline3& addPoint(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mPoints.push_back(Ogre::Vector3(x,y,z));
		return *this;
	}
	
	const Ogre::Vector3& safeGetPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/**
	 * Build a path from Catmull-Rom control points
	 */
	Path realizePath();	
};
//-----------------------------------------------------------------------
/**
 * Enables to build a Path from Bezier control points.
 * Tangents are automatically calculated from control points, so the curve will "touch" every point you define
 */
class _ProceduralExport BezierPath
{
	std::vector<Ogre::Vector3> mPoints;

	int mNumSeg;
	bool mClosed;
public:
	BezierPath() : mNumSeg(4), mClosed(false) {}

	BezierPath& addPoint(const Ogre::Vector3& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	BezierPath& addPoint(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
	    mPoints.push_back(Ogre::Vector3(x,y,z));
	    return *this;
	}

	BezierPath& reset()
	{
		mPoints.clear();
		return *this;
	}

	BezierPath& close()
	{
		mClosed = true;
		return *this;
	}

	BezierPath& setNumSeg(int numSeg)
	{
		assert(numSeg>=1);
		mNumSeg = numSeg;
		return *this;
	}

	const Ogre::Vector3& getPoint(int i)
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/**
	 * Build a shape from bezier shape
	 */
	Path realizePath()
	{
		Path path;

		for (unsigned short i=0;i<mPoints.size();i++)
		{
			const Ogre::Vector3& P0 = mPoints[i];
			const Ogre::Vector3& P3 = getPoint(i+1);

			Ogre::Vector3 P1 = P0 + 0.5 * (getPoint(i+1)-getPoint(i-1));
			Ogre::Vector3 P2 = P3 - 0.5 * (getPoint(i+2)-P0);

			for (unsigned short j=0;j<mNumSeg;j++)
			{
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)mNumSeg;
				Ogre::Vector3 P = pow(1-t,3)*P0 + 3*pow(1-t,2)*t*P1 + 3*(1-t)*pow(t,2)*P2 + pow(t,3)*P3;
				path.addPoint(P);
			}
			if (i==mPoints.size()-1 && !mClosed)
			{
				path.addPoint(P3);
			}
		}
		if (mClosed)
			path.close();

		return path;
	}
};

class _ProceduralExport LinePath
{
	Ogre::Vector3 mPoint1, mPoint2;
	int mNumSeg;

public:
	LinePath() : mPoint1(Ogre::Vector3::ZERO), mPoint2(Ogre::Vector3::UNIT_Y), mNumSeg(1) {}

	LinePath& setPoint1(Ogre::Vector3 point1)
	{
		mPoint1 = point1;
		return *this;
	}

	LinePath& setPoint2(Ogre::Vector3 point2)
	{
		mPoint2 = point2;
		return *this;
	}
	
	LinePath& setNumSeg(int numSeg)
	{
		mNumSeg = numSeg;
		return *this;
	}

	LinePath& betweenPoints(Ogre::Vector3 point1, Ogre::Vector3 point2)
	{
		mPoint1 = point1;
		mPoint2 = point2;
		return *this;
	}

	Path realizePath()
	{
		assert(mNumSeg>0);
		Path p;
		for (int i=0;i<=mNumSeg;i++)
		{
			p.addPoint(i/(Ogre::Real)mNumSeg * mPoint1 + i/(Ogre::Real)mNumSeg * mPoint2);
		}
		return p;
	}

};
}

#endif
