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
#include "ProceduralStableHeaders.h"
#include "ProceduralGeometryHelpers.h"

namespace Procedural
{
//-----------------------------------------------------------------------
Circle::Circle(Ogre::Vector2 p1, Ogre::Vector2 p2, Ogre::Vector2 p3)
{
	Ogre::Vector2 c1 = .5*(p1+p2);
	Ogre::Vector2 d1 = (p2-p1).perpendicular();
	float a1 = d1.y;
	float b1 = -d1.x;
	float g1 = d1.x*c1.y-d1.y*c1.x;

	Ogre::Vector2 c3 = .5*(p2+p3);
	Ogre::Vector2 d3 = (p3-p2).perpendicular();
	float a2 = d3.y;
	float b2 = -d3.x;
	float g2 = d3.x*c3.y-d3.y*c3.x;
	
	Ogre::Vector2 intersect;
	float intersectx = (b2*g1-b1*g2)/(b1*a2-b2*a1);
	float intersecty = (a2*g1-a1*g2)/(a1*b2-a2*b1);		

	intersect = Ogre::Vector2(intersectx, intersecty);

	mCenter = intersect;
	mRadius = (intersect-p1).length();
}
//-----------------------------------------------------------------------
bool Segment2D::intersect(const Segment2D& other, Ogre::Vector2& intersection) const
	{		
		const Ogre::Vector2& p1 = mA;
		const Ogre::Vector2& p2 = mB;
		const Ogre::Vector2& p3 = other.mA;
		const Ogre::Vector2& p4 = other.mB;


		Ogre::Vector2 d1 = p2-p1;
		float a1 = d1.y;
		float b1 = -d1.x;
		float g1 = d1.x*p1.y-d1.y*p1.x;
				
		Ogre::Vector2 d3 = p4-p3;
		float a2 = d3.y;
		float b2 = -d3.x;
		float g2 = d3.x*p3.y-d3.y*p3.x;

		// if both segments are parallel, early out
		if (d1.crossProduct(d3) == 0.)
			return false;
	
		Ogre::Vector2 intersect;
		float intersectx = (b2*g1-b1*g2)/(b1*a2-b2*a1);
		float intersecty = (a2*g1-a1*g2)/(a1*b2-a2*b1);		
	
		intersect = Ogre::Vector2(intersectx, intersecty);

		if ((intersect-p1).dotProduct(intersect-p2)<0 && (intersect-p3).dotProduct(intersect-p4)<0)
		{
			intersection = intersect;
			return true;
		}
		return false;
	}
//-----------------------------------------------------------------------
bool Plane::intersect(const Plane& other, Line& outputLine) const
	{		
		//TODO : handle the case where the plane is perpendicular to T
		Ogre::Vector3 point1;
		Ogre::Vector3 direction = normal.crossProduct(other.normal);
		if (direction.squaredLength() < 1e-08)
			return false;
		
		Ogre::Real denom = 1.f/(normal.x*other.normal.y-other.normal.x*normal.y);
		{
			Ogre::Real d1 = d;
			Ogre::Real d2 = other.d;
			point1.x = (normal.y*d2-other.normal.y*d1)*denom;
			point1.y = (other.normal.x*d1-normal.x*d2)*denom;
			point1.z = 0;
		}
		
		outputLine = Line(point1, direction);

		return true;
	}
//-----------------------------------------------------------------------
Ogre::Vector3 Line::shortestPathToPoint(const Ogre::Vector3& point) const
{
	Ogre::Vector3 projection = (point-mPoint).dotProduct(mDirection) * mDirection;
	Ogre::Vector3 vec = -projection+point-mPoint;
	return vec;
}

}
