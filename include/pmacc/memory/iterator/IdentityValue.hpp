/* Copyright 2013-2018 Sebastian Hahn
 *
 * This file is part of PMacc.
 *
 * PMacc is free software: you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License or
 * the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PMacc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License and the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and the GNU Lesser General Public License along with PMacc.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

namespace pmacc
{
/**
 * @brief The IdentityValue value is used to store a single value. This value
 * is from the type uint_fast32_t. The IdentityValue has the operator-() 
 * overloaded. This returns the value. Actually there are two implementations:
 * 1. The runtime implementation. This version has a constructor to set the 
 * value
 * 2. The compiletime implementation. The value of this version is set by 
 * templates. This is the reason why uint_fast32_t is used, since a template 
 * type as template isn't allowed.
 */
template<
    typename T,
    uint_fast32_t value = static_cast<uint_fast32_t>(0xffffffff)
>
struct IdentityValue;


template<
    typename T,
    uint_fast32_t value
>
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
struct IdentityValue<
    T,
    static_cast<uint_fast32_t>(0xffffffff)
>
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
    IdentityValue&
    operator=( IdentityValue const &) = default;
    
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
