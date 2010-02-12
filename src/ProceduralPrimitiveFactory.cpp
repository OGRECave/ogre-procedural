#include "ProceduralPrimitiveFactory.h"

Ogre::MeshPtr ProceduralPrimitiveFactory::createSphere(const Ogre::String& name, float r, int nRings, int nSegments, bool normals, int numTexCoordSet, float uTile, float vTile)
{
	Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		
	float fDeltaRingAngle = (Ogre::Math::PI / nRings);
	float fDeltaSegAngle = (Ogre::Math::TWO_PI / nSegments);
	unsigned short wVerticeIndex = 0 ;

	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= nRings; ring++ ) {
		float r0 = r * sinf (ring * fDeltaRingAngle);
		float y0 = r * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(int seg = 0; seg <= nSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, y0, z0);
			if (normals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for (int tc=0;tc<numTexCoordSet;tc++)
				manual->textureCoord((float) seg / (float) nSegments * uTile, (float) ring / (float) nRings * vTile);

			if (ring != nRings) {
				// each vertex (except the last) has six indices pointing to it
				manual->index(wVerticeIndex + nSegments + 1);
				manual->index(wVerticeIndex);               
				manual->index(wVerticeIndex + nSegments);
				manual->index(wVerticeIndex + nSegments + 1);
				manual->index(wVerticeIndex + 1);
				manual->index(wVerticeIndex);
				wVerticeIndex ++;
				}
		}; // end for seg
	} // end for ring
	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}

void ProceduralPrimitiveFactory::_internalPlane(Ogre::ManualObject* manual, int& offset, const Ogre::Vector3& normal, int numSeg1, int numSeg2, const Ogre::Vector3& orig, const Ogre::Vector3& delta1, const Ogre::Vector3& delta2, float uTile, float vTile)
{
for (int i1 = 0; i1<=numSeg1;i1++)
		for (int i2 = 0; i2<=numSeg2;i2++)
			{
				manual->position(orig+i1*delta1+i2*delta2);
				manual->textureCoord(i1/(float)numSeg1*uTile, i2/(float)numSeg2*vTile);
				manual->normal(normal);
			}

	bool reverse = false;
	if (delta1.crossProduct(delta2).dotProduct(normal)>0)
		reverse= true;
	for (int n1 = 0; n1<numSeg1;n1++)
	{
		for (int n2 = 0; n2<numSeg2;n2++)
		{	
			if (reverse)
			{	
			manual->index(offset+0);
			manual->index(offset+(numSeg2+1));		
			manual->index(offset+1);
			manual->index(offset+1);		
			manual->index(offset+(numSeg2+1));					
			manual->index(offset+(numSeg2+1)+1);
			} else {				
			manual->index(offset+0);
			manual->index(offset+1);
			manual->index(offset+(numSeg2+1));		
			manual->index(offset+1);		
			manual->index(offset+(numSeg2+1)+1);
			manual->index(offset+(numSeg2+1));	
			}
			offset++;
		}
		offset++;
	}
	offset+=numSeg2+1;
}

Ogre::MeshPtr ProceduralPrimitiveFactory::createBox(const Ogre::String& name, float sizeX, float sizeY, float sizeZ, int numSegX, int numSegY, int numSegZ, float uTile, float vTile)
{
	Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	
	int offset=0;	
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_Z, numSegX, numSegY, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeX/(float)numSegX*Ogre::Vector3::UNIT_X, sizeY/(float)numSegY*Ogre::Vector3::UNIT_Y, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_Z,          numSegX, numSegY, Ogre::Vector3(-.5*sizeX,-.5*sizeY, .5*sizeZ), sizeX/(float)numSegX*Ogre::Vector3::UNIT_X, sizeY/(float)numSegY*Ogre::Vector3::UNIT_Y, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_Y, numSegX, numSegZ, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeX/(float)numSegX*Ogre::Vector3::UNIT_X, sizeZ/(float)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_Y,          numSegX, numSegZ, Ogre::Vector3(-.5*sizeX, .5*sizeY,-.5*sizeZ), sizeX/(float)numSegX*Ogre::Vector3::UNIT_X, sizeZ/(float)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::NEGATIVE_UNIT_X, numSegY, numSegZ, Ogre::Vector3(-.5*sizeX,-.5*sizeY,-.5*sizeZ), sizeY/(float)numSegY*Ogre::Vector3::UNIT_Y, sizeZ/(float)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 
	_internalPlane(manual, offset, Ogre::Vector3::UNIT_X,          numSegY, numSegZ, Ogre::Vector3( .5*sizeX,-.5*sizeY,-.5*sizeZ), sizeY/(float)numSegY*Ogre::Vector3::UNIT_Y, sizeZ/(float)numSegZ*Ogre::Vector3::UNIT_Z, uTile, vTile); 

	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	float r = sizeX;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}

Ogre::MeshPtr ProceduralPrimitiveFactory::createCylinder(const Ogre::String& name, float radius, float height, bool capped, int numSegHeight, int numSegBase, float uTile, float vTile)
{
	Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	float deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	float deltaHeight = height/(float)numSegHeight;

	int verticeIndex=0;
	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			float x0 = radius * cosf(j*deltaAngle);
			float z0 = radius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(float)numSegBase*uTile, i/(float)numSegHeight*vTile);

			if (i != numSegHeight) {
				manual->index(verticeIndex + numSegBase + 1);
				manual->index(verticeIndex);               
				manual->index(verticeIndex + numSegBase);
				manual->index(verticeIndex + numSegBase + 1);
				manual->index(verticeIndex + 1);
				manual->index(verticeIndex);		
				}
					verticeIndex ++;
		}
	if (capped)
	{
		//low cap
		int centerIndex = verticeIndex;
		manual->position(0,0,0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(0.0,vTile);
		verticeIndex++;
		for (int j=0;j<=numSegBase;j++)
		{
			float x0 = radius * cosf(j*deltaAngle);
			float z0 = radius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(float)numSegBase,0.0);
			if (j!=numSegBase)
			{
				manual->index(centerIndex);
				manual->index(verticeIndex);
				manual->index(verticeIndex+1);
			}
			verticeIndex++;
		}
		// high cap
		centerIndex = verticeIndex;
		manual->position(0,height,0);
		manual->normal(Ogre::Vector3::UNIT_Y);
		manual->textureCoord(0.0,0.0);
		verticeIndex++;
		for (int j=0;j<=numSegBase;j++)
		{
			float x0 = radius * cosf(j*deltaAngle);
			float z0 = radius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(float)numSegBase*uTile,vTile);
			if (j!=numSegBase)
			{
				manual->index(centerIndex);
				manual->index(verticeIndex+1);				
				manual->index(verticeIndex);
			}
			verticeIndex++;
		}
	}

	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	float r = height;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}


Ogre::MeshPtr ProceduralPrimitiveFactory::createCone(const Ogre::String& name, float radius, float height, int numSegHeight, int numSegBase, float uTile, float vTile)
{
	Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	float deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	float deltaHeight = height/(float)numSegHeight;

	int verticeIndex=0;
	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			float r0 = radius * (1 - i / (float)numSegHeight);
			float x0 = r0 * cosf(j*deltaAngle);
			float z0 = r0 * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(float)numSegBase*uTile, i/(float)numSegHeight*vTile);

			if (i != numSegHeight) {
				manual->index(verticeIndex + numSegBase + 1);
				manual->index(verticeIndex);               
				manual->index(verticeIndex + numSegBase);
				manual->index(verticeIndex + numSegBase + 1);
				manual->index(verticeIndex + 1);
				manual->index(verticeIndex);		
				}
			verticeIndex ++;
		}

		//low cap
		int centerIndex = verticeIndex;
		manual->position(0,0,0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(0.0,vTile);
		verticeIndex++;
		for (int j=0;j<=numSegBase;j++)
		{
			float x0 = radius * cosf(j*deltaAngle);
			float z0 = radius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(float)numSegBase*uTile,0.0);
			if (j!=numSegBase)
			{
				manual->index(centerIndex);
				manual->index(verticeIndex);
				manual->index(verticeIndex+1);
			}
			verticeIndex++;
		}

	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	float r = height;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}

Ogre::MeshPtr ProceduralPrimitiveFactory::createTube(const Ogre::String& name, float innerRadius, float outerRadius, float height, int numSegHeight, int numSegBase, float uTile, float vTile)
{
	Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	float deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	float deltaHeight = height/(float)numSegHeight;

	int verticeIndex=0;
	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			float x0 = outerRadius * cosf(j*deltaAngle);
			float z0 = outerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(float)numSegBase*uTile, i/(float)numSegHeight*vTile);

			if (i != numSegHeight) {
				manual->index(verticeIndex + numSegBase + 1);
				manual->index(verticeIndex);               
				manual->index(verticeIndex + numSegBase);
				manual->index(verticeIndex + numSegBase + 1);
				manual->index(verticeIndex + 1);
				manual->index(verticeIndex);		
				}
					verticeIndex ++;
		}

	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			float x0 = innerRadius * cosf(j*deltaAngle);
			float z0 = innerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(-Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(float)numSegBase*uTile, i/(float)numSegHeight*vTile);

			if (i != numSegHeight) {
				manual->index(verticeIndex + numSegBase + 1);
				manual->index(verticeIndex + numSegBase);
				manual->index(verticeIndex);               
				manual->index(verticeIndex + numSegBase + 1);				
				manual->index(verticeIndex);		
				manual->index(verticeIndex + 1);
				}
					verticeIndex ++;
		}


		//low cap
		for (int j=0;j<=numSegBase;j++)
		{
			float x0 = innerRadius * cosf(j*deltaAngle);
			float z0 = innerRadius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(float)numSegBase*uTile,vTile);

			x0 = outerRadius * cosf(j*deltaAngle);
			z0 = outerRadius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);			
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(float)numSegBase*uTile,0.0);

			if (j!=numSegBase)
			{
				manual->index(verticeIndex);
				manual->index(verticeIndex+1);
				manual->index(verticeIndex+3);
				manual->index(verticeIndex+2);
				manual->index(verticeIndex);
				manual->index(verticeIndex+3);
			}
			verticeIndex+=2;
		}

		
		//high cap
		for (int j=0;j<=numSegBase;j++)
		{
			float x0 = innerRadius * cosf(j*deltaAngle);
			float z0 = innerRadius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(float)numSegBase*uTile,0.0);

			x0 = outerRadius * cosf(j*deltaAngle);
			z0 = outerRadius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);			
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(float)numSegBase*uTile,vTile);

			if (j!=numSegBase)
			{
				manual->index(verticeIndex+1);
				manual->index(verticeIndex);				
				manual->index(verticeIndex+3);
				manual->index(verticeIndex);
				manual->index(verticeIndex+2);
				manual->index(verticeIndex+3);
			}
			verticeIndex+=2;
		}
		

	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	float r = height;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}

Ogre::MeshPtr ProceduralPrimitiveFactory::createTorus(const Ogre::String& name, float radius, float sectionRadius, int numSegCircle, int numSegSection, float uTile, float vTile)
{
	Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	float deltaSection = (Ogre::Math::TWO_PI / numSegSection);
	float deltaCircle = (Ogre::Math::TWO_PI / numSegCircle);

	int verticeIndex=0;
	for (int i = 0; i <=numSegCircle; i++)
		for (int j = 0; j<=numSegSection; j++)
		{
			Ogre::Vector3 c0(radius, 0.0, 0.0);
			Ogre::Vector3 v0(radius+sectionRadius * cosf(j*deltaSection),sectionRadius * sinf(j*deltaSection),0.0);
			Ogre::Quaternion q;
			q.FromAngleAxis(Ogre::Radian(i*deltaCircle),Ogre::Vector3::UNIT_Y);
			Ogre::Vector3 v = q * v0;
			Ogre::Vector3 c = q * c0;
			manual->position(v);
			manual->normal((v-c).normalisedCopy());
			manual->textureCoord(i/(float)numSegCircle*uTile, j/(float)numSegSection*vTile);

			if (i != numSegCircle) 
			{
				manual->index(verticeIndex + numSegSection + 1);
				manual->index(verticeIndex);               
				manual->index(verticeIndex + numSegSection);
				manual->index(verticeIndex + numSegSection + 1);
				manual->index(verticeIndex + 1);
				manual->index(verticeIndex);		
			}
			verticeIndex ++;
		}	

	manual->end();
	Ogre::MeshPtr mesh = manual->convertToMesh(name);
	float r = radius;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}