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
#include "ProceduralGeometryHelpers.h"

using namespace Ogre;

namespace Procedural
{
	
//-----------------------------------------------------------------------
Side Shape::findRealOutSide() 
{

	throw std::exception("not implemented yet!");
	//TODO 
}
//-----------------------------------------------------------------------
void Shape::_findAllIntersections(const Shape& other, std::vector<IntersectionInShape>& intersections) const
{
	for (int i=0; i<getSegCount(); i++)
	{		
		Segment2D seg1(getPoint(i), getPoint(i+1));
		
		for (int j=0; j<other.getSegCount(); j++)
		{
			Segment2D seg2(other.getPoint(j), other.getPoint(j+1));
			
			Vector2 intersect;
			if (seg1.intersect(seg2, intersect))
			{
				IntersectionInShape inter(i, j, intersect);
				// check if intersection is "borderline" : too near to a vertex				
				if (seg1.getA().squaredDistance(intersect)<1e-8)
				{
					inter.onVertex[0] = true;					
				}
				if (seg1.getB().squaredDistance(intersect)<1e-8)
				{
					inter.onVertex[0] = true;
					inter.index[0]++;
				}
				if (seg2.getA().squaredDistance(intersect)<1e-8)
				{
					inter.onVertex[1] = true;
				}
				if (seg2.getB().squaredDistance(intersect)<1e-8)
				{
					inter.onVertex[1] = true;
					inter.index[1]++;
				}

				intersections.push_back(inter);
			}
		}
	}
}
//-----------------------------------------------------------------------
MultiShape Shape::booleanUnion(const Shape& other) const
{
	return _booleanOperation(other, BOT_UNION);
}
//-----------------------------------------------------------------------
MultiShape Shape::booleanIntersect(const Shape& other) const
{
	return _booleanOperation(other, BOT_INTERSECTION);
}
//-----------------------------------------------------------------------
MultiShape Shape::booleanDifference(const Shape& other) const
{
	return _booleanOperation(other, BOT_DIFFERENCE);
}
//-----------------------------------------------------------------------
bool Shape::_isLookingForOutside(BooleanOperationType opType, char shapeSelector) const
{
	switch(opType)
	{
		case BOT_UNION: return true;
		case BOT_INTERSECTION: return false;
		case BOT_DIFFERENCE: if (shapeSelector == 0) return true;return false;
		default : return true;
	}
}
//-----------------------------------------------------------------------
char Shape::_isIncreasing(Real d, BooleanOperationType opType, char shapeSelector) const
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
bool _sortAngles(std::pair<Radian, uint8> one, std::pair<Radian, uint8> two) // waiting for lambda functions!
{ 
	return one.first<two.first; 
}
//-----------------------------------------------------------------------
bool Shape::_findWhereToGo(const Shape* inputShapes[], BooleanOperationType opType, IntersectionInShape intersection, uint8& shapeSelector, char& isIncreasing, int& currentSegment) const
{
	if (intersection.onVertex[0] || intersection.onVertex[1])
	{
	// determine 4 directions with normal info
	// if 2 normals "face each other" then you have the couple of outside directions
	Vector2 directions[4];
	char sides[4];
	uint8 incomingDirection;
	
	// fill-in the incoming arrays
	if (isIncreasing==0)
		incomingDirection=255;
	else
		incomingDirection = shapeSelector + (isIncreasing==1?2:0);
	for (uint8 i = 0;i<2;i++)
		if (intersection.onVertex[i])
		{
			directions[i] = inputShapes[i]->getDirectionBefore(intersection.index[i]);
			directions[2+i] = -	inputShapes[i]->getDirectionAfter(intersection.index[i]);
		} else 
		{
			directions[2+i] = - inputShapes[i]->getDirectionAfter(intersection.index[i]);
			directions[i] = - directions[2+i];
		}
	for (uint8 i=0;i<4;i++)
	{
		sides[i]=(i/2==0?-1:1)*(inputShapes[i%2]->outSide == SIDE_RIGHT?-1:1);
	}
	
	bool isOutside[4];
	uint8 selectedDirection;
	std::pair<Radian, uint8> sortedDirections[4];

	// sort by angle	
	for (int i=0;i<4;i++)
	{
		if (i==0) 
			sortedDirections[i].first = 0;
		else
			sortedDirections[i].first = sides[0] * Utils::angleTo(directions[0], directions[i]);
		sortedDirections[i].second=i;
	}

	std::sort(sortedDirections, sortedDirections+4, _sortAngles);

	//find which segments are outside
	if (sides[0] != sides[sortedDirections[1].second])
	{
		isOutside[0]=isOutside[sortedDirections[1].second]=true;
		isOutside[sortedDirections[2].second]=isOutside[sortedDirections[3].second]=false;
	} else {
		isOutside[sortedDirections[1].second]=isOutside[sortedDirections[2].second]=true;
		isOutside[sortedDirections[3].second]=isOutside[sortedDirections[0].second]=false;
	}

	//find first eligible segment that is not the current segment
	for (int i=0;i<4;i++)
		if ((isOutside[i] == _isLookingForOutside(opType, i%2)) && (i!=incomingDirection))
		{
			shapeSelector = i%2;
			isIncreasing = i/2==0?1:-1;
			currentSegment = intersection.index[shapeSelector];
			return true;
		}
	// if we reach here, it means that no segment is eligible! (it should only happen with difference opereation
		return false;
	}
	else
	{
		// determine which way to go
		int nextShapeSelector = (shapeSelector+1)%2;					
				
		Real d = inputShapes[nextShapeSelector]->getDirectionAfter(intersection.index[nextShapeSelector]).dotProduct(inputShapes[shapeSelector]->getNormalAfter(currentSegment));
		isIncreasing = _isIncreasing(d, opType, nextShapeSelector);
		
		shapeSelector = nextShapeSelector;

		currentSegment = intersection.index[shapeSelector];
		return true;
	}
}
//-----------------------------------------------------------------------
MultiShape Shape::_booleanOperation(const Shape& other, BooleanOperationType opType) const
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
			{
				MultiShape ms;
				ms.addShape(*this);
				ms.addShape(other);
				ms.getShape(1).switchSide();
				return ms;
			}
			
		}
		else if (other.isPointInside(getPoint(0)))
		{// Shape A is completely inside shape B
			if (opType == BOT_UNION)
				return other;
			else if (opType == BOT_INTERSECTION)
				return *this;
			else if (opType == BOT_DIFFERENCE)
			{
				MultiShape ms;
				ms.addShape(*this);
				ms.addShape(other);
				ms.getShape(0).switchSide();
				return ms;
			}				
		}
		else
		{
			if (opType == BOT_UNION)
			{
				MultiShape ms;
				ms.addShape(*this);
				ms.addShape(other);
				return ms;
			}
			else if (opType == BOT_INTERSECTION)
				return Shape();//empty result
			else if (opType == BOT_DIFFERENCE)
				return Shape();//empty result
		}		
	}
	MultiShape outputMultiShape;
	
	const Shape* inputShapes[2];
	inputShapes[0]=this;
	inputShapes[1]=&other;		
	
	while (!intersections.empty())
	{
		Shape outputShape;
		uint8 shapeSelector = 0; // 0 : first shape, 1 : second shape
		
		Vector2 currentPosition = intersections.begin()->position;
		IntersectionInShape firstIntersection = *intersections.begin();
		int currentSegment =  firstIntersection.index[shapeSelector];
		intersections.erase(intersections.begin());
		outputShape.addPoint(currentPosition);

		char isIncreasing = 0;// +1 if increasing, -1 if decreasing, 0 if undefined

		if (!_findWhereToGo(inputShapes, opType, firstIntersection, shapeSelector, isIncreasing, currentSegment))
		{
			// That intersection is located on a place where the resulting shape won't go => discard			
			continue;
		}
						
		while (true)
		{
			// find the closest intersection on the same segment, in the correct direction			
			std::vector<IntersectionInShape>::iterator found_next_intersection = intersections.end();
			Real distanceToNextIntersection = std::numeric_limits<Real>::max();
			
			int nextPoint = currentSegment+ (isIncreasing==1?1:0);
			bool nextPointIsOnIntersection = false;
			
			for (std::vector<IntersectionInShape>::iterator it = intersections.begin(); it != intersections.end(); it++)
			{					
				if (currentSegment == it->index[shapeSelector])
				{
					if (((it->position-currentPosition).dotProduct(it->position-inputShapes[shapeSelector]->getPoint(nextPoint)) < 0)
						|| (it->onVertex[shapeSelector] && nextPoint == it->index[shapeSelector]))
					{ // found an intersection between the current one and the next segment point
						float d = (it->position-currentPosition).length();
						if (d < distanceToNextIntersection)
						{ // check if we have the nearest intersection
							found_next_intersection = it;
							distanceToNextIntersection = d;
						}
					}
				}
				if (nextPoint == it->index[shapeSelector] && it->onVertex[shapeSelector])
					nextPointIsOnIntersection = true;
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
				bool result = _findWhereToGo(inputShapes, opType, currentIntersection, shapeSelector, isIncreasing, currentSegment);
				if (!result)
				{
					assert(false && "we should not be here!");
				}
			}
			else
			{ // no intersection found for the moment => just continue on the current segment	
				if (!nextPointIsOnIntersection)
				{
				if (isIncreasing ==1)
					currentPosition = inputShapes[shapeSelector]->getPoint(currentSegment+1);
				else 
					currentPosition = inputShapes[shapeSelector]->getPoint(currentSegment);

				outputShape.addPoint(currentPosition);
				}
				currentSegment=Utils::modulo(currentSegment+isIncreasing,inputShapes[shapeSelector]->getSegCount());
			}
		}

		outputMultiShape.addShape(outputShape);
	}
	return outputMultiShape;	
}
//-----------------------------------------------------------------------
bool Shape::isPointInside(const Vector2& point) const
{
	// Draw a horizontal lines that goes through "point"
	// Using the closest intersection, find whethe the point is actually inside
	int closestSegmentIndex=-1;
	Real closestSegmentDistance = std::numeric_limits<Real>::max();
	Vector2 closestSegmentIntersection;

	for (int i =0;i<getSegCount();i++)
	{
		Vector2 A = getPoint(i);
		Vector2 B = getPoint(i+1);
		if (A.y!=B.y && (A.y-point.y)*(B.y-point.y)<0.)
		{
			Vector2 intersect((point.y-A.y)/(B.y-A.y), point.y);			
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
MeshPtr Shape::realizeMesh(const std::string& name)
{
	ManualObject * manual = Root::getInstance()->sceneManager->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_STRIP);
	
	_appendToManualObject(manual);
	
	manual->end();
	MeshPtr mesh = manual->convertToMesh(name);
	return mesh;
}
//-----------------------------------------------------------------------
void Shape::_appendToManualObject(ManualObject* manual)
{
	for (std::vector<Vector2>::iterator itPos = points.begin(); itPos != points.end();itPos++)		
		manual->position(Vector3(itPos->x, itPos->y, 0.f));		
	if (closed)
		manual->position(Vector3(points.begin()->x, points.begin()->y, 0.f));
}

}

