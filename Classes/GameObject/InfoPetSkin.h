//
//  InfoPetSkin.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoPetSkin_h
#define InfoPetSkin_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoPetSkin : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoPetSkin);
    
    InfoPetSkin();
    virtual ~InfoPetSkin();
    virtual bool init();
    
public:
    E_PET getPetType();
    void setPetType(E_PET ePet);
    
    int getIdx();
    void setIdx(int nIdx);
    
    std::string getName();
    void setNameKey(std::string strNameKey);
    
    
    int getItemIdx();
    void setItemIdx(int nIdx);
    
    int getItemTypeParams();
    void setItemTypeParams(int nTypeParams);
    
    std::string getItemCount();
    void setItemCount(int nCount);
    void setItemCount(std::string strCount);
    
    // event sale
    bool isEventSale();
    int64_t getEventSaleTimeStart();
    void setEventSaleTimeStart(int64_t nTime);
    
    int64_t getnEventSaleTimeEnd();
    void setEventSaleTimeEnd(int64_t nTime);
    
private:
    E_PET _ePet;
    int _nIdx;
    std::string _strNameKey;
    
    int _nItemIdx;
    int _nItemTypeParams;
    std::string _strItemCount;
    
    // event sale
    int64_t _nEventSaleTimeStart;
    int64_t _nEventSaleTimeEnd;
};

#endif /* InfoPetSkin_h */
