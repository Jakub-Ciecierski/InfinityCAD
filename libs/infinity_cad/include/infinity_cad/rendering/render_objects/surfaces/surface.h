//
// Created by jakub on 5/2/16.
//

#ifndef IC_SURFACE_C0_H
#define IC_SURFACE_C0_H


#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/geometry/surfaces/bicubic_bezier_patch.h>

class Surface : public RenderObject{
private:

protected:
    int screenWidth = 1300;
    int screenHeight = 700;

    const int CUBIC_COUNT = 4;

    Matrix<BicubicBezierPatch*> patches;

    int MAX_PIXEL_COUNT = 100000;
    glm::vec4* surfacePixels;

    std::vector<ifc::Point*> allPoints;

    bool doDrawPolygon;

    int n;
    int m;

    virtual void build() = 0;

    virtual void initVertices() override;
    virtual void initEdges() override;

    std::string createPointName(int patchN, int patchM,
                                int pointI, int pointJ);

    void drawCPU(const glm::mat4& VP, const Color& color,
                          float u_min, float u_max,
                          float v_min, float v_max,
                          float du, float dv);

    void drawGPU(const glm::mat4& VP, const Color& color,
                          float u_min, float u_max,
                          float v_min, float v_max,
                          float du, float dv);

    void drawPatch(const BicubicBezierPatch* patch,
                            const glm::mat4& VP,
                            float u_min, float u_max,
                            float v_min, float v_max,
                            float du, float dv);

    virtual void drawPolygon(const glm::mat4& VP, int segments = 1);

    void draw(const glm::mat4& VP, const Color& color);

    float getMaximumPolygonLength();

public:
    const static int U_DIVISION_COUNT_DEFAULT = 4;
    const static int V_DIVISION_COUNT_DEFAULT = 4;

    static int uDivisionCount;
    static int vDivisionCount;

    /*
     * Creates a Surface made of nxm Bezier Patches connected with C0.
     */
    Surface(SceneID id, std::string name,
            int n, int m);

    ~Surface();

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;

    virtual void update() override;

    const std::vector<ifc::Point*>& getAllPoints();

    void setDrawPolygon(bool v);
    bool isDrawPolygon();
};


#endif //IC_SURFACE_C0_H
