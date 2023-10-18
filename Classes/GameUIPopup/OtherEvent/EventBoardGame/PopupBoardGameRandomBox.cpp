//
//  PopupBoardGameRandomBox.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/19.
//

#include "PopupBoardGameRandomBox.h"

#include "ManagerEvent/EventBoardGameManager.h"

USING_NS_CC;

PopupBoardGameRandomBox* PopupBoardGameRandomBox::create(InfoItems* infoItem)
{
    PopupBoardGameRandomBox* pRet = new(std::nothrow) PopupBoardGameRandomBox();
    if ( pRet && pRet->init(infoItem) )
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

PopupBoardGameRandomBox::PopupBoardGameRandomBox() :
_infoItem(nullptr),
_layerContents(nullptr),
_callbackClose(nullptr)
{
    
}

PopupBoardGameRandomBox::~PopupBoardGameRandomBox()
{
    _infoItem->release();
}

bool PopupBoardGameRandomBox::init(InfoItems* infoItem)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _infoItem = infoItem;
    if(_infoItem != nullptr)
        _infoItem->retain();
    //
    setHideBackKey(false);
    setHideAll(false);
    
    //
    initVar();
    initUI();
    
    return true;
    
}

#pragma mark - init
void PopupBoardGameRandomBox::initVar()
{
    
}

void PopupBoardGameRandomBox::initUI()
{
    Size size = Size(500, 580);
    
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    std::string strTitle = GAME_TEXT("t_ui_event_dice_result_1");
    if ( _infoItem == nullptr)
    {
        strTitle = GAME_TEXT("t_ui_event_dice_outpnt_1");
    }
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    uiRandBox();
}

#pragma mark - ui
void PopupBoardGameRandomBox::uiRandBox()
{
    //
    _layerContents = Layer::create();
    _layerContents->setContentSize(Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - 25 - 25));
    _layerContents->setIgnoreAnchorPointForPosition(false);
    _layerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContents->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContents);
    
    
    //
    auto sprRandBox = Sprite::create("Assets/ui/event/event_box_open1_1.png");
    sprRandBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprRandBox->setPosition(_layerContents->getContentSize().width / 2,  _layerContents->getContentSize().height / 2 - 32 - 50);
    _layerContents->addChild(sprRandBox, 10);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        for(int i = 1; i <= 8; i++)
        {
            std::string stdPath = MafUtils::format("Assets/ui/event/event_box_open1_%d.png", i);
            animation->addSpriteFrameWithFile(stdPath);
        }
        
        sprRandBox->runAction(Sequence::create(Animate::create(animation),FadeOut::create(0.8f),nullptr));
    }
    
    //
    auto action1 = DelayTime::create(1.1);
    auto action2 = CallFunc::create(CC_CALLBACK_0(PopupBoardGameRandomBox::uiReward, this));
    auto seq = Sequence::create(action1, action2, nullptr);
    _layerContents->runAction(seq);
    
}
void PopupBoardGameRandomBox::uiReward()
{
    auto spriteEffect = Sprite::create("Assets/ui/event/event_fx1_1.png");
    spriteEffect->setPosition(Vec2(_layerContents->getContentSize()/2) + Vec2::UP * 50);
    _layerContents->addChild(spriteEffect);
    
    if(_infoItem != nullptr)
    {
        auto sprIcon = Sprite::create(_infoItem->getPath());
        sprIcon->getTexture()->setTexParameters(texParams);
        sprIcon->setScale(4);
        sprIcon->setPosition(spriteEffect->getContentSize()/2);
        spriteEffect->addChild(sprIcon);
        
        std::string strContent = _infoItem->getText() + "\n" + GAME_TEXTFORMAT("t_ui_msg_get_5", std::atoi(_infoItem->getCount().c_str()));
        
        auto lbContent = Label::createWithTTF(strContent, GAME_FONT, 24);
        lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbContent->setPosition(spriteEffect->getPosition() + Vec2::DOWN * 80);
        lbContent->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
        lbContent->setColor(COLOR_TITLE);
        _layerContents->addChild(lbContent);
        
        auto btnBox = ui::Button::create("Assets/ui_common/btn_ok.png");
        btnBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        btnBox->setPosition(Vec2(_layerContents->getContentSize().width/2, 20));
        btnBox->addClickEventListener(CC_CALLBACK_1(PopupBoardGameRandomBox::onClickClose, this));
        btnBox->setTitleText(GAME_TEXT("t_ui_system_update_confirm"));
        btnBox->setTitleFontSize(24);
        btnBox->setZoomScale(0.05f);
        _layerContents->addChild(btnBox);
    }
    
}

#pragma mark - set, get, sort, add
void PopupBoardGameRandomBox::setCallbackClose(std::function<void(void)> callback)
{
    _callbackClose = callback;
}

#pragma mark - click
void PopupBoardGameRandomBox::onClickClose(cocos2d::Ref* sender)
{
    if ( _callbackClose != nullptr )
    {
        _callbackClose();
    }
    
    hide();
}
 
