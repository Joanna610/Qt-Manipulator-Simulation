/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "inc/SceneView.h"

#include <QExposeEvent>
#include <QOpenGLShaderProgram>
#include <QDateTime>

#include "inc/DebugApplication.h"

#define SHADER(x) m_shaderPrograms[x].shaderProgram()

SceneView::SceneView() :
	m_inputEventReceived(false)
{
	// *** create scene (no OpenGL calls are being issued below, just the data structures are created.

	// Shaderprogram #0 : grid (painting grid lines)
	ShaderProgram grid(":/shaders/grid.vert",":/shaders/grid.frag");
	grid.m_uniformNames.append("worldToView"); // mat4
	grid.m_uniformNames.append("gridColor"); // vec3
	m_shaderPrograms.append( grid );

    ShaderProgram cube(":/shaders/cube.vert", ":/shaders/cube.frag");
    cube.m_uniformNames.append("uMvpMatrix");
    cube.m_uniformNames.append("uNormalMatrix");
    m_shaderPrograms.append(cube);


	// *** initialize camera placement and model placement in the world

	// move camera a little back (mind: positive z) and look straight ahead
    m_camera.translate(-500,500,600);
	// look slightly down
    m_camera.rotate(-30, m_camera.right());
	// look slightly left
    m_camera.rotate(-40, QVector3D(0.0f, 1.0f, 0.0f));

}

SceneView::~SceneView() {
	if (m_context) {
		m_context->makeCurrent(this);

		for (ShaderProgram & p : m_shaderPrograms)
			p.destroy();

		m_gridObject.destroy();

		m_gpuTimers.destroy();
	}
}

void SceneView::initializeGL() {
	FUNCID(SceneView::initializeGL);
	try {
		// initialize shader programs
		for (ShaderProgram & p : m_shaderPrograms)
			p.create();

		// tell OpenGL to show only faces whose normal vector points towards us
        glDisable(GL_CULL_FACE);
		// enable depth testing, important for the grid and for the drawing order of several objects
        glEnable(GL_DEPTH_TEST);

		// initialize drawable objects
		m_gridObject.create(SHADER(0));
        m_Object.create(SHADER(1));

		// Timer
		m_gpuTimers.setSampleCount(3);
		m_gpuTimers.create();
	}
	catch (OpenGLException & ex) {
		throw OpenGLException(ex, "OpenGL initialization failed.", FUNC_ID);
	}
}


void SceneView::resizeGL(int width, int height) {
	// the projection matrix need to be updated only for window size changes
	m_projection.setToIdentity();
	// create projection matrix, i.e. camera lens
	m_projection.perspective(
				/* vertical angle */ 45.0f,
				/* aspect ratio */   width / float(height),
                /* near */           0.1f,
                /* far */            10000.0f
		);
	// Mind: to not use 0.0 for near plane, otherwise depth buffering and depth testing won't work!

	// update cached world2view matrix
	updateWorld2ViewMatrix();
}


void SceneView::paintGL() {
	m_cpuTimer.start();
	if (((DebugApplication *)qApp)->m_aboutToTerminate)
		return;

	// process input, i.e. check if any keys have been pressed
	if (m_inputEventReceived)
		processInput();

	const qreal retinaScale = devicePixelRatio(); // needed for Macs with retina display
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.2f, 0.2f, 0.3f, 0.1f);
	// set the background color = clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QVector3D gridColor(0.5f, 0.5f, 0.7f);

	m_gpuTimers.reset();

	// *** render grid ***

	m_gpuTimers.recordSample(); // setup grid
	SHADER(0)->bind();
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[0], m_worldToView);
	SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[1], gridColor);

	m_gpuTimers.recordSample(); // render grid
	m_gridObject.render();
	SHADER(0)->release();
    SHADER(1)->bind();

    QMatrix4x4 mvpMatrix =
        m_worldToView * m_Object.m_modelMatrix;

    SHADER(1)->setUniformValue(
        m_shaderPrograms[1].m_uniformIDs[0],
        mvpMatrix
        );

    QMatrix4x4 normalMatrix =
        m_Object.m_modelMatrix.inverted().transposed();

    SHADER(1)->setUniformValue(
        m_shaderPrograms[1].m_uniformIDs[1],
        normalMatrix
        );
    m_Object.drawBox();
    SHADER(1)->release();

	m_gpuTimers.recordSample(); // done painting


#if 0
	// do some animation stuff
	m_transform.rotate(1.0f, QVector3D(0.0f, 0.1f, 0.0f));
	updateWorld2ViewMatrix();
	renderLater();
#endif

	checkInput();

	QVector<GLuint64> intervals = m_gpuTimers.waitForIntervals();
	for (GLuint64 it : intervals)
        qDebug() << "  " << it*1e-6 << "ms/frame";
	QVector<GLuint64> samples = m_gpuTimers.waitForSamples();
	qDebug() << "Total render time: " << (samples.back() - samples.front())*1e-6 << "ms/frame";

	qint64 elapsedMs = m_cpuTimer.elapsed();
    qDebug() << "Total paintGL time: " << elapsedMs << "ms\n";
}


void SceneView::keyPressEvent(QKeyEvent *event) {
	m_keyboardMouseHandler.keyPressEvent(event);
	checkInput();
}

void SceneView::keyReleaseEvent(QKeyEvent *event) {
	m_keyboardMouseHandler.keyReleaseEvent(event);
	checkInput();
}

void SceneView::mousePressEvent(QMouseEvent *event) {
	m_keyboardMouseHandler.mousePressEvent(event);
	checkInput();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event) {
	m_keyboardMouseHandler.mouseReleaseEvent(event);
	checkInput();
}

void SceneView::mouseMoveEvent(QMouseEvent * /*event*/) {
	checkInput();
}

void SceneView::wheelEvent(QWheelEvent *event) {
	m_keyboardMouseHandler.wheelEvent(event);
	checkInput();
}


void SceneView::checkInput() {
	// this function is called whenever _any_ key/mouse event was issued

	// we test, if the current state of the key handler requires a scene update
	// (camera movement) and if so, we just set a flag to do that upon next repaint
	// and we schedule a repaint

	// trigger key held?
    if (m_keyboardMouseHandler.buttonDown(Qt::LeftButton)) {
		// has the mouse been moved?
		if (m_keyboardMouseHandler.mouseDownPos() != QCursor::pos()) {
			m_inputEventReceived = true;
//			qDebug() << "SceneView::checkInput() inputEventReceived: " << QCursor::pos() << m_keyboardMouseHandler.mouseDownPos();
			renderLater();
			return;
		}
	}
	// has the left mouse butten been release
    if (m_keyboardMouseHandler.buttonReleased(Qt::RightButton)) {
		m_inputEventReceived = true;
		renderLater();
		return;
	}

	// scroll-wheel turned?
	if (m_keyboardMouseHandler.wheelDelta() != 0) {
		m_inputEventReceived = true;
		renderLater();
		return;
	}
}


void SceneView::processInput() {
	// function must only be called if an input event has been received
	Q_ASSERT(m_inputEventReceived);
	m_inputEventReceived = false;

	// check for trigger key
    if (m_keyboardMouseHandler.buttonDown(Qt::LeftButton)) {
		QPoint mouseDelta = m_keyboardMouseHandler.resetMouseDelta(QCursor::pos()); // resets the internal position
        static const float rotatationSpeed  = 0.2f;
		const QVector3D LocalUp(0.0f, 1.0f, 0.0f); // same as in Camera::up()
        m_camera.rotate(rotatationSpeed * mouseDelta.x(), LocalUp);
        m_camera.rotate(rotatationSpeed * mouseDelta.y(), m_camera.right());

	}

	int wheelDelta = m_keyboardMouseHandler.resetWheelDelta();
	if (wheelDelta != 0) {
		float transSpeed = 8.f;
		if (m_keyboardMouseHandler.keyDown(Qt::Key_Shift))
			transSpeed = 0.8f;
		m_camera.translate(wheelDelta * transSpeed * m_camera.forward());
	}

	// check for picking operation
    if (m_keyboardMouseHandler.buttonReleased(Qt::RightButton)) {
//		pick(m_keyboardMouseHandler.mouseReleasePos());
    }

	// finally, reset "WasPressed" key states
	m_keyboardMouseHandler.clearWasPressedKeyStates();

	updateWorld2ViewMatrix();
	// not need to request update here, since we are called from paint anyway
}


void SceneView::updateWorld2ViewMatrix() {
	// transformation steps:
	//   model space -> transform -> world space
	//   world space -> camera/eye -> camera view
	//   camera view -> projection -> normalized device coordinates (NDC)
    m_worldToView = m_projection * m_camera.toMatrix() * m_transform.toMatrix();
}


