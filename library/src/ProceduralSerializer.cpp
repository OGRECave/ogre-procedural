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
#include "ProceduralStableHeaders.h"
#include "ProceduralSerializer.h"
#include "ProceduralUtils.h"

#include "ProceduralPathGenerators.h"
#include "ProceduralShapeGenerators.h"
#include "ProceduralExtruder.h"

#include "rapidxml.hpp"

#include <vector>

using namespace Ogre;
using namespace rapidxml;
using namespace std;

namespace Procedural
{

	Serializer::Serializer()
	{
	}

	Serializer::~Serializer(void)
	{
	}

	int Serializer::loadXMLFile(Ogre::String filename, Ogre::String group)
	{
		String xml_str = String();
		{
			// try to open the file, will throw on error
			DataStreamPtr ds = ResourceGroupManager::getSingleton().openResource(filename, group);
			// read it into the string
			xml_str = ds->getAsString();
		}
		return loadXMLFromString(xml_str);
	}
	
	int Serializer::loadXMLFromString(Ogre::String &xml_str)
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

		// since we have parsed the XML declaration, it is the first node
		// (otherwise the first node would be our root node)
		string encoding = doc.first_node()->first_attribute("encoding")->value();
		if(encoding != "utf-8")
		{
			Utils::log("XML malformed, no utf-8 xml header");
			return 1;
		}

		// we didn't keep track of our previous traversal, so let's start again
		// we can match nodes by name, skipping the xml declaration entirely
		xml_node<>* cur_node = doc.first_node("proceduralmesh");
		if(!cur_node)
		{
			Utils::log("XML malformed, no proceduralmesh root node");
			return 1;
		}

		// walk the nodes
		for (xml_node<> *n = cur_node->first_node(); n; n = n->next_sibling())
		{
			// read the name
			String name = "unnamed";
			if(n->first_attribute("name"))
				name = String(n->first_attribute("name")->value());
			
			// and realize attribute if existing
			int realize = 0;
			if(n->first_attribute("realize"))
				realize = StringConverter::parseInt(n->first_attribute("realize")->value());

			//Utils::log("Node name: " + String(n->name()));
			if(!strcmp(n->name(), "CatmullRomSpline3"))
			{
				CatmullRomSpline3 p;
				// walk its nodes
				for (xml_node<> *pn = n->first_node(); pn; pn = pn->next_sibling())
				{
					if        (!strcmp(pn->name(), "point"))
					{
						p.addPoint(StringConverter::parseVector3(pn->value()));
					} else if (!strcmp(pn->name(), "asPath"))
					{
						// add the spline to the paths map
						Path path = p.realizePath();

						int realize2 = 0;
						if(pn->first_attribute("realize"))
							realize2 = StringConverter::parseInt(pn->first_attribute("realize")->value());

						paths[pn->value()] = serializationStorage<Path>(path, realize2);
						if(realize2) realizeables[pn->value()] = dynamic_cast<MeshGeneratorInterface *>(&paths[pn->value()].obj);
					}
				}
				// add to spline map
				splines[name] = serializationStorage<CatmullRomSpline3>(p, realize);
				// splines are not mesh-able
			}
			else if(!strcmp(n->name(), "CircleShape"))
			{
				Real radius = 1.0f;
				int segments = 8;
				if(n->first_attribute("radius"))
					radius   = StringConverter::parseReal(n->first_attribute("radius")->value());
				if(n->first_attribute("segments"))
					segments = StringConverter::parseInt(n->first_attribute("segments")->value());

				Shape s = CircleShape().setRadius(radius).setNumSeg(segments).realizeShape();

				// add to shape map
				shapes[name] = serializationStorage<Shape>(s, realize);
				if(realize) realizeables[name] = dynamic_cast<MeshGeneratorInterface *>(&shapes[name].obj);
			}
			else if(!strcmp(n->name(), "Extruder"))
			{
				Extruder e;
				for (xml_node<> *en = n->first_node(); en; en = en->next_sibling())
				{
					if      (!strcmp(en->name(), "ShapeToExtrude"))
					{
						e.setShapeToExtrude(&shapes[en->value()].obj);
					}
					else if (!strcmp(en->name(), "ExtrusionPath"))
					{
						e.setExtrusionPath(&paths[en->value()].obj);
					}
					else if (!strcmp(en->name(), "fixSharpAngles"))
					{
						e.setFixSharpAngles(true);
					}
					
				}
				extruders[name] = serializationStorage<Extruder>(e, realize);
				if(realize) realizeables[name] = dynamic_cast<MeshGeneratorInterface *>(&extruders[name].obj);
			}
		}
		return 0;
	}


	Ogre::MeshPtr Serializer::realizeMesh(const std::string& meshName, const std::string& name, const Ogre::String& group)
	{
		std::map < Ogre::String, MeshGeneratorInterface * >::iterator it = realizeables.find(meshName);
		
		if(it == realizeables.end())
		{
			Utils::log("mesh "+meshName+" not in XML found");
			return MeshPtr(NULL);
		}

		return it->second->realizeMesh(name, group);
	}

	std::vector<Ogre::MeshPtr> Serializer::realizeAllMeshes(const Ogre::String& group)
	{
		std::vector<Ogre::MeshPtr> res;
		std::map < Ogre::String, MeshGeneratorInterface * >::iterator it;
		for(it = realizeables.begin(); it!=realizeables.end(); it++)
		{
			res.push_back(it->second->realizeMesh("", group));
		}
		return res;
	}
}
