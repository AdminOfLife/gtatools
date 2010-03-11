/*
 * DFFRenderer.cpp
 *
 *  Created on: 10.03.2010
 *      Author: alemariusnexus
 */

#include "DFFRenderer.h"


BEGIN_EVENT_TABLE(DFFRenderer, wxGLCanvas)
	EVT_PAINT(DFFRenderer::render)
END_EVENT_TABLE()


DFFRenderer::DFFRenderer(wxWindow* parent, int* attribs)
		: wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxT("DFFRenderer"), attribs),
		  geometry(NULL)
{

}


void DFFRenderer::render(wxPaintEvent& evt)
{
	if (!IsShown()) {
		return;
	}

	wxGLCanvas::SetCurrent();
	wxPaintDC(this);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, (GLsizei) GetSize().x, (GLsizei) GetSize().y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -50.0, 50.0, -500.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();*/

	if (geometry) {
		GLenum mode = geometry->isTriangleStripFormat() ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
		float* verts = geometry->getVertices();

		DFFGeometryPart** parts = geometry->getParts();

		glBegin(mode);

		for (int32_t i = 0 ; i < geometry->getPartCount() ; i++) {
			DFFGeometryPart* part = parts[i];
			int32_t* indices = part->getIndices();

			for (int32_t j = 0 ; j < part->getIndexCount() ; j++) {
				int32_t index = indices[j];
				glVertex3f(verts[index*3], verts[index*3 + 1], verts[index*3 + 2]);
			}
		}

		glEnd();
	}

	glFlush();
	SwapBuffers();
}


void DFFRenderer::setGeometry(DFFGeometry* geom)
{
	this->geometry = geom;
}


