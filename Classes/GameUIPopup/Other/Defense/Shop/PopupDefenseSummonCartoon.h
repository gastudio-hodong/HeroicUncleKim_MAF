//
//  PopupSummonCartoon.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/23.
//

#ifndef PopupDefenseSummonCartoon_hpp
#define PopupDefenseSummonCartoon_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class PopupDefenseSummonCartoon : public PopupBase
{
public:
    static PopupDefenseSummonCartoon* create(bool isSpecial, bool isSumonOnGoing = false);
    
    PopupDefenseSummonCartoon();
    virtual ~PopupDefenseSummonCartoon();
    
    virtual bool init(bool isSpecial, bool isSumonOnGoing);
    
    // init
    void initVar();
    void initUI();
    
    void uiMain();
    void playCartoon();
private:
    bool _isFake;
    bool _isSpecial;
    bool _isSumonOnGoing;
    int _currentCut;
    cocos2d::Layer* _lyBG;
    cocos2d::Layer* _lyCartoon;
    
    Vector<Sprite*> _cutList;
    
};

#endif /* PopupSummonCartoon_hpp */
