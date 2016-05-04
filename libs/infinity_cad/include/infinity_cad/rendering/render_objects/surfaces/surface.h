//
// Created by jakub on 5/2/16.
//

#ifndef IC_SURFACE_C0_H
#define IC_SURFACE_C0_H


#include <infinity_cad/rendering/render_object.h>

class Surface : public RenderObject{
private:

protected:

    int n;
    int m;

    virtual void draw(const glm::mat4& VP, const Color& color) = 0;
    virtual void build() = 0;

    virtual void initVertices() override;
    virtual void initEdges() override;

public:

    /*
     * Creates a Surface made of nxm Bezier Patches connected with C0.
     */
    Surface(SceneID id, std::string name,
            int n, int m);

    ~Surface();

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;
};


#endif //IC_SURFACE_C0_H
