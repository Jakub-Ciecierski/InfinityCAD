#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "mainwindow.h"

#include <QGLWidget>
#include <QTimer>
#include <QtGui>

#include <vector>

#include <gm/rendering/renderer.h>
#include <gm/scene/scene.h>
#include <gm/ray_casting/ray.h>
#include <gm/ray_casting/ellipsoid.h>

class GLWidget : public QGLWidget
{
private:
    Renderer* renderer;
    Scene* scene;

    QTimer timer;

    QPoint mouseDragPosition;
    QPoint rightMouseDragPosition;
    bool isMouseDrag;
    bool isRightMouseDrag;

    void setupRenderer();
    void setupFocusPolicy();
    void setupRayTracing();

    void startMainLoop();

    void do_movement();

    Ray* ray;
    Ellipsoid* ellipsoid;
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent* event);
public:
    explicit GLWidget(QWidget* parent = 0);
    ~GLWidget();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
};

#endif // GLWIDGET_H
