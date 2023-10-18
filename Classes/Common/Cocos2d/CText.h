//
//  CText.h
//
//

#ifndef CText_h
#define CText_h

#include "cocos2d.h"
#include "ui/UIText.h"

NS_CC_BEGIN

namespace ui{

class CText : public cocos2d::ui::Text
{
public:
    enum class E_TYPE
    {
        SYSTEM,
        TTF,
        BMFONT,
    };
    
public:
    CText();
    virtual ~CText();
    
    /**
     * @brief create
     */
    static CText* create(const std::string& textContent, std::string fontName, float fontSize);
    static CText* createWithBMFont(const std::string& textContent, std::string fontName, float fontSize);
    
    /**
     * @brief init
     */
    virtual bool initWithTTF(const std::string& textContent, std::string fontName, float fontSize);
    virtual bool initWithBMFont(const std::string& textContent, std::string fontName, float fontSize);
    
    
public:
    void setFontName(const std::string& textName);
    void setFontSize(float size);
    
    void setString(const std::string& text);
    
    void enableBold();
    void enableWrap(bool enable);
    void enableStrikethrough();
    void disableEffect();
    void disableEffect(LabelEffect effect);
    void setTextOverflow(Label::Overflow overflow);
    
    void labelScaleChangedWithSize();

    void setLineSpacing(float height);
    
    void setTextColor(const Color4B color);
    
protected:
    std::string getStringColor(std::string text);
    void setStringColor(std::string text);
    
    
private:
    E_TYPE _eType;
    bool _bChangeColor;
    
    ValueVector _listColorPosStart;
    ValueVector _listColorPosEnd;
    ValueVector _listColorHex;
};

}

NS_CC_END

#endif /* CText_h */
