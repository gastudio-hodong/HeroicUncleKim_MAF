//
//  PopupAdsMafGames.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/04/01.
//

#ifndef PopupAdsMafGames_h
#define PopupAdsMafGames_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoMoreGame.h"

class PopupAdsMafGames : public PopupBase
{
public:
    static PopupAdsMafGames* create();
    
    PopupAdsMafGames();
    virtual ~PopupAdsMafGames();
    
    virtual bool init() override;
    void scheduleTimer(float dt);
    void videoEventCallback(Ref* p, cocos2d::experimental::ui::VideoPlayer::EventType evt);
    
    // init
    void initVar();
    void initUI();
    void initSound();
    
    //utils
    void loadMoreDataManager();
    virtual void hide() override;
    void setAdsType();
    void onClick(Ref* sender);
    void onClickClose();
    
    void setCallback(const std::function<void(ADS_RESULT)>& pCallback);
    void setLogParams(std::string logParams);
    void setLogValue(std::string logValue);
    
    InfoMoreGame* getMoreGameFromIdx(int nIdx);
    
private:
    cocos2d::experimental::ui::VideoPlayer* _video;
    
    MafNode::MafMenuItemSprite* _itemClose;
    cocos2d::Sprite* _sprClose;
    cocos2d::Label* _lbTimer;
    float _timer;
    float _timerMax;
    
    float _volumeBGM;
    float _volumeEffect;
    
    std::function<void(ADS_RESULT)> _callbackAds;
    
    E_MAF_ADS_TYPE _eType;
    std::string _strLogParams;
    std::string _strLogValue;
    cocos2d::Vector<InfoMoreGame*> _listMoreGame;
};

#endif /* PopupAdsMafGames_hpp */
