#include "SphereGenerator.h"

Ogre::MeshPtr SphereGenerator::realizeMesh()
	{
		Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		
	Ogre::Real fDeltaRingAngle = (Ogre::Math::PI / numRings);
	Ogre::Real fDeltaSegAngle = (Ogre::Math::TWO_PI / numSegments);
	unsigned short wVerticeIndex = 0 ;

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
				manual->index(wVerticeIndex + numSegments + 1);
				manual->index(wVerticeIndex);               
				manual->index(wVerticeIndex + numSegments);
				manual->index(wVerticeIndex + numSegments + 1);
				manual->index(wVerticeIndex + 1);
				manual->index(wVerticeIndex);
				wVerticeIndex ++;
				}
		}; // end for seg
	} // end for ring
	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-radius, -radius, -radius), Ogre::Vector3(radius, radius, radius) ), false );

	mesh->_setBoundingSphereRadius(radius);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
	}