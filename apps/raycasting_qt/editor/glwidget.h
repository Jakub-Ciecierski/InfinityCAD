#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "mainwindow.h"
#include <QObject>
#include <QGLWidget>
#include <QTimer>
#include <QtGui>

#include <vector>

#include <gm/rendering/renderer.h>
#include <gm/scene/scene.h>
#include <rc/ray.h>
#include <rc/ellipsoid.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
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

    bool do_movement();

    Ray* ray;
    Ellipsoid* ellipsoid;
    float sliderValueDivider;
    int sliderValueMax;
    bool continueAdaptiveRendering;

    bool doAdaptive;

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

public slots:
    void setEllipsoidARadius(int value);
    void setEllipsoidBRadius(int value);
    void setEllipsoidCRadius(int value);

    void setRayLightIntensity(int value);

    void setDoAdaptive(bool v);

signals:
    void ellipsoidARadiusChanged(int value);
    void ellipsoidBRadiusChanged(int value);
    void ellipsoidCRadiusChanged(int value);
};

#endif // GLWIDGET_H
