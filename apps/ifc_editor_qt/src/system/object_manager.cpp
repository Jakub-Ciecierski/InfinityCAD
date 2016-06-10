#include <system/object_manager.h>
#include "system/ifc_types.h"

#include <infinity_cad/rendering/scene/object_factory.h>
#include <infinity_cad/rendering/color/color_settings.h>
#include <dialogs/surface_c0_dialog.h>
#include <ui_mainwindow.h>

#include <system/serialization/serialization_scene.h>
#include <system/serialization/deserialization_scene.h>

using namespace std;
using namespace ifc;

//--------------------------//
//  PRIVATE
//--------------------------//

ObjectManager::ObjectManager(){
    EditorWindow& m = EditorWindow::getInstance();
    Ui::MainWindow* ui = m.getUI();

    this->glWidget = ui->glRendererWidget;
    this->scene = ui->glRendererWidget->getRenderer()->getScene();
    this->sceneTree = ui->sceneTree;

    bSplineBinding = new BSplineBinding(scene, sceneTree);
    bSplineInterpBinding = new BSplineInterpBinding(scene, sceneTree);
}

Item * ObjectManager::addTorus(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderObject * t = objectFactory.createTorus(name);

    this->scene->addRenderObject(t);
    Item* item = sceneTree->addObject(t, RB_TORUS_TYPE);

    return item;
}

Item * ObjectManager::addPoint(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderObject * p = objectFactory.createPoint(name);

    this->scene->addRenderObject(p);
    Item* pointItem = sceneTree->addObject(p, RB_POINT_TYPE);

    // Add Point to all selected bezier curves
    std::vector<Item*> selectedBezierItems =
            sceneTree->getSelectedItems(RB_BEZIER_TYPE);
    for(unsigned int i = 0; i < selectedBezierItems.size(); i++){
        addChildItem(selectedBezierItems[i], pointItem);
    }

    selectedBezierItems = sceneTree->getSelectedItems(RB_BSPLINE_TYPE);
    for(unsigned int i = 0; i < selectedBezierItems.size(); i++){
        addChildItem(selectedBezierItems[i], pointItem);
    }

    selectedBezierItems =
            sceneTree->getSelectedItems(RB_BSPLINE_INTERPOLATING_TYPE);
    for(unsigned int i = 0; i < selectedBezierItems.size(); i++){
        addChildItem(selectedBezierItems[i], pointItem);
    }
    return pointItem;
}

Item * ObjectManager::addBezierCurve(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    RenderObject * p = objectFactory.createBezier(name);

    this->scene->addRenderObject(p);
    Item* bezierItem = sceneTree->addObject(p, RB_BEZIER_TYPE);

    // Add all selected points to curve
    std::vector<Item*> selectedPointItems =
            sceneTree->getSelectedItems(RB_POINT_TYPE);
    for(unsigned int i = 0; i < selectedPointItems.size(); i++){
        addChildItem(bezierItem, selectedPointItems[i]);
    }

    return bezierItem;
}

Item* ObjectManager::addSurfaceC0Rect(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    SurfaceC0Dialog dialog;

    dialog.exec();
    bool result = dialog.getResult();

    int n,m;
    float width, height;
    if(result){
        SurfaceC0Data data = dialog.getData();
        n = data.n;
        m = data.m;
        width = data.width;
        height = data.height;
    }else{
        return NULL;
    }

    SurfaceC0Rect * surface = objectFactory.createSurfaceC0Rect(name, n, m,
                                                                width, height);
    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C0_RECT_TYPE);

    const std::vector<ifc::Point*>& points = surface->getAllPoints();
    for(unsigned int i = 0; i < points.size(); i++){
        this->scene->addRenderObject(points[i]);
        Item* pointItem = this->sceneTree->addObject(points[i], RB_POINT_TYPE);
        sceneTree->addChildItem(surfaceItem, pointItem);
    }

    return surfaceItem;
}

Item* ObjectManager::addSurfaceC0Rect(string name,
                                      Matrix<ifc::Point*> pointsMatrix,
                                      vector<Item*> pointItems){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();


    SurfaceC0Rect * surface = objectFactory.createSurfaceC0Rect(name,
                                                                pointsMatrix);
    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C0_RECT_TYPE);

    for(unsigned int i = 0; i < pointItems.size(); i++){
        sceneTree->addChildItem(surfaceItem, pointItems[i]);
    }

    return surfaceItem;
}

Item* ObjectManager::addSurfaceC0Cylind(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    SurfaceC0Dialog dialog(RB_SURFACE_C0_CYLIND_TYPE);

    dialog.exec();
    bool result = dialog.getResult();

    int n,m;
    float radius, height;
    if(result){
        SurfaceC0Data data = dialog.getData();
        n = data.n;
        m = data.m;
        radius = data.radius;
        height = data.height;
    }else{
        return NULL;
    }

    SurfaceC0Cylind* surface = objectFactory.createSurfaceC0Cylind(name, n, m,
                                                                   radius, height);
    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C0_CYLIND_TYPE);

    const std::vector<ifc::Point*>& points = surface->getAllPoints();
    for(unsigned int i = 0; i < points.size(); i++){
        this->scene->addRenderObject(points[i]);
        Item* pointItem = this->sceneTree->addObject(points[i], RB_POINT_TYPE);
        sceneTree->addChildItem(surfaceItem, pointItem);
    }

    return surfaceItem;
}

Item* ObjectManager::addSurfaceC0Cylind(string name,
                                        Matrix<ifc::Point*> pointsMatrix,
                                        vector<Item*> pointItems){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    SurfaceC0Cylind* surface = objectFactory.createSurfaceC0Cylind(name, pointsMatrix);

    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C0_CYLIND_TYPE);

    for(unsigned int i = 0; i < pointItems.size(); i++){
        sceneTree->addChildItem(surfaceItem, pointItems[i]);
    }

    return surfaceItem;
}

Item* ObjectManager::addSurfaceC2Rect(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    SurfaceC0Dialog dialog(RB_SURFACE_C2_RECT_TYPE);

    dialog.exec();
    bool result = dialog.getResult();

    int n,m;
    float width, height;
    if(result){
        SurfaceC0Data data = dialog.getData();
        n = data.n;
        m = data.m;
        width = data.width;
        height = data.height;
    }else{
        return NULL;
    }

    SurfaceC2Rect* surface = objectFactory.createSurfaceC2Rect(name, n, m,
                                                                width, height);
    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C2_RECT_TYPE);

    const std::vector<ifc::Point*>& points = surface->getAllPoints();
    for(unsigned int i = 0; i < points.size(); i++){
        this->scene->addRenderObject(points[i]);
        Item* pointItem = this->sceneTree->addObject(points[i], RB_POINT_TYPE);
        sceneTree->addChildItem(surfaceItem, pointItem);
    }

    return surfaceItem;
}

Item* ObjectManager::addSurfaceC2Rect(string name,
                                      Matrix<ifc::Point*> pointsMatrix,
                                      vector<Item*> pointItems){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    SurfaceC2Rect* surface = objectFactory.createSurfaceC2Rect(name, pointsMatrix);

    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C2_RECT_TYPE);

    for(unsigned int i = 0; i < pointItems.size(); i++){
        sceneTree->addChildItem(surfaceItem, pointItems[i]);
    }

    return surfaceItem;
}

Item* ObjectManager::addSurfaceC2Cylind(string name){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    SurfaceC0Dialog dialog(RB_SURFACE_C2_CYLIND_TYPE);

    dialog.exec();
    bool result = dialog.getResult();

    int n,m;
    float radius, height;
    SurfaceAxis surfaceAxis;

    if(result){
        SurfaceC0Data data = dialog.getData();
        n = data.n;
        m = data.m;
        radius = data.radius;
        height = data.height;
        surfaceAxis = data.surfaceAxis;
    }else{
        return NULL;
    }

    SurfaceC2Cylind* surface
            = objectFactory.createSurfaceC2Cylind(name, n, m,
                                                  radius, height,
                                                  surfaceAxis);
    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C2_CYLIND_TYPE);

    const std::vector<ifc::Point*>& points = surface->getAllPoints();
    for(unsigned int i = 0; i < points.size(); i++){
        this->scene->addRenderObject(points[i]);
        Item* pointItem = this->sceneTree->addObject(points[i], RB_POINT_TYPE);
        sceneTree->addChildItem(surfaceItem, pointItem);
    }

    return surfaceItem;
}

Item* ObjectManager::addSurfaceC2Cylind(string name,
                                        Matrix<ifc::Point*> pointsMatrix,
                                        vector<Item*> pointItems){
    ObjectFactory& objectFactory = ObjectFactory::getInstance();

    SurfaceC2Cylind* surface = objectFactory.createSurfaceC2Cylind(name,
                                                                   pointsMatrix);
    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C2_CYLIND_TYPE);

    for(unsigned int i = 0; i < pointItems.size(); i++){
        sceneTree->addChildItem(surfaceItem, pointItems[i]);
    }

    return surfaceItem;
}


string ObjectManager::getDefaultName(const Type& type){
    static int id_count = 0;
    string defaultName = type.type + "_" + to_string(id_count++);

    return defaultName;
}

//--------------------------//
//  PUBLIC
//--------------------------//

ObjectManager::~ObjectManager(){
    delete bSplineBinding;
    delete bSplineInterpBinding;
}

ObjectManager& ObjectManager::getInstance(){
    static ObjectManager objectManager;

    return objectManager;
}

Item* ObjectManager::addObject(const Type& type){
    string defaultName = getDefaultName(type);

    return addObject(type, defaultName);
}

Item* ObjectManager::addObject(const Type& type, string name){
    if(sceneTree->objectExists(name)) {
        string title = "Name";
        string text = "Name: " + name + " already exists. Try other name";
        EditorWindow::getInstance().showInfoBox(title, text);
        return NULL;
    }

    Item* item = NULL;
    if(type == RB_TORUS_TYPE){
        item = addTorus(name);
    }else if(type == RB_POINT_TYPE){
        item = addPoint(name);
    }else if(type == RB_BEZIER_TYPE){
        item = addBezierCurve(name);
    }else if(type == RB_BSPLINE_TYPE){
        item = bSplineBinding->createBSpline(name);
    }else if(type == RB_BSPLINE_INTERPOLATING_TYPE){
        item = bSplineInterpBinding->createBSplineInterp(name);
    }else if(type == RB_SURFACE_C0_RECT_TYPE){
        item = addSurfaceC0Rect(name);
    }else if(type == RB_SURFACE_C0_CYLIND_TYPE){
        item = addSurfaceC0Cylind(name);
    }else if(type == RB_SURFACE_C2_RECT_TYPE){
        item = addSurfaceC2Rect(name);
    }else if(type == RB_SURFACE_C2_CYLIND_TYPE){
        item = addSurfaceC2Cylind(name);
    }
    if(item != NULL){
        Cross* cross = scene->getCross();
        item->object->moveTo(cross);
        if(cross->isGrabAttracting()){
            sceneTree->deactivateAll();
            sceneTree->activateObject(item->object);
        }
    }

    return item;
}

void ObjectManager::addChildItem(Item* parentItem,
                                 Item* childItem){
    if(parentItem == NULL || childItem == NULL ||
            childItem->type != RB_POINT_TYPE){
        return;
    }

    // Check if point is already added
    for(unsigned int i = 0; i < parentItem->children.size();i++){
        Item* currentChildItem = parentItem->children[i];
        if(currentChildItem != NULL && *currentChildItem == *childItem)
            return;
    }

    if(canAddChildren(parentItem->type)){
        Spline* spline = static_cast<Spline*>(parentItem->object);
        Point* point = static_cast<Point*>(childItem->object);

        sceneTree->addChildItem(parentItem, childItem);
        spline->addPoint(point);
    }

    return;
}

void ObjectManager::removeChildItem(Item* objectItem){
    Item* bezierItem = objectItem->parent;
    if(bezierItem == NULL || objectItem == NULL ||
            objectItem->type != RB_POINT_CLONE_TYPE){
        return;
    }

    if(canAddChildren(bezierItem->type)){
        Spline* spline = static_cast<Spline*>(bezierItem->object);
        Point* point = static_cast<Point*>(objectItem->object);

        sceneTree->deleteObject(objectItem);
        spline->removePoint(point);
    }
}

void ObjectManager::moveUpItem(Item* objectItem){
    Item* bezierItem = objectItem->parent;
    if(bezierItem == NULL || objectItem == NULL ||
            objectItem->type != RB_POINT_CLONE_TYPE) {
        return;
    }

    if(canAddChildren(bezierItem->type)){
        Spline* spline = static_cast<Spline*>(bezierItem->object);
        Point* point = static_cast<Point*>(objectItem->object);

        sceneTree->moveItemUpWithinParent(objectItem);
        spline->moveUp(point);
    }
}

void ObjectManager::moveDownItem(Item* objectItem){
    Item* bezierItem = objectItem->parent;
    if(bezierItem == NULL || objectItem == NULL ||
            objectItem->type != RB_POINT_CLONE_TYPE){
        return;
    }
    if(canAddChildren(bezierItem->type)){
        Spline* spline = static_cast<Spline*>(bezierItem->object);
        Point* point = static_cast<Point*>(objectItem->object);

        sceneTree->moveItemDownWithinParent(objectItem);
        spline->moveDown(point);
    }
}

void ObjectManager::deleteObject(Item* item){
    if(item->treeItem == NULL) return;

    if(item->type == RB_POINT_TYPE){
        std::vector<Item*> surfaces = item->getConnectedSurfaces();
        if(surfaces.size() > 0){
            string text = "Can't delete point connected to a Surface";
            string title = "Warning";
            EditorWindow::getInstance().showInfoBox(title, text);
            return;
        }
    }

    if(isSurface(item->type)){
        std::vector<Item*> originalPoints = item->getOriginalChildrenItems();

        SceneID id = this->sceneTree->deleteObject(item);
        this->scene->removeObject(id);

        string text = "Delete all the points as well ?";
        string title = "Delete";
        if(EditorWindow::getInstance().showQuestionBox(title, text)){
            for(unsigned int i = 0;i < originalPoints.size();i++){
                deleteObject(originalPoints[i]);
            }
        }
    }else{
        SceneID id = this->sceneTree->deleteObject(item);
        this->scene->removeObject(id);
    }
}

void ObjectManager::changeName(Item* item){
    string text = "Input new name";
    string title = "Change Name";
    string dstName = EditorWindow::getInstance().showInputBox(title, text);

    if(sceneTree->objectExists(dstName)) {
        string title = "Name";
        string text = "Name: " + dstName + " already exists. Try other name";
        EditorWindow::getInstance().showInfoBox(title, text);
        return;
    }
    if(dstName.empty()) return;

    sceneTree->changeName(item, dstName);
}

void ObjectManager::moveCross(Item* item){
    if(item == NULL) return;
    Cross* cross = scene->getCross();
    cross->moveTo(item->object);
}

void ObjectManager::moveCamera(Item * item){
    if(item == NULL) return;

    Camera* camera = scene->getActiveCamera();
    camera->moveTo(item->object);
}

void ObjectManager::setActive(SceneID id){

    RenderObject * body = scene->getRenderBody(id);
    if(body != NULL)
        body->setColor(COLOR_OBJECT_ACTIVE);
}

void ObjectManager::setDeactive(SceneID id){
    RenderObject * body = scene->getRenderBody(id);
    if(body != NULL)
        body->setColor(COLOR_OBJECT_DEFAULT);
}

void ObjectManager::saveSystem(std::string filepath){
    SerializationScene serializer(this->scene);

    serializer.save(filepath);
}

void ObjectManager::loadSystem(std::string filepath){
    DeserializationScene deserializer;

    deserializer.load(filepath);
}

SceneTree* ObjectManager::getSceneTree(){
    return this->sceneTree;
}

void ObjectManager::colapsSelectedPoints(){
    EditorWindow& window = EditorWindow::getInstance();
    const vector<Item*>& items = sceneTree->getSelectedItems(RB_POINT_NAME);
    if(items.size() != 2){
        window.showInfoBox("Colaps", "Please select 2 points");
        return;
    }

    Item* pointItem1 = items[0];
    Item* pointItem2 = items[1];

    glm::vec3 avgPosition = (pointItem1->object->getPosition() +
            pointItem2->object->getPosition());
    avgPosition *= 0.5f;

    Item* newPoint = addPoint(pointItem1->object->getName() + "_tmp");
    newPoint->object->moveTo(avgPosition);

    vector<Item*>& clones1 = pointItem1->clones;
    vector<Item*>& clones2 = pointItem2->clones;

    Item* surfacePointClone1 = NULL;
    Item* surfacePointClone2 = NULL;

    Item* surfaceItem1 = NULL;
    Item* surfaceItem2 = NULL;
    for(unsigned int i = 0; i < clones1.size(); i++){
        Item* clone = clones1[i];
        if(isSurface(clone->parent->type)){
            surfaceItem1 = clone->parent;
            surfacePointClone1 = clone;
            break;
        }
    }
    for(unsigned int i = 0; i < clones2.size(); i++){
        Item* clone = clones2[i];
        if(isSurface(clone->parent->type)){
            surfaceItem2 = clone->parent;
            surfacePointClone2 = clone;
            break;
        }
    }
    if(surfaceItem1 == NULL && surfaceItem2 == NULL){
        window.showInfoBox("Colaps", "Points must belong to a surface");
        return;
    }
    Surface* surface1 = static_cast<Surface*>(surfaceItem1->object);
    Surface* surface2 = static_cast<Surface*>(surfaceItem2->object);

    bool ret1 = surface1->replacePoint(static_cast<Point*>(pointItem1->object),
                                       static_cast<Point*>(newPoint->object));
    bool ret2 = surface2->replacePoint(static_cast<Point*>(pointItem2->object),
                                       static_cast<Point*>(newPoint->object));
    if(!ret1 || !ret2){
        window.showInfoBox("Colaps", "Points not found");
        return;
    }

    SceneID id1 = sceneTree->deleteObject(pointItem1);
    sceneTree->deleteObject(surfacePointClone1);
    this->scene->removeObject(id1);

    SceneID id2 = sceneTree->deleteObject(pointItem2);
    sceneTree->deleteObject(surfacePointClone2);
    this->scene->removeObject(id2);

    sceneTree->addChildItem(surfaceItem1, newPoint);
    sceneTree->addChildItem(surfaceItem2, newPoint);
}

//--------------------------//
//  DEBUG
//--------------------------//

void ObjectManager::TEST_BSPLINE(){
    addObject(RB_POINT_TYPE, "test1");
    addObject(RB_POINT_TYPE, "test2");
    addObject(RB_POINT_TYPE, "test3");
    addObject(RB_POINT_TYPE, "test4");

    addObject(RB_BSPLINE_TYPE, "test5");

    const std::vector<Item*>& allItems = sceneTree->getAllItems();
    Item* spline;
    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if(item->type == RB_BSPLINE_TYPE)
            spline = item;
    }

    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if(item->type == RB_POINT_TYPE){
            item->object->moveTo((float)i/5, (float)i/5 , 0);
            addChildItem(spline, item);
        }

    }

}

void ObjectManager::TEST_PERFORMANCE(){
    int torusCount = 10;
    for(int i = 0; i < torusCount; i++){
        addObject(RB_TORUS_TYPE, "test" + std::to_string(i));
    }
}

void ObjectManager::TEST_BSPLINE_INTERP(){
    addObject(RB_POINT_TYPE, "test1");
    addObject(RB_POINT_TYPE, "test2");
    addObject(RB_POINT_TYPE, "test3");
    addObject(RB_POINT_TYPE, "test4");
    addObject(RB_POINT_TYPE, "test5");

    addObject(RB_BSPLINE_INTERPOLATING_TYPE, "test6");

    const std::vector<Item*>& allItems = sceneTree->getAllItems();
    Item* spline;
    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if(item->type == RB_BSPLINE_INTERPOLATING_TYPE)
            spline = item;
    }

    for(unsigned int i = 0; i < allItems.size(); i++){
        Item* item = allItems[i];
        if(item->type == RB_POINT_TYPE){
            item->object->moveTo((float)i/5, (float)i/5 , 0);
            addChildItem(spline, item);
        }

    }

}

void ObjectManager::TEST_SURFACE(){
    int n = 1;
    int m = 1;
    std::string name = "TEST_SURFACE";
    float width = 0.5;
    float height = 0.5;

    ObjectFactory& objectFactory = ObjectFactory::getInstance();
    SurfaceC0Rect * surface = objectFactory.createSurfaceC0Rect(name, n, m,
                                                                width, height);
    this->scene->addRenderObject(surface);
    Item* surfaceItem = sceneTree->addObject(surface, RB_SURFACE_C0_RECT_TYPE);

    const std::vector<ifc::Point*>& points = surface->getAllPoints();
    for(unsigned int i = 0; i < points.size(); i++){
        this->scene->addRenderObject(points[i]);
        Item* pointItem = this->sceneTree->addObject(points[i], RB_POINT_TYPE);
        sceneTree->addChildItem(surfaceItem, pointItem);
    }

}
