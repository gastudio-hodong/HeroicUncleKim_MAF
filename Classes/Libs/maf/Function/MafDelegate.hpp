//
//  MafDelegate.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/29.
//

#ifndef MafDelegate_hpp
#define MafDelegate_hpp

#pragma once
#include <vector>
#include "Libs/maf/Function/MafFunction.hpp"

// default template class
template <typename RETURN_TYPE> class MafDelegate;

// function with return value
template <typename RETURN_TYPE, typename... PARAMETER_TYPE>
class MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>
{
public:
    typedef MafFunction<RETURN_TYPE(PARAMETER_TYPE...)> Function_Type;
private:
    std::vector<Function_Type*> functions;
public:
    //
    MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>(bool CanAddDuplicateFunction = false);

    // It is a value for whether you can add the same function.
    bool CanAddDuplicateFunction;

    // Add new function
    MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>& operator += (Function_Type* _function);

    // Create the parameter as newand never delete it.
    MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>& operator -= (Function_Type* _function);

    // Invoke all functions
    RETURN_TYPE operator()(PARAMETER_TYPE... params);
    RETURN_TYPE Invoke(PARAMETER_TYPE... params);

    // Delete all functions
    void Clear();
    void Clear(void* object);

    // Create the parameter as newand never delete it.
    bool HasThisFunction(Function_Type* _function);
};

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::MafDelegate(bool CanAddDuplicateFunction)
{
    this->CanAddDuplicateFunction = CanAddDuplicateFunction;
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>& MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::operator+=(Function_Type* _function)
{
    if (!CanAddDuplicateFunction)
    {
        for (unsigned int i = 0; i < functions.size(); i++)
        {
            if (*functions[i] == *_function)
            {
                return *this;
            }
        }
    }
    functions.push_back(_function);
    return *this;
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>& MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::operator-=(Function_Type* _function)
{
    typename std::vector<Function_Type*>::iterator it = functions.begin();
    // c++ 버전에 따라 typename을 붙여주지 않으면 컴파일 에러가 발생하기도 함

    for (unsigned int i = 0; i < functions.size(); i++)
    {
        if (*functions[i] == *_function)
        {
            Function_Type* _current_function = functions[i];
            functions.erase(it);
            delete _current_function;
            _function = nullptr;
            break;
        }
        it++;
    }

    // self delete
    if(_function != nullptr)
        delete _function;
    return *this;
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline RETURN_TYPE MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::operator()(PARAMETER_TYPE ...params)
{
    RETURN_TYPE result = 0;
    for (unsigned int i = 0; i < functions.size(); i++)
        result = functions[i]->Invoke(params...);
    return result;
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline RETURN_TYPE MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::Invoke(PARAMETER_TYPE ...params)
{
    RETURN_TYPE result = 0;
    for (unsigned int i = 0; i < functions.size(); i++)
        result = functions[i]->Invoke(params...);
    return result;
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline void MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::Clear()
{
    for (unsigned int i = 0; i < functions.size(); i++)
        delete functions[i];
    functions.clear();
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline void MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::Clear(void* object)
{
    typename std::vector<Function_Type*>::iterator iter = functions.begin();
    // c++ 버전에 따라 typename을 붙여주지 않으면 컴파일 에러가 발생하기도 함
    
    for(; iter != functions.end();)
    {
        if((*iter)->getObject() == object)
        {
            delete (*iter);
            iter = functions.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline bool MafDelegate<RETURN_TYPE(PARAMETER_TYPE...)>::HasThisFunction(Function_Type* _function)
{
    for (unsigned int i = 0; i < functions.size(); i++)
    {
        if (*functions[i] == *_function)
        {
            delete _function;
            _function = nullptr;
            return true;
        }
    }

    // self delete
    if(_function != nullptr)
        delete _function;
    return false;
}

// function with non return
template <typename... PARAMETER_TYPE>
class MafDelegate<void(PARAMETER_TYPE...)>
{
public:
    typedef MafFunction<void(PARAMETER_TYPE...)> Function_Type;
private:
    std::vector<Function_Type*> functions;
public:
    //
    MafDelegate<void(PARAMETER_TYPE...)>(bool CanAddDuplicateFunction = false);

    // It is a value for whether you can add the same function.
    bool CanAddDuplicateFunction;

    // Add new function
    MafDelegate<void(PARAMETER_TYPE...)>& operator += (Function_Type* _function);

    // Create the parameter as newand never delete it.
    MafDelegate<void(PARAMETER_TYPE...)>& operator -= (Function_Type* _function);

    // Invoke all functions
    void operator()(PARAMETER_TYPE... params);
    void Invoke(PARAMETER_TYPE... params);

    // Delete all functions
    void Clear();
    void Clear(void* object);

    // Create the parameter as newand never delete it.
    bool HasThisFunction(Function_Type* _function);
};

template <typename... PARAMETER_TYPE>
inline MafDelegate<void(PARAMETER_TYPE...)>::MafDelegate(bool CanAddDuplicateFunction)
{
    this->CanAddDuplicateFunction = CanAddDuplicateFunction;
}

template <typename... PARAMETER_TYPE>
inline MafDelegate<void(PARAMETER_TYPE...)>& MafDelegate<void(PARAMETER_TYPE...)>::operator+=(Function_Type* _function)
{
    if (!CanAddDuplicateFunction)
    {
        for (unsigned int i = 0; i < functions.size(); i++)
        {
            if (*functions[i] == *_function)
            {
                return *this;
            }
        }
    }
    functions.push_back(_function);
    return *this;
}

template <typename... PARAMETER_TYPE>
inline MafDelegate<void(PARAMETER_TYPE...)>& MafDelegate<void(PARAMETER_TYPE...)>::operator-=(Function_Type* _function)
{
    typename std::vector<Function_Type*>::iterator it = functions.begin();
    // c++ 버전에 따라 typename을 붙여주지 않으면 컴파일 에러가 발생하기도 함

    for (unsigned int i = 0; i < functions.size(); i++)
    {
        if (*functions[i] == *_function)
        {
            Function_Type* _current_function = functions[i];
            functions.erase(it);
            delete _current_function;
            _function = nullptr;
            break;
        }
        it++;
    }

    // self delete
    if(_function != nullptr)
        delete _function;
    return *this;
}

template <typename... PARAMETER_TYPE>
inline void MafDelegate<void(PARAMETER_TYPE...)>::operator()(PARAMETER_TYPE ...params)
{
    for (unsigned int i = 0; i < functions.size(); i++)
        functions[i]->Invoke(params...);
}

template<typename... PARAMETER_TYPE>
inline void MafDelegate<void(PARAMETER_TYPE...)>::Invoke(PARAMETER_TYPE ...params)
{
    for (unsigned int i = 0; i < functions.size(); i++)
        functions[i]->Invoke(params...);
}


template <typename... PARAMETER_TYPE>
inline void MafDelegate<void(PARAMETER_TYPE...)>::Clear()
{
    for (unsigned int i = 0; i < functions.size(); i++)
        delete functions[i];
    functions.clear();
}

template <typename... PARAMETER_TYPE>
inline void MafDelegate<void(PARAMETER_TYPE...)>::Clear(void* object)
{
    typename std::vector<Function_Type*>::iterator iter = functions.begin();
    // c++ 버전에 따라 typename을 붙여주지 않으면 컴파일 에러가 발생하기도 함
    
    for(; iter != functions.end();)
    {
        if((*iter)->getObject() == object)
        {
            delete (*iter);
            iter = functions.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

template <typename... PARAMETER_TYPE>
inline bool MafDelegate<void(PARAMETER_TYPE...)>::HasThisFunction(Function_Type* _function)
{
    for (unsigned int i = 0; i < functions.size(); i++)
    {
        if (*functions[i] == *_function)
        {
            delete _function;
            _function = nullptr;
            return true;
        }
    }

    // self delete
    if(_function != nullptr)
        delete _function;
    return false;
}

#endif /* MafDelegate_hpp */
