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

#ifndef ANIMATIONCACHEENTRY_H_
#define ANIMATIONCACHEENTRY_H_

#include <nxcommon/ResourceCache.h>
#include "AnimationPackage.h"
#include "../../Engine.h"


class AnimationCacheEntry : public Engine::StringResourceCache::Entry {
public:
	AnimationCacheEntry(AnimationPackage* pkg) : pkg(pkg) {}
	virtual ~AnimationCacheEntry();
	virtual cachesize_t getSize() const { return pkg->getSize(); }
	AnimationPackage* getPackage() const { return pkg; }

private:
	AnimationPackage* pkg;
};

#endif /* ANIMATIONCACHEENTRY_H_ */
