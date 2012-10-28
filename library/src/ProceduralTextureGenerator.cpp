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
#include "ProceduralTextureGenerator.h"
#include "ProceduralUtils.h"
#include "ProceduralTextureGeneratorHelpers.h"

using namespace Ogre;

namespace Procedural
{

#if OGRE_ENDIAN == OGRE_ENDIAN_LITTLE
#define PROCEDURAL_RED 3
#define PROCEDURAL_GREEN 2
#define PROCEDURAL_BLUE 1
#define PROCEDURAL_ALPHA 0
#else
#define PROCEDURAL_RED 0
#define PROCEDURAL_GREEN 1
#define PROCEDURAL_BLUE 2
#define PROCEDURAL_ALPHA 3
#endif

TextureBuffer::TextureBuffer(TextureBufferPtr tocopy)
{
	assert(tocopy != NULL);
	mWidth = tocopy->mWidth;
	mHeight = tocopy->mHeight;

	mPixels = new Ogre::uchar[mWidth * mHeight * 4];
	memcpy(mPixels, tocopy->mPixels, mWidth * mHeight * 4 * sizeof(Ogre::uchar));
}

TextureBuffer::TextureBuffer(Ogre::uint width_height)
	: mWidth(width_height), mHeight(width_height)
{
	assert(width_height > 7);
	mPixels = new Ogre::uchar[mWidth * mHeight * 4];
	memset(mPixels, 0, mWidth * mHeight * 4 * sizeof(Ogre::uchar));
	for (size_t y = 0; y < mHeight; y++)
	{
		for(size_t x = 0; x < mWidth; x++)
		{
			setAlpha(x, y, (Ogre::uchar)255);
		}
	}

#if OGRE_DEBUG_MODE
	Procedural::Utils::log("Create texture buffer : " + StringConverter::toString(mWidth) + "x" + StringConverter::toString(mHeight));
#endif
}

TextureBuffer::~TextureBuffer()
{
	delete mPixels;
}

void TextureBuffer::setPixel(size_t x, size_t y, Ogre::ColourValue colour)
{
	setPixel(x, y, colour.r, colour.g, colour.b, colour.a);
}

void TextureBuffer::setPixel(size_t x, size_t y, Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	setRed(x, y, red);
	setGreen(x, y, green);
	setBlue(x, y, blue);
	setAlpha(x, y, alpha);
}

void TextureBuffer::setPixel(size_t x, size_t y, Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	setRed(x, y, red);
	setGreen(x, y, green);
	setBlue(x, y, blue);
	setAlpha(x, y, alpha);
}

void TextureBuffer::setRed(size_t x, size_t y, Ogre::uchar red)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_RED] = red;
}

void TextureBuffer::setGreen(size_t x, size_t y, Ogre::uchar green)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_GREEN] = green;
}

void TextureBuffer::setBlue(size_t x, size_t y, Ogre::uchar blue)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_BLUE] = blue;
}

void TextureBuffer::setAlpha(size_t x, size_t y, Ogre::uchar alpha)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_ALPHA] = alpha;
}

void TextureBuffer::setRed(size_t x, size_t y, Ogre::Real red)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_RED] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(red * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setGreen(size_t x, size_t y, Ogre::Real green)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_GREEN] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(green * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setBlue(size_t x, size_t y, Ogre::Real blue)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_BLUE] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(blue * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setAlpha(size_t x, size_t y, Ogre::Real alpha)
{
	mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_ALPHA] = (Ogre::uchar)(std::min<Ogre::Real>(std::max<Ogre::Real>(alpha * 255.0f, 0.0f), 255.0f));
}

void TextureBuffer::setData(size_t width, size_t height, Ogre::uchar* data)
{
	if(data == NULL) return;
	if(width != mWidth || height != mHeight) return;
	memcpy(mPixels, data, mWidth * mHeight * 4 * sizeof(Ogre::uchar));
}

void TextureBuffer::setData(TextureBufferPtr buffer)
{
	if(buffer == NULL) return;
	if(buffer->mWidth != mWidth || buffer->mHeight != mHeight) return;
	memcpy(mPixels, buffer->mPixels, mWidth * mHeight * 4 * sizeof(Ogre::uchar));
}

Ogre::ColourValue TextureBuffer::getPixel(size_t x, size_t y)
{
	return Ogre::ColourValue(getPixelRedReal(x, y), getPixelGreenReal(x, y), getPixelBlueReal(x, y), getPixelAlphaReal(x, y));
}

Ogre::uchar TextureBuffer::getPixelRedByte(size_t x, size_t y)
{
	return mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_RED];
}

Ogre::uchar TextureBuffer::getPixelGreenByte(size_t x, size_t y)
{
	return mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_GREEN];
}

Ogre::uchar TextureBuffer::getPixelBlueByte(size_t x, size_t y)
{
	return mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_BLUE];
}

Ogre::uchar TextureBuffer::getPixelAlphaByte(size_t x, size_t y)
{
	return mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_ALPHA];
}

Ogre::Real TextureBuffer::getPixelRedReal(size_t x, size_t y)
{
	return ((Ogre::Real)mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_RED]) / 255.0f;
}

Ogre::Real TextureBuffer::getPixelGreenReal(size_t x, size_t y)
{
	return ((Ogre::Real)mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_GREEN]) / 255.0f;
}

Ogre::Real TextureBuffer::getPixelBlueReal(size_t x, size_t y)
{
	return ((Ogre::Real)mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_BLUE]) / 255.0f;
}

Ogre::Real TextureBuffer::getPixelAlphaReal(size_t x, size_t y)
{
	return ((Ogre::Real)mPixels[y * mWidth * 4 + x * 4 + PROCEDURAL_ALPHA]) / 255.0f;
}

TextureBufferPtr TextureBuffer::clone()
{
	TextureBufferPtr clon = new TextureBuffer(mWidth);
	memcpy(clon->mPixels, mPixels, mWidth * mHeight * 4 * sizeof(Ogre::uchar));
	return clon;
}

Ogre::Image* TextureBuffer::getImage()
{
	Ogre::Image* image = new Ogre::Image();
	image->loadDynamicImage(mPixels, mWidth, mHeight, 1, PF_R8G8B8A8);
	return image;
}

void TextureBuffer::saveImage(Ogre::String filename)
{
	Ogre::Image* image = getImage();
	image->save(filename);
	delete image;
}

Ogre::TexturePtr TextureBuffer::createTexture(Ogre::String name, Ogre::String group)
{
	Ogre::TexturePtr texture = Ogre::TextureManager::getSingletonPtr()->createManual(
		name,
		group,
		TEX_TYPE_2D,
		mWidth,
		mHeight,
		0,
		PF_R8G8B8A8,
		TU_DEFAULT);

	Ogre::Image* image = getImage();
	texture->loadImage(*image);
	delete image;

	return texture;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TextureProcessing::TextureProcessing(TextureBufferPtr pBuffer, Ogre::String name)
{
	assert(pBuffer != NULL);
	mBuffer = pBuffer;
	mName = name;
#if OGRE_DEBUG_MODE
	mLog = true;
#else
	mLog = false;
#endif
}

Ogre::String TextureProcessing::getName()
{
	return mName;
}

void TextureProcessing::setLog(bool enable)
{
	mLog = enable;
}

void TextureProcessing::logMsg(Ogre::String msg)
{
	if(mLog)
		Procedural::Utils::log(msg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Cell & Cell::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Cell & Cell::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Cell & Cell::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Cell & Cell::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

Cell & Cell::setRegularity(Ogre::uchar regularity)
{
	mRegularity = regularity;
	return *this;
}

Cell & Cell::setDensity(Ogre::uint density)
{
	mDensity = density;
	if(mDensity == 0) mDensity = 1;
	return *this;
}

Cell & Cell::setMode(Cell::CELL_MODE mode)
{
	mMode = mode;
	return *this;
}

Cell & Cell::setPattern(Cell::CELL_PATTERN pattern)
{
	mPattern = pattern;
	return *this;
}

TextureBufferPtr Cell::process()
{
	bool cfc;
	Ogre::Real coeff;

	srand(mSeed);
	const Ogre::Real regularity = mRegularity / 255.0f;
	Ogre::Vector3* cellPoints = (Ogre::Vector3*)malloc(sizeof(Ogre::Vector3) * mDensity * mDensity);

	for(size_t y = 0; y < mDensity; ++y)
	{
		for(size_t x = 0; x < mDensity; ++x)
		{
			Ogre::Real rand1 = (Ogre::Real)rand() / 65536.0f;
			Ogre::Real rand2 = (Ogre::Real)rand() / 65536.0f;
			cellPoints[x + y * mDensity].x = (x + 0.5f + (rand1 - 0.5f) * (1 - regularity)) / mDensity - 1.f / mBuffer->mWidth;
			cellPoints[x + y * mDensity].y = (y + 0.5f + (rand2 - 0.5f) * (1 - regularity)) / mDensity - 1.f / mBuffer->mHeight;
			cellPoints[x + y * mDensity].z = 0;
		}
	}

	for(size_t y = 0; y < mBuffer->mHeight; ++y)
	{
		for(size_t x = 0; x < mBuffer->mWidth; ++x)
		{
			Ogre::Vector3 pixelPos;
			pixelPos.x = (Ogre::Real)x / (Ogre::Real)mBuffer->mWidth,
			pixelPos.y = (Ogre::Real)y / (Ogre::Real)mBuffer->mHeight;
			pixelPos.z = 0.0f;

			Ogre::Real minDist = 10;
			Ogre::Real nextMinDist = minDist;
			int xo = x * mDensity / mBuffer->mWidth;
			int yo = y * mDensity / mBuffer->mHeight;
			for(long v = -1; v < 2; ++v)
			{
				int vo = ((yo + mDensity + v) % mDensity) * mDensity;
				for (long u = -1; u < 2; ++u)
				{
					Ogre::Vector3 cellPos = cellPoints[((xo + mDensity + u) % mDensity) + vo];
					if(u == -1 && x * mDensity < mBuffer->mWidth) cellPos.x -= 1;
					if(v == -1 && y * mDensity < mBuffer->mHeight) cellPos.y -= 1;
					if(u == 1 && x * mDensity >= mBuffer->mWidth * (mDensity - 1)) cellPos.x += 1;
					if(v == 1 && y * mDensity >= mBuffer->mHeight * (mDensity - 1)) cellPos.y += 1;
					Ogre::Vector3 tmp = pixelPos - cellPos;
					float norm = tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z;
					if(norm > 0.0f) norm = sqrtf(norm);
					if (norm < minDist)
					{
						nextMinDist = minDist;
						minDist = norm;
					}
					else if (norm < nextMinDist)
					{
						nextMinDist = norm;
					}
				}
			}

			switch(mPattern)
			{
				default:
				case PATTERN_BOTH:
					minDist = (nextMinDist - minDist) * mDensity;
					break;

				case PATTERN_CROSS:
					minDist = 2 * nextMinDist * mDensity - 1;
					break;

				case PATTERN_CONE:
					minDist = 1 - minDist * mDensity;
					break;
			}

			if(minDist < 0) minDist = 0;
			if(minDist > 1) minDist = 1;

			switch(mMode)
			{
				case MODE_CHESSBOARD:
					cfc = ((xo & 1) ^ (yo & 1)) != 0;
					coeff = (1 - 2 * cfc) / 2.5f;
					mBuffer->setRed(x, y, (Ogre::uchar)((cfc + coeff * minDist) * mColour.r * 255.0f));
					mBuffer->setGreen(x, y, (Ogre::uchar)((cfc + coeff * minDist) * mColour.g * 255.0f));
					mBuffer->setBlue(x, y, (Ogre::uchar)((cfc + coeff * minDist) * mColour.b * 255.0f));
					break;

				default:
				case MODE_GRID:
					mBuffer->setRed(x, y, (Ogre::uchar)(minDist * mColour.r * 255.0f));
					mBuffer->setGreen(x, y, (Ogre::uchar)(minDist * mColour.g * 255.0f));
					mBuffer->setBlue(x, y, (Ogre::uchar)(minDist * mColour.b * 255.0f));
					break;
			}
			mBuffer->setAlpha(x, y, mColour.a);
		}
	}

	logMsg("Create cell texture : " + StringConverter::toString(mDensity) + "x" + StringConverter::toString(mDensity));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Cloud & Cloud::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Cloud & Cloud::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Cloud & Cloud::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Cloud & Cloud::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

TextureBufferPtr Cloud::process()
{
	srand(mSeed);
	int r = rand();
	PerlinNoise noise(8, 0.5, 1.0 / 32.0, 1.0);
	Ogre::Real filterLevel = 0.7f;
	Ogre::Real preserveLevel = 0.3f;

	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			Ogre::Real noiseVal = std::max(0.0f, std::min(1.0f, noise.function2D(x + r, y + r) * 0.5f + 0.5f));
			mBuffer->setRed(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.r * 255.0f + filterLevel * mColour.r * 255.0f * noiseVal, 255.0f));
			mBuffer->setGreen(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.g * 255.0f + filterLevel * mColour.g * 255.0f * noiseVal, 255.0f));
			mBuffer->setBlue(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.b * 255.0f + filterLevel * mColour.b * 255.0f * noiseVal, 255.0f));
			mBuffer->setAlpha(x, y, mColour.a);
		}
	}

	logMsg("Create cloud texture");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Gradient & Gradient::setColourA(Ogre::ColourValue colour)
{
	mColourA = colour;
	return *this;
}

Gradient & Gradient::setColourA(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourA = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Gradient & Gradient::setColourA(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourA = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Gradient & Gradient::setColourB(Ogre::ColourValue colour)
{
	mColourB = colour;
	return *this;
}

Gradient & Gradient::setColourB(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourB = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Gradient & Gradient::setColourB(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourB = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Gradient & Gradient::setColourC(Ogre::ColourValue colour)
{
	mColourC = colour;
	return *this;
}

Gradient & Gradient::setColourC(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourC = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Gradient & Gradient::setColourC(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourC = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Gradient & Gradient::setColourD(Ogre::ColourValue colour)
{
	mColourD = colour;
	return *this;
}

Gradient & Gradient::setColourD(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourD = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Gradient & Gradient::setColourD(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourD = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Gradient & Gradient::setColours(Ogre::ColourValue colourA, Ogre::ColourValue colourB, Ogre::ColourValue colourC, Ogre::ColourValue colourD)
{
	mColourA = colourA;
	mColourB = colourB;
	mColourC = colourC;
	mColourD = colourD;
	return *this;
}

TextureBufferPtr Gradient::process()
{
	float finv_WH = 1.0f / (float)(mBuffer->mWidth * mBuffer->mHeight);
	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			float a = (float)((mBuffer->mWidth - x) * (mBuffer->mHeight - y)) * finv_WH;
			float b = (float)((                  x) * (mBuffer->mHeight - y)) * finv_WH;
			float c = (float)((mBuffer->mWidth - x) * (                   y)) * finv_WH;
			float d = (float)((                  x) * (                   y)) * finv_WH;

			mBuffer->setRed(x, y, (Ogre::uchar)(((mColourA.r * a) + (mColourB.r * b) + (mColourC.r * c) + (mColourD.r * d)) * 255.0f));
			mBuffer->setGreen(x, y, (Ogre::uchar)(((mColourA.g * a) + (mColourB.g * b) + (mColourC.g * c) + (mColourD.g * d)) * 255.0f));
			mBuffer->setBlue(x, y, (Ogre::uchar)(((mColourA.b * a) + (mColourB.b * b) + (mColourC.b * c) + (mColourD.b * d)) * 255.0f));
			mBuffer->setAlpha(x, y, (Ogre::uchar)(((mColourA.a * a) + (mColourB.a * b) + (mColourC.a * c) + (mColourD.a * d)) * 255.0f));
		}
	}

	logMsg("Create gradient texture");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Image & Image::setFile(Ogre::String filename, Ogre::String groupname)
{
	mFile = filename;
	mGroup = groupname;
	return *this;
}

TextureBufferPtr Image::process()
{
	Ogre::Image img;
	img.load(mFile, mGroup);
	if(img.getHeight() < mBuffer->getHeight() || img.getWidth() < mBuffer->getWidth()) return mBuffer;

	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			mBuffer->setPixel(x, y, img.getColourAt(x, y, 0));
		}
	}

	logMsg("Create texture from image");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Labyrinth & Labyrinth::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Labyrinth & Labyrinth::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Labyrinth & Labyrinth::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Labyrinth & Labyrinth::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

TextureBufferPtr Labyrinth::process()
{
	srand(mSeed);
	int r = rand();
	PerlinNoise noise(1, 0.65f, 1.0f / 16.0f, 1.0f);
	Ogre::Real filterLevel = 0.7f;
	Ogre::Real preserveLevel = 0.3f;

	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			Ogre::Real noiseVal = std::min(1.0f, std::abs(noise.function2D(x + r, y + r)));
			mBuffer->setRed(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.r * 255.0f + filterLevel * mColour.r * 255.0f * noiseVal, 255.0f));
			mBuffer->setGreen(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.g * 255.0f + filterLevel * mColour.g * 255.0f * noiseVal, 255.0f));
			mBuffer->setBlue(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.b * 255.0f + filterLevel * mColour.b * 255.0f * noiseVal, 255.0f));
			mBuffer->setAlpha(x, y, mColour.a);
		}
	}

	logMsg("Create labyrinth texture");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Marble & Marble::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Marble & Marble::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Marble & Marble::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Marble & Marble::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

TextureBuffer* Marble::process()
{
	srand(mSeed);
	int r = rand();
	PerlinNoise noise(2, 0.65f, 1.0f / 32.0f, 1.0f);
	Ogre::Real xFact = 1.0f / 96.0f;
	Ogre::Real yFact = 1.0f / 48.0f;
	Ogre::Real filterLevel = 0.7f;
	Ogre::Real preserveLevel = 0.3f;

	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			Ogre::Real noiseVal = std::min(1.0f, Ogre::Math::Abs(Ogre::Math::Sin(x * xFact + y * yFact + noise.function2D(x + r, y + r)) * Ogre::Math::PI));
			mBuffer->setRed(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.r * 255.0f + filterLevel * mColour.r * 255.0f * noiseVal, 255.0f));
			mBuffer->setGreen(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.g * 255.0f + filterLevel * mColour.g * 255.0f * noiseVal, 255.0f));
			mBuffer->setBlue(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.b * 255.0f + filterLevel * mColour.b * 255.0f * noiseVal, 255.0f));
			mBuffer->setAlpha(x, y, mColour.a);
		}
	}

	logMsg("Create marble texture");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Noise & Noise::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Noise & Noise::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Noise & Noise::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Noise & Noise::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

Noise & Noise::setType(NOISE_TYPE type)
{
	mType = type;
	return *this;
}

TextureBufferPtr Noise::process()
{
	NoiseBasePtr noiseGen;
	switch(mType)
	{
		case NOISE_PERLIN:
			noiseGen = new PerlinNoise();
			break;

		default:
		case NOISE_WHITE:
			noiseGen = new WhiteNoise(mSeed);
			break;
	}

	Ogre::uchar* field = noiseGen->field2D(mBuffer->mWidth, mBuffer->mHeight);
	for(size_t y = 0; y < mBuffer->mHeight; ++y)
	{
		for(size_t x = 0; x < mBuffer->mWidth; ++x)
		{
			Ogre::Real noiseVal = (Ogre::Real)field[y * mBuffer->mWidth + x];
			mBuffer->setRed(x, y, (Ogre::uchar)(noiseVal * mColour.r));
			mBuffer->setGreen(x, y, (Ogre::uchar)(noiseVal * mColour.g));
			mBuffer->setBlue(x, y, (Ogre::uchar)(noiseVal * mColour.b));
			mBuffer->setAlpha(x, y, (Ogre::uchar)(mColour.a * 255.0f));
		}
	}

	delete field;
	delete noiseGen;
	logMsg("Create noise texture : " + StringConverter::toString(mType));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Solid & Solid::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Solid & Solid::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Solid & Solid::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

TextureBufferPtr Solid::process()
{
	for(size_t y = 0; y < mBuffer->getHeight(); y++)
	{
		for(size_t x = 0; x < mBuffer->getWidth(); x++)
		{
			mBuffer->setPixel(x, y, mColour);
		}
	}

	logMsg("Create solid colour texture : " + StringConverter::toString((int)(mColour.r * 255.0f)) + ", " + StringConverter::toString((int)(mColour.g * 255.0f)) + ", " + StringConverter::toString((int)(mColour.b * 255.0f)));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Textile & Textile::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Textile & Textile::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Textile & Textile::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Textile & Textile::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

TextureBufferPtr Textile::process()
{
	srand(mSeed);
	int r = rand();
	PerlinNoise noise(3, 0.65f, 1.0f / 8.0f, 1.0f);
	Ogre::Real filterLevel = 0.7f;
	Ogre::Real preserveLevel = 0.3f;

	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			Ogre::Real noiseVal = std::max(0.0f, std::min(1.0f, (Ogre::Math::Sin(x + noise.function2D(x + r, y + r )) + Ogre::Math::Sin(y + noise.function2D(x + r, y + r))) * 0.25f + 0.5f));
			mBuffer->setRed(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.r * 255.0f + filterLevel * mColour.r * 255.0f * noiseVal, 255.0f));
			mBuffer->setGreen(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.g * 255.0f + filterLevel * mColour.g * 255.0f * noiseVal, 255.0f));
			mBuffer->setBlue(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.b * 255.0f + filterLevel * mColour.b * 255.0f * noiseVal, 255.0f));
			mBuffer->setAlpha(x, y, mColour.a);
		}
	}

	logMsg("Create textile texture");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Wood & Wood::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Wood & Wood::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Wood & Wood::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Wood & Wood::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

Wood & Wood::setRings(Ogre::uint rings)
{
	mRings = rings;
	if(mRings < 3) mRings = 3;
	return *this;
}

TextureBufferPtr Wood::process()
{
	srand(mSeed);
	int r = rand();
	Ogre::Real filterLevel = 0.7f;
	Ogre::Real preserveLevel = 0.3f;

	PerlinNoise noise(8, 0.5f, 1.0f / 32.0f, 0.05f);
	long w2 = mBuffer->mWidth / 2;
	long h2 = mBuffer->mHeight / 2;

	for(long y = 0; y < (long)mBuffer->mHeight; y++)
	{
		for(long x = 0; x < (long)mBuffer->mWidth; x++)
		{
			Ogre::Real xv = ((Ogre::Real)(x - w2)) / (Ogre::Real)mBuffer->mWidth;
			Ogre::Real yv = ((Ogre::Real)(y - h2)) / (Ogre::Real)mBuffer->mHeight;
			Ogre::Real noiseVal = std::min(1.0f, Ogre::Math::Abs(Ogre::Math::Sin((sqrt(xv * xv + yv * yv) + noise.function2D(x + r, y + r)) * Ogre::Math::PI * 2 * mRings)));
			mBuffer->setRed(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.r * 255.0f + filterLevel * mColour.r * 255.0f * noiseVal, 255.0f));
			mBuffer->setGreen(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.g * 255.0f + filterLevel * mColour.g * 255.0f * noiseVal, 255.0f));
			mBuffer->setBlue(x, y, (Ogre::uchar)std::min<Ogre::Real>(preserveLevel * mColour.b * 255.0f + filterLevel * mColour.b * 255.0f * noiseVal, 255.0f));
			mBuffer->setAlpha(x, y, mColour.a);
		}
	}

	logMsg("Create wood texture : " + StringConverter::toString(mRings));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Abnormals & Abnormals::setParameterImage(TextureBufferPtr image)
{
	mParam = image;
	return *this;
}

Abnormals & Abnormals::setRotation(Ogre::Real rotation)
{
	mW = Ogre::Radian(rotation * Ogre::Math::TWO_PI);
	return *this;
}

Abnormals & Abnormals::setRotation(Ogre::Radian rotation)
{
	mW = rotation;
	return *this;
}

Abnormals & Abnormals::setRotation(Ogre::Degree rotation)
{
	mW = (Ogre::Radian)rotation;
	return *this;
}

Abnormals & Abnormals::setAxis(Ogre::Vector3 axis)
{
	mAxis = axis;
	return *this;
}

Abnormals & Abnormals::setAxis(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	mAxis = Ogre::Vector3(x, y, z);
	return *this;
}

Abnormals & Abnormals::setSensitivity(Ogre::uchar sensitivity)
{
	mSensitivity = sensitivity;
	return *this;
}

Abnormals & Abnormals::setCompensation(Abnormals::ABNORMALS_COMPENSATION compensation)
{
	mCompensation = compensation;
	return *this;
}

Abnormals & Abnormals::setMirror(Abnormals::ABNORMALS_MIRROR mirror)
{
	mMirror = mirror;
	return *this;
}

TextureBufferPtr Abnormals::process()
{
	Ogre::Quaternion qion;
	Ogre::Real sum;
	Ogre::Vector3 q;

	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();
	Ogre::Quaternion rotation(mW, mAxis);

	if(mParam != NULL && (mParam->getWidth() < w || mParam->getHeight() < h)) return mBuffer;

	for(size_t y = 0; y < h; y++)
	{
		for(size_t x = 0; x < w; x++)
		{
			Ogre::ColourValue pixel = mBuffer->getPixel(x, y);
			Ogre::Quaternion v(0.0f, ((pixel.r * 255.0f) - 127.5f) / 127.5f, ((pixel.b * 255.0f) - 127.5f) / 127.5f, ((pixel.g * 255.0f) - 127.5f) / 127.5f);

			if(mParam != NULL)
			{
				pixel = mParam->getPixel(x, y);
				switch(mCompensation)
				{
					case COMPENSATION_NORMAL:
						qion = Ogre::Quaternion(0.0f, (pixel.r * 255.0f) - 127.5f, (pixel.b * 255.0f) - 127.5f, (pixel.g * 255.0f) - 127.5f);
						v = v * (Ogre::Real)(1 - mSensitivity);
						v = v + qion * ((Ogre::Real)mSensitivity / 127.5f);
						break;

					case COMPENSATION_HEIGHT:
						sum = ((pixel.r + pixel.g + pixel.b) / 3.0f) * 255.0f;
						qion = Ogre::Quaternion(Ogre::Radian(Ogre::Math::TWO_PI * sum / 765.f * mSensitivity), Ogre::Vector3(0.0f, 1.0f, 0.0f));
						rotation = rotation * qion;
						break;

					case COMPENSATION_QUATERNION:
						q = Ogre::Vector3((pixel.r * 255.0f) - 127.5f, (pixel.b * 255.0f) - 127.5f, (pixel.g * 255.0f) - 127.5f);
						qion = Ogre::Quaternion(Ogre::Radian(2.0f / 255.f * Ogre::Math::PI * pixel.a * mSensitivity), q);
						rotation = rotation * qion;
						break;
				}
			}

			v = rotation * v * rotation.Inverse();
			Ogre::Real norm = v.normalise();

			if(mMirror == MIRROR_X_YZ || mMirror == MIRROR_X_Y_Z)
				mBuffer->setRed(x, y, (Ogre::uchar)(255.0f - v.x * 127.5f + 127.5f));
			else
				mBuffer->setRed(x, y, (Ogre::uchar)(v.x * 127.5f + 127.5f));
			if(mMirror == MIRROR_Y_XZ || mMirror == MIRROR_X_Y_Z)
				mBuffer->setGreen(x, y, (Ogre::uchar)(255.0f - v.z * 127.5f + 127.5f));
			else
				mBuffer->setGreen(x, y, (Ogre::uchar)(v.z * 127.5f + 127.5f));
			mBuffer->setBlue(x, y, (Ogre::uchar)(v.y * 127.5f + 127.5f));
		}
	}

	logMsg("Modify texture with abnormals filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Alpha & Alpha::setExtractColour(Ogre::ColourValue colour)
{
	mExtractColour = colour;
	return *this;
}

Alpha & Alpha::setExtractColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mExtractColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Alpha & Alpha::setExtractColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mExtractColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

TextureBufferPtr Alpha::process()
{
	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();

	for(size_t y = 0; y < h; y++)
	{
		for(size_t x = 0; x < w; x++)
		{
			Ogre::Real r = (Ogre::Real)mBuffer->getPixelRedByte(x, y) * mExtractColour.r;
			Ogre::Real g = (Ogre::Real)mBuffer->getPixelGreenByte(x, y) * mExtractColour.g;
			Ogre::Real b = (Ogre::Real)mBuffer->getPixelBlueByte(x, y) * mExtractColour.b;
			Ogre::uchar a = (Ogre::uchar)(((unsigned long)mBuffer->getPixelAlphaByte(x, y) + (unsigned long)(r + g + b))>>1);
			mBuffer->setPixel(x, y, a, a, a, a);
		}
	}

	logMsg("Modify texture with alpha filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AlphaMask & AlphaMask::setColourAlphaMask(bool colourmask)
{
	mColourMask = colourmask;
	return *this;
}

AlphaMask & AlphaMask::setParameterImage(TextureBufferPtr image)
{
	mParam = image;
	return *this;
}

TextureBufferPtr AlphaMask::process()
{
	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();

	if(mParam != NULL && (mParam->getWidth() < w || mParam->getHeight() < h)) return mBuffer;

	for(size_t y = 0; y < h; y++)
	{
		for(size_t x = 0; x < w; x++)
		{
			if(mParam != NULL)
			{
				if(mColourMask)
				{
					Ogre::ColourValue pixelA = mBuffer->getPixel(x, y);
					Ogre::ColourValue pixelB = mParam->getPixel(x, y);
					Ogre::Vector3 c1(pixelA.r * 255.0f, pixelA.g * 255.0f, pixelA.b * 255.0f);
					Ogre::Vector3 c2(pixelB.r * 255.0f, pixelB.g * 255.0f, pixelB.b * 255.0f);

					Ogre::Real c1norm = c1.normalise();
					Ogre::Real c2norm = c2.normalise();

					Ogre::Real correctness = 0;

					if(c1norm > 0.0f && c2norm > 0.0f)
						correctness = c1.x * c2.x + c1.y * c2.y + c1.z * c2.z;

					mBuffer->setAlpha(x, y, (Ogre::uchar)(pixelA.a * correctness));
				}
				else
				{
					Ogre::ColourValue pixel = mParam->getPixel(x, y);
					Ogre::Real alpha = (pixel.r + pixel.g + pixel.b) / 3.f;
					mBuffer->setAlpha(x, y, mBuffer->getPixelAlphaReal(x, y) * alpha);
				}
			}
			else
			{
				Ogre::uchar a = mBuffer->getPixelAlphaByte(x, y);
				mBuffer->setPixel(x, y, a, a, a, 255);
			}
		}
	}

	logMsg("Modify texture with alphamask filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Blur & Blur::setSize(Ogre::uchar size)
{
	mSize = size;
	if(mSize < 3) mSize = 3;
	if((mSize % 2) == 0) mSize++;
	return *this;
}

Blur & Blur::setSigma(Ogre::uchar sigma)
{
	mSigma = sigma;
	return *this;
}

Blur & Blur::setType(Blur::BLUR_TYPE type)
{
	mType = type;
	return *this;
}

TextureBufferPtr Blur::process()
{
	Ogre::Real blurKernel[25] = {	1, 2, 3, 2, 1, 2, 4, 5, 4, 2, 3, 5, 6, 5, 3, 2, 4, 5, 4, 2, 1, 2, 3, 2, 1 }; 
	Convolution filter(mBuffer);
	switch(mType)
	{
		default:
		case BLUR_BOX:
			filter.setKernel(5, blurKernel);
			break;

		case BLUR_MEAN:
			filter.setKernel(Ogre::Matrix3(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)).calculateDivisor();
			break;

		case BLUR_GAUSSIAN:
			Ogre::Real fSigma = 0.5f + ((3.0f - 0.5f) / 255.0f) * (Ogre::Real)mSigma;
			int r = (int)mSize / 2;
			double min = Ogre::Math::Exp((Ogre::Real)(2 * r * r) / (-2.0f * fSigma * fSigma)) / (Ogre::Math::TWO_PI * fSigma * fSigma);
			int* kernel = new int[mSize * mSize];
			int divisor = 0;
			int y = -r;
			int x = -r;
			for(int i = 0; i < mSize; i++)
			{
				for(int j = 0; j < mSize; j++)
				{
					kernel[i * mSize + j] = (int)((Ogre::Math::Exp((Ogre::Real)(x * x + y * y) / (-2.0f * fSigma * fSigma)) / (Ogre::Math::TWO_PI * fSigma * fSigma)) / min);
					divisor += kernel[i * mSize + j];
					x++;
				}
				y++;
			}
			filter.setKernel(mSize, kernel).setDivisor((Ogre::Real)divisor);
			delete kernel;
			break;
	}
	filter.setIncludeAlphaChannel(true).process();

	logMsg("Modify texture with blur filter : " + StringConverter::toString(mType));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Channel & Channel::setSelection(CANNEL_SELECTION selection)
{
	mSelection = selection;
	return *this;
}

TextureBufferPtr Channel::process()
{
	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			if(mSelection == SELECT_GRAY)
			{
				Ogre::ColourValue pixel = mBuffer->getPixel(x, y);
				Ogre::Real gray = (pixel.r + pixel.g + pixel.b) / 3.0f;
				mBuffer->setPixel(x, y, gray, gray, gray, pixel.a);
			}
			else
			{
				if(mSelection != SELECT_RED) mBuffer->setRed(x, y, 0.0f);
				if(mSelection != SELECT_GREEN) mBuffer->setGreen(x, y, 0.0f);
				if(mSelection != SELECT_BLUE) mBuffer->setBlue(x, y, 0.0f);
				if(mSelection != SELECT_BLUE) mBuffer->setBlue(x, y, 0.0f);
			}
		}
	}

	logMsg("Modify texture with channel filter : " + StringConverter::toString(mSelection));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Colours & Colours::setColourBase(Ogre::ColourValue colour)
{
	mColourBase = colour;
	return *this;
}

Colours & Colours::setColourBase(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourBase = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Colours & Colours::setColourBase(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourBase = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Colours & Colours::setColourPercent(Ogre::ColourValue colour)
{
	mColourPercent = colour;
	return *this;
}

Colours & Colours::setColourPercent(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourPercent = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Colours & Colours::setColourPercent(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourPercent = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Colours & Colours::setBrithness(Ogre::uchar brithness)
{
	mBrithness = brithness;
	return *this;
}

Colours & Colours::setContrast(Ogre::uchar contrast)
{
	mContrast = contrast;
	return *this;
}

Colours & Colours::setSaturation(Ogre::uchar saturation)
{
	mSaturation = saturation;
	return *this;
}
	
Colours & Colours::setAlpha(Ogre::uchar alpha)
{
	mAlpha = alpha;
	return *this;
}

TextureBufferPtr Colours::process()
{
	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();
	long brithness = (((long)mBrithness) * 2) - 256;
	long contrast = (((long)mContrast));
	Ogre::Real fconstrast = (Ogre::Real)mContrast / 128.0f;
	fconstrast = fconstrast * fconstrast * fconstrast;
	contrast = (long)(fconstrast * 256.0f);
	Ogre::uchar minalpha = (mAlpha >= 127) ? (Ogre::uchar)((mAlpha - 127) * 2.f - (mAlpha - 127) / 128.f) : 0;
	Ogre::uchar maxalpha = (mAlpha <= 127) ? (Ogre::uchar)(mAlpha * 2.f + mAlpha / 127.f) : 255;
	Ogre::Real alphamult = (Ogre::Real)(maxalpha - minalpha) / 255.f;

	for(unsigned long y = 0; y < h; y++)
	{
		for(unsigned long x = 0; x < w; x++)
		{
			long r = (long)(mColourBase.r * 255.0f) + (((long)mBuffer->getPixelRedByte(x, y) * (long)(mColourPercent.r * 255.0f))>>8) + brithness;
			long g = (long)(mColourBase.g * 255.0f) + (((long)mBuffer->getPixelGreenByte(x, y) * (long)(mColourPercent.g * 255.0f))>>8) + brithness;
			long b = (long)(mColourBase.b * 255.0f) + (((long)mBuffer->getPixelBlueByte(x, y) * (long)(mColourPercent.b * 255.0f))>>8) + brithness;

			long c = (long)(((r - 127) * contrast)>>8) + 127;
			r = (c < 0x00) ? 0x00 : (c > 0xff) ? 0xff : c;

			c = (long) (((g - 127) * contrast)>>8) + 127;
			g = (c < 0x00) ? 0x00 : (c > 0xff) ? 0xff : c;

			c = (long) (((b - 127) * contrast)>>8) + 127;
			b = (c < 0x00) ? 0x00 : (c > 0xff) ? 0xff : c;

			if(mSaturation != 127)
			{
				long l = r + g + b;
				long u = (3 * r - l) * mSaturation / 127;
				long v = (3 * b - l) * mSaturation / 127;
				r = (u + l) / 3;
				g = (l - (u + v)) / 3;
				b = (v + l) / 3;
			}

			mBuffer->setRed(x, y, (Ogre::uchar)std::min<long>(std::max<long>(r, 0), 255));
			mBuffer->setGreen(x, y, (Ogre::uchar)std::min<long>(std::max<long>(g, 0), 255));
			mBuffer->setBlue(x, y, (Ogre::uchar)std::min<long>(std::max<long>(b, 0), 255));
			mBuffer->setAlpha(x, y, std::min<Ogre::uchar>(std::max<Ogre::uchar>((Ogre::uchar)((Ogre::Real)mBuffer->getPixelAlphaByte(x, y) * alphamult) + minalpha, 0), 255));
		}
	}

	logMsg("Modify texture with colours filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Combine & Combine::addImage(TextureBufferPtr image, COMBINE_METHOD method)
{
	if(image != NULL)
		if(image->getHeight() >= mBuffer->getHeight() && image->getWidth() >= mBuffer->getWidth())
		{
			LAYER l;
			l.action = method;
			l.image = image;
			mQueue.push(l);
		}
	return *this;
}

Combine & Combine::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Combine & Combine::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Combine & Combine::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

TextureBufferPtr Combine::process()
{
	int i = 0;
	while(mQueue.size() > 0)
	{
		LAYER l = mQueue.front();
		_process(l.image, l.action);
		mQueue.pop();
		i++;
		logMsg("Combine textures : " + StringConverter::toString(l.action));
	}

	return mBuffer;
}

void Combine::_process(TextureBufferPtr image, COMBINE_METHOD method)
{
	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();
	unsigned long rcolPercent = (unsigned long)(mColour.r * 255.0f);
	unsigned long gcolPercent = (unsigned long)(mColour.g * 255.0f);
	unsigned long bcolPercent = (unsigned long)(mColour.b * 255.0f);

	switch(method)
	{
		case METHOD_ADD_CLAMP:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					unsigned long r = (unsigned long)(pxDst.r * 255.0f) + (((unsigned long)(pxSrc.r * 255.0f) * rcolPercent)>>8);
					unsigned long g = (unsigned long)(pxDst.g * 255.0f) + (((unsigned long)(pxSrc.g * 255.0f) * gcolPercent)>>8);
					unsigned long b = (unsigned long)(pxDst.b * 255.0f) + (((unsigned long)(pxSrc.b * 255.0f) * bcolPercent)>>8);
					mBuffer->setPixel(x, y, (Ogre::uchar)((r < 255) ? r : 255), (Ogre::uchar)((g < 255) ? g : 255), (Ogre::uchar)((b < 255) ? b : 255), (Ogre::uchar)(pxDst.a * 255.0f));
				}
			}
			break;

		case METHOD_ADD_WRAP:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					unsigned long r = (unsigned long)(pxDst.r * 255.0f) + (((unsigned long)(pxSrc.r * 255.0f) * rcolPercent)>>8);
					unsigned long g = (unsigned long)(pxDst.g * 255.0f) + (((unsigned long)(pxSrc.g * 255.0f) * gcolPercent)>>8);
					unsigned long b = (unsigned long)(pxDst.b * 255.0f) + (((unsigned long)(pxSrc.b * 255.0f) * bcolPercent)>>8);
					mBuffer->setPixel(x, y, (Ogre::uchar)(r % 255), (Ogre::uchar)(g % 255), (Ogre::uchar)(b % 255), (Ogre::uchar)(pxDst.a * 255.0f));
				}
			}
			break;

		case METHOD_SUB_CLAMP:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					long r = (long)(pxDst.r * 255.0f) - (((long)(pxSrc.r * 255.0f) * rcolPercent)>>8);
					long g = (long)(pxDst.g * 255.0f) - (((long)(pxSrc.g * 255.0f) * gcolPercent)>>8);
					long b = (long)(pxDst.b * 255.0f) - (((long)(pxSrc.b * 255.0f) * bcolPercent)>>8);
					mBuffer->setPixel(x, y, (Ogre::uchar)((r > 0) ? r : 0), (Ogre::uchar)((g > 0) ? g : 0), (Ogre::uchar)((b > 0) ? b : 0), (Ogre::uchar)(pxDst.a * 255.0f));
				}
			}
			break;

		case METHOD_SUB_WRAP:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					long r = (long)(pxDst.r * 255.0f) - (((long)(pxSrc.r * 255.0f) * rcolPercent)>>8);
					long g = (long)(pxDst.g * 255.0f) - (((long)(pxSrc.g * 255.0f) * gcolPercent)>>8);
					long b = (long)(pxDst.b * 255.0f) - (((long)(pxSrc.b * 255.0f) * bcolPercent)>>8);
					mBuffer->setPixel(x, y, (Ogre::uchar)(r % 255), (Ogre::uchar)(g % 255), (Ogre::uchar)(b % 255), (Ogre::uchar)(pxDst.a * 255.0f));
				}
			}
			break;

		case METHOD_MULTIPLY:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					unsigned long r = (unsigned long)(pxDst.r * 255.0f) * (((unsigned long)(pxSrc.r * 255.0f) * rcolPercent)>>8);
					unsigned long g = (unsigned long)(pxDst.g * 255.0f) * (((unsigned long)(pxSrc.g * 255.0f) * gcolPercent)>>8);
					unsigned long b = (unsigned long)(pxDst.b * 255.0f) * (((unsigned long)(pxSrc.b * 255.0f) * bcolPercent)>>8);
					mBuffer->setPixel(x, y, (Ogre::uchar)(r >> 8), (Ogre::uchar)(g >> 8), (Ogre::uchar)(b >> 8), (Ogre::uchar)(pxDst.a * 255.0f));
				}
			}
			break;

		case METHOD_MULTIPLY2:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					unsigned long r = (unsigned long)(pxDst.r * 255.0f) * (((unsigned long)(pxSrc.r * 255.0f) * rcolPercent)>>8);
					r >>= 7;
					unsigned long g = (unsigned long)(pxDst.g * 255.0f) * (((unsigned long)(pxSrc.g * 255.0f) * gcolPercent)>>8);
					g >>= 7;
					unsigned long b = (unsigned long)(pxDst.b * 255.0f) * (((unsigned long)(pxSrc.b * 255.0f) * bcolPercent)>>8);
					b >>= 7;
					mBuffer->setPixel(x, y, (Ogre::uchar)((r < 255) ? r : 255), (Ogre::uchar)((g < 255) ? g : 255), (Ogre::uchar)((b < 255) ? b : 255), (Ogre::uchar)(pxDst.a * 255.0f));
				}
			}
			break;

		case METHOD_BLEND:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					unsigned long r = (unsigned long)(pxDst.r * 255.0f) + (((unsigned long)(pxSrc.r * 255.0f) * rcolPercent)>>8);
					unsigned long g = (unsigned long)(pxDst.g * 255.0f) + (((unsigned long)(pxSrc.g * 255.0f) * gcolPercent)>>8);
					unsigned long b = (unsigned long)(pxDst.b * 255.0f) + (((unsigned long)(pxSrc.b * 255.0f) * bcolPercent)>>8);
					mBuffer->setPixel(x, y, (Ogre::uchar)(r >> 1), (Ogre::uchar)(g >> 1), (Ogre::uchar)(b >> 1), (Ogre::uchar)(pxDst.a * 255.0f));
				}
			}
			break;

		case METHOD_ALPHA:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					unsigned long a = (unsigned long)(pxDst.a * 255.0f) + (((unsigned long)(pxSrc.a * 255.0f) * bcolPercent)>>8);
					mBuffer->setAlpha(x, y, (Ogre::uchar)(a >> 1));
				}
			}
			break;

		default:
		case METHOD_LAYER:
			for(size_t y = 0; y < h; y++)
			{
				for(size_t x = 0; x < w; x++)
				{
					Ogre::ColourValue pxSrc = image->getPixel(x, y);
					Ogre::ColourValue pxDst = mBuffer->getPixel(x, y);
					mBuffer->setPixel(x, y,
						(Ogre::uchar)(pxSrc.r * pxSrc.a * 255.0f + pxDst.r * 255.0f * (1.0f - pxSrc.a)),
						(Ogre::uchar)(pxSrc.g * pxSrc.a * 255.0f + pxDst.g * 255.0f * (1.0f - pxSrc.a)),
						(Ogre::uchar)(pxSrc.b * pxSrc.a * 255.0f + pxDst.b * 255.0f * (1.0f - pxSrc.a)),
						(Ogre::uchar)((pxDst.a - pxDst.a * pxSrc.a) * 255.0f + pxSrc.a * 255.0f));
				}
			}
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Convolution & Convolution::setKernel(Ogre::uchar size, Ogre::Real* data)
{
	if(size < 3 || size % 2 == 0) return *this;
	delete mKernelData;
	mKernelSize = size;
	mKernelData = new Ogre::Real[mKernelSize * mKernelSize];
	for(int y = 0; y < mKernelSize; y++)
	{
		for(int x = 0; x < mKernelSize; x++)
		{
			mKernelData[y * mKernelSize + x] = data[y * mKernelSize + x];
		}
	}
	calculateDivisor();
	return *this;
}

Convolution & Convolution::setKernel(Ogre::uchar size, int* data)
{
	if(size < 3 || size % 2 == 0) return *this;
	delete mKernelData;
	mKernelSize = size;
	mKernelData = new Ogre::Real[mKernelSize * mKernelSize];
	for(int y = 0; y < mKernelSize; y++)
	{
		for(int x = 0; x < mKernelSize; x++)
		{
			mKernelData[y * mKernelSize + x] = (Ogre::Real)data[y * mKernelSize + x];
		}
	}
	return *this;
}

Convolution & Convolution::setKernel(Ogre::Matrix3 data)
{
	delete mKernelData;
	mKernelSize = 3;
	mKernelData = new Ogre::Real[mKernelSize * mKernelSize];
	for(int y = 0; y < mKernelSize; y++)
	{
		for(int x = 0; x < mKernelSize; x++)
		{
			mKernelData[y * mKernelSize + x] = data[y][x];
		}
	}
	return *this;
}

Convolution & Convolution::setDivisor(Ogre::Real divisor)
{
	mDivisor = divisor;
	if(mDivisor == 0.0) mDivisor = 1.0;
	return *this;
}

Convolution & Convolution::calculateDivisor()
{
	mDivisor = 0.0;
	for(int y = 0; y < mKernelSize; y++)
	{
		for(int x = 0; x < mKernelSize; x++)
		{
			mDivisor += mKernelData[y * mKernelSize + x];
		}
	}
	if(mDivisor == 0.0) mDivisor = 1.0;

	return *this;
}

Convolution & Convolution::setThreshold(Ogre::uchar threshold)
{
	mThreshold = threshold;
	return *this;
}

Convolution & Convolution::setCalculateEdgeDivisor(bool calculateedgedivisor)
{
	mCalculateEdgeDivisor = calculateedgedivisor;
	return *this;
}

Convolution & Convolution::setIncludeAlphaChannel(bool usealpha)
{
	mIncludeAlphaChannel = usealpha;
	return *this;
}

TextureBufferPtr Convolution::process()
{
    int radius = ((int)mKernelSize) >> 1;
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	for(long y = 0; y < (long)mBuffer->mWidth; y++)
    {
		for(long x = 0; x < (long)mBuffer->mHeight; x++)
        {
			long r = 0;
			long g = 0;
			long b = 0;
			long a = 0;
			long div = 0;
			int processedKernelSize = 0;

            for(int i = 0; i < mKernelSize; i++)
            {
                int ir = i - radius;

                if((y + ir) < 0)
                    continue;
                if((y + ir) >= (long)mBuffer->mHeight)
                    break;

                for(int j = 0; j < (int)mKernelSize; j++)
                {
                    int jr = j - radius;

                    if((x + jr) < 0)
                        continue;
                    if((x + jr) < (long)mBuffer->mWidth)
                    {
                        Ogre::Real k = mKernelData[i * mKernelSize + j];
						Ogre::ColourValue pixel = mBuffer->getPixel(y + ir, x + jr);
                        div += (long)k;
						k *= 255.0f;
                        r += (long)(k * pixel.r);
                        g += (long)(k * pixel.g);
                        b += (long)(k * pixel.b);
                        a += (long)(k * pixel.a);

                        processedKernelSize++;
                    }
                }
            }

            if(processedKernelSize == (mKernelSize * mKernelSize))
                div = (long)mDivisor;
            else
            {
                if(!mCalculateEdgeDivisor)
					div = (long)mDivisor;
            }

            if(div != 0)
            {
                r /= div;
                g /= div;
                b /= div;
                a /= div;
            }
            r += ((long)mThreshold - 128);
            g += ((long)mThreshold - 128);
            b += ((long)mThreshold - 128);
			if(mIncludeAlphaChannel)
				a += ((long)mThreshold - 128);
			else
				a = (long)mBuffer->getPixelAlphaByte(x, y);

			tmpBuffer->setPixel(y, x,
				(Ogre::uchar)((r > 255) ? 255 : ((r < 0) ? 0 : r)),
				(Ogre::uchar)((g > 255) ? 255 : ((g < 0) ? 0 : g)),
				(Ogre::uchar)((b > 255) ? 255 : ((b < 0) ? 0 : b)),
				(Ogre::uchar)((a > 255) ? 255 : ((a < 0) ? 0 : a)));
        }
    }

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	if(mLog)
	{
		Ogre::String strKernel = "Modify texture with convolution filter :\n";
		for(int i = 0; i < mKernelSize; i++)
		{
			strKernel.append("\t");
			for(int j = 0; j < mKernelSize; j++)
			{
				strKernel.append(StringConverter::toString(mKernelData[i * mKernelSize + j]));
				if(j < (mKernelSize - 1)) strKernel.append("\t");
			}
			strKernel.append("\n");
		}
		logMsg(strKernel);
	}
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Crack & Crack::setParameterImage(TextureBufferPtr image)
{
	mParam = image;
	return *this;
}

Crack & Crack::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Crack & Crack::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Crack & Crack::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Crack & Crack::setCount(Ogre::uint count)
{
	mCount = count;
	return *this;
}

Crack & Crack::setVariation(Ogre::uchar variation)
{
	mVariation = variation;
	return *this;
}

Crack & Crack::setLength(Ogre::uchar length)
{
	mLength = length;
	return *this;
}

Crack & Crack::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

Crack & Crack::setLengthDecision(CRACK_LENGTH_DECISION lengthdecision)
{
	mLengthDecision = lengthdecision;
	return *this;
}
	
Crack & Crack::setQuality(CRACK_QUALITY quality)
{
	mQuality = quality;
	return *this;
}

TextureBufferPtr Crack::process()
{
	Ogre::ColourValue x1, y1, x2, y2;
	double cy2, cy1, cx2, cx1;
	size_t oxn, oyn;

	srand(mSeed);

	if(mParam == NULL) return mBuffer;

	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();

	if(mParam->getWidth() < w || mParam->getHeight() < h) return mBuffer;

	TextureBufferPtr tmpBuffer = mBuffer->clone();

	for(Ogre::uint n = 0; n < mCount; n++)
	{
		double x = ((double)rand() / RAND_MAX) * (double)w;
		double y = ((double)rand() / RAND_MAX) * (double)h;
		double a = Ogre::Math::TWO_PI * ((double)rand() / RAND_MAX);
		long count = (long)mLength;
		Ogre::ColourValue pixel = mParam->getPixel((size_t)x, (size_t)y);

		if(mParam->getWidth() && mLengthDecision == LENGTH_DECISION_NORMAL_BASED)
		{
			Ogre::Vector3 normal(pixel.r * 255.0f - 127.0f, pixel.g * 255.0f - 127.0f, 0.0f);
			Ogre::Real norm = normal.x * normal.x + normal.y * normal.y;
			norm = (norm > 0) ? Ogre::Math::Sqrt(norm) : 0;
			count = std::min<long>((long)(count * norm * norm / 8.0f), (long)mLength);
		}

		if(mLengthDecision == LENGTH_DECISION_RANDOM)
			count = (long)(count * ((double)rand() / RAND_MAX) * 2.0);

		while(--count >= 0)
		{
			a += (double)mVariation / 256.0 * (2.0 * ((double)rand() / RAND_MAX) - 1.0);

			x = x + Ogre::Math::Cos((Ogre::Real)a);
			y = y + Ogre::Math::Sin((Ogre::Real)a);
			if((size_t)x >= w || (size_t)y >= h) break;

			if(mParam->getWidth())
			{
				Ogre::Vector3 normal(127.0f - pixel.r * 255.0f, pixel.g * 255.0f - 127.0f, 0.0f);
				if(normal.x == 0.0)
				{
					if(normal.y > 0.0)
						a = Ogre::Math::PI;
					else
						a = Ogre::Math::TWO_PI;
				}
				else if(normal.x < 0)
					a = Ogre::Math::ATan(normal.y / normal.x).valueRadians() + 1.5f * Ogre::Math::PI;
				else if(normal.y < 0)
					a = Ogre::Math::ATan(normal.y / normal.x).valueRadians() + 2.5f * Ogre::Math::PI;
				else
					a = Ogre::Math::ATan(normal.y / normal.x).valueRadians() + Ogre::Math::HALF_PI;
				Ogre::Real norm = normal.x * normal.x + normal.y * normal.y;
				norm = (norm > 0) ? Ogre::Math::Sqrt(norm) : 0;
				if(norm < (255.0f - pixel.a * 255.0f) / 4.0f)
					continue;
			}

			switch(mQuality)
			{
				case QUALITY_SUBPIXEL:
					cy2 = (x - floor(x)) * (y - floor(y));
					cy1 = (y - floor(y)) * (ceil(x) - x);
					cx2 = (x - floor(x)) * (ceil(y) - y);
					cx1 = 1 - (cx2 + cy1 + cy2);
					oxn = std::min<size_t>((size_t)x + 1, w);
					oyn = std::min<size_t>((size_t)x + 1, h);

					x1 = mBuffer->getPixel((size_t)x, (size_t)y);
					y1 = mBuffer->getPixel((size_t)x, oyn);
					x2 = mBuffer->getPixel(oxn, (size_t)y);
					y2 = mBuffer->getPixel(oxn, oyn);

					x1 *= (float)(1 - cx1);
					x2 *= (float)(1 - cx2);
					y1 *= (float)(1 - cy1);
					y2 *= (float)(1 - cy2);

					x1 += mColour * (float)cx1;
					y1 += mColour * (float)cy1;
					x2 += mColour * (float)cx2;
					y2 += mColour * (float)cy2;

					tmpBuffer->setPixel((size_t)x, (size_t)y, x1);
					tmpBuffer->setPixel((size_t)x, oyn, y1);
					tmpBuffer->setPixel(oxn, (size_t)y, x2);
					tmpBuffer->setPixel(oxn, oyn, y2);
					break;

				case QUALITY_ALPHA:
					tmpBuffer->setPixel((size_t)x, (size_t)y, mBuffer->getPixel((size_t)x, (size_t)y) + mColour);
					break;

				default:
					tmpBuffer->setPixel((size_t)x, (size_t)y, mColour);
					break;
			}
		}
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with crack filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Dilate & Dilate::setIterations(Ogre::uchar iterations)
{
	mIterations = iterations;
	if(mIterations == 0) mIterations = 1;
	return *this;
}

TextureBufferPtr Dilate::process()
{
	long w = (long)mBuffer->getWidth();
	long h = (long)mBuffer->getHeight();
	TextureBufferPtr intBuffer = mBuffer->clone();
	TextureBufferPtr dstBuffer = mBuffer->clone();

	TextureBufferPtr pSrc = NULL;
	TextureBufferPtr pDst = NULL;

	for(long i = 0; i < (long)mIterations; i++)
	{
		if(i == 0)
			pSrc = mBuffer;
		else
		{
			pSrc = ((i % 2) != (mIterations % 2)) ? dstBuffer : intBuffer;
		}
		pDst = ((i % 2) == (mIterations % 2)) ? dstBuffer : intBuffer;

		for(long y = 0; y < h; y++)
		{
			for(long x = 0; x < w; x++)
			{
				long sum = -1;
				pDst->setPixel(x, y, pSrc->getPixel(x, y));

				for(long v = -1; v < 2; v++)
				{
					for(long u = -1; u < 2; u++)
					{
						Ogre::ColourValue pixel = pSrc->getPixel((x + w + u) % w, (y + h + v) % h);
						if((pixel.r + pixel.g + pixel.b) * 255.0f > sum)
						{
							sum = (long)((pixel.r + pixel.g + pixel.b) * 255.0f);
							pDst->setPixel(x, y, pixel);
						}
					}
				}
			}
		}
	}

	mBuffer->setData(dstBuffer);
	delete intBuffer;
	delete dstBuffer;

	logMsg("Modify texture with dilate filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Distort & Distort::setParameterImage(TextureBufferPtr image)
{
	mParam = image;
	return *this;
}

Distort & Distort::setPower(Ogre::uchar power)
{
	mPower = power;
	return *this;
}

TextureBufferPtr Distort::process()
{
	if(mParam == NULL) return mBuffer;

	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	if(mParam->getWidth() < w || mParam->getHeight() < h) return mBuffer;

	Ogre::Real fPower = (Ogre::Real)mPower;

	for(size_t y = 0; y < h; ++y)
	{
		for(size_t x = 0; x < w; ++x)
		{
			Ogre::ColourValue pixel = mParam->getPixel(x, y);
			Ogre::Vector3 n(pixel.r * 255.0f - 127.0f, pixel.g * 255.0f - 127.0f, pixel.b * 255.0f - 127.0f);
			n.normalise();
			Ogre::Real u = fmod((Ogre::Real)(x + (n.x * fPower)), (Ogre::Real)w);
			Ogre::Real v = fmod((Ogre::Real)(y + (n.y * fPower)), (Ogre::Real)h);
            Ogre::Real uf = (u >= 0) ? (u - (long)u) : 1 + (u - (long)u);
            Ogre::Real vf = (v >= 0) ? (v - (long)v) : 1 + (v - (long)v);
			unsigned long ut = (u >= 0) ? (unsigned long)u : (unsigned long)u - 1;
			unsigned long vt = (v >= 0) ? (unsigned long)v : (unsigned long)v - 1;
			Ogre::ColourValue texel = mBuffer->getPixel(vt % h, ut % w) * (1.0f - uf) * (1.0f - vf);
			texel += mBuffer->getPixel(vt % h, (ut + 1) % w) * uf * (1.0f - vf);
			texel += mBuffer->getPixel((vt + 1) % h, ut % w) * (1.0f - uf) * vf;
			texel += mBuffer->getPixel((vt + 1) % h, (ut + 1) % w) * uf * vf;
			tmpBuffer->setPixel(x, y, texel);
		}
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with distort filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EdgeDetection & EdgeDetection::setThreshshouldLow(Ogre::uchar threshould)
{
	mThreshshouldLow = threshould;
	return *this;
}

EdgeDetection & EdgeDetection::setThreshshouldHigh(Ogre::uchar threshould)
{
	mThreshshouldHigh = threshould;
	return *this;
}

EdgeDetection & EdgeDetection::setSigma(Ogre::uchar sigma)
{
	mSigma = sigma;
	return *this;
}

EdgeDetection & EdgeDetection::setType(DETECTION_TYPE type)
{
	mType = type;
	return *this;
}

TextureBufferPtr EdgeDetection::process()
{
	Ogre::ColourValue pixel;
	Ogre::Vector3* block;
	Ogre::Vector3 d, v, n;
	// Canny specific
	Ogre::Vector3* orientation;
	Ogre::Vector3* gradients;
	Ogre::Real div;

	TextureBufferPtr tmpBuffer = mBuffer->clone();
	Solid(tmpBuffer).setColour(Ogre::ColourValue::Black).process();
	
	long w = (long)mBuffer->getWidth();
	long h = (long)mBuffer->getHeight();

	switch(mType)
	{
		default:
		case DETECTION_SOBEL:
			//n = Ogre::Vector3::ZERO;
            for(long y = 0; y < h; y++)
            {
                for(long x = 0; x < w; x++)
                {
					pixel = mBuffer->getPixel((size_t)x, (size_t)y);
					block = getBlock(x, y);
					d = block[0] + 2.0f * block[1] + block[2] - block[6] - 2.0f * block[7] - block[8];
					d = Ogre::Vector3(Ogre::Math::Abs(d.x), Ogre::Math::Abs(d.y), Ogre::Math::Abs(d.z));
					v = block[2] + 2.0f * block[5] + block[8] - block[0] - 2.0f * block[3] - block[6];
					v = Ogre::Vector3(Ogre::Math::Abs(v.x), Ogre::Math::Abs(v.y), Ogre::Math::Abs(v.z));
					d = d + v;
					/*if(d.x > n.x) n.x = d.x;
					if(d.y > n.y) n.y = d.y;
					if(d.z > n.z) n.z = d.z;*/
					delete block;

					tmpBuffer->setPixel((size_t)x, (size_t)y, d.x, d.y, d.z, pixel.a);
				}
			}
			break;

		case DETECTION_DIFFERENCE:
            for(long y = 0; y < h; y++)
            {
                for(long x = 0; x < w; x++)
                {
					pixel = mBuffer->getPixel((size_t)x, (size_t)y);
					block = getBlock(x, y);
					n = Ogre::Vector3::ZERO;
					for(int j = 0; j < 3; j++)
					{
						d = block[j] - block[6 + (2 - j)];
						if(Ogre::Math::Abs(d.x) > n.x) n.x = Ogre::Math::Abs(d.x);
						if(Ogre::Math::Abs(d.y) > n.y) n.y = Ogre::Math::Abs(d.y);
						if(Ogre::Math::Abs(d.z) > n.z) n.z = Ogre::Math::Abs(d.z);
					}
					d = block[5] - block[3];
					if(Ogre::Math::Abs(d.x) > n.x) n.x = Ogre::Math::Abs(d.x);
					if(Ogre::Math::Abs(d.y) > n.y) n.y = Ogre::Math::Abs(d.y);
					if(Ogre::Math::Abs(d.z) > n.z) n.z = Ogre::Math::Abs(d.z);
					delete block;

					tmpBuffer->setPixel((size_t)x, (size_t)y, n.x, n.y, n.z, pixel.a);
                }
            }
			break;

		case DETECTION_HOMOGENITY:
            for(long y = 0; y < h; y++)
            {
                for(long x = 0; x < w; x++)
                {
					pixel = mBuffer->getPixel((size_t)x, (size_t)y);
					block = getBlock(x, y);
					v = block[4];
					n = Ogre::Vector3::ZERO;
					for(int j = 0; j < 3; j++)
					{
						for(int i = 0; i < 3; i++)
						{
							if(j == 1 && i == 1) continue;
							d = v - block[j * 3 + i];
							if(Ogre::Math::Abs(d.x) > n.x) n.x = Ogre::Math::Abs(d.x);
							if(Ogre::Math::Abs(d.y) > n.y) n.y = Ogre::Math::Abs(d.y);
							if(Ogre::Math::Abs(d.z) > n.z) n.z = Ogre::Math::Abs(d.z);
						}
					}
					delete block;
					tmpBuffer->setPixel((size_t)x, (size_t)y, n.x, n.y, n.z, pixel.a);
                }
            }
			break;

		case DETECTION_CANNY:
			// STEP 1 - blur image
			Blur(mBuffer).setSigma(mSigma).setType(Blur::BLUR_GAUSSIAN).process();

			// STEP 2 - calculate magnitude and edge orientation
			orientation = new Ogre::Vector3[w * h];
			gradients = new Ogre::Vector3[w * h];
			n = Ogre::Vector3(-std::numeric_limits<Ogre::Real>::infinity(), -std::numeric_limits<Ogre::Real>::infinity(), -std::numeric_limits<Ogre::Real>::infinity());
            for(long y = 0; y < h; y++)
            {
                for(long x = 0; x < w; x++)
                {
					pixel = mBuffer->getPixel((size_t)x, (size_t)y);
					block = getBlock(x, y);
					d = block[2] + block[8] - block[0] - block[6] + 2.0f * (block[5] - block[3]);
					v = block[0] + block[2] - block[6] - block[8] + 2.0f * (block[1] - block[7]);
					gradients[y * w + x] = Ogre::Vector3(Ogre::Math::Sqrt(d.x * d.x + v.x * v.x), Ogre::Math::Sqrt(d.y * d.y + v.y * v.y), Ogre::Math::Sqrt(d.z * d.z + v.z * v.z));
					if(gradients[y * w + x].x > n.x) n.x = gradients[y * w + x].x;
					if(gradients[y * w + x].y > n.y) n.y = gradients[y * w + x].y;
					if(gradients[y * w + x].z > n.z) n.z = gradients[y * w + x].z;
					delete block;
					orientation[y * w + x] = Ogre::Vector3::ZERO;
					if(d.x == 0.0f)
					{
						orientation[y * w + x].x = (v.x == 0.0f) ? 0.0f : 90.0f;
					}
					else
					{
						div = v.x / d.x;
						if(div < 0.0f)
                            orientation[y * w + x].x = 180.0f - Ogre::Math::ATan(-div).valueDegrees();
						else
							orientation[y * w + x].x = Ogre::Math::ATan(div).valueDegrees();

                        if(orientation[y * w + x].x < 22.5f)
                            orientation[y * w + x].x = 0.0f;
                        else if(orientation[y * w + x].x < 67.5f)
                            orientation[y * w + x].x = 45.0f;
                        else if(orientation[y * w + x].x < 112.5f)
                            orientation[y * w + x].x = 90.0f;
                        else if(orientation[y * w + x].x < 157.5f)
                            orientation[y * w + x].x = 135.0f;
                        else
							orientation[y * w + x].x = 0.0f;
					}
					if(d.y == 0.0f)
					{
						orientation[y * w + x].y = (v.y == 0.0f) ? 0.0f : 90.0f;
					}
					else
					{
						div = v.y / d.y;
						if(div < 0.0f)
                            orientation[y * w + x].y = 180.0f - Ogre::Math::ATan(-div).valueDegrees();
						else
							orientation[y * w + x].y = Ogre::Math::ATan(div).valueDegrees();

                        if(orientation[y * w + x].y < 22.5f)
                            orientation[y * w + x].y = 0.0f;
                        else if(orientation[y * w + x].y < 67.5f)
                            orientation[y * w + x].y = 45.0f;
                        else if(orientation[y * w + x].y < 112.5f)
                            orientation[y * w + x].y = 90.0f;
                        else if(orientation[y * w + x].y < 157.5f)
                            orientation[y * w + x].y = 135.0f;
                        else
							orientation[y * w + x].y = 0.0f;
					}
					if(d.z == 0.0f)
					{
						orientation[y * w + x].z = (v.z == 0.0f) ? 0.0f : 90.0f;
					}
					else
					{
						div = v.z / d.z;
						if(div < 0.0f)
                            orientation[y * w + x].z = 180.0f - Ogre::Math::ATan(-div).valueDegrees();
						else
							orientation[y * w + x].z = Ogre::Math::ATan(div).valueDegrees();

                        if(orientation[y * w + x].z < 22.5f)
                            orientation[y * w + x].z = 0.0f;
                        else if(orientation[y * w + x].z < 67.5f)
                            orientation[y * w + x].z = 45.0f;
                        else if(orientation[y * w + x].z < 112.5f)
                            orientation[y * w + x].z = 90.0f;
                        else if(orientation[y * w + x].z < 157.5f)
                            orientation[y * w + x].z = 135.0f;
                        else
							orientation[y * w + x].z = 0.0f;
					}
				}
			}
			
			// STEP 3 - suppres non maximums
            for(long y = 1; y < (h - 1); y++)
            {
                for(long x = 1; x < (w - 1); x++)
                {
					div = gradients[y * w + x].x / n.x;
					switch(((int)orientation[y * w + x].x))
					{
						default:
						case 0:
							if((gradients[y * w + x].x < gradients[y * w + (x - 1)].x) || (gradients[y * w + x].x < gradients[y * w + (x + 1)].x))
								div = 0.0f;
							break;
						case 45:
							if((gradients[y * w + x].x < gradients[(y + 1) * w + (x - 1)].x) || (gradients[y * w + x].x < gradients[(y - 1) * w + (x + 1)].x))
								div = 0.0f;
							break;
						case 90:
							if((gradients[y * w + x].x < gradients[(y + 1) * w + x].x) || (gradients[y * w + x].x < gradients[(y - 1) * w + x].x))
								div = 0.0f;
							break;
						case 135:
							if((gradients[y * w + x].x < gradients[(y + 1) * w + (x + 1)].x) || (gradients[y * w + x].x < gradients[(y - 1) * w + (x - 1)].x))
								div = 0.0f;
							break;
					}
					tmpBuffer->setRed((size_t)x, (size_t)y, div);
					div = gradients[y * w + x].y / n.y;
					switch(((int)orientation[y * w + x].y))
					{
						default:
						case 0:
							if((gradients[y * w + x].y < gradients[y * w + (x - 1)].y) || (gradients[y * w + x].y < gradients[y * w + (x + 1)].y))
								div = 0.0f;
							break;
						case 45:
							if((gradients[y * w + x].y < gradients[(y + 1) * w + (x - 1)].y) || (gradients[y * w + x].y < gradients[(y - 1) * w + (x + 1)].y))
								div = 0.0f;
							break;
						case 90:
							if((gradients[y * w + x].y < gradients[(y + 1) * w + x].y) || (gradients[y * w + x].y < gradients[(y - 1) * w + x].y))
								div = 0.0f;
							break;
						case 135:
							if((gradients[y * w + x].y < gradients[(y + 1) * w + (x + 1)].y) || (gradients[y * w + x].y < gradients[(y - 1) * w + (x - 1)].y))
								div = 0.0f;
							break;
					}
					tmpBuffer->setGreen((size_t)x, (size_t)y, div);
					div = gradients[y * w + x].z / n.z;
					switch(((int)orientation[y * w + x].z))
					{
						default:
						case 0:
							if((gradients[y * w + x].z < gradients[y * w + (x - 1)].z) || (gradients[y * w + x].z < gradients[y * w + (x + 1)].z))
								div = 0.0f;
							break;
						case 45:
							if((gradients[y * w + x].z < gradients[(y + 1) * w + (x - 1)].z) || (gradients[y * w + x].z < gradients[(y - 1) * w + (x + 1)].z))
								div = 0.0f;
							break;
						case 90:
							if((gradients[y * w + x].z < gradients[(y + 1) * w + x].z) || (gradients[y * w + x].z < gradients[(y - 1) * w + x].z))
								div = 0.0f;
							break;
						case 135:
							if((gradients[y * w + x].z < gradients[(y + 1) * w + (x + 1)].z) || (gradients[y * w + x].z < gradients[(y - 1) * w + (x - 1)].z))
								div = 0.0f;
							break;
					}
					tmpBuffer->setBlue((size_t)x, (size_t)y, div);
				}
			}

			// STEP 4 - hysteresis
			mBuffer->setData(tmpBuffer);
			div = (Ogre::Real)mThreshshouldHigh / 255.0f;
            for(long y = 1; y < (h - 1); y++)
            {
                for(long x = 1; x < (w - 1); x++)
                {
					pixel = mBuffer->getPixel((size_t)x, (size_t)y);
					if(pixel.r < div)
					{
						if(pixel.r < (Ogre::Real)mThreshshouldLow / 255.0f)
							tmpBuffer->setRed((size_t)x, (size_t)y, 0.0f);
						else
						{
							if( (mBuffer->getPixelRedReal((size_t)(x - 1), (size_t)(y    )) < div) &&
								(mBuffer->getPixelRedReal((size_t)(x + 1), (size_t)(y    )) < div) &&
								(mBuffer->getPixelRedReal((size_t)(x - 1), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelRedReal((size_t)(x    ), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelRedReal((size_t)(x + 1), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelRedReal((size_t)(x - 1), (size_t)(y + 1)) < div) &&
								(mBuffer->getPixelRedReal((size_t)(x    ), (size_t)(y + 1)) < div) &&
								(mBuffer->getPixelRedReal((size_t)(x + 1), (size_t)(y + 1)) < div))
                            {
                                tmpBuffer->setRed((size_t)x, (size_t)y, 0.0f);
                            }
						}
					}
					if(pixel.g < div)
					{
						if(pixel.g < (Ogre::Real)mThreshshouldLow / 255.0f)
							tmpBuffer->setGreen((size_t)x, (size_t)y, 0.0f);
						else
						{
							if( (mBuffer->getPixelGreenReal((size_t)(x - 1), (size_t)(y    )) < div) &&
								(mBuffer->getPixelGreenReal((size_t)(x + 1), (size_t)(y    )) < div) &&
								(mBuffer->getPixelGreenReal((size_t)(x - 1), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelGreenReal((size_t)(x    ), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelGreenReal((size_t)(x + 1), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelGreenReal((size_t)(x - 1), (size_t)(y + 1)) < div) &&
								(mBuffer->getPixelGreenReal((size_t)(x    ), (size_t)(y + 1)) < div) &&
								(mBuffer->getPixelGreenReal((size_t)(x + 1), (size_t)(y + 1)) < div))
                            {
                                tmpBuffer->setGreen((size_t)x, (size_t)y, 0.0f);
                            }
						}
					}
					if(pixel.b < div)
					{
						if(pixel.b < (Ogre::Real)mThreshshouldLow / 255.0f)
							tmpBuffer->setBlue((size_t)x, (size_t)y, 0.0f);
						else
						{
							if( (mBuffer->getPixelBlueReal((size_t)(x - 1), (size_t)(y    )) < div) &&
								(mBuffer->getPixelBlueReal((size_t)(x + 1), (size_t)(y    )) < div) &&
								(mBuffer->getPixelBlueReal((size_t)(x - 1), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelBlueReal((size_t)(x    ), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelBlueReal((size_t)(x + 1), (size_t)(y - 1)) < div) &&
								(mBuffer->getPixelBlueReal((size_t)(x - 1), (size_t)(y + 1)) < div) &&
								(mBuffer->getPixelBlueReal((size_t)(x    ), (size_t)(y + 1)) < div) &&
								(mBuffer->getPixelBlueReal((size_t)(x + 1), (size_t)(y + 1)) < div))
                            {
                                tmpBuffer->setBlue((size_t)x, (size_t)y, 0.0f);
                            }
						}
					}
				}
			}

			delete orientation;
			delete gradients;
			break;
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with edgedetection filter : " + StringConverter::toString(mType));
	return mBuffer;
}

Ogre::Vector3* EdgeDetection::getBlock(long x, long y)
{
	Ogre::ColourValue pixel = mBuffer->getPixel((size_t)x, (size_t)y);
	Ogre::Vector3* block = new Ogre::Vector3[9];
	for(int j = -1; j < 2; j++)
	{
		for(int i = -1; i < 2; i++)
		{
			block[(j + 1) * 3 + (i + 1)] = Ogre::Vector3(pixel.r, pixel.g, pixel.b);
			if(j == 0 && i == 0) continue;
			if((x + i) < 0 || (x + i) >= (long)mBuffer->mWidth) continue;
			if((y + j) < 0 || (y + j) >= (long)mBuffer->mHeight) continue;
			block[(j + 1) * 3 + (i + 1)] = Ogre::Vector3((Ogre::Real)mBuffer->getPixelRedReal((size_t)(x + i), (size_t)(y + j)),
														 (Ogre::Real)mBuffer->getPixelGreenReal((size_t)(x + i), (size_t)(y + j)),
														 (Ogre::Real)mBuffer->getPixelBlueReal((size_t)(x + i), (size_t)(y + j)));
		}
	}
	return block;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Flip & Flip::setAxis(FLIP_AXIS axis)
{
	mAxis = axis;
	return *this;
}

TextureBufferPtr Flip::process()
{
	TextureBufferPtr tmpBuffer = mBuffer->clone();
	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			switch(mAxis)
			{
				case FLIP_HORIZONTAL:
					tmpBuffer->setPixel(x, mBuffer->mHeight - 1 - y, mBuffer->getPixel(x, y));
					break;

				default:
				case FLIP_VERTICAL:
					tmpBuffer->setPixel(mBuffer->mWidth - 1 - x, y, mBuffer->getPixel(x, y));
					break;

				case FLIP_POINT:
					tmpBuffer->setPixel(mBuffer->mWidth - 1 - x, mBuffer->mHeight - 1 - y, mBuffer->getPixel(x, y));
					break;
			}
		}
	}
	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with flip filter : " + StringConverter::toString(mAxis));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Glow & Glow::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Glow & Glow::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Glow & Glow::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Glow & Glow::setCenterX(Ogre::Real centerx)
{
	mCenterX = centerx;
	return *this;
}

Glow & Glow::setCenterY(Ogre::Real centery)
{
	mCenterY = centery;
	return *this;
}

Glow & Glow::setRadiusX(Ogre::Real radiusx)
{
	mRadiusX = radiusx;
	return *this;
}

Glow & Glow::setRadiusY(Ogre::Real radiusy)
{
	mRadiusY = radiusy;
	return *this;
}

Glow & Glow::setAlpha(Ogre::Real alpha)
{
	mAlpha = alpha;
	return *this;
}

Glow & Glow::setGamma(Ogre::Real gamma)
{
	mGamma = gamma;
	return *this;
}

TextureBufferPtr Glow::process()
{
	long w = (long)mBuffer->getWidth();
	long h = (long)mBuffer->getHeight();
	long dwCenterX	= (long)(mCenterX * (Ogre::Real)w);
	long dwCenterY	= (long)(mCenterY * (Ogre::Real)h);
	long dwRadiusX	= (long)(mRadiusX * (Ogre::Real)w);
	long dwRadiusY	= (long)(mRadiusY * (Ogre::Real)h);
	Ogre::Real fRed = mColour.r * 255.0f;
	Ogre::Real fGreen = mColour.g * 255.0f;
	Ogre::Real fBlue = mColour.b * 255.0f;
	Ogre::Real f1_RadiusX = 1.0f / (Ogre::Real)dwRadiusX;
	Ogre::Real f1_RadiusY = 1.0f / (Ogre::Real)dwRadiusY;

	for(long y = 0; y < h; y++)
	{
		Ogre::Real dy = (Ogre::Real)(y - dwCenterY) * f1_RadiusY;

		for(long x = 0; x < w; x++)
		{
			Ogre::Real dx = (Ogre::Real)(x - dwCenterX) * f1_RadiusX;
			Ogre::Real d = sqrt(dx * dx + dy * dy);
			if(d > 1.0f) d = 1.0f;
			d = 1.0f - d;

			unsigned long r = (unsigned long)(((Ogre::Real)mBuffer->getPixelRedByte(x, y)) + ((mGamma * d * fRed) * mAlpha));
			unsigned long g = (unsigned long)(((Ogre::Real)mBuffer->getPixelGreenByte(x, y)) + ((mGamma * d * fGreen) * mAlpha));
			unsigned long b = (unsigned long)(((Ogre::Real)mBuffer->getPixelBlueByte(x, y)) + ((mGamma * d * fBlue) * mAlpha));
			Ogre::uchar a = mBuffer->getPixelAlphaByte(x, y);

			mBuffer->setPixel(x, y, (Ogre::uchar)((r < 255) ? r : 255), (Ogre::uchar)((g < 255) ? g : 255), (Ogre::uchar)((b < 255) ? b : 255), a);
		}
	}

	logMsg("Modify texture with glow filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TextureBufferPtr Invert::process()
{
	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();

	for(size_t y = 0; y < h; y++)
	{
		for(size_t x = 0; x < w; x++)
		{
			Ogre::ColourValue pixel = mBuffer->getPixel(x, y);
			mBuffer->setPixel(x, y, 1.0f - pixel.r, 1.0f - pixel.g, 1.0f - pixel.b, pixel.a);
		}
	}

	logMsg("Modify texture with invert filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Jitter & Jitter::setRadius(Ogre::uchar radius)
{
	mRadius = radius;
	return *this;
}

Jitter & Jitter::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

TextureBufferPtr Jitter::process()
{
	TextureBufferPtr tmpBuffer = mBuffer->clone();
	srand(mSeed);
	int radius = (int)(1.0f + (9.0f / 255.0f) * ((Ogre::Real)mRadius - 1.0f));
	int max = radius * 2 + 1;
    for(long y = 0; y < (long)mBuffer->mHeight; y++)
    {
        for(long x = 0; x < (long)mBuffer->mWidth; x++)
        {
            long rx = x + (rand() % (radius * 2 + 1)) - radius;
            long ry = y + (rand() % (radius * 2 + 1)) - radius;

            if(rx >= 0 && rx < (long)mBuffer->mWidth && ry >= 0 && ry < (long)mBuffer->mHeight)
				tmpBuffer->setPixel((size_t)rx, (size_t)ry, mBuffer->getPixel((size_t)x, (size_t)y));
        }
    }
	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with jitter filter : " + StringConverter::toString(mRadius));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Lerp & Lerp::setImageA(TextureBufferPtr image1)
{
	mBufferA = image1;
	return *this;
}

Lerp & Lerp::setImageB(TextureBufferPtr image2)
{
	mBufferB = image2;
	return *this;
}

TextureBufferPtr Lerp::process()
{
	for(size_t y = 0; y < mBuffer->mHeight; y++)
	{
		for(size_t x = 0; x < mBuffer->mWidth; x++)
		{
			Ogre::ColourValue pixelA = mBufferA->getPixel(x, y);
			Ogre::ColourValue pixelB = mBufferB->getPixel(x, y);
			Ogre::ColourValue pixelC = mBuffer->getPixel(x, y);

			mBuffer->setPixel(x, y,
				pixelA.r * (1.0f - pixelC.r) + pixelB.r * pixelC.r,
				pixelA.g * (1.0f - pixelC.g) + pixelB.g * pixelC.g,
				pixelA.b * (1.0f - pixelC.b) + pixelB.b * pixelC.b,
				pixelA.a * (1.0f - pixelC.a) + pixelB.a * pixelC.a);
		}
	}

	logMsg("Modify texture with lerp filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Light & Light::setNormalMap(TextureBufferPtr normal)
{
	mNormal = normal;
	return *this;
}

Light & Light::setColourAmbient(Ogre::ColourValue colour)
{
	mColourAmbient = colour;
	return *this;
}

Light & Light::setColourAmbient(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourAmbient = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Light & Light::setColourAmbient(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourAmbient = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Light & Light::setColourDiffuse(Ogre::ColourValue colour)
{
	mColourDiffuse = colour;
	return *this;
}

Light & Light::setColourDiffuse(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourDiffuse = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Light & Light::setColourDiffuse(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourDiffuse = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Light & Light::setColourSpecular(Ogre::ColourValue colour)
{
	mColourSpecular = colour;
	return *this;
}

Light & Light::setColourSpecular(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColourSpecular = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Light & Light::setColourSpecular(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColourSpecular = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Light & Light::setColours(Ogre::ColourValue ambient, Ogre::ColourValue diffuse, Ogre::ColourValue specular)
{
	mColourAmbient = ambient;
	mColourDiffuse = diffuse;
	mColourSpecular = specular;
	return *this;
}

Light & Light::setPosition(Ogre::Vector3 position)
{
	return setPosition(position.x, position.y, position.z);
}

Light & Light::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	mPosition = Ogre::Vector3(std::max<Ogre::Real>(std::min<Ogre::Real>(x, 255.0f), 0.0f), std::max<Ogre::Real>(std::min<Ogre::Real>(y, 255.0f), 0.0f), std::max<Ogre::Real>(std::min<Ogre::Real>(z, 255.0f), 0.0f));
	return *this;
}

Light & Light::setSpecularPower(Ogre::uchar power)
{
	mSpecularPower = power;
	return *this;
}

Light & Light::setBumpPower(Ogre::uchar power)
{
	mBumpPower = power;
	return *this;
}

TextureBufferPtr Light::process()
{
	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();
	Ogre::Vector3 light(mPosition.x - 127.0f, -(mPosition.y - 127.0f), -(127.0f - mPosition.z));
	light.normalise();
	Ogre::Real fSpecularPower = ((Ogre::Real)mSpecularPower) / 32.0f;
	Ogre::Real fBumpPower = ((Ogre::Real)mBumpPower) / 32.0f;

	if(mNormal != NULL && (mNormal->getWidth() < w || mNormal->getHeight() < h)) return mBuffer;

	TextureBufferPtr normalMap;
	if(mNormal != NULL) 
		normalMap = mNormal->clone();
	else
	{
		normalMap = mBuffer->clone();
		Normals(normalMap).process();
	}

	for(size_t y = 0; y < h; y++)
	{
		for (size_t x = 0; x < w; x++)
		{
			Ogre::ColourValue pixel = normalMap->getPixel(x, y);
			Ogre::Vector3 n(pixel.r * 255.0f - 127.0f, pixel.g * 255.0f - 127.0f, pixel.b * 255.0f - 127.0f);
			n.normalise();

			Ogre::Real fdot = n.x * light.x + n.y * light.y + n.z * light.z;
			if(fdot < 0.0f) fdot = 0.0f;
			fdot *= fBumpPower;

			long r = (long)(mColourAmbient.r * 255.0f + (fdot * mColourDiffuse.r * 255.0f) + (fdot * fdot * mColourSpecular.r * fSpecularPower));
			long g = (long)(mColourAmbient.g * 255.0f + (fdot * mColourDiffuse.g * 255.0f) + (fdot * fdot * mColourSpecular.g * fSpecularPower));
			long b = (long)(mColourAmbient.b * 255.0f + (fdot * mColourDiffuse.b * 255.0f) + (fdot * fdot * mColourSpecular.b * fSpecularPower));

			pixel = mBuffer->getPixel(x, y);
			r = ((long)(pixel.r * 255.0f) + r) / 2;
			g = ((long)(pixel.g * 255.0f) + g) / 2;
			b = ((long)(pixel.b * 255.0f) + b) / 2;

			mBuffer->setPixel(x, y, (Ogre::uchar)((r < 255) ? r : 255), (Ogre::uchar)((g < 255) ? g : 255), (Ogre::uchar)((b < 255) ? b : 255), (Ogre::uchar)(pixel.a * 255.0f));
		}
	}

	delete normalMap;
	logMsg("Modify texture with light filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Lookup & Lookup::setParameterImage(TextureBufferPtr image)
{
	mParam = image;
	return *this;
}

TextureBufferPtr Lookup::process()
{
	if(mParam == NULL) return mBuffer;

	size_t tw = mBuffer->getWidth();
	size_t th = mBuffer->getHeight();
	size_t w = mParam->getWidth();
	size_t h = mParam->getHeight();
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	if(w < tw || h < th) return mBuffer;

	Ogre::Real scaleW = tw / 256.0f;
	Ogre::Real scaleH = th / 256.0f;

	for(size_t y = 0; y < h; ++y)
	{
		for(size_t x = 0; x < w; ++x)
		{
			Ogre::ColourValue pixel = mParam->getPixel(x, y);
			size_t u = (size_t)(pixel.r * (Ogre::Real)w);
			size_t v = (size_t)(pixel.g * (Ogre::Real)h);
			tmpBuffer->setPixel(x, y, mBuffer->getPixel(v, u));
		}
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with lookup filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Normals & Normals::setAmplify(Ogre::uchar amplify)
{
	mAmplify = amplify;
	return *this;
}

TextureBufferPtr Normals::process()
{
	long w = (long)mBuffer->getWidth();
	long h = (long)mBuffer->getHeight();
	Ogre::Real fAmp = (Ogre::Real)mAmplify * 4.0f / 255.0f;
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	for(long y = 0; y < h; y++)
	{
		for(long x = 0; x < w; x++)
		{
			size_t xp = (x < 1) ? 0 : (x - 1) % w;
			size_t xn = (x + 1) % w;
			size_t yp = (y < 1) ? 0 : (y - 1) % h;
			size_t yn = (y + 1) % h;

			//Y Sobel filter
			Ogre::Real fPix = (Ogre::Real)mBuffer->getPixelRedByte(xp, yn);
			Ogre::Real dY  = fPix * -1.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(x, yn);
			dY += fPix * -2.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xn, yn);
			dY += fPix * -1.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xp, yp);
			dY += fPix * 1.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(x, yp);
			dY += fPix * 2.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xn, yp);
			dY += fPix * 1.0f;

			//X Sobel filter
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xp, yp);
			Ogre::Real dX  = fPix * -1.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xp, y);
			dX += fPix * -2.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xp, yn);
			dX += fPix * -1.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xn, yp);
			dX += fPix * 1.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xn, y);
			dX += fPix * 2.0f;
			fPix = (Ogre::Real)mBuffer->getPixelRedByte(xn, yn);
			dX += fPix * 1.0f;

			Ogre::Real normx = -dX * fAmp / 255.0f;
			Ogre::Real normy = -dY * fAmp / 255.0f;
			Ogre::Real norm = Ogre::Math::Sqrt(normx * normx + normy * normy + 1.0f);
			if(norm > (float)10e-6)
				norm = 1.0f / norm;
			else
				norm = 0.0f;
			normx = (normx * norm) * 127.0f + 128.0f;
			normy = (normy * norm) * 127.0f + 128.0f;
			Ogre::Real normz = norm * 127.0f + 128.0f;

			tmpBuffer->setPixel(x, y, (Ogre::uchar)normx, (Ogre::uchar)normy, (Ogre::uchar)normz, mBuffer->getPixelAlphaByte(x, y));
		}
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with normals filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OilPaint & OilPaint::setRadius(Ogre::uchar radius)
{
	mRadius = radius;
	if(mRadius < 3) mRadius = 3;
	return *this;
}

OilPaint & OilPaint::setIntensity(Ogre::Real intensity)
{
	mIntensity = intensity;
	return *this;
}

TextureBufferPtr OilPaint::process()
{
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	int intensities[256];
	int red[256];
	int green[256];
	int blue[256];
	
	for(int y = mRadius; y < (int)(mBuffer->mHeight - mRadius); y++)
    {
		for(int x = mRadius; x < (int)(mBuffer->mWidth - mRadius); x++)
		{
			memset(intensities, 0, 256 * sizeof(int));
			memset(red, 0, sizeof(red));
			memset(green, 0, sizeof(green));
			memset(blue, 0, sizeof(blue));

			for(int j = -mRadius; j <= mRadius; j++)
			{
				for(int i = -mRadius; i <= mRadius; i++)
				{
					int r = mBuffer->getPixelRedByte((size_t)(x + i), (size_t)(y + j));
					int g = mBuffer->getPixelGreenByte((size_t)(x + i), (size_t)(y + j));
					int b = mBuffer->getPixelBlueByte((size_t)(x + i), (size_t)(y + j));

					int curr =  (int)((((Ogre::Real)(r + g + b) / 3.0f) * mIntensity) / 255.0f);
                    if(curr > 255) curr = 255;
                    intensities[curr]++;

					red[curr] += r;
					green[curr] += g;
					blue[curr] += b;
				}
			}
			
			int maxInt = 0;
			int maxIndex = 0;
			for(int i = 0; i < 256; i++)
			{
				if(intensities[i] > maxInt)
				{
					maxInt = intensities[i];
					maxIndex = i;
				}
			}
			
			tmpBuffer->setPixel((size_t)x, (size_t)y, (Ogre::uchar)(red[maxIndex] / maxInt), (Ogre::uchar)(green[maxIndex] / maxInt), (Ogre::uchar)(blue[maxIndex] / maxInt));
		}
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with oilpaint filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RandomPixels & RandomPixels::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

RandomPixels & RandomPixels::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

RandomPixels & RandomPixels::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

RandomPixels & RandomPixels::setSeed(Ogre::uint seed)
{
	mSeed = seed;
	return *this;
}

RandomPixels & RandomPixels::setCount(Ogre::uint count)
{
	mCount = count;
	size_t area = mBuffer->getWidth() * mBuffer->getHeight();
	if(mCount > area) mCount = (size_t)(0.9f * (Ogre::Real)area);
	return *this;
}

TextureBufferPtr RandomPixels::process()
{
	POINT pt;
	std::vector<POINT> list;

	srand(mSeed);
	size_t area = mBuffer->getWidth() * mBuffer->getHeight();
	if(mCount == area)
		Rectangle(mBuffer).setColour(mColour).process();
	else
	{
		while(list.size() != mCount)
		{
			pt.x = rand() % mBuffer->getWidth();
			pt.y = rand() % mBuffer->getHeight();

			bool bInList = false;
			for(std::vector<POINT>::iterator iter = list.begin(); iter != list.end(); iter++)
				if(iter->x == pt.x && iter->y == pt.y)
				{
					bInList = true;
					break;
				}

			if(!bInList)
			{
				list.push_back(pt);
				mBuffer->setPixel(pt.x, pt.y, mColour);
			}
		}
	}

	logMsg("Modify texture with randompixel filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Rectangle & Rectangle::setColour(Ogre::ColourValue colour)
{
	mColour = colour;
	return *this;
}

Rectangle & Rectangle::setColour(Ogre::uchar red, Ogre::uchar green, Ogre::uchar blue, Ogre::uchar alpha)
{
	mColour = Ogre::ColourValue((Ogre::Real)red / 255.0f, (Ogre::Real)green / 255.0f, (Ogre::Real)blue / 255.0f, (Ogre::Real)alpha / 255.0f);
	return *this;
}

Rectangle & Rectangle::setColour(Ogre::Real red, Ogre::Real green, Ogre::Real blue, Ogre::Real alpha)
{
	mColour = Ogre::ColourValue(red, green, blue, alpha);
	return *this;
}

Rectangle & Rectangle::setX1(size_t x1)
{
	mX1 = std::min<size_t>(x1, mBuffer->getWidth());
	return *this;
}

Rectangle & Rectangle::setX1(Ogre::Real x1)
{
	mX1 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(x1, 1.0f));
	return *this;
}

Rectangle & Rectangle::setY1(size_t y1)
{
	mY1 = std::min<size_t>(y1, mBuffer->getHeight());
	return *this;
}

Rectangle & Rectangle::setY1(Ogre::Real y1)
{
	mY1 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(y1, 1.0f));
	return *this;
}

Rectangle & Rectangle::setX2(size_t x2)
{
	mX2 = std::min<size_t>(x2, mBuffer->getWidth());
	return *this;
}

Rectangle & Rectangle::setX2(Ogre::Real x2)
{
	mX2 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(x2, 1.0f));
	return *this;
}

Rectangle & Rectangle::setY2(size_t y2)
{
	mY2 = std::min<size_t>(y2, mBuffer->getHeight());
	return *this;
}

Rectangle & Rectangle::setY2(Ogre::Real y2)
{
	mY2 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(y2, 1.0f));
	return *this;
}

Rectangle & Rectangle::setRectangle(Ogre::RealRect rect, bool relative)
{
	if(relative)
	{
		mX1 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(rect.left, 1.0f));
		mY1 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(rect.top, 1.0f));
		mX2 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(rect.right, 1.0f));
		mY2 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(rect.bottom, 1.0f));
	}
	else
	{
		mX1 = std::min<size_t>((size_t)rect.left, mBuffer->getWidth());
		mY1 = std::min<size_t>((size_t)rect.top, mBuffer->getHeight());
		mX2 = std::min<size_t>((size_t)rect.right, mBuffer->getWidth());
		mY2 = std::min<size_t>((size_t)rect.bottom, mBuffer->getHeight());
	}
	return *this;
}

Rectangle & Rectangle::setRectangle(Ogre::Rect rect)
{
	mX1 = std::min<size_t>(rect.left, mBuffer->getWidth());
	mY1 = std::min<size_t>(rect.top, mBuffer->getHeight());
	mX2 = std::min<size_t>(rect.right, mBuffer->getWidth());
	mY2 = std::min<size_t>(rect.bottom, mBuffer->getHeight());
	return *this;
}

Rectangle & Rectangle::setRectangle(Ogre::Vector2 pos1, Ogre::Vector2 pos2, bool relative)
{
	if(relative)
	{
		mX1 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(pos1.x, 1.0f));
		mY1 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(pos1.y, 1.0f));
		mX2 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(pos2.x, 1.0f));
		mY2 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(pos2.y, 1.0f));
	}
	else
	{
		mX1 = std::min<size_t>((size_t)pos1.x, mBuffer->getWidth());
		mY1 = std::min<size_t>((size_t)pos1.y, mBuffer->getHeight());
		mX2 = std::min<size_t>((size_t)pos2.x, mBuffer->getWidth());
		mY2 = std::min<size_t>((size_t)pos2.y, mBuffer->getHeight());
	}
	return *this;
}

Rectangle & Rectangle::setRectangle(size_t x1, size_t y1, size_t x2, size_t y2)
{
	mX1 = std::min<size_t>(x1, mBuffer->getWidth());
	mY1 = std::min<size_t>(y1, mBuffer->getHeight());
	mX2 = std::min<size_t>(x2, mBuffer->getWidth());
	mY2 = std::min<size_t>(y2, mBuffer->getHeight());
	return *this;
}

Rectangle & Rectangle::setRectangle(Ogre::Real x1, Ogre::Real y1, Ogre::Real x2, Ogre::Real y2)
{
	mX1 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(x1, 1.0f));
	mY1 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(y1, 1.0f));
	mX2 = (size_t)((Ogre::Real)mBuffer->getWidth() * std::min<Ogre::Real>(x2, 1.0f));
	mY2 = (size_t)((Ogre::Real)mBuffer->getHeight() * std::min<Ogre::Real>(y2, 1.0f));
	return *this;
}

#if PROCEDURAL_PLATFORM == PROCEDURAL_PLATFORM_WIN32
Rectangle & Rectangle::setRectangle(POINT pos1, POINT pos2)
{
	mX1 = std::min<size_t>(pos1.x, mBuffer->getWidth());
	mY1 = std::min<size_t>(pos1.y, mBuffer->getHeight());
	mX2 = std::min<size_t>(pos2.x, mBuffer->getWidth());
	mY2 = std::min<size_t>(pos2.y, mBuffer->getHeight());
	return *this;
}

Rectangle & Rectangle::setRectangle(RECT rect)
{
	mX1 = std::min<size_t>(rect.left, mBuffer->getWidth());
	mY1 = std::min<size_t>(rect.top, mBuffer->getHeight());
	mX2 = std::min<size_t>(rect.right, mBuffer->getWidth());
	mY2 = std::min<size_t>(rect.bottom, mBuffer->getHeight());
	return *this;
}
#endif

TextureBufferPtr Rectangle::process()
{
	size_t xStart = std::min<size_t>(mX1, mX2);
	size_t yStart = std::min<size_t>(mY1, mY2);
	size_t xEnd = std::max<size_t>(mX1, mX2);
	size_t yEnd = std::max<size_t>(mY1, mY2);

	for(size_t y = yStart; y < yEnd; y++)
	{
		for(size_t x = xStart; x < xEnd; x++)
		{
			mBuffer->setPixel(x, y, mColour);
		}
	}

	logMsg("Modify texture with rectangle filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RotationZoom & RotationZoom::setCenterX(Ogre::Real centerx)
{
	mCenterX = centerx;
	return *this;
}

RotationZoom & RotationZoom::setCenterY(Ogre::Real centery)
{
	mCenterY = centery;
	return *this;
}

RotationZoom & RotationZoom::setZoomX(Ogre::Real zoomx)
{
	mZoomX = zoomx;
	return *this;
}

RotationZoom & RotationZoom::setZoomY(Ogre::Real zoomy)
{
	mZoomY = zoomy;
	return *this;
}

RotationZoom & RotationZoom::setRotation(Ogre::Real rotation)
{
	mRotation = Ogre::Real(rotation * Ogre::Math::TWO_PI);
	return *this;
}

RotationZoom & RotationZoom::setRotation(Ogre::Radian rotation)
{
	mRotation = rotation;
	return *this;
}

RotationZoom & RotationZoom::setRotation(Ogre::Degree rotation)
{
	mRotation = Ogre::Radian(rotation.valueRadians());
	return *this;
}

RotationZoom & RotationZoom::setWrap(bool wrap)
{
	mWrap = wrap;
	return *this;
}

TextureBufferPtr RotationZoom::process()
{
	unsigned long tw = mBuffer->getWidth();
	unsigned long th = mBuffer->getHeight();
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	Ogre::Real fZoomX = Ogre::Math::Pow(0.5f, mZoomX - 1);
	Ogre::Real fZoomY = Ogre::Math::Pow(0.5f, mZoomY - 1);
	Ogre::Real c = Ogre::Math::Cos(mRotation.valueRadians());
	Ogre::Real s = Ogre::Math::Sin(mRotation.valueRadians());
	Ogre::Real tw2 = (Ogre::Real)tw / 2.0f;
	Ogre::Real th2 = (Ogre::Real)th / 2.0f;
	Ogre::Real ys = s * -th2;
	Ogre::Real yc = c * -th2;

	for(unsigned long y = 0; y < mBuffer->mHeight; y++)
	{
		Ogre::Real u = (((c * -tw2) - ys) * fZoomX) + (mCenterX * (Ogre::Real)tw);
		Ogre::Real v = (((s * -tw2) + yc) * fZoomY) + (mCenterY * (Ogre::Real)th);
		for(unsigned long x = 0; x < mBuffer->mWidth; x++)
		{
			Ogre::Real uf = (u >= 0) ? (u - (long)u) : 1 + (u - (long)u);
			Ogre::Real vf = (v >= 0) ? (v - (long)v) : 1 + (v - (long)v);
			unsigned long ut = (u >= 0) ? (unsigned long)u : (unsigned long)u - 1;
			unsigned long vt = (v >= 0) ? (unsigned long)v : (unsigned long)v - 1;

			Ogre::ColourValue texel = mBuffer->getPixel(vt % th, ut % tw) * (1.0f - uf) * (1.0f - vf);
			texel += mBuffer->getPixel(vt % th, (ut + 1) % tw) * uf * (1.0f - vf);
			texel += mBuffer->getPixel((vt + 1) % th, ut % tw) * (1.0f - uf) * vf;
			texel += mBuffer->getPixel((vt + 1) % th, (ut + 1) % tw) * uf * vf;
			tmpBuffer->setPixel(x, y, texel);
			u += c * fZoomX;
			v += s * fZoomY;
		}
		ys += s;
		yc += c;
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with rotationzoom filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Segment & Segment::setColourSource(TextureBufferPtr coloursource)
{
	mColourSource = coloursource;
	return *this;
}

Segment & Segment::setThreshold(Ogre::uchar threshold)
{
	mThreshold = threshold;
	return *this;
}

TextureBufferPtr Segment::process()
{
	if(mColourSource == NULL) return mBuffer;

	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();

	if(mColourSource->getWidth() < w || mColourSource->getHeight() < h) return mBuffer;

	Ogre::uchar* pCoverage = new Ogre::uchar[w * h];
	memset(pCoverage, 0, w * h);
	POINT* pStack = new POINT[w * h * 4];
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	size_t stackPtr = 0;
	for(size_t y = 0; y < h; y++)
	{
		for(size_t x = 0; x < w; x++)
		{
			Ogre::ColourValue pixelA = mBuffer->getPixel(x, y);
			Ogre::ColourValue pixelB = mColourSource->getPixel(x, y);

			if((pixelA.r + pixelA.g + pixelA.b) * 255.0f > (Ogre::Real)mThreshold * 3.0f)
			{
				pStack[stackPtr].x = x;
				pStack[stackPtr].y = y;
				stackPtr++;
			}

			while(stackPtr > 0)
			{
				POINT current = pStack[--stackPtr];
				if(pCoverage[current.x + current.y * w] != 0)
					continue;

				pixelA = mBuffer->getPixel(current.x, current.y);
				if((pixelA.r + pixelA.g + pixelA.b) * 255.0f > (Ogre::Real)mThreshold * 3.0f)
				{
					pStack[stackPtr].x = current.x;
					pStack[stackPtr].y = (current.y + h - 1) % h;
					stackPtr++;
					pStack[stackPtr].x = current.x;
					pStack[stackPtr].y = (current.y + 1) % h;
					stackPtr++;
					pStack[stackPtr].x = (current.x + 1) % w;
					pStack[stackPtr].y = current.y ;
					stackPtr++;
					pStack[stackPtr].x = (current.x + w - 1) % w;
					pStack[stackPtr].y = current.y ;
					stackPtr++;

					pCoverage[current.x + current.y * w] = 1;
					tmpBuffer->setPixel(current.x, current.y, pixelB);
				}
			}

			if (pCoverage[x + y * w] == 0)
				tmpBuffer->setPixel(x, y, Ogre::ColourValue::Black);
		}
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;
	delete pCoverage;
	delete pStack;

	logMsg("Modify texture with segment filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sharpen & Sharpen::setSize(Ogre::uchar size)
{
	mSize = size;
	if(mSize < 3) mSize = 3;
	if((mSize % 2) == 0) mSize++;
	return *this;
}

Sharpen & Sharpen::setSigma(Ogre::uchar sigma)
{
	mSigma = sigma;
	return *this;
}

Sharpen & Sharpen::setType(Sharpen::SHARP_TYPE type)
{
	mType = type;
	return *this;
}

TextureBufferPtr Sharpen::process()
{
	Convolution filter(mBuffer);
	switch(mType)
	{
		default:
		case SHARP_BASIC:
			filter.setKernel(Ogre::Matrix3(0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f, 0.0f)).calculateDivisor();
			break;

		case SHARP_GAUSSIAN:
			Ogre::Real fSigma = 0.5f + ((5.0f - 0.5f) / 255.0f) * (Ogre::Real)mSigma;
			int r = (int)mSize / 2;
			double min = Ogre::Math::Exp((Ogre::Real)(2 * r * r) / (-2.0f * fSigma * fSigma)) / (Ogre::Math::TWO_PI * fSigma * fSigma);
			int* kernel = new int[mSize * mSize];
			int sum = 0;
			int y = -r;
			int x = -r;
			for(int i = 0; i < mSize; i++)
			{
				for(int j = 0; j < mSize; j++)
				{
					kernel[i * mSize + j] = (int)((Ogre::Math::Exp((Ogre::Real)(x * x + y * y) / (-2.0f * fSigma * fSigma)) / (Ogre::Math::TWO_PI * fSigma * fSigma)) / min);
					sum += kernel[i * mSize + j];
					x++;
				}
				y++;
			}
			int c = (int)mSize >> 1;
			int divisor = 0;
			for(int i = 0; i < mSize; i++)
			{
				for(int j = 0; j < mSize; j++)
				{
					if((i == c) && (j == c))
						kernel[i * mSize + j] = 2 * sum - kernel[i * mSize + j];
					else
						kernel[i * mSize + j] = -kernel[i * mSize + j];

					divisor += kernel[i * mSize + j];
				}
			}
			filter.setKernel(mSize, kernel).setDivisor((Ogre::Real)divisor);
			delete kernel;
			break;
	}
	filter.setIncludeAlphaChannel(true).process();

	logMsg("Modify texture with sharpen filter : " + StringConverter::toString(mType));
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Threshold & Threshold::setThreshold(Ogre::uchar threshold)
{
	mThreshold = threshold;
	return *this;
}

Threshold & Threshold::setRatio(Ogre::uchar ratio)
{
	mRatio = ratio;
	return *this;
}

Threshold & Threshold::setMode(Threshold::THRESHOLD_MODE mode)
{
	mMode = mode;
	return *this;
}

TextureBufferPtr Threshold::process()
{
	long t;
	size_t w = mBuffer->getWidth();
	size_t h = mBuffer->getHeight();
	Ogre::Real ratio = (mMode == MODE_EXPAND_DOWNWARDS || mMode == MODE_EXPAND_UPWARDS) ? 1 + mRatio * 0.1f : 1 + mRatio * 0.05f;

	for(size_t y = 0; y < h; y++)
	{
		for(size_t x = 0; x < w; x++)
		{
			Ogre::uchar r = mBuffer->getPixelRedByte(x, y);
			Ogre::uchar g = mBuffer->getPixelGreenByte(x, y);
			Ogre::uchar b = mBuffer->getPixelBlueByte(x, y);
			Ogre::uchar a = mBuffer->getPixelAlphaByte(x, y);

			if(mMode == MODE_EXPAND_DOWNWARDS)
			{
				if(r < mThreshold)
				{
					t = mThreshold - (long)((mThreshold - r) * ratio);
					r = (t < 0) ? 0 : (Ogre::uchar)t;
				}
				if(g < mThreshold)
				{
					t = mThreshold - (long)((mThreshold - g) * ratio);
					g = (t < 0) ? 0 : (Ogre::uchar)t;
				}
				if(b < mThreshold)
				{
					t = mThreshold - (long)((mThreshold - b) * ratio);
					b = (t < 0) ? 0 : (Ogre::uchar)t;
				}
			}
			else if(mMode == MODE_EXPAND_UPWARDS)
			{
				if(r > mThreshold)
				{
					t = (long)((r - mThreshold) * ratio) - mThreshold;
					r = (t > 255) ? 255 : (Ogre::uchar)t;
				}
				if(g > mThreshold)
				{
					t = (long)((g - mThreshold) * ratio) - mThreshold;
					g = (t > 255) ? 255 : (Ogre::uchar)t;
				}
				if(b > mThreshold)
				{
					t = (long)((b - mThreshold) * ratio) - mThreshold;
					b = (t > 255) ? 255 : (Ogre::uchar)t;
				}
			}
			else if(mMode == MODE_COMPRESS_BELOW)
			{
				if(r < mThreshold)
				{
					t = mThreshold - (long)((mThreshold - r) / ratio);
					r = (t < 0) ? 0 : (Ogre::uchar)t;
				}
				if(g < mThreshold)
				{
					t = mThreshold - (long)((mThreshold - g) / ratio);
					g = (t < 0) ? 0 : (Ogre::uchar)t;
				}
				if(b < mThreshold)
				{
					t = mThreshold - (long)((mThreshold - b) / ratio);
					b = (t < 0) ? 0 : (Ogre::uchar)t;
				}
			}
			else if(mMode == MODE_COMPRESS_ABOVE)
			{
				if(r > mThreshold)
				{
					t = (long)((r - mThreshold) / ratio) - mThreshold;
					r = (t > 255) ? 255 : (Ogre::uchar)t;
				}
				if(g > mThreshold)
				{
					t = (long)((g - mThreshold) / ratio) - mThreshold;
					g = (t > 255) ? 255 : (Ogre::uchar)t;
				}
				if(b > mThreshold)
				{
					t = (long)((b - mThreshold) / ratio) - mThreshold;
					b = (t > 255) ? 255 : (Ogre::uchar)t;
				}
			}
			mBuffer->setPixel(x, y, r, g, b, a);
		}
	}

	logMsg("Modify texture with threshold filter");
	return mBuffer;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vortex & Vortex::setCenterX(Ogre::Real centerx)
{
	mCenterX = centerx;
	return *this;
}

Vortex & Vortex::setCenterY(Ogre::Real centery)
{
	mCenterY = centery;
	return *this;
}

Vortex & Vortex::setRadiusX(Ogre::Real radiusx)
{
	mRadiusX = radiusx;
	return *this;
}

Vortex & Vortex::setRadiusY(Ogre::Real radiusy)
{
	mRadiusY = radiusy;
	return *this;
}

Vortex & Vortex::setTwist(Ogre::Real twist)
{
	mTwist = Ogre::Radian(twist * Ogre::Math::TWO_PI);
	return *this;
}

Vortex & Vortex::setTwist(Ogre::Radian twist)
{
	mTwist = twist;
	return *this;
}

Vortex & Vortex::setTwist(Ogre::Degree twist)
{
	mTwist = twist;
	return *this;
}

TextureBufferPtr Vortex::process()
{
	long w = (long)mBuffer->getWidth();
	long h = (long)mBuffer->getHeight();
	long dwCenterX	= (long)(mCenterX * (Ogre::Real)w);
	long dwCenterY	= (long)(mCenterY * (Ogre::Real)h);
	long dwRadiusX	= (long)(mRadiusX * (Ogre::Real)w);
	long dwRadiusY	= (long)(mRadiusY * (Ogre::Real)h);
	Ogre::Real f1_RadiusX = 1.0f / (Ogre::Real)dwRadiusX;
	Ogre::Real f1_RadiusY = 1.0f / (Ogre::Real)dwRadiusY;
	TextureBufferPtr tmpBuffer = mBuffer->clone();

	for(long y = 0; y < h; y++)
	{
		Ogre::Real dy = (Ogre::Real)(y - dwCenterY) * f1_RadiusY;

		for(long x = 0; x < w; x++)
		{
			Ogre::Real dx = (Ogre::Real)(x - dwCenterX) * f1_RadiusX;
			Ogre::Real d = sqrt(dx * dx + dy * dy);

			if(d > 1.0f)
				tmpBuffer->setPixel(x, y, mBuffer->getPixel(x, y));
			else
			{
				d = Ogre::Math::Cos(d * Ogre::Math::HALF_PI - Ogre::Math::HALF_PI);
				d = 1.0f - d;
				Ogre::Real nx = (Ogre::Real)(x - dwCenterX);
				Ogre::Real ny = (Ogre::Real)(y - dwCenterY);
				Ogre::Real rad = mTwist.valueRadians() * d;

				Ogre::Real bx = nx;
				nx = bx * Ogre::Math::Cos(rad) - ny * Ogre::Math::Sin(rad) + dwCenterX;
				ny = bx * Ogre::Math::Sin(rad) + ny * Ogre::Math::Cos(rad) + dwCenterY;

				if(nx >= w) nx = nx - w;
				if(ny >= h) ny = ny - h;
				if(nx < 0) nx = w + nx;
				if(ny < 0) ny = h + ny;

				int ix = (int)nx;
				int iy = (int)ny;

				Ogre::Real fracX = nx - ix;
				Ogre::Real fracY = ny - iy;

				Ogre::Real ul = (1.0f - fracX) * (1.0f - fracY);
				Ogre::Real ll = (1.0f - fracX) * fracY;
				Ogre::Real ur = fracX * (1.0f - fracY);
				Ogre::Real lr = fracX * fracY;

				int wrapx = (ix + 1) % w;
				int wrapy = (iy + 1) % h;
				Ogre::ColourValue pixelUL = mBuffer->getPixel(ix, iy);
				Ogre::ColourValue pixelUR = mBuffer->getPixel(wrapx, iy);
				Ogre::ColourValue pixelLL = mBuffer->getPixel(ix, wrapy);
				Ogre::ColourValue pixelLR = mBuffer->getPixel(wrapx, wrapy);

				tmpBuffer->setPixel(x, y,
					(Ogre::uchar)(ul * pixelUL.r * 255.0f + ll * pixelLL.r * 255.0f + ur * pixelUR.r * 255.0f + lr * pixelLR.r * 255.0f),
					(Ogre::uchar)(ul * pixelUL.g * 255.0f + ll * pixelLL.g * 255.0f + ur * pixelUR.g * 255.0f + lr * pixelLR.g * 255.0f),
					(Ogre::uchar)(ul * pixelUL.b * 255.0f + ll * pixelLL.b * 255.0f + ur * pixelUR.b * 255.0f + lr * pixelLR.b * 255.0f),
					(Ogre::uchar)(ul * pixelUL.a * 255.0f + ll * pixelLL.a * 255.0f + ur * pixelUR.a * 255.0f + lr * pixelLR.a * 255.0f));
			}
		}
	}

	mBuffer->setData(tmpBuffer);
	delete tmpBuffer;

	logMsg("Modify texture with vortex filter : " + StringConverter::toString(mTwist.valueDegrees()));
	return mBuffer;
}
}
