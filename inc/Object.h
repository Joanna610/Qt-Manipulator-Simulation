#ifndef OBJECT_H
#define OBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

class Object
{
public:
    Object() = delete;
    Object(const QVector3D& size, const QVector3D& position);
    ~Object();

    void drawBox();
    int initVertexBuffers();
    void setShaders(QOpenGLShaderProgram *shaderProgramm);

    QMatrix4x4 returnModelMatrix(){ return m_modelMatrix; }


private:
    QOpenGLVertexArrayObject    m_vao;

    QOpenGLBuffer               m_vertPosBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer               m_normalBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer               m_indexBuffer{QOpenGLBuffer::IndexBuffer};

    QMatrix4x4                  m_modelMatrix;

    int                         m_amountOfVertices = 0;
};

#endif // OBJECT_H
