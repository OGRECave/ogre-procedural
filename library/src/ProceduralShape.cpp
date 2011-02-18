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
#include "ProceduralShape.h"

namespace Procedural
{
//-----------------------------------------------------------------------
void Shape::_findAllIntersections(const Shape& other, std::vector<IntersectionInShape>& intersections) const
{
	for (int i=0; i<getSegCount(); i++)
	{
		Ogre::Vector2 a1 = getPoint(i);
		Ogre::Vector2 a2 = getPoint(i+1);
		
		for (int j=0; j<other.getSegCount(); j++)
		{
			Ogre::Vector2 b1 = other.getPoint(j);
			Ogre::Vector2 b2 = other.getPoint(j+1);
			
			Ogre::Vector2 intersect;
			if (Utils::segmentIntersection(a1,a2,b1,b2, intersect))
			{
				intersections.push_back(IntersectionInShape(i, j, intersect));
			}
		}
	}
}
//-----------------------------------------------------------------------
Shape Shape::booleanUnion(const Shape& other) const
{
	return _booleanOperation(other, BOT_UNION);
}
//-----------------------------------------------------------------------
Shape Shape::booleanIntersect(const Shape& other) const
{
	return _booleanOperation(other, BOT_INTERSECTION);
}
//-----------------------------------------------------------------------
Shape Shape::booleanDifference(const Shape& other) const
{
	return _booleanOperation(other, BOT_DIFFERENCE);
}
//-----------------------------------------------------------------------
char Shape::_isIncreasing(Ogre::Real d, BooleanOperationType opType, char shapeSelector) const
{
	if (d<0 && opType == BOT_UNION)
		return -1;
	if (d>0 && opType == BOT_INTERSECTION)
		return -1;
	if (opType == BOT_DIFFERENCE)
	{
		if ((d<0 && shapeSelector == 0)||(d>0 && shapeSelector == 1))
			return -1;
	}
	return 1;
}
//-----------------------------------------------------------------------
Shape Shape::_booleanOperation(const Shape& other, BooleanOperationType opType) const
{
	assert(closed && other.closed);
	assert(points.size()>1 && other.points.size()>1);
	
	// Compute the intersection between the 2 shapes
	std::vector<IntersectionInShape> intersections;
	_findAllIntersections(other, intersections);
	
	// Build the resulting shape
	if (intersections.empty())
	{
		if (isPointInside(other.getPoint(0)))
		{// Shape B is completely inside shape A
			if (opType == BOT_UNION)
				return *this;
			else if (opType == BOT_INTERSECTION)
				return other;
			else if (opType == BOT_DIFFERENCE)
				return *this;//TODO : multi-shape with other normals inverted
		}
		else if (other.isPointInside(getPoint(0)))
		{// Shape A is completely inside shape B
			if (opType == BOT_UNION)
				return other;
			else if (opType == BOT_INTERSECTION)
				return *this;
			else if (opType == BOT_DIFFERENCE)
				return *this;//TODO : multi-shape with this normals inverted
		}
		else
		{
			if (opType == BOT_UNION)
				return *this;//TODO: multi-shape
			else if (opType == BOT_INTERSECTION)
				return Shape();//empty result
			else if (opType == BOT_DIFFERENCE)
				return Shape();//empty result
		}		
	}
	else
	{
		Shape outputShape;		
		char shapeSelector = 0; // 0 : first shape, 1 : second shape
		const Shape* inputShapes[2];
		inputShapes[0]=this;
		inputShapes[1]=&other;		
		
		Ogre::Vector2 currentPosition = intersections.begin()->position;
		IntersectionInShape firstIntersection = *intersections.begin();
		int currentSegment =  firstIntersection.index[shapeSelector];
		intersections.erase(intersections.begin());
		outputShape.addPoint(currentPosition);

		
		Ogre::Real d = inputShapes[shapeSelector]->getDirectionAfter(currentSegment).dotProduct(inputShapes[(shapeSelector+1)%2]->getNormalAfter(firstIntersection.index[(shapeSelector+1)%2]));
		char isIncreasing = _isIncreasing(d, opType, shapeSelector);// +1 if increasing, -1 if decreasing	
				
		while (true)
		{
			// find the closest intersection on the same segment, in the correct direction			
				std::vector<IntersectionInShape>::iterator found_next_intersection = intersections.end();
				Ogre::Real distanceToNextIntersection = std::numeric_limits<Ogre::Real>::max();
				
				int nextPoint = currentSegment+ (isIncreasing==1?1:0);
				
				for (std::vector<IntersectionInShape>::iterator it = intersections.begin(); it != intersections.end(); it++)
				{					
					if (currentSegment == it->index[shapeSelector])
					{
						if ((it->position-currentPosition).dotProduct(it->position-inputShapes[shapeSelector]->getPoint(nextPoint)) < 0)
						{ // found an intersection between the current one and the next segment point
							float d = (it->position-currentPosition).length();
							if (d < distanceToNextIntersection)
							{ // check if we have the nearest intersection
								found_next_intersection = it;
								distanceToNextIntersection = d;
							}
						}
					}
				}

				// stop condition
				if (currentSegment == firstIntersection.index[shapeSelector]) {
					// we found ourselves on the same segment as the first intersection and no other
						if ((firstIntersection.position-currentPosition).dotProduct(firstIntersection.position-inputShapes[shapeSelector]->getPoint(nextPoint)) < 0)
						{
							float d = (firstIntersection.position-currentPosition).length();
							if (d>0. && d < distanceToNextIntersection)
							{
								outputShape.close();
								break;
							}
						}							
				}
				
				// We actually found the next intersection => change direction and add current intersection to the list
				if (found_next_intersection != intersections.end())
				{ 
					IntersectionInShape currentIntersection = *found_next_intersection;
					intersections.erase(found_next_intersection);
					outputShape.addPoint(currentIntersection.position);
					currentPosition = currentIntersection.position;
	
					// determine which way to go
					Ogre::Vector2 currentNormal = inputShapes[shapeSelector]->getNormalAfter(currentSegment);
					shapeSelector = (shapeSelector+1)%2;					
					
					Ogre::Real d = inputShapes[shapeSelector]->getDirectionAfter(currentIntersection.index[shapeSelector]).dotProduct(currentNormal);
					isIncreasing = _isIncreasing(d, opType, shapeSelector);

					currentSegment = currentIntersection.index[shapeSelector];
				}
				else
				{ // no intersection found for the moment => just continue on the current segment					
					if (isIncreasing ==1)
						currentPosition = inputShapes[shapeSelector]->getPoint(currentSegment+1);
					else 
						currentPosition = inputShapes[shapeSelector]->getPoint(currentSegment);

					outputShape.addPoint(currentPosition);
					currentSegment=Utils::modulo(currentSegment+isIncreasing,inputShapes[shapeSelector]->getSegCount());
				}
			}	
		return outputShape;
	}
}
//-----------------------------------------------------------------------
bool Shape::isPointInside(const Ogre::Vector2& point) const
{
	// Draw a horizontal lines that goes through "point"
	// Using the closest intersection, find whethe the point is actually inside
	int closestSegmentIndex=-1;
	Ogre::Real closestSegmentDistance = std::numeric_limits<Ogre::Real>::max();
	Ogre::Vector2 closestSegmentIntersection;

	for (int i =0;i<getSegCount();i++)
	{
		Ogre::Vector2 A = getPoint(i);
		Ogre::Vector2 B = getPoint(i+1);
		if (A.y!=B.y && (A.y-point.y)*(B.y-point.y)<0.)
		{
			Ogre::Vector2 intersect((point.y-A.y)/(B.y-A.y), point.y);			
			float dist = abs(point.x-closestSegmentIntersection.x);
			if (dist<closestSegmentDistance)
			{
				closestSegmentIndex = i;
				closestSegmentDistance = dist;
				closestSegmentIntersection = intersect;
			}
		}
	}
	if (closestSegmentIndex!=-1)
		if (getNormalAfter(closestSegmentIndex).x * (point.x-closestSegmentIntersection.x)<0)		
			return true;

	return false;
}
//-----------------------------------------------------------------------
Ogre::MeshPtr Shape::realizeMesh(const std::string& name)
{
	Ogre::ManualObject * manual = Root::getInstance()->sceneManager->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
	
	for (std::vector<Ogre::Vector2>::iterator itPos = points.begin(); itPos != points.end();itPos++)		
		manual->position(Ogre::Vector3(itPos->x, itPos->y, 0.f));		
	if (closed)
		manual->position(Ogre::Vector3(points.begin()->x, points.begin()->y, 0.f));
	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	return mesh;
}

}

