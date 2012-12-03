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
#ifndef PROCEDURAL_MESH_MODIFIERS_INCLUDED
#define PROCEDURAL_MESH_MODIFIERS_INCLUDED

#include "ProceduralPlatform.h"
#include "ProceduralTriangleBuffer.h"

namespace Procedural
{
/**
 \brief Projects all TriangleBufferVertices on a sphere
 */
class _ProceduralExport SpherifyModifier
{
private:
	TriangleBuffer* mInputTriangleBuffer;
	Ogre::Vector3 mCenter;
	Ogre::Real mRadius;

public:

	SpherifyModifier() :
			mInputTriangleBuffer(0), mCenter(Ogre::Vector3::ZERO), mRadius(1)
	{
	}

	/// \exception Ogre::InvalidParametersException Input triangle buffer must not be null
	SpherifyModifier& setInputTriangleBuffer(TriangleBuffer* inputTriangleBuffer)
	{
		if (inputTriangleBuffer == NULL)
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Input triangle buffer must not be null",
					"Procedural::SpherifyModifier::setInputTriangleBuffer(Procedural::TriangleBuffer*)");
		mInputTriangleBuffer = inputTriangleBuffer;
		return *this;
	}

	SpherifyModifier& setRadius(Ogre::Real radius)
	{
		if (mRadius <= 0)
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Radius must be positive",
					"Procedural::SpherifyModifier::setInputTriangleBuffer(Procedural::TriangleBuffer*)");
		mRadius = radius;
		return *this;
	}

	SpherifyModifier& setCenter(Ogre::Vector3 center)
	{
		mCenter = center;
		return *this;
	}

	/// \exception Ogre::InvalidStateException Input triangle buffer must be set
	void modify();
};

//--------------------------------------------------------------
/* TODO

 class _ProceduralExport CalculateNormalsModifier
 {
 public:
 enum NormalComputeMode
 {
 NCM_VERTEX, NCM_TRIANGLE
 };

 NormalComputeMode mComputeMode;

 CalculateNormalsModifier& setComputeMode(NormalComputeMode computeMode)
 {
 mComputeMode = computeMode;
 return *this;
 }

 void modify();
 };*/
//--------------------------------------------------------------
/**
 * \brief Recomputes the mesh's UVs based on its projection on a plane
 */
class _ProceduralExport PlaneUVModifier
{
	Ogre::Vector3 mPlaneNormal;
	Ogre::Vector3 mPlaneCenter;
	Ogre::Vector2 mPlaneSize;
	TriangleBuffer* mInputTriangleBuffer;
public:

	PlaneUVModifier() :
			mPlaneNormal(Ogre::Vector3::UNIT_Y), mPlaneCenter(Ogre::Vector3::ZERO), mPlaneSize(Ogre::Vector2::UNIT_SCALE), mInputTriangleBuffer(0)
	{
	}

	PlaneUVModifier& setPlaneNormal(const Ogre::Vector3& planeNormal)
	{
		mPlaneNormal = planeNormal;
		return *this;
	}

	PlaneUVModifier& setInputTriangleBuffer(TriangleBuffer* inputTriangleBuffer)
	{
		mInputTriangleBuffer = inputTriangleBuffer;
		return *this;
	}

	PlaneUVModifier& setPlaneCenter(const Ogre::Vector3& planeCenter)
	{
		mPlaneCenter = planeCenter;
		return *this;
	}

	PlaneUVModifier& setPlaneSize(const Ogre::Vector2& planeSize)
	{
		mPlaneSize = planeSize;
		return *this;
	}

	/// \exception Ogre::InvalidStateException Input triangle buffer must be set
	void modify();
};
//--------------------------------------------------------------
class _ProceduralExport SphereUVModifier
{
	TriangleBuffer* mInputTriangleBuffer;

public:
	void modify();

	SphereUVModifier() :
			mInputTriangleBuffer(0)
	{
	}

	SphereUVModifier& setInputTriangleBuffer(TriangleBuffer* inputTriangleBuffer)
	{
		mInputTriangleBuffer = inputTriangleBuffer;
		return *this;
	}

};
//--------------------------------------------------------------
class _ProceduralExport HemisphereUVModifier
{
	TriangleBuffer* mInputTriangleBuffer;
public:
	void modify();

	HemisphereUVModifier() :
			mInputTriangleBuffer(0)
	{
	}

	HemisphereUVModifier& setInputTriangleBuffer(TriangleBuffer* inputTriangleBuffer)
	{
		mInputTriangleBuffer = inputTriangleBuffer;
		return *this;
	}

};
//--------------------------------------------------------------
class _ProceduralExport CylinderUVModifier
{
	TriangleBuffer* mInputTriangleBuffer;
	Ogre::Real mHeight;
	Ogre::Real mRadius;
public:
	void modify();

	CylinderUVModifier() :
			mInputTriangleBuffer(0), mRadius(1.0), mHeight(1.0)
	{
	}

	CylinderUVModifier& setInputTriangleBuffer(TriangleBuffer* inputTriangleBuffer)
	{
		mInputTriangleBuffer = inputTriangleBuffer;
		return *this;
	}

	CylinderUVModifier& setRadius(Ogre::Real radius)
	{
		mRadius = radius;
		return *this;
	}

	CylinderUVModifier& setHeight(Ogre::Real height)
		{
			mHeight = height;
			return *this;
		}

};
//--------------------------------------------------------------
class _ProceduralExport BoxUVModifier
{
public:
	enum MappingType
	{
		MT_FULL, MT_CROSS, MT_PACKED
	};
private:
	TriangleBuffer* mInputTriangleBuffer;
	MappingType mMappingType;
	Ogre::Vector3 mBoxSize;
	Ogre::Vector3 mBoxCenter;
public:

	void modify();

	BoxUVModifier() :
			mInputTriangleBuffer(0), mMappingType(MT_FULL), mBoxSize(Ogre::Vector3::UNIT_SCALE), mBoxCenter(Ogre::Vector3::ZERO)
	{
	}

	BoxUVModifier& setInputTriangleBuffer(TriangleBuffer* inputTriangleBuffer)
	{
		mInputTriangleBuffer = inputTriangleBuffer;
		return *this;
	}
};
}

#endif
