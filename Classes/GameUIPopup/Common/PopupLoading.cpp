//
//  PopupLoading.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#include "PopupLoading.h"

USING_NS_CC;

int PopupLoading::_nCountVisible = 0;

PopupLoading::PopupLoading(void)
{
    
}

PopupLoading::~PopupLoading(void)
{

}

bool PopupLoading::init(void)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setContentSize(UtilsRect::getVisibleSize());
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Color3B::BLACK);
    setBackGroundColorOpacity(165);
    setTouchEnabled(true);
    
    // ui
    uiLoading();
    
    return true;
}

#pragma mark - set, get

#pragma mark - ui
void PopupLoading::uiLoading()
{
    // container
    auto container = Node::create();
    container->setContentSize( Size(getContentSize().width, getContentSize().height) );
    container->setAnchorPoint( Vec2(0.5f, 0.5f) );
    container->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    addChild(container);
    
    // run
    auto aniframeRun = Animation::create();
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading1.png");
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading2.png");
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading3.png");
    aniframeRun->addSpriteFrameWithFile("Assets/ui_common/loading4.png");
    aniframeRun->setDelayPerUnit(0.1f);
    
    auto aniRun = Animate::create(aniframeRun);
    
    auto sprintRun = Sprite::create("Assets/ui_common/loading1.png");
    sprintRun->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    sprintRun->runAction(RepeatForever::create(aniRun));
    container->addChild(sprintRun);
    
    // text
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_msg_wait"), GAME_FONT, 36);
    labelText->setAnchorPoint( Vec2(0.5, 1) );
    labelText->setPosition( Vec2(getContentSize().width / 2, getContentSize().height * 0.45) );
    container->addChild(labelText);
}

#pragma mark - show, lock, hide
bool PopupLoading::isShow()
{
    bool ret = false;
    
    // notification remove
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOADING) != nullptr )
    {
        ret = true;
    }
    
    return ret;
}

void PopupLoading::show()
{
    // notification
    auto node = Director::getInstance()->getNotificationNode();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( PopupLoading::_nCountVisible < 0 )
        PopupLoading::_nCountVisible = 0;
    
    if ( node->getChildByTag(TAG_NOTI_LOADING) != nullptr )
    {
        PopupLoading::_nCountVisible++;
        return;
    }
    
    PopupLoading::_nCountVisible = 1;
    
    // notification add
    auto popup = PopupLoading::create();
    node->addChild(popup, ZORDER_NOTI_LOADING, TAG_NOTI_LOADING);
}

void PopupLoading::hide(bool bAll)
{
    // visible
    if ( bAll == false )
    {
        PopupLoading::_nCountVisible--;
        
        if ( PopupLoading::_nCountVisible < 0 )
            PopupLoading::_nCountVisible = 0;
        
        if ( PopupLoading::_nCountVisible > 0 )
            return;
    }
    
    PopupLoading::_nCountVisible = 0;
    
    // notification remove
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOADING) != nullptr )
    {
        // event
        node->removeChildByTag(TAG_NOTI_LOADING);
    }
}
