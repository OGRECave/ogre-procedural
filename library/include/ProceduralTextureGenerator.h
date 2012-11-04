/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010-2012 Michael Broutin

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
#ifndef PROCEDURAL_TEXTURE_GENERATOR_INCLUDED
#define PROCEDURAL_TEXTURE_GENERATOR_INCLUDED

#include "ProceduralPlatform.h"
#include <Ogre.h>

namespace Procedural
{

/** \addtogroup texturegrp Textures
Elements for procedural texture creation.
@{
*/

class TextureBuffer;
//! Type for a TextureBuffer pointer
typedef TextureBuffer* TextureBufferPtr;


/**
\brief class to store image data while processing
\details Create a TextureBuffer object and move it to all classes inherited from TextureProcessing
\todo check byte order for image generation (OGRE_ENDIAN, OGRE_ENDIAN_LITTLE, OGRE_ENDIAN_BIG), see <a href="http://www.ogre3d.org/forums/viewtopic.php?f=2&t=72832" target="_blank">Ogre forum</a> for details.
*/
class _ProceduralExport TextureBuffer
{
private:
	Ogre::uchar* mPixels;
	Ogre::uint mWidth;
	Ogre::uint mHeight;

public:
	void setPixel(size_t x, size_t y, Ogre::ColourValue colour);
	void setPixel(size_t x, size_t y, Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);
	void setPixel(size_t x, size_t y, Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);
	void setRed(size_t x, size_t y, Ogre::uchar red);
	void setGreen(size_t x, size_t y, Ogre::uchar green);
	void setBlue(size_t x, size_t y, Ogre::uchar blue);
	void setAlpha(size_t x, size_t y, Ogre::uchar alpha);
	void setRed(size_t x, size_t y, Ogre::Real red);
	void setGreen(size_t x, size_t y, Ogre::Real green);
	void setBlue(size_t x, size_t y, Ogre::Real blue);
	void setAlpha(size_t x, size_t y, Ogre::Real alpha);
	void setData(size_t width, size_t height, Ogre::uchar* data);
	void setData(TextureBufferPtr buffer);
	Ogre::ColourValue getPixel(size_t x, size_t y);
	Ogre::uchar getPixelRedByte(size_t x, size_t y);
	Ogre::uchar getPixelGreenByte(size_t x, size_t y);
	Ogre::uchar getPixelBlueByte(size_t x, size_t y);
	Ogre::uchar getPixelAlphaByte(size_t x, size_t y);
	Ogre::Real getPixelRedReal(size_t x, size_t y);
	Ogre::Real getPixelGreenReal(size_t x, size_t y);
	Ogre::Real getPixelBlueReal(size_t x, size_t y);
	Ogre::Real getPixelAlphaReal(size_t x, size_t y);
	
	inline Ogre::uint getWidth() const
	{
		return mWidth;
	}

	inline Ogre::uint getHeight() const
	{
		return mHeight;
	}

	TextureBufferPtr clone();

public:
	/**
	\brief Standard constructor which copy a given image
	\param tocopy Image which to copy
	*/
	TextureBuffer(TextureBufferPtr tocopy);

	/**
	\brief Standard constructor which creates a quadratic image buffer with the given size
	\param width_height Edge length in px
	*/
	TextureBuffer(Ogre::uint width_height);

	/// Destructor which release memory
	~TextureBuffer();

	/// Get the width of the stored image in px
	Ogre::uint getWidth() { return mWidth; }

	/// Get the height of the stored image in px
	Ogre::uint getHeight() { return mHeight; }

	/// Create a new image from buffer.
	Ogre::Image* getImage();

	/**
	\brief Save the image from the buffer to a file.
	\param filename Name (and path) of the image file where to save the buffer.
	*/
	void saveImage(Ogre::String filename);

	/**
	\brief Creates an OGRE texture and add it to current TextureManager instance.
	\param name Name of the texture
	\param group Name of the resource group where to list the texture
	*/
	Ogre::TexturePtr createTexture(Ogre::String name, Ogre::String group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
};

/**
\brief base class for material generation classes.
*/
class _ProceduralExport TextureProcessing
{
protected:
	TextureBufferPtr mBuffer;
	Ogre::String mName;
	bool mLog;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	\param name Filter name
	*/
	TextureProcessing(TextureBufferPtr pBuffer, Ogre::String name);
	
	/** Run processing algorithmus */
	virtual TextureBufferPtr process() abstract;

	/** Get name of the filter as string. */
	Ogre::String getName();

	/**
	Enable/Disable logging.
	\param enable true enables logging of filter actions
	*/
	void setLog(bool enable = true);

protected:
	void logMsg(Ogre::String msg);
};

/** 
\brief Create a texture consisting of cells aligned in a grid, or a chessboard.
\details Cells can be irregular. If considered bump map, they have pyramid form.

Examples:

\b Default (MODE_GRID + PATTERN_BOTH)
\code{.cpp}
Procedural::TextureBuffer bufferCellDefault(256);
Procedural::Cell(&bufferCellDefault).setDensity(4).process();
\endcode
\image html texture_cell_default.png

\b MODE_CHESSBOARD + PATTERN_CONE
\code{.cpp}
Procedural::TextureBuffer bufferCellChessCone(256);
Procedural::Cell(&bufferCellChessCone).setDensity(4).setMode(Procedural::Cell::MODE_CHESSBOARD).setPattern(Procedural::Cell::PATTERN_CONE).process();
\endcode
\image html texture_cell_chess.png

\b MODE_GRID + PATTERN_CROSS
\code{.cpp}
Procedural::TextureBuffer bufferCellGridCross(256);
Procedural::Cell(&bufferCellGridCross).setDensity(4).setMode(Procedural::Cell::MODE_GRID).setPattern(Procedural::Cell::PATTERN_CROSS).process();
\endcode
\image html texture_cell_grid.png
*/
class _ProceduralExport Cell : public TextureProcessing
{
public:
	//! Mode how to paint cells
	enum CELL_MODE
	{
		MODE_GRID,		//!< Paint cells on a grid
		MODE_CHESSBOARD	//!< Paint cells on a chessboard
	};

	//! Mode how to construct cells
	enum CELL_PATTERN
	{
		PATTERN_BOTH,	//!< PATTERN_CROSS | PATTERN_CONE
		PATTERN_CROSS,	//!< Construct cells from vertices
		PATTERN_CONE	//!< Construct cells from cicles
	};

private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;
	Ogre::uint mRegularity;
	Ogre::uint mDensity;
	CELL_MODE mMode;
	CELL_PATTERN mPattern;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Cell(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Cell"), mColour(Ogre::ColourValue::White), mRegularity(128), mDensity(8), mMode(MODE_GRID), mPattern(PATTERN_BOTH), mSeed(5120)
	{
	}

	/**
	Set the colour of the cell top.
	\param colour New colour of the cell top (default Ogre::ColourValue::White)
	*/
	Cell & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the cell top.
	\param red Red value of the cell top colour [0, 255] (default 255)
	\param green Green value of the cell top colour [0, 255] (default 255)
	\param blue Blue value of the cell top colour [0, 255] (default 255)
	\param alpha %Alpha value of the cell top colour [0, 255] (default 255)
	*/
	Cell & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour of the cell top.
	\param red Red value of the cell top colour [0.0, 1.0] \(default 1.0)
	\param green Green value of the cell top colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of the cell top colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of the cell top colour [0.0, 1.0] \(default 1.0)
	*/
	Cell & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Cell & setSeed(Ogre::uint seed);

	/**
	Set the regularity of texture.

	The maximum value of 255 creates identical cells. The minimum 0 creates random forms for each cells.
	\param regularity New value for chaotic cell forms (default 128)
	*/
	Cell & setRegularity(Ogre::uchar regularity);

	/**
	Set the density of cells in texture.

	At least you have to set number of rows and columns in the grid to 1 or above.
	\param density New number of columns and rows (default 8)
	*/
	Cell & setDensity(Ogre::uint density);

	/**
	Set the cell mode of texture.
	\param mode New mode for cell ground (default MODE_GRID)
	*/
	Cell & setMode(CELL_MODE mode);

	/**
	Set the cell pattern of texture.
	\param pattern New base of cell construction (default PATTERN_BOTH)
	*/
	Cell & setPattern(CELL_PATTERN pattern);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Creates a cloud structured image.
\details Creates a cloud structure from a specified perlin noise on a coloured background.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferCloud(256);
Procedural::Cloud(&bufferCloud).process();
\endcode
\image html texture_cloud.png
*/
class _ProceduralExport Cloud : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Cloud(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Cloud"), mColour(Ogre::ColourValue::White), mSeed(5120)
	{
	}

	/**
	Set the colour of the background.
	\param colour New colour for background (default Ogre::ColourValue::White)
	*/
	Cloud & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the background.
	\param red Red value of background colour [0, 255] (default 255)
	\param green Green value of background colour [0, 255] (default 255)
	\param blue Blue value of background colour [0, 255] (default 255)
	\param alpha %Alpha value of background colour [0, 255] (default 255)
	*/
	Cloud & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Sets the colour of the background
	\param red Red value of background colour [0.0, 1.0] \(default 1.0)
	\param green Green value of background colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of background colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of background colour [0.0, 1.0] \(default 1.0)
	*/
	Cloud & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Cloud & setSeed(Ogre::uint seed);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Fills full image with given colour gradients.
\details Each corner of the image has unique color.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();
\endcode
\image html texture_gradient.png
*/
class _ProceduralExport Gradient : public TextureProcessing
{
private:
	Ogre::ColourValue mColourA;
	Ogre::ColourValue mColourB;
	Ogre::ColourValue mColourC;
	Ogre::ColourValue mColourD;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Gradient(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Gradient"), mColourA(Ogre::ColourValue::Blue), mColourB(Ogre::ColourValue::Green), mColourC(Ogre::ColourValue::Red), mColourD(Ogre::ColourValue(0.0f, 1.0f, 1.0f))
	{
	}

	/**
	Set the colour in the top left corner of the image.
	\param colour New colour in the top left corner for processing (default Ogre::ColourValue::Blue)
	*/
	Gradient & setColourA(Ogre::ColourValue colour);

	/**
	Set the colour in the top left corner of the image.
	\param red Red value of new colour in the top left corner [0, 255] (default 0)
	\param green Green value of new colour in the top left corner [0, 255] (default 0)
	\param blue Blue value of new colour in the top left corner [0, 255] (default 255)
	\param alpha %Alpha value of new colour in the top left corner [0, 255] (default 255)
	*/
	Gradient & setColourA(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour in the top left corner of the image.
	\param red Red value of new colour in the top left corner [0.0, 1.0] \(default 0.0)
	\param green Green value of new colour in the top left corner [0.0, 1.0] \(default 0.0)
	\param blue Blue value of new colour in the top left corner [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of new colour in the top left corner [0.0, 1.0] \(default 1.0)
	*/
	Gradient & setColourA(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the colour in the top right corner of the image.
	\param colour New colour in the top right corner for processing (default Ogre::ColourValue::Green)
	*/
	Gradient & setColourB(Ogre::ColourValue colour);

	/**
	Set the colour in the top right corner of the image.
	\param red Red value of new colour in the top right corner [0, 255] (default 0)
	\param green Green value of new colour in the top right corner [0, 255] (default 255)
	\param blue Blue value of new colour in the top right corner [0, 255] (default 0)
	\param alpha %Alpha value of new colour in the top right corner [0, 255] (default 255)
	*/
	Gradient & setColourB(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour in the top right corner of the image.
	\param red Red value of new colour in the top right corner [0.0, 1.0] \(default 0.0)
	\param green Green value of new colour in the top right corner [0.0, 1.0] \(default 1.0)
	\param blue Blue value of new colour in the top right corner [0.0, 1.0] \(default 0.0)
	\param alpha %Alpha value of new colour in the top right corner [0.0, 1.0] \(default 1.0)
	*/
	Gradient & setColourB(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the colour in the bottom left corner of the image.
	\param colour New colour in the bottom left corner for processing (default Ogre::ColourValue::Red)
	*/
	Gradient & setColourC(Ogre::ColourValue colour);

	/**
	Set the colour in the bottom left corner of the image.
	\param red Red value of new colour in the bottom left corner [0, 255] (default 255)
	\param green Green value of new colour in the bottom left corner [0, 255] (default 0)
	\param blue Blue value of new colour in the bottom left corner [0, 255] (default 0)
	\param alpha %Alpha value of new colour in the bottom left corner [0, 255] (default 255)
	*/
	Gradient & setColourC(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour in the bottom left corner of the image.
	\param red Red value of new colour in the bottom left corner [0.0, 1.0] \(default 1.0)
	\param green Green value of new colour in the bottom left corner [0.0, 1.0] \(default 0.0)
	\param blue Blue value of new colour in the bottom left corner [0.0, 1.0] \(default 0.0)
	\param alpha %Alpha value of new colour in the bottom left corner [0.0, 1.0] \(default 1.0)
	*/
	Gradient & setColourC(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the colour in the bottom right corner of the image.
	\param colour New colour in the bottom right corner for processing (default Ogre::ColourValue(0.0f, 1.0f, 1.0f))
	*/
	Gradient & setColourD(Ogre::ColourValue colour);

	/**
	Set the colour in the bottom right corner of the image.
	\param red Red value of new colour in the bottom right corner [0, 255] (default 0)
	\param green Green value of new colour in the bottom right corner [0, 255] (default 255)
	\param blue Blue value of new colour in the bottom right corner [0, 255] (default 255)
	\param alpha %Alpha value of new colour in the bottom right corner [0, 255] (default 255)
	*/
	Gradient & setColourD(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour in the bottom right corner of the image.
	\param red Red value of new colour in the bottom right corner [0.0, 1.0] \(default 0.0)
	\param green Green value of new colour in the bottom right corner [0.0, 1.0] \(default 1.0)
	\param blue Blue value of new colour in the bottom right corner [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of new colour in the bottom right corner [0.0, 1.0] \(default 1.0)
	*/
	Gradient & setColourD(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Sets the colours of the image corners.
	\param colourA New colour in the top left corner (default Ogre::ColourValue::Blue)
	\param colourB New colour in the top right corner (default Ogre::ColourValue::Green)
	\param colourC New colour in the bottom left corner (default Ogre::ColourValue::Red)
	\param colourD New colour in the bottom right corner (default Ogre::ColourValue(0.0f, 1.0f, 1.0f))
	*/
	Gradient & setColours(Ogre::ColourValue colourA, Ogre::ColourValue colourB, Ogre::ColourValue colourC, Ogre::ColourValue colourD);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Load an image from a resource.
\details Try to load an image from a resource.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
\endcode
\image html texture_image.png
*/
class _ProceduralExport Image : public TextureProcessing
{
private:
	Ogre::String mFile;
	Ogre::String mGroup;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Image(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Image")
	{
	}

	/**
	Set the colour of the background.
	\param filename Name of an image file to load.
	\param groupname Name of the resource group to search for the image
	*/
	Image & setFile(Ogre::String filename, Ogre::String groupname = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Creates a labyrinth structured image.
\details Creates a labyrinth structure from a specified perlin noise on a coloured background.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferLabyrinth(256);
Procedural::Labyrinth(&bufferLabyrinth).process();
\endcode
\image html texture_labyrinth.png
*/
class _ProceduralExport Labyrinth : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Labyrinth(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Labyrinth"), mColour(Ogre::ColourValue::White), mSeed(5120)
	{
	}

	/**
	Set the colour of the background.
	\param colour New colour for background (default Ogre::ColourValue::White)
	*/
	Labyrinth & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the background.
	\param red Red value of background colour [0, 255] (default 255)
	\param green Green value of background colour [0, 255] (default 255)
	\param blue Blue value of background colour [0, 255] (default 255)
	\param alpha %Alpha value of background colour [0, 255] (default 255)
	*/
	Labyrinth & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Sets the colour of the background
	\param red Red value of background colour [0.0, 1.0] \(default 1.0)
	\param green Green value of background colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of background colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of background colour [0.0, 1.0] \(default 1.0)
	*/
	Labyrinth & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Labyrinth & setSeed(Ogre::uint seed);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Creates a marble structured image.
\details Creates a marbel structure from a specified perlin noise on a coloured background.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferMarble(256);
Procedural::Marble(&bufferMarble).process();
\endcode
\image html texture_marble.png
*/
class _ProceduralExport Marble : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Marble(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Marble"), mColour(Ogre::ColourValue::White), mSeed(5120)
	{
	}

	/**
	Set the colour of the background.
	\param colour New colour for marble structure (default Ogre::ColourValue::White)
	*/
	Marble & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the background.
	\param red Red value of the marble structure colour [0, 255] (default 255)
	\param green Green value of the marble structure colour [0, 255] (default 255)
	\param blue Blue value of the marble structure colour [0, 255] (default 255)
	\param alpha %Alpha value of the marble structure colour [0, 255] (default 255)
	*/
	Marble & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour of the background.
	\param red Red value of the marble structure colour [0.0, 1.0] \(default 1.0)
	\param green Green value of the marble structure colour [0.0, 1.0] \(default 1.0)
	\param blue Blue valu of the marble structure coloure [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of the marble structure colour [0.0, 1.0] \(default 1.0)
	*/
	Marble & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Marble & setSeed(Ogre::uint seed);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Fills full image with noise in a given colour.
\details High quality noise with various noise algorithms.

Examples:
\b White noise (default)
\code{.cpp}
Procedural::TextureBuffer bufferNoiseWhite(256);
Procedural::Noise(&bufferNoiseWhite).setType(Procedural::Noise::NOISE_WHITE).process();
\endcode
\image html texture_noise_white.png

\b Perlin noise
\code{.cpp}
Procedural::TextureBuffer bufferNoisePerlin(256);
Procedural::Noise(&bufferNoisePerlin).setType(Procedural::Noise::NOISE_PERLIN).process();
\endcode
\image html texture_noise_perlin.png
*/
class _ProceduralExport Noise : public TextureProcessing
{
public:
	//! Noise generator type
	enum NOISE_TYPE
	{
		NOISE_WHITE,	//!< White noise
		NOISE_PERLIN	//!< Perlin noise
	};

private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;
	NOISE_TYPE mType;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Noise(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Noise"), mColour(Ogre::ColourValue::White), mSeed(5120), mType(NOISE_WHITE)
	{
	}

	/**
	Set the colour of the noise.
	\param colour New colour of the noise (default Ogre::ColourValue::White)
	*/
	Noise & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the noise.
	\param red Red value of the noise colour [0, 255] (default 255)
	\param green Green value of the noise colour [0, 255] (default 255)
	\param blue Blue value of the noise colour [0, 255] (default 255)
	\param alpha %Alpha value of the noise colour [0, 255] (default 255)
	*/
	Noise & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour of the noise.
	\param red Red value of the noise colour [0.0, 1.0] \(default 1.0)
	\param green Green value of the noise colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of the noise colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of the noise colour [0.0, 1.0] \(default 1.0)
	*/
	Noise & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Noise & setSeed(Ogre::uint seed);

	/**
	Set the type of noise generation.
	\param type Type of noise generator (default NOISE_WHITE)
	*/
	Noise & setType(NOISE_TYPE type);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Fills full image with given colour.
\details Set all pixel to the same color.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferSolid(256);
Procedural::Solid(&bufferSolid).setColour(Ogre::ColourValue(0.0f, 0.5f, 1.0f, 1.0f)).process();
\endcode
\image html texture_solid.png
*/
class _ProceduralExport Solid : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Solid(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Solid"), mColour(Ogre::ColourValue::Black)
	{
	}

	/**
	Sets the fill colour of the image.
	\param colour New colour for processing (default Ogre::ColourValue::Black)
	*/
	Solid & setColour(Ogre::ColourValue colour);

	/**
	Sets the fill colour of the image.
	\param red Red value [0, 255] (default 0)
	\param green Green value [0, 255] (default 0)
	\param blue Blue value [0, 255] (default 0)
	\param alpha %Alpha value [0, 255] (default 255)
	*/
	Solid & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Sets the fill colour of the image.
	\param red Red value [0.0, 1.0] \(default 0.0)
	\param green Green value [0.0, 1.0] \(default 0.0)
	\param blue Blue value [0.0, 1.0] \(default 0.0)
	\param alpha %Alpha value [0.0, 1.0] \(default 1.0)
	*/
	Solid & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Creates a textile structured image.
\details Creates a textile structure from a specified perlin noise on a coloured background.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferTextile(256);
Procedural::Textile(&bufferTextile).process();
\endcode
\image html texture_textile.png
*/
class _ProceduralExport Textile : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Textile(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Textile"), mColour(Ogre::ColourValue::White), mSeed(5120)
	{
	}

	/**
	Set the colour of the background.
	\param colour New colour for background (default Ogre::ColourValue::White)
	*/
	Textile & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the background.
	\param red Red value of background colour [0, 255] (default 255)
	\param green Green value of background colour [0, 255] (default 255)
	\param blue Blue value of background colour [0, 255] (default 255)
	\param alpha %Alpha value of background colour [0, 255] (default 255)
	*/
	Textile & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour of the background.
	\param red Red value of background colour [0.0, 1.0] \(default 1.0)
	\param green Green value of background colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of background colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of background colour [0.0, 1.0] \(default 1.0)
	*/
	Textile & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Textile & setSeed(Ogre::uint seed);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Creates a wood slice image.
\details Creates a structure of annual rings from a specified perlin noise on a coloured background.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferWood(256);
Procedural::Wood(&bufferWood).setRings(5).process();
\endcode
\image html texture_wood.png
*/
class _ProceduralExport Wood : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;
	Ogre::uint mRings;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to store the generated image.
	*/
	Wood(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Wood"), mColour(Ogre::ColourValue::White), mSeed(5120), mRings(8)
	{
	}

	/**
	Set the colour of the background.
	\param colour New colour for background (default Ogre::ColourValue::White)
	*/
	Wood & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the background.
	\param red Red value of background colour [0, 255] (default 255)
	\param green Green value of background colour [0, 255] (default 255)
	\param blue Blue value of background colour [0, 255] (default 255)
	\param alpha %Alpha value of background colour [0, 255] (default 255)
	*/
	Wood & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour of the background.
	\param red Red value of background colour [0.0, 1.0] \(default 1.0)
	\param green Green value of background colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of background colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of background colour [0.0, 1.0] \(default 1.0)
	*/
	Wood & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Wood & setSeed(Ogre::uint seed);

	/**
	Sets the number of annual rings.
	\param rings Number of annual rings (minimum 3, default 8)
	*/
	Wood & setRings(Ogre::uint rings);

	/**
	Run image generation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
\brief Use alpha channel as an mask for an other image.
\details Can take normal, height or quaternion map as second input.

<b>Map translations</b>
<ul><li>%Alpha channel: a(0, 255) -> angle(0, 2pi)</li>
<li>Coords: r(0,255) -> x(-1, 1)</li>
<li>Height map: luminance(0, 255) -> angle(0, 2pi) (axis is Z)</li>
<li>Normal map: is blended with the source</li></ul>

<b>Options</b>
<ul><li>Rotation: w(0, 1) -> angle(0, 2pi), rest are axis direction coordinates</li>
<li>Sensitivity: (0, 255) -> angle(0, 2pi) * map alpha</li>
<li>Compensation: How to deal with map</li>
<li>Mirror: For broken normal maps</li></ul>

Example:
\code{.cpp}
// Image colour
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();

// Image structure
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();

// Filter
Procedural::Abnormals(&bufferGradient).setParameterImage(&bufferCell).process();
\endcode
\dotfile texture_02.gv
\todo Need bugfix
*/
class _ProceduralExport Abnormals : public TextureProcessing
{
public:
	//! Methods how to work with parameter map
	enum ABNORMALS_COMPENSATION
	{
		COMPENSATION_NORMAL,	//!< Use as a normal map
		COMPENSATION_HEIGHT,	//!< Use as a height map
		COMPENSATION_QUATERNION	//!< Use as a list of quaternion
	};

	//! Methods how to fix broken normal maps
	enum ABNORMALS_MIRROR
	{
		MIRROR_NONE,	//!< None
		MIRROR_X_YZ,	//!< X : YZ
		MIRROR_Y_XZ,	//!< Y : XZ
		MIRROR_X_Y_Z	//!< X+Y : Z
	};

private:
	TextureBufferPtr mParam;
	Ogre::Radian mW;
	Ogre::Vector3 mAxis;
	Ogre::uchar mSensitivity;
	ABNORMALS_COMPENSATION mCompensation;
	ABNORMALS_MIRROR mMirror;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Abnormals(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Abnormals"), mParam(NULL), mW(0.0f), mAxis(0.0f, 0.0f, 1.0f), mSensitivity(127), mCompensation(COMPENSATION_NORMAL), mMirror(MIRROR_NONE)
	{
	}

	/**
	Set parameter image for compensation.
	\param image Pointer to second image (default NULL)
	\note If the parameter image is set to NULL there won't be any compensation.
	*/
	Abnormals & setParameterImage(TextureBufferPtr image);

	/**
	Set rotation angle.
	\param rotation New rotation angle [0.0, 1.0] \(default 0.0)
	*/
	Abnormals & setRotation(Ogre::Real rotation);

	/**
	Set rotation angle.
	\param rotation New rotation angle [0.0, Ogre::Math::TWO_PI] rad \(default 0.0)
	*/
	Abnormals & setRotation(Ogre::Radian rotation);

	/**
	Set rotation angle.
	\param rotation New rotation angle [0, 360] degree \(default 0)
	*/
	Abnormals & setRotation(Ogre::Degree rotation);

	/**
	Set rotation axis.
	\param axis New rotation axis (default Ogre::Vector3(0.0f, 0.0f, 1.0f))
	*/
	Abnormals & setAxis(Ogre::Vector3 axis);

	/**
	Set rotation axis.
	\param x New x coordinate of rotation axis \(default 0.0)
	\param y New y coordinate of rotation axis \(default 0.0)
	\param z New z coordinate of rotation axis \(default 1.0)
	*/
	Abnormals & setAxis(Ogre::Real x, Ogre::Real y, Ogre::Real z = 1.0f);

	/**
	Set sensitivity.
	\param sensitivity New sensitivity value [0, 255] (default 127)
	*/
	Abnormals & setSensitivity(Ogre::uchar sensitivity);

	/**
	Set compensation method.
	\param compensation Compensation method to use (default COMPENSATION_NORMAL)
	*/
	Abnormals & setCompensation(ABNORMALS_COMPENSATION compensation);

	/**
	Set mirror method.
	\param mirror Compensation method to use (default MIRROR_NONE)
	*/
	Abnormals & setMirror(ABNORMALS_MIRROR mirror);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Colour extraction.
\details Extract single color intensity, replace it with white smoke, discard the rest.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();
Procedural::Alpha(&bufferCell).process();
\endcode
\dotfile texture_03.gv
*/
class _ProceduralExport Alpha : public TextureProcessing
{
private:
	Ogre::ColourValue mExtractColour;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Alpha(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Alpha"), mExtractColour(Ogre::ColourValue::White)
	{
	}

	/**
	Set the colour to extract.
	\param colour New colour for extraction (default Ogre::ColourValue::White)
	*/
	Alpha & setExtractColour(Ogre::ColourValue colour);

	/**
	Set the colour to extract.
	\param red Red value of extraction colour [0, 255] (default 255)
	\param green Green value of extraction colour [0, 255] (default 255)
	\param blue Blue value of extraction colour [0, 255] (default 255)
	\param alpha %Alpha value of extraction colour [0, 255] (default 255)
	*/
	Alpha & setExtractColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour to extract.
	\param red Red value of extraction colour [0.0, 1.0] \(default 1.0)
	\param green Green value of extraction colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of extraction colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of extraction colour [0.0, 1.0] \(default 1.0)
	*/
	Alpha & setExtractColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Use alpha channel as an mask for an other image.
\details <ol><li>Extract alpha channel as an opaque monochrome bitmap or</li><li>multiply alpha channel with prameter image luminance, or</li><li>use parameter image as color alpha mask.</li></ol> The luminance is not taken into account.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();

Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();

Procedural::AlphaMask(&bufferGradient).setParameterImage(&bufferCell).process();
\endcode
\dotfile texture_04.gv
*/
class _ProceduralExport AlphaMask : public TextureProcessing
{
private:
	TextureBufferPtr mParam;
	bool mColourMask;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	AlphaMask(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "AlphaMask"), mColourMask(false), mParam(NULL)
	{
	}

	/**
	Set mode of alpha masking.
	\param colourmask If set to true parameter image will be used as alph mask (default false)
	*/
	AlphaMask & setColourAlphaMask(bool colourmask);

	/**
	Set parameter image for masking/colouring.
	\param image Pointer to second image (default NULL)
	\note Methode 1 is used if the parameter image is set to zero. If the size of the parameter image is smaller than the base buffer the operation will be canceled without any image manipulation.
	*/
	AlphaMask & setParameterImage(TextureBufferPtr image);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Reduce sharpness on input image.
\details Blurs the input image by a specified algorithm.

Examples:
\b BLUR_MEAN (default)
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Blur(&bufferImage).setType(Procedural::Blur::BLUR_MEAN).process();
\endcode
\dotfile texture_05a.gv

\b BLUR_GAUSSIAN
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Blur(&bufferImage).setType(Procedural::Blur::BLUR_GAUSSIAN).process();
\endcode
\dotfile texture_05b.gv
*/
class _ProceduralExport Blur : public TextureProcessing
{
public:
	//! List of algorithms to blur
	enum BLUR_TYPE
	{
		BLUR_BOX,		//!< Use simplified block filter to blur
		BLUR_MEAN,		//!< Use mean filter to blur
		BLUR_GAUSSIAN	//!< Use gaussian filter to blur
	};

private:
	Ogre::uchar mSize;
	Ogre::uchar mSigma;
	BLUR_TYPE mType;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Blur(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Blur"), mSize(5), mSigma(92), mType(BLUR_BOX)
	{
	}

	/**
	Set the gaussian block size.
	\param size New block size for gaussian blur filter [3, 255] (default 5)
	*/
	Blur & setSize(Ogre::uchar size);

	/**
	Set sigma constant for gaussian filter.
	\param sigma New sigma constant for gaussian blur filter [0, 255] (default 92)
	*/
	Blur & setSigma(Ogre::uchar sigma);

	/**
	Set the algorithm to blur.
	\param type New algorithm to blur (default BLUR_BOX)
	*/
	Blur & setType(BLUR_TYPE type);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Extract one channel or create gray image.
\details Reduce given image to a given channel or calculate gray image of it

Examples:
\b SELECT_BLUE
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Channel(&bufferImage).setSelection(Procedural::Channel::SELECT_BLUE).process();
\endcode
\dotfile texture_06a.gv

\b SELECT_GRAY
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Channel(&bufferImage).setSelection(Procedural::Channel::SELECT_GRAY).process();
\endcode
\dotfile texture_06b.gv
*/
class _ProceduralExport Channel : public TextureProcessing
{
public:
	enum CANNEL_SELECTION
	{
		SELECT_RED,
		SELECT_GREEN,
		SELECT_BLUE,
		SELECT_ALPHA,
		SELECT_GRAY 
	};

private:
	CANNEL_SELECTION mSelection;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Channel(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Channel"), mSelection(SELECT_GRAY)
	{
	}

	/**
	Set selection.
	\param selection Mode which channel should selected (default SELECT_GRAY)
	*/
	Channel & setSelection(CANNEL_SELECTION selection);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief A colour filter.
\details Adjust colour, contrast, brightness and alpha.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();
Procedural::Colours(&bufferGradient).setColourBase(Ogre::ColourValue::Red).setColourPercent(Ogre::ColourValue::Blue).process();
\endcode
\dotfile texture_07.gv
*/
class _ProceduralExport Colours : public TextureProcessing
{
private:
	Ogre::ColourValue mColourBase;
	Ogre::ColourValue mColourPercent;
	Ogre::uchar mBrithness;
	Ogre::uchar mContrast;
	Ogre::uchar mSaturation;
	Ogre::uchar mAlpha;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Colours(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Colours"), mColourBase(Ogre::ColourValue::Black), mColourPercent(Ogre::ColourValue::White), mBrithness(127), mContrast(127), mSaturation(127), mAlpha(127)
	{
	}

	/**
	Set the base colour to work on.
	\param colour New colour to work on (default Ogre::ColourValue::Black)
	*/
	Colours & setColourBase(Ogre::ColourValue colour);

	/**
	Set the base colour to work on.
	\param red Red value of base colour [0, 255] (default 0)
	\param green Green value of base colour [0, 255] (default 0)
	\param blue Blue value of base colour [0, 255] (default 0)
	\param alpha %Alpha value of base colour [0, 255] (default 255)
	*/
	Colours & setColourBase(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the base colour to work on.
	\param red Red value of base colour [0.0, 1.0] \(default 0.0)
	\param green Green value of base colour [0.0, 1.0] \(default 0.0)
	\param blue Blue value of base colour [0.0, 1.0] \(default 0.0)
	\param alpha %Alpha value of base colour [0.0, 1.0] \(default 0.0)
	*/
	Colours & setColourBase(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the percent colour to add on image.
	\param colour New colour to add (default Ogre::ColourValue::White)
	*/
	Colours & setColourPercent(Ogre::ColourValue colour);

	/**
	Set the percent colour to add on image.
	\param red Red value of percent colour [0, 255] (default 255)
	\param green Green value of percent colour [0, 255] (default 255)
	\param blue Blue value of percent colour [0, 255] (default 255)
	\param alpha %Alpha value of percent colour [0, 255] (default 255)
	*/
	Colours & setColourPercent(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the percent colour to add on image.
	\param red Red value of percent colour [0.0, 1.0] \(default 1.0)
	\param green Green value of percent colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of percent colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of percent colour [0.0, 1.0] \(default 1.0)
	*/
	Colours & setColourPercent(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set brithness of the image.
	\param brithness New image brithness (default 127)
	*/
	Colours & setBrithness(Ogre::uchar brithness);

	/**
	Set contrast of the image.
	\param contrast New image contrast (default 127)
	*/
	Colours & setContrast(Ogre::uchar contrast);

	/**
	Set saturation of the image.
	\param saturation New image saturation (default 127)
	*/
	Colours & setSaturation(Ogre::uchar saturation);
	
	/**
	Set alpha of the image.
	\param alpha New image alpha (default 127)
	*/
	Colours & setAlpha(Ogre::uchar alpha);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief %Combine inputs together.
\details Provides several ways of combining. Clamps bitmap to base input size.

Example:
\code{.cpp}
// Image input
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();

// Image structure
Procedural::TextureBuffer bufferCloud(256);
Procedural::Cloud(&bufferCloud).process();

// Filter
Procedural::Combine(&bufferCloud).addImage(&bufferGradient, Procedural::Combine::METHOD_ADD_CLAMP).process();
\endcode
\dotfile texture_08.gv
*/
class _ProceduralExport Combine : public TextureProcessing
{
public:
	//! Methods how to combine image
	enum COMBINE_METHOD
	{
		METHOD_ADD_CLAMP,
		METHOD_ADD_WRAP,
		METHOD_SUB_CLAMP,
		METHOD_SUB_WRAP,
		METHOD_MULTIPLY,
		METHOD_MULTIPLY2,
		METHOD_BLEND,
		METHOD_ALPHA,
		METHOD_LAYER
	};

private:
	struct LAYER
	{
		TextureBufferPtr image;
		COMBINE_METHOD action;
	};
	Ogre::ColourValue mColour;
	std::queue<LAYER> mQueue;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Combine(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Combine"), mColour(Ogre::ColourValue::White)
	{
	}

	/**
	Add image to process queue.
	\param image Pointer on image to process
	\param method Method how to process the image
	*/
	Combine & addImage(TextureBufferPtr image, COMBINE_METHOD method = METHOD_LAYER);

	/**
	Set the percent colour to add on image.
	\param colour New colour for drawing (default Ogre::ColourValue::White)
	*/
	Combine & setColour(Ogre::ColourValue colour);

	/**
	Set the percent colour to add on image.
	\param red Red value of drawing colour [0, 255] (default 255)
	\param green Green value of drawing colour [0, 255] (default 255)
	\param blue Blue value of drawing colour [0, 255] (default 255)
	\param alpha %Alpha value of drawing colour [0, 255] (default 255)
	*/
	Combine & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the percent colour to add on image.
	\param red Red value of drawing colour [0.0, 1.0] \(default 1.0)
	\param green Green value of drawing colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of drawing colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of drawing colour [0.0, 1.0] \(default 1.0)
	*/
	Combine & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();

private:
	void _process(TextureBufferPtr image, COMBINE_METHOD method);
};

/**
\brief %Convolution filter.
\details The filter calculates each pixel of the result image as weighted sum of the correspond pixel and its neighbors.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Convolution(&bufferImage).setKernel(
	Ogre::Matrix3(10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -10.0f)
	).process();
\endcode
\dotfile texture_09.gv

\note All kernels must be square matrices and larger than 2x2 (min. 3x3)!
*/
class _ProceduralExport Convolution : public TextureProcessing
{
private:
	Ogre::uchar mKernelSize;
	Ogre::Real* mKernelData;
	Ogre::Real mDivisor;
	Ogre::uchar mThreshold;
	bool mCalculateEdgeDivisor;
	bool mIncludeAlphaChannel;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Convolution(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Convolution"), mKernelSize(3), mDivisor(1.0f), mThreshold(128), mCalculateEdgeDivisor(true), mIncludeAlphaChannel(false)
	{
		int mid = 2;
		if((mKernelSize % 2) == 0)
			mid = mKernelSize / 2;
		else
			mid = (mKernelSize - 1) / 2 + 1;
		mKernelData = new Ogre::Real[mKernelSize * mKernelSize];
		memset(mKernelData, 0, mKernelSize * mKernelSize * sizeof(Ogre::Real));
		mKernelData[mKernelSize * mid + mid] = 1.0;
	}

	/**
	Default destructor to release memory.
	*/
	~Convolution()
	{
		delete mKernelData;
	}

	/**
	Set a new kernel.
	\param size Number of lines/rows of the quadratic kernel (default 3)
	\param data Array with data for new kernel by rows
	\remark setKernel calls calculateDivisor after changeing the kernel memory! If you like to set a user defined devisor call setDivisor always after setKernel!
	*/
	Convolution & setKernel(Ogre::uchar size, Ogre::Real* data);

	/**
	Set a new kernel.
	\param size Number of lines/rows of the quadratic kernel
	\param data Array with data for new kernel by rows
	\remark setKernel calls calculateDivisor after changeing the kernel memory! If you like to set a user defined devisor call setDivisor always after setKernel!
	*/
	Convolution & setKernel(Ogre::uchar size, int* data);

	/**
	Set a new kernel.
	\param data Matrix with data for new kernel
	\remark setKernel calls calculateDivisor after changeing the kernel memory! If you like to set a user defined devisor call setDivisor always after setKernel!
	*/
	Convolution & setKernel(Ogre::Matrix3 data);

	/**
	Set a devisor.
	\param divisor Set specific devisor \(default 1.0)
	\remark setKernel calls calculateDivisor after changeing the kernel memory! If you like to set a user defined devisor call setDivisor always after setKernel!
	*/
	Convolution & setDivisor(Ogre::Real divisor);

	/**
	Calculate a new devisor from given kernel.
	\remark setKernel calls calculateDivisor after changeing the kernel memory!
	*/
	Convolution & calculateDivisor();

	/**
	Set threshold value.
	\param threshold New threshold value [0, 255] (default 128)
	*/
	Convolution & setThreshold(Ogre::uchar threshold);

	/**
	Switch dynamic divisor for edges on or off.
	\param calculateedgedivisor Set true to use dynamic divisor for edges (default true)
	*/
	Convolution & setCalculateEdgeDivisor(bool calculateedgedivisor);

	/**
	Switch on/off the use of the alpha channel.
	\param usealpha Set true to also modify the alpha channel (default false)
	*/
	Convolution & setIncludeAlphaChannel(bool usealpha);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Create lines that can be randomly cracked or follow a normal map.
\details Number, length and variation of cracked lines can be set; high quality mode is available.

<b>Normal tracking</b><br />Lines are drawn where normals look at sides (r, g channels are set tosomething different than 127), and according the difference between alpha channel and normals elevation. By default, normals are rotated 90 degrees with X axis flipped, so that they "wrap" objects.

Example:
\code{.cpp}
// Image input
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();

// Image structure
Procedural::TextureBuffer bufferCloud(256);
Procedural::Cloud(&bufferCloud).process();

// Filter
Procedural::Crack(&bufferCloud).setParameterImage(&bufferGradient).process();
\endcode
\dotfile texture_10.gv
*/
class _ProceduralExport Crack : public TextureProcessing
{
public:
	//! Methods how to create line length
	enum CRACK_LENGTH_DECISION
	{
		LENGTH_DECISION_RANDOM,			//!< Use a random number generator
		LENGTH_DECISION_CONSTANT,		//!< Use a constant value
		LENGTH_DECISION_NORMAL_BASED	//!< Use a normal map
	};

	//! High quality settings
	enum CRACK_QUALITY
	{
		QUALITY_HIGH_OFF,	//!< Switch high quality off
		QUALITY_ALPHA,		//!< Use alpha channel
		QUALITY_SUBPIXEL	//!< Use sub pixel block
	};

private:
	TextureBufferPtr mParam;
	Ogre::ColourValue mColour;
	Ogre::uint mCount;
	Ogre::uchar mVariation;
	Ogre::uchar mLength;
	Ogre::uint mSeed;
	CRACK_LENGTH_DECISION mLengthDecision;
	CRACK_QUALITY mQuality;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Crack(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Crack"), mParam(NULL), mColour(Ogre::ColourValue::White), mCount(100), mVariation(64), mLength(255), mSeed(5120), mLengthDecision(LENGTH_DECISION_RANDOM), mQuality(QUALITY_HIGH_OFF)
	{
	}

	/**
	Set parameter image for normal mapping.
	\param image Pointer to second image (default NULL)
	\note If the parameter image is set to NULL there won't be any compensation.
	*/
	Crack & setParameterImage(TextureBufferPtr image);

	/**
	Set the colour to draw.
	\param colour New colour for drawing (default Ogre::ColourValue::White)
	*/
	Crack & setColour(Ogre::ColourValue colour);

	/**
	Set the colour to draw.
	\param red Red value of drawing colour [0, 255] (default 255)
	\param green Green value of drawing colour [0, 255] (default 255)
	\param blue Blue value of drawing colour [0, 255] (default 255)
	\param alpha %Alpha value of drawing colour [0, 255] (default 255)
	*/
	Crack & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour to draw.
	\param red Red value of drawing colour [0.0, 1.0] \(default 1.0)
	\param green Green value of drawing colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of drawing colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of drawing colour [0.0, 1.0] \(default 1.0)
	*/
	Crack & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the number of generated lines.
	\param count New number of lines to generate (default 100)
	*/
	Crack & setCount(Ogre::uint count);

	/**
	Set the smoothness of generated lines.
	\param variation New value for the smoothness of the generated lines (default 64)
	*/
	Crack & setVariation(Ogre::uchar variation);

	/**
	Set the minimum length of generated line segments.
	\param length New minimal length of the generated line segments (default 255)
	*/
	Crack & setLength(Ogre::uchar length);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Crack & setSeed(Ogre::uint seed);

	/**
	Set method for calculating the line segments length.
	\param lengthdecision New decision length (default LENGTH_DECISION_RANDOM)
	*/
	Crack & setLengthDecision(CRACK_LENGTH_DECISION lengthdecision);
	
	/**
	Set method for high quality painting.
	\param quality New high quality setting (default QUALITY_HIGH_OFF)
	*/
	Crack & setQuality(CRACK_QUALITY quality);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Expands bright areas over darker areas.
\details This filter dilate mid range area of the input image.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferCloud(256);
Procedural::Cloud(&bufferCloud).process();
Procedural::Dilate(&bufferCloud).process();
\endcode
\dotfile texture_11.gv
*/
class _ProceduralExport Dilate : public TextureProcessing
{
private:
	Ogre::uchar mIterations;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Dilate(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Dilate"), mIterations(10)
	{
	}

	/**
	Set number of iterations for dilating.
	\param iterations New number of dilating iterations [1, 255] (default 10)
	*/
	Dilate & setIterations(Ogre::uchar iterations);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Apply normal (ie. vector) map to a bitmap.
\details Example:
\code{.cpp}
// Image colour
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();

// Image structure
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();

// Filter
Procedural::Distort(&bufferGradient).setParameterImage(&bufferCell).setPower(255).process();
\endcode
\dotfile texture_12.gv
*/
class _ProceduralExport Distort : public TextureProcessing
{
private:
	TextureBufferPtr mParam;
	Ogre::uchar mPower;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Distort(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Distort"), mParam(NULL), mPower(0)
	{
	}

	/**
	Set parameter image for normal mapping.
	\param image Pointer to second image (default NULL)
	\note If the parameter image is set to NULL there won't be any image manipulation.
	*/
	Distort & setParameterImage(TextureBufferPtr image);

	/**
	Set power for distort effect.
	\param power New power for calculation (default 0)
	*/
	Distort & setPower(Ogre::uchar power);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Edge detection on input image.
\details Perform an edge detection with specified algorithm.

Example (Sobel):
\code{.cpp}
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();
Procedural::EdgeDetection(&bufferCell).setType(Procedural::EdgeDetection::DETECTION_SOBEL).process();
\endcode
\dotfile texture_13.gv
*/
class _ProceduralExport EdgeDetection : public TextureProcessing
{
public:
	//! List of algorithms used for edge detection
	enum DETECTION_TYPE
	{
		DETECTION_HOMOGENITY,	//!< Homogenity edge detector
		DETECTION_DIFFERENCE,	//!< Difference edge detector
		DETECTION_SOBEL,		//!< Sobel edge detector
		DETECTION_CANNY			//!< Canny edge detector
	};

private:
	Ogre::uchar mThreshshouldLow;
	Ogre::uchar mThreshshouldHigh;
	Ogre::uchar mSigma;
	DETECTION_TYPE mType;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	EdgeDetection(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "EdgeDetection"), mThreshshouldLow(20), mThreshshouldHigh(100), mSigma(92), mType(DETECTION_SOBEL)
	{
	}

	/**
	Set the lower threshould for canny filter.
	\param threshould New lower threshould value for canny filter [0, 255] (default 20)
	*/
	EdgeDetection & setThreshshouldLow(Ogre::uchar threshould);

	/**
	Set the upper threshould for canny filter.
	\param threshould New upper threshould value for canny filter [0, 255] (default 100)
	*/
	EdgeDetection & setThreshshouldHigh(Ogre::uchar threshould);

	/**
	Set sigma constant for canny filter.
	\param sigma New sigma constant for gaussian filter in canny filter [0, 255] (default 92)
	*/
	EdgeDetection & setSigma(Ogre::uchar sigma);

	/**
	Set the algorithm to sharp.
	\param type New algorithm to sharp (default SHARP_BASIC)
	*/
	EdgeDetection & setType(DETECTION_TYPE type);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();

private:
	Ogre::Vector3* getBlock(long x, long y);
};

/** 
\brief %Flip the image.
\details Flip the input image on different axis.

Examples:
\b FLIP_POINT
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Flip(&bufferImage).setAxis(Procedural::Flip::FLIP_POINT).process();
\endcode
\dotfile texture_14a.gv

\b FLIP_VERTICAL
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Flip(&bufferImage).setAxis(Procedural::Flip::FLIP_VERTICAL).process();
\endcode
\dotfile texture_14b.gv

\b FLIP_HORIZONTAL
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Flip(&bufferImage).setAxis(Procedural::Flip::FLIP_HORIZONTAL).process();
\endcode
\dotfile texture_14c.gv
*/
class _ProceduralExport Flip : public TextureProcessing
{
public:
	//! Flip axis selection
	enum FLIP_AXIS
	{
		FLIP_HORIZONTAL,	//!< Flip horizontal
		FLIP_VERTICAL,		//!< Flip vertical
		FLIP_POINT			//!< Flip middle
	};

private:
	FLIP_AXIS mAxis;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Flip(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Flip"), mAxis(FLIP_VERTICAL)
	{
	}

	/**
	Set the axis to flip.
	\param axis Axis where to flip the image arround (default FLIP_VERTICAL)
	*/
	Flip & setAxis(FLIP_AXIS axis);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief %Render a glow.
\details %Glow is a blurred, filled ellipse of given color, that is added to existing content.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).process();
Procedural::Glow(&bufferGradient).process();
\endcode
\dotfile texture_15.gv
*/
class _ProceduralExport Glow : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	Ogre::Real mCenterX;
	Ogre::Real mCenterY;
	Ogre::Real mRadiusX;
	Ogre::Real mRadiusY;
	Ogre::Real mAlpha;
	Ogre::Real mGamma;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Glow(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Glow"), mColour(Ogre::ColourValue::White), mCenterX(0.5f), mCenterY(0.5f), mRadiusX(0.5f), mRadiusY(0.5f), mAlpha(1.0f), mGamma(1.0f)
	{
	}

	/**
	Set the colour of the glow ellipse.
	\param colour New colour for glow ellipse (default Ogre::ColourValue::White)
	*/
	Glow & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the glow ellipse.
	\param red Red value of the glow ellipse [0, 255] (default 255)
	\param green Green value of the glow ellipse [0, 255] (default 255)
	\param blue Blue value of the glow ellipse [0, 255] (default 255)
	\param alpha %Alpha value of the glow ellipse [0, 255] (default 255)
	*/
	Glow & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour of the glow ellipse.
	\param red Red value of the glow ellipse [0.0, 1.0] \(default 1.0)
	\param green Green value of the glow ellipse [0.0, 1.0] \(default 1.0)
	\param blue Blue value of the glow ellipse [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of the glow ellipse [0.0, 1.0] \(default 1.0)
	*/
	Glow & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the relative center position of the blur circle on x axis.
	\param centerx New relative center of the blur circle [0.0, 1.0] \(default 0.5)
	*/
	Glow & setCenterX(Ogre::Real centerx);

	/**
	Set the relative center position of the blur circle on y axis.
	\param centery New relative center of the blur circle [0.0, 1.0] \(default 0.5)
	*/
	Glow & setCenterY(Ogre::Real centery);

	/**
	Set the relative radius of the blur circle in x direction.
	\param radiusx New relative radius of the blur circle [0.0, 1.0] \(default 0.5)
	*/
	Glow & setRadiusX(Ogre::Real radiusx);

	/**
	Set the relative radius of the blur circle in y direction.
	\param radiusy New relative radius of the blur circle [0.0, 1.0] \(default 0.5)
	*/
	Glow & setRadiusY(Ogre::Real radiusy);

	/**
	Set alpha value of blur effect.
	\param alpha New alpha value for blur effect (default 1)
	*/
	Glow & setAlpha(Ogre::Real alpha);

	/**
	Set gamma value of blur effect.
	\param gamma New gamma value for blur effect (default 1)
	*/
	Glow & setGamma(Ogre::Real gamma);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief %Invert image.
\details %Invert all three color channels, %Alpha is left unchanged (use Colours filter to adjust alpha).

Example:
\code{.cpp}
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();
Procedural::Invert(&bufferGradient).process();
\endcode
\dotfile texture_16.gv
*/
class _ProceduralExport Invert : public TextureProcessing
{
public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Invert(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Invert")
	{
	}

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Exchange pixels at random positions.
\details Exchange pixels in a small area randomly.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Jitter(&bufferImage).process();
\endcode
\dotfile texture_17.gv
*/
class _ProceduralExport Jitter : public TextureProcessing
{
private:
	Ogre::uint mSeed;
	Ogre::uchar mRadius;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Jitter(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Jitter"), mRadius(57), mSeed(5120)
	{
	}

	/**
	Set the radius of the detection area.
	\param radius New radius for detection area [0, 255] (default 57)
	*/
	Jitter & setRadius(Ogre::uchar radius);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	Jitter & setSeed(Ogre::uint seed);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Linear interpolation.
\details Mix first two inputs (a, b) in proportions given by base input (c). Each color channel is processed separately. Equation: x = bc + a(1 - c)

Example:
\code{.cpp}
// Image C
Procedural::TextureBuffer bufferCloud(256);
Procedural::Cloud(&bufferCloud).process();

// Image A
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).process();

// Image B
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(233).process();

// Filter
Procedural::Lerp(&bufferCloud).setImageA(&bufferGradient).setImageB(&bufferCell).process();
\endcode
\dotfile texture_18.gv
*/
class _ProceduralExport Lerp : public TextureProcessing
{
private:
	TextureBufferPtr mBufferA;
	TextureBufferPtr mBufferB;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	\note This is the third image (c).
	*/
	Lerp(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Lerp"), mBufferA(NULL), mBufferB(NULL)
	{
	}

	/**
	Set first image (a).
	\param image1 Pointer to a new first image (default NULL)
	*/
	Lerp & setImageA(TextureBufferPtr image1);

	/**
	Set second image (b).
	\param image2 Pointer to a new second image (default NULL)
	*/
	Lerp & setImageB(TextureBufferPtr image2);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Apply an illumination on a surface.
\details The parameter image is the required normal map. You can also set ambient, diffuse and specular light with different colors and intensity.

Example:
\code{.cpp}
// Generate structure
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();

// Create normal map
Procedural::TextureBufferPtr bufferNormalMap = bufferColour.clone();
Procedural::Normals(bufferNormalMap).process();

Procedural::Light(&bufferCell).setNormalMap(bufferNormalMap).setColourAmbient(127, 60, 0, 0).setColourDiffuse(60, 25, 0, 0).setBumpPower(255).process();
delete bufferNormalMap;
\endcode
\dotfile texture_19a.gv
\note If you don't set the normal map a clone of the base input image will be used as normal map with Normals filter. \dotfile texture_19b.gv
*/
class _ProceduralExport Light : public TextureProcessing
{
private:
	TextureBufferPtr mNormal;
	Ogre::ColourValue mColourAmbient;
	Ogre::ColourValue mColourDiffuse;
	Ogre::ColourValue mColourSpecular;
	Ogre::Vector3 mPosition;
	Ogre::uchar mSpecularPower;
	Ogre::uchar mBumpPower;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Light(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Light"), mNormal(NULL), mColourAmbient(Ogre::ColourValue::Black), mColourDiffuse(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f)), mColourSpecular(Ogre::ColourValue::White), mPosition(255.0f, 255.0f, 127.0f), mSpecularPower(0), mBumpPower(0)
	{
	}

	/**
	Set parameter image for compensation.
	\param normal Pointer to an normal map image (default NULL)
	\note If the parameter normal is set to NULL a clone of the base input image will be used as normal map with Normals filter.
	*/
	Light & setNormalMap(TextureBufferPtr normal);

	/**
	Set the ambient light colour.
	\param colour New ambient light colour (default Ogre::ColourValue::Black)
	*/
	Light & setColourAmbient(Ogre::ColourValue colour);

	/**
	Set the ambient light colour.
	\param red Red value of ambient light colour [0, 255] (default 0)
	\param green Green value of ambient light colour [0, 255] (default 0)
	\param blue Blue value of ambient light colour [0, 255] (default 0)
	\param alpha %Alpha value of ambient light colour [0, 255] (default 255)
	*/
	Light & setColourAmbient(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the ambient light colour.
	\param red Red value of ambient light colour [0.0, 1.0] \(default 0.0)
	\param green Green value of ambient light colour [0.0, 1.0] \(default 0.0)
	\param blue Blue value of ambient light colour [0.0, 1.0] \(default 0.0)
	\param alpha %Alpha value of ambient light colour [0.0, 1.0] \(default 0.0)
	*/
	Light & setColourAmbient(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the diffuse light colour.
	\param colour New diffuse light colour (default Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f))
	*/
	Light & setColourDiffuse(Ogre::ColourValue colour);

	/**
	Set the diffuse light colour.
	\param red Red value of diffuse light colour [0, 255] (default 128)
	\param green Green value of diffuse light colour [0, 255] (default 128)
	\param blue Blue value of diffuse light colour [0, 255] (default 128)
	\param alpha %Alpha value of diffuse light colour [0, 255] (default 255)
	*/
	Light & setColourDiffuse(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the diffuse light colour.
	\param red Red value of diffuse light colour [0.0, 1.0] \(default 0.5)
	\param green Green value of diffuse light colour [0.0, 1.0] \(default 0.5)
	\param blue Blue value of diffuse light colour [0.0, 1.0] \(default 0.5)
	\param alpha %Alpha value of diffuse light colour [0.0, 1.0] \(default 1.0)
	*/
	Light & setColourDiffuse(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the specular light colour.
	\param colour New specular light colour (default Ogre::ColourValue::White)
	*/
	Light & setColourSpecular(Ogre::ColourValue colour);

	/**
	Set the specular light colour.
	\param red Red value of specular light colour [0, 255] (default 255)
	\param green Green value of specular light colour [0, 255] (default 255)
	\param blue Blue value of specular light colour [0, 255] (default 255)
	\param alpha %Alpha value of specular light colour [0, 255] (default 255)
	*/
	Light & setColourSpecular(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the specular light colour.
	\param red Red value of specular light colour [0.0, 1.0] \(default 1.0)
	\param green Green value of specular light colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of specular light colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of specular light colour [0.0, 1.0] \(default 1.0)
	*/
	Light & setColourSpecular(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the light colours.
	\param ambient New ambient light colour (default Ogre::ColourValue::Black)
	\param diffuse New diffuse light colour (default Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f))
	\param specular New specular light colour (default Ogre::ColourValue::White)
	*/
	Light & setColours(Ogre::ColourValue ambient, Ogre::ColourValue diffuse, Ogre::ColourValue specular);

	/**
	Set the position of light on/over the image.
	\param position New light position (default Ogre::Vector3(255.0f, 255.0f, 127.0f))
	*/
	Light & setPosition(Ogre::Vector3 position);

	/**
	Set the position of light on/over the image.
	\param x New light position on x axis \(default 255.0f)
	\param y New light position on y axis \(default 255.0f)
	\param z New light position on z axis \(default 127.0f)
	*/
	Light & setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);

	/**
	Set specular light power.
	\param power New power value for specular light (default 0)
	*/
	Light & setSpecularPower(Ogre::uchar power);

	/**
	Set bump mapping power.
	\param power New power value for bump mapping (default 0)
	*/
	Light & setBumpPower(Ogre::uchar power);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Copy pixels from base input (x, y) to given coordinates from parameter image (red, green).
\details Use the red and green value of the parameter image as coordinates for colour painting.

Example:
\code{.cpp}
// Image colour
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();

// Image structure
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();

// Filter
Procedural::Lookup(&bufferGradient).setParameterImage(&bufferCell).process();
\endcode
\dotfile texture_20.gv
*/
class _ProceduralExport Lookup : public TextureProcessing
{
private:
	TextureBufferPtr mParam;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Lookup(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Lookup"), mParam(NULL)
	{
	}

	/**
	Set parameter image for coordinates mapping.
	\param image Pointer to second image (default NULL)
	\note If the parameter image is set to NULL there won't be any image manipulation.
	*/
	Lookup & setParameterImage(TextureBufferPtr image);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Convert height map to normal map.
\details Use Sobel algorithm to calculate a normal map out of the given image.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();
Procedural::Normals(&bufferCell).process();
\endcode
\dotfile texture_21a.gv
\par Tip
Create a copy of your working TextureBuffer for normal mapping before you colour it:
\code{.cpp}
// Generate structure
Procedural::TextureBuffer bufferColour(256);
Procedural::Cell(&bufferColour).setDensity(4).setRegularity(234).process();

// Create normal map
Procedural::TextureBufferPtr bufferNormalMap = bufferColour.clone();
Procedural::Normals(bufferNormalMap).process();

// Colourize structure
Procedural::Colours(&bufferColour).setColourBase(Ogre::ColourValue::Red).setColourPercent(Ogre::ColourValue::Blue).process();
delete bufferNormalMap;
\endcode
\dotfile texture_21b.gv
*/
class _ProceduralExport Normals : public TextureProcessing
{
private:
	Ogre::uchar mAmplify;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Normals(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Normals"), mAmplify(64)
	{
	}

	/**
	Set amplify for normal calculation
	\param amplify New amplify for calculation (default 64)
	*/
	Normals & setAmplify(Ogre::uchar amplify);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Create an oil painted image.
\details Combine similar pixels to a small group which gives the effect of an oil painted picture.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::OilPaint(&bufferImage).setRadius(5).process();
\endcode
\dotfile texture_22.gv
*/
class _ProceduralExport OilPaint : public TextureProcessing
{
private:
	Ogre::uchar mRadius;
	Ogre::Real mIntensity;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	OilPaint(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "OilPaint"), mRadius(3), mIntensity(20.0f)
	{
	}

	/**
	Set radius size for calculation.
	\param radius New radius for detection arround current pixel [3, 255] (default 3)
	*/
	OilPaint & setRadius(Ogre::uchar radius);

	/**
	Set intensity for painting.
	\param intensity New intensity factor which affects brush size \(default 20.0)
	*/
	OilPaint & setIntensity(Ogre::Real intensity);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Draw a number of pixels at random positions.
\details Paint a specific number of pixels at random positions in a given colour.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferSolid(256);
Procedural::Solid(&bufferSolid).setColour(Ogre::ColourValue(0.0f, 0.5f, 1.0f, 1.0f)).process();
Procedural::RandomPixels(&bufferSolid).setColour(Ogre::ColourValue::Red).setCount(200).process();
\endcode
\dotfile texture_23.gv
*/
class _ProceduralExport RandomPixels : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	Ogre::uint mSeed;
	Ogre::uint mCount;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	RandomPixels(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "RandomPixels"), mColour(Ogre::ColourValue::White), mSeed(5120)
	{
		mCount = ((Ogre::uint)Ogre::Math::Sqrt((Ogre::Real)mBuffer->getWidth()) + (Ogre::uint)Ogre::Math::Sqrt((Ogre::Real)mBuffer->getHeight())) * 10;
	}

	/**
	Set the colour of the pixel to paint.
	\param colour New colour for painting pixels (default Ogre::ColourValue::White)
	*/
	RandomPixels & setColour(Ogre::ColourValue colour);

	/**
	Set the colour of the pixel to paint.
	\param red Red value of the pixel colour [0, 255] (default 255)
	\param green Green value of the pixel colour [0, 255] (default 255)
	\param blue Blue value of the pixel colour [0, 255] (default 255)
	\param alpha %Alpha value of the pixel colour [0, 255] (default 255)
	*/
	RandomPixels & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the colour of the pixel to paint.
	\param red Red value of the pixel colour [0.0, 1.0] \(default 1.0)
	\param green Green value of the pixel colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of the pixel colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of the pixel colour [0.0, 1.0] \(default 1.0)
	*/
	RandomPixels & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set the seed for "random" number generator.
	\param seed Seed value where to set the random number generator (default 5120)
	*/
	RandomPixels & setSeed(Ogre::uint seed);

	/**
	Set the number of random painted pixels.
	\param count Number of pixels to paint (maximum: image height * image weight, default: (Sqrt(image width) + Sqrt(image height)) * 10)
	*/
	RandomPixels & setCount(Ogre::uint count);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Draw a rectangle.
\details Draw a filled rectangle on top of previous content.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferSolid(256);
Procedural::Solid(&bufferSolid).setColour(Ogre::ColourValue(0.0f, 0.5f, 1.0f, 1.0f)).process();
Procedural::Rectangle(&bufferSolid).setColour(Ogre::ColourValue::Red).setRectangle(0.25f, 0.25f, 0.75f, 0.75f).process();
\endcode
\dotfile texture_24.gv
*/
class _ProceduralExport Rectangle : public TextureProcessing
{
private:
	Ogre::ColourValue mColour;
	size_t mX1;
	size_t mY1;
	size_t mX2;
	size_t mY2;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Rectangle(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Rectangle"), mColour(Ogre::ColourValue::White), mX1(0), mY1(0)
	{
		mX2 = pBuffer->getWidth();
		mY2 = pBuffer->getHeight();
	}

	/**
	Set the fill colour of the rectangle.
	\param colour New colour for processing (default Ogre::ColourValue::White)
	*/
	Rectangle & setColour(Ogre::ColourValue colour);

	/**
	Set the fill colour of the rectangle.
	\param red Red value of the fill colour [0, 255] (default 255)
	\param green Green value of the fill colour [0, 255] (default 255)
	\param blue Blue value of the fill colour [0, 255] (default 255)
	\param alpha %Alpha value of the fill colour [0, 255] (default 255)
	*/
	Rectangle & setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha = 255);

	/**
	Set the fill colour of the rectangle.
	\param red Red value of the fill colour [0.0, 1.0] \(default 1.0)
	\param green Green value of the fill colour [0.0, 1.0] \(default 1.0)
	\param blue Blue value of the fill colour [0.0, 1.0] \(default 1.0)
	\param alpha %Alpha value of the fill colour [0.0, 1.0] \(default 1.0)
	*/
	Rectangle & setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha = 1.0f);

	/**
	Set absolute x position of top left start point of the rectangle in px
	\param x1 New absolute x position of rectangle start (default 0)
	*/
	Rectangle & setX1(size_t x1);

	/**
	Set relative x position of top left start point of the rectangle as Real
	\param x1 New relative x position of rectangle start [0.0, 1.0] \(default 0.0)
	*/
	Rectangle & setX1(Ogre::Real x1);

	/**
	Set absolute y position of top left start point of the rectangle in px
	\param y1 New absolute x position of rectangle start (default 0)
	*/
	Rectangle & setY1(size_t y1);

	/**
	Set relative y position of top left start point of the rectangle as Real
	\param y1 New relative y position of rectangle start [0.0, 1.0] \(default 0.0)
	*/
	Rectangle & setY1(Ogre::Real y1);

	/**
	Set absolute x position of bottom right end point of the rectangle in px
	\param x2 New absolute x position of rectangle end (default: image width)
	*/
	Rectangle & setX2(size_t x2);

	/**
	Set relative x position of bottom right end point of the rectangle as Real
	\param x2 New relative x position of rectangle end [0.0, 1.0] \(default 1.0)
	*/
	Rectangle & setX2(Ogre::Real x2);

	/**
	Set absolute y position of bottom right end point of the rectangle in px
	\param y2 New absolute x position of rectangle end (default: image height)
	*/
	Rectangle & setY2(size_t y2);

	/**
	Set relative y position of bottom right end point of the rectangle as Real
	\param y2 New relative y position of rectangle end [0.0, 1.0] \(default 1.0)
	*/
	Rectangle & setY2(Ogre::Real y2);

	/**
	Set the full rectangle coordinates.
	\param rect Full rectangle description (default: left=0.0, top=0.0, right=1.0, bottom=1.0)
	\param relative If this is set to true (default) the rectangle data are relative [0.0, 1.0]; else absolut [px]
	*/
	Rectangle & setRectangle(Ogre::RealRect rect, bool relative = true);

	/**
	Set the full rectangle coordinates.
	\param rect Full absolute rectangle description (default: left=0, top=0, right=image width, bottom=image height)
	*/
	Rectangle & setRectangle(Ogre::Rect rect);

	/**
	Set the full rectangle coordinates.
	\param pos1 Vector to top left start point of the rectangle (default: x=0.0, y=0.0)
	\param pos2 Vector to bottom right end point of the rectangle (default: x=1.0, y=1.0)
	\param relative If this is set to true (default) the vector data are relative [0.0, 1.0]; else absolut [px]
	*/
	Rectangle & setRectangle(Ogre::Vector2 pos1, Ogre::Vector2 pos2, bool relative = true);

	/**
	Set the full rectangle coordinates.
	\param x1 New absolute x position of rectangle start (default 0)
	\param y1 New absolute y position of rectangle start (default 0)
	\param x2 New absolute x position of rectangle end (default: image width)
	\param y2 New absolute y position of rectangle end (default: image height)
	*/
	Rectangle & setRectangle(size_t x1, size_t y1, size_t x2, size_t y2);

	/**
	Set the full rectangle coordinates.
	\param x1 New relative x position of rectangle start [0.0, 1.0] \(default 0.0)
	\param y1 New relative y position of rectangle start [0.0, 1.0] \(default 0.0)
	\param x2 New relative x position of rectangle end [0.0, 1.0] \(default 1.0)
	\param y2 New relative y position of rectangle end [0.0, 1.0] \(default 1.0)
	*/
	Rectangle & setRectangle(Ogre::Real x1, Ogre::Real y1, Ogre::Real x2, Ogre::Real y2);

#if PROCEDURAL_PLATFORM == PROCEDURAL_PLATFORM_WIN32
	/**
	Set the full rectangle coordinates.
	\param pos1 New absolute coordinates of the rectangle start point (default: x=0, y=0)
	\param pos2 New absolute coordinates of the rectangle end point (default: x=image width, y=image height)
	*/
	Rectangle & setRectangle(POINT pos1, POINT pos2);

	/**
	Set the full rectangle coordinates.
	\param rect Full absolute rectangle description (default: left=0, top=0, right=image width, bottom=image height)
	*/
	Rectangle & setRectangle(RECT rect);
#endif

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/** 
\brief Rotate & zoom image.
\details Rotate the image and/or zoom on a specific pat of it.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();
Procedural::RotationZoom(&bufferGradient).setRotation(0.125f).process();
\endcode
\dotfile texture_25.gv
*/
class _ProceduralExport RotationZoom : public TextureProcessing
{
private:
	Ogre::Real mCenterX;
	Ogre::Real mCenterY;
	Ogre::Real mZoomX;
	Ogre::Real mZoomY;
	Ogre::Radian mRotation;
	bool mWrap;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	RotationZoom(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "RotationZoom"), mCenterX(0.5f), mCenterY(0.5f), mZoomX(1.0f), mZoomY(1.0f), mRotation(0.0f), mWrap(true)
	{
	}

	/**
	Set the relative center position of the rotation on x axis.
	\param centerx New relative center of the rotation center [0.0, 1.0] \(default 0.5)
	*/
	RotationZoom & setCenterX(Ogre::Real centerx);

	/**
	Set the relative center position of the rotation on y axis.
	\param centery New relative center of the rotation center [0.0, 1.0] \(default 0.5)
	*/
	RotationZoom & setCenterY(Ogre::Real centery);

	/**
	Set the zoom factor in x direction.
	\param zoomx New factor for zoom in x direction \(default 1.0)
	*/
	RotationZoom & setZoomX(Ogre::Real zoomx);

	/**
	Set the zoom factor in y direction.
	\param zoomy New factor for zoom in y direction \(default 1.0)
	*/
	RotationZoom & setZoomY(Ogre::Real zoomy);

	/**
	Set the rotation angle.
	\param rotation New rotation angle [0.0, 1.0] \(default 0.0)
	*/
	RotationZoom & setRotation(Ogre::Real rotation);

	/**
	Set the rotation angle.
	\param rotation New rotation angle [0.0, Ogre::Math::TWO_PI] rad (default 0.0)
	*/
	RotationZoom & setRotation(Ogre::Radian rotation);

	/**
	Set the rotation angle.
	\param rotation New rotation angle [0, 360] degree (default 0)
	*/
	RotationZoom & setRotation(Ogre::Degree rotation);

	/**
	Set wrap.
	\param wrap New wrap value (default true)
	*/
	RotationZoom & setWrap(bool wrap);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief %Colours image segments based on threshold.
\details Takes segments of base image based on a threshold, and colors each segment based on .

Example:
\code{.cpp}
// Image colour
Procedural::TextureBuffer bufferGradient(256);
Procedural::Gradient(&bufferGradient).setColours(Ogre::ColourValue::Black, Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue).process();

// Image structure
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();

// Filter
Procedural::Segment(&bufferCell).setColourSource(&bufferGradient).process();
\endcode
\dotfile texture_26.gv
*/
class _ProceduralExport Segment : public TextureProcessing
{
private:
	TextureBufferPtr mColourSource;
	Ogre::uchar mThreshold;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Segment(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Segment"), mThreshold(128), mColourSource(NULL)
	{
	}

	/**
	Set parameter image for colour source.
	\param coloursource Pointer to an input image (default NULL)
	*/
	Segment & setColourSource(TextureBufferPtr coloursource);

	/**
	Set threshold value.
	\param threshold New threshold value [0, 255] (default 128)
	*/
	Segment & setThreshold(Ogre::uchar threshold);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Increase sharpness on input image.
\details Sharp the input image by a specified algorithm.

Examples:
\b SHARP_BASIC
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Sharpen(&bufferImage).setType(Procedural::Sharpen::SHARP_BASIC).process();
\endcode
\dotfile texture_27a.gv

\b SHARP_GAUSSIAN
\code{.cpp}
Procedural::TextureBuffer bufferImage(256);
Procedural::Image(&bufferImage).setFile("red_brick.jpg").process();
Procedural::Sharpen(&bufferImage).setType(Procedural::Sharpen::SHARP_GAUSSIAN).process();
\endcode
\dotfile texture_27b.gv
*/
class _ProceduralExport Sharpen : public TextureProcessing
{
public:
	//! List of algorithms to blur
	enum SHARP_TYPE
	{
		SHARP_BASIC,	//!< Use simplified block filter to sharp
		SHARP_GAUSSIAN	//!< Use gaussian filter to sharp
	};

private:
	Ogre::uchar mSize;
	Ogre::uchar mSigma;
	SHARP_TYPE mType;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Sharpen(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Sharpen"), mSize(5), mSigma(92), mType(SHARP_BASIC)
	{
	}

	/**
	Set the gaussian block size.
	\param size New block size for gaussian sharp filter [3, 255] (default 5)
	*/
	Sharpen & setSize(Ogre::uchar size);

	/**
	Set sigma constant for gaussian filter.
	\param sigma New sigma constant for gaussian sharp filter [0, 255] (default 92)
	*/
	Sharpen & setSigma(Ogre::uchar sigma);

	/**
	Set the algorithm to sharp.
	\param type New algorithm to sharp (default SHARP_BASIC)
	*/
	Sharpen & setType(SHARP_TYPE type);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Simple threshold filter.
\details Change luminance curve around given value.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();
Procedural::Threshold(&bufferCell).process();
\endcode
\dotfile texture_28.gv
*/
class _ProceduralExport Threshold : public TextureProcessing
{
public:
	//! Selection what to do with image parts below/above the threshould value.
	enum THRESHOLD_MODE
	{
		MODE_EXPAND_DOWNWARDS,	//!< Set pixel below threshould value to black
		MODE_EXPAND_UPWARDS,	//!< Set pixel above threshould value to white
		MODE_COMPRESS_BELOW,	//!< Set pixel below threshould value to an avarage value of all pixel below threshould value
		MODE_COMPRESS_ABOVE		//!< Set pixel above threshould value to an avarage value of all pixel above threshould value
	};

private:
	Ogre::uchar mThreshold;
	Ogre::uchar mRatio;
	THRESHOLD_MODE mMode;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Threshold(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Threshold"), mThreshold(128), mRatio(128), mMode(MODE_EXPAND_DOWNWARDS)
	{
	}

	/**
	Set threshold value.
	\param threshold New threshold value [0, 255] (default 128)
	*/
	Threshold & setThreshold(Ogre::uchar threshold);

	/**
	Set threshold ratio which affects the painting mode.
	\param ratio New painting factor [0, 255] (default 128)
	*/
	Threshold & setRatio(Ogre::uchar ratio);

	/**
	Set threshold mode.
	\param mode New mode what to do with pixels below/above threshold value (default MODE_EXPAND_DOWNWARDS)
	*/
	Threshold & setMode(THRESHOLD_MODE mode);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};

/**
\brief Twist some fragment of input image.
\details Twist parts of the input image arround a given point within a given radius.

Example:
\code{.cpp}
Procedural::TextureBuffer bufferCell(256);
Procedural::Cell(&bufferCell).setDensity(4).setRegularity(234).process();
Procedural::Vortex(&bufferCell).process();
\endcode
\dotfile texture_29.gv
*/
class _ProceduralExport Vortex : public TextureProcessing
{
private:
	Ogre::Real mCenterX;
	Ogre::Real mCenterY;
	Ogre::Real mRadiusX;
	Ogre::Real mRadiusY;
	Ogre::Radian mTwist;

public:
	/**
	Default constructor.
	\param pBuffer Image buffer where to modify the image.
	*/
	Vortex(TextureBufferPtr pBuffer)
		: TextureProcessing(pBuffer, "Vortex"), mCenterX(0.5f), mCenterY(0.5f), mRadiusX(0.5f), mRadiusY(0.5f), mTwist(Ogre::Math::HALF_PI)
	{
	}

	/**
	Set the relative position of the twist center on x axis.
	\param centerx New relative x position of the twist center [0.0, 1.0] \(default 0.5)
	*/
	Vortex & setCenterX(Ogre::Real centerx);

	/**
	Set the relative position of the twist center on y axis.
	\param centery New relative y position of the twist center [0.0, 1.0] \(default 0.5)
	*/
	Vortex & setCenterY(Ogre::Real centery);

	/**
	Set the relative radius of the twist area on x axis.
	\param radiusx New relative x radius of the twist area [0.0, 1.0] \(default 0.5)
	*/
	Vortex & setRadiusX(Ogre::Real radiusx);

	/**
	Set the relative radius of the twist area on y axis.
	\param radiusy New relative y radius of the twist area [0.0, 1.0] \(default 0.5)
	*/
	Vortex & setRadiusY(Ogre::Real radiusy);

	/**
	Set the twist angle.
	\param twist New twist angle for deformation [0.0, 1.0] \(default 0.25)
	*/
	Vortex & setTwist(Ogre::Real twist);

	/**
	Set the twist angle.
	\param twist New twist angle for deformation [0.0, Ogre::Math::TWO_PI] rad (default Ogre::Math::HALF_PI)
	*/
	Vortex & setTwist(Ogre::Radian twist);

	/**
	Set the twist angle.
	\param twist New twist angle for deformation [0, 360] degree (default 90)
	*/
	Vortex & setTwist(Ogre::Degree twist);

	/**
	Run image manipulation
	\return Pointer to image buffer which has been set in the constructor.
	*/
	TextureBufferPtr process();
};
/** @} */
}
#endif
