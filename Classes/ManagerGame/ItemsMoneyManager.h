//
//  ItemsMoneyManager.hpp
//  FantasyClicker
//
//  Created by SanHeo on 2017. 10. 20..
//
//

#ifndef ItemsMoneyManager_hpp
#define ItemsMoneyManager_hpp

#include "Common/ConfigDefault.h"

class ItemsMoneyManager
{
public:
    static ItemsMoneyManager *getInstance();
    static void destroyInstance();
    
    ItemsMoneyManager();
    virtual ~ItemsMoneyManager();
    
private:
    int _nEventRaceSkinTicket;
    std::string _uGold;
    std::string _uKey;
    
    std::string _uGem;
    std::string _uPoint;
    std::string _uCube;
    std::string _uCostumeCoupon;
    std::string _uMonsterSoulPiece;
    std::string _uMythril;
    
    std::string _uCostumeCoin;
    std::string _uMaterial;
    std::string _uHelltrium;
    std::string _uHonorPoint;
    
    std::string _uDevilGold;
    std::string _uDevilKey;
    
    std::string _uFriendship;       // 우전 포인트
    
    std::string _uEventTicket;       // 이벤트 티켓
    
    // 골드, 열쇠, 보석 획득 최대 길이(품위 유지용 셉터 코스튬)
    std::string _uGoldLength;
    std::string _uKeyLength;
    std::string _uGemLength;
    
public:
    void resetNormal();
    void resetDevil();
    
    void initGold();
    void setGoldZero(bool bSave = true);
    void setGold(std::string plus, bool bSave = true, bool bRefresh = true);
    std::string getGold();
    int getGoldLength();
    
    void initKey();
    void setKeyZero();
    void setKey(double plus,bool bSave = true);
    void setKey(std::string plus,bool bSave = true);
    std::string getKey();
    int getKeyLength();
    
    void initGem();
    void setGem(int plus,bool bSave = true);
    int getGem();
    int getGemLength();
    
    void initPoint();
    void setPoint(int plus,bool bSave = true);
    int getPoint();
    
    void initCube();
    void setCube(int plus, bool bSave = true);
    int getCube();
    
    void initCostumeCoupon();
    void setCostumeCoupon(int nCount);
    int getCostumeCoupon();
    
    void initMonsterSoulPiece();
    void setMonsterSoulPiece(int nCount);
    int getMonsterSoulPiece();
    
    void initMythril();
    void setMythril(int plus, bool bSave = true);
    int getMythril();
    
#pragma mark-
    void setDevilGoldZero(bool bSave = true);
    void setDevilGold(std::string plus, bool bSave = true, bool bRefresh = true);
    std::string getDevilGold();
    
    void setDevilKeyZero();
    void setDevilKey(double plus,bool bSave = true);
    void setDevilKey(std::string plus,bool bSave = true);
    std::string getDevilKey();
};


#endif /* ItemsMoneyManager_hpp */
