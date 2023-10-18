//
//  UIBuff.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UIBuff_h
#define UIBuff_h

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

class UIBuff : public cocos2d::ui::Layout, public IRefresh
{
public:
    static UIBuff* create(std::vector<E_BUFF_TYPE> listBuff);
    
    UIBuff(void);
    virtual ~UIBuff(void);
    virtual bool init(std::vector<E_BUFF_TYPE> listBuff);

    // override : refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    

protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    
    // draw
    void drawContainer(float dt = 0);
    
    // set, get
    bool isAdsAvailable(E_BUFF_TYPE eType);
    bool isVisible(E_BUFF_TYPE eType);
    std::string getPath(E_BUFF_TYPE eType);
    
    // schedule
    
    // callback
    void callbackRank(bool Result);
    
    // click
    void onClickBuff(cocos2d::Ref* sender);
    
    // event
    void onExpand();
    
private:
    std::vector<E_BUFF_TYPE> _listBuff;
    
    // widget
    cocos2d::ui::Layout* _uiContainer;
};

#endif /* UIBuff_h */
