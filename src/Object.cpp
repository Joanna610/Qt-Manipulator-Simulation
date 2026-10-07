
#include "inc/Object.h"

#include <QOpenGLShaderProgram>
#include <vector>

/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
             see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

Object::Object(const QVector3D& size, const QVector3D& position)
{
    // Model transformation
    m_modelMatrix.setToIdentity();

    // The camera is quite far away, so make the cube large for testing.
    m_modelMatrix.scale(size);
    m_modelMatrix.translate(position);

    // Create Vertex Array Object
    m_vao.create();
    m_vao.bind();

    // Create vertex, normal and index buffers
    m_amountOfVertices = initVertexBuffers();

    m_vao.release();
}

void Object::setShaders(QOpenGLShaderProgram *shaderProgramm){

    Q_ASSERT(shaderProgramm != nullptr);

    m_vao.bind();
    m_vertPosBuffer.bind();

    shaderProgramm->enableAttributeArray(0);
    shaderProgramm->setAttributeBuffer(
        0,          // attribute location
        GL_FLOAT,   // data type
        0,          // offset
        3,          // number of components: vec3
        0           // stride
        );

    m_vertPosBuffer.release();

    m_normalBuffer.bind();

    shaderProgramm->enableAttributeArray(1);
    shaderProgramm->setAttributeBuffer(
        1,          // attribute location
        GL_FLOAT,   // data type
        0,          // offset
        3,          // number of components: vec3
        0           // stride
        );

    m_normalBuffer.release();
    m_vao.release();
}

Object::~Object()
{
    m_indexBuffer.destroy();
    m_normalBuffer.destroy();
    m_vertPosBuffer.destroy();
    m_vao.destroy();
}

void Object::drawBox()
{
    m_vao.bind();
    m_indexBuffer.bind();

    glDrawElements(
        GL_TRIANGLES,
        m_amountOfVertices,
        GL_UNSIGNED_INT,
        nullptr
        );

    m_indexBuffer.release();

    m_vao.release();
}


int Object::initVertexBuffers()
{
    // ============================================================
    // Cube vertex positions
    //
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    //
    // Each face has its own vertices so that each face can have
    // its own normal.
    // ============================================================

    float vertPositions[] =
        {
            // v0-v1-v2-v3 front
            1.f,  1.f,  1.f,
            -1.f,  1.f,  1.f,
            -1.f, -1.f,  1.f,
            1.f, -1.f,  1.f,

            // v0-v3-v4-v5 right
            1.f,  1.f,  1.f,
            1.f, -1.f,  1.f,
            1.f, -1.f, -1.f,
            1.f,  1.f, -1.f,

            // v0-v5-v6-v1 up
            1.f,  1.f,  1.f,
            1.f,  1.f, -1.f,
            -1.f,  1.f, -1.f,
            -1.f,  1.f,  1.f,

            // v1-v6-v7-v2 left
            -1.f,  1.f,  1.f,
            -1.f,  1.f, -1.f,
            -1.f, -1.f, -1.f,
            -1.f, -1.f,  1.f,

            // v7-v4-v3-v2 down
            -1.f, -1.f, -1.f,
            1.f, -1.f, -1.f,
            1.f, -1.f,  1.f,
            -1.f, -1.f,  1.f,

            // v4-v7-v6-v5 back
            1.f, -1.f, -1.f,
            -1.f, -1.f, -1.f,
            -1.f,  1.f, -1.f,
            1.f,  1.f, -1.f
        };

    // ============================================================
    // Normals
    // ============================================================

    float normals[] =
        {
            // front
            0.f,  0.f,  1.f,
            0.f,  0.f,  1.f,
            0.f,  0.f,  1.f,
            0.f,  0.f,  1.f,

            // right
            1.f,  0.f,  0.f,
            1.f,  0.f,  0.f,
            1.f,  0.f,  0.f,
            1.f,  0.f,  0.f,

            // up
            0.f,  1.f,  0.f,
            0.f,  1.f,  0.f,
            0.f,  1.f,  0.f,
            0.f,  1.f,  0.f,

            // left
            -1.f,  0.f,  0.f,
            -1.f,  0.f,  0.f,
            -1.f,  0.f,  0.f,
            -1.f,  0.f,  0.f,

            // down
            0.f, -1.f,  0.f,
            0.f, -1.f,  0.f,
            0.f, -1.f,  0.f,
            0.f, -1.f,  0.f,

            // back
            0.f,  0.f, -1.f,
            0.f,  0.f, -1.f,
            0.f,  0.f, -1.f,
            0.f,  0.f, -1.f
        };

    // ============================================================
    // Create position buffer
    // ============================================================

    m_vertPosBuffer.create();
    m_vertPosBuffer.bind();
    m_vertPosBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_vertPosBuffer.allocate(
        vertPositions,
        sizeof(vertPositions)
        );

    m_vertPosBuffer.release();

    // ============================================================
    // Create normal buffer
    // ============================================================

    m_normalBuffer.create();
    m_normalBuffer.bind();
    m_normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_normalBuffer.allocate(
        normals,
        sizeof(normals)
        );

    m_normalBuffer.release();

    // ============================================================
    // Index buffer
    // ============================================================

    unsigned int indices[] =
        {
            0,  1,  2,
            0,  2,  3,       // front

            4,  5,  6,
            4,  6,  7,       // right

            8,  9, 10,
            8, 10, 11,       // up

            12, 13, 14,
            12, 14, 15,       // left

            16, 17, 18,
            16, 18, 19,       // down

            20, 21, 22,
            20, 22, 23        // back
        };

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    m_indexBuffer.allocate(
        indices,
        sizeof(indices)
        );

    m_indexBuffer.release();

    // 6 faces × 2 triangles × 3 indices = 36
    int amountOfVertices =
        sizeof(indices) / sizeof(indices[0]);

    return amountOfVertices;
}

