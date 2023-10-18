//
//  PopupCloudSave.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 28/06/2019.
//

#include "PopupCloudSaveLoad.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupCloudSaveLoad* PopupCloudSaveLoad::create(bool mode)
{
    PopupCloudSaveLoad* pRet = new(std::nothrow) PopupCloudSaveLoad();
    if ( pRet && pRet->init(mode) )
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
PopupCloudSaveLoad::PopupCloudSaveLoad()
{
    
}

PopupCloudSaveLoad::~PopupCloudSaveLoad(void)
{
    
}

bool PopupCloudSaveLoad::init(bool mode)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initUi(mode);
    
    return true;
}

#pragma mark - init
void PopupCloudSaveLoad::initUi(bool mode)
{
    _spriteContainer->setContentSize( Size(497, 423) );
    if(mode == true)
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_data_save"), GAME_FONT, 32);
        lbTitle->setAnchorPoint(Vec2(0.5,0.5));
        lbTitle->setColor(Color3B(255,223,28));
        lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height*0.90));
        _spriteContainer->addChild(lbTitle);
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_menu_msg_3"), GAME_FONT, 24);
        lbText->setAnchorPoint(Vec2(0.5,1));
        lbText->setPosition(Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height*0.7));
        lbText->setLineSpacing(-5);
        lbText->setDimensions(_spriteContainer->getContentSize().width * 0.9, 140);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        _spriteContainer->addChild(lbText);
        
        MafNode::MafMenu *menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        _spriteContainer->addChild(menu);
        
        auto itemYes = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr,
                                                 [=](Ref* sender){
                                                     
                                                     requestCloudSaveData();
                                                 });
        itemYes->setPosition(Vec2(_spriteContainer->getContentSize().width*0.75,_spriteContainer->getContentSize().width*0.15));
        itemYes->setAnchorPoint(Vec2(0.5,0.5));
        menu->addChild(itemYes);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 26);
            label->setPosition( Vec2(itemYes->getContentSize().width / 2, itemYes->getContentSize().height / 2) );
            label->enableBold();
            itemYes->addChild(label);
        }
        
        auto itemNo = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr,
                                                [=](Ref* sender){
                                                    
                                                    hide();
                                                    
                                                });
        itemNo->setPosition(Vec2(_spriteContainer->getContentSize().width*0.25,_spriteContainer->getContentSize().width*0.15));
        itemNo->setAnchorPoint(Vec2(0.5,0.5));
        menu->addChild(itemNo);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_no"), GAME_FONT, 26);
            label->setPosition( Vec2(itemNo->getContentSize().width / 2, itemNo->getContentSize().height / 2) );
            label->enableBold();
            itemNo->addChild(label);
        }
    }
    else if(mode == false)
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_data_load"), GAME_FONT, 32);
        lbTitle->setAnchorPoint(Vec2(0.5,0.5));
        lbTitle->setColor(Color3B(255,50,50));
        lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height*0.90));
        _spriteContainer->addChild(lbTitle);
        
        auto lbText1 = Label::createWithTTF(GAME_TEXT("t_ui_menu_msg_7"), GAME_FONT, 30);
        lbText1->setPosition(Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height*0.65));
        lbText1->setDimensions(_spriteContainer->getContentSize().width * 0.9, lbText1->getContentSize().height);
        lbText1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText1->setOverflow(Label::Overflow::SHRINK);
        lbText1->setColor(Color3B(255,255,255));
        _spriteContainer->addChild(lbText1);
        
        std::string strWarning = GAME_TEXT("t_ui_menu_msg_8");
        strWarning.append("\n");
        strWarning.append(GAME_TEXT("t_ui_menu_msg_9"));
        
        auto lbText2 = Label::createWithTTF(strWarning, GAME_FONT, 26);
        lbText2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbText2->setPosition(Vec2(_spriteContainer->getContentSize().width / 2,_spriteContainer->getContentSize().height*0.58));
        lbText2->setDimensions(_spriteContainer->getContentSize().width * 0.95, lbText2->getContentSize().height);
        lbText2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText2->setOverflow(Label::Overflow::SHRINK);
        lbText2->setColor(Color3B(255,25,25));
        lbText2->setLineBreakWithoutSpace(true);
        _spriteContainer->addChild(lbText2);
        
        MafNode::MafMenu *menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        _spriteContainer->addChild(menu);
        
        auto itemYes = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr,
                                                 [=](Ref* sender){
                                                     
                                                     requestCloudLoadData();
                                                 });
        itemYes->setPosition(Vec2(_spriteContainer->getContentSize().width*0.75,_spriteContainer->getContentSize().width*0.15));
        itemYes->setAnchorPoint(Vec2(0.5,0.5));
        menu->addChild(itemYes);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_yes"), GAME_FONT, 26);
            label->setPosition( Vec2(itemYes->getContentSize().width / 2, itemYes->getContentSize().height / 2) );
            label->enableBold();
            itemYes->addChild(label);
        }
        
        auto itemNo = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr,
                                                [=](Ref* sender){
                                                    
                                                    hide();
                                                });
        itemNo->setPosition(Vec2(_spriteContainer->getContentSize().width*0.25,_spriteContainer->getContentSize().width*0.15));
        itemNo->setAnchorPoint(Vec2(0.5,0.5));
        menu->addChild(itemNo);
        {
            auto label = Label::createWithTTF(GAME_TEXT("t_ui_no"), GAME_FONT, 26);
            label->setPosition( Vec2(itemNo->getContentSize().width / 2, itemNo->getContentSize().height / 2) );
            label->enableBold();
            itemNo->addChild(label);
        }
    }
}


#pragma mark - network
void PopupCloudSaveLoad::requestCloudSaveData(void)
{
    if ( AccountManager::getInstance()->getUserID().length() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //

    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestSave(CC_CALLBACK_1(PopupCloudSaveLoad::callbackSave, this));
}

  
void PopupCloudSaveLoad::requestCloudLoadData()
{
    if ( AccountManager::getInstance()->getUserID().length() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    PopupLoading::show();
    SaveManager::getInstance()->requestLoad(CC_CALLBACK_2(PopupCloudSaveLoad::callbackLoad, this));
}

#pragma mark - callback
void PopupCloudSaveLoad::callbackSave(bool bResult)
{
    //
    PopupLoading::hide();
      
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_msg_4"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    //
    hide();
}

void PopupCloudSaveLoad::callbackLoad(bool bResult, bool bData)
{
    //
    PopupLoading::hide();
      
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( bData == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_msg_10"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            SaveManager::getInstance()->loadCloudData();
            
            // 불러오기를 할때 서버에 로그를 남김.
            SaveManager::getInstance()->requestLoadFinish();
        });
        popup->show();
    }
    else
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_menu_msg_11"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}
