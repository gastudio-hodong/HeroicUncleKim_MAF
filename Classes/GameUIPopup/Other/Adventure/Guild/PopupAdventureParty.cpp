//
//  PopupAdventureParty.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/18.
//

#include "PopupAdventureParty.h"

#include "GameObject/InfoSetting.h"
#include "GameObject/Adventure/InfoAdventureUpgradeAbility.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Common/PopupButtonForceTutorial.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureMemberUpgrade.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventurePartyAbilityDrawInfo.h"
#include "GameUIPopup/Other/Adventure/PopupAdventureTutorial.h"
#include "GameUIPopup/Other/Adventure/Guild/PopupAdventureFurnitureUpgrade.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AdventureManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupAdventureParty* PopupAdventureParty::create(int nParty)
{
    PopupAdventureParty* pRet = new(std::nothrow) PopupAdventureParty();
    if ( pRet && pRet->init(nParty) )
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

PopupAdventureParty::PopupAdventureParty(void):
_bUpgrade(false),
_isFilterArcana(true),
_isDrawStop(false),
_nDrawCount(0),
_nCurrentParty(1),
_nCurrentPresetIdx(1),
_fDrawTimer(0),
_fDrawTimerMax(0.2f),
_infoParty(nullptr),
_infoCurrentMember(nullptr),
_lyTopUI(nullptr),
_lyMiddleUI(nullptr),
_lyAbility(nullptr),
_lyBottomUI(nullptr),
_btnTutorial(nullptr),
callbackRefresh(nullptr),
_table(nullptr)
{
}

PopupAdventureParty::~PopupAdventureParty(void)
{
}


#pragma mark - table
void PopupAdventureParty::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAdventureParty::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 80);
}

extension::TableViewCell* PopupAdventureParty::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    Size size = tableCellSizeForIndex(table, idx);
    cell->setContentSize(size);
    
    auto listAbility = _infoPreset->getListAblity();
    auto listSeal = _infoPreset->getListSeal();
    
    auto obj = listAbility.at(idx);
    int nAbilityIdx = obj->getIdx();
    int nAbilityGrade = obj->getGrade();
    bool isOpen = obj->isOpen();
    int nPartyLv = AdventureManager::getInstance()->getPartyLevel(_nCurrentParty);
    
    std::string strBoxPath = "Assets/ui/adventure/adventure_bonus_list_1_%d.png";
    
    if(!isOpen)
        strBoxPath = "Assets/ui/adventure/adventure_bonus_list_1_lock.png";
    else
    {
        if(nAbilityGrade == 0)
            strBoxPath = "Assets/ui/adventure/adventure_bonus_list_1_lock.png";
        else
            strBoxPath = MafUtils::format(strBoxPath.c_str(), nAbilityGrade);
    }
    //base
    
    auto sprBG = Sprite::create(strBoxPath);
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(Vec2(cell->getContentSize()/2));
    cell->addChild(sprBG);
    {
        std::string strAbility = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", nAbilityIdx));
        strAbility += MafUtils::format(" %s", AdventureManager::getPropertyEffectText(obj->getType(), obj->getEffect(nPartyLv)).c_str());
        if(isOpen)
        {
            auto lbAbilityGrade = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_party_ability_grade_%d", nAbilityGrade)), GAME_FONT, 25);
            lbAbilityGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lbAbilityGrade->setPosition(50, sprBG->getContentSize().height/2);
            lbAbilityGrade->setDimensions(180, sprBG->getContentSize().height/2);
            lbAbilityGrade->setOverflow(cocos2d::Label::Overflow::SHRINK);
            lbAbilityGrade->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
            sprBG->addChild(lbAbilityGrade);
        }
        else
        {
            auto sprSeal = Sprite::create("Assets/ui_common/tap_lock_2.png");
            sprSeal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprSeal->setPosition(70,sprBG->getContentSize().height/2);
            sprBG->addChild(sprSeal);
            
            strAbility = GAME_TEXT("t_ui_advt_party_ability_unlock");
        }
        
        auto lbAbilityName = Label::createWithTTF(strAbility, GAME_FONT, 25);
        lbAbilityName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbAbilityName->setPosition(190, sprBG->getContentSize().height/2);
        lbAbilityName->setDimensions(sprBG->getContentSize().width - 270, sprBG->getContentSize().height/2);
        lbAbilityName->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbAbilityName->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        sprBG->addChild(lbAbilityName);
        
    }
    if(isOpen && obj->getType() != E_ADVENTURE_PROPERTY_TYPE::NONE)
    {
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        cell->addChild(menu);
        
        std::string strCheckBoxPath = "Assets/ui_common/checkbox_1_off.png";
        if(listSeal[idx])
            strCheckBoxPath = "Assets/ui_common/checkbox_1_on.png";
        
        auto btnSeal = MafNode::MafMenuItemSprite::create(Sprite::create(strCheckBoxPath), nullptr, CC_CALLBACK_1(PopupAdventureParty::onClickSeal, this));
        btnSeal->setTag((int)idx);
        btnSeal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btnSeal->setPosition(Vec2(cell->getContentSize().width - 80, cell->getContentSize().height/2));
        btnSeal->setTouchRect(btnSeal->getContentSize()*2);
        menu->addChild(btnSeal);
    }
    
    return cell;
}

ssize_t PopupAdventureParty::numberOfCellsInTableView(extension::TableView *table)
{
    return _infoPreset->getListAblity().size();
}

#pragma mark -init
bool PopupAdventureParty::init(int nParty)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _nCurrentParty = nParty;
    _infoParty = AdventureManager::getInstance()->getInfoParty(_nCurrentParty);
    _infoCurrentMember = _infoParty->getMember(1);
    _nCurrentPresetIdx = _infoParty->getPresetIdx();
    _infoPreset = _infoParty->getAblityPreset(_nCurrentPresetIdx);
    _bHideBackKey = false;
    
    initVar();
    initUI();
    scheduleUpdate();
    
    showTutorial();
    if(AdventureManager::getInstance()->getTutorialStep() <= 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}

void PopupAdventureParty::onEnter()
{
    PopupBase::onEnter();
    
}

void PopupAdventureParty::update(float dt)
{
    if (_btnDraw->isSelected() )
    {
        if(_isDrawStop)
        {
            _bUpgrade = false;
            _fDrawTimer = 0;
            _fDrawTimerMax = 0.2f;
            return;
        }
        _fDrawTimer += dt;
        if (_fDrawTimerMax <= _fDrawTimer)
        {
            _fDrawTimer = 0;
            _fDrawTimerMax -= 0.02f;
            
            _bUpgrade = true;
            
            onClickDraw(nullptr);
        }
        
    }
    else
    {
        _fDrawTimer = 0;
        _fDrawTimerMax = 0.2f;
        if( _bUpgrade == true )
            _bUpgrade = false;
        if( _isDrawStop == true )
            _isDrawStop = false;
    }
}
#pragma mark - Override
void PopupAdventureParty::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureParty::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
#pragma mark - Tutorial
void PopupAdventureParty::showButtonForceTutorial()
{
    if(_btnTutorial == nullptr)
        return;
    
    auto popup = PopupButtonForceTutorial::create();
    popup->setButton(_btnTutorial);
    //if(AdventureManager::getInstance()->getTutorialStep() == 6)
             //popup->setCallbackListener(CC_CALLBACK_1(PopupAdventureParty::onClickPartyUpgrade, this));
    popup->show();
    
    _btnTutorial = nullptr;
}

void PopupAdventureParty::showTutorial()
{
    if(AdventureManager::getInstance()->getTutorialStep() != 6)
        return;
    
    auto popup = PopupAdventureTutorial::create(AdventureManager::getInstance()->getTutorialStep());
    popup->show();
}

void PopupAdventureParty::setCallbackRefresh(const std::function<void(void)>& callback)
{
    callbackRefresh = callback;
}

void PopupAdventureParty::initVar()
{
    _spriteContainer->setVisible(false);
    
    _sprContainerAbility = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale.png", Rect::ZERO, Rect(70,110,10,10));
    _sprContainerAbility->setContentSize(Size(750, 850));
    _sprContainerAbility->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _sprContainerAbility->setPosition(Vec2(getContentSize().width / 2, 0) );
    addChild(_sprContainerAbility);
    
    _sprContainerMember = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale.png", Rect::ZERO, Rect(70,110,10,10));
    _sprContainerMember->setContentSize(Size(750, 480));
    _sprContainerMember->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _sprContainerMember->setPosition(_sprContainerAbility->getPosition() + Vec2::UP * _sprContainerAbility->getContentSize().height);
    addChild(_sprContainerMember);
}

void PopupAdventureParty::initUI()
{
    // Member title
    auto labelMemberTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_crew_title_1"), GAME_FONT, 28);
    labelMemberTitle->setColor(COLOR_TITLE);
    labelMemberTitle->setPosition( Vec2(_sprContainerMember->getContentSize().width / 2, _sprContainerMember->getContentSize().height - 37.5) );
    _sprContainerMember->addChild(labelMemberTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _sprContainerMember->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureParty::onClickClose, this));
    itemClose->setPosition( Vec2(_sprContainerMember->getContentSize().width - itemClose->getContentSize().width, _sprContainerMember->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    // Ability title
    
    std::string strNum = "";
    if(_nCurrentParty == 1)
        strNum = "Ⅰ";
    else if(_nCurrentParty == 2)
        strNum = "Ⅱ";
    else if(_nCurrentParty == 3)
        strNum = "Ⅲ";
    
    auto labelAbilityTitle = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_party_ability_title_3", strNum.c_str()), GAME_FONT, 28);
    labelAbilityTitle->setColor(COLOR_TITLE);
    labelAbilityTitle->setName("TITLE");
    labelAbilityTitle->setPosition( Vec2(_sprContainerAbility->getContentSize().width / 2, _sprContainerAbility->getContentSize().height - 37.5) );
    _sprContainerAbility->addChild(labelAbilityTitle);
    
    //650
    Size size = _sprContainerMember->getContentSize();
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize( Size(size.width - 10, _sprContainerMember->getContentSize().height - 75));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyTopUI->setPosition( Vec2(_sprContainerMember->getContentSize().width / 2, _sprContainerMember->getContentSize().height - 75));
    _sprContainerMember->addChild(_lyTopUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(size.width - 10, 170));
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition(_sprContainerAbility->getContentSize().width / 2, _sprContainerAbility->getContentSize().height - 75);
    _sprContainerAbility->addChild(_lyMiddleUI);
    
    _lyAbility = LayerColor::create();
    _lyAbility->setContentSize(Size(size.width - 10, _sprContainerAbility->getContentSize().height - _lyMiddleUI->getContentSize().height - 270 - 75));
    _lyAbility->setIgnoreAnchorPointForPosition(false);
    _lyAbility->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyAbility->setPosition(_lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    _sprContainerAbility->addChild(_lyAbility);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(size.width - 10, 270));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyAbility->getPosition() + Vec2::DOWN * _lyAbility->getContentSize().height);
    _sprContainerAbility->addChild(_lyBottomUI);
    
    uiTop();
    uiMiddle();
    uiTable();
    uiBottom();
}

#pragma mark -ui
void PopupAdventureParty::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    std::string strNum = "";
    if(_nCurrentParty == 1)
        strNum = "Ⅰ";
    else if(_nCurrentParty == 2)
        strNum = "Ⅱ";
    else if(_nCurrentParty == 3)
        strNum = "Ⅲ";
    
    auto labelAbilityTitle = (Label*)_sprContainerAbility->getChildByName("TITLE");
    if(labelAbilityTitle != nullptr)
        labelAbilityTitle->setString(GAME_TEXTFORMAT("t_ui_advt_party_ability_title_3", strNum.c_str()));
    std::vector<int> listTab = {1,2,3,};
    int size = listTab.size();
    
    float width = _lyTopUI->getContentSize().width / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2);
    
    for(int i = 0; i < listTab.size(); ++i)
    {
        auto nPartyIdx = listTab[i];
        
        auto objParty = AdventureManager::getInstance()->getInfoParty(nPartyIdx);
        
        std::string strPath = "Assets/ui_common/btn_tap_off.png";
        if(nPartyIdx == _nCurrentParty)
        {
            strPath = "Assets/ui_common/btn_tap_on.png";
        }
        
        std::string strNum = "";
        if(nPartyIdx == 1)
            strNum = " Ⅰ";
        else if(nPartyIdx == 2)
            strNum = " Ⅱ";
        else if(nPartyIdx == 3)
            strNum = " Ⅲ";
                
        std::string strText = GAME_TEXT("t_ui_advt_crew_title_1") + strNum;
        
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 60));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnTab->setPosition(Vec2(posX, _lyTopUI->getContentSize().height - 5));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickTab, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)nPartyIdx);
        _lyTopUI->addChild(btnTab);
        {
            if(!objParty->isOpen())
            {
                auto sprSealIcon = Sprite::create("Assets/ui_common/tap_lock.png");
                sprSealIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                sprSealIcon->setPosition(Vec2(25, btnTab->getContentSize().height/2));
                btnTab->addChild(sprSealIcon);
                btnTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickFurnitureUpgrade, this));
            }
            
            auto lbText = Label::createWithTTF(strText, GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnTab->getContentSize().width/2, btnTab->getContentSize().height/2);
            lbText->setDimensions(btnTab->getContentSize().width - 60, lbText->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbText->setOverflow(Label::Overflow::SHRINK);
            btnTab->addChild(lbText);
            if(nPartyIdx != _nCurrentParty)
            {
                lbText->setColor(Color3B(90,90,90));
            }
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        
        posX += width ;
    }
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprBox->setContentSize(Size(_lyTopUI->getContentSize().width - 5, 320));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBox->setPosition(_lyTopUI->getContentSize().width/2, 15);
    _lyTopUI->addChild(sprBox);
    {
        auto sprBg = Sprite::create("Assets/ui/adventure/adventure_crew_bg_01.png");
        sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBg->setScale(4.f);
        sprBg->setPosition(sprBox->getContentSize()/2);
        sprBox->addChild(sprBg);
    }
    
    auto listMember = _infoParty->getListMemeber();
    
    width = 136;
    posX = sprBox->getContentSize().width/2 - width * (listMember.size()/2) - 10 * (listMember.size()/2) ;
    for(int i = 0; i < listMember.size(); ++i)
    {
        auto objMember = listMember.at(i);
        
        std::string strBoxPath = "Assets/ui/adventure/btn_crew_icon_slot1_1.png";
        if(!objMember->isOpen())
            strBoxPath = "Assets/ui/pet/btn_pet_icon_slot2_1.png";
        
        auto btnMember = ui::Button::create(strBoxPath);
        btnMember->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickMember, this));
        btnMember->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnMember->setPosition(Vec2(posX, sprBox->getContentSize().height - 20));
        btnMember->setTag(i + 1);
        btnMember->setZoomScale(0.05f);
        sprBox->addChild(btnMember);
        
        if(objMember->isOpen())
        {
            auto sprIcon = Sprite::create(MafUtils::format("Assets/ui/adventure/icon_crew_%d_%d.png", objMember->getImageIdx(), objMember->getImageGrade()));
            sprIcon->getTexture()->setTexParameters(texParams);
            sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprIcon->setPosition(btnMember->getContentSize()/2);
            sprIcon->setScale((btnMember->getContentSize().width - 20) / sprIcon->getContentSize().width);
            btnMember->addChild(sprIcon);
            
            if(_infoCurrentMember->getImageIdx() == objMember->getImageIdx())
            {
                auto sprRelicSelect = Sprite::create("Assets/ui/pet/btn_pet_icon_slot1_2.png");
                sprRelicSelect->setPosition(btnMember->getContentSize().width / 2, btnMember->getContentSize().height / 2);
                btnMember->addChild(sprRelicSelect);
                
                auto fadeOut = FadeOut::create(0.5f);
                auto fadeIn = FadeIn::create(0.5f);
                auto sequience = Sequence::create(fadeOut, fadeIn, nullptr);
                sprRelicSelect->runAction(RepeatForever::create(sequience));
            }
        }
        else
        {
            btnMember->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickFurnitureUpgrade, this));
        }
        
        if(listMember.size() % 2 == 0)
            btnMember->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        
        posX += width + 10;
    }
    
    auto sprMemberAbilityBox = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_12.png");
    sprMemberAbilityBox->setContentSize(Size(sprBox->getContentSize().width - 10, 126));
    sprMemberAbilityBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprMemberAbilityBox->setPosition(sprBox->getContentSize().width/2, 20);
    sprBox->addChild(sprMemberAbilityBox);
    {
        auto lbTotalLv = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_party_ability_title_1", AdventureManager::getInstance()->getPartyLevel(_nCurrentParty)), GAME_FONT, 24);
        lbTotalLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbTotalLv->setPosition(20, sprMemberAbilityBox->getContentSize().height - 5);
        lbTotalLv->setDimensions(sprMemberAbilityBox->getContentSize().width - 200, lbTotalLv->getContentSize().height);
        lbTotalLv->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbTotalLv->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        sprMemberAbilityBox->addChild(lbTotalLv);
        
        auto lbMemberName = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_member_%d", _infoCurrentMember->getIdx())) + MafUtils::format(" Lv. %d", _infoCurrentMember->getCurrentLv()), GAME_FONT, 27);
        lbMemberName->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbMemberName->setPosition(lbTotalLv->getPosition() + Vec2::DOWN * lbTotalLv->getContentSize().height);
        lbMemberName->setDimensions(sprMemberAbilityBox->getContentSize().width - 200, lbMemberName->getContentSize().height);
        lbMemberName->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbMemberName->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        lbMemberName->setColor(COLOR_TITLE);
        sprMemberAbilityBox->addChild(lbMemberName);
        
        auto objProperty = _infoCurrentMember->getInfoUpgradeAbility();
        double fCurrentEffect = 0;
        
        if(objProperty != nullptr)
            fCurrentEffect = objProperty->getEffect();
        
        std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)_infoCurrentMember->getType()));
        
        strEffect += MafUtils::format(" %s", AdventureManager::getPropertyEffectText(_infoCurrentMember->getType(), fCurrentEffect).c_str());
        
        auto lbEffect = Label::createWithTTF(strEffect, GAME_FONT, 20);
        lbEffect->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        lbEffect->setPosition(lbMemberName->getPosition() + Vec2::DOWN * lbMemberName->getContentSize().height);
        lbEffect->setDimensions(sprMemberAbilityBox->getContentSize().width - 200, lbEffect->getContentSize().height);
        lbEffect->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
        lbEffect->setOverflow(cocos2d::Label::Overflow::SHRINK);
        sprMemberAbilityBox->addChild(lbEffect);
        
        if(_infoCurrentMember->getCurrentLv() < _infoCurrentMember->getMaxLv())
        {
            std::string strPath = "Assets/ui_common/btn_upgrade_bg.png";
            if(!isUpgradeCondition())
                strPath = "Assets/ui_common/btn_upgrade_max.png";
            
            auto btnUpgrade = ui::Button::create(strPath);
            btnUpgrade->setScale9Enabled(true);
            btnUpgrade->setContentSize(Size(150,116));
            btnUpgrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            btnUpgrade->setPosition(Vec2(sprMemberAbilityBox->getContentSize().width - 15, sprMemberAbilityBox->getContentSize().height/2));
            btnUpgrade->setZoomScale(0.05f);
            btnUpgrade->setTag(_infoCurrentMember->getIdx());
            btnUpgrade->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickUpgrade, this));
            btnUpgrade->setTitleText("LEVEL UP");
            btnUpgrade->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
            btnUpgrade->getTitleLabel()->setDimensions(btnUpgrade->getContentSize().width, btnUpgrade->getContentSize().height);
            btnUpgrade->setTitleFontSize(25);
            sprMemberAbilityBox->addChild(btnUpgrade);
        }
    }
}

void PopupAdventureParty::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);

    auto uiCurrency = UICurrency::create(list, Size(220, 40));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiCurrency->setPosition(Vec2(_lyMiddleUI->getContentSize().width, _lyMiddleUI->getContentSize().height-25));
    _lyMiddleUI->addChild(uiCurrency, E_ZORER::UI+1);
    
    auto btnInfo = ui::Button::create("Assets/ui_common/base_text_2_4.png");
    btnInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnInfo->setPosition(Vec2(5, _lyMiddleUI->getContentSize().height));
    btnInfo->setZoomScale(0.05f);
    btnInfo->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickHelp, this));
    _lyMiddleUI->addChild(btnInfo);
    {
        auto sprIcon = Sprite::create("Assets/ui/pet/btn_pet_info.png");
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprIcon->setPosition(Vec2(5, btnInfo->getContentSize().height/2));
        btnInfo->addChild(sprIcon);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_raidshop_spiritbox_drawrate_pop_title"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        lbText->setDimensions(btnInfo->getContentSize().width - 5 - sprIcon->getContentSize().width, btnInfo->getContentSize().height - 5);
        lbText->setPosition(sprIcon->getPosition() + Vec2::RIGHT * sprIcon->getContentSize().width);
        btnInfo->addChild(lbText);
    }
    
    int nPresetCount = 3;
    auto objSettingPresetCount = AdventureManager::getInstance()->getSetting("party_ability_preset_count");
    
    if(objSettingPresetCount != nullptr)
        nPresetCount = objSettingPresetCount->getValueToInt();
    
    std::vector<int> listPreset;
    listPreset.clear();
    for(int i = 0; i < nPresetCount; ++i)
    {
        listPreset.push_back(i+1);
    }
    
    int size = listPreset.size();
    
    float width = _lyTopUI->getContentSize().width / size - 10;
    float posX = _lyTopUI->getContentSize().width/2 - width * (size/2);
    
    for(int i = 0; i < listPreset.size(); ++i)
    {
        auto nPresetNum = listPreset[i];
        
        auto objParty = AdventureManager::getInstance()->getInfoParty(nPresetNum);
        
        std::string strPath = "Assets/ui_common/btn_1_off.png";
        if(nPresetNum == _nCurrentPresetIdx)
        {
            strPath = "Assets/ui_common/btn_1_2_on.png";
        }
        
        std::string strNum = "";
        if(nPresetNum == 1)
            strNum = " Ⅰ";
        else if(nPresetNum == 2)
            strNum = " Ⅱ";
        else if(nPresetNum == 3)
            strNum = " Ⅲ";
                
        std::string strText = "Preset" + strNum;
        
        auto btnTab = ui::Button::create(strPath);
        btnTab->setScale9Enabled(true);
        btnTab->setContentSize(Size(width, 60));
        btnTab->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnTab->setPosition(Vec2(posX, _lyMiddleUI->getContentSize().height - 50));
        btnTab->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickPreset, this));
        btnTab->setZoomScale(0.05f);
        btnTab->setTag((int)nPresetNum);
        _lyMiddleUI->addChild(btnTab);
        {
            auto lbText = Label::createWithTTF(strText, GAME_FONT, 25);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnTab->getContentSize().width/2, btnTab->getContentSize().height/2);
            lbText->setDimensions(btnTab->getContentSize().width - 60, lbText->getContentSize().height);
            lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            lbText->setOverflow(Label::Overflow::SHRINK);
            lbText->setColor(Color3B(90,90,90));
            btnTab->addChild(lbText);
            if(nPresetNum == _nCurrentPresetIdx)
            {
                lbText->setColor(COLOR_TITLE);
            }
            
        }
        
        if(size % 2 == 0)
            btnTab->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        
        posX += width ;
    }
    
    auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box.png");
    sprBox->setContentSize(Size(723, 50));
    sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBox->setPosition(_lyMiddleUI->getContentSize().width/2, 5);
    _lyMiddleUI->addChild(sprBox);
    {
        //t_ui_advt_party_ability_title_1
        auto lbGrade = Label::createWithTTF(GAME_TEXT("t_ui_grade_1"), GAME_FONT, 24);
        lbGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbGrade->setPosition(10, sprBox->getContentSize().height/2);
        lbGrade->setDimensions(sprBox->getContentSize().width - 100, sprBox->getContentSize().height - 10);
        lbGrade->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbGrade->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        sprBox->addChild(lbGrade);
        
        auto lbAbilityTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_party_ability_bt"), GAME_FONT, 24);
        lbAbilityTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbAbilityTitle->setPosition(sprBox->getContentSize()/2);
        lbAbilityTitle->setDimensions(200, sprBox->getContentSize().height - 10);
        lbAbilityTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbAbilityTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
        sprBox->addChild(lbAbilityTitle);
        
        auto lbSeal = Label::createWithTTF(GAME_TEXT("t_ui_advt_party_ability_title_2"), GAME_FONT, 24);
        lbSeal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbSeal->setPosition(sprBox->getContentSize().width - 77, sprBox->getContentSize().height/2);
        lbSeal->setDimensions(120, sprBox->getContentSize().height - 10);
        lbSeal->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbSeal->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        sprBox->addChild(lbSeal);
    }
}
void PopupAdventureParty::uiTable()
{
    _lyAbility->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_lyAbility->getContentSize().width, _lyAbility->getContentSize().height - 5));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition(_lyAbility->getContentSize().width / 2, _lyAbility->getContentSize().height - 5);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _lyAbility->addChild(_table);
    
    _table->reloadData();
}

void PopupAdventureParty::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    std::string path = "Assets/ui/adventure/btn_tier_filter1_2.png";
    std::string text = GAME_TEXT("t_ui_advt_party_ability_grade_5");
    
    if(_isFilterArcana == false)
    {
        path = "Assets/ui/adventure/btn_tier_filter1_1.png";
        text = GAME_TEXT("t_ui_advt_party_ability_grade_4");
    }
    _btnFilter = ui::Button::create(path);
    _btnFilter->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _btnFilter->setPosition(Vec2(_lyBottomUI->getContentSize().width - 10, _lyBottomUI->getContentSize().height - 5));
    _btnFilter->addClickEventListener(CC_CALLBACK_1(PopupAdventureParty::onClickFilter, this));
    _btnFilter->setZoomScale(0.05f);
    _lyBottomUI->addChild(_btnFilter);
    {
        auto lbTextGrade4 = Label::createWithTTF(GAME_TEXT("t_ui_advt_party_ability_grade_4"), GAME_FONT, 23);
        lbTextGrade4->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbTextGrade4->setPosition(Vec2(_btnFilter->getContentSize().width/4, _btnFilter->getContentSize().height/2));
        lbTextGrade4->setDimensions(_btnFilter->getContentSize().width/2, _btnFilter->getContentSize().height - 10);
        lbTextGrade4->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbTextGrade4->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbTextGrade4->setName("GRADE_4");
        _btnFilter->addChild(lbTextGrade4);
        
        auto lbTextGrade5 = Label::createWithTTF(GAME_TEXT("t_ui_advt_party_ability_grade_5"), GAME_FONT, 23);
        lbTextGrade5->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbTextGrade5->setPosition(Vec2(_btnFilter->getContentSize().width - _btnFilter->getContentSize().width/4, _btnFilter->getContentSize().height/2));
        lbTextGrade5->setDimensions(_btnFilter->getContentSize().width/2, _btnFilter->getContentSize().height - 10);
        lbTextGrade5->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbTextGrade5->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbTextGrade5->setName("GRADE_5");
        _btnFilter->addChild(lbTextGrade5);
        if(_isFilterArcana)
        {
            lbTextGrade4->setColor(Color3B(128,128,128));
            lbTextGrade5->setColor(Color3B(255,255,255));
        }
        else
        {
            lbTextGrade4->setColor(Color3B(255,255,255));
            lbTextGrade5->setColor(Color3B(128,128,128));
        }
    }
    
    _lbTextFilter = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_advt_party_ability_msg_1", text.c_str()), GAME_FONT, 24);
    _lbTextFilter->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _lbTextFilter->setPosition(_btnFilter->getPosition() + Vec2::LEFT * (_btnFilter->getContentSize().width + 30));
    _lbTextFilter->setDimensions(_lyBottomUI->getContentSize().width - _btnFilter->getContentSize().width - 40, _lbTextFilter->getContentSize().height);
    _lbTextFilter->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lbTextFilter->setAlignment(cocos2d::TextHAlignment::RIGHT, cocos2d::TextVAlignment::TOP);
    _lyBottomUI->addChild(_lbTextFilter);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    _lyBottomUI->addChild(menu);
    
    _btnDraw = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg.png"), nullptr, CC_CALLBACK_1(PopupAdventureParty::onClickDraw, this));
    _btnDraw->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _btnDraw->setPosition(Vec2(_lyBottomUI->getContentSize().width/2, 15));
    menu->addChild(_btnDraw);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_advt_party_ability_bt"), GAME_FONT, 22);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbText->setPosition(Vec2(_btnDraw->getContentSize()/2) + Vec2::UP * 10 );
        lbText->setDimensions(_btnDraw->getContentSize().width - 10, _btnDraw->getContentSize().height/2 -10);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
        _btnDraw->addChild(lbText);
        
        auto sprGem = Sprite::create("Assets/icon/icon_jewel.png");
        sprGem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprGem->setPosition(40, _btnDraw->getContentSize().height/2 + 10);
        _btnDraw->addChild(sprGem);
        
        auto labelGem = Label::createWithTTF(MafUtils::format("%d", AdventureManager::getInstance()->getDrawPrice(_nCurrentParty)), GAME_FONT, 25);
        labelGem->setColor(COLOR_COIN);
        labelGem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelGem->setPosition(80, _btnDraw->getContentSize().height/2 - 20);
        _btnDraw->addChild(labelGem);
    }
    
    auto lbAutoSave = Label::createWithTTF(GAME_TEXT("t_ui_advt_upgrade_mag_2"), GAME_FONT, 22);
    lbAutoSave->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbAutoSave->setPosition(_btnDraw->getPosition() + Vec2::UP * (_btnDraw->getContentSize().height + 25));
    lbAutoSave->setDimensions(_lyBottomUI->getContentSize().width - 20, lbAutoSave->getContentSize().height);
    lbAutoSave->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
    lbAutoSave->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyBottomUI->addChild(lbAutoSave);
    
}

#pragma mark -click
void PopupAdventureParty::onClickClose(Ref* const pSender)
{
    if(callbackRefresh != nullptr)
        callbackRefresh();
    if(_nDrawCount <= 0)
    {
        hide();
        return;
    }
    
    PopupLoading::show();
    SaveManager::getInstance()->requestSave(CC_CALLBACK_1(PopupAdventureParty::callbackSaveHide, this));
}

void PopupAdventureParty::onClickHelp(cocos2d::Ref* sender)
{
    auto popup = PopupAdventurePartyAbilityDrawInfo::create();
    popup->show();
}

void PopupAdventureParty::onClickTab(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    int nParty = node->getTag();
    
    auto objParty = AdventureManager::getInstance()->getInfoParty(nParty);
    
    if(!objParty->isOpen())
        return;
    
    _nCurrentParty = nParty;
    _infoParty = objParty;
    _infoCurrentMember = _infoParty->getMember(1);
    _nCurrentPresetIdx = _infoParty->getPresetIdx();
    _infoPreset = _infoParty->getAblityPreset(_nCurrentPresetIdx);
    
    uiTop();
    uiMiddle();
    uiBottom();
    _table->reloadData();
}

void PopupAdventureParty::onClickPreset(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    _nCurrentPresetIdx = node->getTag();
    
    _infoParty->setPresetIdx(_nCurrentPresetIdx);
    _infoPreset = _infoParty->getAblityPreset(_nCurrentPresetIdx);
    
    AdventureManager::getInstance()->saveAdventurePartyAbilityData();
    uiMiddle();
    uiBottom();
    _table->reloadData();
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
}

void PopupAdventureParty::onClickUpgrade(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    auto popup = PopupAdventureMemberUpgrade::create(_nCurrentParty, node->getTag());
    popup->setCallbackUpgrade(CC_CALLBACK_0(PopupAdventureParty::callbackUpgrade, this));
    popup->show();
}

void PopupAdventureParty::onClickSeal(cocos2d::Ref* sender)
{
    auto node = (Node*)sender;
    
    int nIdx = node->getTag();
    
    auto listSeal = _infoPreset->getListSeal();
    
    if(nIdx < listSeal.size())
        listSeal[nIdx] = !listSeal[nIdx];
    
    _infoPreset->setListSeal(listSeal);
    AdventureManager::getInstance()->saveAdventurePartyAbilitySeal(_nCurrentParty, _nCurrentPresetIdx);
    uiBottom();
    _table->reloadData();
}

void PopupAdventureParty::onClickDraw(cocos2d::Ref* sender)
{
    auto listAbility = _infoPreset->getListAblity();
    auto listSeal = _infoPreset->getListSeal();
    
    if(_isDrawStop || listAbility.size() == 0)
        return;
    
    for(int i = 0; i < listAbility.size(); ++i)
    {
        bool isSeal = listSeal[i];
        auto obj = listAbility.at(i);
        
        if(!obj->isOpen())
            continue;
        
        if(!isSeal)
        {
            int nFilterGrade = 5;
            
            if(!_isFilterArcana)
                nFilterGrade = 4;
            
            if(obj->getGrade() >= nFilterGrade)
            {
                std::string strText = "{#29CC8B:%s}";
                if(!_isFilterArcana)
                    strText = "{#CC2929:%s}";
                strText = MafUtils::format(strText.c_str(), GAME_TEXT(MafUtils::format("t_ui_advt_party_ability_grade_%d", nFilterGrade)).c_str());
                
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_advt_party_ability_stop_title"), GAME_TEXTFORMAT("t_ui_advt_party_ability_stop_mag", strText.c_str()));
                
                popup->addButton(GAME_TEXT("t_ui_advt_party_ability_bt"), false, [=](Ref*){
                    AdventureManager::getInstance()->requestPartyAbilityDraw(CC_CALLBACK_2(PopupAdventureParty::callbackDraw, this), _nCurrentParty);
                    _isDrawStop = false;
                });
                
                popup->addButton(GAME_TEXT("t_ui_close"), true, [=](Ref*){
                    _isDrawStop = false;
                }, true);
                
                popup->show();
                if(AdventureManager::getInstance()->getTutorialStep() < 7)
                    popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
                _isDrawStop = true;
                return;
            }
        }
    }
    AdventureManager::getInstance()->requestPartyAbilityDraw(CC_CALLBACK_2(PopupAdventureParty::callbackDraw, this), _nCurrentParty);
}
void PopupAdventureParty::onClickFilter(cocos2d::Ref* sender)
{
    _isFilterArcana = !_isFilterArcana;
    
    std::string path = "Assets/ui/adventure/btn_tier_filter1_2.png";
    std::string text = GAME_TEXT("t_ui_advt_party_ability_grade_5");
    
    if(_isFilterArcana == false)
    {
        path = "Assets/ui/adventure/btn_tier_filter1_1.png";
        text = GAME_TEXT("t_ui_advt_party_ability_grade_4");
    }
    
    auto lbTextGrade4 = (Label*)_btnFilter->getChildByName("GRADE_4");
    auto lbTextGrade5 = (Label*)_btnFilter->getChildByName("GRADE_5");
    _btnFilter->loadTextureNormal(path);
    if(_isFilterArcana)
    {
        lbTextGrade4->setColor(Color3B(128,128,128));
        lbTextGrade5->setColor(Color3B(255,255,255));
    }
    else
    {
        lbTextGrade4->setColor(Color3B(255,255,255));
        lbTextGrade5->setColor(Color3B(128,128,128));
    }
    
    _lbTextFilter->setString(GAME_TEXTFORMAT("t_ui_advt_party_ability_msg_1", text.c_str()));
    
}
void PopupAdventureParty::onClickMember(Ref* const pSender)
{
    auto node = (Node*)pSender;
    
    _infoCurrentMember = _infoParty->getMember(node->getTag());
    uiTop();
}

void PopupAdventureParty::onClickFurnitureUpgrade(Ref* const pSender)
{
    if(AdventureManager::getInstance()->getTutorialStep() > 6)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_advt_party_error_title_1"));
        popup->addButton(GAME_TEXT("t_ui_advt_bt_1"), true, [&](Ref*){
            auto popup = PopupAdventureFurnitureUpgrade::create(2);
            popup->setCallbackUpgrade(CC_CALLBACK_0(PopupAdventureParty::callbackFurnitureUpgrade, this));
            popup->show();
            if(AdventureManager::getInstance()->getTutorialStep() < 7)
                popup->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
        });
        popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
        popup->show();
    }
    else
    {
        Vec2 posToast = _spriteContainer->getParent()->convertToWorldSpace(_spriteContainer->getContentSize()/2);
        PopupToast::showImmediately(GAME_TEXT("t_ui_advt_error_7"), posToast.x, posToast.y);
    }
    
}
#pragma mark -utils
bool PopupAdventureParty::isUpgradeCondition() const
{
    bool result = true;
    
    auto objProperty = _infoCurrentMember->getInfoUpgradeAbility(_infoCurrentMember->getCurrentLv() + 1);
    
    for ( int i = 0; i < C_COUNT_MINE_MINERAL; i++ )
    {
        E_MINERAL mineral = (E_MINERAL)(i+1);
        int needCount = 0;
        
        if(objProperty == nullptr)
            continue;
        auto objItems = objProperty->getInfoItems((int)E_ITEMS::MINE_STONE_IRON + i);
        
        if(objItems != nullptr)
            needCount = std::atoi(objItems->getCount().c_str());
        
        if ( MineralManager::getInstance()->getCount(mineral) < needCount )
        {
            result = false;
            break;
        }
    }
    
    auto objItemCost = objProperty->getInfoItems((int)E_ITEMS::GEM);
    if(std::atoi(objItemCost->getCount().c_str()) > ItemsMoneyManager::getInstance()->getGem())
        result = false;
    
    objItemCost = objProperty->getInfoItems((int)E_ITEMS::POINT);
    if(std::atoi(objItemCost->getCount().c_str()) > ItemsMoneyManager::getInstance()->getPoint())
        result = false;
    
    return result;
}
#pragma mark -callback
void PopupAdventureParty::callbackUpgrade()
{
    uiTop();
}

void PopupAdventureParty::callbackDraw(bool bResult, int nResult)
{
    if(bResult == true)
        _nDrawCount++;
    
    if(nResult == 2)
    {
        _isDrawStop = true;
        return;
    }
    
    _infoPreset = _infoParty->getAblityPreset(_nCurrentPresetIdx);
    _table->reloadData();
}

void PopupAdventureParty::callbackSaveHide(bool bResult)
{
    PopupLoading::hide();
    
    hide();
}

void PopupAdventureParty::callbackFurnitureUpgrade()
{
    _table->reloadData();
    uiTop();
    uiMiddle();
    uiBottom();
    
    RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
}
