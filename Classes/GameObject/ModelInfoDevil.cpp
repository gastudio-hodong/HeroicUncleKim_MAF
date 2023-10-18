//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "ModelInfoDevil.h"

USING_NS_CC;
using namespace ModelInfoDevil;

#pragma mark-
DevilArtifactInfo::DevilArtifactInfo(){
    
    _idx =  1;
    _title = "";
//    _text  = "";
//    _price = "";
    _price = 0;
//    _type = 0;
    _effect = 0;
    _type = E_D_ARTIFACT_TYPE::E_D_ARTI_TYPE_NONE;
}

DevilArtifactInfo::~DevilArtifactInfo(){
}

#pragma mark-
static DevilEquipInfo *s_Instance = nullptr;
DevilEquipInfo* DevilEquipInfo::getInstance(){
    if(s_Instance==nullptr){
        s_Instance = new DevilEquipInfo();
    }
    return s_Instance;
}

void DevilEquipInfo::destroyInstance()
{
    if ( s_Instance != nullptr )
    {
        delete s_Instance;
        s_Instance = nullptr;
    }
}

DevilEquipInfo::DevilEquipInfo(){
    
    _type = E_EQUIP_TYPE::E_D_NONE;
    _idx =  E_EQUIP_IDX::E_D_IDX_1;
    _title = "";
//    _text  = "";
//    _price = 0;
}

DevilEquipInfo::~DevilEquipInfo(){
}

DevilEquipInfo* DevilEquipInfo::getData(E_EQUIP_TYPE type,E_EQUIP_IDX idx){
    if(_type == type && _idx == idx){
        return this;
    }
    std::string txt = FileUtils::getInstance()->getStringFromFile(DATA_FILE_D2_EQUIP);
//    txt = MafAes256::Decrypt(txt);
    
    DevilEquipInfo *info = this;
    rapidjson::Document jsondoc;
    jsondoc.ParseInsitu<0>((char*)(const char*)txt.c_str()).HasParseError();
    
    auto iter = jsondoc.Begin();
    for(; iter != jsondoc.End(); iter++){
        const rapidjson::Value& oneItem = *iter;
        auto ntype = oneItem["_type"].GetInt();
        auto nidx = oneItem["_idx"].GetInt();
        if(ntype == type && nidx == idx){
            info->_type =  (E_EQUIP_TYPE)ntype;
            info->_idx =  (E_EQUIP_IDX)nidx;
            
            
            std::string strKeyTitle = MafUtils::format("t_equip_d2_title_%d_%d", info->_type, info->_idx);
            info->_title = GAME_TEXT(strKeyTitle);

            break;
        }
    }
    
    return info;
}

//std::string DevilEquipInfo::getUpgradeMoney(E_EQUIP_TYPE type,E_EQUIP_IDX idx,int level, int reinforce){
//    return  "100";
//}

#pragma mark-

