//
// Created by jakub on 5/2/16.
//

#ifndef IC_SURFACE_C0_H
#define IC_SURFACE_C0_H


#include <infinity_cad/rendering/render_object.h>
#include <infinity_cad/geometry/surfaces/bicubic_bezier_patch.h>

enum SurfaceAxis{
    VERTICAL, HORIZONTAL
};

enum MatrixMajor{
    COLUMN, ROW
};

enum TrimmingStatuses{
    TRIM_NONE, TRIM_TOP, TRIM_BOTTOM
};

struct PointIndex{
    int patch_index;
    int column_index;
    int row_index;

    bool operator==(const PointIndex& rhs){
        return (patch_index == rhs.patch_index &&
                column_index == rhs.column_index&&
                row_index == rhs.row_index);
    }
};

class Surface : public RenderObject{
private:
    TrimmingStatuses trimmingStatus;

    std::vector<glm::vec2> trimmingParameters;
    void trimParameters(std::vector<glm::vec2>& params);
    glm::vec2 findTrimmingParameter(const glm::vec2& v);

protected:
    int screenWidth = 1300;
    int screenHeight = 700;

    const int CUBIC_COUNT = 4;

    Matrix<BicubicBezierPatch*> patches;

    int MAX_PIXEL_COUNT = 100000;
    glm::vec4* surfacePixels;

    std::vector<ifc::Point*> allPoints;
    Matrix<ifc::Point*> allPointsMatrix;

    bool doDrawPolygon;

    int n;
    int m;

    SurfaceAxis surfaceAxis;

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
    const static int U_DIVISION_COUNT_DEFAULT = 16;
    const static int V_DIVISION_COUNT_DEFAULT = 16;

    static int uDivisionCount;
    static int vDivisionCount;

    MatrixMajor matrixMajor;

    const glm::mat4* VPMatrix;

    /*
     * Creates a Surface made of nxm Bezier Patches connected with C0.
     */
    Surface(SceneID id, std::string name,
            int n, int m,
            SurfaceAxis surfaceAxis = SurfaceAxis::HORIZONTAL);

    ~Surface();

    virtual glm::vec3 compute(float u, float v);
    virtual glm::vec3 computeDu(float u, float v);
    virtual glm::vec3 computeDuu(float u, float v);
    virtual glm::vec3 computeDuv(float u, float v);

    virtual glm::vec3 computeDv(float u, float v);
    virtual glm::vec3 computeDvv(float u, float v);
    virtual glm::vec3 computeDvu(float u, float v);

    virtual void render(const glm::mat4 &VP) override;
    virtual void render(const glm::mat4 &VP, const Color &color) override;

    virtual void update() override;

    const std::vector<ifc::Point*>& getAllPoints();
    const Matrix<ifc::Point*>& getMatrixPoints();

    void setDrawPolygon(bool v);
    bool isDrawPolygon();

    int getRowPatchCount();
    int getColumnPatchCount();

    void setTrimming(TrimmingStatuses status);
    void setTrimming(TrimmingStatuses status, std::vector<glm::vec2>& params);

    PointIndex getIndex(ifc::Point* point);

    /*
     * Replaces src with dest. Returns false if src was not found,
     * true other wise.
     */
    virtual bool replacePoint(ifc::Point* src, ifc::Point* dest);
};

#endif //IC_SURFACE_C0_H

