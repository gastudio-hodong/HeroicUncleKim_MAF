//
//  PopupDefault.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 05/06/2019.
//

#ifndef PopupDefault_h
#define PopupDefault_h

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoItems.h"

class PopupDefault : public PopupBase
{
public:
    static PopupDefault* create(std::string title, std::string text);

    
    PopupDefault(void);
    virtual ~PopupDefault(void);
    virtual bool init(std::string title, std::string text);
    virtual void onEnter() override;
    virtual void backkey() override;
    
public:
    // set, get
    void setContentSize(cocos2d::Size size);
    void setContentPosition(double x, double y);
    void setContentAlignment(cocos2d::TextHAlignment hAlignment, cocos2d::TextVAlignment vAlignment);
    
    // add button
    cocos2d::ui::Button* addButton(std::string strText, bool bDefault, const std::function<void(Ref*)>& callback, bool bHide = true);

protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiTitle();
    void uiText();
    void uiButtons();
    
    //
    void drawTitle();
    void drawText();
    void drawButtons();
    
private:
    bool _bInitUI;
    
    //
    std::string _title;
    std::string _text;
    std::function<void(Ref*)> _callbackBack;
    
    cocos2d::Size _sizeContents;
    cocos2d::TextHAlignment _hAlignment;
    cocos2d::TextVAlignment _vAlignment;
    
    cocos2d::Vector<cocos2d::ui::Button*> _listButton;
    
    // widget
    cocos2d::ui::Layout* _uiContentsTitle;
    cocos2d::ui::Layout* _uiContentsText;
    cocos2d::ui::Layout* _uiContentsButtons;
    
};

#endif /* PopupDefault_h */
