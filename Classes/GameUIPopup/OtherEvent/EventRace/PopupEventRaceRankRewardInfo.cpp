//
//  PopupEventRaceRankReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/08.
//

#include "PopupEventRaceRankRewardInfo.h"

#include "ManagerEvent/EventRaceManager.h"
#include "ManagerGame/ItemsManager.h"

#include "GameUIPopup/Other/Ranking/PopupPageRanking.h"

USING_NS_CC;

PopupEventRaceRankRewardInfo* PopupEventRaceRankRewardInfo::create()
{
    PopupEventRaceRankRewardInfo* pRet = new(std::nothrow) PopupEventRaceRankRewardInfo();
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

PopupEventRaceRankRewardInfo::PopupEventRaceRankRewardInfo():
_lyContainerParent(nullptr)
,_lyTitle(nullptr)
,_lyBanner(nullptr)
,_lyTable(nullptr)
,_lyBottom(nullptr)
,_table(nullptr)
{
    _vecInfo.clear();
}

PopupEventRaceRankRewardInfo::~PopupEventRaceRankRewardInfo(void)
{
    for(auto obj : _vecInfo)
        delete obj;
    _vecInfo.clear();
}
bool PopupEventRaceRankRewardInfo::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    setLoadRankReward();
    initVar();
    initUI();
    return true;
}
#pragma mark - table
void PopupEventRaceRankRewardInfo::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupEventRaceRankRewardInfo::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 75);
}

extension::TableViewCell* PopupEventRaceRankRewardInfo::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    auto objRankReward = _vecInfo[idx];
    auto sprBox = Sprite::create("Assets/ui/race_event/race_reward_rank_listbox.png");
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBox->setPosition(cell->getContentSize()/2);
    sprBox->getTexture()->setTexParameters(texParams);
    sprBox->setScale(4.f);
    cell->addChild(sprBox);
    
    if(objRankReward->getType() == PopupEventRaceRankRewardInfo::E_RANK_TYPE::RANK)
    {
        if(auto sprRank = Sprite::create(MafUtils::format("Assets/ui/race_event/race_reward_rank_no%d.png", objRankReward->getCondition1())))
        {
            sprRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprRank->setPosition(90, cell->getContentSize().height/2);
            cell->addChild(sprRank);
        }
    }
    else
    {
        std::string strText = MafUtils::format("%d ~ %d", objRankReward->getCondition1(), objRankReward->getCondition2());
        if(objRankReward->getIdx() >= _vecInfo.size())
            strText = MafUtils::format("%d ~", objRankReward->getCondition1());
        //160
        auto lbRank = Label::createWithTTF(strText, GAME_FONT, 30);
        lbRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbRank->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbRank->setDimensions(160, cell->getContentSize().height - 10);
        lbRank->setOverflow(Label::Overflow::SHRINK);
        lbRank->setPosition(90, cell->getContentSize().height/2);
        lbRank->setColor(Color3B(166,193,207));
        cell->addChild(lbRank);
    }
    
    auto lyReward = LayerColor::create();
    lyReward->setContentSize( Size(cell->getContentSize().width - 200, 58));
    lyReward->setIgnoreAnchorPointForPosition(false);
    lyReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
    lyReward->setPosition(200, cell->getContentSize().height/2);
    cell->addChild(lyReward);
    {
        auto layerScroll = ui::ScrollView::create();
        layerScroll->setBounceEnabled(false);
        layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        layerScroll->setContentSize(lyReward->getContentSize());
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        layerScroll->setPosition(Vec2(0, lyReward->getContentSize().height / 2));
        layerScroll->setSwallowTouches(false);
        lyReward->addChild(layerScroll);
        
        double innerWidth = 0;
        double innerHeight = layerScroll->getContentSize().height;
        
        auto listReward = objRankReward->getListReward();
        
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItem = listReward.at(i);
            
            double posX = 58 * i + 15 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 58;
            
            auto pathBoxBG = "Assets/ui_common/box_item.png";
            
            auto spriteItemBG = ui::Scale9Sprite::create(pathBoxBG);
            spriteItemBG->setContentSize(Size(58, 58));
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto lyIcon = LayerColor::create(Color4B::RED);
            lyIcon->setIgnoreAnchorPointForPosition(false);
            lyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lyIcon->setContentSize(spriteItemBG->getContentSize() - Size(2, 2));
            
            auto clipper = ClippingNode::create(lyIcon);
            clipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            clipper->setPosition(spriteItemBG->getContentSize()/2);
            clipper->setIgnoreAnchorPointForPosition(false);
            spriteItemBG->addChild(clipper);
            
            auto spriteItems = Sprite::create(objItem->getPath());
            if ( spriteItems != nullptr )
            {
                spriteItems->setPosition(clipper->getContentSize().width / 2, clipper->getContentSize().height / 2 + 5);
                spriteItems->getTexture()->setTexParameters(texParams);
                clipper->addChild(spriteItems);
            }
            
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }

            auto labelCount = ui::CText::create(strItemsCount, GAME_FONT, 14);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(Vec2(spriteItemBG->getContentSize().width / 2, 0));
            labelCount->enableOutline(Color4B::BLACK, 2);
            spriteItemBG->addChild(labelCount);
        }
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
    }
    
    return cell;
}

ssize_t PopupEventRaceRankRewardInfo::numberOfCellsInTableView(extension::TableView *table)
{
    return _vecInfo.size();
}


#pragma mark - PopupEventRaceRankReward
void PopupEventRaceRankRewardInfo::initVar()
{
    _spriteContainer->setVisible(false);
}

void PopupEventRaceRankRewardInfo::initUI()
{
    _lyContainerParent = Layer::create();
    _lyContainerParent->setContentSize(Size(630, 1030));
    _lyContainerParent->setIgnoreAnchorPointForPosition(false);
    _lyContainerParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _lyContainerParent->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    addChild(_lyContainerParent);
    
    auto sprBG = ui::Scale9Sprite::create("Assets/ui/race_event/race_reward_rank_popupbox.png", Rect::ZERO, Rect(9,9,15,15));
    sprBG->setContentSize(_lyContainerParent->getContentSize());
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(_lyContainerParent->getContentSize()/2);
    _lyContainerParent->addChild(sprBG);
    
    _lyTitle = LayerColor::create();
    _lyTitle->setContentSize(Size(_lyContainerParent->getContentSize().width - 20, 75));
    _lyTitle->setIgnoreAnchorPointForPosition(false);
    _lyTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTitle->setPosition(_lyContainerParent->getContentSize().width / 2, _lyContainerParent->getContentSize().height - 10);
    _lyContainerParent->addChild(_lyTitle);
    
    _lyBanner = LayerColor::create();
    _lyBanner->setContentSize(Size(_lyContainerParent->getContentSize().width - 20, 100));
    _lyBanner->setIgnoreAnchorPointForPosition(false);
    _lyBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBanner->setPosition(_lyTitle->getPosition() + Vec2::DOWN * _lyTitle->getContentSize().height);
    _lyContainerParent->addChild(_lyBanner);
    
    _lyTable = LayerColor::create();
    _lyTable->setContentSize(Size(_lyContainerParent->getContentSize().width - 20, _lyContainerParent->getContentSize().height - _lyTitle->getContentSize().height - _lyBanner->getContentSize().height - 20 - 150));
    _lyTable->setIgnoreAnchorPointForPosition(false);
    _lyTable->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTable->setPosition(_lyBanner->getPosition() + Vec2::DOWN * _lyBanner->getContentSize().height);
    _lyContainerParent->addChild(_lyTable);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(_lyContainerParent->getContentSize().width - 20, 150));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottom->setPosition(_lyTable->getPosition() + Vec2::DOWN * _lyTable->getContentSize().height);
    _lyContainerParent->addChild(_lyBottom);
    
    uiTitle();
    uiBanner();
    uiTable();
    uiBottom();
}

#pragma mark -
void PopupEventRaceRankRewardInfo::setLoadRankReward()
{
    _vecInfo.clear();

    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::EVENT_RACE_RANK_REWARD);

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
            auto idx = jsonValue["idx"].GetInt();
            auto rank_type = jsonValue["rank_type"].GetInt();
            auto condition = jsonValue["condition1"].GetInt();
            auto condition2 = jsonValue["condition2"].GetInt();
            auto reward = jsonValue["reward"].GetString();
            
            auto listReward = ItemsManager::getInstance()->getConvertArray(reward);
            
            auto info = new InfoEventRaceRankReward();
            info->setIdx(idx);
            info->setType((PopupEventRaceRankRewardInfo::E_RANK_TYPE) rank_type);
            info->setCondition1(condition);
            info->setCondition2(condition2);
            info->setListReward(listReward);
            _vecInfo.push_back(info);
        }
    }
    
}

#pragma mark - UI
void PopupEventRaceRankRewardInfo::uiTitle()
{
    _lyTitle->removeAllChildren();
    
    auto sprHiddenBox = ui::Scale9Sprite::create("Assets/ui/race_event/race_reward_title.png",Rect::ZERO, Rect(67, 0, 22, 40));
    sprHiddenBox->setContentSize(Size(_lyTitle->getContentSize().width - 10, sprHiddenBox->getContentSize().height));
    sprHiddenBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprHiddenBox->setPosition(_lyTitle->getContentSize().width/2, _lyTitle->getContentSize().height/2);
    _lyTitle->addChild(sprHiddenBox);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_raidreward_icon"), GAME_FONT, 30);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbTitle->setPosition(sprHiddenBox->getContentSize()/2);
    lbTitle->setDimensions(sprHiddenBox->getContentSize().width - 140, sprHiddenBox->getContentSize().height - 5);
    lbTitle->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->enableBold();
    lbTitle->setColor(COLOR_TITLE);
    sprHiddenBox->addChild(lbTitle);
}
void PopupEventRaceRankRewardInfo::uiBanner()
{
    _lyBanner->removeAllChildren();
    auto sprBanner = Sprite::create("Assets/ui/race_event/race_reward_rank_top.png");
    sprBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBanner->setPosition(_lyBanner->getContentSize()/2);
    _lyBanner->addChild(sprBanner);
    
    auto sprSkinIcon = Sprite::create("Assets/ui/race_event/race_reward_skin_icon.png");
    sprSkinIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprSkinIcon->setPosition(Vec2(sprBanner->getPositionX() - sprBanner->getContentSize().width/2, 0));
    _lyBanner->addChild(sprSkinIcon);
}
void PopupEventRaceRankRewardInfo::uiTable()
{
    _lyTable->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyTable->getContentSize().width, _lyTable->getContentSize().height));
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
void PopupEventRaceRankRewardInfo::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto btnClose = ui::Button::create("Assets/ui/race_event/race_btn_big_blue.png");
    btnClose->setScale9Enabled(true);
    btnClose->getRendererNormal()->setCapInsets(Rect(11, 10, 65, 65));
    btnClose->setContentSize(Size(200, 84));
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnClose->setPosition(_lyBottom->getContentSize()/2);
    btnClose->setZoomScale(0.02f);
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupEventRaceRankRewardInfo::onClickClose, this));
    _lyBottom->addChild(btnClose);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnClose->getContentSize()/2);
        lbText->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height - 10);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnClose->addChild(lbText);
    }
}
#pragma mark - Utils
void PopupEventRaceRankRewardInfo::onClickClose(Ref* sender)
{
    hide();
}


#pragma mark -info
InfoEventRaceRankReward::InfoEventRaceRankReward():
_eType(PopupEventRaceRankRewardInfo::E_RANK_TYPE::RANK),
_nIdx(0),
_nCondition1(0),
_nCondition2(0)
{
    _listReward.clear();
}
InfoEventRaceRankReward::~InfoEventRaceRankReward()
{
    _listReward.clear();
}

PopupEventRaceRankRewardInfo::E_RANK_TYPE InfoEventRaceRankReward::getType()
{
    return _eType;
}
int InfoEventRaceRankReward::getIdx()
{
    return _nIdx;
}
int InfoEventRaceRankReward::getCondition1()
{
    return _nCondition1;
}
int InfoEventRaceRankReward::getCondition2()
{
    return _nCondition2;
}

Vector<InfoItems*> InfoEventRaceRankReward::getListReward()
{
    return _listReward;
}

void InfoEventRaceRankReward::setType(PopupEventRaceRankRewardInfo::E_RANK_TYPE value)
{
    _eType = value;
}
void InfoEventRaceRankReward::setIdx(int value)
{
    _nIdx = value;
}
void InfoEventRaceRankReward::setCondition1(int value)
{
    _nCondition1 = value;
}
void InfoEventRaceRankReward::setCondition2(int value)
{
    _nCondition2 = value;
}
void InfoEventRaceRankReward::setListReward(Vector<InfoItems*> value)
{
    _listReward = value;
}
