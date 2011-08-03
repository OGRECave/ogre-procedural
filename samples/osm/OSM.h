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
#ifndef __Sample_OSM_h_
#define __Sample_OSM_h_

#include "BaseApplication.h"
using namespace Ogre;

class Sample_OSM : public BaseApplication
{
	Light* movingLight;
	virtual bool frameStarted(const FrameEvent& evt);
protected:
	virtual void createScene(void);

	void putMesh(const std::string& meshName, const std::string& material, const Ogre::Vector3& position = Ogre::Vector3::ZERO);

	virtual void createCamera(void);

	struct way_t {
		int id;
		int type;
		std::vector<int> ref;
	};

	struct node_t {
		Ogre::Vector3 pos;
	};

	enum {WAY_OTHER=0, WAY_RESIDENTIAL, WAY_SECONDARY, WAY_PRIMARY, WAY_MOTORWAY, WAY_END};

	typedef std::map<int, node_t> nodeMap;
	typedef std::vector<way_t> wayVector;

	void generateStreets(nodeMap &nodes, wayVector &ways);

	void generateStreetsFromOSMXMLFile(Ogre::String filename);
	int loadOSMXMLFromString(Ogre::String &xml_str, nodeMap &nodes, wayVector &ways);

	Ogre::String loadXMLFile(Ogre::String filename, Ogre::String group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::SceneNode* rootNode;


};

#endif // #ifndef __Sample_OSM_h_
