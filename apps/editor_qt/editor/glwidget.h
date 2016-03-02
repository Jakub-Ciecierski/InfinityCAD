#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "mainwindow.h"

#include <QGLWidget>
#include <QTimer>
#include <QtGui>

#include <vector>

#include <geometry/objects/torus.h>
#include <geometry/objects/cube.h>
#include <geometry/cameras/camera.h>
#include <geometry/projection.h>
#include <geometry/renderable.h>

class GLWidget : public QGLWidget
{
private:
    Camera* camera;
    Projection* orthogonalProjection;
    Projection* perspectiveProjection;

    std::vector<Renderable*> renderableObjects;

    int width;
    int height;

    QTimer timer;
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
public:
    explicit GLWidget(QWidget* parent = 0);
    ~GLWidget();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
};

#endif // GLWIDGET_H
