//
//  PopupLoadingSpiritRankUp.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/07.
//

#ifndef PopupLoadingSpiritRankUp_hpp
#define PopupLoadingSpiritRankUp_hpp

#include "Common/ConfigDefault.h"

class PopupLoadingSequentialUpgrade : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(PopupLoadingSequentialUpgrade);
    
    PopupLoadingSequentialUpgrade(void);
    virtual ~PopupLoadingSequentialUpgrade(void);
    virtual bool init(void);
    
public:
    static void show();
    static void hide(bool bAll = false);
    static bool isShow();
    static void setCurrentIdx(int value);
    static void setTextKey(std::string value);
    static void setClosingRemark(std::string value);
protected:
    // ui
    void uiLoading();
    void uiReload();
    
    // set, get
    void setAddTouch();
    void setDelTouch();
    
private:
    cocos2d::Label* _labelText;
    static int _nCountVisible;
    static int _nCurrentIdx;
    static std::string _strTextKey;
    static std::string _strClosingRemark;
};

#endif /* PopupLoadingSpiritRankUp_hpp */
