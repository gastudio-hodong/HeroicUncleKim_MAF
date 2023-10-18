//
//  UIAccordionLayer.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionLayer_hpp
#define UIAccordionLayer_hpp

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/InfoAccordionUI.h"

class UIAccordionLayerBase : public cocos2d::LayerColor
{
public:
    static UIAccordionLayerBase* create(const cocos2d::Vector<InfoAccordionUI*> listInfoAccodion, bool isActive = false);
    
    UIAccordionLayerBase(void);
    virtual ~UIAccordionLayerBase(void);
    virtual bool init(const cocos2d::Vector<InfoAccordionUI*> listInfoAccodion, bool isActive = false);
    
protected:
    // init
    virtual void initUi();
    virtual void initVar();

    
protected:
    virtual void uiButton();
    virtual void uiContainer();
    
    virtual void updateLayout() final;
    
    virtual void onClickAccordion(cocos2d::Ref* const sender) final;
    
public:
    virtual void setButtonText(const std::string strText);
    virtual void setButtonImage(const std::string strPath, const cocos2d::Size size = cocos2d::Size(), const cocos2d::Rect rect = cocos2d::Rect());
    
    virtual void setCallbackRefresh(const std::function<void(void)>& callback);
    
    void reloadData();
    void setList(const cocos2d::Vector<InfoAccordionUI*> listInfoAccodion);
    
protected:
    bool _isActive;
    std::string _strBtnText;
    std::string _strBtnPath;
    cocos2d::Size _btnSize;
    cocos2d::Rect _btnCapInsets;
    
    // widget
    cocos2d::Layer* _uiContainer;
    cocos2d::Layer* _uiButton;
    
    cocos2d::ui::Button* _btnAccordion;
    cocos2d::Label* _lbTitle;
    cocos2d::Vector<InfoAccordionUI*> _listInfoAccodion;
    
    std::function<void(void)> _callbackRefresh;
};

#endif /* UIAccordionLayer_hpp */
