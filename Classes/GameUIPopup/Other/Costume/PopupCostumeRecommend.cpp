//
//  PopupCostumeRecommend.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupCostumeRecommend.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/CostumeManager.h"

USING_NS_CC;

PopupCostumeRecommend* PopupCostumeRecommend::create(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    PopupCostumeRecommend* pRet = new(std::nothrow) PopupCostumeRecommend();
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

PopupCostumeRecommend::PopupCostumeRecommend(void) :
_eType(E_COSTUME::HEAD),
_eIdx(E_COSTUME_IDX::IDX_1_DEFAULT),
_onCallbackYes(nullptr),

_layerContainerBottom(nullptr),
_layerContainerContent(nullptr)
{
    
}

PopupCostumeRecommend::~PopupCostumeRecommend(void)
{
    
}

bool PopupCostumeRecommend::init(E_COSTUME eType, E_COSTUME_IDX eIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
        
    // init
    _eType = eType;
    _eIdx = eIdx;
    
    // init
    initVar();
    initUi();

    return true;
}

#pragma mark - init
void PopupCostumeRecommend::initVar()
{

    
}

void PopupCostumeRecommend::initUi()
{
    Size size = Size(500, 455);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_cosrec_title_1"), GAME_FONT, 32);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setColor(COLOR_TITLE);
    _spriteContainer->addChild(labelTitle);
    
    
    uiBottom();
    uiContent();
}

#pragma mark - ui
void PopupCostumeRecommend::uiBottom()
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
    spriteCoupon->setContentSize(Size(140, 80));
    auto spriteSelectCoupon = ui::Scale9Sprite::create("Assets/ui_common/btn_1_3_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectCoupon->setContentSize(Size(140, 80));
    
    auto itemCoupon = MafNode::MafMenuItemSprite::create(spriteCoupon, spriteSelectCoupon, nullptr, CC_CALLBACK_1(PopupCostumeRecommend::onClickCoupon, this));
    menu->addChild(itemCoupon);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_cosrec_btn_1"), GAME_FONT, 24);
        label->setPosition(itemCoupon->getContentSize().width / 2, itemCoupon->getContentSize().height / 2);
        label->setDimensions(itemCoupon->getContentSize().width * 0.9, itemCoupon->getContentSize().height * 0.9);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        itemCoupon->addChild(label);
    }
    //
    menu->alignItemsHorizontallyWithPadding(10);
}

void PopupCostumeRecommend::uiContent()
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
        
    //
    auto objCostume = CostumeManager::getInstance()->getCostume(_eType, _eIdx);
    
    //
    std::string str = DataManager::GetCostumeIconPath(objCostume->getType(), objCostume->getIdx());
    auto _sprIcon = Sprite::create(str);
    _sprIcon->getTexture()->setTexParameters(texParams);
    _sprIcon->setPosition(_layerContainerContent->getContentSize().width*0.5, _layerContainerContent->getContentSize().height*0.6);
    _sprIcon->setScale(4);
    _layerContainerContent->addChild(_sprIcon);
    
    auto labelContent1 = Label::createWithTTF(objCostume->getDesc(), GAME_FONT, 28);
    labelContent1->setPosition(_layerContainerContent->getContentSize().width *0.5, -15);
    labelContent1->setDimensions(_layerContainerContent->getContentSize().width * 0.8, labelContent1->getContentSize().height * 2.3);
    labelContent1->setAlignment(TextHAlignment::CENTER);
    labelContent1->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContent->addChild(labelContent1);
}

#pragma mark - set, get, sort, add
void PopupCostumeRecommend::setCallbackYes(const std::function<void(E_COSTUME, E_COSTUME_IDX)>& callback)
{
    _onCallbackYes = callback;
}

#pragma mark - click


void PopupCostumeRecommend::onClickCoupon(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
        
    if ( _onCallbackYes != nullptr )
    {
        _onCallbackYes(_eType, _eIdx);
    }
    
    hide();
}
