#include "inc/Drone.h"

#include <memory>

Drone::Drone() {
    // Corpse = std::make_unique<Object>(QVector3D(50.0f, 50.0f, 50.0f),
    //                                   QVector3D(0.0f, 1.0f, 0.0f));

    m_objects.push_back(
        std::make_unique<Object>(QVector3D(50.0f, 50.0f, 50.0f),
                                 QVector3D(0.0f, 1.0f, 0.0f))
        );
}

bool Drone::setShaders(QOpenGLShaderProgram *shaderProgramm){

    for(auto& object : m_objects)
        object->setShaders(shaderProgramm);
    return true;
}

std::vector<Matrices> Drone::setMatrices(const QMatrix4x4& worldToView) const{
    std::vector<Matrices> listOfMatrices;
    Matrices Metrix;

    for(auto& object : m_objects){
        Metrix.mvpMatrix = worldToView * object->returnModelMatrix();
        Metrix.normalMatrix = object->returnModelMatrix().inverted().transposed();
        listOfMatrices.push_back(Metrix);
    }
    return listOfMatrices;
}

void Drone::drawDrone() const{
    for(auto& object : m_objects)
        object->drawBox();
}
