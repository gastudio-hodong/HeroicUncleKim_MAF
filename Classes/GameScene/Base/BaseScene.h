//
//  BaseScene.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 04/06/2019.
//

#ifndef BaseScene_h
#define BaseScene_h

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

class BaseScene : public cocos2d::Layer
{
public:
    BaseScene();
    virtual ~BaseScene();
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

public:

protected:
    

protected:
    
};

#endif /* BaseScene_h */
