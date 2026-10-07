/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef OPENGLGRIDOBJECT_H
#define OPENGLGRIDOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

class GridObject {
public:
    GridObject();
    ~GridObject();

	void render();
    void setShaders(QOpenGLShaderProgram * shaderProgramm);
    bool shadersSet() const {return isShaderSet;}

private:
    bool                        isShaderSet;
	unsigned int				m_bufferSize;

	/*! Wraps an OpenGL VertexArrayObject, that references the vertex coordinates. */
	QOpenGLVertexArrayObject	m_vao;
	/*! Holds positions of grid lines. */
	QOpenGLBuffer				m_vbo;

};

#endif // OPENGLGRIDOBJECT_H
