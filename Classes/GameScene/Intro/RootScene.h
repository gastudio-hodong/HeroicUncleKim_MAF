//
//  RootScene.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 31/05/2019.
//

#ifndef RootScene_h
#define RootScene_h

#include "GameScene/Base/BaseScene.h"

class RootScene : public BaseScene
{
public:
    CREATE_FUNC(RootScene);
    
    RootScene();
    virtual ~RootScene();
    virtual bool init() override;
    
    //
    void onInit();
    void onStart();
    void onRefresh();
    
protected:
    bool _bInitUI;
       
};

#endif /* RootScene_h */
