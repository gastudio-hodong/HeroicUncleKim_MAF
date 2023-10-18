//
//  PopupAttend.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 06/11/2019.
//

#ifndef PopupAttend_h
#define PopupAttend_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoAttend;
class InfoItems;
class PopupAttend : public PopupBase
{
public:
    static PopupAttend* create();
    
    PopupAttend();
    virtual ~PopupAttend();
    virtual bool init();
    virtual void onEnter();

protected:
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiContents();
    void uiDetail();
    void uiBonus();
    void uiVipBonus();
    
    //
    void drawTop();
    void drawList();
    void drawDetail();
    
    // set, get, other
    void setLoad(int vip);
    void setResetDaliy();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBonus(cocos2d::Ref* sender);
    void onClickVip10Bonus(cocos2d::Ref* sender);
    
    // network
    void requestAttendData();
    void responseAttendData(cocos2d::network::HttpResponse* response,std::string &data);
    
    // callback
    void callbackBonus(ADS_RESULT result);
    void callbackVIPBonus(bool bResult, int nResult, int nSkin);
    
private:
    cocos2d::Vector<InfoAttend*> _listAttendAll;
    cocos2d::Vector<InfoAttend*> _listAttend;
    cocos2d::Vector<InfoItems*> _listReward;
    
    int _result;
    int _totalDay;
    int _day;
    int _vip;
    
    // widget
    cocos2d::Layer* _layerContainerTop;
    cocos2d::Layer* _layerContainerContents;
    cocos2d::Layer* _layerContainerDetail;
    cocos2d::Layer* _layerContainerBonus;
    
    cocos2d::Label* _labelVip;
    cocos2d::Label* _labelDetail;

};
#endif /* PopupAttend_h */
