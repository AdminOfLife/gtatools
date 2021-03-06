/*
	Copyright 2010-2014 David "Alemarius Nexus" Lerch

	This file is part of gtatools-gui.

	gtatools-gui is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	gtatools-gui is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with gtatools-gui.  If not, see <http://www.gnu.org/licenses/>.

	Additional permissions are granted, which are listed in the file
	GPLADDITIONS.
 */

#ifndef GTATOOLS_GUI_CONFIG_CMAKE_H_
#define GTATOOLS_GUI_CONFIG_CMAKE_H_

// This _MUST_ be the first file included. Especially, it _MUST_ be included before windows.h...
#include <gta/config.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

#ifdef GTA_USE_OPENGL_ES
#define GTATOOLS_GUI_USE_OPENGL_ES
#else
#undef GTATOOLS_GUI_USE_OPENGL_ES
#endif

#endif /* GTATOOLS_GUI_CONFIG_CMAKE_H_ */
