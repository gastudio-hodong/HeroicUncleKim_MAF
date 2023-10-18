//
//  PopupEventRaceReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/29.
//

#include "PopupEventRaceReward.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerEvent/EventRaceManager.h"
#include "ManagerEvent/EventManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

bool PopupEventRaceReward::sortRank(InfoEventRaceHorse* const a, InfoEventRaceHorse* const b)
{
    return a->getFinalRank() < b->getFinalRank();
}

PopupEventRaceReward* PopupEventRaceReward::create()
{
    PopupEventRaceReward* pRet = new(std::nothrow) PopupEventRaceReward();
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

PopupEventRaceReward::PopupEventRaceReward():
_lyTop(nullptr)
,_lyTopLane(nullptr)
,_lyTopPlatform(nullptr)
,_lyTable(nullptr)
,_lyBottom(nullptr)
,_table(nullptr)
{
    _listHorse.clear();
    _listInfoGameRewardRule.clear();
}

PopupEventRaceReward::~PopupEventRaceReward(void)
{
    _listHorse.clear();
    _listInfoGameRewardRule.clear();
}
bool PopupEventRaceReward::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    _bHideBackKey = false;
    _listHorse = EventRaceManager::getInstance()->getListHorse();
    std::sort(_listHorse.begin(), _listHorse.end(), PopupEventRaceReward::sortRank);
    setLoadInfoGameRewardRule();
    initVar();
    initUI();
    
    return true;
}
#pragma mark - table
void PopupEventRaceReward::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupEventRaceReward::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 90);
}

extension::TableViewCell* PopupEventRaceReward::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    auto objHorse = _listHorse.at(idx);
    if(objHorse == nullptr)
        return cell;
    
    int nRank = objHorse->getFinalRank();
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(cell->getContentSize()/2);
    sprBox->setContentSize(Size(708, 80));
    cell->addChild(sprBox);
    
    if(nRank <= 3)
    {
        auto sprRank = Sprite::create(MafUtils::format("Assets/ui/race_event/race_medal_%d.png", nRank));
        sprRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprRank->setPosition(Vec2(10, sprBox->getContentSize().height/2));
        sprBox->addChild(sprRank);
    }
    else
    {
        auto lbRankNum = Label::createWithTTF(std::to_string(nRank), GAME_FONT, 28);
        lbRankNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbRankNum->setPosition(20, sprBox->getContentSize().height/2);
        lbRankNum->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbRankNum->setColor(Color3B(160,150,120));
        sprBox->addChild(lbRankNum);
        
        auto lbRank = Label::createWithTTF("th", GAME_FONT, 26);
        lbRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbRank->setPosition(lbRankNum->getPosition() + Vec2::RIGHT * lbRankNum->getContentSize().width);
        lbRank->setDimensions(64 - lbRankNum->getContentSize().width, lbRank->getContentSize().height);
        lbRank->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbRank->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbRank->setColor(Color3B(160,150,120));
        sprBox->addChild(lbRank);
    }
    
    auto sprIconBox = Sprite::create("Assets/ui/main/main_hero_edge.png");
    sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconBox->setPosition(90, sprBox->getContentSize().height/2);
    sprBox->addChild(sprIconBox);
    {
        auto sprHeroIcon = Sprite::create(MafUtils::format("Assets/icon_hero/skin_%d.png", objHorse->getSkinIdx()));
        sprHeroIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprHeroIcon->setPosition(sprIconBox->getContentSize()/2);
        sprHeroIcon->setScale(sprIconBox->getContentSize().width / sprHeroIcon->getContentSize().width);
        sprIconBox->addChild(sprHeroIcon);
    }
    
    auto sprIconNum = Sprite::create(MafUtils::format("Assets/ui/race_event/race_rail_number%d.png", objHorse->getIdx()));
    sprIconNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconNum->setPosition(sprIconBox->getPosition() + Vec2::RIGHT * (sprIconBox->getContentSize().width + 15));
    sprBox->addChild(sprIconNum);
    
    auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(objHorse->getSkinIdx());
    
    auto lbName = Label::createWithTTF(objSkin->getName(), GAME_FONT, 26);
    lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbName->setPosition(sprIconNum->getPosition() + Vec2::RIGHT * (sprIconNum->getContentSize().width + 20));
    lbName->setDimensions(230, lbName->getContentSize().height);
    lbName->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbName->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbName->enableBold();
    lbName->setColor(Color3B(250,255,205));
    sprBox->addChild(lbName);
    
    auto sprRewardBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_14.png",Rect::ZERO, Rect(3,3,10,10));
    sprRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprRewardBox->setPosition(sprBox->getContentSize().width - 15, sprBox->getContentSize().height/2);
    sprRewardBox->setContentSize(Size(175, 50));
    sprBox->addChild(sprRewardBox);
    {
        auto objRule = getInfoGameRewardRule(nRank);
        auto objItem = ItemsManager::getInstance()->getInfoItems(objRule.nRewardIdx);
        
        auto sprRewardIcon = Sprite::create(objItem->getPath());
        sprRewardIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprRewardIcon->setPosition(Vec2(0, sprRewardBox->getContentSize().height/2));
        sprRewardBox->addChild(sprRewardIcon);
        
        double count = objHorse->getInvest();
        
        count *= objRule.nRewardMag;
        
        if(nRank == 1)
            count *= EventRaceManager::getInstance()->getGameRewardMag();
        
        std::string strCount = MafUtils::doubleToString(count);
        MafUtils::convertNumberToShort(strCount);
        
        auto lbCount = Label::createWithTTF(strCount, GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        lbCount->setPosition(sprRewardBox->getContentSize().width - 10, sprRewardBox->getContentSize().height/2);
        lbCount->setDimensions(sprRewardBox->getContentSize().width - sprRewardIcon->getContentSize().width - 10, sprRewardBox->getContentSize().height - 10);
        lbCount->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::CENTER);
        lbCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbCount->setColor(Color3B(250,255,205));
        sprRewardBox->addChild(lbCount);
    }
    
     if(EventRaceManager::getInstance()->getGameRewardMag() > 1 && nRank == 1)
     {
         auto sprDoubleIcon = Sprite::create("Assets/ui/race_event/ad_x2_mini.png");
         sprDoubleIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
         sprDoubleIcon->setPosition(sprRewardBox->getPosition() + Vec2::LEFT * (sprRewardBox->getContentSize().width) + Vec2::UP * (sprRewardBox->getContentSize().height/2 - 15));
         sprDoubleIcon->setScale(0.5f);
         sprBox->addChild(sprDoubleIcon);
     }
    return cell;
}

ssize_t PopupEventRaceReward::numberOfCellsInTableView(extension::TableView *table)
{
    return _listHorse.size();
}


#pragma mark - init
void PopupEventRaceReward::initVar()
{
    _spriteContainer->setContentSize(Size(730,1010));
}

void PopupEventRaceReward::initUI()
{
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_result_1"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize( Size(720, 254));
    _lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _spriteContainer->addChild(_lyTop);
    
    auto ly = LayerColor::create();
    ly->setContentSize(_lyTop->getContentSize());

    auto clipper = ClippingNode::create(ly);
    clipper->setContentSize(ly->getContentSize());
    clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    clipper->setPosition(_lyTop->getContentSize()/2);
    clipper->setIgnoreAnchorPointForPosition(false);
    _lyTop->addChild(clipper);
    
    _lyTopLane = LayerColor::create();
    _lyTopLane->setContentSize( _lyTop->getContentSize());
    _lyTopLane->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    _lyTopLane->setPosition(clipper->getContentSize()/2);
    _lyTopLane->setIgnoreAnchorPointForPosition(false);
    clipper->addChild(_lyTopLane);
    
    _lyTopPlatform = LayerColor::create();
    _lyTopPlatform->setContentSize(_lyTop->getContentSize());
    _lyTopPlatform->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    _lyTopPlatform->setPosition(clipper->getContentSize()/2);
    _lyTopPlatform->setIgnoreAnchorPointForPosition(false);
    clipper->addChild(_lyTopPlatform);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize(Size(720, 560));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTable->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    _spriteContainer->addChild(_lyTable);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(720, _spriteContainer->getContentSize().height - 75 - _lyTop->getContentSize().height - _lyTable->getContentSize().height));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottom->setPosition(_lyTable->getPosition() + Vec2::DOWN * _lyTable->getContentSize().height);
    _spriteContainer->addChild(_lyBottom);
    
    uiTop();
    uiTable();
    uiBottom();
}

void PopupEventRaceReward::setLoadInfoGameRewardRule()
{
    std::string strText = EventManager::getInstance()->getEventOhterDataByEventIdx(33);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == true )
    {
        return;
    }
    
    _listInfoGameRewardRule.clear();
    
    for ( int i = 0; i < jsonParser["game_reward"].Size(); i++ )
    {
        const rapidjson::Value& jsonValue = jsonParser["game_reward"][i];
        auto jsonReward = jsonValue["reward"].GetObject();
        
        //Scene
        const int nRank = jsonValue["rank"].GetInt();
        const int nRewardIdx = jsonReward["idx"].GetInt();
        const float nRewardMag = jsonReward["mag"].GetFloat();

        PopupEventRaceReward::InfoGameRankRewardRule obj;
        obj.nRank = nRank;
        obj.nRewardIdx = nRewardIdx;
        obj.nRewardMag = nRewardMag;
        
        _listInfoGameRewardRule.push_back(obj);
    }
}
#pragma mark - ui
void PopupEventRaceReward::uiTop()
{
    uiTopLane();
    uiTopPlatform();
}
void PopupEventRaceReward::uiTopLane()
{
    _lyTopLane->removeAllChildren();
    
    auto sprBG = Sprite::create("Assets/ui/race_event/race_result_bg0.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(_lyTopLane->getContentSize()/2);
    sprBG->setScale(2);
    sprBG->getTexture()->setTexParameters(texParams);
    _lyTopLane->addChild(sprBG);
    
    auto nodeLane = Node::create();
    nodeLane->setPosition(Vec2::ZERO);
    nodeLane->setAnchorPoint(Vec2::ZERO);
    _lyTopLane->addChild(nodeLane);
    
    //74 170
    for(int i = 4; i <= 6; ++i)
    {
        auto objHorse = getHorseByRank(i);
        if(objHorse == nullptr) continue;
        
        auto sprHorse = Sprite::create(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_1.png", objHorse->getSkinIdx()));
        sprHorse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprHorse->setScale(0.6f);
        {
            auto anim = Animation::create();
            anim->setDelayPerUnit(0.1f);
            for(int j = 0; j < 4; ++j)
            {
                anim->addSpriteFrameWithFile(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_%d.png", objHorse->getSkinIdx(), j+1));
            }
            auto animate = Animate::create(anim);
            auto repeat = RepeatForever::create(animate);
            sprHorse->runAction(repeat);
        }
        
        auto node = Node::create();
        node->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        node->setTag(i-4);
        node->setPosition(Vec2(-100 - 10 * node->getTag() , random(30, 100)));
        node->setLocalZOrder(-node->getPositionY());
        node->setContentSize(sprHorse->getContentSize());
        nodeLane->addChild(node);
        {
            auto move = MoveBy::create(random(3.5f, 6.5f), Vec2(_lyTopLane->getContentSize().width + 50, 0));
            auto func = CallFunc::create([&, node](){
                if(node->getPositionX() >= _lyTopLane->getContentSize().width)
                {
                    node->setPosition(Vec2(-100, random(30, 100)));
                    node->setLocalZOrder(-node->getPositionY());
                }
            });
            auto seq = Sequence::create(DelayTime::create(0.1f), func, nullptr);
            auto spwan = Spawn::create(move, seq, nullptr);
            node->runAction(RepeatForever::create(spwan));
        }
        
        sprHorse->setPosition(node->getContentSize()/2);
        node->addChild(sprHorse);
        
        auto sprTire = Sprite::create("Assets/ui/race_event/tire_ani_1.png");
        sprTire->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        sprTire->setPosition(110, node->getContentSize().height/2 - 28);
        sprTire->setScale(0.6);
        node->addChild(sprTire);
        {
            auto anim = Animation::create();
            anim->setDelayPerUnit(0.1f);
            for(int j = 0; j < 4; ++j)
            {
                anim->addSpriteFrameWithFile(MafUtils::format("Assets/ui/race_event/tire_ani_%d.png", j+1));
            }
            auto animate = Animate::create(anim);
            auto repeat = RepeatForever::create(animate);
            sprTire->runAction(repeat);
        }
    }
}
void PopupEventRaceReward::uiTopPlatform()
{
    _lyTopPlatform->removeAllChildren();
    
    auto sprPlatform = Sprite::create("Assets/ui/race_event/race_result_bg1.png");
    sprPlatform->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprPlatform->setPosition(_lyTopPlatform->getContentSize().width/2, 0);
    sprPlatform->setScale(2);
    sprPlatform->getTexture()->setTexParameters(texParams);
    _lyTopPlatform->addChild(sprPlatform);
    
    //2등 178, 97, 1등 x 절반, y 120, 3등 -178, 97
    
    std::vector<Vec2> listPos = {Vec2(_lyTopPlatform->getContentSize().width/2 - 2, 120),
        Vec2(173, 97),
        Vec2(_lyTopPlatform->getContentSize().width - 178, 84)
    };
    
    for(int i = 1; i <= 3; ++i)
    {
        auto objHorse = getHorseByRank(i);
        if(objHorse == nullptr)
            continue;
        auto sprHorse = Sprite::create(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_1.png", objHorse->getSkinIdx()));
        sprHorse->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprHorse->setPosition(listPos[i-1] + Vec2::RIGHT * 30 + Vec2::DOWN * 36);
        sprHorse->setScale(0.8f);
        _lyTopPlatform->addChild(sprHorse);
        {
            auto anim = Animation::create();
            anim->setDelayPerUnit(0.1f);
            for(int j = 0; j < 4; ++j)
            {
                anim->addSpriteFrameWithFile(MafUtils::format("Assets/character_hero/skin/skin_%02d_walk_%d.png", objHorse->getSkinIdx(), j+1));
            }
            auto animate = Animate::create(anim);
            auto repeat = RepeatForever::create(animate);
            sprHorse->runAction(repeat);
        }
    }
    auto fxPollen = ParticleSystemQuad::create("Assets/Particle/particle_pollen.plist");
    fxPollen->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    fxPollen->setPosition(_lyTopPlatform->getContentSize()/2);
    fxPollen->setScale(0.8f);
    _lyTopPlatform->addChild(fxPollen);
}
void PopupEventRaceReward::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height - 10));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyTable->getContentSize().width / 2, _lyTable->getContentSize().height - 5);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyTable->addChild(_table);
    
    _table->reloadData();
}

void PopupEventRaceReward::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto btnClose = ui::Button::create("Assets/ui/race_event/race_btn_big_blue.png");
    btnClose->setScale9Enabled(true);
    btnClose->getRendererNormal()->setCapInsets(Rect(11, 10, 65, 65));
    btnClose->setContentSize(Size(204, 84));
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnClose->setPosition(_lyBottom->getContentSize()/2);
    btnClose->setZoomScale(0.02f);
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupEventRaceReward::onClickClose, this));
    _lyBottom->addChild(btnClose);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnClose->getContentSize()/2);
        lbText->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height - 10);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnClose->addChild(lbText);
    }
    
}

#pragma mark - utils
void PopupEventRaceReward::onClickClose(Ref* sender)
{
    EventRaceManager::getInstance()->invokeRaceGameOverEvent(isBankrupt());
    hide();
}

InfoEventRaceHorse* PopupEventRaceReward::getHorseByRank(const int nRank)
{
    InfoEventRaceHorse* result = nullptr;
    
    for(auto obj : _listHorse)
    {
        if(obj->getFinalRank() == nRank)
        {
            result = obj;
            break;
        }
    }
    
    return result;
}

bool PopupEventRaceReward::isBankrupt()
{
    bool result = false;
    
    if(EventRaceManager::getInstance()->getCoin() > 0)
        return false;
    
    double nCoin = 0;
    for(auto objHorse : _listHorse)
    {
        auto objRule = getInfoGameRewardRule(objHorse->getFinalRank());
        
        if((E_ITEMS)objRule.nRewardIdx != E_ITEMS::EVENT_RACE_COIN)
            continue;
        
        nCoin += objHorse->getInvest() * objRule.nRewardMag;
    }
    
    if(nCoin <= 0)
        result = true;
    
    return result;
}

PopupEventRaceReward::InfoGameRankRewardRule PopupEventRaceReward::getInfoGameRewardRule(const int nRank)
{
    PopupEventRaceReward::InfoGameRankRewardRule result;
    
    for(auto obj : _listInfoGameRewardRule)
    {
        if(obj.nRank == nRank)
            return obj;
    }
    
    return result;
}
