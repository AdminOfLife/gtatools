/*
	Copyright 2010-2014 David "Alemarius Nexus" Lerch

	This file is part of libgta.

	libgta is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libgta is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with libgta.  If not, see <http://www.gnu.org/licenses/>.

	Additional permissions are granted, which are listed in the file
	GPLADDITIONS.
 */

#ifndef NULLTEXTURESOURCE_H_
#define NULLTEXTURESOURCE_H_

#include <gta/config.h>
#include "TextureSource.h"


class NullTextureSource : public TextureSource {
public:
	virtual Texture* getTexture(const CString& texName, bool locked = false) { return NULL; }
	virtual TextureSource* clone() const { return new NullTextureSource; }
};

#endif /* NULLTEXTURESOURCE_H_ */
