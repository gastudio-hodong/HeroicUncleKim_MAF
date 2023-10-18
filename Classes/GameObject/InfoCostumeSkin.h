//
//  InfoCostumeSkin.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoCostumeSkin_h
#define InfoCostumeSkin_h

#include "Common/ConfigDefault.h"

class InfoCostumeSkin : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoCostumeSkin);
    
    InfoCostumeSkin();
    virtual ~InfoCostumeSkin();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int value);
    
    int getSort();
    void setSort(int value);
    
    std::string getName();
    void setNameKey(std::string value);
    
    std::string getIconPath();

    std::string getBuffType();
    void setBuffType(std::string value);
    
    double getBuffStrength();
    void setBuffStrength(double value);
    
private:
    int _idx;
    int _sort;
    
    std::string _nameKey;
    
    std::string _buffType;
    double _buffStrength;
};

class InfoCostumeSkinShop : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoCostumeSkinShop);
    
    InfoCostumeSkinShop();
    virtual ~InfoCostumeSkinShop();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int value);
    
    int getSort();
    void setSort(int value);
    
    std::string getComment();
    void setCommentKey(std::string value);
    
    int getSkinIdx();
    void setSkinIdx(int value);
    
    int getLabel();
    void setLabel(int value);
    
    int getRare();
    void setRare(int value);

    //
    std::string getBuyType();
    void setBuyType(std::string value);
    
    int getBuyCashIdx();
    void setBuyCashIdx(int value);
    
    int getBuyItemIdx();
    void setBuyItemIdx(int value);
    
    int getBuyItemParams();
    void setBuyItemParams(int value);
    
    int getBuyItemCount();
    void setBuyItemCount(int value);
    
    bool isBuySaleTime();
    
    int64_t getBuySaleTimeStart();
    void setBuySaleTimeStart(int64_t time);
    
    int64_t getBuySaleTimeEnd();
    void setBuySaleTimeEnd(int64_t time);
    
private:
    int _idx;
    int _sort;
    
    std::string _commentKey;
    
    int _skinIdx;
    
    int _label;
    int _rare;
    
    std::string _buyType;
    int _buyCashIdx;
    int _buyItemIdx;
    int _buyItemParams;
    int _buyItemCount;
    
    int64_t _nEventSaleTimeStart;
    int64_t _nEventSaleTimeEnd;
};

#endif /* InfoCostumeSkin_h */
