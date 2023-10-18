//
//  LayerEventRaceMainGame.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#ifndef LayerEventRaceMainGame_hpp
#define LayerEventRaceMainGame_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/EventRace/InfoEventRaceHorse.h"

#include "GameUIPopup/OtherEvent/EventRace/Layer/LayerEventRaceLane.h"

class LayerEventRaceMainGame : public cocos2d::LayerColor
{
public:
    static LayerEventRaceMainGame* create(const cocos2d::Size size);
    
    LayerEventRaceMainGame(void);
    virtual ~LayerEventRaceMainGame(void);
    virtual bool init(const cocos2d::Size size);
    
    virtual void update(float dt) override;
protected:
    // init
    void initVar();
    void initUi();
    
    void uiLane();
    void onClickStatus(cocos2d::Ref* const sender);
    
    void onActionFinish();
    
    //
    void onEventClickStatus(const int nIdx);
    void onEventClickReady();
    void onEventClickStart();
    void onEventClickReset();
private:
    int _nCurrentStatusIdx;
    cocos2d::Layer* _lyLane;
    cocos2d::Vector<LayerEventRaceLane*> _listLane;
};

#endif /* LayerEventRaceMainGame_hpp */
