//
//  UILayerDefenseRankUp.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/07.
//

#ifndef UILayerDefenseObjectRankUp_hpp
#define UILayerDefenseObjectRankUp_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "GameObject/InfoSetting.h"

class InfoDefenseCharacter;
class InfoDefenseWeapon;
class UILayerDefenseObjectRankUp : public cocos2d::LayerColor
{
public:
    static UILayerDefenseObjectRankUp* create(cocos2d::Size size, E_DF_Trench_SettingType type, int idx);
    
    UILayerDefenseObjectRankUp(void);
    virtual ~UILayerDefenseObjectRankUp(void);
    virtual bool init(cocos2d::Size size, E_DF_Trench_SettingType type, int idx);
public:
    // set, get
    
    void callbackRankup(bool bResult, int nResult);
    void onChangeObject(int idx);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiMain();
    
    void calculateMaxMakeCount();
    
    void onClickRankUp(cocos2d::Ref* sender);
    void onClickFluctuation(cocos2d::Ref* sender);
private:
    
    int _nMakeCount;
    int _nMaxMakeCount;
    
    E_DF_Trench_SettingType _type;
    int _idx;
    
    InfoSetting*  _objRankUpSetting;
    cocos2d::ui::CText* _lbNowCnt;
    cocos2d::ui::CText* _lbNextCnt;
    cocos2d::Sprite* _sprEffect;
    
    InfoDefenseCharacter* _characterData;
    InfoDefenseWeapon* _weaponData;
    
    cocos2d::Label* _lbRankUp;
public:
    MafDelegate<void(void)> _onHide;
};
#endif /* UILayerDefenseRankUp_hpp */
