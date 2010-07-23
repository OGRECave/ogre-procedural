#include "ProceduralTorusGenerator.h"

namespace Procedural
{
Ogre::MeshPtr TorusGenerator::realizeMesh()
{
Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	Ogre::Real deltaSection = (Ogre::Math::TWO_PI / numSegSection);
	Ogre::Real deltaCircle = (Ogre::Math::TWO_PI / numSegCircle);

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
			manual->textureCoord(i/(Ogre::Real)numSegCircle*uTile, j/(Ogre::Real)numSegSection*vTile);

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
	Ogre::Real r = radius;
	mesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );

	mesh->_setBoundingSphereRadius(r);
       unsigned short src, dest;
	   if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
   	{
		mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
   	}
	return mesh;
}
/*Ogre::MeshPtr ProceduralPrimitiveFactory::createTorus(const Ogre::String& name, Ogre::Real radius, Ogre::Real sectionRadius, int numSegCircle, int numSegSection, Ogre::Real uTile, Ogre::Real vTile)
{
	
}*/
}