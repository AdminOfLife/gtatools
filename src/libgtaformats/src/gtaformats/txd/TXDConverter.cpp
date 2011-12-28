/*
	Copyright 2010-2011 David "Alemarius Nexus" Lerch

	This file is part of gtaformats.

	gtaformats is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	gtaformats is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with gtaformats.  If not, see <http://www.gnu.org/licenses/>.

	Additional permissions are granted, which are listed in the file
	GPLADDITIONS.
 */

#include "TXDConverter.h"
#include <gtaformats/config.h>
#include <cmath>
#include <cstring>

#ifdef GTAFORMATS_ENABLE_SQUISH
#include <squish.h>
using namespace squish;
#endif

#ifdef GTAFORMATS_ENABLE_PVRTEXLIB
#include <PVRTexLib.h>
using namespace pvrtexlib;
#endif






TXDConverter::TXDConverter()
		: comprHint(CompressionHintSlow)
{
}


int32_t TXDConverter::convert(const TXDTextureHeader& from, const TXDTextureHeader& to,
		const uint8_t* fromData, uint8_t* toData, int8_t mipStart, int8_t mipEnd)
{
	if (!canConvert(from, to)) {
		return -1;
	}

	if (mipEnd == -1) {
		mipEnd = to.getMipmapCount()-1;
	}

	char fromFormatDesc[256];
	from.getFormat(fromFormatDesc);
	char toFormatDesc[256];
	to.getFormat(toFormatDesc);

	int16_t mipW = from.getWidth() / pow(2.0f, mipStart);
	int16_t mipH = from.getHeight() / pow(2.0f, mipStart);

	for (int8_t mipNum = mipStart ; mipNum <= mipEnd ; mipNum++) {
		TXDCompression fromCompr = from.getCompression();
		TXDCompression toCompr = to.getCompression();

		// The intermediate format is the format of the input data after decompression. For uncompressed input
		// data, it's the same as for the input data. The intermediate data is the data created by
		// decompression or a pointer to the input data for uncompressed data.
		int32_t intermediateFormat = from.getFullRasterFormat();
		const uint8_t* intermediateData = fromData;

		// The out format is the format which should be generated by the raster conversion loop. The
		// conversion is done into the out data.
		int32_t outFormat = to.getRasterFormat();
		uint8_t* outDataWritable = toData;
		const uint8_t* outData = outDataWritable;

		// If compressed, decompress into an intermediate format (R8G8B8A8)
		if (fromCompr == DXT1  ||  fromCompr == DXT3) {
			uint8_t* decompressedData;

			if (outFormat == RasterFormatR8G8B8A8) {
				decompressedData = toData;
			} else {
				decompressedData = new uint8_t[mipW * mipH * 4];
			}

			if (fromCompr == DXT1) {
#ifdef GTAFORMATS_ENABLE_SQUISH
				DecompressImage(decompressedData, mipW, mipH, fromData, kDxt1);
#endif
			} else if (fromCompr == DXT3) {
#ifdef GTAFORMATS_ENABLE_SQUISH
				DecompressImage(decompressedData, mipW, mipH, fromData, kDxt3);
#endif
			}

			intermediateFormat = RasterFormatR8G8B8A8;
			intermediateData = decompressedData;
		} else if (fromCompr == PVRTC2  ||  fromCompr == PVRTC4) {
			uint8_t* decompressedData;

			if (outFormat == RasterFormatR8G8B8A8) {
				decompressedData = toData;
			} else {
				decompressedData = new uint8_t[mipW * mipH * 4];
			}

#ifdef GTAFORMATS_ENABLE_PVRTEXLIB
			PixelType pvrInType;

			if (fromCompr == PVRTC2) {
				pvrInType = OGL_PVRTC2;
			} else {
				pvrInType = OGL_PVRTC4;
			}

			CPVRTexture pvrInTex(mipW, mipH, 0, 1, pvrInType);
			pvrInTex.setData(const_cast<uint8_t*>(fromData));

			CPVRTexture pvrOutTex(mipW, mipH, 0, 1, eInt8StandardPixelType);

			PVRTextureUtilities pvr;
			pvr.DecompressPVR(pvrInTex, pvrOutTex, 0);

			memcpy(decompressedData, pvrOutTex.getData().getData(), mipW * mipH * 4);
#endif

			intermediateFormat = RasterFormatR8G8B8A8;
			intermediateData = decompressedData;
		}

		// For DXT or PVRTC compression, we need R8G8B8A8. If the input data was already compressed, this
		// intermediate format was already generated by decompression. Otherwise, we have to to it in the
		// raster conversion loop, so we just set the out format to R8G8B8A8.
		if (toCompr == DXT1  ||  toCompr == DXT3  ||  toCompr == PVRTC2  ||  toCompr == PVRTC4) {
			outFormat = RasterFormatR8G8B8A8;

			if (intermediateFormat != outFormat) {
				outDataWritable = new uint8_t[mipW * mipH * 4];
				outData = outDataWritable;
			}
		}

		bool pal4 = false;
		bool pal8 = false;

		if ((intermediateFormat & RasterFormatEXTPAL4)  !=  0) {
			pal4 = true;
		} else if ((intermediateFormat & RasterFormatEXTPAL8)  !=  0) {
			pal8 = true;
		}

		if (intermediateFormat != outFormat) {
			// Get specifics of the intermediate raster format
			int32_t rm, gm, bm, am; // Masks to extract the various colors from a pixel
			int32_t rs, gs, bs, as; // Right-shift value to get the actual value from extracted color data.
			uint8_t rMax, gMax, bMax, aMax; // Maximum values for the colors
			int8_t bpp; // BYTES per pixel
			getFormatSpecifics(intermediateFormat, rm, gm, bm, am, rs, gs, bs, as, rMax, gMax, bMax, aMax,
					bpp);

			// Get specifics of the output raster format
			int32_t orm, ogm, obm, oam;
			int32_t ors, ogs, obs, oas;
			uint8_t orMax, ogMax, obMax, oaMax;
			int8_t obpp;
			getFormatSpecifics(outFormat, orm, ogm, obm, oam, ors, ogs, obs, oas, orMax, ogMax, obMax, oaMax,
					obpp);

			int32_t outCombinedMask = rm | gm | bm | am;

			const uint8_t* intermediateDataPtr = intermediateData;
			uint8_t* outDataPtr = outDataWritable;

			bool alpha = from.hasAlphaChannel();

			const uint8_t* palette = NULL;

			if (pal4) {
				palette = intermediateDataPtr;
				intermediateDataPtr += 16*4;
			} else if (pal8) {
				palette = intermediateDataPtr;
				intermediateDataPtr += 256*4;
			}

			// Convert the raster pixel by pixel
			for (int16_t y = 0 ; y < mipH ; y++) {
				for (int16_t x = 0 ; x < mipW ; x++) {
					uint32_t pixel = *((int32_t*) intermediateDataPtr);

					if (pal4) {
						pixel = ((uint32_t*) palette)[(pixel & 0xF)];
					} else if (pal8) {
						pixel = ((uint32_t*) palette)[(pixel & 0xFF)];
					}

					uint8_t r = (pixel & rm) >> rs;
					uint8_t g = (pixel & gm) >> gs;
					uint8_t b = (pixel & bm) >> bs;
					uint8_t a = (pixel & am) >> as;

					uint8_t modR = (r * orMax) / rMax;
					uint8_t modG = (g * ogMax) / gMax;
					uint8_t modB = (b * obMax) / bMax;
					uint8_t modA;

					if (alpha)
						modA = (float) (a * oaMax) / aMax;
					else
						modA = oaMax;

					uint32_t& outPixel = *((uint32_t*) outDataPtr);
					outPixel &= ~outCombinedMask;
					outPixel |= (((uint32_t) modR) << ors);
					outPixel |= (((uint32_t) modG) << ogs);
					outPixel |= (((uint32_t) modB) << obs);
					outPixel |= (((uint32_t) modA) << oas);

					intermediateDataPtr += bpp;
					outDataPtr += obpp;
				}
			}
		} else {
			outData = intermediateData;
		}

		// If DXT or PVRTC was requested, we have R8G8B8A8 in the out data and are ready to compress
		if (toCompr == DXT1  ||  toCompr == DXT3) {
#ifdef GTAFORMATS_ENABLE_SQUISH
			int dxtFlags = 0;
			switch (comprHint) {
			case CompressionHintFast:
				dxtFlags = kColourRangeFit;
				break;
			case CompressionHintSlow:
				dxtFlags = kColourClusterFit;
				break;
			case CompressionHintVerySlow:
				dxtFlags = kColourIterativeClusterFit;
				break;
			}

			if (toCompr == DXT1) {
				CompressImage(outData, mipW, mipH, toData, kDxt1);
			} else {
				CompressImage(outData, mipW, mipH, toData, kDxt3);
			}
#endif
		} else if (toCompr == PVRTC2  ||  toCompr == PVRTC4) {
#ifdef GTAFORMATS_ENABLE_PVRTEXLIB
			CPVRTexture pvrInTex(mipW, mipH, 0, 1, eInt8StandardPixelType);
			pvrInTex.setData(const_cast<uint8_t*>(outData));

			PixelType pvrOutType;
			int pvrMode;

			switch (comprHint) {
			case CompressionHintFast:
				pvrMode = ePVRTC_FAST;
				break;
			case CompressionHintSlow:
				pvrMode = ePVRTC_HIGH;
				break;
			case CompressionHintVerySlow:
				pvrMode = ePVRTC_BEST;
				break;
			}

			if (toCompr == PVRTC2) {
				pvrOutType = OGL_PVRTC2;
			} else {
				pvrOutType = OGL_PVRTC4;
			}

			CPVRTexture pvrOutTex(mipW, mipH, 0, 1, pvrOutType);

			PVRTextureUtilities pvr;
			pvr.CompressPVR(pvrInTex, pvrOutTex, false, pvrMode);

			memcpy(toData, pvrOutTex.getData().getData(), pvrOutTex.getData().getDataSize());
#endif
		}

		// Delete the intermediate data if it was dynamically allocated by this method
		if (intermediateData != fromData  &&  intermediateData != toData  &&  intermediateData != outData) {
			delete[] intermediateData;
		}

		// Delete the out data if it was dynamically allocated by this method
		if (outData != toData  &&  outData != fromData) {
			delete[] outData;
		}

		if (fromData == outData) {
			memcpy(toData, fromData, from.getBytesPerPixel() * mipW * mipH);
		}

		fromData += from.computeMipmapDataSize(mipNum);
		toData += to.computeMipmapDataSize(mipNum);

		mipW /= 2;
		mipH /= 2;
	}

	return to.computeDataSize();
}


uint8_t* TXDConverter::convert(const TXDTextureHeader& from, const TXDTextureHeader& to,
		const uint8_t* fromData, int8_t mipStart, int8_t mipEnd)
{
	char desc[128];
	to.getFormat(desc);
	uint8_t* toData = new uint8_t[to.computeDataSize()];
	int32_t size = convert(from, to, fromData, toData, mipStart, mipEnd);

	if (size == -1) {
		return NULL;
	} else {
		return toData;
	}
}


bool TXDConverter::canConvert(const TXDTextureHeader& from, const TXDTextureHeader& to)
{
	// Find out what has changed
	bool rasterFormatChanged = (from.getFullRasterFormat() != to.getFullRasterFormat());
	bool sizeChanged = (from.getWidth() != to.getWidth())  ||  (from.getHeight() != to.getHeight());
	bool comprChanged = (from.getCompression() != to.getCompression());
	bool mipmapCountChanged = (from.getMipmapCount() != to.getMipmapCount());

	if (sizeChanged) {
		return false;
	}

	// Mipmap generation is not supported, but removing mipmaps will work.
	if (to.getMipmapCount() > from.getMipmapCount()) {
		return false;
	}

	// Check if the operation is supported
	TXDCompression fromCompr = from.getCompression();
	TXDCompression toCompr = to.getCompression();

	if (fromCompr == PVRTC2  ||  fromCompr == PVRTC4 ||  toCompr == PVRTC2  ||  toCompr == PVRTC4) {
		// No support for PVRTC. Only information which doesn't involve raster changes are allowed.
#ifdef GTAFORMATS_ENABLE_PVRTEXLIB
		// PVRTC4 uses 4x4 pixel blocks to compress, so both width and height must be dividable by 4
		if (fromCompr == PVRTC4  &&  (from.getWidth()%4 != 0  ||  from.getHeight()%4 != 0))
			return false;
		if (toCompr == PVRTC4  &&  (to.getWidth()%4 != 0  ||  to.getHeight()%4 != 0))
			return false;

		// PVRTC4 uses 8x4 pixel blocks to compress, so width must be dividable by 8, and height must be
		// dividable by 4
		if (fromCompr == PVRTC2  &&  (from.getWidth()%8 != 0  ||  from.getHeight()%4 != 0))
			return false;
		if (toCompr == PVRTC2  &&  (to.getWidth()%8 != 0  ||  to.getHeight()%4 != 0))
			return false;
#else
		if (rasterFormatChanged  ||  sizeChanged  ||  comprChanged  ||  mipmapCountChanged) {
			return false;
		}
#endif
	} else if (fromCompr == DXT1  ||  fromCompr == DXT3  ||  toCompr == DXT1  ||  toCompr == DXT3) {
		// libsquish is needed for DXT (de-)compression
#ifdef GTAFORMATS_ENABLE_SQUISH
		// DXTn uses 4x4 pixel blocks to compress, so both width and height must be dividable by 4
		if (to.getWidth() % 4 != 0  ||  to.getHeight() % 4 != 0) {
			return false;
		}
#else
		if (rasterFormatChanged  ||  sizeChanged  ||  comprChanged  ||  mipmapCountChanged) {
			return false;
		}
#endif
	} else {
		if (to.getFullRasterFormat() == RasterFormatDefault) {
			// Makes no sense for uncompressed textures
			return false;
		}
	}

	// Conversion to paletted format is currently not supported
	if (	(to.getRasterFormatExtension() & RasterFormatEXTPAL4)  !=  0
			||  (to.getRasterFormatExtension() & RasterFormatEXTPAL4)  !=  0
	) {
		if (from.getFullRasterFormat() != to.getFullRasterFormat()  ||  fromCompr != toCompr) {
			return false;
		}
	}

	return true;
}


void TXDConverter::getFormatSpecifics(int32_t rasterFormat, int32_t& rm, int32_t& gm, int32_t& bm, int32_t& am,
		int32_t& rs, int32_t& gs, int32_t& bs, int32_t& as, uint8_t& rMax, uint8_t& gMax, uint8_t& bMax,
		uint8_t& aMax, int8_t& bpp)
{
	if (	(rasterFormat & RasterFormatEXTPAL4)  !=  0
			||  (rasterFormat & RasterFormatEXTPAL8)  !=  0
	) {
		rm = 0x000000FF;
		gm = 0x0000FF00;
		bm = 0x00FF0000;
		am = 0xFF000000;
		bpp = 1;
	} else {
		switch (rasterFormat & RasterFormatMask) {
		case RasterFormatA1R5G5B5:
			am = 0x0001;
			rm = 0x003E;
			gm = 0x07C0;
			bm = 0xF800;
			bpp = 2;
			break;
		case RasterFormatB8G8R8:
			am = 0x000000;
			bm = 0x0000FF;
			gm = 0x00FF00;
			rm = 0xFF0000;
			bpp = 4;
			break;
		case RasterFormatB8G8R8A8:
			bm = 0x000000FF;
			gm = 0x0000FF00;
			rm = 0x00FF0000;
			am = 0xFF000000;
			bpp = 4;
			break;
		case RasterFormatLUM8:
			rm = 0xFF;
			gm = 0xFF;
			bm = 0xFF;
			am = 0x0;
			bpp = 1;
			break;
		case RasterFormatR4G4B4A4:
			rm = 0x000F;
			gm = 0x00F0;
			bm = 0x0F00;
			am = 0xF000;
			bpp = 2;
			break;
		case RasterFormatR5G5B5:
			rm = 0x001F;
			gm = 0x03E0;
			bm = 0x7C00;
			am = 0x0000;
			bpp = 2;
			break;
		case RasterFormatR5G6B5:
			rm = 0x001F;
			gm = 0x07E0;
			bm = 0x7C00;
			am = 0xF800;
			bpp = 2;
			break;
		case RasterFormatR8G8B8A8:
			rm = 0x000000FF;
			gm = 0x0000FF00;
			bm = 0x00FF0000;
			am = 0xFF000000;
			bpp = 4;
			break;
		}
	}

	for (int i = 0 ; i < 32 ; i++) {
		if ((rm & (1 << i)) != 0) {
            rs = i;
            break;
        }
	}
	for (int i = 0 ; i < 32 ; i++) {
		if ((gm & (1 << i)) != 0) {
            gs = i;
            break;
        }
	}
	for (int i = 0 ; i < 32 ; i++) {
		if ((bm & (1 << i)) != 0) {
            bs = i;
            break;
        }
	}
	for (int i = 0 ; i < 32 ; i++) {
		if ((am & (1 << i)) != 0) {
            as = i;
            break;
        }
	}

	rMax = rm >> rs;
	gMax = gm >> gs;
	bMax = bm >> bs;
	aMax = am >> as;
}
