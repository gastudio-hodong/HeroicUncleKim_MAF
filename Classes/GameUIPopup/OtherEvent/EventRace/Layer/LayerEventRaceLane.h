//
//  LayerEventRaceLane.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/22.
//

#ifndef LayerEventRaceLane_hpp
#define LayerEventRaceLane_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/EventRace/InfoEventRaceHorse.h"
#include "GameObject/EventRace/InfoEventRaceSimulateLog.h"

class LayerEventRaceLane : public cocos2d::LayerColor
{
public:
    static LayerEventRaceLane* create(const cocos2d::Size size, const int nIdx);
    
    LayerEventRaceLane(void);
    virtual ~LayerEventRaceLane(void);
    virtual bool init(const cocos2d::Size size, const int nIdx);
    
    virtual void race(float dt);
    virtual void update(float dt);
    
    const bool isRaceComplete();
protected:
    // init
    void initVar();
    void initUi();
    
    void uiLane();
    void uiHorse();
    
    void uiLobby();
    
    //
    void viewRankMark();
    void viewRunningDust();
    //
    void onEventClickStatus(const int nIdx);
    void onEventGameRaedy();
    void onEventGameStart();
    void onEventAds(int);
    
private:
    bool _isGameStart;
    bool _isCurrentSelect;
    bool _isCompleteRankMarkAction;
    bool _isRunningRankMarkAction;
    bool _isCurrentBoost;
    float _currentStamina;
    int _nIdx;
    float _nTick;
    
    float _nTimer;
    
    float _curSpeed;
    InfoEventRaceHorse* _infoHorse;
    cocos2d::ClippingNode* _clipper;
    cocos2d::Layer* _lyLane;
    cocos2d::Layer* _lyHorse;
    cocos2d::Layer* _lyLobby;
    
    cocos2d::Sprite* _sprRankMark;
    cocos2d::Sprite* _sprSelect;
    cocos2d::Sprite* _sprCommentBubble;
    cocos2d::Sprite* _sprDoubleEffect;
    cocos2d::Sprite* _sprHorse;
    cocos2d::Sprite* _sprRunningDust;
    cocos2d::Sprite* _sprNumber;
    
    cocos2d::ui::Scale9Sprite* _sprBetBox;
    cocos2d::ui::CText* _lbBetCount;
};


#endif /* LayerEventRaceLane_hpp */
