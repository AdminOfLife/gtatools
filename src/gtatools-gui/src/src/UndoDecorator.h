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

#ifndef UNDODECORATOR_H_
#define UNDODECORATOR_H_

#include <QUndoCommand>


class UndoDecorator : public QObject
{
	Q_OBJECT

public:
	virtual void push(QUndoCommand* cmd) = 0;

protected:
	void commit(QUndoCommand* cmd);

signals:
	void committed(QUndoCommand* cmd);
};

#endif /* UNDODECORATOR_H_ */
