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
#include "ProceduralShapeGenerators.h"

#include <Ogre.h>
#include "rapidxml.hpp"

using namespace Ogre;
using namespace rapidxml;

namespace Procedural
{
//-----------------------------------------------------------------------
Shape CubicHermiteSpline2::realizeShape()
{
Shape shape;

	unsigned int numPoints = mClosed ? mPoints.size() : (mPoints.size() - 1);
	for (unsigned int i = 0; i < numPoints; ++i)
	{
		const Ogre::Vector2& p0 = mPoints[i].position;
		const Ogre::Vector2& m0 = mPoints[i].tangentAfter;
		const Ogre::Vector2& p1 = safeGetPoint(i+1).position;
		const Ogre::Vector2& m1 = safeGetPoint(i+1).tangentBefore;

		for (unsigned int j = 0; j < mNumSeg; ++j)
		{
			Ogre::Real t = (Ogre::Real)j/(Ogre::Real)mNumSeg;
			Ogre::Real t2 = t*t;
			Ogre::Real t3 = t2*t;
			Ogre::Vector2 P = (2*t3-3*t2+1)*p0+(t3-2*t2+t)*m0+(-2*t3+3*t2)*p1+(t3-t2)*m1;
			shape.addPoint(P);
		}
		if (i == mPoints.size() - 2 && !mClosed)
		{
			shape.addPoint(p1);
		}
	}
	if (mClosed)
		shape.close();
	shape.setOutSide(mOutSide);

	return shape;
}
int CubicHermiteSpline2::loadFromSVG(Ogre::String filename, Ogre::String shapeName, bool moveToCenter, Ogre::String group)
{
	String xml_str = String();
	{
		// try to open the file, will throw on error
		DataStreamPtr ds = ResourceGroupManager::getSingleton().openResource(filename, group);
		// read it into the string
		xml_str = ds->getAsString();
	}

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
	xml_node<> *svg_node = doc.first_node("svg");
	if(!svg_node)
	{
		Utils::log("No SVG node in file found.");
		return 1;
	}

	xml_node<> *g_node = svg_node->first_node("g");
	if(!g_node)
	{
		Utils::log("No G node in SVG found.");
		return 1;
	}

	xml_node<> *path_node = g_node->first_node("path");
	if(!path_node)
	{
		Utils::log("No path node in graphics found.");
		return 1;
	}

	while(path_node)
	{
		xml_attribute<> *atr = path_node->first_attribute("d");
		if(!atr)
		{
			Utils::log("Wrong path format, 'd' not found");
			return 1;
		}

		String path_id = "";
		if(path_node->first_attribute("id")) path_id = String(path_node->first_attribute("id")->value());

		// is this the path we want?
		if(shapeName.empty() || shapeName == path_id)
		{
			// actually parse the path
			String path_def = String(atr->value());

			// clear all existing points
			mPoints.clear();

			printf("found path %s\n", path_id.c_str());

			// now the fun begins ;)
			StringVector args = Ogre::StringUtil::split(path_def, " ");

			int state = 0;

			// walk the args
			Vector2 lp = Vector2::ZERO; // last point
			for(int i = 0; i < args.size(); i++)
			{
				String &arg = args[i];
				if(state == 0 && arg != "m" && arg != "M")
				{
					continue;
				} else if(state == 0 && arg == "m" || arg == "M")
				{
					state = 1;
				} else if(state  == 1)
				{
					StringVector args2 = Ogre::StringUtil::split(arg, ",");
					if(args2.size() == 2)
					{
						lp = Vector2(atof(args2[0].c_str()), atof(args2[1].c_str()));
					} else if (arg == "c" || arg == "C")
					{
						state = 2;
					}
					continue;
				} else if(state == 2)
				{
					StringVector args2 = Ogre::StringUtil::split(arg, ",");
					if(args2.size() == 2)
					{
						Vector2 p = Vector2(atof(args2[0].c_str()), atof(args2[1].c_str()));

						addPoint(lp, Vector2::ZERO, p);

						lp = p;
					}
				}
			}

			printf("DONE !!\n");
			break;
		}
		// try the next path
		path_node = path_node->next_sibling("path");
	}
	// seems we are done :)
	return 0;
}
//-----------------------------------------------------------------------
Shape CatmullRomSpline2::realizeShape()
	{
		Shape shape;

		unsigned int numPoints = mClosed ? mPoints.size() : (mPoints.size() - 1);
		for (unsigned int i = 0; i < numPoints; ++i)
		{			
			const Ogre::Vector2& P1 = safeGetPoint(i-1);
			const Ogre::Vector2& P2 = safeGetPoint(i);
			const Ogre::Vector2& P3 = safeGetPoint(i+1);
			const Ogre::Vector2& P4 = safeGetPoint(i+2);

			for (unsigned int j = 0; j < mNumSeg; ++j)
			{				
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)mNumSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t*t2;
				Ogre::Vector2 P = 0.5f*((-t3+2.f*t2-t)*P1 + (3.f*t3-5.f*t2+2.f)*P2 + (-3.f*t3+4.f*t2+t)*P3 + (t3-t2)*P4);
				shape.addPoint(P);
			}
			if (i == mPoints.size() - 2 && !mClosed)
			{
				shape.addPoint(P3);
			}

		}
		if (mClosed)
			shape.close();
		shape.setOutSide(mOutSide);

		return shape;
	}
//-----------------------------------------------------------------------
	Shape KochanekBartelsSpline2::realizeShape()	
	{
		Shape shape;
		
		unsigned int numPoints = mClosed ? mPoints.size() : (mPoints.size() - 1);
		for (unsigned int i = 0; i < numPoints; ++i)
		{
			const ControlPoint& pm1 = safeGetPoint(i-1);
			const ControlPoint& p0 = safeGetPoint(i);
			const ControlPoint& p1 = safeGetPoint(i+1);
			const ControlPoint& p2 = safeGetPoint(i+2);
			
			Ogre::Vector2 m0 = (1-p0.tension)*(1+p0.bias)*(1+p0.continuity)/2.f*(p0.position-pm1.position)+(1-p0.tension)*(1-p0.bias)*(1-p0.continuity)/2.f*(p1.position-p0.position);
			Ogre::Vector2 m1 = (1-p1.tension)*(1+p1.bias)*(1-p1.continuity)/2.f*(p1.position-p0.position)+(1-p1.tension)*(1-p1.bias)*(1+p1.continuity)/2.f*(p2.position-p1.position);
			
			for (unsigned int j = 0;j < mNumSeg; ++j)
			{
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)mNumSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t2*t;
				Ogre::Vector2 P = (2*t3-3*t2+1)*p0.position+(t3-2*t2+t)*m0+(-2*t3+3*t2)*p1.position+(t3-t2)*m1;
				shape.addPoint(P);
			}
			if (i == mPoints.size() - 2 && !mClosed)
			{
				shape.addPoint(p1.position);
			}
		}
		if (mClosed)
			shape.close();
		shape.setOutSide(mOutSide);
		return shape;
	}
}
