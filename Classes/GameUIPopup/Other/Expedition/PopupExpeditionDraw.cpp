//
//  PopupExpeditionDraw.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupExpeditionDraw.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/ExpeditionManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupExpeditionDraw* PopupExpeditionDraw::create()
{
    PopupExpeditionDraw* pRet = new(std::nothrow) PopupExpeditionDraw();
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

PopupExpeditionDraw::PopupExpeditionDraw(void) :
_strType("FRIEND_POINT"),
_nSelectIdx(-1),

_layerContainer(nullptr),
_layerContainerDate(nullptr),
_layerContainerCurrency(nullptr),
_layerContainerList(nullptr),
_layerContainerBottom(nullptr),
_layerContainerBonus(nullptr),
_spriteBonus(nullptr),
_tableList(nullptr)
{
    
}

PopupExpeditionDraw::~PopupExpeditionDraw(void)
{
    
}

bool PopupExpeditionDraw::init()
{
    if( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // touch
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        
        if ( _layerContainer == nullptr )
        {
            return;
        }
        
        cocos2d::Rect rect = _layerContainer->getBoundingBox();
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())) == false)
        {
            hide();
        }
        
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //
    initVar();
    initUi();
    
    
    return true;
}
void PopupExpeditionDraw::onEnter()
{
    PopupBase::onEnter();
    
    requestInfo();
    
}
#pragma mark - table
void PopupExpeditionDraw::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size PopupExpeditionDraw::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 88 + 10);
}

extension::TableViewCell* PopupExpeditionDraw::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    
    //
    auto list = ExpeditionManager::getInstance()->getDrawRewardAll();
    
    // obj
    ssize_t nIdxStart = idx * 6;
    ssize_t nIdxEnd = nIdxStart + 6;
    if ( nIdxEnd > list.size() )
    {
        nIdxEnd = list.size();
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdxDraw = (int)nIdxStart + i;
        auto obj = list.at(nIdxDraw);
        if ( obj == nullptr )
        {
            continue;
        }
        
        double nPosX = 42 + 84 * i + 10 * i;
        double nPosY = size.height / 2;
     
        if ( (E_ITEMS)obj->getIdx() == E_ITEMS::NONE )
        {
            auto spriteBG = Sprite::create("Assets/ui/expedition/draw_btn_1_2.png");
            spriteBG->setPosition(nPosX, nPosY);
            cell->addChild(spriteBG);
            
            auto item = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/expedition/draw_btn_1_2.png"), nullptr, CC_CALLBACK_1(PopupExpeditionDraw::onClickDraw, this));
            item->setPosition(nPosX, nPosY);
            item->setTag(nIdxDraw);
            menu->addChild(item);
        }
        else
        {
            auto spriteBG = Sprite::create("Assets/ui/expedition/draw_btn_1_1.png");
            spriteBG->setPosition(nPosX, nPosY);
            cell->addChild(spriteBG);
            
            if ( _nSelectIdx == nIdxDraw )
            {
                _nSelectIdx = -1;
                auto spriteEffect = Sprite::create("Assets/ui/expedition/draw_btn_1_3.png");
                spriteEffect->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2);
                spriteEffect->setOpacity(0);
                spriteBG->addChild(spriteEffect);
                
                auto action1_1 = FadeIn::create(0.5);
                auto action1_2 = FadeTo::create(0.5, 20);
                auto seq1 = Sequence::create(action1_1, action1_2, NULL);
                spriteEffect->runAction(RepeatForever::create(seq1));
                
                auto seq2 = Sequence::create(DelayTime::create(4.0), RemoveSelf::create(), NULL);
                spriteEffect->runAction(seq2);
            }
            
            auto spriteIcon = Sprite::create(obj->getPath());
            spriteIcon->setPosition(spriteBG->getContentSize().width / 2, 55);
            spriteBG->addChild(spriteIcon);
            
            auto labelCount = Label::createWithTTF(MafUtils::toString(obj->getCount()), GAME_FONT, 18);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(spriteBG->getContentSize().width / 2, 15);
            labelCount->setColor(Color3B::BLACK);
            spriteBG->addChild(labelCount);
        }
    }
    
    return cell;
}

ssize_t PopupExpeditionDraw::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    auto list = ExpeditionManager::getInstance()->getDrawRewardAll();
    
    ssize_t size = list.size() / 6;
    if ( list.size() % 6 != 0 )
        size++;
    
    return size;
}

#pragma mark - init
void PopupExpeditionDraw::initVar()
{
    
}

void PopupExpeditionDraw::initUi()
{
    _layerContainer = Layer::create();
    _layerContainer->setContentSize( Size(680, 992));
    _layerContainer->setIgnoreAnchorPointForPosition(false);
    _layerContainer->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _layerContainer->setPosition( getContentSize().width / 2, getContentSize().height / 2 );
    addChild(_layerContainer);
    
    //
    auto spriteBG = Sprite::create("Assets/ui/expedition/draw_box_01.png");
    spriteBG->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2);
    _layerContainer->addChild(spriteBG);
    {
        auto spriteEffect1 = Sprite::create("Assets/ui/expedition/draw_box_ef_1.png");
        spriteEffect1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteEffect1->setPosition(0, spriteBG->getContentSize().height - 56);
        spriteBG->addChild(spriteEffect1);
        {
            auto animationRun = Animation::create();
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_1.png");
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_2.png");
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_3.png");
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_4.png");
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_5.png");
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_6.png");
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_7.png");
            animationRun->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_ef_8.png");
            animationRun->setDelayPerUnit(0.08f);
            
            auto animate = Animate::create(animationRun);
            spriteEffect1->runAction(RepeatForever::create(animate));
        }
        
        auto spriteEffect2 = Sprite::create("Assets/ui/expedition/draw_box_01_1.png");
        spriteEffect2->setPosition(spriteBG->getContentSize().width - spriteEffect2->getContentSize().width / 2, spriteBG->getContentSize().height - spriteEffect2->getContentSize().height / 2);
        spriteBG->addChild(spriteEffect2, -1);
        {
            spriteEffect2->runAction(RepeatForever::create(RotateBy::create(1.5, 180)));
        }
    }
 
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_draw_ver2"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 50);
    spriteBG->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupExpeditionDraw::onClickClose, this));
    itemClose->setPosition(spriteBG->getContentSize().width - 155 - itemClose->getContentSize().width, spriteBG->getContentSize().height - 50);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerDate = Layer::create();
    _layerContainerDate->setContentSize( Size(347, 116) );
    _layerContainerDate->setIgnoreAnchorPointForPosition(false);
    _layerContainerDate->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerDate->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 88);
    _layerContainerDate->setOpacity(150);
    spriteBG->addChild(_layerContainerDate);
    
    _layerContainerCurrency = Layer::create();
    _layerContainerCurrency->setContentSize( Size(605, 40) );
    _layerContainerCurrency->setIgnoreAnchorPointForPosition(false);
    _layerContainerCurrency->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerCurrency->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 220);
    spriteBG->addChild(_layerContainerCurrency);
    
    _layerContainerBonus = Layer::create();
    _layerContainerBonus->setContentSize( Size(spriteBG->getContentSize().width - 48 - 50, 105) );
    _layerContainerBonus->setIgnoreAnchorPointForPosition(false);
    _layerContainerBonus->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBonus->setPosition(spriteBG->getContentSize().width / 2, 123);
    spriteBG->addChild(_layerContainerBonus);
    
    _layerContainerList = Layer::create();
    _layerContainerList->setContentSize( Size(580, 552) );
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerList->setPosition(spriteBG->getContentSize().width / 2, 228);
    spriteBG->addChild(_layerContainerList);
    
    _layerContainerBottom = Layer::create();
    _layerContainerBottom->setContentSize( Size(spriteBG->getContentSize().width - 48 - 50, 98) );
    _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
    _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBottom->setPosition(spriteBG->getContentSize().width / 2, 25);
    spriteBG->addChild(_layerContainerBottom);
    
    //
    uiDate();
    uiCurrency();
    uiList();
    uiBottom();
   
}
#pragma mark - ui
void PopupExpeditionDraw::uiDate()
{
    _layerContainerDate->removeAllChildren();
    
    //
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_draw_msg_4"), GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelText->setPosition(_layerContainerDate->getContentSize().width / 2, _layerContainerDate->getContentSize().height);
    labelText->setDimensions(_layerContainerDate->getContentSize().width, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerDate->addChild(labelText);
    
    auto labelTextDay = Label::createWithTTF("DAY", GAME_FONT, 16);
    labelTextDay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelTextDay->setPosition(-30, 60);
    labelTextDay->setColor(Color3B::BLACK);
    _layerContainerDate->addChild(labelTextDay);
    
    {
        auto labelDay = Label::createWithTTF("0", GAME_FONT, 16);
        labelDay->setPosition(-30, 22);
        labelDay->setColor(Color3B::BLACK);
        labelDay->setName("D1");
        _layerContainerDate->addChild(labelDay);
    }
    
    {
        auto labelHour1 = Label::createWithTTF("0", GAME_FONT, 28);
        labelHour1->setPosition(35, 45);
        labelHour1->setColor(Color3B::BLACK);
        labelHour1->setName("H1");
        _layerContainerDate->addChild(labelHour1);
        
        auto labelHour2 = Label::createWithTTF("0", GAME_FONT, 28);
        labelHour2->setPosition(88, 45);
        labelHour2->setColor(Color3B::BLACK);
        labelHour2->setName("H2");
        _layerContainerDate->addChild(labelHour2);
    }
    
    {
        auto labelMin1 = Label::createWithTTF("0", GAME_FONT, 28);
        labelMin1->setPosition(148, 45);
        labelMin1->setColor(Color3B::BLACK);
        labelMin1->setName("M1");
        _layerContainerDate->addChild(labelMin1);
        
        auto labelMin2 = Label::createWithTTF("0", GAME_FONT, 28);
        labelMin2->setPosition(200, 45);
        labelMin2->setColor(Color3B::BLACK);
        labelMin2->setName("M2");
        _layerContainerDate->addChild(labelMin2);
    }
    
    
    {
        auto labelSec1 = Label::createWithTTF("0", GAME_FONT, 28);
        labelSec1->setPosition(260, 45);
        labelSec1->setColor(Color3B::BLACK);
        labelSec1->setName("S1");
        _layerContainerDate->addChild(labelSec1);
        
        auto labelSec2 = Label::createWithTTF("0", GAME_FONT, 28);
        labelSec2->setPosition(310, 45);
        labelSec2->setColor(Color3B::BLACK);
        labelSec2->setName("S2");
        _layerContainerDate->addChild(labelSec2);
    }
    
    
    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
        
        auto labelDay = (Label*)_layerContainerDate->getChildByName("D1");
        auto labelHour1 = (Label*)_layerContainerDate->getChildByName("H1");
        auto labelHour2 = (Label*)_layerContainerDate->getChildByName("H2");
        auto labelMin1 = (Label*)_layerContainerDate->getChildByName("M1");
        auto labelMin2 = (Label*)_layerContainerDate->getChildByName("M2");
        auto labelSec1 = (Label*)_layerContainerDate->getChildByName("S1");
        auto labelSec2 = (Label*)_layerContainerDate->getChildByName("S2");
        
        //
        int nTime = (int)(ExpeditionManager::getInstance()->getDrawEndTime() - UtilsDate::getInstance()->getTime());
        if ( nTime < 0 )
        {
            nTime = 0;
            labelDay->setString("0");
            labelHour1->setString("0");
            labelHour2->setString("0");
            labelMin1->setString("0");
            labelMin2->setString("0");
            labelSec1->setString("0");
            labelSec2->setString("0");
            _layerContainerDate->stopAllActions();
            return;
        }
        
        int nDay = nTime / 86400;
        int nHour = nTime % 86400 / 3600;
        int nMin = nTime % 3600 / 60;
        int nSec = nTime % 60;
        
        labelDay->setString(MafUtils::toString(nDay));
        labelHour1->setString(MafUtils::toString(nHour/10));
        labelHour2->setString(MafUtils::toString(nHour%10));
        labelMin1->setString(MafUtils::toString(nMin/10));
        labelMin2->setString(MafUtils::toString(nMin%10));
        labelSec1->setString(MafUtils::toString(nSec/10));
        labelSec2->setString(MafUtils::toString(nSec%10));
    });
    auto action2 = DelayTime::create(1.0);
    auto seq = Sequence::create(action1, action2, nullptr);
    _layerContainerDate->runAction( RepeatForever::create(seq) );
}

void PopupExpeditionDraw::uiCurrency()
{
    _layerContainerCurrency->removeAllChildren();
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::FRIEND_POINT);
    
    auto uiCurrency = UICurrency::create(list, Size(300, _layerContainerCurrency->getContentSize().height));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiCurrency->setPosition(Vec2(0, _layerContainerCurrency->getContentSize().height / 2));
    _layerContainerCurrency->addChild(uiCurrency);
}
 
void PopupExpeditionDraw::uiList()
{
    //
    _layerContainerList->removeAllChildren();
    
    //
    _tableList = extension::TableView::create(this, Size(504 + 50, 440 + 50));
    _tableList->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableList->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _tableList->setIgnoreAnchorPointForPosition(false);
    _tableList->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _tableList->setPosition(_layerContainerList->getContentSize().width / 2, _layerContainerList->getContentSize().height / 2);
    _tableList->setDelegate(this);
    _tableList->setBounceable(false);
    _layerContainerList->addChild(_tableList);
}

void PopupExpeditionDraw::uiBottom()
{
    //
    _layerContainerBottom->removeAllChildren();
    
    //
    bool bBonus = ExpeditionManager::getInstance()->isDrawBounus();
    if ( bBonus == false )
    {
        _layerContainerBottom->setContentSize(Size(_layerContainerBottom->getContentSize().width, 200));
    }
    else
    {
        _layerContainerBottom->setContentSize(Size(_layerContainerBottom->getContentSize().width, 98));
    }
    
    
    auto spriteIcon = Sprite::create("Assets/icon/icon_friendship.png");
    spriteIcon->setPosition(_layerContainerBottom->getContentSize()/2);
    spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _layerContainerBottom->addChild(spriteIcon);
    
    auto labelText = Label::createWithTTF("X 30", GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labelText->setPosition(_layerContainerBottom->getContentSize()/2);
    _layerContainerBottom->addChild(labelText);
}

void PopupExpeditionDraw::uiBonus()
{
    //
    _layerContainerBonus->removeAllChildren();
    
    //
    bool bBonus = ExpeditionManager::getInstance()->isDrawBounus();
    if ( bBonus == false )
    {
        return;
    }
    

    //
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerBonus->addChild(menu);
    
    //
    double listPos[] = {100, 290, 480};
    int listBonusIdx[][10] = {
        {0,1,    6,7,    12,13,  18,19,  24,25},
        {2,3,    8,9,    14,15,  20,21,  26,27},
        {4,5,    10,11,  16,17,  22,23,  28,29}
    };

    auto listDraw = ExpeditionManager::getInstance()->getDrawRewardAll();
    auto listDrawBonus = ExpeditionManager::getInstance()->getDrawRewardBonusAll();
    for ( int i = 0; i < sizeof(listPos)/sizeof(double); i++ )
    {
        auto objDrawBonus = listDrawBonus.at(i);
        bool bBonusEnable = true;
        
        for ( int j = 0; j < sizeof(listBonusIdx[i])/sizeof(int); j++ )
        {
            auto objReward = listDraw.at(listBonusIdx[i][j]);
            if ( (E_ITEMS)objReward->getIdx() == E_ITEMS::NONE )
            {
                bBonusEnable = false;
                break;
            }
        }

        auto spriteBonus = Sprite::create("Assets/ui/expedition/draw_box_01_2.png");
        spriteBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteBonus->setPosition(listPos[i], _layerContainerBonus->getContentSize().height);
        _layerContainerBonus->addChild(spriteBonus);
        
        auto spriteBonusIcon = Sprite::create("Assets/ui/expedition/draw_box_01_3.png");
        spriteBonusIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteBonusIcon->setPosition(listPos[i], _layerContainerBonus->getContentSize().height);
        _layerContainerBonus->addChild(spriteBonusIcon, -1);
        
        //
        auto itemBonus = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/expedition/draw_box_s1_1.png"), nullptr, CC_CALLBACK_1(PopupExpeditionDraw::onClickDrawBonus, this));
        itemBonus->setPosition(listPos[i], itemBonus->getContentSize().height / 2);
        itemBonus->setTag(i);
        itemBonus->setEnabled(bBonusEnable);
        menu->addChild(itemBonus);
        if ( (E_ITEMS)objDrawBonus->getIdx() != E_ITEMS::NONE )
        {
            ((Sprite*)itemBonus->getNormalImage())->setTexture("Assets/ui/expedition/draw_box_s2_8.png");
            itemBonus->setEnabled(false);
            
            auto spriteIcon = Sprite::create(objDrawBonus->getPath());
            spriteIcon->setPosition(itemBonus->getContentSize().width / 2, itemBonus->getContentSize().height / 2);
            itemBonus->addChild(spriteIcon);
        }
        else if ( bBonusEnable == true )
        {
            ((Sprite*)itemBonus->getNormalImage())->setTexture("Assets/ui/expedition/draw_box_s2_1.png");
        }
    }
}

#pragma mark - draw
void PopupExpeditionDraw::drawTable()
{
    Point offset = _tableList->getContentOffset();
    _tableList->reloadData();
    if ( offset.y <= 0 )
    {
        _tableList->setContentOffset(offset);
    }
}

#pragma mark - click
void PopupExpeditionDraw::onClickClose(Ref* sender)
{
    hide();
}

void PopupExpeditionDraw::onClickDraw(Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    int nIdx = item->getTag();
    
    std::string strMessage = "";
    
    if ( ExpeditionManager::getInstance()->getDrawSwapFriendPoint() == 0 )
    {
        strMessage = GAME_TEXT("t_ui_draw_msg_2");
    }
    else if ( atoi(ItemsManager::getInstance()->getItems(E_ITEMS::FRIEND_POINT).c_str()) < 30 )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::FRIEND_POINT);
        popup->show();
        return;
    }
    
    if ( strMessage.length() != 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    _nSelectIdx = nIdx;
    
    //
    PopupLoading::show();
    ExpeditionManager::getInstance()->requestDrawUpdate(CC_CALLBACK_2(PopupExpeditionDraw::callbackDraw, this), _strType, nIdx);
}

void PopupExpeditionDraw::onClickDrawBonus(Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    int nIdx = item->getTag();
    
    _spriteBonus = (Sprite*)item->getNormalImage();
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.15);
    for ( int i = 2; i <= 5; i++ )
    {
        animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/expedition/draw_box_s2_%d.png",i));
    }
    _spriteBonus->runAction(RepeatForever::create(Animate::create(animation)));
    

    //
    PopupLoadingLock::show();
    auto action1 = DelayTime::create(1.0);
    auto action2 = CallFunc::create([=](){

        ExpeditionManager::getInstance()->requestDrawBonus(CC_CALLBACK_2(PopupExpeditionDraw::callbackDrawBonus, this), nIdx);
    });
    auto seq = Sequence::create(action1, action2, NULL);
    runAction(seq);
}

#pragma mark - network
void PopupExpeditionDraw::requestInfo()
{
    PopupLoading::show();
    ExpeditionManager::getInstance()->requestDrawInfo(CC_CALLBACK_2(PopupExpeditionDraw::callbackInfo, this));
}

#pragma mark - callback
void PopupExpeditionDraw::callbackInfo(bool bResult, int nResult)
{
    //

    //
    if ( bResult == false )
    {
        PopupLoading::hide();
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_draw_msg_1");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    //
    uiDate();
    uiBottom();
    uiBonus();
    drawTable();
    PopupLoading::hide();
}

void PopupExpeditionDraw::callbackDraw(bool bResult, int nResult)
{
    //
    PopupLoading::hide();

    //
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )             strMessage = GAME_TEXT("t_ui_error_31");
        else if ( nResult == 2 )        strMessage = GAME_TEXT("t_ui_draw_msg_1");
        else if ( nResult == 3 )        strMessage = GAME_TEXT("t_ui_draw_msg_3");
        else if ( nResult == 8 )        strMessage = GAME_TEXT("t_ui_draw_msg_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    //
    requestInfo();
}

void PopupExpeditionDraw::callbackDrawBonus(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        //
        PopupLoadingLock::hide();
        
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )             strMessage = GAME_TEXT("t_ui_error_31");
        else if ( nResult == 2 )        strMessage = GAME_TEXT("t_ui_draw_msg_1");
        else if ( nResult == 3 )        strMessage = GAME_TEXT("t_ui_draw_msg_3");
        else if ( nResult == 8 )        strMessage = GAME_TEXT("t_ui_draw_msg_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.07f);
    animation->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_s2_6.png");
    animation->addSpriteFrameWithFile("Assets/ui/expedition/draw_box_s2_7.png");
    
    auto action1 = DelayTime::create(0.14);
    auto action2 = CallFunc::create([=](){
        //
        PopupLoadingLock::hide();
        
        //
        requestInfo();
    });
    auto seq = Sequence::create(action1, action2, NULL);

    _spriteBonus->stopAllActions();
    _spriteBonus->runAction(Animate::create(animation));
    _spriteBonus->runAction(seq);
}
