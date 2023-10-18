//
//  TutorialManager.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 18/11/2019.
//

#ifndef TutorialManager_h
#define TutorialManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

enum EFFECT
{
    EFFECT_NONE = 0,
    EFFECT_FINGER,
    EFFECT_FLITTER,
    EFFECT_FLITTER_BIG,
};
enum PROGRESS_MAIN
{
    PROGRESS_NONE = 0,
    PROGRESS_QUEST,
    PROGRESS_WEAPON,
    PROGRESS_ARTIFACT,
    PROGRESS_PET,
    PROGRESS_ATTEND,
    PROGRESS_SHOP,
    PROGRESS_COSTUME,
    PROGRESS_PRISON,
    PROGRESS_FLOOR,
    PROGRESS_RAID,
    PROGRESS_END
};

enum PROGRESS_SUB
{
    SUB_NONE,
    SUB_SHORTCUT_BUTTON = 1,
    SUB_REWARD_BUTTON,
    SUB_OBJECT_1,
    SUB_OBJECT_2,
    SUB_OBJECT_3,
    SUB_OBJECT_4
};
enum E_STEPWISE_OPEN_TYPE
{
    ST_SPECIAL_MISSION,
    ST_FLOOR,
    ST_ETC,
    ST_NONE
};

class TutorialManager : public MafSingleton<TutorialManager>
{
public:
    TutorialManager(void);
    virtual ~TutorialManager(void);
    virtual bool init();
    void initTutorialOpenData();
    
public:
    void startTutorial();
    void endTutorial();
    
    void attachEffect(EFFECT eff, std::pair<PROGRESS_MAIN, PROGRESS_SUB> progress, cocos2d::Node* button, float time = 0);
    void removeAllEffect(std::pair<PROGRESS_MAIN, PROGRESS_SUB> progress);
    
    void refreshTutorialProgress();
    void clickShortcutButton();
    std::pair<PROGRESS_MAIN, PROGRESS_SUB> getTutorialProgress();
    
    void setPosition(cocos2d::Vec2 position);

    void saveTutorialProgress();
    void loadTutorialProgress();
    
    void drawTutorial(bool refresh = false);
    
    void setFirstReviver(bool active);
    bool getFirstReviver();
    
    void setOpenData(E_STEPWISE_OPEN_TYPE type, int value, bool active);
    void setOpenData(std::string key, bool active);
    
    bool getOpenData(E_STEPWISE_OPEN_TYPE type, int value);
    bool getOpenData(std::string key);
    
private:
    cocos2d::Node* _tutorialParent;
    std::pair<PROGRESS_MAIN,PROGRESS_SUB>  _nowTutorialProgress;
    cocos2d::Sprite*    _tutorialObject;
    cocos2d::Layer*     _nowLayer;
    E_PLACE    _ePlace;
    bool _firstReviver = false;
    std::map<std::string, bool> _mapTutorialOpenData;
    
};
#endif /* TutorialManager_h */
