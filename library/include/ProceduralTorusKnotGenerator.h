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
#ifndef PROCEDURAL_TORUS_KNOT_GENERATOR_INCLUDED
#define PROCEDURAL_TORUS_KNOT_GENERATOR_INCLUDED

#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

namespace Procedural
{
/**
 * Builds a torus knot mesh
 */
class _ProceduralExport TorusKnotGenerator : public MeshGenerator<TorusKnotGenerator>
{
	int numSegSection;
	int numSegCircle;
	Ogre::Real radius;
	Ogre::Real sectionRadius;
	int p;
	int q;
public:
	TorusKnotGenerator() : numSegSection(8),
						numSegCircle(16),
						radius(1.f),
						sectionRadius(.2f), p(2),q(3) {}

	/**
	 * Builds the mesh into the given TriangleBuffer
	 * @param buffer The TriangleBuffer on where to append the mesh.
	 */
	void addToTriangleBuffer(TriangleBuffer& buffer) const;

	/** Sets the number of segments along the section (default=8) */
	inline TorusKnotGenerator & setNumSegSection(int numSegSection)
	{
		this->numSegSection = numSegSection;
		return *this;
	}

	/** Sets the number of segments along the circle (default=16) */
	inline TorusKnotGenerator & setNumSegCircle(int numSegCircle)
	{
		this->numSegCircle = numSegCircle;
		return *this;
	}

	/** Sets the main radius of the knot (default=1) */
	inline TorusKnotGenerator & setRadius(Ogre::Real radius)
	{
		this->radius = radius;
		return *this;
	}

	/** Sets the section radius (default=0.2) */
	inline TorusKnotGenerator & setSectionRadius(Ogre::Real sectionRadius)
	{
		this->sectionRadius = sectionRadius;
		return *this;
	}

	/** Sets the p parameter of the knot (default=2) */
	inline TorusKnotGenerator & setP(int p)
	{
		this->p = p;
		return *this;
	}

	/** Sets the q parameter of the knot (default=3) */
	inline TorusKnotGenerator & setQ(int q)
	{
		this->q = q;
		return *this;
	}
	
};
}
#endif
