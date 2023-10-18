//
//  UIDialogueText.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/14.
//

#ifndef UIDialogueText_hpp
#define UIDialogueText_hpp

#include "Common/ConfigDefault.h"

class UIDialogueText : public cocos2d::LayerColor
{
public:
    static UIDialogueText* create(cocos2d::Size size, cocos2d::Vec2 anchor = cocos2d::Vec2::ANCHOR_MIDDLE);
    
    UIDialogueText(void);
    virtual ~UIDialogueText(void);
    virtual bool init(cocos2d::Size size, cocos2d::Vec2 anchor = cocos2d::Vec2::ANCHOR_MIDDLE);
    
public:
    const bool isEnd() const;
    const bool isLineEnd() const;
    void setTextSpeed(const float value);
    void addText(const std::string line);
    void setListText(const std::vector<std::string> list);
    void setTextAnchorPoint(cocos2d::Vec2 anchor);
    void setTextAlignment(cocos2d::TextVAlignment v, cocos2d::TextHAlignment h);
    void setTextPosition(cocos2d::Vec2 pos);
    void setTextDefaultColor(const cocos2d::Color3B color);
    void setCallbackEndEvent(const std::function<void(void)>& callback);
    
    void startDialogue();
    void onEventNextLine();
    
protected:
    void onEventClick();
    void nextLine();
    void dialogueLine();
    
protected:
    int _currentIdx;
    float _speed;
    cocos2d::ui::Text* _text;
    std::vector<std::string> _listLine;
    
    std::function<void(void)> _callbackEndEvent;
};

#endif /* UIDialogueText_hpp */
