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
#ifndef PROCEDURAL_EXTRUDER_INCLUDED
#define PROCEDURAL_EXTRUDER_INCLUDED

#include "ProceduralShape.h"
#include "ProceduralPath.h"
#include "ProceduralPlatform.h"
#include "ProceduralMeshGenerator.h"
#include "ProceduralMultiShape.h"

namespace Procedural
{
/** Extrudes a 2D shape along a path to build an extruded mesh.
 * Can be used to build things such as pipelines, roads...
 */
class _ProceduralExport Extruder : public MeshGenerator<Extruder>
{
	Shape* mShapeToExtrude;
	MultiShape* mMultiShapeToExtrude;
	Path* mExtrusionPath;
	bool mCapped;
	bool mFixSharpAngles;

	void _extrudeBodyImpl(TriangleBuffer& buffer, const Shape* shapeToExtrude) const;

	void _extrudeCapImpl(TriangleBuffer& buffer) const;
	
public:
	/// Default constructor
	Extruder() : mShapeToExtrude(0), mExtrusionPath(0), mCapped(true), mFixSharpAngles(false)
	{}
	
	/**
	 * Builds the mesh into the given TriangleBuffer
	 * @param buffer The TriangleBuffer on where to append the mesh.
	 */
	void addToTriangleBuffer(TriangleBuffer& buffer) const;

	/** Sets the shape to extrude. Mutually exclusive with setMultiShapeToExtrude. */
	inline Extruder & setShapeToExtrude(Shape* shapeToExtrude)
	{
		mMultiShapeToExtrude = 0;
		mShapeToExtrude = shapeToExtrude;
		return *this;
	}

	/** Sets the multishape to extrude. Mutually exclusive with setShapeToExtrude. */
	inline Extruder & setMultiShapeToExtrude(MultiShape* multiShapeToExtrude)
	{
		mShapeToExtrude = 0;
		mMultiShapeToExtrude = multiShapeToExtrude;
		return *this;
	}

	/** Sets the extrusion path */
	inline Extruder & setExtrusionPath(Path* extrusionPath)
	{
		mExtrusionPath = extrusionPath;
		return *this;
	}

	/** Sets whether caps are added to the extremities or not (not closed paths only) */
	inline Extruder & setCapped(bool capped)
	{
		mCapped = capped;
		return *this;
	}

	/// WIP
	/// Sets the FixSharpAngles option (default = false)
	/// When enabled, extruder tries to prevent the generated mesh to self intersect when 
	/// the angles coming from the path are too sharp
	inline Extruder & setFixSharpAngles(bool fixSharpAngles)
	{
		mFixSharpAngles = fixSharpAngles;
		return *this;
	}
};
}

#endif