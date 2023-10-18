//
//  DataManager.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 8. 6..
//
//

#ifndef DataManager_h
#define DataManager_h

#include "Libs/maf/MafSingleton.h"

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

#include "GameObject/InfoArtifact.h"
#include "GameObject/ModelInfoDevil.h"

class DataManager : public MafSingleton<DataManager>
{
public:
    DataManager();
    virtual ~DataManager(void);
    virtual bool init();
    void initData();

public:
    void loadArtifactData();

    cocos2d::Map<int, InfoArtifact*> _mapArtifact;
public:
    cocos2d::Vector<ModelInfoDevil::DevilArtifactInfo*> _vecDevilArtifact[5];
    

private:
    static void LoadDevilArtifactData(E_D_ARTIFACT_TYPE type);
public:
    static cocos2d::Vector<ModelInfoDevil::DevilArtifactInfo*> GetDevilArtifactDataType(E_D_ARTIFACT_TYPE type);

    static std::string GetMonsterHp(E_PLACE ePlace, int nFloor = 0);
    static std::string GetMonsterHpDevil();
    static std::string GetMonsterHpPrison(int kill);
    
    static std::string GetDungeonEarnKey(int kill);
    
    std::string GetPrestigeKey(E_PLACE ePlace, int nFloor);
    std::string GetPrestigeKeyNormal(E_PLACE ePlace, int nFloor);
    std::string GetPrestigeKeyDevil(E_PLACE ePlace, int nFloor);
    
    std::string GetPrestigeBonusKeyNormal(std::string value);
    std::string GetPrestigeGoldenKeyNormal(std::string value);
    
    // quest
    static std::string GetQuestName(E_PLACE ePlace, E_QUEST eQuest);
    static std::string GetQuestEarnMoney(E_PLACE ePlace, E_QUEST eQuest, int nLevel);
    static double GetQuestUpgradeMoney(E_PLACE ePlace, E_QUEST eQuest, int nLevel);
    static double GetQuestSpendTime(E_PLACE ePlace, E_QUEST eQuest);
    
    // equip
    static double GetEquipUpgradeMoneyDevil(E_EQUIP_IDX idx,int level);
    static std::string GetEquipReinforceKeyDevil(E_EQUIP_IDX idx,int reinforcelevel);
    static stEvolutionDevil GetEquipEvolutionSpendDevil(E_EQUIP_IDX idx,int reinforcelevel);
    static double GetEquipEffectRateDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level);
    static double GetEquipReinforceRateDevil(int reinforcelevel);
    static double GetEquipEvolutionRateDevil(E_EQUIP_IDX idx);
    static int GetEquipOptionTotalRateDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level,E_EQUIP_OPTION option);
    static int GetEquipOptionOneRateDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level,E_EQUIP_OPTION option);
    static std::string GetEquipOptionExplainDevil(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level,E_EQUIP_OPTION option);

    // artifact
    static InfoArtifact* GetArtifactInfo(E_ARTIFACT arti);
    static InfoArtifact* GetArtifactInfo(E_ARTIFACT_TYPE type, int gen);
    static int GetArtifactMaxLevel(E_ARTIFACT arti);
    static int GetArtifactGeneration(E_ARTIFACT arti);
    static int GetArtifactHighGeneration(E_ARTIFACT_TYPE type);
    static E_ARTIFACT_TYPE GetArtifactType(E_ARTIFACT arti);
    static bool IsArtifactTypeNormal(E_ARTIFACT arti);  // 힘의반지, 폭퐁의칼날, 독수리의상, 황금수정
    static bool IsArtifactTypeCross(E_ARTIFACT arti);   // 십자가
    static bool IsArtifactTypeSpecial(E_ARTIFACT arti); // 쾌속의 반지 등등
    static double GetArtifactUpgradeKey(E_PLACE ePlace, E_ARTIFACT eArti, int nLevel);
    static double GetArtifactUpgradeKey(E_PLACE ePlace, E_ARTIFACT eArti, int nLevel, int nFloorHigh, int nFloorTotal);
    static void initArtifactSaveData(E_PLACE place);
    static double GetArtifactEffect(E_ARTIFACT arti, int level, int reinforce);
    std::string GetArtifacEffecttCalculation(std::string strValue, double nPer);
    
    static double GetArtifactReinforceEffect(E_ARTIFACT arti,int level);
    static int GetArtifactReinforceExpandPrice(E_ARTIFACT arti);
    static int GetArtifactReinforcePrice(E_ARTIFACT arti);
    
    
    static std::string GetRelicName(E_RELIC relic);
    static std::string GetRelicExplain(E_RELIC relic);
    static int GetRelicUpgradeGem(E_RELIC relic,int level);
    static double GetRelicEffect(E_RELIC relic,int level);
    static int GetRelicMaxLevel(E_RELIC relic);
    static int GetRelicOpenFloor(E_RELIC relic);
    
    static std::string GetBonggiokUpgradeKey(long long level);
    static double GetBonggiokEffect(int nLevel);
    static int GetBonggiokMaxLevel();
    
    static std::string GetShopAdsName(E_SHOP_ADS ads);
    static std::string GetShopName(E_SHOP shop);
    static std::string GetShopExpain(E_SHOP shop);
    static int GetShopPrice(E_SHOP ads);
    
     
    
    static std::string GetCostumeIconPath(E_COSTUME type, E_COSTUME_IDX cidx);
    static int GetCostumeLegendMaterial(E_COSTUME type);
    static int GetCostumeLegendTermsCount(E_COSTUME type);
    static double GetCostumeLegendEffect(E_COSTUME type, int level);
    static double GetCostumeLegendValue(E_COSTUME type);
    static double GetCostumeLegendIncrement(E_COSTUME type);
    static double GetCostumeLegendSetEffect(int count);
    static int GetCostumeLegendMaxLevel();
    
    static std::string GetCostumeLegendName(E_COSTUME eType, E_COSTUME_IDX eIdx);
    static std::string GetCostumeLegendDesc(E_COSTUME eType, E_COSTUME_IDX eIdx);
    
    //
    static cocos2d::Color3B GetAuraColor(E_COSTUME_IDX idx);
    
    static double GetMinePickaxEffectData(int level);
    static double GetMinePickaxUpgradeGem(int level);
    static int GetMineMineralCount(E_MINERAL mineral);
    
    static std::string GetMineMineralPath(E_MINERAL mineral);
    static std::string GetMineMineralName(E_MINERAL mineral);
    static double GetMineSpendTime(E_MINERAL mineral);
    
    static int GetHonorPrice(E_HONOR honor, int level);
    static int GetHonorEffect(E_HONOR honor, int level);
    static int GetHonorMaxLevel(E_HONOR honor);
    
    // 지하감옥 젤리
    static std::string GetJellyIcon(E_JELLY type);
    static std::string GetJellyName(E_JELLY type);
    static std::string GetJellyExplain(E_JELLY type);
    static std::string GetJellyOptionKey(E_JELLY type);
    static int GetJellyUpgrade(E_JELLY type, int level);
    static int GetJellyEffect(E_JELLY type, int level);
    static int GetJellyMaxLevel(E_JELLY type);
    
    // 이벤트
    static bool isEventChristmas();
    static bool isEvent5th();
    static bool isNewYear();
    static bool isEventSummer();
    static bool isEventThanksgiving();
    static bool isEventHalloween();
    static bool isEventFoolsDay();
    
    static E_NEWBIE_STEP GetNewbieStep(int highFloor, int totalFloor, int specialMission);
    
    
    // network
    void requestPushInfo(std::string strKey);
    void requestPushInfo(std::vector<std::string> listPush);
    void responsePushInfo(cocos2d::network::HttpResponse* response,std::string &data);
    
    void requestPushTopic(std::string strKey, bool bSubscribe);
};


#endif /* DataManager_h */
