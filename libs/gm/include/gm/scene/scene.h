//
// Created by jakub on 3/5/16.
//

#ifndef MG1_SCENE_H
#define MG1_SCENE_H

#include <gm/color/color.h>
#include <gm/rendering/render_object.h>
#include <gm/cameras/camera_std.h>
#include <gm/projections/stereoscopic_projection.h>
#include "gm/scene/scene_id.h"
#include "gm/scene/scene_id_factory.h"

/*
 * Scene is responsible for deallocating all objects in the scene
 */
class Scene : public Object{
private:
    //-----------------------------------------------------------//
    //  PRIVATE FIELDS
    //-----------------------------------------------------------//

    std::vector<RenderObject*> sceneObjects;

    std::vector<Camera*> cameras;
    Camera* activeCamera;

    Color sceneColor;

    bool rendering3DEnabled;

    SceneIDFactory sceneIDFactory;

    // Model of the scene, View and Projection from camera
    glm::mat4 MVP;

    void updateMVP();

    void renderAllObjects();

    void renderAllObjects3D();
public:
    //-----------------------------------------------------------//
    //  CONSTRUCTORS
    //-----------------------------------------------------------//

    Scene();

    ~Scene();

    //-----------------------------------------------------------//
    //  PUBLIC METHODS
    //-----------------------------------------------------------//

    /*
     * After the object is added to the scene, the Scene takes ownership
     * and deallocates its memory when needed.
     *
     * Returns an unique ID of the object in the scene.
     */
    SceneID addRenderObject(RenderObject* object);

    /*
     * The camera is taken to the ownership of the scene
     */
    SceneID addCamera(Camera*);

    bool setActiveCamera(Camera *);
    bool setActiveCamera(const SceneID &);

    bool removeObject(Object* object);
    bool removeObject(const SceneID& sceneID);

    void setColor(Color color);

    void set3DRendering(bool v);

    const std::vector<RenderObject*>& getRenderObjects();
    const std::vector<Camera *>& getCameras();

    Camera* getActiveCamera();

    const Color& getColor();

    const glm::mat4& getMVP();

    void renderScene();

    virtual void update() override;
};


#endif //MG1_SCENE_H
