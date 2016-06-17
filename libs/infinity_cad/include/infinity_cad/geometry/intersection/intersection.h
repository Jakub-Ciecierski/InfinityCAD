//
// Created by jakub on 6/13/16.
//

#ifndef IC_INTERSECTION_H
#define IC_INTERSECTION_H

#include <infinity_cad/rendering/render_objects/surfaces/surface.h>
#include <infinity_cad/rendering/scene/scene.h>
#include <deque>
#include <math/point.h>

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

    bool doErrorCorrection;

    float distance;
    float newtonConvergTolerance;
    float distanceInitPointTolerance;
    TraceStatus currentTraceStatus;

    const TracePoint& getLastPoint();

    glm::vec2 getClosestParameters(Surface* surface,
                                   glm::vec2& ndcPosition,
                                   const glm::mat4& VP,
                                   Scene* scene);
    glm::vec2 getClosestParameters(Surface* surface,
                                   glm::vec3& position);
    TracePoint getInitialPoint();
    TracePoint getInitialPoint(Point<double> startPos);

    void runTrace();
    glm::vec4 findNextTraceNewton();
    glm::vec4 newtonStep(const glm::vec4& params);

    bool updateStatus(const glm::vec4& point);
    bool checkNextPointStatus(const glm::vec4& point);
    void normalizeNumericalError(glm::vec4& point);

public:

    Intersection(Surface* surface1,
                 Surface* surface2,
                 Scene* scene, float distance = 0.01);

    ~Intersection();

    const std::vector<TracePoint>& getTracePoints();
    std::vector<glm::vec3> getComputedPoints();

    bool start();
    bool start(glm::vec2& ndcPosition, const glm::mat4& VP, Scene* scene);
};


#endif //IC_INTERSECTION_H
