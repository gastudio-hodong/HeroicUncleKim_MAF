//
//  PopupCostumeRent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupCostumeRent.h"

#include "ManagerGame/CostumeManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupCostumeRent* PopupCostumeRent::create(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    PopupCostumeRent* pRet = new(std::nothrow) PopupCostumeRent();
    if ( pRet && pRet->init(eType, eIdx) )
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

PopupCostumeRent::PopupCostumeRent(void) :
_eType(E_COSTUME::HEAD),
_eIdx(E_COSTUME_IDX::IDX_1_DEFAULT),
_onCallbackYes(nullptr),

_layerContainerBottom(nullptr),
_layerContainerContent(nullptr)
{
    
}

PopupCostumeRent::~PopupCostumeRent(void)
{
    
}

bool PopupCostumeRent::init(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _eType = eType;
    _eIdx = eIdx;
    
    // init
    initVar();
    initUi();

    return true;
}

#pragma mark - init
void PopupCostumeRent::initVar()
{

    
}

void PopupCostumeRent::initUi()
{
    Size size = Size(500, 455);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    auto objCostume = CostumeManager::getInstance()->getCostume(_eType, _eIdx);
    if ( objCostume == nullptr )
    {
        return;
    }

    auto labelTitle = Label::createWithTTF(objCostume->getTitle(), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    
    uiBottom();
    uiContent();
}

#pragma mark - ui
void PopupCostumeRent::uiBottom()
{
    if ( _layerContainerBottom == nullptr )
    {
        _layerContainerBottom = Layer::create();
        _layerContainerBottom->setContentSize( Size(_spriteContainer->getContentSize().width, 130) );
        _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
        _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerContainerBottom->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 0) );
        _spriteContainer->addChild(_layerContainerBottom);
    }
    _layerContainerBottom->removeAllChildren();

    
    auto menu = Menu::create();
    menu->setPosition(_layerContainerBottom->getContentSize().width / 2, 44.5 + 20);
    _layerContainerBottom->addChild(menu);
    
    auto spriteCoupon = ui::Scale9Sprite::create("Assets/ui_common/btn_1_3_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteCoupon->setContentSize(Size(150, 89));
    auto spriteSelectCoupon = ui::Scale9Sprite::create("Assets/ui_common/btn_1_3_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectCoupon->setContentSize(Size(150, 89));
    
    auto spriteNo = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteNo->setContentSize(Size(150, 89));
    auto spriteSelectNo = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectNo->setContentSize(Size(150, 89));
    
    auto spriteCoin = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteCoin->setContentSize(Size(150, 89));
    auto spriteSelectCoin = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectCoin->setContentSize(Size(150, 89));
    
    auto itemCoupon = MafNode::MafMenuItemSprite::create(spriteCoupon, spriteSelectCoupon, nullptr, CC_CALLBACK_1(PopupCostumeRent::onClickCoupon, this));
    menu->addChild(itemCoupon);
    {
        auto label = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_costume_msg_41", ItemsMoneyManager::getInstance()->getCostumeCoupon()), GAME_FONT, 24);
        label->setPosition(itemCoupon->getContentSize().width / 2, itemCoupon->getContentSize().height / 2);
        label->setDimensions(itemCoupon->getContentSize().width * 0.9, itemCoupon->getContentSize().height * 0.9);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        itemCoupon->addChild(label);
    }
    
    auto itemNo = MafNode::MafMenuItemSprite::create(spriteNo, spriteSelectNo, nullptr, CC_CALLBACK_1(PopupCostumeRent::onClickNo, this));
    menu->addChild(itemNo);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_no"), GAME_FONT, 24);
        label->setPosition( itemNo->getContentSize().width / 2, itemNo->getContentSize().height / 2);
        itemNo->addChild(label);
    }
    
    auto itemCoin = MafNode::MafMenuItemSprite::create(spriteCoin, spriteSelectCoin, nullptr, CC_CALLBACK_1(PopupCostumeRent::onClickCoin, this));
    menu->addChild(itemCoin);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_msg_43"), GAME_FONT, 24);
        label->setPosition( itemCoin->getContentSize().width / 2, itemCoin->getContentSize().height / 2);
        label->setDimensions(itemCoin->getContentSize().width * 0.9, itemCoin->getContentSize().height * 0.9);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        itemCoin->addChild(label);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupCostumeRent::uiContent()
{
    if ( _layerContainerContent == nullptr )
    {
        _layerContainerContent = Layer::create();
        _layerContainerContent->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - _layerContainerBottom->getContentSize().height - 75) );
        _layerContainerContent->setIgnoreAnchorPointForPosition(false);
        _layerContainerContent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerContent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
        _spriteContainer->addChild(_layerContainerContent);
    }
    _layerContainerContent->removeAllChildren();
    
    auto labelContent1 = Label::createWithTTF(GAME_TEXT("t_ui_costume_msg_29"), GAME_FONT, 28);
    labelContent1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelContent1->setPosition(_layerContainerContent->getContentSize().width / 2, _layerContainerContent->getContentSize().height - 50);
    labelContent1->setDimensions(_layerContainerContent->getContentSize().width * 0.9, labelContent1->getContentSize().height);
    labelContent1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelContent1->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelContent1);
    
    
    auto labelContent2 = Label::createWithTTF(GAME_TEXT("t_ui_costume_msg_42"), GAME_FONT, 24);
    labelContent2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelContent2->setPosition(_layerContainerContent->getContentSize().width / 2, 20);
    labelContent2->setDimensions(_layerContainerContent->getContentSize().width * 0.9, 100);
    labelContent2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelContent2->setOverflow(Label::Overflow::SHRINK);
    labelContent2->setColor(COLOR_COIN);
    _layerContainerContent->addChild(labelContent2);
}

#pragma mark - set, get, sort, add
void PopupCostumeRent::setCallbackYes(const std::function<void(int, int, bool)>& callback)
{
    _onCallbackYes = callback;
}

#pragma mark - click
void PopupCostumeRent::onClickNo(Ref* sender)
{
    hide();
}

void PopupCostumeRent::onClickCoupon(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    int nCoupon = ItemsMoneyManager::getInstance()->getCostumeCoupon();
    if ( nCoupon <= 0 )
    {
        return;
    }
    
    if ( _onCallbackYes != nullptr )
    {
        _onCallbackYes(_eType, _eIdx, true);
    }
    
    hide();
}

void PopupCostumeRent::onClickCoin(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    if ( _onCallbackYes != nullptr )
    {
        _onCallbackYes(_eType, _eIdx, false);
    }
    
    hide();
}
