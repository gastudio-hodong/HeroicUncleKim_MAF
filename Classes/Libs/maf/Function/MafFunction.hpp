//
//  MafFunction.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/29.
//

#ifndef MafFunction_hpp
#define MafFunction_hpp

#pragma once
#include <functional>
using namespace std::placeholders;

// default template class
template <typename RETURN_TYPE> class MafFunction;

// function with return value
template <typename RETURN_TYPE, typename... PARAMETER_TYPE>
class MafFunction<RETURN_TYPE(PARAMETER_TYPE...)>
{
public:
    typedef std::function<RETURN_TYPE(PARAMETER_TYPE...)> functional_type;
    typedef void* object;
private:
    typedef MafFunction<RETURN_TYPE(PARAMETER_TYPE...)> my_type;
private:
    functional_type _function;
    object _object;
public:
    // Example.
    // int ExmapleFunction(int int_01, int int_02) To bind a function :
    // bind(&ClassName::ExampleFunction, &test, std::placeholder::_ 1, std::placeholder::_ 2);
    MafFunction(functional_type _bind_function, object _object);
    virtual ~MafFunction();
    // Invoke all functions
    RETURN_TYPE Invoke(PARAMETER_TYPE... params);
    object getObject(){return _object;};

    // Invoke all functions
    RETURN_TYPE operator()(PARAMETER_TYPE... params);

    // Compare two functions
    bool operator == (const my_type& other);
};

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline MafFunction<RETURN_TYPE(PARAMETER_TYPE...)>::MafFunction(functional_type _bind_function, object _object)
{
    this->_function = _bind_function;
    this->_object = _object;
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline MafFunction<RETURN_TYPE(PARAMETER_TYPE...)>::~MafFunction()
{
    this->_function = nullptr;
    this->_object = nullptr;
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline RETURN_TYPE MafFunction<RETURN_TYPE(PARAMETER_TYPE...)>::Invoke(PARAMETER_TYPE ...params)
{
    return _function(params...);
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline RETURN_TYPE MafFunction<RETURN_TYPE(PARAMETER_TYPE...)>::operator()(PARAMETER_TYPE ...params)
{
    return _function(params...);
}

template<typename RETURN_TYPE, typename ...PARAMETER_TYPE>
inline bool MafFunction<RETURN_TYPE(PARAMETER_TYPE...)>::operator==(const my_type& other)
{
    return (_function.target_type().name() == other._function.target_type().name()) && (_object == other._object);
}

// function with non return
template <typename... PARAMETER_TYPE>
class MafFunction<void(PARAMETER_TYPE...)>
{
public:
    typedef std::function<void(PARAMETER_TYPE...)> functional_type;
    typedef void* object;
private:
    typedef MafFunction<void(PARAMETER_TYPE...)> my_type;
private:
    functional_type _function;
    object _object;
public:
    MafFunction(functional_type _bind_function, object _object);
    virtual ~MafFunction();
    void Invoke(PARAMETER_TYPE... params);
    object getObject(){return _object;};

    void operator()(PARAMETER_TYPE... params);

    bool operator == (const my_type& other);
};

template<typename ...PARAMETER_TYPE>
inline MafFunction<void(PARAMETER_TYPE...)>::MafFunction(functional_type _bind_function, object _object)
{
    this->_function = _bind_function;
    this->_object = _object;
}
template<typename ...PARAMETER_TYPE>
inline MafFunction<void(PARAMETER_TYPE...)>::~MafFunction()
{
    this->_function = nullptr;
    this->_object = nullptr;
};
template<typename ...PARAMETER_TYPE>
inline void MafFunction<void(PARAMETER_TYPE...)>::Invoke(PARAMETER_TYPE ...params)
{
    _function(params...);
}

template<typename ...PARAMETER_TYPE>
inline void MafFunction<void(PARAMETER_TYPE...)>::operator()(PARAMETER_TYPE ...params)
{
    _function(params...);
}

template<typename ...PARAMETER_TYPE>
inline bool MafFunction<void(PARAMETER_TYPE...)>::operator==(const my_type& other)
{
    return (_function.target_type().name() == other._function.target_type().name()) && (_object == other._object);
}

#endif /* MafFunction_hpp */
