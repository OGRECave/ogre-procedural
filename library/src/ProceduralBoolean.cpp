#include "ProceduralStableHeaders.h"
#include "ProceduralBoolean.h"
#include "ProceduralGeometryHelpers.h"

using namespace Ogre;

namespace Procedural
{
void Boolean::addToTriangleBuffer(TriangleBuffer& buffer) const
{
	const std::vector<TriangleBuffer::Vertex>& vec1 = mMesh1->getVertices();
	const std::vector<int>& ind1 = mMesh1->getIndices();
	const std::vector<TriangleBuffer::Vertex>& vec2 = mMesh2->getVertices();
	const std::vector<int>& ind2 = mMesh2->getIndices();
	Segment3D intersectionResult;

	std::vector<Segment3D> intersectionList;
	
	// Find all intersections between mMesh1 and mMesh2
	for (std::vector<int>::const_iterator it = ind1.begin(); it != ind1.end();)
	{
		Triangle3D t1(vec1[*it++].mPosition,vec1[*it++].mPosition,vec1[*it++].mPosition);

		for (std::vector<int>::const_iterator it2 = ind2.begin(); it2 != ind2.end();)
		{
			int i1=*it2;
			it2++;
			int i2=*it2;
			it2++;
			int i3=*it2;
			it2++;

			Triangle3D t2(vec2[i1].mPosition,vec2[i2].mPosition,vec2[i3].mPosition);

			if (t1.findIntersect(t2, intersectionResult))
			{
				intersectionList.push_back(intersectionResult);
			}		
		}		
	}

	Ogre::SceneManager *smgr = Ogre::Root::getSingleton().getSceneManagerIterator().begin()->second;
	ManualObject * manual = smgr->createManualObject();
	manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
	for (std::vector<Segment3D>::iterator it = intersectionList.begin(); it!=intersectionList.end();it++)
	{
		manual->position(it->mA);
		manual->position(it->mB);
	}	

	manual->end();
	manual->convertToMesh("toto");
	smgr->destroyManualObject(manual);
	
	Utils::log("number of intersect : " + StringConverter::toString(intersectionList.size()));
	// Triangulate

	// Trace contours

	// Build resulting mesh
}
}