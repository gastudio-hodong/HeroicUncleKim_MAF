//
//  LayerFriendList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerFriendList.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"
#include "GameUIPopup/Other/Expedition/PopupExpeditionDraw.h"

#include "ManagerGame/FriendManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/PetNewManager.h"

USING_NS_CC;

LayerFriendList* LayerFriendList::create(cocos2d::Size size, E_PLACE ePlace)
{
    LayerFriendList* pRet = new(std::nothrow) LayerFriendList();
    if ( pRet && pRet->init(size, ePlace) )
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

LayerFriendList::LayerFriendList(void) :
_onCallbackHide(nullptr),

_layerContainerInfo(nullptr),
_layerContainerList(nullptr),
_layerContainerBottom(nullptr),
_layerContainerPetSelect(nullptr),
_tableList(nullptr),
_spritePetIcon(nullptr)
{
    
}

LayerFriendList::~LayerFriendList(void)
{
    
}

bool LayerFriendList::init(cocos2d::Size size, E_PLACE ePlace)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //
    _ePlace = ePlace;
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - table
void LayerFriendList::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size LayerFriendList::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, C_BAR_MISSION_SIZE);
}

extension::TableViewCell* LayerFriendList::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    Vector<InfoFriend*> vec = FriendManager::getInstance()->getFriendList();
    
    //
    auto obj = vec.at(idx);
 

    // bg
    std::string strPathBackground = MafUtils::format("Assets/ui_common/list_bg_%d.png", 1);
    if( obj->getDibson() == true )
    {
        strPathBackground = MafUtils::format("Assets/ui_common/list_bg_%d.png", 2);
    }
    
    auto spriteBG = ui::Scale9Sprite::create(strPathBackground, Rect::ZERO, Rect(0,51,150,10));
    spriteBG->setPosition(size.width / 2, size.height / 2);
    spriteBG->setContentSize(size);
    spriteBG->setTag((int)idx);
    cell->addChild(spriteBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    {
        auto spriteHeroBG = ui::Scale9Sprite::create("Assets/ui_common/box_quest.png", Rect::ZERO, Rect(2,2,112,112));
        
        auto itemHeroBox = MafNode::MafMenuItemSprite::create(spriteHeroBG, nullptr, CC_CALLBACK_1(LayerFriendList::onClickUserDetail, this));
        itemHeroBox->setPosition(spriteHeroBG->getContentSize().width / 2 + 10, spriteBG->getContentSize().height / 2);
        itemHeroBox->setTag((int)idx);
        itemHeroBox->setTouchRect(itemHeroBox->getContentSize() - Size(10, 10));
        menu->addChild(itemHeroBox);
        
        //
        auto vecCostume = vec.at(idx)->getCostume();
        int nCostumeWeapon = vecCostume.at(0);
        int nCostumeHead = vecCostume.at(1);
        int nCostumeBody = vecCostume.at(2);
        int nCostumeCape = vecCostume.at(3);
        int nCostumeHand = vecCostume.at(4);
        int nCostumeFoot = vecCostume.at(5);
        
        auto hero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);
        hero->setPosition(Vec2(itemHeroBox->getPositionX()+25, itemHeroBox->getPositionY()+10));
        hero->setImage(nCostumeWeapon, nCostumeHead, nCostumeBody, nCostumeCape, nCostumeHand,nCostumeFoot);
        hero->setMotion(nCostumeWeapon, nCostumeHead, nCostumeBody, nCostumeCape, nCostumeHand,nCostumeFoot);
        hero->actionMove();
        hero->getLayerCharacter()->setScale(0.6);
        itemHeroBox->addChild(hero);
        
    }

    
    // nick
    std::string strTitle = obj->getNickname();
    if (strTitle == "")
    {
        strTitle = GAME_TEXT("t_ui_friend_msg_12");
    }
    auto lbTitle = Label::createWithSystemFont(strTitle, SYSTEM_FONT, 30);//28로 맞출지는 보고 확인
    lbTitle->setAnchorPoint(Vec2(0,0));
    lbTitle->setPosition(spriteBG->getContentSize().width*0.21, spriteBG->getContentSize().height*0.6);
    lbTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbTitle->setOverflow(Label::Overflow::SHRINK);
    lbTitle->setColor(COLOR_TITLE);
    spriteBG->addChild(lbTitle);

    // info
    auto lbBonusSpeed = Label::createWithTTF(GAME_TEXT("t_ui_friend_info_4"), GAME_FONT, 18);
    lbBonusSpeed->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbBonusSpeed->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbBonusSpeed->setPosition(spriteBG->getContentSize().width*0.21, spriteBG->getContentSize().height*0.6);
    lbBonusSpeed->setDimensions(250, lbBonusSpeed->getContentSize().height);
    lbBonusSpeed->setOverflow(Label::Overflow::SHRINK);
    lbBonusSpeed->setTextColor(Color4B(136, 136, 136, 255));
    spriteBG->addChild(lbBonusSpeed);
    
    // info : login
    auto lbLogin = Label::createWithTTF("", GAME_FONT, 18);
    lbLogin->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    lbLogin->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbLogin->setPosition(spriteBG->getContentSize().width*0.21, 0);
    lbLogin->setDimensions(250, lbLogin->getContentSize().height);
    lbLogin->setOverflow(Label::Overflow::SHRINK);
    lbLogin->setTextColor(Color4B(136, 136, 136, 255));
    spriteBG->addChild(lbLogin);
    
    int nDay = obj->getLoginDay();
    if ( nDay != -1 )
    {
        std::string strDay = "";
        if ( nDay == 0 )        strDay = GAME_TEXT("t_ui_friend_info_5");
        else if ( nDay < 7 )    strDay = GAME_TEXTFORMAT("t_ui_friend_info_6", nDay);
        else                    strDay = GAME_TEXTFORMAT("t_ui_friend_info_7", nDay);
        
        lbLogin->setString(strDay);
    }
    
    // button
    
    // wishlist
    std::string stdPathDibs = "Assets/ui/costume/btn_wishlist_1_2.png";
    if ( obj->getDibson() == true )
    {
        stdPathDibs = "Assets/ui/costume/btn_wishlist_1_1.png";
    }
    auto btnDibs = MafNode::MafMenuItemSprite::create(Sprite::create(stdPathDibs), nullptr, CC_CALLBACK_1(LayerFriendList::onClickDibs,this));
    btnDibs->setAnchorPoint(Vec2::ZERO);
    btnDibs->setPosition(lbTitle->getPositionX(),lbTitle->getPositionY()+5);
    btnDibs->setTouchRect(btnDibs->getContentSize()*2);
    btnDibs->setTag((int)idx);
    menu->addChild(btnDibs);
        
    lbTitle->setPositionX(lbTitle->getPositionX()+btnDibs->getContentSize().width+5);

    // del
    auto btnDelete = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/friend/btn_friend_x.png"), nullptr, CC_CALLBACK_1(LayerFriendList::onClickDelete,this));
    btnDelete->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btnDelete->setPosition(spriteBG->getContentSize().width, spriteBG->getContentSize().height);
    btnDelete->setTag((int)idx);
    menu->addChild(btnDelete);
      
    // receive
    std::string strPathPointReceive = "Assets/ui/friend/btn_friendship1_1.png";
    if ( obj->getPointReceiveAvailable() == false )
    {
        strPathPointReceive = "Assets/ui/friend/btn_friendship1_2.png";
    }
    auto btnPointReceive = MafNode::MafMenuItemSprite::create(Sprite::create(strPathPointReceive), nullptr, CC_CALLBACK_1(LayerFriendList::onClickPointReceive,this));
    btnPointReceive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnPointReceive->setPosition(spriteBG->getContentSize().width - 140 - 5, spriteBG->getContentSize().height / 2);
    btnPointReceive->setTag((int)idx);
    btnPointReceive->setEnabled(obj->getPointReceiveAvailable());
    menu->addChild(btnPointReceive);

    // send
    std::string strPathPointSend = "Assets/ui/friend/btn_friendship2_1.png";
    if ( obj->getPointSendAvailable() == false )
    {
        strPathPointSend = "Assets/ui/friend/btn_friendship2_2.png";
    }
    auto btnPointSend = MafNode::MafMenuItemSprite::create(Sprite::create(strPathPointSend), nullptr, CC_CALLBACK_1(LayerFriendList::onClickPointSend,this));
    btnPointSend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPointSend->setPosition(spriteBG->getContentSize().width - 140 + 5, spriteBG->getContentSize().height / 2);
    btnPointSend->setTag((int)idx);
    btnPointSend->setEnabled(obj->getPointSendAvailable());
    menu->addChild(btnPointSend);
    
    
    return cell;
}

ssize_t LayerFriendList::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    return FriendManager::getInstance()->getFriendList().size();
}

#pragma mark - init
void LayerFriendList::initVar()
{
    
}

void LayerFriendList::initUi()
{
    //
    _layerContainerInfo = Layer::create();
    _layerContainerInfo->setContentSize( Size(getContentSize().width, 55) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_layerContainerInfo);
    
    //
    _layerContainerBottom = Layer::create();
    _layerContainerBottom->setContentSize( Size(getContentSize().width, 240) );
    _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
    _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBottom->setPosition(getContentSize().width / 2, 0);
    addChild(_layerContainerBottom);
    
    //
    _layerContainerList = Layer::create();
    _layerContainerList->setContentSize( Size(getContentSize().width, getContentSize().height - _layerContainerInfo->getContentSize().height - _layerContainerBottom->getContentSize().height) );
    _layerContainerList->setIgnoreAnchorPointForPosition(false);
    _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerList->setPosition(getContentSize().width / 2, _layerContainerBottom->getPositionY() + _layerContainerBottom->getContentSize().height );
    addChild(_layerContainerList);
    
    //
    uiInfo();
    uiList();
    uiBottom();
}

#pragma mark - ui
void LayerFriendList::uiInfo()
{
    //
    _layerContainerInfo->removeAllChildren();
    

    //친구
    auto iconFriendAmount = Sprite::create("Assets/ui/friend/icon_friend_1.png");
    iconFriendAmount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    iconFriendAmount->setPosition(20, _layerContainerInfo->getContentSize().height / 2);
    _layerContainerInfo->addChild(iconFriendAmount);
    
    auto lbFriendAmount = MafNode::createBmFont(GAME_TEXTFORMAT("t_ui_friend_info_1",FriendManager::getInstance()->getFriendListAmount(),30), GAME_BM_FONT_AL, 18);
    lbFriendAmount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbFriendAmount->setPosition(iconFriendAmount->getPositionX() + 50, iconFriendAmount->getPositionY());
    lbFriendAmount->setName("AMOUNT");
    _layerContainerInfo->addChild(lbFriendAmount);
    
    //이동속도
    auto iconFriendSpeed = Sprite::create("Assets/ui/shop/shop_new_3.png");
    iconFriendSpeed->setPosition(iconFriendAmount->getPositionX() + 170, _layerContainerInfo->getContentSize().height / 2);
    _layerContainerInfo->addChild(iconFriendSpeed);
    
    auto lbFriendSpeed = MafNode::createBmFont(GAME_TEXTFORMAT("t_ui_friend_info_2",FriendManager::getInstance()->getFriendBonusSpeed()), GAME_BM_FONT_AL, 18);
    lbFriendSpeed->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbFriendSpeed->setPosition(iconFriendSpeed->getPositionX() + 30, iconFriendSpeed->getPositionY());
    lbFriendSpeed->setName("SPEED");
    _layerContainerInfo->addChild(lbFriendSpeed);
    
    // 우정 포인트
    {
        //
        std::vector<E_ITEMS> list;
        list.push_back(E_ITEMS::FRIEND_POINT);
        
        auto uiCurrency = UICurrency::create(list, Size(150, _layerContainerInfo->getContentSize().height));
        uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiCurrency->setPosition(Vec2(_layerContainerInfo->getContentSize().width, _layerContainerInfo->getContentSize().height / 2));
        _layerContainerInfo->addChild(uiCurrency);
    }
}

void LayerFriendList::uiList()
{
    //
    _layerContainerList->removeAllChildren();
    
    //
    _tableList = extension::TableView::create(this, Size(_layerContainerList->getContentSize().width-10, _layerContainerList->getContentSize().height) );
    _tableList->setDirection(extension::ScrollView::Direction::VERTICAL);
    _tableList->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _tableList->setIgnoreAnchorPointForPosition(false);
    _tableList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _tableList->setPosition(_layerContainerList->getContentSize().width / 2, 0);
    _tableList->setDelegate(this);
    _tableList->setBounceable(false);
    _layerContainerList->addChild(_tableList);
}

void LayerFriendList::uiBottom()
{
    _layerContainerBottom->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerBottom->addChild(menu);
    
    //
    auto itemAddFriend = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/friend/btn_friend_invite_new.png"), nullptr, CC_CALLBACK_1(LayerFriendList::onClickDeepLink,this));
    itemAddFriend->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    itemAddFriend->setPosition(_layerContainerBottom->getContentSize().width - 25, _layerContainerBottom->getContentSize().height / 2 + 5);
    menu->addChild(itemAddFriend);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_friend_invite"), GAME_FONT, 25);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(110, itemAddFriend->getContentSize().height/2);
        labelText->setColor(COLOR_COIN);
        labelText->setDimensions(150, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        itemAddFriend->addChild(labelText);
    }
    
    auto itemAllPoint = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/friend/btn_friendship_01.png"), nullptr, CC_CALLBACK_1(LayerFriendList::onClickPointAll,this));
    itemAllPoint->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    itemAllPoint->setPosition(_layerContainerBottom->getContentSize().width - 25, _layerContainerBottom->getContentSize().height / 2 - 5);
    menu->addChild(itemAllPoint);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_friend_msg_21"), GAME_FONT, 25);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(110, itemAllPoint->getContentSize().height/2);
        labelText->setColor(COLOR_COIN);
        labelText->setDimensions(150, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        labelText->setLineSpacing(-10);
        itemAllPoint->addChild(labelText);
    }

    //
    auto itemDraw = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/friend/btn_draw_01.png"), nullptr, CC_CALLBACK_1(LayerFriendList::onClickDraw,this));
    itemDraw->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemDraw->setPosition(160, _layerContainerBottom->getContentSize().height / 2);
    menu->addChild(itemDraw);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        animation->addSpriteFrameWithFile("Assets/ui/friend/btn_draw_01.png");
        animation->addSpriteFrameWithFile("Assets/ui/friend/btn_draw_02.png");
        animation->addSpriteFrameWithFile("Assets/ui/friend/btn_draw_03.png");
        animation->addSpriteFrameWithFile("Assets/ui/friend/btn_draw_04.png");
        animation->addSpriteFrameWithFile("Assets/ui/friend/btn_draw_05.png");
        animation->addSpriteFrameWithFile("Assets/ui/friend/btn_draw_06.png");
        animation->addSpriteFrameWithFile("Assets/ui/friend/btn_draw_01.png");
        
        auto animate = Animate::create(animation);
        auto seq = Sequence::create(DelayTime::create(0.3), animate, NULL);
        itemDraw->getNormalImage()->runAction(RepeatForever::create(seq));
        
        auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_4.png", Rect::ZERO, Rect(10,10,2,2));
        spriteBG->setContentSize(Size(116, 30));
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteBG->setPosition(itemDraw->getContentSize().width / 2, itemDraw->getContentSize().height / 2 + 65);
        itemDraw->addChild(spriteBG);
        
        auto lableText = Label::createWithTTF(GAME_TEXT("t_ui_draw"), GAME_FONT, 18);
        lableText->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2);
        lableText->setDimensions(spriteBG->getContentSize().width * 0.9, lableText->getContentSize().height);
        lableText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lableText->setOverflow(Label::Overflow::SHRINK);
        lableText->setColor(Color3B(140,140,140));
        spriteBG->addChild(lableText);
    }
    
    //
    auto spritePointBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_4.png", Rect::ZERO, Rect(10,10,2,2));
    spritePointBG->setContentSize(Size(257, 30));
    spritePointBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spritePointBG->setPosition(160, 20);
    _layerContainerBottom->addChild(spritePointBG);
    {
        auto spriteIcon = Sprite::create("Assets/icon/icon_friendship.png");
        spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteIcon->setPosition(50, spritePointBG->getContentSize().height / 2);
        spriteIcon->setScale(0.9);
        spritePointBG->addChild(spriteIcon);
        
        int nNow = FriendManager::getInstance()->getPointSendMax() - FriendManager::getInstance()->getPointSend();
        int nMax = FriendManager::getInstance()->getPointSendMax();
        auto lableText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_friend_msg_22", nNow, nMax), GAME_FONT, 18);
        lableText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lableText->setPosition(50, spritePointBG->getContentSize().height / 2);
        lableText->setDimensions(180, lableText->getContentSize().height);
        lableText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lableText->setOverflow(Label::Overflow::SHRINK);
        lableText->setColor(Color3B(140,140,140));
        spritePointBG->addChild(lableText);
    }
}

#pragma mark - draw
void LayerFriendList::drawInfo()
{
    //
    auto labelAmount = (Label*)_layerContainerInfo->getChildByName("AMOUNT");
    if ( labelAmount != nullptr )
    {
        labelAmount->setString(GAME_TEXTFORMAT("t_ui_friend_info_1",FriendManager::getInstance()->getFriendListAmount(),30));
    }

    auto labelSpeed = (Label*)_layerContainerInfo->getChildByName("SPEED");
    if ( labelSpeed != nullptr )
    {
        labelSpeed->setString(GAME_TEXTFORMAT("t_ui_friend_info_2",FriendManager::getInstance()->getFriendBonusSpeed()));
    }
}

void LayerFriendList::drawTable()
{
    //
    Point offset = _tableList->getContentOffset();
    _tableList->reloadData();
    if ( offset.y <= 0 )
    {
        _tableList->setContentOffset(offset);
    }
    
    auto labelText = (Label*)_layerContainerList->getChildByName("ZERO");
    if ( labelText == nullptr )
    {
        labelText = Label::createWithTTF(GAME_TEXT("t_ui_friend_msg_1"), GAME_FONT, 25);
        labelText->setPosition(_layerContainerList->getContentSize().width/2, _layerContainerList->getContentSize().height/2);
        labelText->setName("ZERO");
        _layerContainerList->addChild(labelText);
    }
    
    if ( FriendManager::getInstance()->getFriendListAmount() == 0 )
    {
        labelText->setVisible(true);
    }
    else
    {
        labelText->setVisible(false);
    }
}

void LayerFriendList::showPetSelect(double nPosX, double nPosY)
{
    if ( _layerContainerPetSelect != nullptr )
    {
        _layerContainerPetSelect->removeFromParent();
        _layerContainerPetSelect = nullptr;
    }
    
    _layerContainerPetSelect = Layer::create();
    UtilsScene::getInstance()->getRunningScene()->addChild(_layerContainerPetSelect, ZORDER_SCENE_POPUP);
    
    // event
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, _layerContainerPetSelect->getContentSize().width, _layerContainerPetSelect->getContentSize().height);
        if (rect.containsPoint(_layerContainerPetSelect->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
            
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        _eventDispatcher->removeEventListenersForTarget(_layerContainerPetSelect);
        _layerContainerPetSelect->removeFromParent();
        _layerContainerPetSelect = nullptr;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _layerContainerPetSelect);
        
        
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui/pet/pet_icon_box1_1.png", Rect::ZERO, Rect(5,10,100,90));
    spriteBG->setContentSize(Size(82.8, 622.4));
    spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteBG->setPosition(nPosX, nPosY);
    _layerContainerPetSelect->addChild(spriteBG);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
 
    E_PET listPet[] = {E_PET::CHICK_1, E_PET::HAMSTER_2, E_PET::DRAGON_3, E_PET::PRINCESS_4, E_PET::PURIRING_5, E_PET::HONG_6, E_PET::MOLE_7, E_PET::HANDSOME_8};
    for ( int i = 0; i < sizeof(listPet)/sizeof(E_PET); i++ )
    {
        E_PET pet = listPet[i];
        double nPosCellX = spriteBG->getContentSize().width / 2;
        double nPosCellY = 38.9 + 72.8 * i + 5 * i;
        
        int evolution = 1;
        if ( PetNewManager::getInstance()->getPetEvolution(pet) > 0 )
        {
            evolution = 2;
        }
        
        auto spritePet = Sprite::create(MafUtils::format("Assets/ui/pet/peticon_0%d_l1_%d.png", pet, evolution));
        spritePet->setScale(0.7);
        if ( PetNewManager::getInstance()->getPet(pet) == false )
        {
            spritePet->setColor(Color3B(50, 50, 50));
        }
        
        auto itemPet = MafNode::MafMenuItemSprite::create(spritePet, nullptr, CC_CALLBACK_1(LayerFriendList::onClickPetChange,this));
        itemPet->setPosition(nPosCellX, nPosCellY);
        itemPet->setTag(pet);
        itemPet->setEnabled(PetNewManager::getInstance()->getPet(pet));
        menu->addChild(itemPet);
    }
}

void LayerFriendList::hidePetSelect()
{
    if ( _layerContainerPetSelect == nullptr )
    {
        return;
    }
    
    _eventDispatcher->removeEventListenersForTarget(_layerContainerPetSelect);
    _layerContainerPetSelect->removeFromParent();
    _layerContainerPetSelect = nullptr;
}

#pragma mark - set, get, add
void LayerFriendList::setCallbackHide(const std::function<void(void)>& callback)
{
    _onCallbackHide = callback;
}

#pragma mark - click
void LayerFriendList::onClickDibs(cocos2d::Ref *sender)
{
    auto item = (MenuItem*)sender;
    auto vecList = FriendManager::getInstance()->getFriendList();

    vecList.at(item->getTag())->setDibson( !(vecList.at(item->getTag())->getDibson()) );
    FriendManager::getInstance()->sortFriendList();
    
    //
    drawTable();
}

void LayerFriendList::onClickDeepLink(cocos2d::Ref *sender)
{
    if ( AccountManager::getInstance()->isGuest() == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_transfer_guest_login_error"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    MafNative::CreateDeepLink("user_id", AccountManager::getInstance()->getUserID().c_str());
}

void LayerFriendList::onClickDelete(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    std::string strYes = GAME_TEXT("t_ui_yes");
    strYes.append(MafUtils::format("(%d/%d)", FriendManager::getInstance()->getFriendDeleteNow(), FriendManager::getInstance()->getFriendDeleteMax()));
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_friend_msg_7"), GAME_TEXT("t_ui_friend_msg_8"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(strYes, false, [=](Ref* sender){
        
        auto list = FriendManager::getInstance()->getFriendList();
        auto objFriend = list.at(item->getTag());
        
        PopupLoading::show();
        FriendManager::getInstance()->requestFriendDelete(CC_CALLBACK_2(LayerFriendList::callbackDel,this), objFriend->getUseridx(), objFriend->getPlatform());
    });
    popup->show();
}

void LayerFriendList::onClickPointReceive(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    auto vecList = FriendManager::getInstance()->getFriendList();
    auto obj = vecList.at(item->getTag());
    
    PopupLoading::show();
    FriendManager::getInstance()->requestPointReceive(CC_CALLBACK_2(LayerFriendList::callbackPointReceive, this), obj->getUseridx(), obj->getPlatform());;
}

void LayerFriendList::onClickPointSend(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    auto vecList = FriendManager::getInstance()->getFriendList();
    auto obj = vecList.at(item->getTag());
    
    PopupLoading::show();
    FriendManager::getInstance()->requestPointSend(CC_CALLBACK_2(LayerFriendList::callbackPointSend, this), obj->getUseridx(), obj->getPlatform());
}

void LayerFriendList::onClickPointAll(cocos2d::Ref* sender)
{
    auto list = FriendManager::getInstance()->getFriendList();
    if ( list.size() == 0 )
    {
        return;
    }
    
    PopupLoading::show();
    FriendManager::getInstance()->requestPointAll(CC_CALLBACK_2(LayerFriendList::callbackPointAll, this));
}

void LayerFriendList::onClickPetSelect(cocos2d::Ref *sender)
{
    auto item = (MenuItem*)sender;
    
    //
    Vec2 posItem = item->getParent()->convertToWorldSpace(item->getPosition());
    posItem.x -= item->getContentSize().width / 2;
    posItem.y += item->getContentSize().height / 2;
    showPetSelect(posItem.x, posItem.y);
}

void LayerFriendList::onClickPetChange(cocos2d::Ref *sender)
{
    auto item = (MenuItem*)sender;
    E_PET ePet = (E_PET)item->getTag();
    
    //
    hidePetSelect();
    
    if ( ePet == FriendManager::getInstance()->getMyPet() )
    {
        return;
    }
    
    //
    PopupLoading::show();
    FriendManager::getInstance()->requestChangeMyPet(CC_CALLBACK_1(LayerFriendList::callbackPet,this), (E_PET)(item->getTag()));
}

void LayerFriendList::onClickDraw(cocos2d::Ref* sender)
{
    auto popup = PopupExpeditionDraw::create();
    popup->show();
}

void LayerFriendList::onClickUserDetail(cocos2d::Ref* sender)
{
    auto item = (Node*)sender;
    
    auto obj = FriendManager::getInstance()->getFriendList().at(item->getTag());
    
    if(obj == nullptr)
        return;
    
    auto popup = PopupOtherUserInfo::create(obj->getUseridx(), obj->getPlatform(), obj->getNickname());
    popup->show();
}

#pragma mark - network
void LayerFriendList::requestInfo()
{
    PopupLoading::show();
    FriendManager::getInstance()->requestFriendInfo(CC_CALLBACK_1(LayerFriendList::callbackList, this));
}

#pragma mark - callback
void LayerFriendList::callbackList(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            if ( _onCallbackHide != nullptr )
            {
                _onCallbackHide();
            }
        });
        popup->show();
        return;
    }
    
    //
    drawInfo();
    drawTable();
    uiBottom();
}
 
void LayerFriendList::callbackPet(bool bResult)
{
    PopupLoading::hide();
    
     //
     if ( bResult == false )
     {
         auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
         popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
         popup->show();
               
         if ( _onCallbackHide != nullptr )
         {
             _onCallbackHide();
         }
         return;
     }
    
    //
    uiBottom();
}

void LayerFriendList::callbackDel(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( nResult == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_friend_msg_7"), GAME_TEXT("t_ui_friend_msg_11"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else
    {
        requestInfo();
    }
}

void LayerFriendList::callbackPointReceive(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    
    //
    drawInfo();
    requestInfo();
    
    std::string strMessage = "";
    if ( bResult == true )
    {
        strMessage = GAME_TEXT("t_ui_friend_msg_18");
    }
    else if ( bResult == false && nResult == 5 )
    {
        strMessage = GAME_TEXT("t_ui_friend_msg_20");
    }
    
    //
    if ( strMessage.length() != 0 )
    {
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFunc::create([=](){
            PopupToast::show(strMessage);
        });
        
        auto seq = Sequence::create(action1, action2, NULL);
        runAction(seq);
    }
}

void LayerFriendList::callbackPointSend(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    
    //
    drawInfo();
    requestInfo();
    
    std::string strMessage = "";
    if ( bResult == true )
    {
        strMessage = GAME_TEXT("t_ui_friend_msg_17");
    }
    else if ( bResult == false && nResult == 3 )
    {
        strMessage = GAME_TEXT("t_ui_friend_msg_19");
    }
    
    //
    if ( strMessage.length() != 0 )
    {
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFunc::create([=](){
            PopupToast::show(strMessage);
        });
        
        auto seq = Sequence::create(action1, action2, NULL);
        runAction(seq);
    }
}

void LayerFriendList::callbackPointAll(bool bResult, int nResult)
{
    //
    PopupLoading::hide();
    
    //
    drawInfo();
    requestInfo();
    
    std::string strMessage = "";
    if ( bResult == true )
    {
        strMessage = GAME_TEXT("t_ui_friend_msg_17");
    }
    else if ( bResult == false && nResult == 3 )
    {
        strMessage = GAME_TEXT("t_ui_friend_msg_19");
    }
    
    //
    if ( strMessage.length() != 0 )
    {
        auto action1 = DelayTime::create(1.0);
        auto action2 = CallFunc::create([=](){
            PopupToast::show(strMessage);
        });
        
        auto seq = Sequence::create(action1, action2, NULL);
        runAction(seq);
    }
}
