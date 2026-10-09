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
    Object(const QVector3D& size,
           const QVector3D& position,
           int rotationDirection = 0,
           const QVector3D& rotation = QVector3D(0.0f, 1.0f, 0.0f));
    ~Object();

    void drawObject();
    int initVertexBuffers();
    void setShaders(QOpenGLShaderProgram *shaderProgramm);
    void translateObject();
    void rotateObject();

    QMatrix4x4 returnModelMatrix(){ return m_modelMatrix; }

private:

    QVector3D                   m_position;
    QVector3D                   m_size;

    QOpenGLVertexArrayObject    m_vao;

    QOpenGLBuffer               m_vertPosBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer               m_normalBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer               m_indexBuffer{QOpenGLBuffer::IndexBuffer};

    QMatrix4x4                  m_modelMatrix;

    float                       m_rotationAngle = 0.0f;
    int                         m_amountOfVertices = 0;
};

#endif // OBJECT_H
