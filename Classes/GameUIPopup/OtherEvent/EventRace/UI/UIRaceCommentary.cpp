//
//  UIRaceCommentary.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/02.
//

#include "UIRaceCommentary.h"

#include "GameUIPopup/OtherEvent/EventRace/UI/UISubItemStatus.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerEvent/EventRaceManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

UIRaceCommentary* UIRaceCommentary::create(Size size)
{
    UIRaceCommentary* pRet = new(std::nothrow) UIRaceCommentary();
    if ( pRet && pRet->init(size) )
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

UIRaceCommentary::UIRaceCommentary(void):
_eType(E_TYPE::READY)
,_nCommentaryIdx(0)
,_timer(0.5)
,_table(nullptr)
{
    _listCommentaryKeyReady.clear();
    _listCommentaryKeyStart.clear();
    _listCommentaryKeyMiddle.clear();
    _listCommentaryKeyEnd.clear();
    _listCommentaryKeyGoal.clear();
    _listCommentaryKeyTalk.clear();
    _listCommentaryKeyAds.clear();
    _listCurGameCommentary.clear();
}

UIRaceCommentary::~UIRaceCommentary(void)
{
    _listCommentaryKeyReady.clear();
    _listCommentaryKeyStart.clear();
    _listCommentaryKeyMiddle.clear();
    _listCommentaryKeyEnd.clear();
    _listCommentaryKeyGoal.clear();
    _listCommentaryKeyTalk.clear();
    _listCommentaryKeyAds.clear();
    _listCurGameCommentary.clear();
    
    EventRaceManager::getInstance()->unSubRaceReadyEvent(this);
    EventRaceManager::getInstance()->unSubRaceStartEvent(this);
    EventRaceManager::getInstance()->unSubAdsEvent(this);
}

bool UIRaceCommentary::init(const Size size)
{
    if (!LayerColor::init())
    {
        return false;
    }
    setContentSize(size);
    // init
    setLoadTextKey();
    initVar();
    initUI();
    
    EventRaceManager::getInstance()->subRaceReadyEvent(new MafFunction<void()>(CC_CALLBACK_0(UIRaceCommentary::onEventRaceRaedy, this), this));
    EventRaceManager::getInstance()->subRaceStartEvent(new MafFunction<void()>(CC_CALLBACK_0(UIRaceCommentary::onEventClickStart, this), this));
    EventRaceManager::getInstance()->subAdsEvent(new MafFunction<void(int)>(CC_CALLBACK_1(UIRaceCommentary::onEventClickAds, this), this));
    
    scheduleUpdate();
    return true;
}

void UIRaceCommentary::setLoadTextKey()
{
    std::string strText = EventManager::getInstance()->getEventOhterDataByEventIdx(33);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    const int nCommentReadyCount = jsonParser["comment_ready_count"].GetInt();
    const int nCommentStartCount = jsonParser["comment_start_count"].GetInt();
    const int nCommentMiddleCount = jsonParser["comment_middle_count"].GetInt();
    const int nCommentEndCount = jsonParser["comment_end_count"].GetInt();
    const int nCommentGoalCount = jsonParser["comment_goal_count"].GetInt();
    const int nCommentAdsCount = jsonParser["comment_ads_count"].GetInt();
    const int nCommentTalkCount = jsonParser["comment_talk_count"].GetInt();
    std::string key = "";
    for(int i = 1; i <= nCommentReadyCount; ++i)
    {
        key = MafUtils::format("t_ui_event_race_caster_ready_%02d", i);
        if(GAME_TEXT(key).size() > 0)
            _listCommentaryKeyReady.push_back(key);
    }
    for(int i = 1; i <= nCommentStartCount; ++i)
    {
        key = MafUtils::format("t_ui_event_race_caster_start_%02d", i);
        if(GAME_TEXT(key).size() > 0)
            _listCommentaryKeyStart.push_back(key);
    }
    for(int i = 1; i <= nCommentMiddleCount; ++i)
    {
        key = MafUtils::format("t_ui_event_race_caster_middle_%02d", i);
        if(GAME_TEXT(key).size() > 0)
            _listCommentaryKeyMiddle.push_back(key);
    }
    for(int i = 1; i <= nCommentEndCount; ++i)
    {
        key = MafUtils::format("t_ui_event_race_caster_end_%02d", i);
        if(GAME_TEXT(key).size() > 0)
            _listCommentaryKeyEnd.push_back(key);
    }
    for(int i = 1; i <= nCommentGoalCount; ++i)
    {
        key = MafUtils::format("t_ui_event_race_caster_goal_%02d", i);
        if(GAME_TEXT(key).size() > 0)
            _listCommentaryKeyGoal.push_back(key);
    }
    for(int i = 1; i <= nCommentTalkCount; ++i)
    {
        key = MafUtils::format("t_ui_event_race_caster_talk_%02d", i);
        if(GAME_TEXT(key).size() > 0)
            _listCommentaryKeyTalk.push_back(key);
    }
    for(int i = 1; i <= nCommentAdsCount; ++i)
    {
        key = MafUtils::format("t_ui_event_race_caster_double_%02d", i);
        if(GAME_TEXT(key).size() > 0)
            _listCommentaryKeyAds.push_back(key);
    }
}

void UIRaceCommentary::update(float dt)
{
    _timer += dt;
    
    if(EventRaceManager::getInstance()->isGameRunning())
        checkCommentType();
    
    if((_eType != UIRaceCommentary::E_TYPE::READY && _timer >= 0.5f) ||
       (_eType == UIRaceCommentary::E_TYPE::READY && _timer >= 1.f))
        _timer = 0;
    else
        return;
    
    std::string strComment = getCurrentComment();
    
    if(strComment == "")
        return;
    
    _listCurGameCommentary.insert(_listCurGameCommentary.begin(), strComment);
    _nCommentaryIdx++;
    if(_listCurGameCommentary.size() > 20)
        _listCurGameCommentary.pop_back();
    
    _table->reloadData();
}
#pragma mark - table
void UIRaceCommentary::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size UIRaceCommentary::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 30);
}

extension::TableViewCell* UIRaceCommentary::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    auto lbComment = Label::createWithTTF(getText(_listCurGameCommentary[idx]), GAME_FONT, 24);
    lbComment->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbComment->setPosition(10, cell->getContentSize().height/2);
    lbComment->setDimensions(cell->getContentSize().width, cell->getContentSize().height - 5);
    lbComment->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbComment->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbComment->enableBold();
    lbComment->setColor(Color3B(250,255,205));
    cell->addChild(lbComment);
    
    return cell;
}

ssize_t UIRaceCommentary::numberOfCellsInTableView(extension::TableView *table)
{
    return _listCurGameCommentary.size();
}

#pragma mark - init
void UIRaceCommentary::initVar()
{
    setIgnoreAnchorPointForPosition(false);
}

void UIRaceCommentary::initUI()
{
    removeAllChildren();
    
    _table = extension::TableView::create(this, getContentSize());
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _table->setPosition(getContentSize()/2);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::BOTTOM_UP);
    _table->setDelegate(this);
    _table->setBounceable(false);
    addChild(_table);
    
    _table->reloadData();
}

#pragma mark - utils
void UIRaceCommentary::nextType()
{
    int tmpType = (int)_eType + 1;
    _eType = (UIRaceCommentary::E_TYPE)tmpType;
    _nCommentaryIdx = 0;
}
void UIRaceCommentary::checkCommentType()
{
    auto objHorse = EventRaceManager::getInstance()->getInfoHorseByCurrentRank(1);
    if(objHorse == nullptr)
        return;
    
    if((objHorse->getCurrentSection() >= 33 && _eType == UIRaceCommentary::E_TYPE::START)||
       (objHorse->getCurrentSection() >= 66 && _eType == UIRaceCommentary::E_TYPE::MIDDLE) ||
       (objHorse->isComplateRace() && _eType == UIRaceCommentary::E_TYPE::END))
        nextType();
}
const std::string UIRaceCommentary::getText(const std::string strKey) const
{
    std::string result = "";
    auto objHorse1st = EventRaceManager::getInstance()->getInfoHorseByCurrentRank(1);
    auto objHorse2nd = EventRaceManager::getInstance()->getInfoHorseByCurrentRank(2);
    auto objHorse3th = EventRaceManager::getInstance()->getInfoHorseByCurrentRank(3);
    
    //1등 인덱스 1회
    if(strKey.compare("t_ui_event_race_caster_start_03") == 0 ||
       strKey.compare("t_ui_event_race_caster_middle_05") == 0)
    {
        if(objHorse1st != nullptr)
            result = GAME_TEXTFORMAT(strKey.c_str(), std::to_string((int)objHorse1st->getIdx()).c_str());
    }
    //2등, 3등 인덱스 각 1회
    else if(strKey.compare("t_ui_event_race_caster_start_04") == 0 ||
            strKey.compare("t_ui_event_race_caster_middle_03") == 0 ||
            strKey.compare("t_ui_event_race_caster_end_04") == 0)
    {
        if(objHorse2nd != nullptr && objHorse3th != nullptr)
            result = GAME_TEXTFORMAT(strKey.c_str(), std::to_string((int)objHorse2nd->getIdx()).c_str(), std::to_string((int)objHorse3th->getIdx()).c_str());
    }
    //1등 인덱스 2회
    else if(strKey.compare("t_ui_event_race_caster_middle_02") == 0 ||
            strKey.compare("t_ui_event_race_caster_end_03") == 0)
    {
        if(objHorse1st != nullptr)
            result = GAME_TEXTFORMAT(strKey.c_str(), std::to_string((int)objHorse1st->getIdx()).c_str(), std::to_string((int)objHorse1st->getIdx()).c_str());
    }
    //1등 스킨 이름 1회
    else if(strKey.compare("t_ui_event_race_caster_goal_02") == 0 ||
            strKey.compare("t_ui_event_race_caster_goal_04") == 0)
    {
        if(objHorse1st != nullptr)
        {
            auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(objHorse1st->getSkinIdx());
            
            if(objSkin != nullptr)
                result = GAME_TEXTFORMAT(strKey.c_str(), objSkin->getName().c_str());
        }
    }
    //1등 스킨 이름 2회
    else if(strKey.compare("t_ui_event_race_caster_goal_03") == 0)
    {
        if(objHorse1st != nullptr)
        {
            auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(objHorse1st->getSkinIdx());
            
            if(objSkin != nullptr)
                result = GAME_TEXTFORMAT(strKey.c_str(), objSkin->getName().c_str(), objSkin->getName().c_str());
        }
    }
    else
        result = GAME_TEXT(strKey.c_str());
    
    
    return result;
}

const std::string UIRaceCommentary::getCurrentComment()
{
    std::string result = "";
    switch (_eType)
    {
        case UIRaceCommentary::E_TYPE::READY:
        {
            if(_nCommentaryIdx < _listCommentaryKeyReady.size())
                result = _listCommentaryKeyReady[_nCommentaryIdx];
            else
                _nCommentaryIdx = 0;
        }break;
        case UIRaceCommentary::E_TYPE::START:
        {
            if(_nCommentaryIdx < _listCommentaryKeyStart.size())
                result = _listCommentaryKeyStart[_nCommentaryIdx];
            else
                result = _listCommentaryKeyTalk[random(0, (int)_listCommentaryKeyTalk.size()-1)];
        }break;
        case UIRaceCommentary::E_TYPE::MIDDLE:
        {
            if(_nCommentaryIdx < _listCommentaryKeyMiddle.size())
                result = _listCommentaryKeyMiddle[_nCommentaryIdx];
            else
                result = _listCommentaryKeyTalk[random(0, (int)_listCommentaryKeyTalk.size()-1)];
        }break;
        case UIRaceCommentary::E_TYPE::END:
        {
            if(_nCommentaryIdx < _listCommentaryKeyEnd.size())
                result = _listCommentaryKeyEnd[_nCommentaryIdx];
            else
                result = _listCommentaryKeyTalk[random(0, (int)_listCommentaryKeyTalk.size()-1)];
        }break;
        case UIRaceCommentary::E_TYPE::GOAL:
        {
            if(_nCommentaryIdx < _listCommentaryKeyGoal.size())
                result = _listCommentaryKeyGoal[_nCommentaryIdx];
        }break;
        case UIRaceCommentary::E_TYPE::ADS:
        {
            if(_nCommentaryIdx < _listCommentaryKeyAds.size())
                result = _listCommentaryKeyAds[_nCommentaryIdx];
        }break;
    }
    
    return result;
}

#pragma mark - event
void UIRaceCommentary::onEventRaceRaedy()
{
    
}
void UIRaceCommentary::onEventClickStart()
{
    _listCurGameCommentary.clear();
    
    if(_eType == UIRaceCommentary::E_TYPE::ADS)
        _eType = UIRaceCommentary::E_TYPE::READY;
    
    nextType();
}
void UIRaceCommentary::onEventClickAds(int nType)
{
    if(nType == 2)
    {
        _eType = UIRaceCommentary::E_TYPE::ADS;
        _nCommentaryIdx = 0;
    }
}
