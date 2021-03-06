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

#include "DefaultGUIModule.h"
#include <QDesktopServices>
#include <QtCore/QUrl>
#include <QActionGroup>
#include "../Profile.h"
#include "../ProfileManager.h"
#include "ConfigWidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include "../System.h"
#include <gtaformats/config.h>
#include "FileSearchDialog.h"
#include "VersionDialog.h"
#include "PVSDialog.h"
#include "../DisplayedEntityHandler.h"



DefaultGUIModule::DefaultGUIModule()
{
	System* sys = System::getInstance();

	logConsoleDock = new QDockWidget(tr("Error Log Console"));
	logConsole = new LogConsole(logConsoleDock);
	logConsoleDock->setWidget(logConsole);
	logConsoleDock->setObjectName("logConsoleDock");

	fileTreeDock = new QDockWidget(tr("File Tree"));
	fileTreeDock->setObjectName("fileTreeDock");
	fileTree = new FileTree(fileTreeDock);
	fileTreeDock->setWidget(fileTree);

	fileOpenAction = new QAction(QIcon::fromTheme("document-open", QIcon(":/src/resource/document-open.png")),
			tr("Open..."), NULL);
	fileOpenAction->setShortcut(QKeySequence::Open);
	connect(fileOpenAction, SIGNAL(triggered(bool)), this, SLOT(onFileOpen(bool)));

	fileCloseAction = new QAction(tr("Close"), NULL);
	fileCloseAction->setShortcut(QKeySequence::Close);
	connect(fileCloseAction, SIGNAL(triggered(bool)), this, SLOT(onFileClose(bool)));
	fileCloseAction->setEnabled(false);

	fileSaveAction = new QAction(QIcon::fromTheme("document-save", QIcon(":/src/resource/document-save.png")),
			tr("Save"), NULL);
	fileSaveAction->setShortcut(QKeySequence::Save);
	connect(fileSaveAction, SIGNAL(triggered(bool)), this, SLOT(onFileSave(bool)));
	fileSaveAction->setEnabled(false);

	fileSaveAsAction = new QAction(QIcon::fromTheme("document-save-as",
			QIcon(":/src/resource/document-save-as.png")), tr("Save As..."), NULL);
	fileSaveAsAction->setShortcut(QKeySequence::SaveAs);
	connect(fileSaveAsAction, SIGNAL(triggered(bool)), this, SLOT(onFileSaveAs(bool)));
	fileSaveAsAction->setEnabled(false);

	searchFileAction = new QAction(tr("Search File..."), NULL);
	searchFileAction->setShortcut(QKeySequence::Find);
	connect(searchFileAction, SIGNAL(triggered(bool)), this, SLOT(onSearchFile(bool)));

	settingsAction = new QAction(tr("Settings..."), NULL);
	settingsAction->setShortcut(QKeySequence::Preferences);
	connect(settingsAction, SIGNAL(triggered(bool)), this, SLOT(settingsRequested(bool)));

	aboutQtAction = new QAction(tr("About Qt"), NULL);
	connect(aboutQtAction, SIGNAL(triggered(bool)), this, SLOT(onAboutQt(bool)));

	aboutAction = new QAction(tr("About"), NULL);
	connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(onAbout(bool)));

	versionInfoAction = new QAction(tr("Version Information"), NULL);
	connect(versionInfoAction, SIGNAL(triggered(bool)), this, SLOT(onVersionInfo(bool)));

	systemOpenAction = new QAction(tr("Execute System Program"), NULL);
	connect(systemOpenAction, SIGNAL(triggered(bool)), this, SLOT(onOpenSystemProgram(bool)));

	pvsAction = new QAction(tr("Build PVS Data"), NULL);
	connect(pvsAction, SIGNAL(triggered(bool)), this, SLOT(onBuildPVS(bool)));

	undoAction = undoGroup.createUndoAction(NULL);
	undoAction->setShortcut(QKeySequence::Undo);

	redoAction = undoGroup.createRedoAction(NULL);
	redoAction->setShortcut(QKeySequence::Redo);

	undoGroup.addStack(&dummyUndoStack);

	connect(sys, SIGNAL(entityOpened(DisplayedEntity*)), this, SLOT(entityOpened(DisplayedEntity*)));
	connect(sys, SIGNAL(entityClosed(DisplayedEntity*)), this, SLOT(entityClosed(DisplayedEntity*)));
	connect(sys, SIGNAL(currentEntityChanged(DisplayedEntity*, DisplayedEntity*)), this,
			SLOT(currentEntityChanged(DisplayedEntity*, DisplayedEntity*)));
}


DefaultGUIModule::~DefaultGUIModule()
{
	delete logConsoleDock;
	delete fileTreeDock;

	delete fileOpenAction;
	delete fileCloseAction;
	delete fileSaveAction;
	delete fileSaveAsAction;
	delete searchFileAction;
	delete settingsAction;
	delete aboutQtAction;
	delete aboutAction;
	delete versionInfoAction;
	delete systemOpenAction;
	delete pvsAction;

	QList<QAction*> actions = profileSwitchGroup->actions();
	for (int i = 0 ; i < actions.size() ; i++) {
		delete actions.at(i);
	}

	delete profileSwitchGroup;
}


void DefaultGUIModule::doInstall()
{
	QMenu* fileMenu = mainWindow->getFileMenu();
	QMenu* editMenu = mainWindow->getEditMenu();
	QMenu* settingsMenu = mainWindow->getSettingsMenu();
	QMenu* profileMenu = mainWindow->getProfileMenu();
	QMenu* helpMenu = mainWindow->getHelpMenu();
	QMenu* toolsMenu = mainWindow->getToolsMenu();
	QToolBar* toolBar = mainWindow->getToolBar();

	mainWindow->addDockWidget(Qt::BottomDockWidgetArea, logConsoleDock);
	mainWindow->addDockWidget(Qt::LeftDockWidgetArea, fileTreeDock);

	logConsoleDock->close();

	fileOpenAction->setParent(mainWindow);
	fileCloseAction->setParent(mainWindow);
	fileSaveAction->setParent(mainWindow);
	fileSaveAsAction->setParent(mainWindow);
	searchFileAction->setParent(mainWindow);
	settingsAction->setParent(mainWindow);
	aboutQtAction->setParent(mainWindow);
	aboutAction->setParent(mainWindow);
	versionInfoAction->setParent(mainWindow);
	systemOpenAction->setParent(mainWindow);
	pvsAction->setParent(mainWindow);
	undoAction->setParent(mainWindow);
	redoAction->setParent(mainWindow);

	fileMenu->addAction(fileOpenAction);
	fileMenu->addAction(fileCloseAction);
	fileMenu->addAction(fileSaveAction);
	fileMenu->addAction(fileSaveAsAction);
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addAction(searchFileAction);
	settingsMenu->addAction(settingsAction);
	helpMenu->addAction(aboutQtAction);
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(versionInfoAction);
	toolsMenu->addAction(pvsAction);

	toolBar->addAction(fileOpenAction);
	toolBar->addAction(fileSaveAction);
	toolBar->addAction(fileSaveAsAction);

	profileSwitchMenu = new QMenu(tr("Switch"), profileMenu);
	profileMenu->addMenu(profileSwitchMenu);

	profileSwitchGroup = new QActionGroup(this);

	ProfileManager* pm = ProfileManager::getInstance();

	connect(profileSwitchGroup, SIGNAL(triggered(QAction*)), this, SLOT(profileSwitchRequested(QAction*)));
	connect(pm, SIGNAL(profileAdded(Profile*)), this, SLOT(profileAdded(Profile*)));
	connect(pm, SIGNAL(profileRemoved(Profile*)), this, SLOT(profileRemoved(Profile*)));
	//connect(pm, SIGNAL(profilesLoaded()), this, SLOT(profilesLoaded()));
	connect(pm, SIGNAL(currentProfileChanged(Profile*, Profile*)), this,
			SLOT(currentProfileChanged(Profile*, Profile*)));

	profilesLoaded();
}


void DefaultGUIModule::loadProfileSwitchMenu()
{
	QList<QAction*> actions = profileSwitchGroup->actions();
	QList<QAction*>::iterator it;

	for (it = actions.begin() ; it != actions.end() ; it++) {
		QAction* action = *it;
		profileSwitchGroup->removeAction(action);
		profileSwitchMenu->removeAction(action);
		delete action;
	}

	ProfileManager* pm = ProfileManager::getInstance();
	ProfileManager::ProfileIterator pit;

	Profile* currentProfile = pm->getCurrentProfile();

	QAction* noProfileAction = new QAction(tr("No Profile"), this);
	noProfileAction->setCheckable(true);
	noProfileAction->setData(-1);

	if (currentProfile == NULL) {
		noProfileAction->setChecked(true);
	}

	profileSwitchGroup->addAction(noProfileAction);
	profileSwitchMenu->addAction(noProfileAction);

	profileSwitchMenu->addSeparator();

	for (pit = pm->getProfileBegin() ; pit != pm->getProfileEnd() ; pit++) {
		Profile* profile = *pit;

		QAction* action = new QAction(profile->getName(), this);
		action->setCheckable(true);
		action->setData(pm->indexOfProfile(profile));

		if (profile == currentProfile) {
			action->setChecked(true);
		}

		profileSwitchGroup->addAction(action);
		profileSwitchMenu->addAction(action);
	}
}


void DefaultGUIModule::doUninstall()
{
	QMenu* fileMenu = mainWindow->getFileMenu();
	QMenu* editMenu = mainWindow->getEditMenu();
	QMenu* settingsMenu = mainWindow->getSettingsMenu();
	//QMenu* profileMenu = mainWindow->getProfileMenu();
	QMenu* helpMenu = mainWindow->getHelpMenu();
	QMenu* toolsMenu = mainWindow->getToolsMenu();
	QToolBar* toolBar = mainWindow->getToolBar();

	mainWindow->removeDockWidget(fileTreeDock);
	mainWindow->removeDockWidget(logConsoleDock);

	fileOpenAction->setParent(NULL);
	fileCloseAction->setParent(NULL);
	fileSaveAction->setParent(NULL);
	fileSaveAsAction->setParent(NULL);
	searchFileAction->setParent(NULL);
	settingsAction->setParent(NULL);
	aboutQtAction->setParent(NULL);
	aboutAction->setParent(NULL);
	versionInfoAction->setParent(NULL);
	pvsAction->setParent(NULL);

	fileMenu->removeAction(fileOpenAction);
	fileMenu->removeAction(fileCloseAction);
	fileMenu->removeAction(fileSaveAction);
	fileMenu->removeAction(fileSaveAsAction);
	editMenu->removeAction(searchFileAction);
	settingsMenu->removeAction(settingsAction);
	helpMenu->removeAction(aboutQtAction);
	helpMenu->removeAction(aboutAction);
	helpMenu->removeAction(versionInfoAction);
	toolsMenu->removeAction(pvsAction);

	toolBar->removeAction(fileOpenAction);
	toolBar->removeAction(fileSaveAction);
	toolBar->removeAction(fileSaveAsAction);


	ProfileManager* pm = ProfileManager::getInstance();

	disconnect(pm, SIGNAL(profileAdded(Profile*)), this, SLOT(profileAdded(Profile*)));
	disconnect(pm, SIGNAL(profileRemoved(Profile*)), this, SLOT(profileRemoved(Profile*)));
	disconnect(pm, SIGNAL(profilesLoaded()), this, SLOT(profilesLoaded()));

	delete profileSwitchMenu;
}


void DefaultGUIModule::profileSwitchRequested(QAction* action)
{
	ProfileManager* pm = ProfileManager::getInstance();
	int index = action->data().toInt();

	Profile* profile = NULL;

	if (index != -1) {
		profile = pm->getProfile(action->data().toInt());
	}

	pm->setCurrentProfile(profile);
}


void DefaultGUIModule::settingsRequested(bool checked)
{
	ConfigWidget* cfgWidget = new ConfigWidget;
	cfgWidget->show();
}


void DefaultGUIModule::buildFileTreeMenu(const QLinkedList<File*>& files, QMenu& menu)
{
	menu.addAction(systemOpenAction);
	contextFiles = files;
}


void DefaultGUIModule::onOpenSystemProgram(bool checked)
{
	QLinkedList<File*>::iterator it;

	for (it = contextFiles.begin() ; it != contextFiles.end() ; it++) {
		QDesktopServices::openUrl(QUrl(QString("file://%1").arg((*it)->getPath().toString().get())));
	}
}


void DefaultGUIModule::onAboutQt(bool checked)
{
	QMessageBox::aboutQt(mainWindow, tr("About Qt"));
}


void DefaultGUIModule::onAbout(bool checked)
{
	QString aboutText = tr("AboutText").arg(GTATOOLS_VERSION);
	QMessageBox::about(mainWindow, tr("About GTATools GUI Tool"), aboutText);
}


void DefaultGUIModule::onVersionInfo(bool checked)
{
	VersionDialog dialog(mainWindow);
	dialog.exec();
}


void DefaultGUIModule::onFileOpen(bool checked)
{
	QString fname = QFileDialog::getOpenFileName(mainWindow, tr("Select file"));

	if (!fname.isNull()) {
		System::getInstance()->openFile(File(fname.toLocal8Bit().constData()));
	}
}


void DefaultGUIModule::onFileClose(bool checked)
{
	System* sys = System::getInstance();

	sys->closeEntity(sys->getCurrentEntity());
}


void DefaultGUIModule::onFileSave(bool checked)
{
	DisplayedEntity* dent = System::getInstance()->getCurrentEntity();
	dent->saveChanges(false);
}


void DefaultGUIModule::onFileSaveAs(bool checked)
{
	DisplayedEntity* dent = System::getInstance()->getCurrentEntity();
	dent->saveChanges(true);
}


void DefaultGUIModule::entityOpened(DisplayedEntity* ent)
{
	fileCloseAction->setEnabled(true);

	connect(ent, SIGNAL(hasUnsavedChangesStateChanged(bool)), this, SLOT(entityHasUnsavedChangesStateChanged(bool)));
	connect(ent, SIGNAL(currentEditHandlerChanged(DisplayedEntityHandler*, DisplayedEntityHandler*)), this,
			SLOT(entityCurrentEditHandlerChanged(DisplayedEntityHandler*, DisplayedEntityHandler*)));

	if (ent->getCurrentEditHandler()) {
		undoGroup.addStack(ent->getCurrentEditHandler()->getUndoStack());
	}
}


void DefaultGUIModule::entityClosed(DisplayedEntity* ent)
{
	System* sys = System::getInstance();

	if (!sys->hasOpenEntity()) {
		fileCloseAction->setEnabled(false);
	}

	if (ent->getCurrentEditHandler()) {
		undoGroup.removeStack(ent->getCurrentEditHandler()->getUndoStack());
	}

	entityLastReleasedEditLockHandlers.remove(ent);

	disconnect(ent, NULL, this, NULL);
}


void DefaultGUIModule::currentEntityChanged(DisplayedEntity* prev, DisplayedEntity* current)
{
	fileSaveAction->setEnabled(current  &&  current->canSave()  &&  current->hasUnsavedChanges());
	fileSaveAsAction->setEnabled(current  &&  current->canSave());

	if (current  &&  current->getCurrentEditHandler()) {
		undoGroup.setActiveStack(current->getCurrentEditHandler()->getUndoStack());
	} else {
		if (current) {
			// Special case: See comment in entityCurrentEditHandlerChanged()

			QMap<DisplayedEntity*, DisplayedEntityHandler*>::iterator it = entityLastReleasedEditLockHandlers.find(current);

			if (it == entityLastReleasedEditLockHandlers.end()) {
				undoGroup.setActiveStack(&dummyUndoStack);
			} else {
				DisplayedEntityHandler* oldHandler = it.value();
				undoGroup.setActiveStack(oldHandler->getUndoStack());
			}
		} else {
			undoGroup.setActiveStack(&dummyUndoStack);
		}
	}
}


void DefaultGUIModule::entityHasUnsavedChangesStateChanged(bool hasChanges)
{
	DisplayedEntity* dent = (DisplayedEntity*) sender();
	fileSaveAction->setEnabled(dent->canSave()  &&  dent->hasUnsavedChanges());
}


void DefaultGUIModule::entityCurrentEditHandlerChanged(DisplayedEntityHandler* oldHandler, DisplayedEntityHandler* newHandler)
{
	DisplayedEntity* entity = (DisplayedEntity*) sender();

	if (oldHandler  &&  !newHandler) {
		// Special case: This means that the oldHandler has released its edit lock. As long as no other handler
		// acquires a lock, it should still be possible to redo changes on the oldHandler's QUndoStack.

		entityLastReleasedEditLockHandlers.insert(entity, oldHandler);
	} else {
		if (oldHandler) {
			undoGroup.removeStack(oldHandler->getUndoStack());
		}

		if (newHandler) {
			undoGroup.addStack(newHandler->getUndoStack());

			if (System::getInstance()->getCurrentEntity() == entity)
				undoGroup.setActiveStack(newHandler->getUndoStack());
		}

		entityLastReleasedEditLockHandlers.remove(entity);
	}
}


void DefaultGUIModule::onSearchFile(bool checked)
{
	FileSearchDialog dialog(mainWindow);
	dialog.exec();
}


void DefaultGUIModule::profileAdded(Profile* profile)
{
	connect(profile, SIGNAL(nameChanged(const QString&, const QString&)), this,
			SLOT(profileNameChanged(const QString&, const QString&)));
	loadProfileSwitchMenu();
}


void DefaultGUIModule::profileRemoved(Profile* profile)
{
	disconnect(profile, SIGNAL(nameChanged(const QString&)), this, SLOT(profileNameChanged(const QString&)));
	loadProfileSwitchMenu();
}


void DefaultGUIModule::profilesLoaded()
{
	loadProfileSwitchMenu();
}


void DefaultGUIModule::profileNameChanged(const QString& oldName, const QString& newName)
{
	loadProfileSwitchMenu();
}


void DefaultGUIModule::currentProfileChanged(Profile* oldProfile, Profile* newProfile)
{
	QAction* checkedAction = profileSwitchGroup->checkedAction();

	if (checkedAction) {
		checkedAction->setChecked(false);
	}

	QList<QAction*> actions = profileSwitchGroup->actions();

	if (newProfile) {
		ProfileManager* pm = ProfileManager::getInstance();
		int idx = pm->indexOfProfile(newProfile);

		if (actions.size() > idx+1) {
			actions[idx+1]->setChecked(true);
		}

		searchFileAction->setEnabled(true);
	} else {
		if (actions.size() > 0) {
			actions[0]->setChecked(true);
		}

		searchFileAction->setEnabled(false);
	}
}


void DefaultGUIModule::onBuildPVS(bool checked)
{
	PVSDialog dlg(mainWindow);
	dlg.exec();
}
