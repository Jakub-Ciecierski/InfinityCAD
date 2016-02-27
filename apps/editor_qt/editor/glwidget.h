#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <geometry/objects/torus.h>

class GLWidget : public QGLWidget
{
private:
    Torus* torus;
    int width;
    int height;
public:
    explicit GLWidget(QWidget* parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
};

#endif // GLWIDGET_H
