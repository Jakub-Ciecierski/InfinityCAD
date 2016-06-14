//
// Created by jakub on 6/13/16.
//

#ifndef IC_INTERSECTION_H
#define IC_INTERSECTION_H


#include <infinity_cad/rendering/render_objects/surfaces/surface.h>
#include <infinity_cad/rendering/scene/scene.h>
#include <deque>

struct TracePoint{
    glm::vec4 params;

    glm::vec3 point;
};

enum TraceStatus{
    FORWARDS, BACKWARDS, DEAD_END
};

class Intersection {
private:
    Surface* surface1;
    Surface* surface2;

    Scene* scene;

    ifc::Point* point1_DEBUG;
    ifc::Point* point2_DEBUG;

    std::vector<TracePoint> tracePoints;
    std::deque<TracePoint> tracePointsQueue;

    float distance;
    float newtonConvergTolerance;
    TraceStatus currentTraceStatus;

    const TracePoint& getLastPoint();
    void normalizeTracePoint(glm::vec4& v);
    TracePoint getInitialPoint();

    void runTrace();
    glm::vec4 findNextTraceNewton();
    glm::vec4 newtonStep(const glm::vec4& params);

    void checkNextPointStatus(const glm::vec4& point);
public:

    Intersection(Surface* surface1,
                 Surface* surface2,
                 Scene* scene, float distance = 0.01);

    ~Intersection();

    const std::vector<TracePoint>& getTracePoints();

    void start();
};


#endif //IC_INTERSECTION_H
