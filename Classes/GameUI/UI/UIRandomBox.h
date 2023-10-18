//
//  UIRandomBox.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UIRandomBox_h
#define UIRandomBox_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class UIRandomBox : public cocos2d::LayerColor
{
public:
    static UIRandomBox* create(cocos2d::Size size, E_PLACE ePlace);
    
    UIRandomBox(void);
    virtual ~UIRandomBox(void);
    virtual bool init(cocos2d::Size size, E_PLACE ePlace);
    
public:
    // set, get, add
    void addData(std::string str);
    
protected:
    // init
    void initVar();
    void initUi();
    void uiBox();
    
    // click
    void onClickBox(cocos2d::Ref* sender);
    
    // set, get, other
    void setTime();
    void setReward(bool bAdsView);
    
    // schedule
    void scheduleTime(float dt);
    
    // callback
    void callbackAds(ADS_RESULT result);
    
    //
    void onReward();

private:
    E_PLACE _ePlace;
    
    double _timeShow;
    double _timeNow;
    
    int _nFloorPrev;        // 이전 박스 출현 층수(시간 설정이 될때)
    bool _bResetTime;
    
    bool _bAdsReward;


    cocos2d::Vector<InfoItems*> _listItems;
    
    // widget
    cocos2d::Layer* _layerContainerContent;
    
    
};

#endif /* UIRandomBox_h */
