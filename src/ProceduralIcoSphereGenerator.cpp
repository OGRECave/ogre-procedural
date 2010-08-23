#include "ProceduralIcoSphereGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void IcoSphereGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
/*Ogre::Real fDeltaRingAngle = (Ogre::Math::PI / numRings);
	Ogre::Real fDeltaSegAngle = (Ogre::Math::TWO_PI / numSegments);

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= numRings; ring++ ) {
		Ogre::Real r0 = radius * sinf (ring * fDeltaRingAngle);
		Ogre::Real y0 = radius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= numSegments; seg++) {
			Ogre::Real x0 = r0 * sinf(seg * fDeltaSegAngle);
			Ogre::Real z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, y0, z0);
			if (enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for (unsigned int tc=0;tc<numTexCoordSet;tc++)
				manual->textureCoord((Ogre::Real) seg / (Ogre::Real) numSegments * uTile, (Ogre::Real) ring / (Ogre::Real) numRings * vTile);

			if (ring != numRings) {
				// each vertex (except the last) has six indices pointing to it
				manual->index(offset + numSegments + 1);
				manual->index(offset);
				manual->index(offset + numSegments);
				manual->index(offset + numSegments + 1);
				manual->index(offset + 1);
				manual->index(offset);
				offset ++;
				}
		}; // end for seg
	} // end for ring

	boundingRadius = radius;
	Utils::updateAABB(aabb, Ogre::AxisAlignedBox(-radius, -radius, -radius, radius, radius, radius));*/
}
}
