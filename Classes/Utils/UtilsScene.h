//
//  UtilsScene.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 10/10/2019.
//

#ifndef UtilsScene_h
#define UtilsScene_h

#include "cocos2d.h"

class InfoScene;
class UtilsScene
{
public:
    enum eType {
        scene_root = 0,
        scene_splash,
        scene_intro,
        scene_dungeon,
        scene_dungeon_normal,
        scene_dungeon_devil,
    };
    
public:
    static UtilsScene* getInstance();
    
    UtilsScene(void);
    virtual ~UtilsScene(void);
    virtual bool init(void);
    
public:
    cocos2d::Scene* getRunningScene();
    cocos2d::Layer* getRunningLayer();
    
    
    void scenePop();
    void scenePopRoot();
    void scenePush(UtilsScene::eType type, InfoScene* obj = nullptr);
    cocos2d::Scene* sceneReplace(UtilsScene::eType type, InfoScene* obj = nullptr);
    
private:
    
};

#endif /* UtilsScene_h */
