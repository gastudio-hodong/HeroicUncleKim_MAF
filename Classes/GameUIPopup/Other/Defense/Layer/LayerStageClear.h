//
//  LayerStageClear.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/30.
//

#ifndef LayerStageClear_hpp
#define LayerStageClear_hpp


#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class LayerStageClear : public cocos2d::LayerColor
{
public:
    static LayerStageClear* create(const cocos2d::Size size);
    
    LayerStageClear(void);
    virtual ~LayerStageClear(void);
    virtual bool init(const cocos2d::Size size);
    
protected:
    // init
    void initVar();
    void initUi();
    
    
    void play(bool isSuccess);
    void uiMiddle(bool isSuccess);
    
    void onPlayClear();
    void onPlayFail();
private:
    bool _isPlay;
    LayerColor* _lyBG;
    LayerColor* _lyMiddle;
public:
    MafDelegate<void(void)> _onFinishedProduction;
};


#endif /* LayerStageClear_hpp */
