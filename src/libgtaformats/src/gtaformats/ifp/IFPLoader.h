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

#ifndef IFPLOADER_H_
#define IFPLOADER_H_

#include "IFPAnimation.h"
#include "../util/File.h"
#include <istream>

using std::istream;



class IFPLoader {
public:
	IFPLoader(istream* stream);
	IFPLoader(const File& file);
	~IFPLoader();
	IFPAnimation* readAnimation();
	const char* getName() const { return ifpName; }
	int32_t getAnimationCount() const { return numAnims; }

private:
	void init();

private:
	istream* stream;
	char ifpName[25];
	int32_t endOffs;
	int32_t numAnims;
};

#endif /* IFPLOADER_H_ */
