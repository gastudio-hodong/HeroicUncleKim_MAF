//
//  LayerDefenseMain.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#ifndef LayerDefenseMain_hpp
#define LayerDefenseMain_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class DFBattleManager;
class LayerStageClear;
class LayerDefenseMain : public cocos2d::LayerColor
{
public:
    static LayerDefenseMain* create(const cocos2d::Size size);
    
    LayerDefenseMain(void);
    virtual ~LayerDefenseMain(void);
    virtual bool init(const cocos2d::Size size);
    
protected:
    // init
    void initVar();
    void initUi();
    
    //ui
    void uiTop();
    void uiBG();
    void uiFloor();
    void uiMonsterCount();
    void uiManaAMP();
    
    void onClickHelp(Ref* sender);
    void onClickManaAMP(Ref* sender);
    void onClickClose(Ref* sender);
    void onClickBackground(Ref* sender);
    void onClickChat(Ref* sender);
    
    //Event
    void onUpdateFloor();
    void onManaAMPUpdate(bool bResult, int nResult);
private:
    bool _isManaAMPOn;
    DFBattleManager* _battleManager;
    LayerColor* _lyTop;
    Sprite* _sprBg;
    Sprite* _sprFloor;
    ui::Text* _textFloor;
    Sprite* _sprHighFloor;
    ui::Text* _textHighFloor;
    LayerStageClear* _stageClearProduction;
    
    Sprite* _sprMonsterCount;
    Label* _textMonsterCount;
    Sprite* _manaAMP;
    
public:
    MafDelegate<void(void)> _onClose;
};

#endif /* LayerDefenseMain_hpp */
