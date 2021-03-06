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

#ifndef SUBMESH_H_
#define SUBMESH_H_

#include <gta/config.h>
#include "../../gl.h"
#include "Mesh.h"
#include "Material.h"
#include <gtaformats/dff/DFFGeometryPart.h>



class Submesh {
private:
	struct UniformData
	{
		uint32_t hasVertexColors;
		uint32_t isTextured;
		Vector4 materialColor;
		Vector4 matAmbientReflection;
		Vector4 matDiffuseReflection;
		Vector4 matSpecularReflection;
	};

public:
	Submesh(Mesh* mesh, int indexCount, uint32_t* indices);
	Submesh(Mesh* mesh, const DFFGeometryPart& part);
	~Submesh();
	Material* getMaterial() { return material; }
	void setMaterial(Material* mat);
	GLuint getIndexOffset() const { return indexOffset; }
	bool isLinked() const { return indices == NULL; }
	int getIndexCount() const { return indexCount; }

	int guessSize() const { return indexCount*4 + sizeof(Submesh); }

private:
	void setLinked(GLuint indexOffset);

private:
	Mesh* mesh;
	Material* material;
	int indexCount;
	uint32_t* indices;
	GLuint indexOffset;

	friend class Mesh;
};

#endif /* SUBMESH_H_ */
