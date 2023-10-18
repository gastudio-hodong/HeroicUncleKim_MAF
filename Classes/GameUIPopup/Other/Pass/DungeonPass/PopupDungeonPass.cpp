//
//  PopupDungeonPass.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/15.
//

#include "PopupDungeonPass.h"

#include "Common/ConfigGameNormal.h"
#include "Common/ConfigKey.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUIPopup/Other/Pass/DungeonPass/PopupPurchaseDungeonPass.h"
#include "GameUIPopup/Other/Pass/DungeonPass/PopupDungeonPassBadge.h"
#include "GameUIPopup/Other/Mission/PopupMissionDefault.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

PopupDungeonPass* PopupDungeonPass::create()
{
    PopupDungeonPass* pRet = new(std::nothrow) PopupDungeonPass();
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

PopupDungeonPass::PopupDungeonPass(void) :
_objPass(nullptr),
_bListPosInit(false),

//
_layerContainerContents(nullptr),
_layerContainerList(nullptr),
_layerContainerBadge(nullptr),

// widget other
_progress(nullptr),
_labelProgressTitle(nullptr),
_labelProgressLv(nullptr),
_labelProgressRate(nullptr),

_itemPurchase(nullptr),
_labelPurchase(nullptr),
_labelTime(nullptr),

_layerBuff(nullptr),

_spriteBadgeFreeIconBG(nullptr),
_spriteBadgePurIconBG(nullptr),

// table
_tableList(nullptr)
{
    
}

PopupDungeonPass::~PopupDungeonPass(void)
{
    if ( _objPass != nullptr )
    {
        _objPass->release();
        _objPass = nullptr;
    }
}

bool PopupDungeonPass::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    initVar();
    initUI();

    return true;
}

void PopupDungeonPass::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    PassManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupDungeonPass::callbackPassInfo, this));
}

#pragma mark - table
void PopupDungeonPass::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
     
}

void PopupDungeonPass::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size PopupDungeonPass::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 116);
}

extension::TableViewCell* PopupDungeonPass::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    
    if ( _objPass == nullptr )
    {
        return cell;
    }

    auto objStep = _listPassStep.at(idx);
    
    int nLevel = _objPass->getLevel();
    
    int nPoint = _objPass->getPoint();
    int nStepUpCost = objStep->getStepUpCost();
    
    bool bBuyPass = _objPass->isBuyPass();
    
    //
    auto layerBG = Layer::create();
    layerBG->setContentSize(Size(size.width, 113));
    layerBG->setIgnoreAnchorPointForPosition(false);
    layerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layerBG->setPosition(size.width / 2, size.height / 2);
    cell->addChild(layerBG);
    

    //
    std::string strFreePath = "Assets/ui/purchase/pass_bar_1_2.png";
    if ( nPoint >= objStep->getPoint() )
    {
        if(objStep->isRewardFree() == true )
            strFreePath = "Assets/ui/purchase/pass_bar_1_3.png";
        else
            strFreePath = "Assets/ui/purchase/pass_bar_1_1.png";
    }
    
    auto spriteFreeBG = Sprite::create(strFreePath);
    spriteFreeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    spriteFreeBG->setPosition(0, layerBG->getContentSize().height / 2);
    layerBG->addChild(spriteFreeBG);
    {
        auto objItems = ItemsManager::getInstance()->getInfoItems(objStep->getRewardFreeIdx());
        if ( objItems != nullptr )
        {
            objItems->setTypeParams(objStep->getRewardFreeTypeParams());
            objItems->setCount(objStep->getRewardFreeCount());
        }
        
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        spriteFreeBG->addChild(menu);
        
        auto spriteItemsBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
        spriteItemsBG->setContentSize(Size(82, 82));
        spriteItemsBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteItemsBG->setPosition(25, spriteFreeBG->getContentSize().height / 2);
        spriteFreeBG->addChild(spriteItemsBG);
        if ( objItems != nullptr )
        {
            auto spriteIcon = Sprite::create(objItems->getPath());
            if(spriteIcon == nullptr)
            {
                auto lbNone = Label::createWithTTF(GAME_TEXT("t_server_dailymission_title_update"), GAME_FONT, 20);
                lbNone->setPosition(spriteItemsBG->getContentSize()/2);
                lbNone->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lbNone->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                lbNone->setDimensions(spriteItemsBG->getContentSize().width - 5, spriteItemsBG->getContentSize().height - 5);
                lbNone->setOverflow(Label::Overflow::SHRINK);
                spriteItemsBG->addChild(lbNone);
            }
            else
            {
                spriteIcon->setPosition(spriteItemsBG->getContentSize()/2);
                spriteIcon->setScale(1.25f);
                spriteIcon->getTexture()->setTexParameters(texParams);
                spriteItemsBG->addChild(spriteIcon);
                
                auto labelCount = Label::createWithTTF(objItems->getCount(), GAME_FONT, 20);
                labelCount->setPosition(spriteItemsBG->getContentSize().width/2, 0);
                spriteItemsBG->addChild(labelCount);
                
                auto spriteAccquire = Sprite::create("Assets/ui/purchase/pass_check.png");
                spriteAccquire->setPosition(spriteItemsBG->getContentSize()/2);
                spriteAccquire->setVisible(false);
                spriteItemsBG->addChild(spriteAccquire);
                if ( nPoint >= objStep->getPoint() &&  objStep->isRewardFree() == true )
                {
                    spriteAccquire->setVisible(true);
                }
            }
        }
        
        auto labelAccquire = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_accquired"), GAME_FONT, 35);
        labelAccquire->setPosition(220, spriteFreeBG->getContentSize().height / 2);
        labelAccquire->setDimensions(180, labelAccquire->getContentSize().height);
        labelAccquire->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelAccquire->setOverflow(Label::Overflow::SHRINK);
        labelAccquire->setColor(Color3B(128,128,128));
        labelAccquire->setVisible(false);
        spriteFreeBG->addChild(labelAccquire);
        if ( nPoint >= objStep->getPoint() &&  objStep->isRewardFree() == true )
        {
            labelAccquire->setVisible(true);
        }
        
        auto itemReward = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/purchase/pass_bar_btn1_1.png"), Sprite::create("Assets/ui/purchase/pass_bar_btn1_1.png"), nullptr, CC_CALLBACK_1(PopupDungeonPass::onClickReward, this));
        itemReward->setPosition(220, spriteFreeBG->getContentSize().height / 2);
        itemReward->setName("FREE");
        itemReward->setTag((int)idx);
        itemReward->setVisible(false);
        menu->addChild(itemReward);
        if ( nPoint >= objStep->getPoint() &&  objStep->isRewardFree() == false )
        {
            itemReward->setVisible(true);
        }
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_bnt_accquire"), GAME_FONT, 35);
            labelText->setPosition(itemReward->getContentSize()/2);
            labelText->setDimensions(itemReward->getContentSize().width - 20, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            itemReward->addChild(labelText);
        }
    }
    
    //
    std::string strPurPath = "Assets/ui/purchase/pass_bar_2_2.png";
    if ( nPoint >= objStep->getPoint() )
    {
        if( bBuyPass == true )
        {
            if ( objStep->isRewardPur() == true )
                strPurPath = "Assets/ui/purchase/pass_bar_2_3.png";
            else
                strPurPath = "Assets/ui/purchase/pass_bar_2_1.png";
        }
    }
    auto spritePurBG = Sprite::create(strPurPath);
    spritePurBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spritePurBG->setPosition(layerBG->getContentSize().width, layerBG->getContentSize().height / 2);
    layerBG->addChild(spritePurBG);
    {
        auto objItems = ItemsManager::getInstance()->getInfoItems(objStep->getRewardPurIdx());
        if ( objItems != nullptr )
        {
            objItems->setTypeParams(objStep->getRewardPurTypeParams());
            objItems->setCount(objStep->getRewardPurCount());
        }
        
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        spritePurBG->addChild(menu);
        
        auto spriteItemsBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
        spriteItemsBG->setContentSize(Size(82, 82));
        spriteItemsBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteItemsBG->setPosition(spritePurBG->getContentSize().width - 25, spritePurBG->getContentSize().height / 2);
        spritePurBG->addChild(spriteItemsBG);
        if ( objItems != nullptr )
        {
            //예외처리 구간
            auto spriteIcon = Sprite::create(objItems->getPath());
            if(spriteIcon == nullptr)
            {
                auto lbNone = Label::createWithTTF(GAME_TEXT("t_server_dailymission_title_update"), GAME_FONT, 20);
                lbNone->setPosition(spriteItemsBG->getContentSize()/2);
                lbNone->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lbNone->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                lbNone->setDimensions(spriteItemsBG->getContentSize().width - 5, spriteItemsBG->getContentSize().height - 5);
                lbNone->setOverflow(Label::Overflow::SHRINK);
                spriteItemsBG->addChild(lbNone);
            }
            else
            {
                spriteIcon->setPosition(spriteItemsBG->getContentSize()/2);
                spriteIcon->setScale(1.25f);
                spriteIcon->getTexture()->setTexParameters(texParams);
                spriteItemsBG->addChild(spriteIcon);
                
                auto labelCount = Label::createWithTTF(objItems->getCount(), GAME_FONT, 20);
                labelCount->setPosition(spriteItemsBG->getContentSize().width/2, 0);
                spriteItemsBG->addChild(labelCount);
                
                auto spriteAccquire = Sprite::create("Assets/ui/purchase/pass_check.png");
                spriteAccquire->setPosition(spriteItemsBG->getContentSize()/2);
                spriteAccquire->setVisible(false);
                spriteItemsBG->addChild(spriteAccquire);
                if ( nPoint >= objStep->getPoint() &&  objStep->isRewardPur() == true )
                {
                    spriteAccquire->setVisible(true);
                }
            }
        }
        
        auto labelAccquire = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_accquired"), GAME_FONT, 35);
        labelAccquire->setPosition(spritePurBG->getContentSize().width - 220, spritePurBG->getContentSize().height / 2);
        labelAccquire->setDimensions(180, labelAccquire->getContentSize().height);
        labelAccquire->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelAccquire->setOverflow(Label::Overflow::SHRINK);
        labelAccquire->setColor(Color3B(128,128,128));
        labelAccquire->setVisible(false);
        spritePurBG->addChild(labelAccquire);
        if ( nPoint >= objStep->getPoint() &&  objStep->isRewardPur() == true )
        {
            labelAccquire->setVisible(true);
        }
        
        auto itemReward = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/purchase/pass_bar_btn1_1.png"), Sprite::create("Assets/ui/purchase/pass_bar_btn1_1.png"), nullptr, CC_CALLBACK_1(PopupDungeonPass::onClickReward, this));
        itemReward->setPosition(spritePurBG->getContentSize().width - 220, spritePurBG->getContentSize().height / 2);
        itemReward->setName("PUR");
        itemReward->setTag((int)idx);
        itemReward->setVisible(false);
        menu->addChild(itemReward);
        if ( bBuyPass == true && nPoint >= objStep->getPoint() &&  objStep->isRewardPur() == false )
        {
            itemReward->setVisible(true);
        }
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_bnt_accquire"), GAME_FONT, 35);
            labelText->setPosition(itemReward->getContentSize()/2);
            labelText->setDimensions(itemReward->getContentSize().width - 20, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            itemReward->addChild(labelText);
        }
    }
    
    
    //
    std::string strArrowPath = "Assets/ui/purchase/pass_step1_1.png";
    std::string strArrowBarPath1 = "Assets/ui/purchase/pass_step2_1.png";
    std::string strArrowBarPath2 = "Assets/ui/purchase/pass_step2_1.png";
    
    if ( nPoint >= objStep->getPoint() )
    {
        strArrowPath = "Assets/ui/purchase/pass_step1_2.png";
        if ( objStep->getIdx() != 1 )
        {
            strArrowBarPath1 = "Assets/ui/purchase/pass_step2_2.png";
        }
        if ( objStep->getIdx() < nLevel )
        {
            strArrowBarPath2 = "Assets/ui/purchase/pass_step2_2.png";
        }
    }
    auto spriteArrow = Sprite::create(strArrowPath);
    spriteArrow->setPosition(layerBG->getContentSize().width / 2, layerBG->getContentSize().height / 2);
    layerBG->addChild(spriteArrow, 10);
    {
        auto labelText = Label::createWithTTF(MafUtils::format("Lv.%d", objStep->getIdx()), GAME_FONT, 20);
        labelText->setPosition(spriteArrow->getContentSize()/2);
        spriteArrow->addChild(labelText);
        
        auto spriteArrowBar1 = Sprite::create(strArrowBarPath1);
        spriteArrowBar1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteArrowBar1->setPosition(layerBG->getContentSize().width / 2, layerBG->getContentSize().height / 2);
        layerBG->addChild(spriteArrowBar1, 9);
        
        auto spriteArrowBar2 = Sprite::create(strArrowBarPath2);
        spriteArrowBar2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteArrowBar2->setPosition(layerBG->getContentSize().width / 2, layerBG->getContentSize().height / 2);
        layerBG->addChild(spriteArrowBar2, 9);
    }
    
    if ( nLevel + 1 == objStep->getIdx() && nStepUpCost > 0 )
    {
        auto uiStepUp = ui::Button::create("Assets/ui/purchase/pass_step1_3.png");
        uiStepUp->setPosition(Vec2(layerBG->getContentSize().width / 2, layerBG->getContentSize().height / 2));
        uiStepUp->addClickEventListener(CC_CALLBACK_1(PopupDungeonPass::onClickStepUp, this));
        uiStepUp->setTag(idx);
        layerBG->addChild(uiStepUp, 11);
        
        auto uiStepText = ui::CText::create(MafUtils::toString(nStepUpCost), GAME_FONT, 24);
        uiStepText->setPosition(Vec2(uiStepUp->getContentSize().width / 2, uiStepUp->getContentSize().height / 2));
        uiStepUp->addChild(uiStepText);
    }
    
    
    return cell;
}

ssize_t PopupDungeonPass::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return _listPassStep.size();
}

#pragma mark - PopupDungeonPass : init
void PopupDungeonPass::initVar()
{
    _objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( _objPass != nullptr )
    {
        _objPass->retain();
        
        //
        _listPassStep.clear();
        _listPassStep = _objPass->getListStep();
    }
    
}

void PopupDungeonPass::initUI()
{
    Size size = Size(734, 1088);
    
    //
    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 25 + 113);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDungeonPass::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    // 372
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(_spriteContainer->getContentSize().width - 38, 462) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerContents);
    
    _layerContainerList = Layer::create();
    _layerContainerList->setContentSize( Size(_spriteContainer->getContentSize().width - 38, _spriteContainer->getContentSize().height - 75 - _layerContainerContents->getContentSize().height - 25) );
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerList->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContainerList);
    
    _layerContainerBadge = Layer::create();
    _layerContainerBadge->setContentSize( Size(_spriteContainer->getContentSize().width - 38, 113) );
    _layerContainerBadge->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadge->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBadge->setPosition(_spriteContainer->getContentSize().width / 2, -5);
    _spriteContainer->addChild(_layerContainerBadge);
    
 
    //
    uiContents();
    uiList();
    uiBadge();
}
 
#pragma mark - PopupDungeonPass : ui
void PopupDungeonPass::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    auto spriteBG = Sprite::create("Assets/ui/purchase/pass_bg_01.png");
    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteBG->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height);
    spriteBG->getTexture()->setTexParameters(texParams);
    spriteBG->setScale(4);
    _layerContainerContents->addChild(spriteBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _layerContainerContents->addChild(menu);
    
    auto spriteTitle = Sprite::create("Assets/ui/purchase/dungeonpass_title_1.png");
    spriteTitle->setPosition(500, _layerContainerContents->getContentSize().height - 40);
    _layerContainerContents->addChild(spriteTitle);
    
    //
    {
        auto objStep = _listPassStep.back();
        int itemIdx = objStep->getRewardPurIdx();
        int itemParams = objStep->getRewardPurTypeParams();
        if ( itemIdx == 11000 )
        {
            auto uiSkinMask = Sprite::create("Assets/ui/purchase/pass_skin_box1_1.png");
            uiSkinMask->setAnchorPoint(Vec2::ZERO);
            
            auto clipper = ClippingNode::create(uiSkinMask);
            clipper->setContentSize(uiSkinMask->getContentSize());
            clipper->setIgnoreAnchorPointForPosition(false);
            clipper->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            clipper->setPosition(10, _layerContainerContents->getContentSize().height - 10);
            _layerContainerContents->addChild(clipper);
            
            auto uiBoardBG = ui::ImageView::create("Assets/ui/purchase/pass_skin_box1_1.png");
            uiBoardBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            clipper->addChild(uiBoardBG);
            
            auto unitHero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);
            unitHero->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            unitHero->setPosition(clipper->getContentSize().width / 2 + 68, 80);
            unitHero->setSkin(itemParams);
            unitHero->actionMove();
            unitHero->setTag(1000);
            clipper->addChild(unitHero);
            
            auto uiBoard = ui::ImageView::create("Assets/ui/purchase/pass_skin_box1_2.png");
            uiBoard->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            clipper->addChild(uiBoard);
            {
                auto uiText = ui::CText::create(GAME_TEXT("t_ui_pass_main_skin"), GAME_FONT, 24);
                uiText->setPosition(Vec2(uiBoard->getContentSize().width / 2, 18 + 3));
                uiText->setTextAreaSize(Size(132, uiText->getContentSize().height));
                uiText->setTextOverflow(Label::Overflow::SHRINK);
                uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiText->enableOutline(Color4B::BLACK, 2);
                uiBoard->addChild(uiText);
            }
        }
        
        
    }
    
    // item purchase
    {
        _itemPurchase = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/purchase/title_sparkle_bar_1_1.png"), Sprite::create("Assets/ui/purchase/title_sparkle_bar_1_1.png"),nullptr, CC_CALLBACK_1(PopupDungeonPass::onClickPurchase, this));
        _itemPurchase->setPosition(500, _layerContainerContents->getContentSize().height - 100);
        menu->addChild(_itemPurchase);
        {
            auto aniframeRun = Animation::create();
            aniframeRun->setDelayPerUnit(0.1f);
            for ( int i = 1; i <= 5; i++ )
            {
                aniframeRun->addSpriteFrameWithFile(MafUtils::format("Assets/ui/purchase/title_sparkle_bar_1_%d.png", i));
            }
            aniframeRun->addSpriteFrameWithFile("Assets/ui/purchase/title_sparkle_bar_1_1.png");
            
            auto action1 = Animate::create(aniframeRun);
            auto action2 = DelayTime::create(0.5);
            auto seq = Sequence::create(action1, action2, NULL);
            _itemPurchase->getNormalImage()->runAction(RepeatForever::create(seq));
        }
        
        _labelPurchase = Label::createWithTTF("", GAME_FONT, 24);
        _labelPurchase->setPosition(_itemPurchase->getContentSize()/2);
        _labelPurchase->setName("LABEL_TEXT");
        _itemPurchase->addChild(_labelPurchase);
    }
    
    // text time
    {
        auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_2.png");
        spriteBG->setPosition(500, _layerContainerContents->getContentSize().height - 150);
        _layerContainerContents->addChild(spriteBG);
        
        _labelTime = Label::createWithTTF("", GAME_FONT, 20);
        _labelTime->setPosition(spriteBG->getContentSize()/2);
        spriteBG->addChild(_labelTime);
    }
    
    // text buff
    {
        _layerBuff = Layer::create();
        _layerBuff->setContentSize(Size(_layerContainerContents->getContentSize().width, 110));
        _layerBuff->setIgnoreAnchorPointForPosition(false);
        _layerBuff->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _layerBuff->setPosition(0, _layerContainerContents->getContentSize().height - 296);
        _layerContainerContents->addChild(_layerBuff);
    }
    
    // progress
    {
        auto spriteBG = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/purchase/pass_progress_lv_bar.png"),nullptr, CC_CALLBACK_1(PopupDungeonPass::onClickProgress, this));
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteBG->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 372);
        menu->addChild(spriteBG);

        auto spriteProgressBG = Sprite::create("Assets/ui/purchase/pass_progress_1_1.png");
        spriteProgressBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        spriteProgressBG->setPosition(25, 5);
        spriteBG->addChild(spriteProgressBG);
        {
            _progress = ProgressTimer::create(Sprite::create("Assets/ui/purchase/pass_progress_1_2.png"));
            _progress->setType(ProgressTimer::Type::BAR);
            _progress->setMidpoint(Vec2(0, 0.5));
            _progress->setBarChangeRate(Vec2(1, 0));
            _progress->setAnchorPoint(Vec2(0, 0));
            _progress->setPercentage(0);
            spriteProgressBG->addChild(_progress);
            
            _labelProgressTitle = Label::createWithTTF("", GAME_FONT, 25);
            _labelProgressTitle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            _labelProgressTitle->setPosition(0, spriteProgressBG->getContentSize().height);
            _labelProgressTitle->setColor(Color3B(255,252,178));
            spriteProgressBG->addChild(_labelProgressTitle);
            
            _labelProgressLv = Label::createWithTTF("", GAME_FONT, 25);
            _labelProgressLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _labelProgressLv->setPosition(spriteProgressBG->getContentSize().width + 15, spriteProgressBG->getContentSize().height / 2);
            _labelProgressLv->setColor(Color3B(128, 128, 128));
            spriteProgressBG->addChild(_labelProgressLv);
            
            _labelProgressRate = Label::createWithTTF("", GAME_FONT, 20);
            _labelProgressRate->setPosition(spriteProgressBG->getContentSize() / 2);
            spriteProgressBG->addChild(_labelProgressRate);
        }
    }
    
    // text info
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_desc"), GAME_FONT, 20);
        labelText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelText->setPosition(25, 15);
        labelText->setDimensions(_layerContainerContents->getContentSize().width - 50, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::BOTTOM);
        labelText->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelText);
    }
    
    //
    drawBuffInfo();
    drawProgress();
}

void PopupDungeonPass::uiList()
{
    _layerContainerList->removeAllChildren();
    
    //
    auto spriteBG = Sprite::create("Assets/ui/purchase/pass_bar_top_1_1.png");
    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteBG->setPosition(_layerContainerList->getContentSize().width / 2, _layerContainerList->getContentSize().height);
    _layerContainerList->addChild(spriteBG);
    {
        auto lbLeftTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_column_non_purchase"), GAME_FONT, 30);
        lbLeftTitle->setPosition(spriteBG->getContentSize().width * 0.25, spriteBG->getContentSize().height / 2);
        lbLeftTitle->setDimensions(300, lbLeftTitle->getContentSize().height);
        lbLeftTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbLeftTitle->setOverflow(Label::Overflow::SHRINK);
        lbLeftTitle->enableOutline(Color4B::BLACK, 1);
        spriteBG->addChild(lbLeftTitle);
        
        auto lbRightTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_column_purchase"), GAME_FONT, 30);
        lbRightTitle->setPosition(spriteBG->getContentSize().width * 0.75, spriteBG->getContentSize().height / 2);
        lbRightTitle->setDimensions(300, lbRightTitle->getContentSize().height);
        lbRightTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbRightTitle->setOverflow(Label::Overflow::SHRINK);
        lbRightTitle->enableOutline(Color4B::BLACK, 1);
        spriteBG->addChild(lbRightTitle);
    }
    
    //
    _tableList = extension::TableView::create(this, Size(_layerContainerList->getContentSize().width, _layerContainerList->getContentSize().height - spriteBG->getContentSize().height) );
    _tableList->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableList->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _tableList->setIgnoreAnchorPointForPosition(false);
    _tableList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _tableList->setPosition(_layerContainerList->getContentSize().width / 2, 0);
    _tableList->setDelegate(this);
    _tableList->setBounceable(false);
    _layerContainerList->addChild(_tableList);
    
    //
    drawList();
}

void PopupDungeonPass::uiBadge()
{
    _layerContainerBadge->removeAllChildren();
    if ( _objPass == nullptr )
    {
        return;
    }
    
    // 무료 배지
    if ( _objPass->getBadgeFree() != 0 )
    {
        auto spriteBadgeBG = Sprite::create("Assets/ui/purchase/pass_bar_1_4.png");
        spriteBadgeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteBadgeBG->setPosition(0, _layerContainerBadge->getContentSize().height / 2);
        _layerContainerBadge->addChild(spriteBadgeBG);
        {
            auto objBadge = BadgeManager::getInstance()->getBadge(_objPass->getBadgeFree());
            
            _spriteBadgeFreeIconBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
            _spriteBadgeFreeIconBG->setContentSize(Size(82, 82));
            _spriteBadgeFreeIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _spriteBadgeFreeIconBG->setPosition(25, spriteBadgeBG->getContentSize().height / 2);
            spriteBadgeBG->addChild(_spriteBadgeFreeIconBG);
            if ( objBadge != nullptr )
            {
                auto spriteIcon = Sprite::create(objBadge->getPath());
                spriteIcon->setPosition(_spriteBadgeFreeIconBG->getContentSize()/2);
                spriteIcon->getTexture()->setTexParameters(texParams);
                _spriteBadgeFreeIconBG->addChild(spriteIcon);
                
                //
                auto labelTitle = Label::createWithTTF(objBadge->getName(), GAME_FONT, 18);
                labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                labelTitle->setPosition(220, spriteBadgeBG->getContentSize().height / 2);
                labelTitle->setDimensions(220, labelTitle->getContentSize().height);
                labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                labelTitle->setOverflow(Label::Overflow::SHRINK);
                spriteBadgeBG->addChild(labelTitle);
                
                auto labelEffect = Label::createWithTTF(objBadge->getBuffText(), GAME_FONT, 18);
                labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                labelEffect->setPosition(220, spriteBadgeBG->getContentSize().height / 2);
                labelEffect->setDimensions(220, labelEffect->getContentSize().height);
                labelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                labelEffect->setOverflow(Label::Overflow::SHRINK);
                spriteBadgeBG->addChild(labelEffect);
            }
            
            
        }
    }
    
    // 유료 배지
    if ( _objPass->getBadgePur() != 0 )
    {
        auto spriteBadgeBG = Sprite::create("Assets/ui/purchase/pass_bar_2_4.png");
        spriteBadgeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteBadgeBG->setPosition(_layerContainerBadge->getContentSize().width, _layerContainerBadge->getContentSize().height / 2);
        _layerContainerBadge->addChild(spriteBadgeBG);
        {
            auto objBadge = BadgeManager::getInstance()->getBadge(_objPass->getBadgePur());
            
            _spriteBadgePurIconBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
            _spriteBadgePurIconBG->setContentSize(Size(82, 82));
            _spriteBadgePurIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _spriteBadgePurIconBG->setPosition(spriteBadgeBG->getContentSize().width - 25, spriteBadgeBG->getContentSize().height / 2);
            spriteBadgeBG->addChild(_spriteBadgePurIconBG);
            if ( objBadge != nullptr )
            {
                auto spriteIcon = Sprite::create(objBadge->getPath());
                spriteIcon->setPosition(_spriteBadgePurIconBG->getContentSize()/2);
                spriteIcon->getTexture()->setTexParameters(texParams);
                _spriteBadgePurIconBG->addChild(spriteIcon);
                
                //
                auto labelTitle = Label::createWithTTF(objBadge->getName(), GAME_FONT, 18);
                labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                labelTitle->setPosition(spriteBadgeBG->getContentSize().width - 220, spriteBadgeBG->getContentSize().height / 2);
                labelTitle->setDimensions(220, labelTitle->getContentSize().height);
                labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                labelTitle->setOverflow(Label::Overflow::SHRINK);
                spriteBadgeBG->addChild(labelTitle);
                
                auto labelEffect = Label::createWithTTF(objBadge->getBuffText(), GAME_FONT, 18);
                labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
                labelEffect->setPosition(spriteBadgeBG->getContentSize().width - 220, spriteBadgeBG->getContentSize().height / 2);
                labelEffect->setDimensions(220, labelEffect->getContentSize().height);
                labelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                labelEffect->setOverflow(Label::Overflow::SHRINK);
                spriteBadgeBG->addChild(labelEffect);
            }
        }
    }
    
    
}

#pragma mark - PopupDungeonPass : draw
void PopupDungeonPass::drawPurchase()
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    // purhcase
    {
        auto action1 = CallFuncN::create([=](Node *sender){
            
            auto itemPurchase = (MafNode::MafMenuItemSprite*)sender;
            
            if ( _objPass == nullptr )
            {
                return;
            }
            
            time_t nDiffPurchase = _objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime();
            time_t nDiffReset = _objPass->getResetTime() - UtilsDate::getInstance()->getTime();
            if ( nDiffPurchase < 0 ) nDiffPurchase = 0;
            if ( nDiffReset < 0 ) nDiffReset = 0;
            
            //
            auto spritePurchase = (Sprite*)itemPurchase->getNormalImage();
            if ( _objPass->isBuyPass() == false && nDiffPurchase <= 0 )
            {
                spritePurchase->stopAllActions();
                spritePurchase->setTexture("Assets/ui/purchase/title_sparkle_bar_1_off.png");
            }
            
            //
            std::string strText = GAME_TEXT("t_ui_dungeonpass_btn_purchase");
            if ( _objPass->isBuyPass() == true )
                strText = GAME_TEXT("t_ui_dungeonpass_purchase_title_2");
            else if ( _objPass->isBuyPass() == false && nDiffPurchase <= 0 )
                strText = GAME_TEXT("t_ui_cash_msg_9_0");
            
            _labelPurchase->setString(strText);
            _labelPurchase->setDimensions(300, _labelPurchase->getContentSize().height);
            _labelPurchase->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            _labelPurchase->setOverflow(Label::Overflow::SHRINK);
            
        });
        auto action2 = DelayTime::create(10);
        auto seq = Sequence::create(action1, action2, NULL);
        _itemPurchase->stopAllActions();
        _itemPurchase->runAction(RepeatForever::create(seq));
    }
    
    // time
    {
        auto action1 = CallFuncN::create([=](Node *sender){
            
            auto labelTime = (Label*)sender;
            
            if ( _objPass == nullptr )
            {
                return;
            }
            
            time_t nDiffPurchase = _objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime();
            time_t nDiffReset = _objPass->getResetTime() - UtilsDate::getInstance()->getTime();
            if ( nDiffPurchase < 0 ) nDiffPurchase = 0;
            if ( nDiffReset < 0 ) nDiffReset = 0;
            
            std::string strTime = "";
            if ( _objPass->isBuyPass() == true )
            {
                int nDay = (int)(nDiffReset / 86400);
                int nHour = nDiffReset % 86400 / 3600;
                int nMin = nDiffReset % 3600 / 60;
                strTime = GAME_TEXTFORMAT("t_ui_dungeonpass_timeremain", nDay, nHour, nMin);
            }
            else
            {
                int nDay = (int)(nDiffPurchase / 86400);
                int nHour = nDiffPurchase % 86400 / 3600;
                int nMin = nDiffPurchase % 3600 / 60;
                strTime = GAME_TEXTFORMAT("t_server_dailymission_weekleft", nDay, nHour, nMin);
            }
            
            labelTime->setString(strTime);
            labelTime->setDimensions(250, labelTime->getContentSize().height);
            labelTime->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTime->setOverflow(Label::Overflow::SHRINK);
            labelTime->setColor(Color3B::WHITE);
            if ( _objPass->isBuyPass() == false && nDiffPurchase <= 0 )
                labelTime->setColor(Color3B::RED);
        });
        auto action2 = DelayTime::create(10);
        auto seq = Sequence::create(action1, action2, NULL);
        _labelTime->stopAllActions();
        _labelTime->runAction(RepeatForever::create(seq));
    }
}

void PopupDungeonPass::drawBuffInfo()
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    //
    _layerBuff->removeAllChildren();
    
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_text_2_2.png");
    spriteBG->setContentSize(_layerBuff->getContentSize());
    spriteBG->setPosition(_layerBuff->getContentSize()/2);
    spriteBG->setFlippedX(true);
    _layerBuff->addChild(spriteBG);
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_v2_1"), GAME_FONT, 20);
    labelTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    labelTitle->setPosition(0, _layerBuff->getContentSize().height);
    labelTitle->setColor(COLOR_TITLE);
    _layerBuff->addChild(labelTitle);
    
    //
    std::string listBuffTextKey[] = {"t_ui_dungeonpass_v2_2", "t_ui_dungeonpass_v2_3", "t_ui_dungeonpass_v2_4"};
    for ( int i = 0; i < sizeof(listBuffTextKey)/sizeof(std::string); i++ )
    {
        double posX = 30;
        double posY = 80 - 25 * i;
        
        Color3B color = Color3B(128, 128, 128);
        std::string strPath = "Assets/ui/costume/menu_btn_list_5.png";
        if ( _objPass->isBuyPass() == true )
        {
            color = Color3B::WHITE;
            strPath = "Assets/ui/costume/menu_btn_list_4.png";
        }
        
        auto labelBuff = Label::createWithTTF(GAME_TEXT(listBuffTextKey[i]), GAME_FONT, 18);
        labelBuff->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelBuff->setPosition(posX, posY);
        labelBuff->setColor(color);
        _layerBuff->addChild(labelBuff);
        
        auto spriteCheck = Sprite::create(strPath);
        spriteCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteCheck->setPosition(-4, labelBuff->getContentSize().height / 2);
        spriteCheck->getTexture()->setTexParameters(texParams);
        spriteCheck->setScale(2);
        labelBuff->addChild(spriteCheck);
    }
}

void PopupDungeonPass::drawProgress()
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    if ( _objPass->isLevelFinish() == true )
    {
        _progress->setPercentage(100);
        _labelProgressTitle->setString(GAME_TEXTFORMAT("t_ui_dungeonpass_progress", _objPass->getLevel()));
        _labelProgressLv->setString("MAX");
        _labelProgressRate->setString("MAX");
    }
    else
    {
        double nPercent = (double)_objPass->getExp() / _objPass->getExpNext() * 100;
        if ( nPercent > 100 )
            nPercent = 100;
        
        _progress->setPercentage(nPercent);
        _labelProgressTitle->setString(GAME_TEXTFORMAT("t_ui_dungeonpass_progress", _objPass->getLevel()));
        _labelProgressLv->setString("LV." + MafUtils::toString(_objPass->getLevel() + 1));
        _labelProgressRate->setString(GAME_TEXTFORMAT("t_ui_friend_info_1", _objPass->getExp(), _objPass->getExpNext()));
    }
}

void PopupDungeonPass::drawList()
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    int nFree = -1;
    int nPur = -1;
    
    for ( int i = 0; i < _listPassStep.size(); i++ )
    {
        auto objTemp = _listPassStep.at(i);
        if ( _objPass->getPoint() >= objTemp->getPoint() &&  objTemp->isRewardFree() == false )
        {
            nFree = i;
            break;
        }
    }
    
    if ( nFree == -1 && _objPass->isBuyPass() == true )
    {
        for ( int i = 0; i < _listPassStep.size(); i++ )
        {
            auto objTemp = _listPassStep.at(i);
            if ( _objPass->getPoint() >= objTemp->getPoint() &&  objTemp->isRewardPur() == false )
            {
                nPur = i;
                break;
            }
        }
    }
    
    //
    Vec2 pos = _tableList->getContentOffset();
    _tableList->reloadData();
    
    Vec2 offset = pos;
    Size size = _tableList->getDataSource()->tableCellSizeForIndex(_tableList, 0);
    
    if ( nFree != -1 )
    {
        nFree =  (int)_listPassStep.size() - 1 - nFree;
        offset.y = (size.height * nFree) * -1;
    }
    else  if ( nPur != -1 )
    {
        nPur =  (int)_listPassStep.size() - 1 - nPur;
        offset.y = (size.height * nPur) * -1;
    }
    

    if ( _bListPosInit == true )
    {
        _tableList->setContentOffset(pos);
    }
    else
    {
        const Point minOffset = _tableList->minContainerOffset();
        const Point maxOffset = _tableList->maxContainerOffset();
        offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
        offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
        _tableList->setContentOffset(offset, true);
    }
    
    //
    _bListPosInit = true;
}

void PopupDungeonPass::drawBadge()
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    bool bFree = true;
    bool bPur = true;
    
    for ( auto info : _listPassStep )
    {
        if ( info->isRewardFree() == false )
        {
            bFree = false;
        }
        
        if( info->isRewardPur() == false )
        {
            bPur = false;
        }
    }
    
    if ( bFree == true )
    {
        _spriteBadgeFreeIconBG->removeChildByTag(10);
        
        //
        auto spriteAccquire = Sprite::create("Assets/ui/purchase/pass_check.png");
        spriteAccquire->setPosition(_spriteBadgeFreeIconBG->getContentSize()/2);
        spriteAccquire->setTag(10);
        _spriteBadgeFreeIconBG->addChild(spriteAccquire);
    }
    
    if ( bPur == true )
    {
        _spriteBadgePurIconBG->removeChildByTag(10);
        
        //
        auto spriteAccquire = Sprite::create("Assets/ui/purchase/pass_check.png");
        spriteAccquire->setPosition(_spriteBadgePurIconBG->getContentSize()/2);
        spriteAccquire->setTag(10);
        _spriteBadgePurIconBG->addChild(spriteAccquire);
    }
}

#pragma mark - PopupDungeonPass : click
void PopupDungeonPass::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupDungeonPass::onClickProgress(cocos2d::Ref* sender)
{
    auto popup = PopupMissionDefault::create(E_MISSION::DAILY);
    popup->show();
    
    hide();
}

void PopupDungeonPass::onClickReward(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    std::string strType = item->getName();
    
    auto objStep = _listPassStep.at(item->getTag());
    if ( objStep == nullptr )
    {
        return;
    }
    
    if ( strType.compare("FREE") == 0 )
    {// free
        PopupLoading::show();
        PassManager::getInstance()->requestReward(_objPass->getSeason(), objStep->getIdx(), E_PASS_REWARD_TYPE::FREE, CC_CALLBACK_3(PopupDungeonPass::callbackPassReward, this));
    }
    else if ( strType.compare("PUR") == 0 )
    {// pur
        PopupLoading::show();
        PassManager::getInstance()->requestReward(_objPass->getSeason(), objStep->getIdx(), E_PASS_REWARD_TYPE::PURCHASE, CC_CALLBACK_3(PopupDungeonPass::callbackPassReward, this));
    }
}

void PopupDungeonPass::onClickPurchase(cocos2d::Ref* sender)
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    if ( _objPass->isBuyPass() || _objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime() <= 600 )
    {
        return;
    }
        

    auto popup = PopupPurchaseDungeonPass::create();
    popup->setCallback(CC_CALLBACK_0(PopupDungeonPass::callbackPurchase, this));
    popup->show();
}

void PopupDungeonPass::onClickStepUp(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = dynamic_cast<ui::Button*>(sender);
    
    auto objStep = _listPassStep.at(uiButton->getTag());
    
    auto itemNeed = objStep->getStepUpCost();
    auto itemNow = atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str());
    if ( itemNeed <= 0 )
    {
        return;
    }
    
    if ( itemNeed > itemNow )
    {
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_pass_step_up_msg"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
    
        PopupLoading::show();
        PassManager::getInstance()->requestStepUp(_objPass->getSeason(), objStep->getIdx(), CC_CALLBACK_2(PopupDungeonPass::callbackStepUp, this));
        
    });
    popup->show();
}

#pragma mark - PopupDungeonPass : callback
void PopupDungeonPass::callbackPassInfo(bool bResult)
{
    PopupLoading::hide();
    
    if ( _objPass != nullptr )
    {
        _objPass->release();
        _objPass = nullptr;
    }
    
    _objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( bResult == false || _objPass == nullptr )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    _objPass->retain();
    
    //
    _listPassStep.clear();
    _listPassStep = _objPass->getListStep();
    
    //
    drawPurchase();
    drawBuffInfo();
    drawProgress();
    drawList();
    drawBadge();
    
    // 패스 미구입 상태 : 무료 패스 완료시 하루에 한번 출력
    if ( _objPass->isBuyPass() == false )
    {
        bool bRewardAll = true;
        for ( auto& objTemp : _listPassStep )
        {
            if ( objTemp->isRewardFree() == false )
            {
                bRewardAll = false;
                break;
            }
        }
        
        std::string strToday = UtilsDate::getInstance()->getTimeString(UtilsDate::getInstance()->getTime(), UtilsDate::eType::date_real_Ymd);
        if ( bRewardAll == true && UserDefault::getInstance()->getStringForKey(KEY_TODAY_DUNGEONPASS, "").compare(strToday) != 0 )
        {
            UserDefault::getInstance()->setStringForKey(KEY_TODAY_DUNGEONPASS, strToday);
            
            if ( _objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime() <= 600 )
            {
                auto popup = PopupPurchaseDungeonPass::create();
                popup->setCallback(CC_CALLBACK_0(PopupDungeonPass::callbackPurchase, this));
                popup->show();
            }
            
        }
    }
}

void PopupDungeonPass::callbackPassReward(bool bResult, bool bBadgeFree, bool bBadgePur)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    PassManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupDungeonPass::callbackPassInfo, this));
    
    if ( bBadgeFree == true || bBadgePur == true )
    {
        auto popup = PopupDungeonPassBadge::create(bBadgeFree, bBadgePur);
        popup->setCallbackPurchase(CC_CALLBACK_0(PopupDungeonPass::callbackBadge, this));
        popup->show();
    }
}

void PopupDungeonPass::callbackStepUp(bool bResult, int nResult)
{
    PopupLoading::hide();
  
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    PopupLoading::show();
    PassManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupDungeonPass::callbackPassInfo, this));
}
 
void PopupDungeonPass::callbackPurchase()
{
    if ( _objPass != nullptr )
    {
        _objPass->release();
        _objPass = nullptr;
    }
    
    _objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( _objPass == nullptr )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    _objPass->retain();
    
    //
    _listPassStep.clear();
    _listPassStep = _objPass->getListStep();
    
    //
    drawPurchase();
    drawBuffInfo();
    drawProgress();
    drawList();
    drawBadge();
}

void PopupDungeonPass::callbackBadge()
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    if ( _objPass->isBuyPass() || _objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime() <= 600 )
    {
        return;
    }
        

    auto popup = PopupPurchaseDungeonPass::create();
    popup->setCallback(CC_CALLBACK_0(PopupDungeonPass::callbackPurchase, this));
    popup->show();
}
