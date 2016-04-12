//
// Created by jakub on 4/12/16.
//

#include <gtest/gtest.h>
#include <gm/rendering/render_bodies/curves/bspline.h>
#include <gm/scene/object_factory.h>

TEST(BSpline3D, KnotVectorCount) {
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    BSpline* spline = objectFactory.createBSpline("test");
    std::vector<ic::Point*> points;

    // n control points
    int n = 6;
    int degree = 3;
    int expectedKnotCount = n + degree;
    for(int i = 0; i < n; i++){
        points.push_back(objectFactory.createPoint("test"));
        spline->addPoint(points[i]);
    }

    int actualKnotCount = spline->getKnotCount();

    delete spline;
    for(int i = 0; i < n; i++) delete points[i];

    EXPECT_EQ(expectedKnotCount, actualKnotCount);
}

TEST(BSpline3D, KnotVectorValues) {
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    BSpline* spline = objectFactory.createBSpline("test");
    std::vector<ic::Point*> points;

    // n control points
    int n = 8;
    int degree = 3;
    int expectedKnotCount = n + degree;
    for(int i = 0; i < n; i++){
        points.push_back(objectFactory.createPoint("test"));
        spline->addPoint(points[i]);
    }

    const std::vector<float>& knots = spline->getKnotVector();
/*
    for(unsigned int i = 0; i < knots.size(); i++){
        std::cout << knots[i] << ", " << std::endl;
    }
*/
    delete spline;
    for(int i = 0; i < n; i++) delete points[i];
}

TEST(BSpline3D, KnotVectorMin) {
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    BSpline* spline = objectFactory.createBSpline("test");
    std::vector<ic::Point*> points;

    // n control points
    int n = 8;
    int degree = 3;
    int expectedKnotMin = 0;
    for(int i = 0; i < n; i++){
        points.push_back(objectFactory.createPoint("test"));
        spline->addPoint(points[i]);
    }

    int actualKnotMin = spline->getKnotMin();

    delete spline;
    for(int i = 0; i < n; i++) delete points[i];

    EXPECT_EQ(expectedKnotMin, actualKnotMin);
}

TEST(BSpline3D, KnotVectorMax) {
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    BSpline* spline = objectFactory.createBSpline("test");
    std::vector<ic::Point*> points;

    // n control points
    int n = 8;
    int degree = 3;
    int expectedKnotMax = n - degree +1;
    for(int i = 0; i < n; i++){
        points.push_back(objectFactory.createPoint("test"));
        spline->addPoint(points[i]);
    }

    int actualKnotMax = spline->getKnowMax();

    delete spline;
    for(int i = 0; i < n; i++) delete points[i];

    EXPECT_EQ(expectedKnotMax, actualKnotMax);
}