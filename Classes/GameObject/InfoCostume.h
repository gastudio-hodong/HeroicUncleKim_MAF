//
//  InfoCostume.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoCostume_h
#define InfoCostume_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoCostume : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoCostume);
    
    InfoCostume();
    virtual ~InfoCostume();
    virtual bool init();
    
public:
    E_COSTUME_IDX getIdx();
    void setIdx(int nIdx);
    
    int getSort();
    void setSort(int nSort);
    
    E_COSTUME getType();
    void setType(E_COSTUME eType);
    void setType(std::string strType);
    
    bool isTypeNormal();
    void setTypeNormal(int nData);
    
    bool isTypePrison();
    void setTypePrison(int nData);
    
    std::string getTitle();
    void setTitle(std::string strTitle);
    
    std::string getDesc();
    void setDesc(std::string strDesc);
    
    int getPrice();
    int getPriceRent();
    void setPrice(int nPrice);
    
    bool isDips();
    void setDips(bool bDips);
    
private:
    int _nIdx;
    int _nSort;
    
    E_COSTUME _eType;
    int _nTypeNormal;
    int _nTypePrison;
    
    std::string _strTitle;
    std::string _strDesc;
    
    int _nPrice;
};

#endif /* InfoCostume_h */
