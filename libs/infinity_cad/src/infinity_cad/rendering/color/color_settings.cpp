//
// Created by jakub on 3/5/16.
//

#include <infinity_cad/rendering/color/color_settings.h>

// Default color value for the scene
Color COLOR_SCENE_DEFAULT(0.2, 0.2, 0.2, 1.0);
//Color COLOR_SCENE_DEFAULT(0.0, 0.0, 0.0, 1.0);

// Default color value of an object.
Color COLOR_OBJECT_DEFAULT(0.0, 0.8, 0.0, 1.0);
Color COLOR_LINE_OBJECT_DEFAULT(0.5, 1.0, 0.0, 1.0);
Color COLOR_OBJECT_ACTIVE(1.0, 1.0, 0.0,
                          COLOR_OBJECT_DEFAULT.Alpha);

Color COLOR_STEREOSCOPIC_LEFT_PROJ_DEFAULT(0.6, 0.0, 0.0, 1.0);
Color COLOR_STEREOSCOPIC_RIGHT_PROJ_DEFAULT(0.0, 0.0, 0.8, 1.0);

Color COLOR_X_AXIS(1.0, 0.0, 0.0, 1.0);
Color COLOR_Y_AXIS(0.0, 1.0, 0.0, 1.0);
Color COLOR_Z_AXIS(0.0, 0.0, 1.0, 1.0);

Color COLOR_PLANE(0.8, 0.8, 0.8, 1.0);

Color COLOR_GRABED(COLOR_OBJECT_DEFAULT.R,
                   COLOR_OBJECT_DEFAULT.G + 0.5,
                   COLOR_OBJECT_DEFAULT.B,
                   COLOR_OBJECT_DEFAULT.Alpha);


Color COLOR_BLACK(0.0, 0.0, 0.0, 1.0);

Color COLOR_SELECTION_BOX_BORDER(0.2, 0.65, 1.0, 1.0);
Color COLOR_SELECTION_BOX_FILL(0.2, 0.65, 1.0, 0.1);