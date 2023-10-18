//
//  InfoProduct.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoProduct_h
#define InfoProduct_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoItems;
class InfoProduct : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoProduct);
    
    InfoProduct();
    virtual ~InfoProduct();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int idx);
    
    int getOrder();
    void setOrder(int value);
    
    std::string getName();
    void setName(std::string name);
    
    std::string getIconName();
    void setIconName(std::string name);
    
    int getShopGroup();
    void setShopGroup(int group);
    
    E_PRODUCT getShopType();
    void setShopType(E_PRODUCT eType);
    void setShopType(std::string strType);
    
    //
    int getSaleType();
    void setSaleType(int time);
    
    int64_t getSaleTimeStart();
    void setSaleTimeStart(int64_t time);
    
    int64_t getSaleTimeEnd();
    void setSaleTimeEnd(int64_t time);
    
    int getSaleLimit();
    void setSaleLimit(int limit);
    
    std::string getSaleRatio();
    void setSaleRatio(std::string ratio);
    
    //
    std::string getPrice();
    std::string getPriceOriginal();
    void setPrice(std::string strPrice);
    
    std::string getPriceCurrencyCode();
    void setPriceCurrencyCode(std::string strPriceCurrencyCode);
    
    std::string getProductIdReal();
    void setProductIdReal(std::string strProductId);
    
    std::string getProductIdWatch();
    void setProductIdWatch(std::string strProductIdWatch);
    
    //
    std::string getVersion();
    void setVersion(std::string strVersion);
    
    //
    cocos2d::Vector<InfoItems*> getItems();
    void setItems(std::string strJson);
    
    cocos2d::Vector<InfoItems*> getItemsOther();
    void setItemsOther(std::string strJson);
    
private:
    int _nIdx;
    int _nOrder;
    std::string _strNameKey;
    std::string _strIconName;
    
    int _nShopGroup;
    E_PRODUCT _eShopType;
    
    //
    int _nSaleType;
    int64_t _nSaleTimeStart;
    int64_t _nSaleTimeEnd;
    int _nSaleLimit;
    std::string _strSaleRatio;
    
    //
    std::string _strPrice;
    std::string _strPriceOriginal;
    std::string _strPriceCurrencyCode;
    
    std::string _strProudctIdReal;
    std::string _strProductIdWatch;
    
    // 허용버전
    std::string _strVersion;
    
    // 보상 리스트
    cocos2d::Vector<InfoItems*> _listItems;
    cocos2d::Vector<InfoItems*> _listItemsOther;
    
};

#endif /* InfoProduct_h */
