/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>

class SceneView;

class TestDialog : public QDialog {
	Q_OBJECT
public:
	TestDialog();
    ~TestDialog() = default;

private:
    SceneView * m_sceneView;
};

#endif // TESTDIALOG_H
