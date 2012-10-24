/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2011 Michael Broutin

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
#include "Ogre.h"
#include "Illustrations.h"
#include "Procedural.h"
#include <iostream>
#include <fstream>

using namespace Procedural;

//-------------------------------------------------------------------------------------
	void Illustrations::init()
	{	
	
	String resourcesCfg, pluginsCfg;
	#ifdef _DEBUG
	resourcesCfg = "resources_d.cfg";
	pluginsCfg = "plugins_d.cfg";
	#else
	resourcesCfg = "resources.cfg";
	pluginsCfg = "plugins.cfg";
	#endif

	mRoot = new Ogre::Root(pluginsCfg); 
  
	ConfigFile cf;
	cf.load(resourcesCfg);
 
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
	/*
	if (!mRoot->restoreConfig())
		mRoot->showConfigDialog();*/
	const RenderSystemList& rsList = mRoot->getAvailableRenderers();
	RenderSystem* rs = *rsList.begin();
	for (RenderSystemList::const_iterator it=rsList.begin();it!=rsList.end();it++)
	{
		if ((*it)->getName().find("GL") != String::npos)
			rs = *it;
	}

	ConfigOptionMap optionMap = rs->getConfigOptions();
	String fsaaSetting = optionMap["FSAA"].possibleValues.back();
	rs->setConfigOption("FSAA", fsaaSetting);

	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");	
	
	mRoot->setRenderSystem(rs);	
		 
	mWindow = mRoot->initialise(true); 
	mWindow->setDeactivateOnFocusChange(false);
	mWindow->resize(256, 256);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);  
	mCamera = mSceneMgr->createCamera("SimpleCamera");  
	mViewPort = mWindow->addViewport(mCamera);
	mViewPort->setBackgroundColour(ColourValue::White);
	mCamera->setAspectRatio(1.);
	cameraPerspective();
	mCamera->setNearClipDistance(1.);
	mSceneMgr->setAmbientLight(ColourValue(0.5f,0.3f,0.1f));
	Light* light = mSceneMgr->createLight();
	light->setType(Light::LT_DIRECTIONAL);
	light->setDiffuseColour(ColourValue::White);
	light->setDirection(Vector3(-1,-1,-1).normalisedCopy());

	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
    if (mRaySceneQuery == NULL)
		return;
    mRaySceneQuery->setSortByDistance(true);
	mRenderWindowPixelBox = new PixelBox (mWindow->getWidth(), mWindow->getHeight(), 1, PF_R8G8B8);
	mRenderWindowPixelBox->data = new BYTE[mRenderWindowPixelBox->getConsecutiveSize()];
}

void Illustrations::GetMeshInformation(Entity *entity,
									size_t &vertex_count,
									Ogre::Vector3* &vertices,
									size_t &index_count,
									Ogre::uint32* &indices,
									const Ogre::Vector3 &position,
									const Ogre::Quaternion &orient,
									const Ogre::Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;
	vertex_count = index_count = 0;
 
	Ogre::MeshPtr mesh = entity->getMesh();
 
 
	bool useSoftwareBlendingVertices = entity->hasSkeleton();
 
	if (useSoftwareBlendingVertices)
	{
	  entity->_updateAnimation();
	}
 
	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );
 
		// We only need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if( !added_shared )
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}
 
		// Add the indices
		index_count += submesh->indexData->indexCount;
	}
 
 
	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	indices = new Ogre::uint32[index_count];
 
	added_shared = false;
 
	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
 
		//----------------------------------------------------------------
		// GET VERTEXDATA
		//----------------------------------------------------------------
		Ogre::VertexData* vertex_data;
 
		//When there is animation:
		if(useSoftwareBlendingVertices)
			vertex_data = submesh->useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData();
		else
			vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
 
 
		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}
 
			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
			unsigned char* vertex =
				static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
			// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			//  as second argument. So make it float, to avoid trouble when Ogre::Real will
			//  be comiled/typedefed as double:
			//      Ogre::Real* pReal;
			float* pReal;
 
			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);
 
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
 
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}
 
			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}
 
 
		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
		void* hwBuf = ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY);
 
		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
		size_t index_start = index_data->indexStart;
		size_t last_index = numTris*3 + index_start;
 
		if (use32bitindexes) {
			Ogre::uint32* hwBuf32 = static_cast<Ogre::uint32*>(hwBuf);
			for (size_t k = index_start; k < last_index; ++k)
			{
				indices[index_offset++] = hwBuf32[k] + static_cast<Ogre::uint32>( offset );
			}
		} else {
			Ogre::uint16* hwBuf16 = static_cast<Ogre::uint16*>(hwBuf);
			for (size_t k = index_start; k < last_index; ++k)
			{
				indices[ index_offset++ ] = static_cast<Ogre::uint32>( hwBuf16[k] ) +
					static_cast<Ogre::uint32>( offset );
			}
		}
 
		ibuf->unlock();
		current_offset = next_offset;
	}
}

void Illustrations::next(std::string name, Real size, Shape* pShape1, Shape* pShape2, Path* pPath)
{
	// Optimise camera placing
	Real distance = 2*size/Math::Tan(mCamera->getFOVy());
	mCamera->setPosition(distance * mCamera->getPosition().normalisedCopy());

	// Write scene to png image
	mRoot->renderOneFrame();
	mRoot->renderOneFrame();
	mRoot->renderOneFrame();
	mWindow->writeContentsToFile(name + ".png");

	// Create SVG
	unsigned int height = mWindow->getHeight();
	unsigned int width = mWindow->getWidth();
	Real fw = 0.5f * width;
	Real fh = -0.5f * height;
	Real tw = (Real)width;
	Real th = (Real)height;
	std::vector<SVGPATH> pathList;
	std::ofstream svgfile;
	if(pShape1 == NULL && pShape2 == NULL && pPath == NULL)
	{
		// Render
		Real dx = 1.0f / (Real)width;
		Real dy = 1.0f / (Real)height;
		mWindow->copyContentsToMemory(*mRenderWindowPixelBox);
		Ogre::Image img;
		img.loadDynamicImage(static_cast<Ogre::uchar*>(mRenderWindowPixelBox->data),width, height, PF_R8G8B8);
		img.flipAroundX();

		Ray ray;
		RaySceneQueryResult query_result;
		Real closest_distance = -1.0f;
		size_t vertex_count;
		size_t index_count;
		Vector3 *vertices=0;
		Vector3 eyeSpacePos[3];
		uint32 *indices=0;

		for(Real x = 0.0f; x <= 1.0f; x += dx)
			for(Real y = 0.0f; y <= 1.0f; y += dy)
			{
				mCamera->getCameraToViewportRay(x, y, &ray);
				mRaySceneQuery->setRay(ray);
				if (mRaySceneQuery->execute().size() <= 0) continue;

				SVGPATH closest_result;
				closest_result.lineWidth = 0.5f;
				closest_result.closed = true;
				closest_distance = -1.0f;
				query_result = mRaySceneQuery->getLastResults();

				for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
				{
					if ((closest_distance >= 0.0f) && (closest_distance < query_result[qr_idx].distance))
						break;
 
					if ((query_result[qr_idx].movable != NULL) && (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
					{
						Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);           
 
						if (!indices && !vertices)
						GetMeshInformation( pentity, vertex_count, vertices, index_count, indices,
									   pentity->getParentNode()->_getDerivedPosition(),
									   pentity->getParentNode()->_getDerivedOrientation(),
									   pentity->getParentNode()->_getDerivedScale());

						bool new_closest_found = false;
						for (int i = 0; i < static_cast<int>(index_count); i += 3)
						{
							std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], true, false);
 
							if (hit.first)
							{
								if ((closest_distance < 0.0f) || (hit.second < closest_distance))
								{
									closest_distance = hit.second;
									closest_result.points.clear();
									for(int j = 0; j < 3; j++)
									{
										eyeSpacePos[j] = mCamera->getViewMatrix(true) * vertices[indices[i+j]];
										Vector3 p = mCamera->getProjectionMatrix() * eyeSpacePos[j];
										closest_result.points.push_back(Vector2(p.x * fw, p.y * fh));
									}
									closest_result.distance = closest_distance;
									
									// Bugfix needed
									// closest_result.color = img.getColourAt((size_t)(x * (Real)width), (size_t)(x * (Real)width), 0);
									closest_result.color = ColourValue::White;

									// Check is result is in front of the camera
									new_closest_found = (eyeSpacePos[0].z < 0.0f && eyeSpacePos[1].z < 0.0f && eyeSpacePos[2].z < 0.0f && closest_result.color.r > 0.4f && closest_result.color.g > 0.4f && closest_result.color.b > 0.4f);
								}
							}
						}				
 
						if (new_closest_found && closest_distance >= 0.0f)
						{
							bool inlist = false;
							for(std::vector<SVGPATH>::iterator iter = pathList.begin(); iter != pathList.end(); iter++)
								if((*iter).points[0] == closest_result.points[0] && (*iter).points[1] == closest_result.points[1] && (*iter).points[2] == closest_result.points[2])
								{
									inlist = true;
									break;
								}
							if(!inlist)
							{
								pathList.push_back(closest_result);
								for(int j = 0; j < 3; j++)
								{
									tw = std::min(tw, closest_result.points[j].x);
									th = std::min(th, closest_result.points[j].y);
								}
							}
						}
					}
				}
			}

		delete[] vertices;
		delete[] indices;
		std::sort(pathList.begin(), pathList.end());
		tw = -1.0f * tw + 15.0f; // Create a translation in x direction with a border
		th = -1.0f * th + 15.0f; // Create a translation in y direction with a border
	}
	else
	{
		SVGPATH sp;
		sp.lineWidth = 1.0f;
		sp.distance = 0.0f;
		sp.color = ColourValue::White;
		if(pPath != NULL)
		{
			// Path
			std::vector<Vector3> pl = pPath->getPoints();
			std::vector<Vector3> pl1;
			for(std::vector<Vector3>::iterator iter = pl.begin(); iter != pl.end(); iter++)
			{
				Vector3 eyeSpacePos = mCamera->getViewMatrix(true) * (*iter);
				Vector3 screenSpacePos = mCamera->getProjectionMatrix() * eyeSpacePos;
				screenSpacePos.x *= fw;
				screenSpacePos.y = screenSpacePos.z * fh;
				tw = std::min(tw, screenSpacePos.x);
				th = std::min(th, screenSpacePos.y);
				sp.points.push_back(Vector2(screenSpacePos.x, screenSpacePos.y));
			}
			tw = -1.0f * tw + 15.0f;
			th = -1.0f * th + 15.0f;
		}
		else
		{
			// Shape(s)
			tw = 0.0f;
			th = 0.0f;
			Real fMin = -5.0f;
			Real fMax = 5.0f;
			fw = (Real)width / (fMax - fMin);
			fh = (Real)height / (fMax - fMin);

			sp.points.clear();
			std::vector<Vector2> pl = pShape1->getPoints();
			for(std::vector<Vector2>::iterator iter = pl.begin(); iter != pl.end(); iter++)
				sp.points.push_back(Vector2((iter->x + fMin * -1.0f) * fw, (fMax - iter->y) * fh));
			sp.closed = pShape1->isClosed();
			pathList.push_back(sp);

			if(pShape2 != NULL)
			{
				sp.points.clear();
				std::vector<Vector2> pl = pShape2->getPoints();
				for(std::vector<Vector2>::iterator iter = pl.begin(); iter != pl.end(); iter++)
					sp.points.push_back(Vector2((iter->x + fMin * -1.0f) * fw, (fMax - iter->y) * fh));
				sp.closed = pShape2->isClosed();
				pathList.push_back(sp);
			}
		}
	}
	if(pathList.size() > 0)
	{
		svgfile.open(name + ".svg");
		svgfile << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
		svgfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height << "\">" << std::endl;
		svgfile << "  <g style=\"stroke:" << ((pShape1 == NULL && pShape2 == NULL && pPath == NULL) ? "black" : "red") << ";stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\">" << std::endl;
		char color[8];
		for(std::vector<SVGPATH>::iterator iter = pathList.begin(); iter != pathList.end(); iter++)
		{
#if _MSC_VER > 1310
			sprintf_s(color, 8,
#else
			sprintf(color,
#endif
				"#%02x%02x%02x", (BYTE)(iter->color.r * 255.0f), (BYTE)(iter->color.g * 255.0f), (BYTE)(iter->color.b * 255.0f));
			svgfile << "    <path style=\"fill:" << color << ";stroke-width:" << iter->lineWidth << "px\" d=\"M";
			for(std::vector<Vector2>::iterator pt = iter->points.begin(); pt != iter->points.end(); pt++)
				svgfile << " " << (pt->x + tw) << "," << (pt->y + th);
			if(iter->closed) svgfile << " z";
			svgfile << "\" />" << std::endl;
		}
		svgfile << "  </g>" << std::endl;
		svgfile << "</svg>" << std::endl;
	}

	// Clear the scene
	for (std::vector<SceneNode*>::iterator it = mSceneNodes.begin(); it != mSceneNodes.end(); it++) 
	{
		(*it)->detachAllObjects();
		mSceneMgr->destroySceneNode(*it);
	}
	for (std::vector<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); it++)
	{
		MeshManager::getSingletonPtr()->remove((*it)->getMesh()->getName());			
		mSceneMgr->destroyEntity(*it);
	}		
	mEntities.clear();
	mSceneNodes.clear();
}

void Illustrations::putMesh(MeshPtr mesh, int materialIndex)
{
	Entity* ent = mSceneMgr->createEntity(mesh->getName());
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent);
	if (materialIndex==0)
		ent->setMaterialName("HiddenLine");
	else if (materialIndex==1)
		ent->setMaterialName("RedLine");
	else if (materialIndex==2)
		ent->setMaterialName("Examples/Road");
	mEntities.push_back(ent);
	mSceneNodes.push_back(sn);
}

void Illustrations::go()
{	
	//
	// Primitives
	//

	MeshPtr mp;
	mp = BoxGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_box", 1.1f);

	mp = RoundedBoxGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_roundedbox", 1.3f);

	mp = SphereGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_sphere", 1.4f);

	mp = IcoSphereGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_icosphere", 1.4f);

	mp = TorusGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_torus", 1.6f);

	mp = TorusKnotGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_torusknot", 1.6f);

	mp = CylinderGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_cylinder", 1.8f);

	mp = ConeGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_cone", 1.4f);

	mp = TubeGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_tube", 3);

	mp = CapsuleGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_capsule", 2);

	mCamera->setPosition(mCamera->getPosition() + Vector3(0.0f, 1.5f, 0.0f));
	mp = SpringGenerator().setNumRound(3).realizeMesh();
	putMesh(mp);
	next("primitive_spring", 3);

	//
	// Operations on shapes and splines
	//

	cameraBack();

	Shape s = CatmullRomSpline2().addPoint(0,0).addPoint(1,0).addPoint(1,1).addPoint(2,1).addPoint(2,0).addPoint(3,0).addPoint(3,1).addPoint(4,1).realizeShape().translate(-2, 0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("spline_catmull", 3, &s);

	s = CubicHermiteSpline2().addPoint(Vector2(0,0), AT_CATMULL).addPoint(Vector2(1,0), AT_CATMULL).addPoint(Vector2(1,1), Vector2(0,2), Vector2(0,-2)).addPoint(Vector2(2,1), AT_CATMULL).addPoint(2,0).addPoint(3,0).addPoint(3,1).addPoint(4,1).setNumSeg(16).realizeShape().translate(-2,0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("spline_cubichermite", 3, &s);

	s = KochanekBartelsSpline2().addPoint(Vector2(0,0)).addPoint(Vector2(1,0),1,0,0).addPoint(Vector2(1,1),-1,0,0).addPoint(Vector2(2,1),0,1,0).addPoint(Vector2(2,0),0,-1,0).addPoint(Vector2(3,0),0,0,1).addPoint(Vector2(3,1),0,0,-1).addPoint(Vector2(4,1)).addPoint(Vector2(4,0)).realizeShape().translate(-2,0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("spline_kochanekbartels", 3, &s);

	s = RoundedCornerSpline2().addPoint(Vector2(0,0)).addPoint(Vector2(1,0)).addPoint(Vector2(1,1)).addPoint(Vector2(2,1)).addPoint(Vector2(2,0)).addPoint(Vector2(3,0)).addPoint(Vector2(3,1)).addPoint(Vector2(4,1)).addPoint(Vector2(4,0)).setRadius(0.3f).realizeShape().translate(-2,0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("spline_roundedcorner", 3, &s);

	s = BezierCurve2().addPoint(Vector2(0,0)).addPoint(Vector2(1,0)).addPoint(Vector2(1,1)).addPoint(Vector2(2,1)).addPoint(Vector2(2,0)).addPoint(Vector2(3,0)).addPoint(Vector2(3,1)).addPoint(Vector2(4,1)).addPoint(Vector2(4,0)).realizeShape().translate(-2,0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("spline_beziercurve", 3, &s);

	cameraPerspective();
	Path p = HelixPath().setNumSegPath(64).setNumRound(3).setHeight(1.5f).realizePath().translate(0.0f, -2.2f, 0.0f);
	mp = p.realizeMesh();
	putMesh(mp,1);
	next("spline_helix", 3, NULL, NULL, &p);

	//
	// Boolean operations
	//
	cameraBack();
	{
	Shape s1 = RectangleShape().realizeShape();
	Shape s2 = s1;
	s2.translate(.5f,.5f);

	putMesh(s1.realizeMesh(), 1);
	putMesh(s2.realizeMesh(), 1);
	next("shape_booleansetup", 1.5, &s1, &s2);

	s = s1.booleanUnion(s2).getShape(0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("shape_booleanunion", 1.5f, &s);

	s = s1.booleanIntersect(s2).getShape(0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("shape_booleanintersection", 1.5f, &s);

	s = s1.booleanDifference(s2).getShape(0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("shape_booleandifference", 1.5f, &s);
	}

	//
	// Thicken
	//
	{
	s = Shape().addPoint(-1,-1).addPoint(0.5,0).addPoint(-0.5,0).addPoint(1,1);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("shape_thick1", 1.5f, &s);

	s = s.thicken(.2f).getShape(0);
	mp = s.realizeMesh();
	putMesh(mp,1);
	next("shape_thick2", 1.5f, &s);
	}

	//
	// Delaunay
	//
	{
		cameraFront();
		MultiShape ms;
		CircleShape cs;
		ms.addShape(cs.setRadius(2).realizeShape());
		ms.addShape(cs.setRadius(.3f).realizeShape().translate(-1,.3f).switchSide());
		ms.addShape(cs.realizeShape().translate(1,.3f).switchSide());
		ms.addShape(cs.realizeShape().switchSide());
		ms.addShape(cs.realizeShape().scale(2,1).translate(0,-1).switchSide());
		mp = Triangulator().setMultiShapeToTriangulate(&ms).realizeMesh();
		putMesh(mp);
		next("shape_triangulation", 3);
	}

	//
	// Extrusion
	//
	{
		cameraPerspective();
		//Shape s = RoundedCornerSpline2().addPoint(-1,0).addPoint(0,1).addPoint(1,0).addPoint(0,2).close().realizeShape();
		//Shape s = CircleShape().realizeShape();
		Shape s = Shape().addPoint(-1,-1).addPoint(1,-1).addPoint(1,1).addPoint(0,0).addPoint(-1,1).close();
		Path p = RoundedCornerSpline3().addPoint(-6,2.5,-2.5).addPoint(-5,0,-2.5).addPoint(0,0,2.5).addPoint(5,0,-2.5).setRadius(1.).realizePath();
		mp = Extruder().setShapeToExtrude(&s).setExtrusionPath(&p).realizeMesh();
		putMesh(mp);
		next("extruder_generic", 10);

		Shape s2 = RectangleShape().setHeight(.5).realizeShape();
		Track t = Track(Track::AM_RELATIVE_LINEIC).addKeyFrame(0,0).addKeyFrame(1.0,-1.0);
		Path p2 = LinePath().betweenPoints(Vector3(-5,0,0),Vector3(5,0,0)).setNumSeg(10).realizePath();
		mp = Extruder().setShapeToExtrude(&s2).setExtrusionPath(&p2).setRotationTrack(&t).realizeMesh();
		putMesh(mp);
		next("extruder_rotationtrack", 7);

		t = Track(Track::AM_RELATIVE_LINEIC).addKeyFrame(0,.5f).addKeyFrame(.4f,.5f).addKeyFrame(.5f,1.2f).addKeyFrame(.8f,1).addKeyFrame(1.0f,1);
		mp = Extruder().setShapeToExtrude(&s2).setExtrusionPath(&p2).setScaleTrack(&t).realizeMesh();
		putMesh(mp);
		next("extruder_scaletrack", 7);

		Procedural::Shape s4 = Procedural::Shape().addPoint(-1.2f,.2f).addPoint(-1.f,.2f).addPoint(-.9f,.1f).addPoint(.9f,.1f).addPoint(1.f,.2f).addPoint(1.2f,.2f).scale(2).setOutSide(Procedural::SIDE_LEFT);
		Procedural::Track textureTrack = Procedural::Track(Procedural::Track::AM_POINT).addKeyFrame(0,0).addKeyFrame(2,.2f).addKeyFrame(3,.8f).addKeyFrame(5,1);
		mp = Extruder().setShapeTextureTrack(&textureTrack).setShapeToExtrude(&s4).setExtrusionPath(&p2).setCapped(false).realizeMesh();
		putMesh(mp, 2);
		next("extruder_texturetrack", 7);

		cameraFront();
		Shape s3 = CircleShape().setNumSeg(16).realizeShape();
		MultiShape ms = MultiShape(2, &s3.switchSide(), &Shape(s3).scale(1.1f));
		Path p3 = CatmullRomSpline3().addPoint(0,0,-5).addPoint(0,0,0).addPoint(1,-1,5).realizePath();		
		mp = Extruder().setMultiShapeToExtrude(&ms).setExtrusionPath(&p3).realizeMesh();
		putMesh(mp);
		next("extruder_multishape", 4);	
	}

	//
	// Lathe
	//
	{
		Shape s = Shape().addPoint(0,-3).addPoint(1,-3).addPoint(1,0).addPoint(.8f,1).addPoint(.8f,2).addPoint(1.5f,3).addPoint(0,4);
		mp = Lathe().setShapeToExtrude(&s).realizeMesh();
		putMesh(mp);
		next("lathe_generic",5);

		mp = Lathe().setShapeToExtrude(&s).setAngleBegin((Radian)2).setAngleEnd((Radian)0).realizeMesh();
		putMesh(mp);
		next("lathe_anglerange",5);
	}
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Illustrations app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif