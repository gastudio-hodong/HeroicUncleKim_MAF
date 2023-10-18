//
//  ModelInfoDevil.h
//  Tens
//
//  Created by SanHeo on 2014. 10. 11..
//
//

#ifndef __FantasyClicker__ModelInfoDevil__
#define __FantasyClicker__ModelInfoDevil__

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/ConfigGameDevil.h"

namespace ModelInfoDevil {
    
    class DevilArtifactInfo : public cocos2d::Ref
    {
    public:
        DevilArtifactInfo();
        virtual ~DevilArtifactInfo();
        
        E_D_ARTIFACT_TYPE _type;
        int _idx;
        std::string _title;
        std::string _text;
//        std::string _price;
        double _price;
//        int _type;
        double _effect;
    };
    
    class DevilEquipInfo : public cocos2d::Ref
    {
    public:
        static DevilEquipInfo *getInstance();
        static void destroyInstance();
        
        DevilEquipInfo();
        virtual ~DevilEquipInfo();
        
        E_EQUIP_TYPE _type;
        E_EQUIP_IDX _idx;
//        int _price;
    public:
        DevilEquipInfo* getData(E_EQUIP_TYPE type,E_EQUIP_IDX idx);
//        std::string getUpgradeMoney(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level, int reinforce);
        
        std::string _title;
//        std::string _text;        
    };
    
}

#endif /* defined(__Tens__Model__) */
