//
//  PopupRaidReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/13.
//

#include "PopupRaidReward.h"

PopupRaidReward* PopupRaidReward::create(Vector<InfoItems*> vecReward)
{
    PopupRaidReward* pRet = new(std::nothrow) PopupRaidReward();
    if ( pRet && pRet->init(vecReward) )
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

PopupRaidReward::PopupRaidReward(void):
_lyTopUI(nullptr),
_lyBottomUI(nullptr)
{
     _vecReward.clear();
}

PopupRaidReward::~PopupRaidReward(void)
{
    _vecReward.clear();
}

#pragma mark -init

bool PopupRaidReward::init(Vector<InfoItems*> vecReward)
{
    if ( !PopupBase::init())
    {
        return false;
    }
    _vecReward = vecReward;
    initVar();
    initUI();
    
    return true;
}

void PopupRaidReward::initVar()
{
    Size size = Size(618, 618);
    _spriteContainer->setContentSize( size );
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_new_raid_reward_title_1"), GAME_FONT, 30);
    
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 45) );
    _spriteContainer->addChild(labelTitle);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_ok.png"),nullptr, CC_CALLBACK_1(PopupRaidReward::onClick, this));
    itemClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width/2, 15));
    itemClose->setTouchRect( itemClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    auto lbClose = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 30);
    lbClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbClose->setPosition(itemClose->getContentSize()/2);
    lbClose->setDimensions(itemClose->getContentSize().width - 10, itemClose->getContentSize().height - 10);
    lbClose->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbClose->setOverflow(Label::Overflow::SHRINK);
    itemClose->addChild(lbClose);
    
    _lyTopUI = Layer::create();
    _lyTopUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setContentSize(Size(_spriteContainer->getContentSize().width - 4, _spriteContainer->getContentSize().height/3.3f));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTopUI);
    
    _lyBottomUI = Layer::create();
    _lyBottomUI->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottomUI->setContentSize(Size(600, 250));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setPosition(_lyTopUI->getPosition() + Vec2::DOWN * _lyTopUI->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
}

void PopupRaidReward::initUI()
{
     initTopUI();
     initBottomUI();
}

void PopupRaidReward::initTopUI()
{
    _lyTopUI->removeAllChildren();
    
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_new_raid_reward_1"), GAME_FONT, 24);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbContent->setPosition(Vec2(_lyTopUI->getContentSize()/2));
    _lyTopUI->addChild(lbContent);
    
}

void PopupRaidReward::initBottomUI()
{
    _lyBottomUI->removeAllChildren();
    
    auto sprBg = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png");
    sprBg->setContentSize(_lyBottomUI->getContentSize());
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(_lyBottomUI->getContentSize()/2);
    _lyBottomUI->addChild(sprBg);
    
    auto sprDescBox = Sprite::create("Assets/ui_common/base_text_1_3.png");
    sprDescBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprDescBox->setPosition(_lyBottomUI->getContentSize().width/2, _lyBottomUI->getContentSize().height);
    _lyBottomUI->addChild(sprDescBox);
    
    auto lbDesc = Label::createWithTTF(GAME_TEXT("t_ui_team_raid_reward_item"), GAME_FONT, 30);
    lbDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbDesc->setPosition(sprDescBox->getContentSize()/2);
    lbDesc->setDimensions(sprDescBox->getContentSize().width, sprDescBox->getContentSize().height);
    lbDesc->setOverflow(Label::Overflow::SHRINK);
    lbDesc->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    sprDescBox->addChild(lbDesc);
    
    
    auto layerScroll = ui::ScrollView::create();
    layerScroll->setBounceEnabled(false);
    layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    layerScroll->setIgnoreAnchorPointForPosition(false);
    layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layerScroll->setPosition(sprDescBox->getPosition() + Vec2::DOWN * sprDescBox->getContentSize().height);
    layerScroll->setSwallowTouches(false);
    _lyBottomUI->addChild(layerScroll);
    {
        float width = 100 * _vecReward.size() + 10 * _vecReward.size();
        
        if(width > sprBg->getContentSize().width - 30)
            width = sprBg->getContentSize().width - 30;
        
        layerScroll->setContentSize(Size(width, sprBg->getContentSize().height - sprDescBox->getContentSize().height));
    }
    
    double innerWidth = 0;
    double innerHeight = layerScroll->getContentSize().height;
    
    for ( int i = 0; i < _vecReward.size(); i++ )
    {
        auto objItem = _vecReward.at(i);
        
        double posX = 100 * i + 10 * i;
        double posY = layerScroll->getContentSize().height / 2;
        innerWidth = posX + 100;
        
        auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
        spriteItemBG->setContentSize(Size(100,100));
        spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteItemBG->setPosition(posX, posY);
        layerScroll->addChild(spriteItemBG);
        
        auto spriteItems = Sprite::create(objItem->getPath());
        if ( spriteItems != nullptr )
        {
            spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
            spriteItems->getTexture()->setTexParameters(texParams);

            spriteItems->setScale(MAX(spriteItemBG->getContentSize().width / spriteItems->getContentSize().width, spriteItemBG->getContentSize().height / spriteItems->getContentSize().height));
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
    layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
}

#pragma mark utils

void PopupRaidReward::onClick(Ref* sender)
{
    auto item = (Node*) sender;
    
    if(item == nullptr)
        return;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
}

