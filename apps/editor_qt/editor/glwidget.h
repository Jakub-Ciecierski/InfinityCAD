#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "editor_window.h"
#include <QObject>
#include <QGLWidget>
#include <QTimer>
#include <QtGui>

#include <vector>

#include <gm/rendering/ray_cast.h>
#include <gm/rendering/renderer.h>
#include <gm/scene/scene.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    RayCast* ray;
    Renderer* renderer;
    Scene* scene;

    QTimer timer;

    QPoint mouseDragPosition;
    QPoint rightMouseDragPosition;
    bool isMouseDrag;
    bool isRightMouseDrag;

    void setupRenderer();
    void setupFocusPolicy();

    void startMainLoop();

    bool do_movement();

    Color QColorToGMColor(const QColor& qc);
    QColor GMColorToQColor(const Color& c);
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

    Renderer* getRenderer();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void updateCrossView();
public slots:
    /*
     * Slot connected with check box to enable 3d rendering
     * */
    void set3DRendering(bool value);
    void set3DDistance(QString distance);

    void leftEyeColorPicker();
    void rightEyeColorPicker();

    void moveObject(const SceneID& id, glm::vec3& pos);

signals:
};

#endif // GLWIDGET_H
