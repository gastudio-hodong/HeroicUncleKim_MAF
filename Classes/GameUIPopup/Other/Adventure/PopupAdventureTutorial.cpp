//
//  PopupAdventureTutorial.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/25.
//

#include "PopupAdventureTutorial.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureTutorial* PopupAdventureTutorial::create(const int nStep)
{
    PopupAdventureTutorial* pRet = new(std::nothrow) PopupAdventureTutorial();
    if ( pRet && pRet->init(nStep) )
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

PopupAdventureTutorial::PopupAdventureTutorial(void) :
_nTutorialNum(1),
_lyTopUI(nullptr),
_lyMiddleUI(nullptr),
_lyBottomUI(nullptr),
_callbackHide(nullptr)
{
    
}

PopupAdventureTutorial::~PopupAdventureTutorial(void)
{
    
}

bool PopupAdventureTutorial::init(const int nStep)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _nTutorialNum = nStep;
    // init
    initVar();
    initUI();
    _bHideBackKey = false;
    
    return true;
}

void PopupAdventureTutorial::show()
{
    PopupBase::show();
    
    this->setLocalZOrder(ZORDER_SCENE_POPUP + 11);
}

void PopupAdventureTutorial::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}
#pragma mark - init
void PopupAdventureTutorial::initVar()
{
    _spriteContainer->setContentSize(Size(706, 814));
}

void PopupAdventureTutorial::initUI()
{
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_tutorial"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 158));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTopUI);
    
    _lyMiddleUI = LayerColor::create();
    _lyMiddleUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 140) );
    _lyMiddleUI->setIgnoreAnchorPointForPosition(false);
    _lyMiddleUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyMiddleUI->setPosition( _lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyMiddleUI);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - _lyTopUI->getContentSize().height - _lyMiddleUI->getContentSize().height - 75 - 5) );
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyBottomUI->setPosition( _lyMiddleUI->getPosition() + Vec2::DOWN * _lyMiddleUI->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);

    //
    uiTop();
    uiMiddle();
    uiBottom();
}

#pragma mark - ui
void PopupAdventureTutorial::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBg = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_place_%d_%d.png", random(1, 7), random(1, 4)));
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(_lyTopUI->getContentSize()/2);
    sprBg->setScale(3);
    sprBg->getTexture()->setTexParameters(texParams);
    _lyTopUI->addChild(sprBg);
    
    auto sprCat = Sprite::create("Assets/ui/adventure/adventure_guide_01.png");
    sprCat->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprCat->setPosition(30, 0);
    _lyTopUI->addChild(sprCat);
}
void PopupAdventureTutorial::uiMiddle()
{
    _lyMiddleUI->removeAllChildren();
    
    auto sprTextBox = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_2_on.png");
    sprTextBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprTextBox->setContentSize(Size(696, 130));
    sprTextBox->setPosition(_lyMiddleUI->getContentSize()/2);
    _lyMiddleUI->addChild(sprTextBox);
    
    auto lbManagerText = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_tutorial_manager_msg_%d", _nTutorialNum)), GAME_FONT, 26);
    lbManagerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbManagerText->setPosition(15, sprTextBox->getContentSize().height / 2);
    lbManagerText->setDimensions(sprTextBox->getContentSize().width - 40, sprTextBox->getContentSize().height - 10);
    lbManagerText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbManagerText->setOverflow(Label::Overflow::SHRINK);
    sprTextBox->addChild(lbManagerText);
    
}
void PopupAdventureTutorial::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    auto lbTutorialText = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_tutorial_msg_%d", _nTutorialNum)), GAME_FONT, 26);
    lbTutorialText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTutorialText->setPosition(15, _lyBottomUI->getContentSize().height - 15);
    lbTutorialText->setDimensions(_lyBottomUI->getContentSize().width - 40, _lyBottomUI->getContentSize().height - 100);
    lbTutorialText->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbTutorialText->setOverflow(Label::Overflow::SHRINK);
    _lyBottomUI->addChild(lbTutorialText);
    
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_ok.png");
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnClose->setPosition(Vec2(_lyBottomUI->getContentSize().width/2, 15));
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupAdventureTutorial::onClickClose, this));
    btnClose->setZoomScale(0.05f);
    btnClose->setTitleText(GAME_TEXT("t_ui_confirm"));
    btnClose->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnClose->getTitleLabel()->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height - 5);
    btnClose->setTitleFontSize(26);
    btnClose->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyBottomUI->addChild(btnClose);
}

#pragma mark - onClick
void PopupAdventureTutorial::onClickClose(Ref* const sender)
{
    addTutorialStep();
    if(_callbackHide != nullptr)
        _callbackHide();
    _callbackHide = nullptr;
    hide();
}

#pragma mark - utils
void PopupAdventureTutorial::addTutorialStep()
{
    if(AdventureManager::getInstance()->getTutorialStep() == 4 ||
       AdventureManager::getInstance()->getTutorialStep() == 6 )
        AdventureManager::getInstance()->setTutorialStep(AdventureManager::getInstance()->getTutorialStep() + 1);
}
