//
// Created by jakub on 6/13/16.
//

#include <infinity_cad/geometry/intersection/intersection.h>
#include <infinity_cad/geometry/intersection/pso/pso_factory.h>
#include <threading/thread_util.h>
#include <infinity_cad/geometry/intersection/pso/param_pso_object.h>
#include <infinity_cad/math/math.h>
#include <infinity_cad/rendering/scene/object_factory.h>

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
}

Intersection::~Intersection() {
    //scene->removeObject(point1_DEBUG);
    //scene->removeObject(point2_DEBUG);
}

void Intersection::normalizeTracePoint(glm::vec4& v){
    int x = abs(v.x);
    int y = abs(v.y);
    int z = abs(v.z);
    int w = abs(v.w);
    if(x == 0) x = 1;
    if(y == 0) y = 1;
    if(z == 0) z = 1;
    if(w == 0) w = 1;

    if(v.x < 0){
        v.x = 1.0f + v.x/x ;
    }
    if(v.y < 0)
        v.y = 1.0f + v.y/y;
    if(v.z < 0)
        v.z = 1.0f + v.z/z;
    if(v.w < 0)
        v.w = 1.0f + v.w/w;

    if(v.x > 1.0)
        v.x = v.x/x - 1.0f;
    if(v.y > 1.0)
        v.y = v.y/y - 1.0f;
    if(v.z > 1.0)
        v.z = v.z/z - 1.0f;
    if(v.w > 1.0)
        v.w = v.w/w - 1.0f;

}

const TracePoint& Intersection::getLastPoint(){

    if(currentTraceStatus == TraceStatus::FORWARDS){
        return tracePointsQueue.back();
    }else if(currentTraceStatus == TraceStatus::BACKWARDS){
        return tracePointsQueue.front();
    }
}

TracePoint Intersection::getInitialPoint(){
    int swarmSize = 250;
    double maxVelocity = 0.2f;
    //int maximumIterations = 6000;
    int maximumIterations = 100;
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

    ifc::printVec3(pos1);
    ifc::printVec3(pos2);

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
    const int MAX_ITER_DEBUG = 10000;
    do{
        nextPoint = findNextTraceNewton();

        ifc::printVec4(nextPoint);
        std::cout << std::endl;

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
        if(i++ > MAX_ITER_DEBUG) return;

    }while(currentTraceStatus != TraceStatus::DEAD_END);
}

vec4 Intersection::findNextTraceNewton(){
    // Find new point
    const TracePoint& tracePoint = getLastPoint();
    vec4 oldPoint = tracePoint.params;

    vec4 newPoint;
    int max_iter = 100;
    int i = 0;

    do{
        newPoint = newtonStep(oldPoint);
        ifc::printVec4(newPoint);
        if(ifc::isNan(newPoint)){
            std::cout << "isNan"  << std::endl;
            //newPoint = oldPoint;
            break;
        }
        if(checkNextPointStatus(newPoint)){
            std::cout << "Invalid interval"  << std::endl;
            //newPoint = oldPoint;
            break;
        }
        if(ifc::euclideanDistance(newPoint, oldPoint) < newtonConvergTolerance){
            std::cout << "Converging: " << newtonConvergTolerance << std::endl;
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
    vec3 P0 = surface1->compute(lastParam.x, lastParam.y);

    vec4 result;
    mat4 J;
    vec4 F;

    vec3 Np;
    vec3 Nq;
    vec3 t;

    vec3 P = surface1->compute(params.x, params.y);
    vec3 Q = surface2->compute(params.z, params.w);

    /*
    vec3 Pu = surface1->computeDu(params.x, params.y);
    vec3 Pv = surface1->computeDv(params.x, params.y);
    vec3 Pvu = surface1->computeDvu(params.x, params.y);
    vec3 Puv = surface1->computeDuv(params.x, params.y);
    vec3 Puu = surface1->computeDuu(params.x, params.y);
    vec3 Pvv = surface1->computeDvv(params.x, params.y);

    vec3 Q = surface2->compute(params.z, params.w);
    vec3 Qu = surface2->computeDu(params.z, params.w);
    vec3 Qv = surface2->computeDv(params.z, params.w);
    vec3 Quu = surface2->computeDuu(params.z, params.w);
    vec3 Qvv = surface2->computeDvv(params.z, params.w);
    vec3 Qvu = surface2->computeDvu(params.z, params.w);
    vec3 Quv = surface2->computeDuv(params.z, params.w);
    */

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

    vec3 Pu  = surface1->computeDu(params.x, params.y);
    vec3 Pv  = surface1->computeDv(params.x, params.y);
    vec3 Pvu = surface1->computeDvu(params.x, params.y);
    vec3 Puv = surface1->computeDuv(params.x, params.y);
    vec3 Puu = surface1->computeDuu(params.x, params.y);
    vec3 Pvv = surface1->computeDvv(params.x, params.y);

    vec3 Qu  = surface2->computeDu(params.z, params.w);
    vec3 Qv  = surface2->computeDv(params.z, params.w);
    vec3 Quu = surface2->computeDuu(params.z, params.w);
    vec3 Qvv = surface2->computeDvv(params.z, params.w);
    vec3 Qvu = surface2->computeDvu(params.z, params.w);
    vec3 Quv = surface2->computeDuv(params.z, params.w);

    Np = cross(Pu0, Pv0);
    Nq = cross(Qu0, Qv0);

    t = normalize(cross(Np, Nq));
    t *= r;

    std::cout << std::endl;
    ifc::printVec3(t);
    std::cout << std::endl;
/*
    vec3 tu = cross(cross(Qu, Qv), (cross(Puu, Pv) + cross(Pu,Pvu)));
    vec3 tv = cross(cross(Qu, Qv), (cross(Puv, Pv) + cross(Pu,Pvv)));
    vec3 ts = cross(cross(Pu, Pv), (cross(Quu, Qv) + cross(Qu,Qvu)));
    vec3 tt = cross(cross(Pu, Pv), (cross(Quv, Qv) + cross(Qu,Qvv)));
    */
    vec3 tu = cross(cross(Qu0, Qv0), (cross(Puu0, Pv0) + cross(Pu0,Pvu0)));
    vec3 tv = cross(cross(Qu0, Qv0), (cross(Puv0, Pv0) + cross(Pu0,Pvv0)));
    vec3 ts = cross(cross(Pu0, Pv0), (cross(Quu0, Qv0) + cross(Qu0,Qvu0)));
    vec3 tt = cross(cross(Pu0, Pv0), (cross(Quv0, Qv0) + cross(Qu0,Qvv0)));
    //tu = normalize(tu);
    //tv = normalize(tv);
    //ts = normalize(ts);
    //tt = normalize(tt);
    tu *= r;
    tv *= r;
    ts *= r;
    tt *= r;

    vec3 dP = P-P0;

    auto f4 = [this, P0, t](float x, float y,
                            float z, float w){
        /*
        int _r = 1;
        if(currentTraceStatus == TraceStatus::BACKWARDS)
            _r = -1;

        vec3 _P = surface1->compute(x, y);
        vec3 _Pu = surface1->computeDu(x, y);
        vec3 _Pv = surface1->computeDv(x, y);

        vec3 _Q = surface2->compute(z, w);
        vec3 _Qu = surface2->computeDu(z, w);
        vec3 _Qv = surface2->computeDv(z, w);

        vec3 _Np = cross(_Pu, _Pv);
        vec3 _Nq = cross(_Qu, _Qv);

        vec3 _t = normalize(cross(_Np, _Nq));
        //_t *= _r;

        vec3 _dP = _P-P0;
        float dotValue = ifc::dot(_dP, _t);

        auto value = dotValue - distance;
*/
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
/*
    F.x = r*(P.x - Q.x);
    F.y = r*(P.y - Q.y);
    F.z = r*(P.z - Q.z);
    //F.w = ifc::dot(P-P, t) - distance;
    F.w = ifc::dot(P-P0, t) - distance;

    // zeros left for clarity
    J[0].x = r*Pu.x - 0;
    J[0].y = r*Pu.y - 0;
    J[0].z = r*Pu.z - 0;
    J[0].w = ifc::dot(t, dP) + ifc::dot(t, Pu);

    J[1].x = r*Pv.x - 0;
    J[1].y = r*Pv.y - 0;
    J[1].z = r*Pv.z - 0;
    J[1].w = ifc::dot(t, dP) + ifc::dot(t, Pv);

    J[2].x = r*(0 - Qu.x);
    J[2].y = r*(0 - Qu.y);
    J[2].z = r*(0 - Qu.z);
    J[2].w = ifc::dot(t, dP);

    J[3].x = r*(0 - Qv.x);
    J[3].y = r*(0 - Qv.y);
    J[3].z = r*(0 - Qv.z);
    J[3].w = ifc::dot(t, dP);
*/
/*
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
*/

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

    J = glm::inverse(J);
    result = params - J*F;


    return result;
}

bool Intersection::updateStatus(const glm::vec4& point){
    if((point.x < 0 || point.y < 0 || point.z < 0 || point.w < 0)
       ||(point.x > 1.0f || point.y > 1.0f
          || point.z > 1.0f || point.w > 1.0f) ||
                (isnan(point.x) || isnan(point.y) ||
                        isnan(point.z) ||isnan(point.w))){
        if(currentTraceStatus == TraceStatus::FORWARDS){
            currentTraceStatus = TraceStatus::BACKWARDS;
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

void Intersection::start(){

    TracePoint initPoint = getInitialPoint();
    tracePointsQueue.push_back(initPoint);
    runTrace();


    /*
    vec3 v = surface1->compute(0.20, 0.40f);
    point1_DEBUG->moveTo(v);
    point2_DEBUG->moveTo(v);
*/
}


/*
 https://en.wikipedia.org/wiki/Numerical_differentiation
 private double Derivative(F f, double x, double y, double z, double w, DerivativeType derivativeType)
        {
            double h2 = h * 2;

            switch(derivativeType)
            {
                case DerivativeType.dx:
                    var val1 = (f(x - h2, y, z, w) - 8 * f(x - h, y, z, w) + 8 * f(x + h, y, z, w) - f(x + h2, y, z, w)) / (h2 * 6);
                    if (!val1.HasValue) absoluteBreak = true;
                    return val1.HasValue ? val1.Value : 0;
                case DerivativeType.dy:
                    var val2 = (f(x, y - h2, z, w) - 8 * f(x, y - h, z, w) + 8 * f(x, y + h, z, w) - f(x, y + h2, z, w)) / (h2 * 6);
                    if (!val2.HasValue) absoluteBreak = true;
                    return val2.HasValue ? val2.Value : 0;
                case DerivativeType.dz:
                    var val3 = (f(x, y, z - h2, w) - 8 * f(x, y, z - h, w) + 8 * f(x, y, z + h, w) - f(x, y, z + h2, w)) / (h2 * 6);
                    if (!val3.HasValue) absoluteBreak = true;
                    return val3.HasValue ? val3.Value : 0;
                case DerivativeType.dw:
                    var val4 = (f(x, y, z, w - h2) - 8 * f(x, y, z, w - h) + 8 * f(x, y, z, w + h) - f(x, y, z, w + h2)) / (h2 * 6);
                    if (!val4.HasValue) absoluteBreak = true;
                    return val4.HasValue ? val4.Value : 0;
            }

            return 0.0;
        }*/



/*

vec4 Intersection::newtonStep(const glm::vec4& params){
    // The last point found on the intersection curve
    const TracePoint& lastPoint = getLastPoint();
    vec4 lastParam = lastPoint.params;

    // Determines the direction
    int r = 1;
    if(currentTraceStatus == TraceStatus::BACKWARDS)
        r = -1;

    vec4 result;
    mat4 J;
    vec4 F;

    vec3 Np;
    vec3 Nq;
    vec3 t;

    vec3 P0 = surface1->compute(lastParam.x, lastParam.y);
    vec3 Pu0 = surface1->computeDu(lastParam.x, lastParam.y);
    vec3 Pv0 = surface1->computeDv(lastParam.x, lastParam.y);
    vec3 Qu0  = surface2->computeDu(lastParam.z, lastParam.w);
    vec3 Qv0  = surface2->computeDv(lastParam.z, lastParam.w);

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

    vec3 dP = P-P0;

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

    J = glm::inverse(J);
    result = params - J*F;

    return result;
}

*/