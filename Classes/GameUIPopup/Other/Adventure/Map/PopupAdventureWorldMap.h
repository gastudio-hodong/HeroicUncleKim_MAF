//
//  PopupAdventureWorldMap.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/25.
//

#ifndef PopupAdventureWorldMap_hpp
#define PopupAdventureWorldMap_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "Common/Observer/IRefresh.h"

class PopupAdventureWorldMap : public PopupBase, public IRefresh
{
public:
    static PopupAdventureWorldMap* create(const int nAdventureID, bool isFriend = false);
    
    PopupAdventureWorldMap();
    virtual ~PopupAdventureWorldMap(void);
    virtual bool init(const int nAdventureID, bool isFriend);
    virtual void show() override;
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData)  override;
    
    
    void setCallbackHide(const std::function<void(void)>& callback);
    //
protected:
    
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiMap();
    void uiTouchArea();
    void uiBottom();
    
    //click
    void onClickClose(Ref* const sender);
    void onClickArea(Ref* const sender);
    void onClickAreaBoss(Ref* const sender);
    
    //callbakc
    void callbackHide();
    void callbackRefresh();
    
private:
    bool _isFriend;
    int _nCurrentAdventureID;
    float _fIncrese;
    
    cocos2d::Layer* _lyMapUI;
    cocos2d::Layer* _lyTouchAreaUI;
    cocos2d::Layer* _lyBottomUI;
    std::function<void(void)> _callbackHide;
};

#endif /* PopupAdventureWorldMap_hpp */
