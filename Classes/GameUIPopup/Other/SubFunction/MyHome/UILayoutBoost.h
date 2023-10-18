//
//  UILayoutBoost.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UILayoutBoost_h
#define UILayoutBoost_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class UILayoutBoost : public cocos2d::ui::Layout
{
public:
    static UILayoutBoost* create(cocos2d::Size size);
    
    UILayoutBoost(void);
    virtual ~UILayoutBoost(void);
    virtual bool init(cocos2d::Size size);
    
public:
    // game
    void refresh();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    
    // draw
    void drawContents();
    void drawPorgress();
    void drawLock();

    // set, get
    int getAdsWatchMax(int floor);
    
private:
    int _nMyhomeCount;
    
    // widget
    cocos2d::ui::Layout* _layerContainer;
};

#endif /* UILayoutBoost_h */
