//
//  CText.cpp
//
//

#include "Common/Cocos2d/CText.h"

NS_CC_BEGIN

namespace ui {

CText::CText() :
_eType(E_TYPE::SYSTEM),
_bChangeColor(false)
{

}

CText::~CText()
{
    
}

CText* CText::create(const std::string &textContent, std::string fontName, float fontSize)
{
    CText* widget = new (std::nothrow) CText();
    if (widget && widget->initWithTTF(textContent, fontName, fontSize))
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

CText* CText::createWithBMFont(const std::string& textContent, std::string fontName, float fontSize)
{
    CText* widget = new (std::nothrow) CText();
    if (widget && widget->initWithBMFont(textContent, fontName, fontSize))
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool CText::initWithTTF(const std::string &textContent, std::string fontName, float fontSize)
{
    if (Text::init() == false)
    {
        return false;
    }
    
    //
    setName("CText");
    
    //
    this->setFontName(fontName);
    this->setFontSize(fontSize);
    this->setString(textContent);
 
    return true;
}

bool CText::initWithBMFont(const std::string &textContent, std::string fontName, float fontSize)
{
    if (Text::init() == false)
    {
        return false;
    }
    
    //
    setName("CText");
    
    //
    this->setFontName(fontName);
    this->setFontSize(0);
    this->setString(textContent);
 
    this->setScale(fontSize / 28);
    
    return true;
}

void CText::setFontName(const std::string& textName)
{
    E_TYPE eTypeOld = _eType;
    E_TYPE eType = E_TYPE::SYSTEM;
    if ( textName.rfind(".fnt") != std::string::npos )
    {
        eType = E_TYPE::BMFONT;
    }
 
    if ( eType == E_TYPE::SYSTEM || eType == E_TYPE::TTF )
    {// ttf
        if ( FileUtils::getInstance()->isFileExist(textName) )
        {
            TTFConfig config = _labelRenderer->getTTFConfig();
            config.fontFilePath = textName;
            config.fontSize = _fontSize;
            _labelRenderer->setTTFConfig(config);

            _eType = E_TYPE::TTF;
        }
        else
        {
            _labelRenderer->setSystemFontName(textName);
            
            _eType = E_TYPE::SYSTEM;
        }
        
        //
        if ( eTypeOld != _eType )
        {
            _labelRenderer->requestSystemFontRefresh();
        }
    }
    else
    {// bm font
        _labelRenderer->setBMFontFilePath(textName);
        
        _eType = E_TYPE::BMFONT;
    }
    
    //
    _fontName = textName;
    
    //
    updateContentSizeWithTextureSize(_labelRenderer->getContentSize());
    _labelRendererAdaptDirty = true;
}

void CText::setFontSize(float size)
{
    if ( _eType == E_TYPE::SYSTEM )
    {
        _labelRenderer->setSystemFontSize(size);
    }
    else if ( _eType == E_TYPE::TTF )
    {
        TTFConfig config = _labelRenderer->getTTFConfig();
        config.fontSize = size;
        _labelRenderer->setTTFConfig(config);
    }
    else
    {// bm font
        size = 0;
    }
    
    //
    _fontSize = size;
    
    //
    updateContentSizeWithTextureSize(_labelRenderer->getContentSize());
    _labelRendererAdaptDirty = true;
}

void CText::setString(const std::string& text)
{
    if ( _eType == E_TYPE::TTF )
    {
        std::string textTemp = getStringColor(text);
        if ( textTemp == _labelRenderer->getString() )
        {
            return;
        }
        
        setStringColor(textTemp);

//        _listColorPosStart.clear();
//        _listColorPosEnd.clear();
//        _listColorHex.clear();
    }
    else
    {
        if ( text == _labelRenderer->getString() )
        {
            return;
        }
        
        _labelRenderer->setString(text);
    }
    
    //
    updateContentSizeWithTextureSize(_labelRenderer->getContentSize());
    _labelRendererAdaptDirty = true;
}

std::string CText::getStringColor(std::string text)
{
    _listColorPosStart.clear();
    _listColorPosEnd.clear();
    _listColorHex.clear();
    
    std::u16string text16;
    StringUtils::UTF8ToUTF16(text, text16);
    
    std::string::size_type posStart = 0;
    std::string::size_type posMiddle = 0;
    std::string::size_type posEnd = 0;
    
    std::u16string diffStart, diffMiddle, diffEnd;
    StringUtils::UTF8ToUTF16("{#", diffStart);
    StringUtils::UTF8ToUTF16(":", diffMiddle);
    StringUtils::UTF8ToUTF16("}", diffEnd);
    
    while ( true )
    {
        posStart = text16.find(diffStart, posEnd);
        posMiddle = text16.find(diffMiddle, posStart);
        posEnd = text16.find(diffEnd, posStart);
        
        if ( posStart == -1 || posMiddle == -1 || posEnd == -1 || posStart > posMiddle || posStart > posEnd )
        {
            break;
        }
        
        std::string::size_type nHexLength = posMiddle - (posStart + 2);
        std::u16string strHex16 = text16.substr(posStart + 2, nHexLength);
        std::string strHex = "";
        StringUtils::UTF16ToUTF8(strHex16, strHex);
        
        
        text16.erase(posMiddle, 1);
        text16.erase(posStart, nHexLength + 2);
        posEnd = text16.find(diffEnd, posStart);
        text16.erase(posEnd,  1);
        
        std::string strText = "";
        StringUtils::UTF16ToUTF8(text16, strText);
        text = strText;
 
        _listColorPosStart.push_back(Value((int32_t)posStart));
        _listColorPosEnd.push_back(Value((int32_t)posEnd));
        _listColorHex.push_back(Value(strHex));
        
        _bChangeColor = true;
    }
    
    return text;
}

void CText::setStringColor(std::string text)
{
    _labelRenderer->setString(text);
    
    if ( _bChangeColor == true )
    {
        for (int32_t i = 0; i < getStringLength(); i++)
        {
            auto sprite = (Sprite*)getLetter(i);
            if ( sprite != nullptr )
            {
                sprite->setColor(getColor());
            }
        }
    }

    for (int32_t i = 0; i < _listColorPosStart.size(); i++ )
    {
        std::string strHex = _listColorHex[i].asString();
        if ( strcmp(strHex.c_str(), "red") == 0 )
            strHex = "FF0000";//빨강
        else if ( strcmp(strHex.c_str(), "blue") == 0 )
            strHex = "0000FF";//파랑
        else if ( strcmp(strHex.c_str(), "black") == 0 )
            strHex = "000000";//검정
        else if ( strcmp(strHex.c_str(), "white") == 0 )
            strHex = "FFFFFF";//화이트
        else if ( strcmp(strHex.c_str(), "green") == 0 )
            strHex = "00FF00";//초록
        else if ( strcmp(strHex.c_str(), "yellow") == 0 )
            strHex = "FFCB00";//노랑
        else if ( strcmp(strHex.c_str(), "gray") == 0 )
            strHex = "B3B0AD";//회색
        else if ( strcmp(strHex.c_str(), "lightbrown") == 0 )
            strHex = "854F04";//밝은 갈색
        else if ( strcmp(strHex.c_str(), "lightgreen") == 0 )
            strHex = "0F913A";//밝은 초록
        else if ( strcmp(strHex.c_str(), "lightblue") == 0 )
            strHex = "006AEC";//밝은 파랑
        
        int64_t rgb[3] = {0 , 0 , 0};
        rgb[0] = strtoll(strHex.substr(0,2).c_str(), nullptr, 16);
        rgb[1] = strtoll(strHex.substr(2,2).c_str(), nullptr, 16);
        rgb[2] = strtoll(strHex.substr(4,2).c_str(), nullptr, 16);

        for(int32_t j = _listColorPosStart[i].asInt(); j < _listColorPosEnd[i].asInt(); j++)
        {
            auto sprite = (Sprite*)getLetter(j);
            if ( sprite != nullptr )
                sprite->setColor(Color3B(rgb[0], rgb[1], rgb[2]));
        }
    }
}

void CText::enableBold()
{
    if (_eType == E_TYPE::TTF)
        _labelRenderer->enableBold();
}

void CText::enableWrap(bool enable)
{
    if (_eType == E_TYPE::TTF)
        _labelRenderer->enableWrap(enable);
}

void CText::enableStrikethrough()
{
    if (_eType == E_TYPE::TTF)
        _labelRenderer->enableStrikethrough();
}

void CText::disableEffect()
{
    if (_eType == E_TYPE::TTF)
        _labelRenderer->disableEffect();
}
void CText::disableEffect(LabelEffect effect)
{
    if (_eType == E_TYPE::TTF)
        _labelRenderer->disableEffect(effect);
}

void CText::setTextOverflow(Label::Overflow overflow)
{
    _labelRenderer->setOverflow(Label::Overflow::NONE);
    _labelRenderer->setOverflow(overflow);
    
    updateContentSizeWithTextureSize(_labelRenderer->getContentSize());
    _labelRendererAdaptDirty = true;
}
 
void CText::labelScaleChangedWithSize()
{
    if ( _ignoreSize )
    {
        _labelRenderer->setScale(1.0f);
        _normalScaleValueX = _normalScaleValueY = 1.0f;
    }
    else
    {
        Size textureSize = Size::ZERO;
        if ( _eType == E_TYPE::BMFONT )
        {
            textureSize = _labelRenderer->getContentSize();
        }
        else
        {
            _labelRenderer->setDimensions(_contentSize.width,_contentSize.height);
            textureSize = _labelRenderer->getContentSize();
        }
        
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _labelRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _contentSize.width / textureSize.width;
        float scaleY = _contentSize.height / textureSize.height;
        _labelRenderer->setScaleX(scaleX);
        _labelRenderer->setScaleY(scaleY);
        _normalScaleValueX = scaleX;
        _normalScaleValueY = scaleY;
    }
    _labelRenderer->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f);
}

void CText::setLineSpacing(float height)
{
    _labelRenderer->setLineSpacing(height);
}

void CText::setTextColor(const Color4B color)
{
    setColor(Color3B(color));
    
    if ( _bChangeColor == true )
    {
        for (int32_t i = 0; i < getStringLength(); i++)
        {
            auto sprite = (Sprite*)getLetter(i);
            if ( sprite != nullptr )
            {
                sprite->setColor(getColor());
            }
        }
    }

    for (int32_t i = 0; i < _listColorPosStart.size(); i++ )
    {
        std::string strHex = _listColorHex[i].asString();
        if ( strcmp(strHex.c_str(), "red") == 0 )
            strHex = "FF0000";//빨강
        else if ( strcmp(strHex.c_str(), "blue") == 0 )
            strHex = "0000FF";//파랑
        else if ( strcmp(strHex.c_str(), "black") == 0 )
            strHex = "000000";//검정
        else if ( strcmp(strHex.c_str(), "white") == 0 )
            strHex = "FFFFFF";//화이트
        else if ( strcmp(strHex.c_str(), "green") == 0 )
            strHex = "00FF00";//초록
        else if ( strcmp(strHex.c_str(), "yellow") == 0 )
            strHex = "FFCB00";//노랑
        else if ( strcmp(strHex.c_str(), "gray") == 0 )
            strHex = "B3B0AD";//회색
        else if ( strcmp(strHex.c_str(), "lightbrown") == 0 )
            strHex = "854F04";//밝은 갈색
        else if ( strcmp(strHex.c_str(), "lightgreen") == 0 )
            strHex = "0F913A";//밝은 초록
        else if ( strcmp(strHex.c_str(), "lightblue") == 0 )
            strHex = "006AEC";//밝은 파랑
        
        int64_t rgb[3] = {0 , 0 , 0};
        rgb[0] = strtoll(strHex.substr(0,2).c_str(), nullptr, 16);
        rgb[1] = strtoll(strHex.substr(2,2).c_str(), nullptr, 16);
        rgb[2] = strtoll(strHex.substr(4,2).c_str(), nullptr, 16);

        for(int32_t j = _listColorPosStart[i].asInt(); j < _listColorPosEnd[i].asInt(); j++)
        {
            auto sprite = (Sprite*)getLetter(j);
            if ( sprite != nullptr )
                sprite->setColor(Color3B(rgb[0], rgb[1], rgb[2]));
        }
    }
}


}

NS_CC_END
