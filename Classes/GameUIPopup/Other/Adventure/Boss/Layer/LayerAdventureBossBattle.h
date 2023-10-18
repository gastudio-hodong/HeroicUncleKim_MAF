//
//  LayerAdventureBossBattle.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerAdventureBossBattle_h
#define LayerAdventureBossBattle_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoAdventureBoss;
class LayerAdventureBossBattle : public cocos2d::ui::Layout
{
public:
    static LayerAdventureBossBattle* create(cocos2d::Size size);

    LayerAdventureBossBattle(void);
    virtual ~LayerAdventureBossBattle(void);
    virtual bool init(cocos2d::Size size);
    virtual void onEnter() override;
    
public:
    void setCallbackAttackFinish(const std::function<void(void)>& callback);
    
    void setBulletItemIdx(int itemIdx);
    void setBulletItemCount(int itemCount);
    int getBulletPosition();
    
    void onBulletChange();
    void onAttack();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiLobby();
    
    // ui : draw
    void drawLobby();
    
    // ui : action
    void animationCannon();
    void animationBulletChange();
    
    // ui : click
    
    // callback
    
    // schedule
    
    //

private:
    bool _bInitEnter;
     
    //
    InfoAdventureBoss* _objInfoBoss;
    int _bulletItemIdx;
    int _bulletItemCount;
     
    std::function<void(void)> _callbackAttackFinish;
    
    // widget
    cocos2d::ui::Layout* _uiContentsLobby;
     
    
};
#endif /* LayerAdventureBossBattle_h */
