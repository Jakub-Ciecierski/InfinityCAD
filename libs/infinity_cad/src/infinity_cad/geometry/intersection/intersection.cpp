//
// Created by jakub on 6/13/16.
//

#include <infinity_cad/geometry/intersection/intersection.h>
#include <infinity_cad/geometry/intersection/pso/pso_factory.h>
#include <threading/thread_util.h>
#include <infinity_cad/geometry/intersection/pso/param_pso_object.h>
#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/math/math.h>

using namespace pso;
using namespace glm;

Intersection::Intersection(Surface *surface1, Surface *surface2,
                           Scene* scene, float distance) :
        surface1(surface1), surface2(surface2),
        scene(scene), distance(distance){
    newtonConvergTolerance = 0.001;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    point1_DEBUG = objectFactory.createPoint("point1");
    point2_DEBUG = objectFactory.createPoint("point1");

    point1_DEBUG->setColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
    point2_DEBUG->setColor(Color(0.0f, 0.0f, 1.0f, 1.0f));

    scene->addRenderObject(point1_DEBUG);
    scene->addRenderObject(point2_DEBUG);

    currentTraceStatus = TraceStatus::FORWARDS;

    distanceInitPointTolerance = 0.2f;

    doErrorCorrection = false;
}

Intersection::~Intersection() {
    //scene->removeObject(point1_DEBUG);
    //scene->removeObject(point2_DEBUG);
}

const TracePoint& Intersection::getLastPoint(){

    if(currentTraceStatus == TraceStatus::FORWARDS){
        return tracePointsQueue.back();
    }else if(currentTraceStatus == TraceStatus::BACKWARDS){
        return tracePointsQueue.front();
    }
}


glm::vec2 Intersection::getClosestParameters(Surface* surface,
                                             glm::vec2& ndcPosition,
                                             const glm::mat4& VP,
                                             Scene* scene){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    float u = 0;
    float v = 0;

    float du = 0.01;
    float dv = 0.01;

    float minU;
    float minV;
    vec2 minParas;
    float minDistance = 99999.9f;

    while(v < 1.0f){
        while(u < 1.0f){
            vec3 point = surface->compute(u, v);
            vec4 point4 = vec4(point.x, point.y, point.z, 1.0f);
            point4 = VP * point4;
            vec2 ndcPoint = vec2(point4.x / point4.w,
                                 point4.y / point4.w);

            float dist = ifc::euclideanDistance(ndcPosition, ndcPoint);
            if(minDistance > dist){
                minDistance = dist;
                minU = u;
                minV = v;
            }
            u += du;
        }
        u = 0;
        v += dv;
    }
/*
    minDistance = 99999.9f;
    u = 0;
    v = 0;
    while(u < 1.0f){
        while(v < 1.0f){
            vec3 point = surface->compute(u, v);
            vec4 point4 = vec4(point.x, point.y, point.z, 1.0f);
            point4 = VP * point4;
            vec2 ndcPoint = vec2(point4.x / point4.w,
                                 point4.y / point4.w);

            float dist = ifc::euclideanDistance(ndcPosition, ndcPoint);
            if(minDistance > dist){
                minDistance = dist;
                minV = v;
            }
            v += dv;
        }
        u += du;
    }*/
    minParas = vec2(minU, minV);

    return minParas;
}


glm::vec2 Intersection::getClosestParameters(Surface* surface,
                                             glm::vec3& position){
    float u = 0;
    float v = 0;

    float du = 0.01;
    float dv = 0.01;

    float minU;
    float minV;
    float minDistance = 99999.9f;

    while(v < 1.0f){
        while(u < 1.0f){
            vec3 point = surface->compute(u, v);

            float dist = ifc::euclideanDistance(point, position);
            if(minDistance > dist){
                minDistance = dist;
                minU = u;
                minV = v;
            }
            u += du;
        }
        u = 0;
        v += dv;
    }
/*
    minDistance = 99999.9f;
    float minV = 0;
    u = 0;
    v = 0;
    while(u < 1.0f){
        while(v < 1.0f){
            vec3 point = surface->compute(u, v);

            float dist = ifc::euclideanDistance(point, position);
            if(minDistance > dist){
                minDistance = dist;
                minV = v;
            }
            v += dv;
        }
        u += du;
    }
*/
    vec2 minParams = vec2(minU, minV);

    return minParams;
}

TracePoint Intersection::getInitialPoint(){
    int swarmSize = 150;
    double maxVelocity = 0.2f;
    int maximumIterations = 150;
    int threadCount = threading::getNumberOfCores();
    if (threadCount < 1) threadCount = 4;

    PSOFactory psoFactory(swarmSize, maxVelocity,
                          maximumIterations, threadCount,
                          surface1, surface2);

    PSO* pso = psoFactory.createPSO();
    pso->start();

    ParamPSOObject* psoObject = (ParamPSOObject*)pso->getBestPSOObject();
    glm::vec3 pos1 = surface1->compute(psoObject->param1.x,
                                       psoObject->param1.y);
    glm::vec3 pos2 = surface2->compute(psoObject->param2.x,
                                       psoObject->param2.y);

    point1_DEBUG->moveTo(pos1);
    point2_DEBUG->moveTo(pos2);

    delete psoObject;
    delete pso;

    TracePoint tracePoint;
    tracePoint.point = pos1;
    tracePoint.params = vec4(psoObject->param1.x, psoObject->param1.y,
                             psoObject->param2.x, psoObject->param2.y);

    return tracePoint;
}

TracePoint Intersection::getInitialPoint(Point<double> startPos){
    int swarmSize = 150;
    double maxVelocity = 0.05f;
    int maximumIterations = 150;
    int threadCount = threading::getNumberOfCores();
    if (threadCount < 1) threadCount = 4;

    PSOFactory psoFactory(swarmSize, maxVelocity,
                          maximumIterations, threadCount,
                          surface1, surface2);

    PSO* pso = psoFactory.createPSO(startPos);
    pso->start();

    ParamPSOObject* psoObject = (ParamPSOObject*)pso->getBestPSOObject();
    glm::vec3 pos1 = surface1->compute(psoObject->param1.x,
                                       psoObject->param1.y);
    glm::vec3 pos2 = surface2->compute(psoObject->param2.x,
                                       psoObject->param2.y);


    point1_DEBUG->moveTo(pos1);
    point2_DEBUG->moveTo(pos2);

    delete psoObject;
    delete pso;

    TracePoint tracePoint;
    tracePoint.point = pos1;
    tracePoint.params = vec4(psoObject->param1.x, psoObject->param1.y,
                             psoObject->param2.x, psoObject->param2.y);

    return tracePoint;
}


void Intersection::runTrace(){
    vec4 nextPoint;

    int i = 0;
    const int MAX_ITER_DEBUG = 1000 * 3;
    do{
        nextPoint = findNextTraceNewton();

        //ifc::printVec4(nextPoint);
        //std::cout << std::endl;

        if(!checkNextPointStatus(nextPoint)){
            TracePoint p;
            p.params = nextPoint;
            if(currentTraceStatus == TraceStatus::FORWARDS){
                tracePointsQueue.push_back(p);
            }else if(currentTraceStatus == TraceStatus::BACKWARDS){
                tracePointsQueue.push_front(p);
            }
        }

        updateStatus(nextPoint);
        if(i++ > MAX_ITER_DEBUG) {
            //std::cout << "Max iterations reached" << std::endl;
            return;
        }

    }while(currentTraceStatus != TraceStatus::DEAD_END);
}

vec4 Intersection::findNextTraceNewton(){
    // Find new point
    const TracePoint& tracePoint = getLastPoint();
    vec4 oldPoint = tracePoint.params;

    vec4 newPoint;
    int max_iter = 5;
    int i = 0;

    do{
        newPoint = newtonStep(oldPoint);

        //ifc::printVec4(newPoint);

        if(ifc::isNan(newPoint)){
            std::cout << "isNan"  << std::endl;
            //newPoint = oldPoint;
            break;
        }
        if(checkNextPointStatus(newPoint)){
            //std::cout << "Invalid interval"  << std::endl;
            //newPoint = oldPoint;
            break;
        }
        if(ifc::euclideanDistance(newPoint, oldPoint) < newtonConvergTolerance){
            //std::cout << "Converging"  << std::endl;
            break;
        }
        oldPoint = newPoint;
    }while(i++ < max_iter);

    return newPoint;
}

vec4 Intersection::newtonStep(const glm::vec4& params){
    const TracePoint& lastPoint = getLastPoint();
    // Determines the direction
    int r = 1;
    if(currentTraceStatus == TraceStatus::BACKWARDS)
        r = -1;

    vec4 lastParam = lastPoint.params;

    vec4 result;
    mat4 J;
    vec4 F;

    vec3 Np;
    vec3 Nq;
    vec3 t;

    vec3 P0 = surface1->compute(lastParam.x, lastParam.y);
    vec3 Pu0 = surface1->computeDu(lastParam.x, lastParam.y);
    vec3 Pv0 = surface1->computeDv(lastParam.x, lastParam.y);
    vec3 Pvu0 = surface1->computeDvu(lastParam.x, lastParam.y);
    vec3 Puv0 = surface1->computeDuv(lastParam.x, lastParam.y);
    vec3 Puu0 = surface1->computeDuu(lastParam.x, lastParam.y);
    vec3 Pvv0 = surface1->computeDvv(lastParam.x, lastParam.y);

    vec3 Qu0  = surface2->computeDu(lastParam.z, lastParam.w);
    vec3 Qv0  = surface2->computeDv(lastParam.z, lastParam.w);
    vec3 Quu0 = surface2->computeDuu(lastParam.z, lastParam.w);
    vec3 Qvv0 = surface2->computeDvv(lastParam.z, lastParam.w);
    vec3 Qvu0 = surface2->computeDvu(lastParam.z, lastParam.w);
    vec3 Quv0 = surface2->computeDuv(lastParam.z, lastParam.w);

    vec3 P = surface1->compute(params.x, params.y);
    vec3 Q = surface2->compute(params.z, params.w);
    vec3 Pu  = surface1->computeDu(params.x, params.y);
    vec3 Pv  = surface1->computeDv(params.x, params.y);
    vec3 Qu  = surface2->computeDu(params.z, params.w);
    vec3 Qv  = surface2->computeDv(params.z, params.w);

    Np = cross(Pu0, Pv0);
    Nq = cross(Qu0, Qv0);

    t = normalize(cross(Np, Nq));
    t *= r;

    vec3 tu = cross(cross(Qu0, Qv0), (cross(Puu0, Pv0) + cross(Pu0,Pvu0)));
    vec3 tv = cross(cross(Qu0, Qv0), (cross(Puv0, Pv0) + cross(Pu0,Pvv0)));
    vec3 ts = cross(cross(Pu0, Pv0), (cross(Quu0, Qv0) + cross(Qu0,Qvu0)));
    vec3 tt = cross(cross(Pu0, Pv0), (cross(Quv0, Qv0) + cross(Qu0,Qvv0)));
    tu *= r;
    tv *= r;
    ts *= r;
    tt *= r;

    vec3 dP = P-P0;
/*
    auto f4 = [this, P0, t](float x, float y,
                            float z, float w){
        vec3 _P = surface1->compute(x, y);
        vec3 _dP = _P-P0;

        float dotValue = ifc::dot(_dP, t);
        auto value = dotValue - distance;

        return value;
    };

    auto F4du = ifc::derivative(f4, params.x, params.y, params.z, params.w,
                                ifc::DerivativeTypes::DX);
    auto F4dv = ifc::derivative(f4, params.x, params.y, params.z, params.w,
                                ifc::DerivativeTypes::DY);
    auto F4ds = ifc::derivative(f4, params.x, params.y, params.z, params.w,
                                ifc::DerivativeTypes::DZ);
    auto F4dt = ifc::derivative(f4, params.x, params.y, params.z, params.w,
                                ifc::DerivativeTypes::DW);
*/
    F.x = r*(P.x - Q.x);
    F.y = r*(P.y - Q.y);
    F.z = r*(P.z - Q.z);
    F.w = ifc::dot(P-P0, t) - distance;

    // zeros left for clarity
    J[0].x = r*Pu.x - 0;
    J[0].y = r*Pu.y - 0;
    J[0].z = r*Pu.z - 0;
    J[0].w = ifc::dot(tu, dP) + ifc::dot(t, Pu);

    J[1].x = r*Pv.x - 0;
    J[1].y = r*Pv.y - 0;
    J[1].z = r*Pv.z - 0;
    J[1].w = ifc::dot(tv, dP) + ifc::dot(t, Pv);

    J[2].x = r*(0 - Qu.x);
    J[2].y = r*(0 - Qu.y);
    J[2].z = r*(0 - Qu.z);
    J[2].w = ifc::dot(ts, dP);

    J[3].x = r*(0 - Qv.x);
    J[3].y = r*(0 - Qv.y);
    J[3].z = r*(0 - Qv.z);
    J[3].w = ifc::dot(tt, dP);

/*
    F.x = r*(P.x - Q.x);
    F.y = r*(P.y - Q.y);
    F.z = r*(P.z - Q.z);
    F.w = ifc::dot(dP, t) - distance;

    // zeros left for clarity
    J[0].x = r*Pu.x;
    J[0].y = r*Pu.y;
    J[0].z = r*Pu.z;
    J[0].w = F4du;

    J[1].x = r*Pv.x;
    J[1].y = r*Pv.y;
    J[1].z = r*Pv.z;
    J[1].w = F4dv;

    J[2].x = r*(-Qu.x);
    J[2].y = r*(-Qu.y);
    J[2].z = r*(-Qu.z);
    J[2].w = F4ds;

    J[3].x = r*(-Qv.x);
    J[3].y = r*(-Qv.y);
    J[3].z = r*(-Qv.z);
    J[3].w = F4dt;
*/
    J = glm::inverse(J);
    result = params - J*F;
    //if(doErrorCorrection) normalizeNumericalError(result);

    return result;
}

bool Intersection::updateStatus(const glm::vec4& point){
    if((point.x < 0 || point.y < 0 || point.z < 0 || point.w < 0)
       ||(point.x > 1.0f || point.y > 1.0f
          || point.z > 1.0f || point.w > 1.0f) ||
                (isnan(point.x) || isnan(point.y) ||
                        isnan(point.z) ||isnan(point.w))){
        ifc::printVec4(point);
        if(currentTraceStatus == TraceStatus::FORWARDS){
            currentTraceStatus = TraceStatus::BACKWARDS;
            //currentTraceStatus = TraceStatus::DEAD_END;
            std::cout << "Status: BACKWARDS" << std::endl;
            return true;
        }else if(currentTraceStatus == TraceStatus::BACKWARDS){
            currentTraceStatus = TraceStatus::DEAD_END;
            std::cout << "Status: DEAD_END" << std::endl;
            return true;
        }
    }
    return false;
}

bool Intersection::checkNextPointStatus(const glm::vec4& point){
    return((point.x < 0 || point.y < 0 || point.z < 0 || point.w < 0)
       ||(point.x > 1.0f || point.y > 1.0f
          || point.z > 1.0f || point.w > 1.0f) ||
       (isnan(point.x) || isnan(point.y) ||
        isnan(point.z) ||isnan(point.w)));
}

void Intersection::normalizeNumericalError(glm::vec4& point){
    float err = 0.009;
    if(point.x > 1.0f && (point.x - err) <= 1.0f) point.x = 1.0f;
    if(point.y > 1.0f && (point.y - err) <= 1.0f) point.y = 1.0f;
    if(point.z > 1.0f && (point.z - err) <= 1.0f) point.z = 1.0f;
    if(point.w > 1.0f && (point.w - err) <= 1.0f) point.w = 1.0f;

    if(point.x < 0.0f && (point.x + err) >= 0.0f) point.x = 0.0f;
    if(point.y < 0.0f && (point.y + err) >= 0.0f) point.y = 0.0f;
    if(point.z < 0.0f && (point.z + err) >= 0.0f) point.z = 0.0f;
    if(point.w < 0.0f && (point.w + err) >= 0.0f) point.w = 0.0f;
}

const std::vector<TracePoint>& Intersection::getTracePoints(){
    int size = tracePointsQueue.size();
    tracePoints.clear();
    tracePoints.resize(size);

    for(int i = 0; i < size; i++){
        tracePoints[i] = tracePointsQueue[i];
    }

    return this->tracePoints;
}

std::vector<glm::vec3> Intersection::getComputedPoints(){
    int size = tracePointsQueue.size();
    std::vector<glm::vec3> points(size);

    for(int i = 0; i < size; i++){
        TracePoint& tp = tracePointsQueue[i];
        glm::vec3 point = surface1->compute(tp.params.x, tp.params.y);
        points[i] = point;
    }

    return points;
}

bool Intersection::start(){
    TracePoint initPoint = getInitialPoint();
    vec3 pointPos1 = surface1->compute(initPoint.params.x, initPoint.params.y);
    vec3 pointPos2 = surface2->compute(initPoint.params.z, initPoint.params.w);
    float dist = ifc::euclideanDistance(pointPos1, pointPos2);
    if(dist > distanceInitPointTolerance) return false;

    tracePointsQueue.push_back(initPoint);
    runTrace();

    return true;
}

bool Intersection::start(glm::vec2& ndcPosition, const glm::mat4& VP,
                         Scene* scene){
    doErrorCorrection = true;
    vec2 param1 = getClosestParameters(surface1, ndcPosition, VP, scene);
    vec3 pointPos1 = surface1->compute(param1.x, param1.y);

    vec2 param2 = getClosestParameters(surface2, pointPos1);
    vec3 pointPos2 = surface2->compute(param2.x, param2.y);

    float dist = ifc::euclideanDistance(pointPos1, pointPos2);
    if(dist > distanceInitPointTolerance) return false;

    vec4 params = vec4(param1.x, param1.y, param2.x, param2.y);
    Point<double> point(4);
    point[0] = params.x;
    point[1] = params.y;
    point[2] = params.z;
    point[3] = params.w;

    TracePoint initPoint = getInitialPoint(point);
    tracePointsQueue.push_back(initPoint);
    runTrace();

    return true;
}