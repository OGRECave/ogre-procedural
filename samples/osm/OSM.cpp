/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010 Michael Broutin

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
#include "OSM.h"
#include "Procedural.h"

#include "rapidxml.hpp"

using namespace Ogre;
using namespace rapidxml;
using namespace std;
using namespace Procedural;

//-------------------------------------------------------------------------------------
void Sample_OSM::createScene(void)
{
	rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("OSMRootNode");

	generateStreetsFromOSMXMLFile("sample-data.osm.xml");

}

void Sample_OSM::createCamera(void)
{
	BaseApplication::createCamera();
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowFarDistance(100.0);
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setAmbientLight(ColourValue::White);
	// Setup camera and light
	mCamera->setPosition(911,332,-176);
	mCamera->lookAt(493,0,387);
	// Slow down speed, as the scene is small
	mCameraMan->setTopSpeed(20);

	Light* l = mSceneMgr->createLight("myLight");
	l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(Vector3(0,-1,1).normalisedCopy());
	l->setDiffuseColour(ColourValue(.7f,.5f,.5f));
	l->setSpecularColour(ColourValue::White);

	movingLight = mSceneMgr->createLight("movingLight");
	movingLight->setType(Light::LT_POINT);
	movingLight->setDiffuseColour(ColourValue(.5f,.5f,.7f));
	movingLight->setSpecularColour(ColourValue::White);
	movingLight->setPosition(mCamera->getPosition());
	movingLight->setCastShadows(false);
}

void Sample_OSM::generateStreets(nodeMap &nodes, wayVector &ways)
{
	printf("generating meshes ...\n");
	///PROCEDURAL PART
	//Iterate through the nodes referenced by 'way' and extrude them to their correct position.
	Procedural::Shape shapes[WAY_END];

	shapes[WAY_OTHER] = Shape().addPoint(1,0).addPoint(-1,0).addPoint(-1,-0.3).addPoint(1,-0.3).close();


	shapes[WAY_RESIDENTIAL] = Shape().addPoint(0.5,0).addPoint(-0.5,0).addPoint(-0.5,-0.3).addPoint(0.5,-0.3).close();
	// with sidewalk
	/*
	//   Schemata:
	//   3---4      7---8
	//   |   5------6   |
	//   2--------------1
	//
	// TODO: this crashes, FIX!
	shapes[WAY_RESIDENTIAL] = Shape()
			.addPoint(1,0)       // 1
			.addPoint(-1,0)      // 2
			.addPoint(-1,-0.5)   // 3
			.addPoint(-0.5,-0.5) // 4
			.addPoint(-0.5,-0.1) // 5
			.addPoint(0.5,-0.1)  // 6
			.addPoint(0.5,-0.5)  // 7
			.addPoint(1,-0.5)    // 8
			.close();
	*/

	shapes[WAY_SECONDARY] = Shape().addPoint(0.8,0).addPoint(-0.8,0).addPoint(-0.8,-0.3).addPoint(0.8,-0.3).close();
	shapes[WAY_PRIMARY]   = Shape().addPoint(1,0).addPoint(-1,0).addPoint(-1,-0.3).addPoint(1,-0.3).close();
	shapes[WAY_MOTORWAY]  = Shape().addPoint(1,0).addPoint(-1,0).addPoint(-1,-0.3).addPoint(1,-0.3).close();

	for (size_t g=0;g < ways.size();g++)
	{
		int wayType = ways[g].type;
		String wayName = "way_" + StringConverter::toString(ways[g].id) + "_type" + StringConverter::toString(wayType);
		String materialName = "osm_sample/type_" + StringConverter::toString(wayType);

		// create the ways spline
		Procedural::CatmullRomSpline3 p;
		p.setNumSeg(3);
		for (size_t i=0;i < ways[g].ref.size();i++)
		{
			nodeMap::iterator n = nodes.find(ways[g].ref[i]);
			if(n != nodes.end())
			{
				Vector3 &pos = n->second.pos;


				// bit cheating: if its a motorway, we raise it a bit
				//if(wayType == WAY_MOTORWAY)
				//	pos.y = 10.0f;
				// looks bad in the end as the input data is partly screwed

				p.addPoint(pos);
			}
		}

		// generate a path out of it
		Procedural::Path path = p.realizePath();

		// and create the mesh with the fitting shape
		MeshPtr mesh = Procedural::Extruder().setExtrusionPath(&path).setShapeToExtrude(&shapes[wayType]).realizeMesh(wayName);

		// now add some LOD
		p.simplyfy(4);
		p.setNumSeg(1);
		path = p.realizePath();
		MeshPtr mesh_lod1 = Procedural::Extruder().setExtrusionPath(&path).setShapeToExtrude(&shapes[wayType]).realizeMesh(wayName + "_LOD1");
		mesh->createManualLodLevel(5, wayName + "_LOD1");

		// and put the mesh into the scene
		putMesh(wayName, materialName);
	}
	printf("generating meshes done\n");
}

bool Sample_OSM::frameStarted(const FrameEvent& evt)
{
	movingLight->setPosition(mCamera->getPosition());
	return true;
}

int Sample_OSM::loadOSMXMLFromString(Ogre::String &xml_str, nodeMap &nodes, wayVector &ways)
{
	// make a safe-to-modify copy of input_xml
	// (you should never modify the contents of an std::string directly)
	std::vector<char> xml_copy(xml_str.begin(), xml_str.end());
	xml_copy.push_back('\0');

	// only use xml_copy from here on!
	xml_document<> doc;
	// we are choosing to parse the XML declaration
	// parse_no_data_nodes prevents RapidXML from using the somewhat surprising
	// behavior of having both values and data nodes, and having data nodes take
	// precedence over values when printing
	// >>> note that this will skip parsing of CDATA nodes <<<
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);


	xml_node<> *osm_node = doc.first_node("osm");
	xml_node<> *bounds_node = osm_node->first_node("bounds");

	// figure out the bounds
	float minlat = Ogre::Math::Abs(atof(bounds_node->first_attribute("minlat")->value()));
	float maxlat = Ogre::Math::Abs(atof(bounds_node->first_attribute("maxlat")->value()));
	float minlon = Ogre::Math::Abs(atof(bounds_node->first_attribute("minlon")->value()));
	float maxlon = Ogre::Math::Abs(atof(bounds_node->first_attribute("maxlon")->value()));
	maxlat -= minlat;
	maxlon -= minlon;
	printf("bounds: %f-%f, %f-%f\n", minlat, maxlat, minlon, maxlon);

	// then walk the nodes
	xml_node<> *nodes_node = osm_node->first_node("node");

	while(nodes_node)
	{
		float longtitude = Ogre::Math::Abs(atof(nodes_node->first_attribute("lon")->value()));
		float latitude   = Ogre::Math::Abs(atof(nodes_node->first_attribute("lat")->value()));
		longtitude        = ((longtitude - minlon) / maxlon) * 1000.0f;
		latitude          = ((latitude   - minlat) / maxlat) * 1000.0f;

		int node_id = atoi(nodes_node->first_attribute("id")->value());

		nodes[node_id].pos = Ogre::Vector3(longtitude , 0, latitude);
		printf("added Node %d\n", node_id);
		nodes_node = nodes_node->next_sibling("node");
	}

	// now the ways
	printf("processing ways ...\n");
	xml_node<> *ways_node = osm_node->first_node("way");
	while(ways_node)
	{
		// create storage object
		way_t w;
		w.id = atoi(ways_node->first_attribute("id")->value());
		w.type = WAY_OTHER;

		printf("added way %d\n", w.id);

		// now look for the highway tag
		xml_node<> *tag_node = ways_node->first_node("tag");
		while(tag_node)
		{
			if(!strcmp(tag_node->first_attribute("k")->value(), "highway"))
			{
				// found the highway tag
				String typeValue = String(tag_node->first_attribute("v")->value());
				if       (typeValue == "residential") w.type = WAY_RESIDENTIAL;
				else if (typeValue == "secondary")   w.type = WAY_SECONDARY;
				else if (typeValue == "primary")     w.type = WAY_PRIMARY;
				else if (typeValue == "motorway")    w.type = WAY_MOTORWAY;
			}

			tag_node = tag_node->next_sibling("tag");
		}

		// now figure out the referenced nodes
		xml_node<> *ref_node = ways_node->first_node("nd");
		while(ref_node)
		{
			int n = atoi(ref_node->first_attribute("ref")->value());
			w.ref.push_back(n);
			ref_node = ref_node->next_sibling("nd");
		}

		// done, add way to vector
		if(w.id)
			ways.push_back(w);

		// get next way
		ways_node = ways_node->next_sibling("way");
	}

	printf("parsing done\n");
	return 0;
}

void Sample_OSM::generateStreetsFromOSMXMLFile(Ogre::String filename)
{
	String xml_str = loadXMLFile(filename);

	// data holding structures
	nodeMap nodes;
	wayVector ways;
	// load the data from the XML file into the structures
	loadOSMXMLFromString(xml_str, nodes, ways);
	// and generate the meshes
	generateStreets(nodes, ways);
}

void Sample_OSM::putMesh(const std::string& meshName, const std::string& material, const Ogre::Vector3& position)
{
	Ogre::Entity* ent = mSceneMgr->createEntity(meshName);
	Ogre::SceneNode* sn = rootNode->createChildSceneNode(meshName);
	sn->attachObject(ent);
	sn->setPosition(position);
	ent->setMaterialName(material);
	// set material for all LODs as well
	for(int i=0;i<ent->getNumManualLodLevels(); i++)
		ent->getManualLodLevel(i)->setMaterialName(material);
}

String Sample_OSM::loadXMLFile(Ogre::String filename, Ogre::String group)
{
	String xml_str = String();
	{
		// try to open the file, will throw on error
		DataStreamPtr ds = ResourceGroupManager::getSingleton().openResource(filename, group);
		// read it into the string
		xml_str = ds->getAsString();
	}
	return xml_str;
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
		Sample_OSM app;

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
