//
//  LayerEventRaceTopUI.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/12.
//

#ifndef LayerEventRaceTopUI_hpp
#define LayerEventRaceTopUI_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/EventRace/InfoEventRaceHorse.h"
#include "GameObject/EventRace/InfoEventRaceSimulateLog.h"

class LayerEventRaceTopUI : public cocos2d::LayerColor
{
public:
    static LayerEventRaceTopUI* create(const cocos2d::Size size);
    
    LayerEventRaceTopUI(void);
    virtual ~LayerEventRaceTopUI(void);
    virtual bool init(const cocos2d::Size size);
    
    virtual void update(float dt);
    
protected:
    // init
    void initVar();
    void initUi();
    
    void uiLobby();
    void uiMinimap();
    
    //onClick
    void onClickHelp(cocos2d::Ref* const sender);
    void onClickRankReward(cocos2d::Ref* const sender);
    
    //Event
    void onEventGameRaedy();
    void onEventGameStart();
private:
    cocos2d::ClippingNode* _clipper;
    cocos2d::Layer* _lyLobby;
    cocos2d::Layer* _lyMiniMap;
    cocos2d::Layer* _lyMiniMapLane;
    
    cocos2d::Vector<cocos2d::Sprite*> _listNumber;
};


#endif /* LayerEventRaceTopUI_hpp */
