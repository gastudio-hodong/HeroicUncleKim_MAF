//
//  InfoPetEquip.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoPetEquip_h
#define InfoPetEquip_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoPetEquip : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoPetEquip);
    
    InfoPetEquip();
    virtual ~InfoPetEquip();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int nIdx);
    
    E_PET getType();
    void setType(E_PET eType);
    
    int getSlot();
    void setSlot(int nSlot);
    
    int getLevel();
    void setLevel(int nLevel);
    
    int getAmount();
    void setAmount(int nAmount);
    
    double getProperty1();
    double getProperty2();
    void setProperty1(double nProperty);
    void setProperty2(double nProperty);
    
    double getIncrease();
    void setIncrease(double nIncrease);
    
    E_PET_EQUIP_TYPE getEffectType();
    void setEffectType(E_PET_EQUIP_TYPE eType);
    
    std::string getName();
    void setName(std::string strName);
    
    std::string getDesc();
    void setDesc(std::string strDesc);
    
    std::string getStory();
    void setStory(std::string strStory);
  
    
private:
    int _nIdx;
    E_PET _ePet;
    
    int _nSlot;
    int _nLevel;
    int _nAmount;
    
    double _nProperty_1;
    double _nProperty_2;
    double _nIncrease;
    E_PET_EQUIP_TYPE _eEffecttype;
    
    std::string _strName;
    std::string _strDesc;
    std::string _strStory;
    
};

class InfoPetEquipDraw : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoPetEquipDraw);

    InfoPetEquipDraw();
    virtual ~InfoPetEquipDraw();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int nIdx);
    
    int getCount();
    void setCount(int nCount);
    
private:
    int _nIdx;
    int _nCount;
};

class InfoPetEquipDrawGrade : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoPetEquipDrawGrade);

    InfoPetEquipDrawGrade();
    virtual ~InfoPetEquipDrawGrade();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int nIdx);
    
    int getGrade();
    void setGrade(int nGrade);
    
private:
    int _nIdx;
    int _nGrade;
};

class InfoPetEquipAmount : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoPetEquipAmount);

    InfoPetEquipAmount();
    virtual ~InfoPetEquipAmount();
    virtual bool init();
        
public:
    int getIdx();
    void setIdx(int nIdx);
    
    int getAmount();
    void setAmount(int nGrade);
    
private:
    int _nIdx;
    int _nAmount;
};

#endif /* InfoPetEquip_h */
