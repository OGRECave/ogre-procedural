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

	Shape& addPoint(const Ogre::Vector2& pt)
	{
		points.push_back(pt);
		return *this;
	}

	Shape& addPoint(Ogre::Real x, Ogre::Real y)
	{
		points.push_back(Ogre::Vector2(x, y));
		return *this;
	}

	Shape& reset()
	{
		points.clear();
		return *this;
	}

	std::vector<Ogre::Vector2> getPoints() const
	{
		return points;
	}

	const Ogre::Vector2& getPoint(int i) const
	{
		if (closed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	Shape& close()
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
	Shape& setOutSide(Side side)
	{
		outSide = side;
		return *this;
	}

	Side getOutSide() const
	{
		return outSide;
	}

	int getSegCount() const
	{
		return (points.size()-1) + (closed?1:0);
	}


	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector2 getDirectionAfter(int i) const
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
	Ogre::Vector2 getDirectionBefore(int i) const
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!closed && i == 1)
			return (points[1] - points[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}

	Ogre::Vector2 getAvgDirection(int i) const
	{
		return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();

	}

	/**
	 * Outputs a mesh representing the shape.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name)
	{
		Ogre::ManualObject * manual = Root::getInstance()->sceneManager->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
			   
		
		for (std::vector<Ogre::Vector2>::iterator itPos = points.begin(); itPos != points.end();itPos++)		
			manual->position(Ogre::Vector3(itPos->x, itPos->y, 0.f));		
		if (closed)
			manual->position(Ogre::Vector3(points.begin()->x, points.begin()->y, 0.f));
		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		//mesh->_setBounds( boundingBox, false );
		//mesh->_setBoundingSphereRadius(boundingSphereRadius);

		return mesh;
	}
};

template<class T>
class BaseSpline
{
protected:
	int numSeg;
	bool closed;
	Side outSide;
public:
	BaseSpline() : numSeg(4), closed(false), outSide(SIDE_RIGHT) {}
	
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
class _ProceduralExport CubicHermiteSpline : public BaseSpline<CubicHermiteSpline>
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
	 * Build a shape from control points
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
class _ProceduralExport CatmullRomSpline : public BaseSpline<CatmullRomSpline>
{	
	std::vector<Ogre::Vector2> points;
	public:	
	CatmullRomSpline& addPoint(const Ogre::Vector2& pt)
	{
		points.push_back(pt);
		return *this;
	}

	CatmullRomSpline& addPoint(Ogre::Real x, Ogre::Real y)
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
			const Ogre::Vector2& P0 = points[i];
			const Ogre::Vector2& P3 = safeGetPoint(i+1);

			Ogre::Vector2 P1 = P0 + 0.5 * (safeGetPoint(i+1)-safeGetPoint(i-1));
			Ogre::Vector2 P2 = P3 - 0.5 * (safeGetPoint(i+2)-P0);

			for (int j=0;j<numSeg;j++)
			{
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
				Ogre::Vector2 P = pow(1-t,3)*P0 + 3*pow(1-t,2)*t*P1 + 3*(1-t)*pow(t,2)*P2 + pow(t,3)*P3;
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
class _ProceduralExport KochanekBartelsSpline : public BaseSpline<KochanekBartelsSpline>
{	
	struct ControlPoint
	{
		Ogre::Vector2 position;
		Ogre::Real tension;
		Ogre::Real bias;
		Ogre::Real continuity;
		
		ControlPoint(Ogre::Vector2 p, Ogre::Real t, Ogre::Real b, Ogre::Real c) : position(p), tension(t), bias(b), continuity(c) {}
	};

	std::vector<ControlPoint> points;
	
public:
	/**
	 * Adds a control point to the spline
	 * @arg p Point position
	 * @arg t Tension    +1 = Tight            -1 = Round
	 * @arg b Bias       +1 = Post-shoot       -1 = Pre-shoot
	 * @arg c Continuity +1 = Inverted Corners -1 = Box Corners
	 */
	void addPoint(Ogre::Vector2 p, Ogre::Real t, Ogre::Real b, Ogre::Real c)
	{
		points.push_back(ControlPoint(p,t,b,c));
	}

	/**
	 * Build a shape from control points
	 */
	Shape realizeShape()
	{
		Shape shape;
		
		int numPoints = closed?points.size():points.size()-1;		
		for (int i=0;i<numPoints;i++)
		{
			const ControlPoint& pm1 = points[i-1];
			const ControlPoint& p0 = points[i];
			const ControlPoint& p1 = points[i+1];
			const ControlPoint& p2 = points[i+2];
			
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
