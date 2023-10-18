//
//  PopupLuckyBagDrawEffect.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/01/03.
//

#ifndef PopupLuckyBagDrawEffect_hpp
#define PopupLuckyBagDrawEffect_hpp

#include "GameUIPopup/Base/PopupBase.h" 

class PopupLuckyBagDrawEffect : public PopupBase
{
public:
    enum class E_TYPE
    {
        TRY = 1,
        FAIL,
        ENDING
    };
public:
    static PopupLuckyBagDrawEffect* create(E_TYPE type, std::string strReward = "");
    
    PopupLuckyBagDrawEffect();
    virtual ~PopupLuckyBagDrawEffect(void);
    virtual bool init(E_TYPE type, std::string strReward);
    virtual void onEnter() override;
protected:
    void initVar();
    void initUI();
    
    //ui
    void uiTry();
    void uiFail();
    void uiEnding();
    
private:
    E_TYPE _eType;
    bool _bInitUI;
    cocos2d::ui::Layout* _uiMain;
    std::string _strReward;
};

#endif /* PopupLuckyBagDrawEffect_hpp */
