#ifndef CUBE_H
#define CUBE_H

#include <QtWidgets/QApplication>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

class Cube : public QOpenGLWidget
{
    Q_OBJECT
public:
    Cube(QWidget *parent = nullptr)
        : QOpenGLWidget(parent)
        , m_indexBuffer(QOpenGLBuffer::IndexBuffer)
    {
        // setWindowTitle("Qt C++, OpenGL");
        // resize(268, 268);
    }
    QTimer m_timer;
    float m_rotationAngle = 0.0f;
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vertPosBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_projMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_modelMatrix;
    int m_amountOfVertices;

    // void initializeGL() override;
    void createObject(QOpenGLShaderProgram * shaderProgramm);
    void paintGL() override;
    void drawBox();
    void resizeGL(int w, int h) override;
    int initVertexBuffers();
};

#endif // CUBE_H
