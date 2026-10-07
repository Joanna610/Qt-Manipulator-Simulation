/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "inc/GridObject.h"

#include <QOpenGLShaderProgram>
#include <vector>


GridObject::GridObject() : isShaderSet(false) {
    const unsigned int N = 20;
    float width = 600;
    std::vector<float>  gridVertexBufferData;

    m_bufferSize = 2*N*2*2;
	gridVertexBufferData.resize(m_bufferSize);
	float * gridVertexBufferPtr = gridVertexBufferData.data();

    float x1 = -width*0.5;
	float x2 = width*0.5;
	for (unsigned int i=0; i<N; ++i, gridVertexBufferPtr += 4) {
		float z = width/(N-1)*i-width*0.5;
		gridVertexBufferPtr[0] = x1;
		gridVertexBufferPtr[1] = z;
		gridVertexBufferPtr[2] = x2;
		gridVertexBufferPtr[3] = z;
	}
	// compute grid lines with x = const
	float z1 = -width*0.5;
	float z2 = width*0.5;
	for (unsigned int i=0; i<N; ++i, gridVertexBufferPtr += 4) {
		float x = width/(N-1)*i-width*0.5;
		gridVertexBufferPtr[0] = x;
		gridVertexBufferPtr[1] = z1;
		gridVertexBufferPtr[2] = x;
		gridVertexBufferPtr[3] = z2;
	}

	// Create Vertex Array Object
	m_vao.create();		// create Vertex Array Object
	m_vao.bind();		// and bind it

	// Create Vertex Buffer Object
	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = m_bufferSize*sizeof(float);
	m_vbo.allocate(gridVertexBufferData.data(), vertexMemSize);

	m_vao.release();
	m_vbo.release();
}


GridObject::~GridObject() {
	m_vao.destroy();
	m_vbo.destroy();
}


void GridObject::render() {
	m_vao.bind();
	// draw the grid lines, m_NVertexes = number of floats in buffer
    glDrawArrays(GL_LINES, 0, m_bufferSize);
	m_vao.release();
}

void GridObject::setShaders(QOpenGLShaderProgram * shaderProgramm){

    Q_ASSERT(shaderProgramm != nullptr);

    m_vao.bind();
    m_vbo.bind();

    shaderProgramm->enableAttributeArray(0); // array with index/id 0
    shaderProgramm->setAttributeBuffer(0, GL_FLOAT,
                                       0 /* position/vertex offset */,
                                       2 /* two floats per position = vec2 */,
                                       0 /* vertex after vertex, no interleaving */);
    isShaderSet = true;
    m_vao.release();
    m_vbo.release();
}
