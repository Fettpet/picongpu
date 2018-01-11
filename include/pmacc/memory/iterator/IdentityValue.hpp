#pragma once

namespace pmacc
{
template<typename T, uint_fast32_t value = static_cast<uint_fast32_t>(0xffffffff)>
struct IdentityValue;


template<typename T, uint_fast32_t value>
struct IdentityValue
{
    HDINLINE
    IdentityValue() = default;
    
    HDINLINE
    IdentityValue(IdentityValue const &) = default;
    
    HDINLINE
    IdentityValue(IdentityValue&&) = default;
    
    
    HDINLINE
    IdentityValue& operator=( IdentityValue const &) = default;
    
    HDINLINE
    T 
    operator() ()
    const 
    {
        return value;
    }
};


template<typename T>
struct IdentityValue<T, static_cast<uint_fast32_t>(0xffffffff)>
{
    
    HDINLINE
    IdentityValue(T const & value):
        value(value)
        {}
    
    HDINLINE
    IdentityValue(IdentityValue const &) = default;
    
    HDINLINE
    IdentityValue(IdentityValue&&) = default;
    
    HDINLINE
    IdentityValue() = delete;
    
    HDINLINE
    IdentityValue& operator=( IdentityValue const &) = default;
    
    HDINLINE
    T 
    operator() ()
    const 
    {
        return value;
    }
protected:
    T value;
};

}// namespace pmacc
