#include "inc/Drone.h"

#include <memory>

Drone::Drone() {

    m_objects.emplace(
        "Corpse",
        std::make_unique<Object>(QVector3D(70.0f, 20.0f, 50.0f),
                                 QVector3D(0.0f, 20.0f, 0.0f))
        );


    m_objects.emplace(
        "Element1",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 40.0f),
                                 QVector3D(70.0f, 45.0f, -50.0f),
                                 -45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Element2",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 10.0f),
                                 QVector3D(90.0f, 55.0f, -70.0f),
                                 -45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Wing",
        std::make_unique<Object>(QVector3D(10.0f, 2.5f, 60.0f),
                                 QVector3D(90.0f, 60.0f, -70.0f),
                                 -45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );



    m_objects.emplace(
        "Element1",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 40.0f),
                                 QVector3D(70.0f, 45.0f, 50.0f),
                                 45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Element2",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 10.0f),
                                 QVector3D(90.0f, 55.0f, 70.0f),
                                 45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Wing",
        std::make_unique<Object>(QVector3D(10.0f, 2.5f, 60.0f),
                                 QVector3D(90.0f, 60.0f, 70.0f),
                                 45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );



    m_objects.emplace(
        "Element1",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 40.0f),
                                 QVector3D(-70.0f, 45.0f, -50.0f),
                                 45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Element2",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 10.0f),
                                 QVector3D(-90.0f, 55.0f, -70.0f),
                                 45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Wing",
        std::make_unique<Object>(QVector3D(10.0f, 2.5f, 60.0f),
                                 QVector3D(-90.0f, 60.0f, -70.0f),
                                 45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );



    m_objects.emplace(
        "Element1",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 40.0f),
                                 QVector3D(-70.0f, 45.0f, 50.0f),
                                 -45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Element2",
        std::make_unique<Object>(QVector3D(10.0f, 5.0f, 10.0f),
                                 QVector3D(-90.0f, 55.0f, 70.0f),
                                 -45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

    m_objects.emplace(
        "Wing",
        std::make_unique<Object>(QVector3D(10.0f, 2.5f, 60.0f),
                                 QVector3D(-90.0f, 60.0f, 70.0f),
                                 -45,
                                 QVector3D(0.0f, 1.0f, 0.0f)
                                 )
        );

}

bool Drone::setShaders(QOpenGLShaderProgram *shaderProgramm){

    for(auto& object : m_objects)
        object.second->setShaders(shaderProgramm);
    return true;
}

std::vector<Matrices> Drone::setMatrices(const QMatrix4x4& worldToView) const{
    std::vector<Matrices> listOfMatrices;
    Matrices Metrix;

    for(const auto& object : m_objects){
        Metrix.mvpMatrix = worldToView * object.second->returnModelMatrix();
        Metrix.normalMatrix = object.second->returnModelMatrix().inverted().transposed();
        listOfMatrices.push_back(Metrix);
    }
    return listOfMatrices;
}

void Drone::drawElement(const int & index) const{
    auto it = m_objects.begin();
    std::advance(it, index);

    if (it != m_objects.end())
        it->second->drawObject();
}

void Drone::startPropellers() {
    auto range = m_objects.equal_range("Wing");

    for (auto it = range.first; it != range.second; ++it)
    {
        it->second->rotateObject();
    }
}

void Drone::moveDrone(){
    // m_objects.first->translateObject();
}
