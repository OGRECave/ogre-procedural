#include "ProceduralTubeGenerator.h"

namespace Procedural {
Ogre::MeshPtr TubeGenerator::realizeMesh()
{
Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	int verticeIndex=0;
	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			Ogre::Real x0 = outerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = outerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);

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
			Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(-Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);

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
			Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,vTile);

			x0 = outerRadius * cosf(j*deltaAngle);
			z0 = outerRadius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);			
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);

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
			Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);

			x0 = outerRadius * cosf(j*deltaAngle);
			z0 = outerRadius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);			
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,vTile);

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
	Ogre::Real r = height;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}
/*Ogre::MeshPtr ProceduralPrimitiveFactory::createTube(const Ogre::String& name, Ogre::Real innerRadius, Ogre::Real outerRadius, Ogre::Real height, int numSegHeight, int numSegBase, Ogre::Real uTile, Ogre::Real vTile)
{
	
}*/
}