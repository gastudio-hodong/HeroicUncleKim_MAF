//
//  InfoAccordionUI.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef InfoAccordionUI_hpp
#define InfoAccordionUI_hpp

#include "Common/ConfigDefault.h"

class InfoAccordionUI : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoAccordionUI);
    InfoAccordionUI();
    virtual ~InfoAccordionUI();
    virtual bool init();
    
public:
    const bool isActive() const;
    void setIsActive(const bool isActive);
    
    const int getIdx() const;
    void setIdx(const int value);
    
    const std::string getBoxPath() const;
    void setBoxPath(const std::string value);
    
    const std::string getTitleText() const;
    ///Text key 입력
    void setTitleText(const std::string value);
    
    const std::string getText(const std::string key, const bool isTextKey = false) const;
    ///Text key 입력
    void addTextKey(const std::string key, const std::string value);
    void addText(const std::string key, const std::string value);
    
    const double getValueDouble(const std::string key) const;
    void addValueDouble(const std::string key, const double value);
protected:
    bool _isActivate;
    int _nIdx;
    std::string _strBoxPath;
    std::string _strTitle;
    
    std::map<std::string, std::string> _listText;
    std::map<std::string, double> _listValueDouble;
};

#endif /* InfoAccordionUI_hpp */
