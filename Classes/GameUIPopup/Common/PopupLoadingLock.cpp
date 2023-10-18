//
//  PopupLoadingLock.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#include "PopupLoadingLock.h"

USING_NS_CC;

int PopupLoadingLock::_nCountVisible = 0;

PopupLoadingLock::PopupLoadingLock(void)
{
    
}

PopupLoadingLock::~PopupLoadingLock(void)
{

}

bool PopupLoadingLock::init(void)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setContentSize(UtilsRect::getVisibleSize());
    setTouchEnabled(true);

    return true;
}

#pragma mark - set, get

#pragma mark - ui

#pragma mark - show, lock, hide
bool PopupLoadingLock::isShow()
{
    bool ret = false;
    
    // notification remove
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOADINGLOCK) != nullptr )
    {
        ret = true;
    }
    
    return ret;
}

void PopupLoadingLock::show()
{
    // notification
    auto node = Director::getInstance()->getNotificationNode();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_NOTI_LOADINGLOCK) != nullptr )
    {
        PopupLoadingLock::_nCountVisible++;
        return;
    }
    
    PopupLoadingLock::_nCountVisible = 1;
    
    // notification add
    auto popup = PopupLoadingLock::create();
    node->addChild(popup, ZORDER_NOTI_LOADINGLOCK, TAG_NOTI_LOADINGLOCK);
}

void PopupLoadingLock::hide(bool bAll)
{
    // visible
    if ( bAll == false )
    {
        PopupLoadingLock::_nCountVisible--;
        if ( PopupLoadingLock::_nCountVisible > 0 )
            return;
    }
    
    PopupLoadingLock::_nCountVisible = 0;
    
    // notification remove
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOADINGLOCK) != nullptr )
    {
        // event
        node->removeChildByTag(TAG_NOTI_LOADINGLOCK);
    }
}
