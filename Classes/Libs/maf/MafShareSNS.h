//
//  MafShareSNS.h
//  HexagonSnake
//
//  Created by SanHeo on 2014. 9. 3..
//
//

#ifndef __TapRed__MafShareSNS__
#define __TapRed__MafShareSNS__

#include "cocos2d.h"

class MafShareSNS
{
public:
    
    enum SNS
    {
        FACEBOOK =0,
        TWITTER,
    };
    
    static MafShareSNS* getInstance();
    
//    std::function<void (cocos2d::RenderTexture*, const std::string&)> _callback;
    
    static void PostFacebook(const char* strFileUrl,const char* strTitle,const char* strLinkUrl);
    static void PostTwitter(const char* strFileUrl,const char* strTitle,const char* strLinkUrl);
    
    static void PostFacebookFromLocal(cocos2d::Size captureSize, const char* strTitle,const char* strLinkUrl);
    static void PostTwitterFromLocal(cocos2d::Size captureSize, const char* strTitle,const char* strLinkUrl);

    static void OpenFacebookPage(const char* page);

private:
    static void CaptureScreen(cocos2d::Size captureSize);
    static void PostSNSAtferCapture(cocos2d::RenderTexture* rt,const std::string& str);
    
    SNS _SNS;
    std::string _strTitle;
    std::string _strLinkUrl;
    

    
};

#endif /* defined(__HexagonSnake__MafShareSNS__) */
