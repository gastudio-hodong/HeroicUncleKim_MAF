//
//  PopupRaidRankRewardInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/12.
//

#include "PopupRaidRankRewardInfo.h"

#include "GameUIPopup/Other/NewRaid/Layer/UILayerRankInfo.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/RankingManager.h"

USING_NS_CC;

PopupRaidRankRewardInfo* PopupRaidRankRewardInfo::create()
{
    PopupRaidRankRewardInfo* pRet = new(std::nothrow) PopupRaidRankRewardInfo();
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

PopupRaidRankRewardInfo::PopupRaidRankRewardInfo(void):
_lyTopRankUI(nullptr),
_lyTierUI(nullptr),
_lbTimer(nullptr),
_rankInfo(nullptr),

_timer(0)
{
    _vecInfo.clear();
}

PopupRaidRankRewardInfo::~PopupRaidRankRewardInfo(void)
{
    for(auto info : _vecInfo)
        delete info;
    
    _vecInfo.clear();
}

#pragma mark -init

bool PopupRaidRankRewardInfo::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    creatTierInfo();
    initVar();
    initUI();
    
    return true;
}

void PopupRaidRankRewardInfo::creatTierInfo()
{
    _vecInfo.clear();

    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::RAID_RANK_REWARD);

    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            if(jsonValue.HasMember("version"))
            {
                if ( MafNative::IsLessThanCurrVersion(jsonValue["version"].GetString()) == false )
                {
                    continue;
                }
            }
            auto seasonidx = jsonValue["seasonidx"].GetInt();
            auto idx = jsonValue["idx"].GetInt();
            auto rank_type = jsonValue["rank_type"].GetInt();
            auto condition = jsonValue["condition1"].GetInt();
            auto condition2 = jsonValue["condition2"].GetInt();
            auto reward = jsonValue["reward"].GetString();
            
            auto listReward = ItemsManager::getInstance()->getConvertArray(reward);
            

            if(seasonidx == 0 || seasonidx == NewRaidManager::getInstance()->getSeason())
            {

                auto info = new InfoRaidTierInfo();
                info->setIdx(idx);
                info->setSeasonIdx(seasonidx);
                info->setType((PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE) rank_type);
                info->setCondition1(condition);
                info->setCondition2(condition2);
                info->setListReward(listReward);
                
                if(info->getSeasonIdx() == 0)
                {
                    _vecSeason0Info.push_back(info);
                }
                else if(info->getSeasonIdx() == NewRaidManager::getInstance()->getSeason())
                {
                    _vecInfo.push_back(info);
                }
                else
                {
                    delete info;
                }
            }
        }
    }

    if(_vecInfo.size() == 0)
    {
        _vecInfo = _vecSeason0Info;
    }

    
}

void PopupRaidRankRewardInfo::initVar()
{
    Size size = Size(734, 1210);
    _spriteContainer->setContentSize( size );
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_title"), GAME_FONT, 30);
    
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 45) );
    _spriteContainer->addChild(labelTitle);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupRaidRankRewardInfo::onClick, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    _lyTopRankUI = LayerColor::create();
    _lyTopRankUI->setIgnoreAnchorPointForPosition(false);
    _lyTopRankUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopRankUI->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 340));
    _lyTopRankUI->setContentSize(Size(_spriteContainer->getContentSize().width, (_spriteContainer->getContentSize().height - 340) /2 - 25));
    _spriteContainer->addChild(_lyTopRankUI);
    
    _lyTierUI = LayerColor::create();
    _lyTierUI->setIgnoreAnchorPointForPosition(false);
    _lyTierUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTierUI->setPosition(_lyTopRankUI->getPosition() + Vec2::DOWN * (_lyTopRankUI->getContentSize().height + 10));
    _lyTierUI->setContentSize(Size(_spriteContainer->getContentSize().width, (_spriteContainer->getContentSize().height - 340) - _lyTopRankUI->getContentSize().height - 15));
    _spriteContainer->addChild(_lyTierUI);
    
}

void PopupRaidRankRewardInfo::initUI()
{
    initTopUI();
    initBottomUI();
}

void PopupRaidRankRewardInfo::initTopUI()
{
    auto lbDesc = Label::createWithTTF(GAME_TEXT("t_ui_raidreward_info"), GAME_FONT, 25);
    lbDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbDesc->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    lbDesc->setDimensions(_spriteContainer->getContentSize().width - 40, 180);
    lbDesc->setOverflow(Label::Overflow::SHRINK);
    lbDesc->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 95);
    _spriteContainer->addChild(lbDesc);
    
    _lbTimer = Label::createWithTTF("", GAME_FONT, 23);
    _lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lbTimer->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    _lbTimer->setDimensions(_spriteContainer->getContentSize().width - 40, _lbTimer->getContentSize().height);
    _lbTimer->setOverflow(Label::Overflow::SHRINK);
    _lbTimer->setPosition(_spriteContainer->getContentSize().width/2, lbDesc->getPositionY() - lbDesc->getContentSize().height - 10);
    _lbTimer->setColor(Color3B(128,128,128));
    _spriteContainer->addChild(_lbTimer);
    
    
}

void PopupRaidRankRewardInfo::initBottomUI()
{
    initTopRankUI();
    initTierUI();
}

#pragma mark -UI
void PopupRaidRankRewardInfo::initTopRankUI()
{
    auto sprTop = Sprite::create("Assets/ui/ranking/ranking_base_text_1_1.png");
    sprTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTop->setPosition(_lyTopRankUI->getContentSize().width/2, _lyTopRankUI->getContentSize().height);
    _lyTopRankUI->addChild(sprTop);
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_top"), GAME_FONT, 24);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbTitle->setDimensions(sprTop->getContentSize().width/3, sprTop->getContentSize().height);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        lbTitle->setPosition(sprTop->getContentSize().width/4, sprTop->getContentSize().height/2);
        lbTitle->setColor(Color3B(255,255,0));
        sprTop->addChild(lbTitle);
        
        auto lbCondition = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_condition"), GAME_FONT, 24);
        lbCondition->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbCondition->setDimensions(sprTop->getContentSize().width/3, sprTop->getContentSize().height);
        lbCondition->setOverflow(Label::Overflow::SHRINK);
        lbCondition->setPosition(sprTop->getContentSize().width - sprTop->getContentSize().width/4, sprTop->getContentSize().height/2);
        lbCondition->setColor(Color3B(81,189,140));
        sprTop->addChild(lbCondition);
    }
    
    
    auto infoList = getInfoListFromType(PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::RANK, PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::RANK_RANGE);
    
    auto uiTopTier = UILayerRankInfo::create(Size(_lyTopRankUI->getContentSize().width, _lyTopRankUI->getContentSize().height - sprTop->getContentSize().height), infoList);
    uiTopTier->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTopTier->setPosition(sprTop->getPosition() + Vec2::DOWN * sprTop->getContentSize().height);
    _lyTopRankUI->addChild(uiTopTier);
    
    
}

void PopupRaidRankRewardInfo::initTierUI()
{
    auto sprTop = Sprite::create("Assets/ui/ranking/ranking_base_text_1_2.png");
    sprTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTop->setPosition(_lyTierUI->getContentSize().width/2, _lyTierUI->getContentSize().height);
    _lyTierUI->addChild(sprTop);
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_group"), GAME_FONT, 24);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbTitle->setDimensions(sprTop->getContentSize().width/3, sprTop->getContentSize().height);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        lbTitle->setPosition(sprTop->getContentSize().width/4, sprTop->getContentSize().height/2);
        lbTitle->setColor(Color3B(255,255,0));
        sprTop->addChild(lbTitle);
        
        auto lbCondition = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_condition"), GAME_FONT, 24);
        lbCondition->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbCondition->setDimensions(sprTop->getContentSize().width/3, sprTop->getContentSize().height);
        lbCondition->setOverflow(Label::Overflow::SHRINK);
        lbCondition->setPosition(sprTop->getContentSize().width - sprTop->getContentSize().width/4, sprTop->getContentSize().height/2);
        lbCondition->setColor(Color3B(81,189,140));
        sprTop->addChild(lbCondition);
    }
    
    auto infoList = getInfoListFromType(PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::TIER);
    
    auto uiTopTier = UILayerRankInfo::create(Size(_lyTopRankUI->getContentSize().width, _lyTopRankUI->getContentSize().height - sprTop->getContentSize().height), infoList);
    uiTopTier->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTopTier->setPosition(sprTop->getPosition() + Vec2::DOWN * sprTop->getContentSize().height);
    _lyTierUI->addChild(uiTopTier);
}


#pragma mark -utils

void PopupRaidRankRewardInfo::onClick(Ref* sender)
{
    auto item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
    
}

std::vector<InfoRaidTierInfo*> PopupRaidRankRewardInfo::getInfoListFromType(PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE type, PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE type2)
{
    std::vector<InfoRaidTierInfo*> result;
    result.clear();
    
    for(auto info : _vecInfo)
    {
        if(type == PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::END)
        {
            if(info->getType() == type)
            {
                result.push_back(info);
            }
        }
        else
        {
            if(info->getType() == type || info->getType() == type2)
            {
                result.push_back(info);
            }
        }
    }
    
    return result;
}

#pragma mark -info

InfoRaidTierInfo::InfoRaidTierInfo():
_eType(PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE::RANK),
_nIdx(0),
_nSeasonIdx(0),
_nCondition1(0),
_nCondition2(0),
_infoBadge(nullptr)
{
    _listReward.clear();
}
InfoRaidTierInfo::~InfoRaidTierInfo()
{
    _listReward.clear();
}

PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE InfoRaidTierInfo::getType()
{
    return _eType;
}
int InfoRaidTierInfo::getIdx()
{
    return _nIdx;
}
int InfoRaidTierInfo::getSeasonIdx()
{
    return _nSeasonIdx;
}
int InfoRaidTierInfo::getCondition1()
{
    return _nCondition1;
}
int InfoRaidTierInfo::getCondition2()
{
    return _nCondition2;
}

InfoBadge* InfoRaidTierInfo::getInfoBadge()
{
    return _infoBadge;
}
Vector<InfoItems*> InfoRaidTierInfo::getListReward()
{
    return _listReward;
}

void InfoRaidTierInfo::setType(PopupRaidRankRewardInfo::E_RAID_TIER_RANK_TYPE value)
{
    _eType = value;
}
void InfoRaidTierInfo::setIdx(int value)
{
    _nIdx = value;
}
void InfoRaidTierInfo::setSeasonIdx(int value)
{
    _nSeasonIdx = value;
}
void InfoRaidTierInfo::setCondition1(int value)
{
    _nCondition1 = value;
}
void InfoRaidTierInfo::setCondition2(int value)
{
    _nCondition2 = value;
}

void InfoRaidTierInfo::setInfoBadge(InfoBadge* value)
{
    _infoBadge = value;
}
void InfoRaidTierInfo::setListReward(Vector<InfoItems*> value)
{
    
    for(auto obj : value)
    {
        if(obj->getIdx() == 10000)
        {
            auto badge = BadgeManager::getInstance()->getBadge(obj->getTypeParams());
            setInfoBadge(badge);
        }
        else
            _listReward.pushBack(obj);
    }
}
