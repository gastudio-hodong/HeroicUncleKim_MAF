//
//  InfoTimingPackage.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/29.
//

#ifndef InfoTimingPackage_hpp
#define InfoTimingPackage_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoTimingPackage : public cocos2d::Ref
{
    
public:
    static InfoTimingPackage* create();
    
    InfoTimingPackage();
    virtual ~InfoTimingPackage();
    virtual bool init();
    
    
public:
    int getIdx();
    void setIdx(int value);
    
    E_TIMING_PACKAGE_TYPE getType();
    void setType(E_TIMING_PACKAGE_TYPE value);
    
    int getCashIdx();
    void setCashIdx(int value);
    
    
    E_TIMING_PACKAGE_CONDITION_TYPE getStartConditionType();
    void setStartConditionType(E_TIMING_PACKAGE_CONDITION_TYPE value);
    
    int getStartConditionTypeParams();
    void setStartConditionTypeParams(int value);
    
    int getStartValue();
    void setStartValue(int value);
    
    int getStartRange();
    void setStartRange(int value);
    
    E_TIMING_PACKAGE_CONDITION_TYPE getEndConditionType();
    void setEndConditionType(E_TIMING_PACKAGE_CONDITION_TYPE value);
    
    int getEndConditionTypeParams();
    void setEndConditionTypeParams(int value);
    
    int getCompleteValue();
    void setCompleteValue(int value);

    
    std::string getName();
    void setNameKey(std::string value);
    
    std::string getDescKey();
    void setDescKey(std::string value);
    
    std::string getIconPath();
    void setIconPath(std::string value);
    
    std::string getBannerPath();
    void setBannerPath(std::string value);
    
    cocos2d::Vector<InfoItems*> getListItems();
    void setListItems(cocos2d::Vector<InfoItems*> list);
    
    bool isPurchase();
    void setIsPurchase(bool value);
    
    bool isActivate();
    void setIsActivate(bool value);
    
    bool isBtnActive();
    void setIsBtnActive(bool value);
    
    bool isOpenPopup();
    void setIsOpenPopup(bool value);
    
    float getEndTimer();
    void setEndTimer(float value);
    
private:
    
    int _nIdx;
    E_TIMING_PACKAGE_TYPE _eType;
    int _nCashIdx;
    
    E_TIMING_PACKAGE_CONDITION_TYPE _eStartConditionType;
    int _eStartConditionTypeParams;
    int _nStartValue;
    int _nStartRange;
    
    E_TIMING_PACKAGE_CONDITION_TYPE _eEndConditionType;
    int _eEndConditionTypeParams;
    int _nCompleteValue;
    
    std::string _strNameKey;
    std::string _strDescKey;
    std::string _strIconPath;
    std::string _strBannerPath;
    cocos2d::Vector<InfoItems*> _listItems;
    
    float _nEndTimer;
    bool _isPurchase;
    bool _isOpenPopup;
    bool _isActivate;
    bool _isBtnActive;
    
    
};

#endif /* InfoTimingPackage_hpp */
