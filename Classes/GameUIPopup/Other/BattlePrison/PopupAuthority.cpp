//
//  PopupAuthority.cpp
//  FantasyClicker-mobile
//
//  Created by TK on 2020/06/03.
//

#include "PopupAuthority.h"

USING_NS_CC;

PopupAuthority* PopupAuthority::create(PrisonManager::PrisonType type)
{
    PopupAuthority* pRet = new(std::nothrow) PopupAuthority();
    if ( pRet && pRet->init(type) )
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

PopupAuthority::PopupAuthority(void) :
_layerContainerBottom(nullptr),
_layerContainerTop(nullptr),
_layerContainerList(nullptr),

_spriteIcon(nullptr),
_labelContentTitle(nullptr),
_labelContentInfo(nullptr)
{
    
}

PopupAuthority::~PopupAuthority(void)
{
    
}

bool PopupAuthority::init(PrisonManager::PrisonType type)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    _type = type;
    
    // init
    initUi();
    initVar();
    return true;
}

#pragma mark - init
void PopupAuthority::initVar()
{

}

void PopupAuthority::initUi()
{
    Size size;
    switch (_type) {
        case PrisonManager::PrisonType::DEFAULT:
        {
             size = Size(562, 800);
        }
            break;
        case PrisonManager::PrisonType::FIRE:
        {
            size = Size(562, 800);
        }
            break;
        
    }
    _spriteContainer->setContentSize( size );
    
    switch (_type) {
        case PrisonManager::PrisonType::DEFAULT:
        {
            // label title
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_prison_guide2_title"), GAME_FONT, 32);
        labelTitle->setColor(COLOR_TITLE);
        labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
        _spriteContainer->addChild(labelTitle);
        }
            break;
        case PrisonManager::PrisonType::FIRE:
        {
            auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_hellprison_guide2_title"), GAME_FONT, 32);
            labelTitle->setColor(COLOR_TITLE);
            labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
            _spriteContainer->addChild(labelTitle);
        }
            break;
    }

    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
   // item close
    auto itemConfirm = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_baphomet.png"),Sprite::create("Assets/ui_common/btn_baphomet.png"),  nullptr, CC_CALLBACK_1(PopupAuthority::onClickClose, this));
    itemConfirm->setPosition( Vec2(_spriteContainer->getContentSize().width/2, itemConfirm->getContentSize().height));
    itemConfirm->setTouchRect( itemConfirm->getContentSize() * 2 );
    menu->addChild(itemConfirm);
    {
         auto _labelCount = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_confirm", 0), GAME_FONT, 32);
         _labelCount->setColor( Color3B(255,216,0) );
         _labelCount->setPosition( Vec2(itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2) );
         itemConfirm->addChild(_labelCount);
     }
    
    
    uiBottom();
}


#pragma mark - ui
void PopupAuthority::uiBottom()
{
    if ( _layerContainerBottom == nullptr )
    {
        _layerContainerBottom = Layer::create();
        _layerContainerBottom->setContentSize( Size(_spriteContainer->getContentSize().width, _spriteContainer->getContentSize().height - 75) );
        _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
        _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerContainerBottom->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 0) );
        _spriteContainer->addChild(_layerContainerBottom);
    }
    _layerContainerBottom->removeAllChildren();
    
    switch (_type) {
        case PrisonManager::PrisonType::DEFAULT:
        {
            _spriteIcon = Sprite::create("Assets/ui/prison/icon_baformetz_1.png");

            _layerContainerBottom->addChild(_spriteIcon);
            _spriteIcon->setPosition( Vec2(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height - 75) );
            _spriteIcon->setScale(1.5f);
            
            auto spriteIcon2 = Sprite::create("Assets/ui/prison/icon_baformetz_1.png");
            spriteIcon2->setPosition(_spriteIcon->getContentSize().height/2,_spriteIcon->getContentSize().width/2);
            _spriteIcon->addChild(spriteIcon2);
            
            _labelContentTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_prison_guide2_title"), GAME_FONT, 30);
            _layerContainerBottom->addChild(_labelContentTitle);
            
            _labelContentInfo = Label::createWithTTF(GAME_TEXT("t_ui_menu_prison_guide2_content"), GAME_FONT, 19.f);
            _layerContainerBottom->addChild(_labelContentInfo);
            _labelContentInfo->setPosition( Vec2(_layerContainerBottom->getContentSize().width / 2, -50 ));
        }
            break;
            
        case PrisonManager::PrisonType::FIRE:
        {
            
            _spriteIcon = Sprite::create("Assets/ui/prison/icon_beelzebub_1.png");
            
            _layerContainerBottom->addChild(_spriteIcon);
            _spriteIcon->setPosition( Vec2(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height - 75) );
            _spriteIcon->setScale(1.5f);
            
            auto spriteIcon2 = Sprite::create("Assets/ui/prison/icon_beelzebub_1.png");
            spriteIcon2->setPosition(_spriteIcon->getContentSize().height/2,_spriteIcon->getContentSize().width/2);
            _spriteIcon->addChild(spriteIcon2);
            
            _labelContentTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_hellprison_guide2_title"), GAME_FONT, 30);
            _layerContainerBottom->addChild(_labelContentTitle);
            
            _labelContentInfo = Label::createWithTTF(GAME_TEXT("t_ui_menu_hellprison_guide2_content"), GAME_FONT, 19.f);
            _layerContainerBottom->addChild(_labelContentInfo);
                _labelContentInfo->setPosition( Vec2(_layerContainerBottom->getContentSize().width / 2, -50 ));
        }
            break;
    }

    
    _labelContentTitle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _labelContentTitle->setPosition( Vec2(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height - 220) );
    _labelContentTitle->setLineSpacing(-5);
    _labelContentTitle->setDimensions(_layerContainerBottom->getContentSize().width * 0.9, 100);
    _labelContentTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _labelContentTitle->setOverflow(Label::Overflow::SHRINK);
    _labelContentTitle->setAlignment(cocos2d::TextHAlignment::CENTER);

    
    _labelContentInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _labelContentInfo->setLineSpacing(-5);
    _labelContentInfo->setDimensions(_layerContainerBottom->getContentSize().width * 0.9, _layerContainerBottom->getContentSize().height);
    _labelContentInfo->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _labelContentInfo->setOverflow(Label::Overflow::SHRINK);
    _labelContentInfo->setAlignment(cocos2d::TextHAlignment::CENTER);
}


#pragma mark - click
void PopupAuthority::onClickClose(cocos2d::Ref* sender)
{
    hide();
}
