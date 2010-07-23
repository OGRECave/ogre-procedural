#include "ProceduralConeGenerator.h"

namespace Procedural
{
Ogre::MeshPtr ConeGenerator::realizeMesh()
{
	Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	int verticeIndex=0;
	for (int i = 0; i <=numSegHeight; i++)
		for (int j = 0; j<=numSegBase; j++)
		{
			Ogre::Real r0 = radius * (1 - i / (Ogre::Real)numSegHeight);
			Ogre::Real x0 = r0 * cosf(j*deltaAngle);
			Ogre::Real z0 = r0 * sinf(j*deltaAngle);
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

		//low cap
		int centerIndex = verticeIndex;
		manual->position(0,0,0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(0.0,vTile);
		verticeIndex++;
		for (int j=0;j<=numSegBase;j++)
		{
			Ogre::Real x0 = radius * cosf(j*deltaAngle);
			Ogre::Real z0 = radius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);
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

/*Ogre::MeshPtr ProceduralPrimitiveFactory::createCone(const Ogre::String& name, Ogre::Real radius, Ogre::Real height, int numSegHeight, int numSegBase, Ogre::Real uTile, Ogre::Real vTile)
{

}*/
}