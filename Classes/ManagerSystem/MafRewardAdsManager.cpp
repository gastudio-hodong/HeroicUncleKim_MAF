//
//  AdsManager.cpp
//  FantasyClicker-mobile
//
//  Created by maf on 16/09/2019.
//

#include "MafRewardAdsManager.h"

#include "ManagerSystem/TextManager.h"
#include "ManagerGame/SaveManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "jni/JniHelper.h"
#endif

MafRewardAdsManager::MafRewardAdsManager()
//m_curAdsIndex(0)
{
    
}

MafRewardAdsManager::~MafRewardAdsManager()
{
//    m_adsOrder.clear();
}

bool MafRewardAdsManager::init()
{
    return true;
}

//void MafRewardAdsManager::startAds(std::function<void(ADS_RESULT)> cb)
//{
//    _ResultCBRewardAds = cb;
//    m_curAdsIndex = 0;
//
//    std::list<MafAds *>::iterator it = m_adsOrder.begin();
//    std::advance(it, m_curAdsIndex);
//
//    MafAds *next_ads = *it;
//    next_ads->showAds();
//}

//void MafRewardAdsManager::initAds()
//{
//    std::list<MafAds *>::iterator iter;
//    
//    for(iter = m_adsOrder.begin(); iter!=m_adsOrder.end(); iter++)
//    {
//        (*iter)->initAds();
//    }
//}

//MafAds* MafRewardAdsManager::getAdsClass(MafRewardAdsManager::E_ADS_TYPE type)
//{
//    switch (type) {
////        case MafRewardAdsManager::E_ADS_TYPE::E_APPLOVIN:
////            return (new MafAppLovinRewardAds);
//        case MafRewardAdsManager::E_ADS_TYPE::E_ADMOB:
//            return (new MafGoogleRewardAds);
////        case MafRewardAdsManager::E_ADS_TYPE::E_FACEBOOK:
////            return (new MafFacebookRewardAds);
//    }
//
//    return nullptr;
//}
//
//void MafRewardAdsManager::setAdsOrder(int adsType, ...)
//{
//    std::list<int>adsList;
//
//    va_list ap;
//
//    va_start(ap, adsType);
//
//    MafAds* ads = getAdsClass(static_cast<E_ADS_TYPE>(adsType));
//    if ( ads != nullptr )
//    {
//        m_adsOrder.push_back(ads);
//    }
//
//    while(true){
//        int type_num = va_arg(ap, int);
//
//        if(type_num > 0)
//        {
//            std::list<int>::iterator it = std::find(adsList.begin(), adsList.end(), type_num);
//            if(it == adsList.end())
//            {
//                MafAds* ads = getAdsClass(static_cast<E_ADS_TYPE>(type_num));
//                if(ads != NULL)
//                {
//                    m_adsOrder.push_back(ads);
//                    adsList.push_back(type_num);
//                }
//
//            }
//
//        }
//        else
//        {
//             break;
//        }
//    }
//
//    va_end(ap);
//}
//
//void MafRewardAdsManager::clearAdsOrder()
//{
//    m_adsOrder.clear();
//}

void MafRewardAdsManager::setHistoryData(const char* strAdsType, bool bAdsFree)
{
    int saveTotal = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_VIDEO_COUNT);
    SaveManager::setHistoryData(E_HISTORY_DATA::K_VIDEO_COUNT, (saveTotal+1));
    
    std::string strEventName = MafUtils::format(kAnalEventAdsWatch, saveTotal);
    switch (saveTotal) {
        case 1: case 2: case 3: case 4: case 5: case 10: case 15: case 20: case 25: case 30: case 35: case 40: case 45: case 50:
            MafAnalyticsManager::LogEvent(strEventName.c_str(), kRepeatFalse);
            break;
        default:
            break;
    }
    
    if ( strlen(strAdsType) != 0 )
    {
        if ( bAdsFree == true )
        {
            MafAnalyticsManager::LogEvent(MafUtils::format(kAnalEventAdsSlot, strAdsType).c_str(), kAnalEventAdsParameterSkip, "Y", kRepeatTrue);
        }
        else
        {
            MafAnalyticsManager::LogEvent(MafUtils::format(kAnalEventAdsSlot, strAdsType).c_str(), kAnalEventAdsParameterSkip, "N", kRepeatTrue);
        }
    }
}

#pragma mark - receive

#pragma mark-

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"{
    
}
#endif


