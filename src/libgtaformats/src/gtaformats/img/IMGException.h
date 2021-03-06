/*
	Copyright 2010-2014 David "Alemarius Nexus" Lerch

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

#ifndef IMGEXCEPTION_H_
#define IMGEXCEPTION_H_

#include <gtaformats/config.h>
#include <nxcommon/exception/Exception.h>


/**	\brief Objects of this class are thrown by IMGArchive when an error occurs.
 */
class IMGException : public Exception {
public:
	/**	\brief Creates a new IMGException with the given error code and message.
	 *
	 *	@param code The error code.
	 *	@param message The error message.
	 */
	IMGException(const CString& message, const CString& srcFile = CString(), int srcLine = -1,
			Exception* nestedException = NULL) throw()
			: Exception(message, srcFile, srcLine, nestedException, "IMGException") {}
	IMGException(const IMGException& ex) throw() : Exception(ex) {}
};

#endif /* IMGEXCEPTION_H_ */
