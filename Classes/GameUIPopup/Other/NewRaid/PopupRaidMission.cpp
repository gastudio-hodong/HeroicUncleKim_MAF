//
//  PopupRaidMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/15.
//

#include "PopupRaidMission.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoQuest.h"

#include "ManagerGame/NewRaidManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupRaidMission* PopupRaidMission::create()
{
    PopupRaidMission* pRet = new(std::nothrow) PopupRaidMission();
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

PopupRaidMission::PopupRaidMission(void):
_lyTopUI(nullptr),
_lyMissionUI(nullptr),
_lyBottomUI(nullptr),
_callbackHide(nullptr),
_table(nullptr)
{
    _listTotalReward.clear();
    _listInfoQuest.clear();
}

PopupRaidMission::~PopupRaidMission(void)
{
    _listTotalReward.clear();
    _listInfoQuest.clear();
}


#pragma mark - table
void PopupRaidMission::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupRaidMission::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 105);
}

extension::TableViewCell* PopupRaidMission::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    auto info = _listInfoQuest.at(idx);
    
    if(info == nullptr)
        return cell;
    
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    bool isReceive = info->isReceive();
    bool isComplete = false;
    
    if(!isReceive && (info->getCountNow() >= info->getCountMax()))
        isComplete = true;
    
    std::string strCheckBoxPath = "Assets/ui_common/checkbox_1_off.png";
    
    if(isReceive)
        strCheckBoxPath = "Assets/ui_common/checkbox_1_on.png";
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    //base
    
    auto sprBG = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_1.png");
    sprBG->setContentSize(Size(490, 102));
    
    auto btnBg = MafNode::MafMenuItemSprite::create(sprBG, nullptr, CC_CALLBACK_1(PopupRaidMission::onClickComplete, this));
    btnBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnBg->setTag(info->getIdx());
    btnBg->setPosition(Vec2(5, cell->getContentSize().height/2));
    menu->addChild(btnBg);
    
    btnBg->setEnabled(false);
    if(isComplete)
    {
        auto sprEffect = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_0.png");
        sprEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprEffect->setContentSize(btnBg->getContentSize());
        sprEffect->setPosition(btnBg->getContentSize()/2);
        btnBg->addChild(sprEffect);
        btnBg->setEnabled(true);
        
        auto fadeOut = FadeOut::create(1.f);
        auto fadeIn = FadeIn::create(1.f);
        auto seq = Sequence::create(fadeOut,fadeIn, NULL);
        sprEffect->runAction(RepeatForever::create(seq));
    }
    if(isReceive)
        btnBg->setColor(Color3B(90,90,90));
    
    //text
    auto lbTitle = Label::createWithTTF(info->getTitle(), GAME_FONT, 24);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbTitle->setPosition(15, btnBg->getContentSize().height - btnBg->getContentSize().height/4);
    lbTitle->setDimensions(300, btnBg->getContentSize().height/2);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    btnBg->addChild(lbTitle);
    if(isReceive)
        lbTitle->enableStrikethrough();
    
    //checkbox
    auto sprCheckBox = Sprite::create(strCheckBoxPath);
    sprCheckBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprCheckBox->setPosition(15, btnBg->getContentSize().height/4);
    btnBg->addChild(sprCheckBox);
    
    //progress
    float nPercent = 0;
    
    nPercent = (float)info->getCountNow() / (float)info->getCountMax() * 100;
    if ( nPercent > 100 )
        nPercent = 100;
    
    auto spriteTimerBg = Sprite::create("Assets/ui_common/progressbar_01_1.png");
    spriteTimerBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    spriteTimerBg->setPosition(sprCheckBox->getPosition() + Vec2::RIGHT * (sprCheckBox->getContentSize().width + 10));
    btnBg->addChild(spriteTimerBg);
        
    auto timerCount = ProgressTimer::create(Sprite::create("Assets/ui_common/progressbar_01_2.png"));
    timerCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    timerCount->setPosition( spriteTimerBg->getPosition() );
    timerCount->setMidpoint(Vec2(0, 0.5));
    timerCount->setBarChangeRate(Vec2(1,0));
    timerCount->setType(ProgressTimer::Type::BAR);
    timerCount->setPercentage(nPercent);
    btnBg->addChild(timerCount);
    {
        std::string strTimer = MafUtils::format("%d/%d", (int)info->getCountNow(), (int)info->getCountMax());
        
        auto labelTimer = Label::createWithTTF(strTimer, GAME_FONT, 16);
        labelTimer->setPosition( Vec2(timerCount->getContentSize().width / 2, timerCount->getContentSize().height / 2) );
        timerCount->addChild(labelTimer);
    }
    if(isReceive)
        timerCount->setColor(Color3B(90,90,90));
    
    //rewards
    
    auto lyReward = LayerColor::create();
    lyReward->setContentSize( Size(140, btnBg->getContentSize().height - 15));
    lyReward->setIgnoreAnchorPointForPosition(false);
    lyReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
    lyReward->setPosition( btnBg->getPosition().x + btnBg->getContentSize().width - 10, cell->getContentSize().height/2);
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
        
        auto listItems = info->getListReward();
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objItem = listItems.at(i);
            
            double posX = 58 * i + 10 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 58;
            
            auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
            spriteItemBG->setContentSize(Size(58,58));
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto spriteItems = Sprite::create(objItem->getPath());
            if ( spriteItems != nullptr )
            {
                spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                spriteItems->getTexture()->setTexParameters(texParams);

                switch((E_ITEMS)objItem->getIdx())
                {
                    case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                    case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                }
                if(isReceive)
                {
                    spriteItems->setColor(Color3B(90,90,90));
                }
                spriteItemBG->addChild(spriteItems);
            }
            
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }

            auto labelCount = Label::createWithTTF(strItemsCount, GAME_FONT, 16);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 0);
            spriteItemBG->addChild(labelCount);
            if(isReceive)
            {
                labelCount->enableStrikethrough();
            }
        }
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        
        if(isReceive)
        {
            lyReward->setColor(Color3B(90,90,90));
        }
    }
    
    //double ticket
    auto sprTicketBox = ui::Scale9Sprite::create("Assets/ui_common/list_bg_s_2_3.png");
    sprTicketBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprTicketBox->setContentSize(Size(178, 102));
    sprTicketBox->setPosition(Vec2(cell->getContentSize().width - 5, cell->getContentSize().height/2));
    cell->addChild(sprTicketBox);
    {
        auto layerScroll = ui::ScrollView::create();
        layerScroll->setBounceEnabled(false);
        layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        layerScroll->setContentSize(Size(sprTicketBox->getContentSize().width - 15, sprTicketBox->getContentSize().height - 10));
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layerScroll->setPosition(sprTicketBox->getContentSize() / 2);
        layerScroll->setSwallowTouches(false);
        sprTicketBox->addChild(layerScroll);
        
        double innerWidth = 0;
        double innerHeight = layerScroll->getContentSize().height;
        
        auto listItems = info->getListReward();
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objItem = listItems.at(i);
            
            double posX = 58 * i + 10 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 58;
            
            auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
            spriteItemBG->setContentSize(Size(58,58));
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto spriteItems = Sprite::create(objItem->getPath());
            if ( spriteItems != nullptr )
            {
                spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                spriteItems->getTexture()->setTexParameters(texParams);

                switch((E_ITEMS)objItem->getIdx())
                {
                    case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                    case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                }
                if(NewRaidManager::getInstance()->isTicket())
                {
                    if(isReceive)
                    {
                        spriteItems->setColor(Color3B(90,90,90));
                    }
                }
                spriteItemBG->addChild(spriteItems);
            }
                
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }
            auto labelCount = Label::createWithTTF(strItemsCount, GAME_FONT, 16);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 0);
            spriteItemBG->addChild(labelCount);
            if(NewRaidManager::getInstance()->isTicket())
            {
                if(isReceive)
                {
                    labelCount->enableStrikethrough();
                }
            }
        }
        if(innerWidth < layerScroll->getContentSize().width)
            layerScroll->setContentSize(Size(innerWidth, innerHeight));
        
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        if(NewRaidManager::getInstance()->isTicket())
        {
            if(isReceive)
            {
                sprTicketBox->setColor(Color3B(90,90,90));
                layerScroll->setColor(Color3B(90,90,90));
            }
        }
    }
    
    auto sprPlus = Sprite::create("Assets/ui/purchase/shop_item_plus.png");
    sprPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprPlus->setPosition(btnBg->getPosition() + Vec2:: RIGHT * (btnBg->getContentSize().width + 13));
    cell->addChild(sprPlus);
    
    
    
    return cell;
}

ssize_t PopupRaidMission::numberOfCellsInTableView(extension::TableView *table)
{
    
    return _listInfoQuest.size();
}

#pragma mark -init
bool PopupRaidMission::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _listInfoQuest = NewRaidManager::getInstance()->getQuestList();
    initVar();
    initUI();
    
    
    
    return true;
}

void PopupRaidMission::initVar()
{
    _spriteContainer->setContentSize(Size(732, 1210));
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
}

void PopupRaidMission::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_raid_quest_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupRaidMission::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //650
    Size size = _spriteContainer->getContentSize();
    _lyTopUI = Layer::create();
    _lyTopUI->setContentSize( Size(size.width - 10, 350));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(_lyTopUI);
    
    _lyMissionUI = Layer::create();
    _lyMissionUI->setContentSize( Size(size.width - 10, 650));
    _lyMissionUI->setIgnoreAnchorPointForPosition(false);
    _lyMissionUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMissionUI->setPosition(_lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyMissionUI);
    
    _lyBottomUI = Layer::create();
    _lyBottomUI->setContentSize( Size(size.width - 10, size.height - 75 - 10 - _lyTopUI->getContentSize().height - _lyMissionUI->getContentSize().height));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition(_lyMissionUI->getPosition() + Vec2::DOWN * _lyMissionUI->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    uiTop();
    uiMission();
    uiBottom();
}
#pragma mark -ui
void PopupRaidMission::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBG = Sprite::create("Assets/ui/raid/raid_mission_bg_1.png");
    sprBG->setPosition(_lyTopUI->getContentSize()/2);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(_lyTopUI->getContentSize().width/sprBG->getContentSize().width, _lyTopUI->getContentSize().height/sprBG->getContentSize().height);
    _lyTopUI->addChild(sprBG);
    
    auto sprTicketBox = Sprite::create("Assets/ui/raid/raid_mission_box_1.png");
    sprTicketBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    sprTicketBox->setPosition(_lyTopUI->getContentSize().width - 5, _lyTopUI->getContentSize().height - 5);
    _lyTopUI->addChild(sprTicketBox);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_new_raid_ticket_msg_1"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbText->setPosition(sprTicketBox->getContentSize().width/2, sprTicketBox->getContentSize().height - 10);
        lbText->setDimensions(sprTicketBox->getContentSize().width - 10, 40);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setColor(COLOR_COIN);
        sprTicketBox->addChild(lbText);
        
        
        auto btnBuyTicket = ui::Button::create("Assets/ui_common/btn_1_9_on.png");
        btnBuyTicket->setScale9Enabled(true);
        btnBuyTicket->setContentSize(Size(240,60));
        btnBuyTicket->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnBuyTicket->setPosition(Vec2(sprTicketBox->getContentSize().width/2, 5));
        btnBuyTicket->setZoomScale(0.05f);
        btnBuyTicket->setTitleText(GAME_TEXT("t_ui_go_buy_1"));
        btnBuyTicket->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        btnBuyTicket->getTitleLabel()->setDimensions(btnBuyTicket->getContentSize().width, btnBuyTicket->getContentSize().height);
        btnBuyTicket->setTitleFontSize(26);
        btnBuyTicket->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
        btnBuyTicket->addClickEventListener(CC_CALLBACK_1(PopupRaidMission::onClickTicketBuy, this));
        sprTicketBox->addChild(btnBuyTicket);
        if(NewRaidManager::getInstance()->isTicket())
        {
            btnBuyTicket->setTitleText(GAME_TEXT("t_ui_raid_tickt_noti"));
        }
        
        //rewards
        auto lyReward = LayerColor::create();
        lyReward->setContentSize( Size(sprTicketBox->getContentSize().width - 30, 60));
        lyReward->setIgnoreAnchorPointForPosition(false);
        lyReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        lyReward->setPosition(Vec2(sprTicketBox->getContentSize()/2) + Vec2::UP * 5 );
        sprTicketBox->addChild(lyReward);
        {
            auto layerScroll = ui::ScrollView::create();
            layerScroll->setBounceEnabled(false);
            layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
            layerScroll->setContentSize(lyReward->getContentSize());
            layerScroll->setIgnoreAnchorPointForPosition(false);
            layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            layerScroll->setPosition(lyReward->getContentSize()/2);
            layerScroll->setSwallowTouches(false);
            lyReward->addChild(layerScroll);
            
            double innerWidth = 0;
            double innerHeight = layerScroll->getContentSize().height;
            
            sortingTotalReward();
            for ( int i = 0; i < _listTotalReward.size(); i++ )
            {
                auto objItem = _listTotalReward.at(i);
                
                double posX = 58 * i + 10 * i;
                double posY = layerScroll->getContentSize().height / 2;
                innerWidth = posX + 58;
                
                auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
                spriteItemBG->setContentSize(Size(58,58));
                spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                spriteItemBG->setPosition(posX, posY);
                layerScroll->addChild(spriteItemBG);
                
                auto spriteItems = Sprite::create(objItem->getPath());
                if ( spriteItems != nullptr )
                {
                    spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                    spriteItems->getTexture()->setTexParameters(texParams);

                    switch((E_ITEMS)objItem->getIdx())
                    {
                        case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                        case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                    }
                    spriteItemBG->addChild(spriteItems);
                }
                
                std::string strItemsCount = objItem->getCount();
                if ( objItem != nullptr && objItem->isString() == true )
                {
                    MafUtils::convertNumberToShort(strItemsCount);
                }

                auto labelCount = Label::createWithTTF(strItemsCount, GAME_FONT, 16);
                labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 0);
                spriteItemBG->addChild(labelCount);
                
            }
            layerScroll->setContentSize(Size(innerWidth, innerHeight));
            layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        }
    }
    
    if(NewRaidManager::getInstance()->isTicket())
    {
        auto sprStemp = Sprite::create("Assets/ui/raid/icon_activate_1.png");
        sprStemp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprStemp->setPosition(sprTicketBox->getContentSize()/2);
        sprTicketBox->addChild(sprStemp);
    }
    
    auto lbInfo = Label::createWithTTF("", GAME_FONT, 24);
    lbInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbInfo->setPosition(_lyTopUI->getContentSize().width/2, 15);
    lbInfo->setDimensions(_lyTopUI->getContentSize().width - 10, 90);
    lbInfo->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
    lbInfo->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopUI->addChild(lbInfo);
    {
        auto callFunc = CallFunc::create([&, lbInfo](){
            std::string strText = GAME_TEXT("t_new_quest_raid_msg_1") + "\n";
            
            long time = NewRaidManager::getInstance()->getEndSeasonTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            strText += GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour,min);
            
            lbInfo->setString(strText);
        });
        auto delayTime = DelayTime::create(60.f);
        auto seq = Sequence::create(callFunc, delayTime, nullptr);
        lbInfo->runAction(RepeatForever::create(seq));
        
        
        
    }
}

void PopupRaidMission::uiMission()
{
    _lyMissionUI->removeAllChildren();
    
    auto sprTitleBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_3.png");
    sprTitleBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprTitleBox->setContentSize(Size(490, 40));
    sprTitleBox->setPosition(Vec2(5, _lyMissionUI->getContentSize().height - 5));
    _lyMissionUI->addChild(sprTitleBox);
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_mission_reward"), GAME_FONT, 24);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbTitle->setPosition(sprTitleBox->getContentSize()/2);
        lbTitle->setDimensions(sprTitleBox->getContentSize().width - 10, sprTitleBox->getContentSize().height - 5);
        lbTitle->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbTitle->setColor(Color3B(128,128,128));
        sprTitleBox->addChild(lbTitle);
    }
    
    auto sprTicketRewardBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_9.png");
    sprTicketRewardBox->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    sprTicketRewardBox->setContentSize(Size(178, 40));
    sprTicketRewardBox->setPosition(Vec2(_lyMissionUI->getContentSize().width - 5, _lyMissionUI->getContentSize().height - 5));
    _lyMissionUI->addChild(sprTicketRewardBox);
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_new_raid_ticket_msg_2"), GAME_FONT, 24);
        lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbTitle->setPosition(sprTicketRewardBox->getContentSize()/2);
        lbTitle->setDimensions(sprTicketRewardBox->getContentSize().width - 10, sprTicketRewardBox->getContentSize().height - 5);
        lbTitle->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbTitle->setColor(Color3B(227,199,110));
        sprTicketRewardBox->addChild(lbTitle);
    }
    
    _table = extension::TableView::create(this, Size(_lyMissionUI->getContentSize().width, _lyMissionUI->getContentSize().height - 50));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_lyMissionUI->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyMissionUI->addChild(_table);
    _table->reloadData();
}

void PopupRaidMission::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto btnAllComplete = ui::Button::create("Assets/ui_common/btn_1_9_on.png");
    btnAllComplete->setScale9Enabled(true);
    btnAllComplete->setContentSize(Size(672,_lyBottomUI->getContentSize().height - 20));
    btnAllComplete->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnAllComplete->setPosition(_lyBottomUI->getContentSize()/2);
    btnAllComplete->setZoomScale(0.05f);
    btnAllComplete->setTitleText(GAME_TEXT("t_ui_attend_receive_all"));
    btnAllComplete->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnAllComplete->getTitleLabel()->setDimensions(btnAllComplete->getContentSize().width, btnAllComplete->getContentSize().height);
    btnAllComplete->setTitleFontSize(30);
    btnAllComplete->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    btnAllComplete->addClickEventListener(CC_CALLBACK_1(PopupRaidMission::onClickAllComplete, this));
    _lyBottomUI->addChild(btnAllComplete);
    
    btnAllComplete->setEnabled(false);
    
    for(auto obj : _listInfoQuest)
    {
        if(!obj->isReceive() && (obj->getCountNow() >= obj->getCountMax()))
        {
            btnAllComplete->setEnabled(true);
            break;
        }
    }
    
    
}

#pragma mark -utils
void PopupRaidMission::onClick(Ref* sender)
{
    
}

void PopupRaidMission::onClickComplete(Ref* sender)
{
    PopupLoading::show();
    
    auto node = (Node*)sender;
    
    NewRaidManager::getInstance()->requestMissionComplete(CC_CALLBACK_2(PopupRaidMission::callbackComplete, this), node->getTag());
}

void PopupRaidMission::onClickAllComplete(Ref* sender)
{
    PopupLoading::show();
    
    NewRaidManager::getInstance()->requestMissionAllComplete(CC_CALLBACK_2(PopupRaidMission::callbackComplete, this));
}

void PopupRaidMission::onClickClose(Ref* sender)
{
    hide();
}

void PopupRaidMission::onClickTicketBuy(Ref* pSender)
{
    if(NewRaidManager::getInstance()->isTicket())
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_new_raid_ticket_pop_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto objProduct = CashManager::getInstance()->getProduct(70000000);
    if ( objProduct == nullptr )
    {
        return;
    }
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(70000000, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(PopupRaidMission::callbackPurchase, this));
            });
            popup->show();
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else
        {
            long time = NewRaidManager::getInstance()->getEndSeasonTime() - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            
            std::string strMsg = GAME_TEXT("t_ui_new_raid_ticket_pop_1") + "\n" + GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour,min);
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMsg);
            popup->addButton(GAME_TEXT("t_ui_costume_skin_msg_2"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(70000000, CC_CALLBACK_4(PopupRaidMission::callbackPurchase, this));
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
        }
    });
}

void PopupRaidMission::sortingTotalReward()
{
    _listTotalReward.clear();
    
    for(auto obj : _listInfoQuest)
    {
        auto listReward = obj->getListReward();
        for(auto item : listReward)
        {
            addTotalReward(item);
        }
    }
}
void PopupRaidMission::addTotalReward(InfoItems* infoItems)
{
    bool isInner = false;
    for(auto obj : _listTotalReward)
    {
        if(obj->getIdx() == infoItems->getIdx())
        {
            isInner = true;
            obj->setCount(std::atoi(obj->getCount().c_str()) + std::atoi(infoItems->getCount().c_str()));
        }
    }
    
    if(!isInner)
    {
        auto item = infoItems->clone();
        item->setCount(infoItems->getCount());
        _listTotalReward.pushBack(item);
    }
    
}
void PopupRaidMission::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}

#pragma mark -callback
void PopupRaidMission::callbackComplete(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if(nResult == 3)
            strMessageKey = "t_ui_error_31";
        else if( nResult == 8 )
        {
            strMessageKey = "t_ui_new_raid_error_1";
        }
        else if( nResult == 9 )
        {
            strMessageKey = "t_ui_new_raid_error_2";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        if(nResult == 8 || nResult == 9)
        {
            if(_callbackHide != nullptr)
                _callbackHide();
        }
        
        hide();
        return;
    }
    
    _table->reloadData();
    uiBottom();
}


#pragma mark - Purchase
void PopupRaidMission::callbackProductOk(int nIdx)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(nIdx, CC_CALLBACK_4(PopupRaidMission::callbackPurchase, this));
}

void PopupRaidMission::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestInfo([=](bool){
            PopupLoading::hide();
            RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
        });
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        PopupLoading::show();
        NewRaidManager::getInstance()->requestInfo([=](bool, int){
            PopupLoading::hide();
            _table->reloadData();
            uiBottom();
            uiTop();
            RefreshManager::getInstance()->refreshUI(E_REFRESH::RAID_ALL);
        });
        
        // alert
        std::string strText = GAME_TEXT("t_ui_cash_msg_1");
        if(nIdx == 70000000)
            strText = GAME_TEXT("t_ui_new_raid_ticket_pop_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::CANCEL && nResult != -1 )
    {
        /*
         0 # 더이상 구매 불가능
         1 # 성공
         2 #이벤트 중이 아닙니다. ( 복귀자 )
         3 # 판매 시작한 상품이 아닙니다.
         4 # 기간별 판매이면서 종료시간이 없음 # 주간판매 or 일간판매이면서 간격정보없음
         5 # 샵데이터가 없습니다.
         6 # 판매 종료된 상품입니다.
         7 # 조건이 맞지 않습니다. (뿌리링 선행 상품구매 x)
         8 # 서버 점검중..
         */
        std::string strMessageKey = "t_ui_error_30";
        switch (nResult) {
            case 0: strMessageKey = "t_ui_cash_msg_9_0";    break;
            case 2: strMessageKey = "t_ui_cash_msg_9_2";    break;
            case 3: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 6: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 8: strMessageKey = "t_ui_cash_msg_9_8";    break;
                
            default:
                break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
}

