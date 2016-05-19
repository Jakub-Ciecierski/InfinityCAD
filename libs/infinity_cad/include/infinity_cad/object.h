//
// Created by jakub on 4/3/16.
//

#ifndef MG1_OBJECT_H
#define MG1_OBJECT_H

#include <string>
#include <infinity_cad/rendering/scene/scene_id.h>
#include "infinity_cad/object_types.h"

namespace ifc {

    class  Object {
    private:
        std::string name;
        SceneID id;

    protected:
        ObjectType objectType;

    public:

        Object(SceneID id);
        Object(SceneID id, std::string name);

        virtual ~Object();

        void setName(std::string name);

        std::string getName();
        const SceneID &getID();

        const ObjectType& getType();

    };
}

#endif //MG1_OBJECT_H
