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
#include "ProceduralStableHeaders.h"
#include "ProceduralMeshModifiers.h"

using namespace Ogre;

namespace Procedural
{
void SpherifyModifier::modify()
{
	if (mInputTriangleBuffer == NULL)
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "Input triangle buffer must be set", "Procedural::SpherifyModifier::modify()");

	for (std::vector<TriangleBuffer::Vertex>::iterator it = mInputTriangleBuffer->getVertices().begin(); it != mInputTriangleBuffer->getVertices().end(); ++it)
	{
		Real l = (it->mPosition - mCenter).length();
		if (l > 1e-6)
		{
			it->mNormal = (it->mPosition - mCenter) / l;
			it->mPosition = mCenter + mRadius * it->mNormal;
		}
	}
}

void PlaneUVModifier::modify()
{
	if (mInputTriangleBuffer == NULL)
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "Input triangle buffer must be set", "Procedural::PlanarUVsModifier::modify()");
	Vector3 xvec = mPlaneNormal.perpendicular();
	Vector3 yvec = mPlaneNormal.crossProduct(xvec);
	for (std::vector<TriangleBuffer::Vertex>::iterator it = mInputTriangleBuffer->getVertices().begin(); it != mInputTriangleBuffer->getVertices().end(); ++it)
	{
		Vector3 v = it->mPosition - mPlaneCenter;
		it->mUV.x = v.dotProduct(xvec);
		it->mUV.y = v.dotProduct(yvec);
	}
}

void SphereUVModifier::modify()
{
	if (mInputTriangleBuffer == NULL)
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "Input triangle buffer must be set", "Procedural::SphericalUVsModifier::modify()");
	for (std::vector<TriangleBuffer::Vertex>::iterator it = mInputTriangleBuffer->getVertices().begin(); it != mInputTriangleBuffer->getVertices().end(); ++it)
	{
		Vector3 v = it->mPosition.normalisedCopy();
		Vector2 vxz(v.x, v.z);
		it->mUV.x = Vector2::UNIT_X.angleTo(vxz).valueRadians() / Math::TWO_PI;
		it->mUV.y = (Math::ATan(v.y / vxz.length()).valueRadians() + Math::HALF_PI) / Math::PI;
	}
}

void HemisphereUVModifier::modify()
{
	if (mInputTriangleBuffer == NULL)
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "Input triangle buffer must be set", "Procedural::HemiSphericalUVsModifier::modify()");
	for (std::vector<TriangleBuffer::Vertex>::iterator it = mInputTriangleBuffer->getVertices().begin(); it != mInputTriangleBuffer->getVertices().end(); ++it)
	{
		Vector3 input = it->mPosition.normalisedCopy();
		Vector3 v;
		Radian r;
		if (input.y > 0)
			Vector3::UNIT_Y.getRotationTo(input).ToAngleAxis(r, v);
		else
			Vector3::NEGATIVE_UNIT_Y.getRotationTo(input).ToAngleAxis(r, v);
		Vector2 v2(input.x, input.z);
		v2.normalise();
		Vector2 uv = Vector2(.5, .5) + .5 * (r / Math::HALF_PI).valueRadians() * v2;
		it->mUV = uv;
	}
}

void CylinderUVModifier::modify()
{
	if (mInputTriangleBuffer == NULL)
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "Input triangle buffer must be set", "Procedural::CylindricalUVsModifier::modify()");
	for (std::vector<TriangleBuffer::Vertex>::iterator it = mInputTriangleBuffer->getVertices().begin(); it != mInputTriangleBuffer->getVertices().end(); ++it)
	{
		Vector2 nxz(it->mNormal.x, it->mNormal.z);
		Real alpha = (Math::ATan(it->mNormal.y / nxz.length()).valueRadians() + Math::HALF_PI);
		if (Math::Abs(alpha) > Math::HALF_PI / 2)
		{
			Vector2 vxz(it->mPosition.x, it->mPosition.z);
			it->mUV = vxz;
		} else
		{
			Vector2 vxz(it->mPosition.x, it->mPosition.z);
			it->mUV.x = Vector2::UNIT_X.angleTo(vxz).valueRadians()/Math::TWO_PI;
			it->mUV.y = it->mPosition.y - 0.5;
		}
	}
}

void BoxUVModifier::modify()
{
	if (mInputTriangleBuffer == NULL)
		OGRE_EXCEPT(Exception::ERR_INVALID_STATE, "Input triangle buffer must be set", "Procedural::BoxUVsModifier::modify()");

	Vector3 directions[6] = { Vector3::NEGATIVE_UNIT_X, Vector3::NEGATIVE_UNIT_Y, Vector3::NEGATIVE_UNIT_Z, Vector3::UNIT_X, Vector3::UNIT_Y, Vector3::UNIT_Z };

	for (std::vector<TriangleBuffer::Vertex>::iterator it = mInputTriangleBuffer->getVertices().begin(); it != mInputTriangleBuffer->getVertices().end(); ++it)
	{
		Vector3 v = it->mPosition - mBoxCenter;
		if (v.isZeroLength())
			continue;
		v.normalise();
		Vector3 n = it->mNormal;
		Real maxAxis = 0;
		int principalAxis = 0;
		for (unsigned char i = 0; i < 6; i++)
		{
			if (directions[i].dotProduct(n) > maxAxis)
			{
				maxAxis = directions[i].dotProduct(n);
				principalAxis = i;
			}
		}
		Vector3 vX = directions[principalAxis].perpendicular();
		Vector3 vY = directions[principalAxis].crossProduct(vX);

		it->mUV.x = vX.dotProduct(v);
		it->mUV.y = vY.dotProduct(v);
	}
}
}
