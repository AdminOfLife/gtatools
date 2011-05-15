/*
	Copyright 2010-2011 David "Alemarius Nexus" Lerch

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

#include <gtatools-gui/config.h>
#include "COLGUIModule.h"
#include "../../gui/MainWindow.h"



COLGUIModule::COLGUIModule()
{
	viewSubMenu = new QMenu(tr("COL"));

	wireframeAction = new QAction(tr("Show wireframe"), NULL);
	wireframeAction->setCheckable(true);
	wireframeAction->setChecked(false);

#ifdef GTATOOLS_GUI_USE_OPENGL_ES
	wireframeAction->setEnabled(false);
	wireframeAction->setToolTip(tr("Wireframe rendering is not available in OpenGL ES!"));
#endif

	viewSubMenu->addAction(wireframeAction);

	connect(wireframeAction, SIGNAL(triggered(bool)), this, SLOT(wireframePropertyChangedSlot(bool)));
}


void COLGUIModule::wireframePropertyChangedSlot(bool wireframe)
{
	emit wireframePropertyChanged(wireframe);
}


void COLGUIModule::doInstall()
{
	QMenu* viewMenu = mainWindow->getViewMenu();

	viewMenu->addMenu(viewSubMenu);
	wireframeAction->setParent(mainWindow);
}


void COLGUIModule::doUninstall()
{
	QMenu* viewMenu = mainWindow->getViewMenu();

	viewMenu->removeAction(viewSubMenu->menuAction());
	wireframeAction->setParent(NULL);
}
