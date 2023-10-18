//
//  SplashScene.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 31/05/2019.
//

#ifndef SplashScene_h
#define SplashScene_h

#include "GameScene/Base/BaseScene.h"

class SplashScene : public BaseScene
{
public:
    CREATE_FUNC(SplashScene);
    
    SplashScene();
    virtual ~SplashScene();
    
    // override base
    virtual void onEnter() override;
    
protected:
    // ui : init
    void uiInit();
    
    // ui : draw
    void uiDraw();
    
    // ui : click
    
    // schedule
    
    // game
    
private:
    std::vector<std::string> _listSplash;
};

#endif /* SplashScene_h */
