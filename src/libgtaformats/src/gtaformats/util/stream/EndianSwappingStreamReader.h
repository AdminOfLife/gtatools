/*
 * EndianSwappingStreamReader.h
 *
 *  Created on: 22.12.2011
 *      Author: alemariusnexus
 */

#ifndef ENDIANSWAPPINGSTREAMREADER_H_
#define ENDIANSWAPPINGSTREAMREADER_H_

#include "StreamReader.h"
#include "../util.h"


class EndianSwappingStreamReader : public StreamReader
{
public:
	EndianSwappingStreamReader(istream* stream) : StreamReader(stream) {}

	virtual void readU16(uint16_t* dest) { s->read((char*) dest, 2); *dest = SwapEndianness16(*dest); }
	virtual void readU32(uint32_t* dest) { s->read((char*) dest, 4); *dest = SwapEndianness32(*dest); }
	virtual void readU64(uint64_t* dest) { s->read((char*) dest, 8); *dest = SwapEndianness64(*dest); }
	virtual void read16(int16_t* dest) { s->read((char*) dest, 2); *dest = SwapEndianness16(*dest); }
	virtual void read32(int32_t* dest) { s->read((char*) dest, 4); *dest = SwapEndianness32(*dest); }
	virtual void read64(int64_t* dest) { s->read((char*) dest, 8); *dest = SwapEndianness64(*dest); }
	virtual void readFloat(float* dest) { s->read((char*) dest, 4); *dest = SwapEndiannessF32(*dest); }
	virtual void readDouble(double* dest) { s->read((char*) dest, 8); *dest = SwapEndiannessF64(*dest); }

	virtual void readArrayU16(uint16_t* dest, size_t num)
	{ s->read((char*) dest, num*2); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndianness16(dest[i]); }

	virtual void readArrayU32(uint32_t* dest, size_t num)
	{ s->read((char*) dest, num*4); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndianness32(dest[i]); }

	virtual void readArrayU64(uint64_t* dest, size_t num)
	{ s->read((char*) dest, num*8); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndianness64(dest[i]); }

	virtual void readArray16(int16_t* dest, size_t num)
	{ s->read((char*) dest, num*2); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndianness16(dest[i]); }

	virtual void readArray32(int32_t* dest, size_t num)
	{ s->read((char*) dest, num*4); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndianness32(dest[i]); }

	virtual void readArray64(int64_t* dest, size_t num)
	{ s->read((char*) dest, num*8); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndianness64(dest[i]); }

	virtual void readArrayFloat(float* dest, size_t num)
	{ s->read((char*) dest, num*4); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndiannessF32(dest[i]); }

	virtual void readArrayDouble(double* dest, size_t num)
	{ s->read((char*) dest, num*8); for (size_t i = 0 ; i < num ; i++) dest[i] = SwapEndiannessF64(dest[i]); }

	using StreamReader::readU8;
	using StreamReader::readU16;
	using StreamReader::readU32;
	using StreamReader::readU64;
	using StreamReader::read8;
	using StreamReader::read16;
	using StreamReader::read32;
	using StreamReader::read64;
	using StreamReader::readFloat;
	using StreamReader::readDouble;
};

#endif /* ENDIANSWAPPINGSTREAMREADER_H_ */