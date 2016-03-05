#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "mainwindow.h"

#include <QGLWidget>
#include <QTimer>
#include <QtGui>

#include <vector>

#include <gm/rendering/renderer.h>
#include <gm/scene/scene.h>

class GLWidget : public QGLWidget
{
private:
    Renderer* renderer;
    Scene* scene;

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
