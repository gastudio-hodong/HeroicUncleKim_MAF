//
//  PopupPetEquipBoxReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/05/21.
//
#include "PopupPetEquipBoxReward.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/PetEquip/PopupPetEquipBoxBuyPurchase.h"
#include "GameUIPopup/Other/PetEquip/PopupPetEquipUseMaxWarning.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/PetEquipBoxManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/ServerMissionManager.h"

USING_NS_CC;

PopupPetEquipBoxReward* PopupPetEquipBoxReward::create(E_PET_BOX_TYPE eType)
{
    PopupPetEquipBoxReward* pRet = new(std::nothrow) PopupPetEquipBoxReward();
    if ( pRet && pRet->init(eType) )
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

PopupPetEquipBoxReward::PopupPetEquipBoxReward(void) :
_isEvent(false),
_isMileage(false),
_eType(E_PET_BOX_TYPE::NONE),

_lyContents(nullptr),
_lyRewardTable(nullptr),
_table(nullptr),

_sprBox(nullptr),
_sprMagnificationSPBox(nullptr),

_callbackRedraw(nullptr),
_callbackHide(nullptr)
{
    _listDraw.clear();
    _lisDrawGrade.clear();
    _listBox.clear();
    _listBoxBonusBadge.clear();
}

PopupPetEquipBoxReward::~PopupPetEquipBoxReward(void)
{
    _listDraw.clear();
    _lisDrawGrade.clear();
    _listBox.clear();
    _listBoxBonusBadge.clear();
    _listUseButton.clear();
}


#pragma mark -table

void PopupPetEquipBoxReward::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
}

void PopupPetEquipBoxReward::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupPetEquipBoxReward::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 151);
}

extension::TableViewCell* PopupPetEquipBoxReward::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
    ssize_t nIdxStart = idx * 5;
    ssize_t nIdxEnd = nIdxStart + 5;
    if ( nIdxEnd > _listDraw.size() )
    {
        nIdxEnd = _listDraw.size();
    }
    // obj
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        auto info = _listDraw.at(nIdxStart + i);
        
        double nPosX = 116 * i + 10 * i;
        double nPosY = size.height / 2;
        
        auto sprBox = Sprite::create("Assets/ui/pet/btn_pet_icon_slot3_1.png");
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprBox->setPosition(nPosX, nPosY);
        cell->addChild(sprBox);
        {
            auto spriteEquip = Sprite::create(MafUtils::format("Assets/ui/pet/icon_equip/icon_pet_equip_%d.png", info->getIdx()));
            spriteEquip->setPosition(sprBox->getContentSize().width / 2, sprBox->getContentSize().height / 2 + 10);
            spriteEquip->setScale(2);
            sprBox->addChild(spriteEquip);
            
            auto labelEquipAmount = MafNode::createBmFont(MafUtils::format("x%d", info->getCount()), GAME_BM_FONT_AL, 22);
            labelEquipAmount->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            labelEquipAmount->setPosition(sprBox->getContentSize().width - 20, 25);
            sprBox->addChild(labelEquipAmount);
            
            if(_isMileage)
            {
                auto sprFx = Sprite::create("Assets/ui/pet/btn_pet_icon_slot1_3.png");
                sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprFx->setPosition(sprBox->getContentSize()/2);
                sprBox->addChild(sprFx);
                sprFx->setOpacity(0);
                
                sprFx->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.5f), FadeOut::create(0.5f), nullptr)));
            }
        }
    }
    
    
    
    return cell;
}

ssize_t PopupPetEquipBoxReward::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _listDraw.size() / 5;
    if ( _listDraw.size() % 5 != 0 )
        size++;
    
    return size;
}

bool PopupPetEquipBoxReward::init(E_PET_BOX_TYPE eType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _isEvent = EventManager::getInstance()->isActivatedEvent(16);
    _eType = eType;
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupPetEquipBoxReward::initVar()
{
    float nWidth = 750;
    float nHeight = getContentSize().height;
    if ( nHeight > 1004 )
        nHeight = 1004;
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
}

void PopupPetEquipBoxReward::initUi()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu, E_ZORER::UI+1);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupPetEquipBoxReward::onClick, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_pet_equip_randombox_purchase"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    _lyContents = Layer::create();
    _lyContents->setContentSize( _spriteContainer->getContentSize() - Size(10, 75) );
    _lyContents->setIgnoreAnchorPointForPosition(false);
    _lyContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _lyContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 5));
    _spriteContainer->addChild(_lyContents);

    
    uiContents();
    uiTable();
}

#pragma mark - UI
void PopupPetEquipBoxReward::uiContents()
{
    _listUseButton.clear();
    _lyContents->removeAllChildren();
    
    int nMileageCurrent = PetEquipBoxManager::getInstance()->getEquipBoxMileage();
    int nMileageMax = ParameterManager::getInstance()->getParameterInteger("pet_equip_box_mileage");
    
    auto sprBG = Sprite::create("Assets/ui/pet/pet_equipment_room_3.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBG->setPosition(_lyContents->getContentSize().width/2, _lyContents->getContentSize().height);
    sprBG->getTexture()->setTexParameters(texParams);
    sprBG->setScale(4);
    _lyContents->addChild(sprBG);
    
    auto sprSpBoxIcon = Sprite::create("Assets/icon/icon_pet_equipment_2.png");
    sprSpBoxIcon->getTexture()->setTexParameters(texParams);
    sprSpBoxIcon->setScale(2);
    sprSpBoxIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprSpBoxIcon->setPosition(0, _lyContents->getContentSize().height - 50);
    _lyContents->addChild(sprSpBoxIcon, E_ZORER::UI);
    {
        auto lbCount = Label::createWithTTF(MafUtils::format("x%d", PetEquipBoxManager::getInstance()->getEquipBoxSpecialCount()) , GAME_FONT, 28);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->enableOutline(Color4B::BLACK, 2);
        lbCount->setPosition(sprSpBoxIcon->getPosition() + Vec2::RIGHT * (sprSpBoxIcon->getContentSize().width * sprSpBoxIcon->getScale() - 10));
        _lyContents->addChild(lbCount, E_ZORER::UI);
    }
    
    auto sprBoxIcon = Sprite::create("Assets/icon/icon_pet_equipment.png");
    sprBoxIcon->getTexture()->setTexParameters(texParams);
    sprBoxIcon->setScale(2);
    sprBoxIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprBoxIcon->setPosition(sprSpBoxIcon->getPosition() + Vec2::DOWN * (sprSpBoxIcon->getContentSize().height + 25));
    _lyContents->addChild(sprBoxIcon, E_ZORER::UI);
    {
        auto lbCount = Label::createWithTTF(MafUtils::format("x%d", PetEquipBoxManager::getInstance()->getEquipBoxCount()) , GAME_FONT, 28);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->enableOutline(Color4B::BLACK, 2);
        lbCount->setPosition(sprBoxIcon->getPosition() + Vec2::RIGHT * (sprBoxIcon->getContentSize().width * sprBoxIcon->getScale() - 10));
        _lyContents->addChild(lbCount, E_ZORER::UI);
        
        if ( _isEvent == true )
        {
            lbCount->enableStrikethrough();
            lbCount->setColor(Color3B(104,104,104));
            
            auto labelRestrict = Label::createWithTTF(GAME_TEXT("t_pet_equip_restrict"), GAME_FONT, 28);
            labelRestrict->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelRestrict->enableOutline(Color4B::BLACK, 2);
            labelRestrict->setPosition(lbCount->getContentSize().width + 10, labelRestrict->getContentSize().height / 2);
            lbCount->addChild(labelRestrict, E_ZORER::UI);
        }
    }
    
    if(_isEvent == true)
    {
        auto sprEventBoxIcon = Sprite::create("Assets/icon/icon_treasure.png");
        sprEventBoxIcon->getTexture()->setTexParameters(texParams);
        sprEventBoxIcon->setScale(2);
        sprEventBoxIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprEventBoxIcon->setPosition(sprBoxIcon->getPosition() + Vec2::DOWN * (sprBoxIcon->getContentSize().height + 25));
        _lyContents->addChild(sprEventBoxIcon, E_ZORER::UI);
        {
            auto lbCount = Label::createWithTTF(MafUtils::format("x%d", PetEquipBoxManager::getInstance()->getEquipBoxEventCount()) , GAME_FONT, 28);
            lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbCount->enableOutline(Color4B::BLACK, 2);
            lbCount->setPosition(sprEventBoxIcon->getPosition() + Vec2::RIGHT * (sprEventBoxIcon->getContentSize().width * sprEventBoxIcon->getScale() - 10));
            _lyContents->addChild(lbCount, E_ZORER::UI);
        }
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _lyContents->addChild(menu, E_ZORER::UI+1);
    
    auto itemHelp = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/icon_help.png"), nullptr, CC_CALLBACK_1(PopupPetEquipBoxReward::onClick, this));
    itemHelp->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    itemHelp->setPosition(_lyContents->getContentSize().width - 10, _lyContents->getContentSize().height - 30);
    itemHelp->setName("HELP");
    menu->addChild(itemHelp);
    {
        auto lbHelp = Label::createWithTTF(GAME_TEXT("t_ui_costume_info"), GAME_FONT, 22);
        lbHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbHelp->setAlignment(TextHAlignment::CENTER,TextVAlignment::TOP);
        lbHelp->enableOutline(Color4B::BLACK, 2);
        lbHelp->setDimensions(itemHelp->getContentSize().width * 1.4, lbHelp->getContentSize().height);
        lbHelp->setPosition(itemHelp->getContentSize().width/2, 0);
        itemHelp->addChild(lbHelp);
    }
    
    auto sprBtn = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png");
    sprBtn->setContentSize(Size(220, 104));
    
    auto sprBtnOff = ui::Scale9Sprite::create("Assets/ui_common/btn_off.png");
    sprBtnOff->setContentSize(Size(220, 104));
    
    auto itemBoxUse = MafNode::MafMenuItemSprite::create(sprBtn, nullptr, sprBtnOff, CC_CALLBACK_1(PopupPetEquipBoxReward::onClick, this));
    itemBoxUse->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    itemBoxUse->setPosition(Vec2(20, 10));
    itemBoxUse->setName("BOX_USE");
    itemBoxUse->setTag(1);
    menu->addChild(itemBoxUse);
    {
        auto lbBoxUse = Label::createWithTTF(GAME_TEXT("t_pet_equip_box_name_1") + "\n" + GAME_TEXTFORMAT("t_pet_equip_use", 1), GAME_FONT, 24);
        lbBoxUse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbBoxUse->setPosition(itemBoxUse->getContentSize()/2);
        lbBoxUse->setDimensions(itemBoxUse->getContentSize().width - 10, itemBoxUse->getContentSize().height - 5);
        lbBoxUse->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbBoxUse->setOverflow(Label::Overflow::SHRINK);
        itemBoxUse->addChild(lbBoxUse);
        
        if(nMileageCurrent >= nMileageMax)
        {
            auto sprMileageFx = ui::Scale9Sprite::create("Assets/ui_common/btn_1_10_on.png");
            sprMileageFx->setContentSize(Size(220, 104));
            sprMileageFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprMileageFx->setPosition(itemBoxUse->getContentSize()/2);
            itemBoxUse->addChild(sprMileageFx);
            
            auto fadeOut = FadeOut::create(1.f);
            auto fadeIn = FadeIn::create(1.f);
            auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
            sprMileageFx->runAction(RepeatForever::create(sequience));
            
            auto sprBoost = Sprite::create("Assets/ui/pet/boost_1_2.png");
            sprBoost->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprBoost->setScale(0.7);
            sprBoost->setPosition(Vec2(15, itemBoxUse->getContentSize().height - 15));
            itemBoxUse->addChild(sprBoost);
        }
    }
    
    auto sprBtnX3 = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png");
    sprBtnX3->setContentSize(Size(220, 104));
    
    auto sprBtnX3Off = ui::Scale9Sprite::create("Assets/ui_common/btn_off.png");
    sprBtnX3Off->setContentSize(Size(220, 104));
    
    auto itemBoxUseX3 = MafNode::MafMenuItemSprite::create(sprBtnX3, nullptr, sprBtnX3Off, CC_CALLBACK_1(PopupPetEquipBoxReward::onClick, this));
    itemBoxUseX3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemBoxUseX3->setPosition(_lyContents->getContentSize().width/2, 10);
    itemBoxUseX3->setName("BOX_USE");
    itemBoxUseX3->setTag(3);
    menu->addChild(itemBoxUseX3);
    {
        auto lbBoxUse = Label::createWithTTF(GAME_TEXT("t_pet_equip_box_name_1") + "\n" + GAME_TEXTFORMAT("t_pet_equip_use", 3), GAME_FONT, 24);
        lbBoxUse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbBoxUse->setPosition(itemBoxUseX3->getContentSize()/2);
        lbBoxUse->setDimensions(itemBoxUseX3->getContentSize().width - 10, itemBoxUseX3->getContentSize().height - 5);
        lbBoxUse->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbBoxUse->setOverflow(Label::Overflow::SHRINK);
        itemBoxUseX3->addChild(lbBoxUse);
        
        if(nMileageCurrent >= nMileageMax)
        {
            itemBoxUseX3->setEnabled(false);
        }
    }
    
    auto sprBtnOffSP = ui::Scale9Sprite::create("Assets/ui_common/btn_off.png");
    sprBtnOffSP->setContentSize(Size(220, 104));
    
    auto itemBoxUseSP = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_special_box_1_1.png"), nullptr, sprBtnOffSP, CC_CALLBACK_1(PopupPetEquipBoxReward::onClick, this));
    itemBoxUseSP->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    itemBoxUseSP->setPosition(_lyContents->getContentSize().width - 20, 10);
    itemBoxUseSP->setName("BOX_USE_SP");
    itemBoxUseSP->setTag(999);
    menu->addChild(itemBoxUseSP);
    {
        if(PetEquipBoxManager::getInstance()->getEquipBoxSpecialCount() >= 1)
        {
            auto sprBoxUseSP = Sprite::create("Assets/ui_common/btn_special_box_1_2.png");
            sprBoxUseSP->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprBoxUseSP->setPosition(itemBoxUseSP->getContentSize()/2);
            itemBoxUseSP->addChild(sprBoxUseSP);
            
            auto fadeOut = FadeOut::create(1.f);
            auto fadeIn = FadeIn::create(1.f);
            auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
            sprBoxUseSP->runAction(RepeatForever::create(sequience));
            
            //
            itemBoxUseSP->setEnabled(true);
        }
        else
        {
            //
            itemBoxUseSP->setEnabled(false);
        }
        
        auto sprImpect = ui::Scale9Sprite::create("Assets/ui_common/base_text_3_1.png");
        sprImpect->setContentSize(Size(220, 39));
        sprImpect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        sprImpect->setPosition(itemBoxUseSP->getContentSize().width/2, itemBoxUseSP->getContentSize().height - 5);
        itemBoxUseSP->addChild(sprImpect);
        {
            auto lbImpect = Label::createWithTTF(GAME_TEXT("t_pet_equip_box_special_deco"), GAME_FONT, 22);
            lbImpect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbImpect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            lbImpect->setDimensions(sprImpect->getContentSize().width - 10, sprImpect->getContentSize().height - 5);
            lbImpect->setPosition(sprImpect->getContentSize()/2);
            sprImpect->addChild(lbImpect);
        }
        
        auto lbBoxUse = Label::createWithTTF(GAME_TEXT("t_pet_equip_box_name_2") + "\n" + GAME_TEXT("t_ui_prison_use"), GAME_FONT, 24);
        lbBoxUse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbBoxUse->setPosition(itemBoxUseSP->getContentSize()/2);
        lbBoxUse->setDimensions(itemBoxUseSP->getContentSize().width - 10, itemBoxUseSP->getContentSize().height - 5);
        lbBoxUse->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbBoxUse->setOverflow(Label::Overflow::SHRINK);
        itemBoxUseSP->addChild(lbBoxUse);
        
        if(nMileageCurrent >= nMileageMax)
        {
            auto sprMileageFx = ui::Scale9Sprite::create("Assets/ui_common/btn_1_10_on.png");
            sprMileageFx->setContentSize(Size(220, 104));
            sprMileageFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprMileageFx->setPosition(itemBoxUseSP->getContentSize()/2);
            itemBoxUseSP->addChild(sprMileageFx);
            
            auto fadeOut = FadeOut::create(1.f);
            auto fadeIn = FadeIn::create(1.f);
            auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
            sprMileageFx->runAction(RepeatForever::create(sequience));
            
            auto sprBoost = Sprite::create("Assets/ui/pet/boost_1_2.png");
            sprBoost->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprBoost->setScale(0.7);
            sprBoost->setPosition(Vec2(15, itemBoxUseSP->getContentSize().height - 15));
            itemBoxUseSP->addChild(sprBoost);
                
        }
    }
    
    auto sprBackBar = Sprite::create("Assets/ui/pet/boost_progress_1_1.png");
    sprBackBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprBackBar->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 130));
    _lyContents->addChild(sprBackBar);
    if(nMileageCurrent < nMileageMax)
    {
        float gage = (float)nMileageCurrent/(float)(nMileageMax + 10);
        
        auto progress = ProgressTimer::create(Sprite::create("Assets/ui/pet/boost_progress_1_2.png"));
        progress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        progress->setPosition(Vec2(sprBackBar->getContentSize()/2));
        progress->setType(ProgressTimer::Type::BAR);
        progress->setMidpoint(Vec2::ANCHOR_MIDDLE_LEFT);
        progress->setBarChangeRate(Vec2(1, 0));
        progress->setPercentage(gage*100);
        sprBackBar->addChild(progress);
    }
    else
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        for ( int i = 1; i <= 22; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/pet/boost_progress_2_%d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        
        sprBackBar->runAction(RepeatForever::create(Animate::create(animation)));
    }
    
    _listUseButton.pushBack(itemBoxUse);
    _listUseButton.pushBack(itemBoxUseX3);
    _listUseButton.pushBack(itemBoxUseSP);
    
    
}

void PopupPetEquipBoxReward::uiTable()
{
    if(_lyRewardTable == nullptr)
    {
        //_lyRewardTable = LayerColor::create(Color4B::BLUE);
        _lyRewardTable = Layer::create();
        _lyRewardTable->setContentSize( Size(_spriteContainer->getContentSize().width - 100, _spriteContainer->getContentSize().height - 590) );
        _lyRewardTable->setIgnoreAnchorPointForPosition(false);
        _lyRewardTable->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _lyRewardTable->setPosition(_spriteContainer->getContentSize().width/2, 165);
        _spriteContainer->addChild(_lyRewardTable);
        
        // list table
        _table = extension::TableView::create(this, Size(_lyRewardTable->getContentSize().width, _lyRewardTable->getContentSize().height));
        _table->setDirection(extension::ScrollView::Direction::VERTICAL);
        _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
        _table->setIgnoreAnchorPointForPosition(false);
        _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _table->setPosition( Vec2(_lyRewardTable->getContentSize().width / 2, 0));
        _table->setDelegate(this);
        _lyRewardTable->addChild(_table);
    }
    
    _table->reloadData();
    
}

void PopupPetEquipBoxReward::actionBoxOpen(int nUseCount)
{
    
    for(auto obj : _listUseButton)
        obj->setEnabled(false);
    
    std::string strPath = "";
    std::vector<std::string> vecStartAnimPath;
    vecStartAnimPath.clear();
    std::vector<std::string> vecEndAnimPath;
    vecEndAnimPath.clear();
    
    auto animStart = Animation::create();
    animStart->setDelayPerUnit(0.1f);
    
    switch (_eType)
    {
        case E_PET_BOX_TYPE::COMMON:
        {
            strPath = "Assets/ui/pet/pet_equipment_box_1_1.png";
            
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_1.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_2.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_3.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_4.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_5.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_6.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_7.png");
        
        }
            break;
        case E_PET_BOX_TYPE::EVENT:
        {
            strPath = "Assets/ui/pet/pet_equiopment_event_box_1_1.png";
            
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equiopment_event_box_1_1.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equiopment_event_box_1_2.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equiopment_event_box_1_3.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equiopment_event_box_1_4.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equiopment_event_box_1_5.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equiopment_event_box_1_6.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box_1_7.png");
            
            
        }
            break;
            
        case E_PET_BOX_TYPE::CASH:
        {
            strPath = "Assets/ui/pet/pet_equipment_box2_1_1.png";
            
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box2_1_1.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box2_1_2.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box2_1_3.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box2_1_4.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box2_1_5.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box2_1_6.png");
            vecStartAnimPath.push_back("Assets/ui/pet/pet_equipment_box2_1_7.png");
        }
            break;
    }
    
    for(auto str : vecStartAnimPath)
    {
        animStart->addSpriteFrameWithFile(str);
    }
    
    if(_sprBox == nullptr)
    {
        _sprBox = Sprite::create(strPath);
        _sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _sprBox->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 320);
        _spriteContainer->addChild(_sprBox, E_ZORER::UI);
        
        auto sprBoost = Sprite::create("Assets/ui/pet/boost_1_2.png");
        sprBoost->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBoost->setName("BOOST");
        sprBoost->setPosition(Vec2(_sprBox->getContentSize()/2) + Vec2::RIGHT * 90 + Vec2::UP * 70);
        _sprBox->addChild(sprBoost);
        if(!_isMileage)
            sprBoost->setVisible(false);
    }
    else
    {
        _sprBox->setTexture(strPath);
    }
    
    _sprBox->stopAllActions();
    
    auto sequence = Sequence::create(Animate::create(animStart), CallFunc::create([=](void)
    {
        auto animEnd = Animation::create();
        animEnd->setDelayPerUnit(0.1f);
        
        switch (_eType)
        {
            case E_PET_BOX_TYPE::COMMON:
            {
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_1.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_2.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_3.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_4.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_5.png");
            }
                break;
            case E_PET_BOX_TYPE::EVENT:
            {
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_1.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_2.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_3.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_4.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_5.png");
            }
                break;
            
            case E_PET_BOX_TYPE::CASH:
            {
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_1.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_2.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_3.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_4.png");
                animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_5.png");
            }
                break;
        }
        _sprBox->runAction(RepeatForever::create(Animate::create(animEnd)));
        
        int boxSpecialCount = PetEquipBoxManager::getInstance()->getEquipBoxSpecialCount();
        int boxNormalCount = PetEquipBoxManager::getInstance()->getEquipBoxCount();
        int boxEventCount = PetEquipBoxManager::getInstance()->getEquipBoxEventCount();
        
        bool bEvent = EventManager::getInstance()->isActivatedEvent(16);
        for(auto obj : _listUseButton)
        {
            if ( obj->getTag() == 1 )
            {
                int useCount = 1;
                int boxCount = boxNormalCount;
                if ( bEvent == true )
                {
                    boxCount = boxEventCount;
                }
                
                if ( useCount <= boxCount )
                {
                    obj->setEnabled(true);
                }
                else
                {
                    obj->setEnabled(false);
                }
            }
            else if ( obj->getTag() == 3 )
            {
                int useCount = 3;
                int boxCount = boxNormalCount;
                if ( bEvent == true )
                {
                    boxCount = boxEventCount;
                }
                
                if ( PetEquipBoxManager::getInstance()->getEquipBoxMileage() < ParameterManager::getInstance()->getParameterInteger("pet_equip_box_mileage") )
                {
                    obj->setEnabled(true);
                    
                    if ( useCount <= boxCount )
                    {
                        obj->setEnabled(true);
                    }
                    else
                    {
                        obj->setEnabled(false);
                    }
                }
                else
                {
                    obj->setEnabled(false);
                }   
            }
            else if ( obj->getTag() == 999 )
            {
                int useCount = 1;
                int boxCount = boxSpecialCount;
                
                if ( useCount <= boxCount )
                {
                    obj->setEnabled(true);
                }
                else
                {
                    obj->setEnabled(false);
                }
            }
        }
        
        uiTable();
        
        auto boost = _sprBox->getChildByName("BOOST");
        
        if(boost != nullptr)
        {
            if(_isMileage)
                boost->setVisible(true);
            else
            {
                boost->setVisible(false);
            }
        }
        
        
    }), NULL);
    
    
    for(auto obj : _listBox)
        obj->removeFromParent();
    
    _listBox.clear();
    
    if(nUseCount > 1)
    {
        Vec2 pos = Vec2(_lyContents->getContentSize().width/3, _lyContents->getContentSize().height - 240);
        
        for(int i = 0; i < nUseCount-1; ++i)
        {
            auto sprBox = Sprite::create(strPath);
            sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprBox->setPosition(pos + Vec2::RIGHT * (i * _lyContents->getContentSize().width/3));
            sprBox->setScale(0.7f);
            _lyContents->addChild(sprBox);
            _listBox.pushBack(sprBox);
            
            auto animStart = Animation::create();
            animStart->setDelayPerUnit(0.1f);
            
            for(auto str : vecStartAnimPath)
            {
                animStart->addSpriteFrameWithFile(str);
            }
            
            auto sequence = Sequence::create(Animate::create(animStart), CallFunc::create([=](void)
            {
                auto animEnd = Animation::create();
                animEnd->setDelayPerUnit(0.1f);
                
                switch (_eType)
                {
                    case E_PET_BOX_TYPE::COMMON:
                    {
                        
                            animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_1.png");
                            animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_2.png");
                            animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_3.png");
                            animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_4.png");
                            animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box_2_5.png");
                    }
                        break;
                    case E_PET_BOX_TYPE::EVENT:
                    {
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_1.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_2.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_3.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_4.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equiopment_event_box_2_5.png");
                    }
                        break;
                        
                    case E_PET_BOX_TYPE::CASH:
                    {
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_1.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_2.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_3.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_4.png");
                        animEnd->addSpriteFrameWithFile("Assets/ui/pet/pet_equipment_box2_2_5.png");
                    }
                        break;
                }
                sprBox->runAction(RepeatForever::create(Animate::create(animEnd)));
            }), NULL);
            
            sprBox->runAction(sequence);
        }
        
    }
    
    for(auto obj : _listBoxBonusBadge)
        obj->removeFromParent();
    
    _listBoxBonusBadge.clear();
    
    if(_eType == E_PET_BOX_TYPE::CASH)
    {
        int nMagnification = 0;
        if(_lisDrawGrade.size() > 0)
        {
            auto obj = _lisDrawGrade.at(0);
            if(obj != nullptr)
                nMagnification = obj->getGrade();
        }
        
        if(_sprMagnificationSPBox == nullptr)
        {
            _sprMagnificationSPBox = Sprite::create("Assets/ui_common/base_text_1_6.png");
            _sprMagnificationSPBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            _sprMagnificationSPBox->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 80);
            _spriteContainer->addChild(_sprMagnificationSPBox);
            {
                auto lbContent = Label::createWithTTF(GAME_TEXTFORMAT("t_pet_equip_box_special", nMagnification), GAME_FONT, 24);
                lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                lbContent->setDimensions(_sprMagnificationSPBox->getContentSize().width - 50, _sprMagnificationSPBox->getContentSize().height);
                lbContent->enableOutline(Color4B(Color3B(192, 3, 0)), 1);
                lbContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
                lbContent->setPosition(_sprMagnificationSPBox->getContentSize()/2);
                lbContent->setName("TEXT");
                _sprMagnificationSPBox->addChild(lbContent);
            }
        }
        else
        {
            _sprMagnificationSPBox->setVisible(true);
            
            auto lbContent = (Label*)_sprMagnificationSPBox->getChildByName("TEXT");
            if(lbContent != nullptr)
            {
                int nMagnification = 0;
                if(_lisDrawGrade.size() > 0)
                {
                    auto obj = _lisDrawGrade.at(0);
                    if(obj != nullptr)
                        nMagnification = obj->getGrade();
                }
                lbContent->setString(GAME_TEXTFORMAT("t_pet_equip_box_special", nMagnification));
            }
            
        }
    }
    else
    {
        if(_sprMagnificationSPBox != nullptr)
        {
            _sprMagnificationSPBox->setVisible(false);
        }
        
        for ( int i = 0; i < _lisDrawGrade.size(); ++i )
        {
            auto obj = _lisDrawGrade.at(i);
            
            std::string strGrade = "X1";
            std::string strGradePath = "Assets/ui/pet/event_score_1_1.png";
            Color3B colorGrade = Color3B(219, 219, 219);
            if ( obj->getGrade() == 2 )
            {
            strGrade = "X1.5";
            }
            else if ( obj->getGrade() == 3 )
            {
                strGrade = "X2";
                colorGrade = Color3B(77,219,0);
            }
            else if ( obj->getGrade() == 4 )
            {
                strGrade = "X5";
                strGradePath = "Assets/ui/pet/event_score_1_2.png";
                colorGrade = Color3B(13,255,19);
            }
            else if ( obj->getGrade() == 5 )
            {
                strGrade = "X10";
                strGradePath = "Assets/ui/pet/event_score_1_3.png";
                colorGrade = Color3B(255,247,10);
            }
            
            auto spriteGrade = Sprite::create(strGradePath);
            spriteGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            
            Vec2 startPos = Vec2(_spriteContainer->getContentSize().width/2 , _spriteContainer->getContentSize().height - 375);
            
            startPos += Vec2::LEFT * ((spriteGrade->getContentSize().width - spriteGrade->getContentSize().width/3) * (_lisDrawGrade.size()/2));
            
            spriteGrade->setPosition(startPos + Vec2::RIGHT * (i * (spriteGrade->getContentSize().width - spriteGrade->getContentSize().width/3)));
            spriteGrade->setScale(0.6f);
            _spriteContainer->addChild(spriteGrade,E_ZORER::UI);
            _listBoxBonusBadge.pushBack(spriteGrade);
                    
            auto labelCount = MafNode::createBmFont(strGrade, GAME_BM_FONT_AL, 40);
            labelCount->setPosition(spriteGrade->getContentSize().width / 2, spriteGrade->getContentSize().height / 2 + 10);
            labelCount->setColor(colorGrade);
            labelCount->enableBold();
            spriteGrade->addChild(labelCount);
        }
    }
    
    _sprBox->runAction(sequence);
}

#pragma mark - utils
void PopupPetEquipBoxReward::onClick(cocos2d::Ref* sender)
{
    auto item = (Node*) sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        onHide();
    }
    else if(item->getName().compare("BOX_USE") == 0)
    {
        if(_isEvent == true)
            onUseBox(item->getTag(), 2);
        else
            onUseBox(item->getTag(), 1);
    }
    else if(item->getName().compare("BOX_USE_SP") == 0)
    {
        onUseBox(1, 3);
    }
    else if(item->getName().compare("HELP") == 0)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_pet_equip_box_guide"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->setContentSize(Size(655, 825));
        popup->setContentAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        popup->show();
    }
}
void PopupPetEquipBoxReward::onHide()
{
    if(_callbackRedraw != nullptr)
        _callbackRedraw();
    
    if(_callbackHide != nullptr)
        _callbackHide();
    
    hide();
}

void PopupPetEquipBoxReward::onUseBox(int nUse, int nType)
{
    int petEquipMaxUseCount = PetNewManager::getInstance()->getPetEquipMaxUseCount();
    
    if(petEquipMaxUseCount >= PetNewManager::getInstance()->getListPetEquipAll().size())
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_pet_equip_warn_limit_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;;
    }
    else
    {
        auto strSaveTime = UserDefault::getInstance()->getStringForKey("PET_EQUIP_MAX_WARNING", "0");
        
        auto time = UtilsDate::getInstance()->getTime();
        auto saveTime = std::atoi(strSaveTime.c_str());
        
        int diff = difftime(time, saveTime) / 86400;
        
        if(diff <= 0)
        {
            if(petEquipMaxUseCount >= 1)
            {
                auto popup = PopupPetEquipUseMaxWarning::create(nUse, nType);
                popup->setCallback(CC_CALLBACK_2(PopupPetEquipBoxReward::callbackUseBox, this));
                popup->show();
                return;
            }
        }
    }
    
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    switch ((E_PET_BOX_TYPE)nType)
    {
        case E_PET_BOX_TYPE::COMMON:
        {
            int nBoxCount = PetEquipBoxManager::getInstance()->getEquipBoxCount();
            
            if ( nBoxCount < nUse )
            {
                return;
            }
        }
            break;
            
        case E_PET_BOX_TYPE::EVENT:
        {
            int nBoxCount = PetEquipBoxManager::getInstance()->getEquipBoxEventCount();
            
            if ( nBoxCount < nUse )
            {
                return;
            }
        }break;
            
        case E_PET_BOX_TYPE::CASH:
        {
            int nBoxCount = PetEquipBoxManager::getInstance()->getEquipBoxSpecialCount();
            
            if ( nBoxCount < nUse )
            {
                return;
            }
        }
            break;
    }
    
    _eType = (E_PET_BOX_TYPE)nType;
    
    PopupLoading::show();
    if(PetEquipBoxManager::getInstance()->getEquipBoxMileage() >= ParameterManager::getInstance()->getParameterInteger("pet_equip_box_mileage"))
    {
        _isMileage = true;
    }
    else
    {
        _isMileage = false;
    }
    PetEquipBoxManager::getInstance()->requestEquipBoxUse(nUse, nType,  CC_CALLBACK_4(PopupPetEquipBoxReward::callbackUse, this));
}


#pragma mark - get : set

void PopupPetEquipBoxReward::setCallbackRedraw(const std::function<void(void)>& pCallback)
{
    _callbackRedraw = pCallback;
}

void PopupPetEquipBoxReward::setCallbackHide(const std::function<void(void)>& pCallback)
{
    _callbackHide = pCallback;
}

#pragma mark - callback
void PopupPetEquipBoxReward::callbackUse(bool bResult, int nUse, cocos2d::Vector<InfoPetEquipDraw*> listDraw, cocos2d::Vector<InfoPetEquipDrawGrade*> lisDrawGrade)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        onHide();
        return;
    }
    ServerMissionManager::getInstance()->setServerMission(E_DAILY_QUEST::PET_EQUIP_BOX_USE, nUse);
    _listDraw.clear();
    _lisDrawGrade.clear();
    
    _listDraw = listDraw;
    _lisDrawGrade = lisDrawGrade;
    
    
    //
    uiContents();
    actionBoxOpen(nUse);
    
    if(_callbackRedraw != nullptr)
        _callbackRedraw();
}

void PopupPetEquipBoxReward::callbackRedraw()
{
    uiContents();
}

void PopupPetEquipBoxReward::callbackUseBox(int nUse, int nType)
{
    switch ((E_PET_BOX_TYPE)nType)
    {
        case E_PET_BOX_TYPE::COMMON:
        {
            int nBoxCount = PetEquipBoxManager::getInstance()->getEquipBoxCount();
            
            if ( nBoxCount < nUse )
            {
                return;
            }
        }break;
            
        case E_PET_BOX_TYPE::EVENT:
        {
            int nBoxCount = PetEquipBoxManager::getInstance()->getEquipBoxEventCount();
            
            if ( nBoxCount < nUse )
            {
                return;
            }
        }break;
            
        case E_PET_BOX_TYPE::CASH:
        {
            int nBoxCount = PetEquipBoxManager::getInstance()->getEquipBoxSpecialCount();
            
            if ( nBoxCount < nUse )
            {
                return;
            }
        }
            break;
    }
    
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    _eType = (E_PET_BOX_TYPE)nType;
    
    PopupLoading::show();
    PetEquipBoxManager::getInstance()->requestEquipBoxUse(nUse, nType,  CC_CALLBACK_4(PopupPetEquipBoxReward::callbackUse, this));
}

void PopupPetEquipBoxReward::callbackPurchase()
{
    PetEquipBoxManager::getInstance()->requestEquipBoxInfo(CC_CALLBACK_0(PopupPetEquipBoxReward::callbackRedraw, this));
}
