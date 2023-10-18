//
//  LayerFriendInvite.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerFriendInvite.h"

#include "Common/ConfigGameNormal.h"

#include "GameUnit/UnitBehaviorHeroAnother.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Ranking/PopupOtherUserInfo.h"

#include "ManagerGame/FriendManager.h"

USING_NS_CC;

LayerFriendInvite* LayerFriendInvite::create(cocos2d::Size size)
{
    LayerFriendInvite* pRet = new(std::nothrow) LayerFriendInvite();
    if ( pRet && pRet->init(size) )
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

LayerFriendInvite::LayerFriendInvite(void) :
_eType(LayerFriendInvite::type_wait),
_updateTimeRecommend(0),
_updateTimeWait(0),
_onCallbackHide(nullptr),


_layerContainerSearch(nullptr),
_layerContainerInfo(nullptr),
_layerContainerList(nullptr),
_layerContainerBottom(nullptr),
_tableList(nullptr),
_labelInfoName(nullptr),
_labelInfoRefresh(nullptr),
_itemInfoRefresh(nullptr),
_itemIsRecommend(nullptr),
_itemIsRecommendInfo(nullptr),
_labelIsRecommendText(nullptr)
{
    
}

LayerFriendInvite::~LayerFriendInvite(void)
{
    
}

bool LayerFriendInvite::init(cocos2d::Size size)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    // init
    initVar();
    initUi();
    
    //
    requestWait();
    
    return true;
}

#pragma mark - table
void LayerFriendInvite::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
    
}

Size LayerFriendInvite::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, C_BAR_MISSION_SIZE);
}

extension::TableViewCell* LayerFriendInvite::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
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
    Vector<InfoFriend*> vec;
    if ( _eType == LayerFriendInvite::type_recommend )
    {
        vec = FriendManager::getInstance()->getFriendRecommendList();
    }
    else if ( _eType == LayerFriendInvite::type_wait )
    {
        vec = FriendManager::getInstance()->getFriendWaitList();
    }
    else if ( _eType == LayerFriendInvite::type_search )
    {
        vec = FriendManager::getInstance()->getFriendSearchList();
    }
    
    //
    auto obj = vec.at(idx);
 

    // bg
    std::string strPathBackground = MafUtils::format("Assets/ui_common/list_bg_%d.png", 1);
    
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
        
        auto itemHeroBox = MafNode::MafMenuItemSprite::create(spriteHeroBG, nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickUserDetail, this));
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
        
        auto hero = UnitBehaviorHeroAnother::create(E_PLACE::DG_NORMAL);;
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

    if( _eType == LayerFriendInvite::type_recommend )
    {
        auto spriteAdd = Sprite::create("Assets/ui/friend/icon_friend_1.png");
        spriteAdd->setScale(2);
        
        auto btnAddFriend = MafNode::MafMenuItemSprite::create(spriteAdd, nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickRecommendAdd,this));
        btnAddFriend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        btnAddFriend->setPosition(spriteBG->getContentSize().width - 50, spriteBG->getContentSize().height / 2);
        btnAddFriend->setTag((int)idx);
        menu->addChild(btnAddFriend);
    }
    else if( _eType == LayerFriendInvite::type_wait )
    {
        auto spriteAccept = ui::Scale9Sprite::create("Assets/ui_common/btn_1_5_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteAccept->setContentSize(Size(87, 87));
        
        auto spriteReject = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteReject->setContentSize(Size(87, 87));
        
        
        auto itemReject = MafNode::MafMenuItemSprite::create(spriteReject, nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickWaitReject, this));
        itemReject->setPosition(spriteBG->getContentSize().width - 158.5, spriteBG->getContentSize().height / 2);
        itemReject->setTag((int)idx);
        menu->addChild(itemReject);
        {
            auto spriteIcon = Sprite::create("Assets/ui_common/btn_x_01.png");
            spriteIcon->setPosition(itemReject->getContentSize().width / 2, itemReject->getContentSize().height / 2);
            itemReject->addChild(spriteIcon);
        }
        
        auto itemAccept = MafNode::MafMenuItemSprite::create(spriteAccept, nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickWaitAccept, this));
        itemAccept->setPosition(spriteBG->getContentSize().width - 61.5, spriteBG->getContentSize().height / 2);
        itemAccept->setTag((int)idx);
        menu->addChild(itemAccept);
        {
            auto spriteIcon = Sprite::create("Assets/ui_common/btn_check_01.png");
            spriteIcon->setPosition(itemAccept->getContentSize().width / 2, itemAccept->getContentSize().height / 2);
            itemAccept->addChild(spriteIcon);
        }
    }
    else if( _eType == LayerFriendInvite::type_search )
    {
        std::string strIconPath = "Assets/ui/friend/icon_friend_aos.png";
        if ( obj->getPlatform().compare("ios") == 0 )
        {
            strIconPath = "Assets/ui/friend/icon_friend_ios.png";
        }
        auto spriteIcon = Sprite::create(strIconPath);
        spriteIcon->setPosition(spriteBG->getContentSize().width - 158.5, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteIcon);
            
        //
        auto spriteAdd = Sprite::create("Assets/ui/friend/icon_friend_1.png");
        spriteAdd->setScale(2);
        
        auto btnAddFriend = MafNode::MafMenuItemSprite::create(spriteAdd, nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickSearchAdd,this));
        btnAddFriend->setPosition(spriteBG->getContentSize().width - 61.5, spriteBG->getContentSize().height / 2);
        btnAddFriend->setTag((int)idx);
        menu->addChild(btnAddFriend);
    }
    
    return cell;
}
ssize_t LayerFriendInvite::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    ssize_t size = 0;
    if ( _eType == LayerFriendInvite::type_recommend )
        size = FriendManager::getInstance()->getFriendRecommendList().size();
    else if ( _eType == LayerFriendInvite::type_wait )
        size = FriendManager::getInstance()->getFriendWaitList().size();
    else if ( _eType == LayerFriendInvite::type_search )
        size = FriendManager::getInstance()->getFriendSearchList().size();
    
    return size;
}

#pragma mark - init
void LayerFriendInvite::initVar()
{
    
}

void LayerFriendInvite::initUi()
{
    //
    _layerContainerSearch = Layer::create();
    _layerContainerSearch->setContentSize( Size(getContentSize().width, 170) );
    _layerContainerSearch->setIgnoreAnchorPointForPosition(false);
    _layerContainerSearch->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerSearch->setPosition(getContentSize().width / 2, getContentSize().height);
    addChild(_layerContainerSearch);
    
    //
    _layerContainerBottom = Layer::create();
    _layerContainerBottom->setContentSize( Size(getContentSize().width, 100) );
    _layerContainerBottom->setIgnoreAnchorPointForPosition(false);
    _layerContainerBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBottom->setPosition(getContentSize().width / 2, 0);
    addChild(_layerContainerBottom);
    
    //
    auto layerMiddle = Layer::create();
    layerMiddle->setContentSize(Size(getContentSize().width, getContentSize().height - _layerContainerSearch->getContentSize().height - _layerContainerBottom->getContentSize().height));
    layerMiddle->setIgnoreAnchorPointForPosition(false);
    layerMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layerMiddle->setPosition(getContentSize().width / 2, _layerContainerBottom->getPositionY() + _layerContainerBottom->getContentSize().height);
    addChild(layerMiddle);
    {
        auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_1.png", Rect::ZERO, Rect(0,0,20,20));
        spriteBG->setContentSize(layerMiddle->getContentSize());
        spriteBG->setPosition(layerMiddle->getContentSize().width / 2, layerMiddle->getContentSize().height / 2);
        layerMiddle->addChild(spriteBG);
        
        _layerContainerInfo = Layer::create();
        _layerContainerInfo->setContentSize( Size(layerMiddle->getContentSize().width, 55) );
        _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
        _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerInfo->setPosition(layerMiddle->getContentSize().width / 2, layerMiddle->getContentSize().height);
        layerMiddle->addChild(_layerContainerInfo);

        _layerContainerList = Layer::create();
        _layerContainerList->setContentSize( Size(layerMiddle->getContentSize().width, layerMiddle->getContentSize().height - _layerContainerInfo->getContentSize().height) );
        _layerContainerList->setIgnoreAnchorPointForPosition(false);
        _layerContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
        _layerContainerList->setPosition(getContentSize().width / 2, 0);
        layerMiddle->addChild(_layerContainerList);
    }
    
    //
    uiSearch();
    uiInfo();
    uiList();
    uiBottom();
}

#pragma mark - ui
void LayerFriendInvite::uiSearch()
{
    //
    _layerContainerSearch->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerSearch->addChild(menu);
    
    //
    auto _editBox = ui::EditBox::create(Size(466,68), ui::Scale9Sprite::create("Assets/ui/raid/nickname.png"));
    _editBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _editBox->setPosition(Vec2(25, _layerContainerSearch->getContentSize().height - 50));
    _editBox->setFont(GAME_FONT, 34);
    _editBox->setMaxLength(30);
    _editBox->setInputMode(ui::EditBox::InputMode::URL);
    _editBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    _editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editBox->setName("EDIT");
    _layerContainerSearch->addChild(_editBox);
    
    //
    auto itemSearch = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/friend/btn_friend_name_search1.png"), nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickSearch,this));
    itemSearch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    itemSearch->setPosition(_layerContainerSearch->getContentSize().width - 25, _layerContainerSearch->getContentSize().height - 50);
    menu->addChild(itemSearch);
    
    
    std::string strIsRecommendPath = "Assets/ui/mission/mission_on.png";
    if ( FriendManager::getInstance()->getFriendIsRecommend() == 0 )
    {
        strIsRecommendPath = "Assets/ui/mission/mission_off.png";
    }
    _itemIsRecommend = MafNode::MafMenuItemSprite::create(Sprite::create(strIsRecommendPath), nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickIsRecommend,this));
    _itemIsRecommend->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _itemIsRecommend->setPosition(25, 50);
    menu->addChild(_itemIsRecommend);
    {
        _labelIsRecommendText = Label::createWithTTF("", GAME_FONT, 24);
        _labelIsRecommendText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _labelIsRecommendText->setPosition(200, 50);
        _layerContainerSearch->addChild(_labelIsRecommendText);
        
        _itemIsRecommendInfo = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/mission/btn_info1_1.png"), nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickIsRecommendInfo,this));
        _itemIsRecommendInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        _itemIsRecommendInfo->setPosition(_labelIsRecommendText->getPositionX() + _labelIsRecommendText->getContentSize().width, 50);
        menu->addChild(_itemIsRecommendInfo);
    }
}

void LayerFriendInvite::uiInfo()
{
    //
    _layerContainerInfo->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerInfo->addChild(menu);
    
    //
    _labelInfoName = Label::createWithTTF(GAME_TEXT("t_ui_friend_msg_23"), GAME_FONT, 24);
    _labelInfoName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _labelInfoName->setPosition(10, _layerContainerInfo->getContentSize().height / 2);
    _labelInfoName->setColor(COLOR_COIN);
    _layerContainerInfo->addChild(_labelInfoName);

    _itemInfoRefresh = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/friend/btn_friend_refresh.png"), nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickRefresh,this));
    _itemInfoRefresh->setPosition(_layerContainerInfo->getContentSize().width - 30, _layerContainerInfo->getContentSize().height / 2);
    menu->addChild(_itemInfoRefresh);
    
    _labelInfoRefresh = Label::createWithTTF("", GAME_FONT, 20);
    _labelInfoRefresh->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _labelInfoRefresh->setPosition(_itemInfoRefresh->getPositionX() - 40, _layerContainerInfo->getContentSize().height / 2);
    _layerContainerInfo->addChild(_labelInfoRefresh);
}

void LayerFriendInvite::uiList()
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

void LayerFriendInvite::uiBottom()
{
    //
    _layerContainerBottom->removeAllChildren();
    
    //
    auto spriteLine = Sprite::create("Assets/ui/friend/friend_line_01.png");
    spriteLine->setPosition(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height / 2);
    _layerContainerBottom->addChild(spriteLine);
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerBottom->getContentSize().width / 2, _layerContainerBottom->getContentSize().height / 2);
    _layerContainerBottom->addChild(menu);
    
    std::string strRecommendPath = "Assets/ui/friend/btn_friend_invite1_2.png";
    std::string strWaitPath = "Assets/ui/friend/btn_friend_invite2_2.png";
    if ( _eType == LayerFriendInvite::type_recommend )
    {
        strRecommendPath = "Assets/ui/friend/btn_friend_invite1_1.png";
    }
    else if ( _eType == LayerFriendInvite::type_wait )
    {
        strWaitPath = "Assets/ui/friend/btn_friend_invite2_1.png";
    }
    
    auto itemWait = MafNode::MafMenuItemSprite::create(Sprite::create(strWaitPath), nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickWait,this));
    menu->addChild(itemWait);
    
    auto itemRecommend = MafNode::MafMenuItemSprite::create(Sprite::create(strRecommendPath), nullptr, CC_CALLBACK_1(LayerFriendInvite::onClickRecommend,this));
    menu->addChild(itemRecommend);
    
    //
    menu->alignItemsHorizontallyWithPadding(200);
}

#pragma mark - draw
void LayerFriendInvite::drawInfo()
{
    //
    {
        std::string strName = GAME_TEXT("t_ui_friend_msg_23");
        if ( _eType == LayerFriendInvite::type_recommend )
            strName = GAME_TEXT("t_ui_friend_msg_23");
        else if ( _eType == LayerFriendInvite::type_wait )
            strName = GAME_TEXT("t_ui_friend_msg_24");
        else if ( _eType == LayerFriendInvite::type_search )
            strName = GAME_TEXT("t_ui_friend_msg_25");

        _labelInfoName->setString(strName);
    }
    
    {
        if ( _eType == LayerFriendInvite::type_search )
        {
            _itemInfoRefresh->setVisible(false);
            _labelInfoRefresh->setVisible(false);
        }
        else
        {
            _itemInfoRefresh->setVisible(true);
            _labelInfoRefresh->setVisible(true);
            
            auto action1 = CallFuncN::create([=](Node* sender){
            
                std::string strMessage = GAME_TEXT("t_ui_friend_info_3");
                int64_t nTime = UtilsDate::getInstance()->getTime();
                
                if ( _eType == LayerFriendInvite::type_recommend )
                {
                    nTime = _updateTimeRecommend - nTime;
                }
                else if ( _eType == LayerFriendInvite::type_wait )
                {
                    nTime = _updateTimeWait - nTime;
                }

                if ( nTime > 0 )
                {
                    strMessage = GAME_TEXTFORMAT("t_ui_friend_msg_13", (int)nTime);
                    ((Label*)sender)->setString(strMessage);
                }
                else
                {
                    ((Label*)sender)->setString(strMessage);
                    ((Label*)sender)->stopAllActions();
                }
            });
            auto action2 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, NULL);
            _labelInfoRefresh->runAction(RepeatForever::create(seq));
        }
    }

    auto labelSpeed = (Label*)_layerContainerInfo->getChildByName("SPEED");
    if ( labelSpeed != nullptr )
    {
        labelSpeed->setString(GAME_TEXTFORMAT("t_ui_friend_info_2",FriendManager::getInstance()->getFriendBonusSpeed()));
    }
}

void LayerFriendInvite::drawTable()
{
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
    
    int64_t nSize = 0;
    if ( _eType == LayerFriendInvite::type_recommend )
    {
        nSize = FriendManager::getInstance()->getFriendRecommendList().size();
        labelText->setString(GAME_TEXT("t_ui_friend_msg_27"));
    }
    else if ( _eType == LayerFriendInvite::type_wait )
    {
        nSize = FriendManager::getInstance()->getFriendWaitList().size();
        labelText->setString(GAME_TEXT("t_ui_friend_msg_26"));
    }
    else if ( _eType == LayerFriendInvite::type_search )
    {
        nSize = FriendManager::getInstance()->getFriendSearchList().size();
        labelText->setString(GAME_TEXT("t_ui_friend_msg_28"));
    }
    
    if ( nSize == 0 )
    {
        labelText->setVisible(true);
    }
    else
    {
        labelText->setVisible(false);
    }
}

void LayerFriendInvite::drawSearch()
{
    std::string strIsRecommendPath = "Assets/ui/mission/mission_on.png";
    std::string strIsRecommendText = GAME_TEXT("t_ui_friend_msg_35");
    if ( FriendManager::getInstance()->getFriendIsRecommend() == 0 )
    {
        strIsRecommendPath = "Assets/ui/mission/mission_off.png";
        strIsRecommendText = GAME_TEXT("t_ui_friend_msg_34");
    }
    
    ((Sprite*)_itemIsRecommend->getNormalImage())->setTexture(strIsRecommendPath);
    _labelIsRecommendText->setString(strIsRecommendText);
    _itemIsRecommendInfo->setPositionX(_labelIsRecommendText->getPositionX() + _labelIsRecommendText->getContentSize().width);
    
}

#pragma mark - set, get, add
void LayerFriendInvite::setCallbackHide(const std::function<void(void)>& callback)
{
    _onCallbackHide = callback;
}

#pragma mark - click
void LayerFriendInvite::onClickSearch(cocos2d::Ref* sender)
{
    auto editbox = (ui::EditBox*)_layerContainerSearch->getChildByName("EDIT");
    if ( editbox == nullptr )
    {
        return;
    }
    
    if ( std::strlen(editbox->getText()) <= 0)
    {
        return;
    }
    
    std::string strNickname = editbox->getText();
    MafUtils::eraseCharcter(strNickname, ' ');
    if ( TextManager::getInstance()->isBadWord(strNickname) == true )
    {
        strNickname = "";
    }
    
    
    //
    editbox->setText(strNickname.c_str());
    
    //
    if ( strNickname.length() != 0 )
    {
        _eType = LayerFriendInvite::type_search;
        
        PopupLoading::show();
        FriendManager::getInstance()->requestSearchInfo(CC_CALLBACK_1(LayerFriendInvite::callbackInfo, this), strNickname);
    }
}

void LayerFriendInvite::onClickRecommend(cocos2d::Ref* sender)
{
    if ( _eType == LayerFriendInvite::type_recommend )
    {
        return;
    }
    
    _eType = LayerFriendInvite::type_recommend;
    requestRecommend();
}

void LayerFriendInvite::onClickWait(cocos2d::Ref* sender)
{
    if ( _eType == LayerFriendInvite::type_wait )
    {
        return;
    }
    
    _eType = LayerFriendInvite::type_wait;
    requestWait();
}

void LayerFriendInvite::onClickRefresh(cocos2d::Ref* sender)
{
    if ( _eType == LayerFriendInvite::type_recommend )
    {
        requestRecommend();
    }
    else if ( _eType == LayerFriendInvite::type_wait )
    {
        requestWait();
    }
}

void LayerFriendInvite::onClickIsRecommend(cocos2d::Ref* sender)
{
    int nFriendIsRecommend = FriendManager::getInstance()->getFriendIsRecommend();
    if ( nFriendIsRecommend == 0 )
    {
        nFriendIsRecommend = 1;
    }
    else
    {
        nFriendIsRecommend = 0;
    }
    
    PopupLoading::show();
    FriendManager::getInstance()->requestIsRecommend(CC_CALLBACK_1(LayerFriendInvite::callbackIsRecommend,this), nFriendIsRecommend);
}

void LayerFriendInvite::onClickIsRecommendInfo(cocos2d::Ref* sender)
{
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_friend_msg_36"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
}

void LayerFriendInvite::onClickRecommendAdd(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    auto listMy = FriendManager::getInstance()->getFriendList();
    auto list = FriendManager::getInstance()->getFriendRecommendList();
    auto objFriend = list.at(item->getTag());
    
    if ( listMy.size() >= 30 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_friend_msg_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    FriendManager::getInstance()->requestFriendAddRequest(CC_CALLBACK_2(LayerFriendInvite::callbackAddRequest,this), objFriend->getUseridx(), objFriend->getPlatform(), 2);
}

void LayerFriendInvite::onClickSearchAdd(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    auto listMy = FriendManager::getInstance()->getFriendList();
    auto list = FriendManager::getInstance()->getFriendSearchList();
    auto objFriend = list.at(item->getTag());
    
    if ( listMy.size() >= 30 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_friend_msg_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    FriendManager::getInstance()->requestFriendAddRequest(CC_CALLBACK_2(LayerFriendInvite::callbackAddRequest,this), objFriend->getUseridx(), objFriend->getPlatform(), 3);
}

void LayerFriendInvite::onClickWaitAccept(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    auto listMy = FriendManager::getInstance()->getFriendList();
    auto list = FriendManager::getInstance()->getFriendWaitList();
    auto objFriend = list.at(item->getTag());
    
    if ( listMy.size() >= 30 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_friend_msg_3"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    FriendManager::getInstance()->requestFriendApply(CC_CALLBACK_2(LayerFriendInvite::callbackAddResult,this), objFriend->getUseridx(), objFriend->getPlatform(), 1);
}

void LayerFriendInvite::onClickWaitReject(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    
    auto list = FriendManager::getInstance()->getFriendWaitList();
    auto objFriend = list.at(item->getTag());
    
    PopupLoading::show();
    FriendManager::getInstance()->requestFriendApply(CC_CALLBACK_2(LayerFriendInvite::callbackAddResult,this), objFriend->getUseridx(), objFriend->getPlatform(), 0);
}

void LayerFriendInvite::onClickUserDetail(cocos2d::Ref* sender)
{
    auto item = (Node*)sender;
    
    Vector<InfoFriend*> vec;
    if ( _eType == LayerFriendInvite::type_recommend )
    {
        vec = FriendManager::getInstance()->getFriendRecommendList();
    }
    else if ( _eType == LayerFriendInvite::type_wait )
    {
        vec = FriendManager::getInstance()->getFriendWaitList();
    }
    else if ( _eType == LayerFriendInvite::type_search )
    {
        vec = FriendManager::getInstance()->getFriendSearchList();
    }
    
    //
    int idx = item->getTag();
    if ( idx >= 0 && vec.size() > idx )
    {
        auto obj = vec.at(idx);
        
        auto popup = PopupOtherUserInfo::create(obj->getUseridx(), obj->getPlatform(), obj->getNickname());
        popup->show();
    }
}

#pragma mark - network
void LayerFriendInvite::requestRecommend()
{
    time_t nTime = UtilsDate::getInstance()->getTime();
    if ( _updateTimeRecommend < nTime )
    {
        PopupLoading::show();
        FriendManager::getInstance()->requestRecommendInfo(CC_CALLBACK_1(LayerFriendInvite::callbackInfo, this));
    }
    else
    {
        drawInfo();
        drawTable();
        uiBottom();
    }
}

void LayerFriendInvite::requestWait()
{
    time_t nTime = UtilsDate::getInstance()->getTime();
    if ( _updateTimeWait < nTime )
    {
        PopupLoading::show();
        FriendManager::getInstance()->requestWaitInfo(CC_CALLBACK_1(LayerFriendInvite::callbackInfo, this));
    }
    else
    {
        drawInfo();
        drawTable();
        uiBottom();
    }
}

#pragma mark - callback
void LayerFriendInvite::callbackInfo(bool bResult)
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
    
    if ( _eType == LayerFriendInvite::type_recommend )
    {
        _updateTimeRecommend = UtilsDate::getInstance()->getTime() + 30;
    }
    else if ( _eType == LayerFriendInvite::type_wait )
    {
        _updateTimeWait = UtilsDate::getInstance()->getTime() + 30;
    }
    else if ( _eType == LayerFriendInvite::type_search )
    {
        
    }
    
    //
    drawInfo();
    drawTable();
    drawSearch();
    uiBottom();
}

void LayerFriendInvite::callbackAddRequest(bool bResult, int nResult)
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
    
    /*
     0 # 해당 친구가 친구 많음
     1 # 친구 추가 성공
     3 # 이미 친구요청을 보냄
     4 # 친구의 친구대기리스트 꽉참
     5 # 이미 친구 임
     6 # 자신은 친구가 될수없음
     */
    std::string strText = "";
    switch (nResult)
    {
        case 0: strText = GAME_TEXT("t_ui_friend_msg_14");      break;
        case 1: strText = GAME_TEXT("t_ui_friend_msg_29");       break;
        case 3: strText = GAME_TEXT("t_ui_friend_msg_30");      break;
        case 4: strText = GAME_TEXT("t_ui_friend_msg_3");       break;
        case 5: strText = GAME_TEXT("t_ui_friend_msg_5");       break;
        case 6: strText = GAME_TEXT("t_ui_friend_msg_6");       break;
        case 7: strText = GAME_TEXT("t_ui_friend_msg_31");       break;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    //
    drawTable();
}

void LayerFriendInvite::callbackAddResult(bool bResult, int nResult)
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
    
    /*
     0 # 해당 친구가 친구 많음
     1 # 친구 추가 성공
     3 # 내 친구 꽉참
     4 # 친구가 요청을 보내지 않은 친구
     5 # 이미 친구 임 ( 요청 삭제 불가
     6 # 자신은 친구가 될수없음
     7 # 친구 거절 성공
     */
    std::string strText = "";
    switch (nResult)
    {
        case 0: strText = GAME_TEXT("t_ui_friend_msg_14");      break;
        case 1: strText = GAME_TEXT("t_ui_friend_msg_9");       break;
        case 3: strText = GAME_TEXT("t_ui_friend_msg_3");      break;
        case 4: strText = GAME_TEXT("t_ui_friend_msg_32");      break;
        case 5: strText = GAME_TEXT("t_ui_friend_msg_5");       break;
        case 6: strText = GAME_TEXT("t_ui_friend_msg_6");       break;
        case 7: strText = GAME_TEXT("t_ui_friend_msg_33");       break;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    //
    drawTable();
}

void LayerFriendInvite::callbackIsRecommend(bool bResult)
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
    
    //
    drawSearch();
}
