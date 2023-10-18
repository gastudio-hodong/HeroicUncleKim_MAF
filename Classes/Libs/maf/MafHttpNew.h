//
//  MafHttpManager.h
//  FantasyClicker
//
//  Created by SanHeo on 2015. 6. 21..
//
//

#ifndef __FantasyClicker__MafHttpNewManager__
#define __FantasyClicker__MafHttpNewManager__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include "network/HttpClient.h"
#include "network/HttpRequest.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>

typedef std::function<void(void* http, cocos2d::network::HttpResponse* response,const char *data)> MafResponseNew;

class MafHttpModelNew
{
public:

    enum DATA_TYPE{
        TYPE_CHAR=1,
        TYPE_INT,
        TYPE_DOUBLE,
        TYPE_ARRAY_INT,
        TYPE_ARRAY_DOUBLE,
        TYPE_ARRAY_STR,
        Type_OBJECT,
        Type_OBJECT_FOR_ARRAY,
        Type_ARRAY_OBJECT
    };
    
    MafHttpModelNew();
    

    DATA_TYPE _type;
    std::string _key;
    std::string _value;
    int             _nValue;
    double         _nValueDouble;
    std::vector<int> _listValueInt;
    std::vector<double> _listValueDouble;
    std::vector<std::string> _listValueStr;
    std::vector<MafHttpModelNew*> _ObjValue;
    std::vector<MafHttpModelNew*> _listObjValue;
    
    void setData(const char *key,const char *value);
    void setData(const char *key,int value);
    void setDataDouble(const char *key,double value);
    void setData(const char *key,std::vector<int> value);
    void setData(const char *key,std::vector<double> value);
    void setData(const char *key,std::vector<std::string> value);
    void setData(const char *key, std::vector<MafHttpModelNew*> value, bool isArray);
    void setDataObjArray(const char *key, std::vector<MafHttpModelNew*> value);
    
    void converJsonPost(rapidjson::Writer<rapidjson::StringBuffer> &wr);
    void converGet(std::string &url);
};
 
class MafHttpNew : public cocos2d::Ref
{
public:
    MafHttpNew(cocos2d::network::HttpRequest::Type requestType, std::string url);
    virtual ~MafHttpNew();
    
public:
    void setData(const char *key, const char* value);
    void setData(const char *key, std::string value);
    void setData(const char *key, int value);
    void setDataDouble(const char *key, double value);
    void setData(const char *key, std::vector<int> value);
    void setData(const char *key, std::vector<double> value);
    void setData(const char *key, std::vector<std::string> value);
    void setData(const char *key, std::vector<MafHttpModelNew*> value, bool isArray);
    void setDataObjecArray(const char *key, std::vector<MafHttpModelNew*> value);
    
    //
    void send(MafResponseNew cb);
    void response(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

public:
    // root url
    static std::string _strRootUrl;
    static cocos2d::Vector<cocos2d::network::HttpRequest*> _vecRequest;
    
private:
    cocos2d::network::HttpRequest *_request;
    
    // function
    MafResponseNew _cbFunction;
    
    // data
    std::string _strUrl;
    std::string _strParameter;
    std::vector<MafHttpModelNew*> _vecData;
};


#endif /* defined(__FantasyClicker__MafHttpManager__) */
