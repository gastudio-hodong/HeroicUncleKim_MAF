//
//  PopupChat.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupChat.h"

#include "Common/ConfigKey.h"
#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoChat.h"

#include "GameUIPopup/Other/Chat/PopupChatInfo.h"
#include "GameUIPopup/Other/Chat/PopupChatBlockList.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ChatManager.h"

//
#include <regex>

USING_NS_CC;

PopupChat* PopupChat::create(E_CHAT_TYPE eType)
{
    PopupChat* pRet = new(std::nothrow) PopupChat();
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

PopupChat::PopupChat():
_eType(E_CHAT_TYPE::NORMAL),

_isRequestChatList(false),

_layerContainerContents(nullptr),
_layerontainerInput(nullptr),

_table(nullptr),
_edit(nullptr),

_labelListNothing(nullptr),
_spriteShadow(nullptr)
{
    
}

PopupChat::~PopupChat()
{
    
}

bool PopupChat::init(E_CHAT_TYPE eType)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _eType = eType;
    
    //
    initVar();
    initUI();
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::CHATTING, this);
    
    return true;
}


void PopupChat::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    ChatManager::getInstance()->requestInfo(_eType, CC_CALLBACK_2(PopupChat::callbackInfo, this));
}

#pragma mark - init
void PopupChat::initVar()
{
    _listChatInfo.clear();
    _listChatInfo = ChatManager::getInstance()->getChatList();
}

void PopupChat::initUI()
{
    // size
    Size size = Size(734, getContentSize().height);
    if ( size.height > 1210 )
        size.height = 1210;
    
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_chat"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupChat::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerontainerInput = LayerColor::create();
    _layerontainerInput->setContentSize( Size(size.width - 25, 90) );
    _layerontainerInput->setIgnoreAnchorPointForPosition(false);
    _layerontainerInput->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerontainerInput->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerontainerInput);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 25, size.height - 75 - _layerontainerInput->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
    _spriteContainer->addChild(_layerContainerContents);
    
    //
    uiContent();
    uiInput();
}

#pragma mark - table
void PopupChat::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
 
    if ( view->isDragging() == true )
    {
        if ( view->getContentOffset().y >= 150 )
        {
            _isRequestChatList = true;
        }
        else
        {
            _isRequestChatList = false;
        }

    }
    else if ( _isRequestChatList == true )
    {
        this->runAction(CallFunc::create([=](){
            PopupLoading::show();
            ChatManager::getInstance()->requestInfo(_eType, CC_CALLBACK_2(PopupChat::callbackInfo, this));
        }));
        
        _isRequestChatList = false;
    }

    if ( view->getContentOffset().y >= 100)
    {
        _spriteShadow->setVisible(true);
    }
    else
    {
        _spriteShadow->setVisible(false);
    }
}

void PopupChat::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    ssize_t idx = cell->getIdx();
    
    auto obj = (InfoChat*)_listChatInfo.at(idx);
    
    if ( obj->getUserIdx() != AccountManager::getInstance()->getUserIdx() || obj->getUserPlatform().compare(AccountManager::getInstance()->getUserPlatform()) != 0 )
    {
        auto popup = PopupChatInfo::create(obj);
        popup->show();
    }
}

Size PopupChat::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 100);
}

extension::TableViewCell* PopupChat::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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

    // obj
    auto obj = _listChatInfo.at(idx);
    
    int nRank = obj->getRank();
    int nSkin = obj->getSkin();
    int nCostume = obj->getCostume();
    std::string strNick = obj->getUserNick();
    std::string strMessage = obj->getMessage();
//    strMessage = "환생은 그냥 먹히실때마 하면 됩니다. 환생횟수가 중요한게 아니라서";
    
    auto spriteBG = Sprite::create("Assets/ui/chatting/chatting_bar01.png");
    spriteBG->setPosition(size.width / 2, size.height / 2);
    cell->addChild(spriteBG);
    
    // skin
    auto spriteSkinBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
    spriteSkinBox->setContentSize(Size(62, 62));
    spriteSkinBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    spriteSkinBox->setPosition(52, spriteBG->getContentSize().height - 10);
    spriteBG->addChild(spriteSkinBox);
    {
        bool bSkinShow = false;
        if ( nSkin != 0 )
        {
            auto spriteSkin = Sprite::create(MafUtils::format("Assets/icon_hero/skin_%d.png", nSkin));
            if ( spriteSkin != nullptr )
            {
                spriteSkin->setPosition(spriteSkinBox->getContentSize().width / 2, spriteSkinBox->getContentSize().height / 2);
                spriteSkinBox->addChild(spriteSkin);
                
                auto spriteSkinBoxLight = ui::Scale9Sprite::create("Assets/ui_common/box_item_1_1.png", Rect::ZERO, Rect(5,30,10,10));
                spriteSkinBoxLight->setContentSize(Size(62, 62));
                spriteSkinBoxLight->setPosition(spriteSkinBox->getContentSize().width / 2, spriteSkinBox->getContentSize().height / 2);
                spriteSkinBox->addChild(spriteSkinBoxLight);
                
                //
                bSkinShow = true;
            }
        }
        
        if ( bSkinShow == false )
        {
            auto spriteSkin = Sprite::create(MafUtils::format("Assets/icon_costume/head_%02d.png", nCostume));
            spriteSkin->setPosition(spriteSkinBox->getContentSize().width / 2, spriteSkinBox->getContentSize().height / 2);
            spriteSkin->setScale(1.2f);
            spriteSkinBox->addChild(spriteSkin);
        }
    }
    
    // rank
    if ( nRank > 0 && nRank <= 200 )
    {
        std::string strRankIconPath = "Assets/icon/icon_tower_s.png";
        if ( _eType == E_CHAT_TYPE::RAID )
        {
            strRankIconPath = "Assets/icon/icon_raid_s.png";
        }
        auto spriteRankIcon = Sprite::create(strRankIconPath);
        spriteRankIcon->setPosition(20, 20);
        spriteBG->addChild(spriteRankIcon);
        
        auto labelRank = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_rank_now", nRank), GAME_FONT, 18);
        labelRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelRank->setPosition(20 + 20, 15);
        labelRank->setColor(Color3B(142, 127, 88));
        spriteBG->addChild(labelRank);
    }
    
    
    // nick
    auto labelNick = Label::createWithSystemFont(strNick, SYSTEM_FONT, 24);
    labelNick->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    labelNick->setPosition(150, spriteBG->getContentSize().height / 2 + 10);
    labelNick->setColor(Color3B(128, 128, 128));
    spriteBG->addChild(labelNick);
    
    // message
    auto labelMessage = Label::createWithSystemFont(strMessage, SYSTEM_FONT, 24);
    labelMessage->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    labelMessage->setPosition(150, spriteBG->getContentSize().height / 2 + 10);
    labelMessage->setDimensions(450, 50);
    labelMessage->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelMessage->setOverflow(Label::Overflow::CLAMP);
    spriteBG->addChild(labelMessage);
    
    return cell;
}

ssize_t PopupChat::numberOfCellsInTableView(extension::TableView *table)
{
    return _listChatInfo.size();
}

#pragma mark - refresh
void PopupChat::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType != E_REFRESH::CHATTING )
    {
        return;
    }
    
    //
    _listChatInfo.clear();
    _listChatInfo = ChatManager::getInstance()->getChatList();
    
    //
    drawTable(true);
}

#pragma mark - ui
void PopupChat::uiContent()
{
    _layerContainerContents->removeAllChildren();
    
    //
    _table = extension::TableView::create(this, _layerContainerContents->getContentSize());
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition( Vec2(_layerContainerContents->getContentSize().width / 2, 0) );
    _table->setDelegate(this);
    _layerContainerContents->addChild(_table);
    
    // label nothing
    _labelListNothing = Label::createWithTTF(GAME_TEXT("t_ui_msg_nodata"), GAME_FONT, 24);
    _labelListNothing->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2);
    _labelListNothing->setVisible(false);
    _layerContainerContents->addChild(_labelListNothing);
    
    //
    _spriteShadow = Sprite::create("Assets/ui/chatting/chatting_shadow_1.png");
    _spriteShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteShadow->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
    _spriteShadow->setVisible(false);
    _layerContainerContents->addChild(_spriteShadow);
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    _layerContainerContents->addChild(menu);
    
    auto itemReload = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_refresh2.png"), nullptr, CC_CALLBACK_1(PopupChat::onClickReload, this));
    menu->addChild(itemReload);
    
    auto itemSetting = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/icon_setting.png"), nullptr, CC_CALLBACK_1(PopupChat::onClickSetting, this));
    menu->addChild(itemSetting);
    
    {
        float padding = 10;
        float width = -padding;
        float height = 0;
        for(const auto &child : menu->getChildren())
        {
            width += child->getContentSize().width * child->getScaleX() + padding;
            if ( height < child->getContentSize().height * child->getScaleY() )
            {
                height = child->getContentSize().height * child->getScaleY();
            }
        }
        
        //
        menu->alignItemsHorizontallyWithPadding(20);
        menu->setPosition(_layerContainerContents->getContentSize().width - 20 - width / 2, 20 + height / 2);
    }
    
    //
    drawContent();
}

void PopupChat::uiInput()
{
    _layerontainerInput->removeAllChildren();
    
    //
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerontainerInput->addChild(menu);

    // item send
    auto itemSend = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/chatting/chatting_send_1_2.png"), nullptr, CC_CALLBACK_1(PopupChat::onClickMessage, this));
    itemSend->setPosition(_layerontainerInput->getContentSize().width - 65, _layerontainerInput->getContentSize().height / 2);
    menu->addChild(itemSend);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_costume_review_register"), GAME_FONT, 24);
        label->setPosition( Vec2(itemSend->getContentSize().width / 2, itemSend->getContentSize().height / 2) );
        itemSend->addChild(label);
    }

    // EditBox
    _edit = ui::EditBox::create(Size(_spriteContainer->getContentSize().width - itemSend->getContentSize().width - 40 , 62), "Assets/ui/chatting/chatting_send_1_1.png");
    _edit->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
    _edit->setPosition(Vec2(5, _layerontainerInput->getContentSize().height / 2));
    _edit->setMaxLength(40);
    _edit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    _edit->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    _edit->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);
    _edit->setEnabled(true);
    _edit->setFontColor(Color3B::WHITE);
    _edit->setPlaceholderFont(GAME_FONT, 24);
    _edit->setFont(GAME_FONT, 24);
    _edit->setPlaceHolder(GAME_TEXT("t_ui_transfer_input").c_str());
    _layerontainerInput->addChild(_edit);
}

void PopupChat::drawContent()
{
    // label empty
    if ( _listChatInfo.size() != 0 )
    {
        _labelListNothing->setVisible(false);
    }
    else
    {
        _labelListNothing->setVisible(true);
    }
}

void PopupChat::drawTable(bool bReload)
{
    if ( bReload == true )
    {
        _table->reloadData();
    }
    else
    {
        if ( _table->getContentOffset().y > 0 )
        {
            _table->reloadData();
        }
        else
        {
            auto offset = _table->minContainerOffset();
            _table->reloadData();
            _table->setContentOffset(offset);
        }
    }
}

void PopupChat::drawInput()
{
    _edit->setText("");
}

#pragma mark - set, get

#pragma mark - callback
void PopupChat::callbackInfo(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        /*
         0 : 유저 정보 없음
         1 : 성공
         2 : 채팅 벤 유저
         */
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_error_7");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    _listChatInfo.clear();
    _listChatInfo = ChatManager::getInstance()->getChatList();
    
    //
    drawContent();
    drawTable(true);
    drawInput();
}

void PopupChat::callbackMessage(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        /*
         0 : 유저 정보 없음
         1 : 성공
         2 : 채팅 벤 유저
         */
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_error_7");
        }
      
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    _listChatInfo.clear();
    _listChatInfo = ChatManager::getInstance()->getChatList();
    
    //
    drawContent();
    drawTable(true);
    drawInput();
}

#pragma mark - click
void PopupChat::onClickClose(Ref* sender)
{
    hide();
}

void PopupChat::onClickReload(Ref* sender)
{
    PopupLoading::show();
    ChatManager::getInstance()->requestInfo(_eType, CC_CALLBACK_2(PopupChat::callbackInfo, this));
}

void PopupChat::onClickSetting(Ref* sender)
{
    auto popup = PopupChatBlockList::create();
    popup->show();
}

void PopupChat::onClickMessage(cocos2d::Ref* sender)
{
    std::regex reg("\xF0[\x90-\xBF][\x80-\xBF]{2}|[\xF1-\xF3][\x80-\xBF]{3}|\xF4[\x80-\x8F][\x80-\xBF]{2}");
    std::string strContests = _edit->getText();
    strContests = MafUtils::TrimRight(strContests);
    strContests = std::regex_replace(strContests, reg, "");
    
    _edit->setText(strContests.c_str());
    if ( strContests.length() == 0 )
    {
        return;
    }

    std::string strCheckBadWord = strContests;
    strCheckBadWord = MafUtils::eraseCharcter(strCheckBadWord, ' ');
    
    if ( TextManager::getInstance()->isBadWord(strCheckBadWord) == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_chat_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( ChatManager::getInstance()->getTimeMessageSend() >= UtilsDate::getInstance()->getTime() )
    {
        PopupToast::show(GAME_TEXTFORMAT("t_ui_chat_cooldown_1", ChatManager::getInstance()->getTimeMessageCoolDown()));
        return;
    }
    
    std::string strCloudVer = UserDefault::getInstance()->getStringForKey(KEY_CLOUD_VER, "");
    if ( strCloudVer.length() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_55"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    ChatManager::getInstance()->requestMessage(_eType, strContests, CC_CALLBACK_2(PopupChat::callbackMessage, this));
}
