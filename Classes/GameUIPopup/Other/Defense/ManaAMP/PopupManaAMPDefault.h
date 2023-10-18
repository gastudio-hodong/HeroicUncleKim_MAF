//
//  PopupManaAMPDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#ifndef PopupManaAMPDefault_hpp
#define PopupManaAMPDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoDefenseManaAMP;
class PopupManaAMPDefault : public PopupBase
{
public:
    static PopupManaAMPDefault* create(int number);
    
    PopupManaAMPDefault();
    virtual ~PopupManaAMPDefault(void);
    virtual bool init(int number);
    virtual void onEnter() override;
    
    void hide() override;
protected:
    void initVar();
    void initUI();
    
    void uiBG();
    void uiTop();
    void uiAMPInfo();
    void uiMain();
    void uiBottom();
    
    void updateUI();
    void updateAMPData();
    
    //Click
    void onClickClose(Ref* sender);
    void onClickHelp(Ref* sender);
    void onClickEquipStone(Ref* sender);
    void onClickReset(Ref* sender);
    void onClickAnotherAMP(Ref* sender);
    
    void playEquipEffect(const bool isSuccess, const E_DF_AMP_STONE_Type type);
    //callback
    void onServerEquipStone(const bool bResult, const int nResult, const bool isSuccess, const E_DF_AMP_STONE_Type type);
    void onServerUpdate(const bool bResult, const int nResult);
private:
    int _number;
    InfoDefenseManaAMP* _AMPData;
    
    Sprite* _currentStone;
    cocos2d::Layer* _lyContainerParent;
    cocos2d::Layer* _lyBG;
    cocos2d::Layer* _lyTop;
    cocos2d::Layer* _lyAMPInfo;
    cocos2d::Layer* _lyMain;
    cocos2d::Layer* _lyBottom;
    
public:
    MafDelegate<void(void)> _onReddotUpdate;
};


#endif /* PopupManaAMPDefault_hpp */
