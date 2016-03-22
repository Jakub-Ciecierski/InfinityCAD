//
// Created by jakub on 3/5/16.
//

#ifndef MG1_SCENE_ID_H
#define MG1_SCENE_ID_H


class SceneID {
private:
     int key;

public:
    SceneID();
    SceneID(int key);

    int getKey() const;
};


#endif //MG1_SCENE_ID_H
