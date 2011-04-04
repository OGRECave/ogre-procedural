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
#include "ProceduralMultiShape.h"
#include "ProceduralShape.h"

using namespace Ogre;

namespace Procedural
{
	MeshPtr MultiShape::realizeMesh(const std::string& name)
	{
		ManualObject * manual = Root::getInstance()->sceneManager->createManualObject(name);
				
		for (std::vector<Shape>::iterator it = shapes.begin(); it!=shapes.end(); it++)
		{
			manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_STRIP);
			it->_appendToManualObject(manual);
			manual->end();
		}		
		
		MeshPtr mesh = manual->convertToMesh(name);
		Root::getInstance()->sceneManager->destroyManualObject(manual);
		return mesh;
	}

	std::vector<Vector2> MultiShape::getPoints() const
	{
		std::vector<Vector2> result;		
		for (size_t i = 0;i<shapes.size(); i++)
		{
			std::vector<Vector2> points = shapes[i].getPoints();
			result.insert(result.end(), points.begin(), points.end());
		}
		return result;
	}


	bool MultiShape::isPointInside(const Vector2& point) const
	{
		int score=0;
		for (size_t i =0;i<shapes.size();i++)
		{
			if (shapes[i].isPointInside(point))
				score++;
			else
				score--;
		}
		return score>=1;
	}

}