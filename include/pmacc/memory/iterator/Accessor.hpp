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

#include <iostream>

#include "pmacc/traits/iterator/Componenttype.hpp"
#include "pmacc/types.hpp"

#include "pmacc/traits/iterator/NumberElements.hpp"
#include "pmacc/traits/iterator/IndexType.hpp"
#include "pmacc/traits/iterator/accessor/Ahead.hpp"
#include "pmacc/traits/iterator/accessor/Behind.hpp"
#include "pmacc/traits/iterator/accessor/Equal.hpp"
#include "pmacc/traits/iterator/accessor/Get.hpp"

#include "pmacc/forward.hpp"

namespace pmacc
{
namespace details
{
template <typename T>
class UndefinedAhead
{
    using one = char;
    using two = long ;

    template <typename C>
    static
    one
    test( decltype(&C::UNDEFINED) );

    template <typename C>
    static
    two
    test(...);

public:
    enum 
    { 
        value = sizeof(test<T>(0)) == sizeof(char) };
}; // class UndefinedAhead

struct UndefinedType;
} // namespace details
     
     

/**
 * \struct Accessor
 * 
 * @brief The Accesor is used to describe the current position of the iterator. 
 * The Accessor use several functions: 
 * 1. T_Component& get(TContaier*, T_Index&): returns the current value at the 
 * iterator position
 * 2. bool equal(T_Container*, T_Index&, T_Container*, T_Index&): returns true,
 * if the iterator is at the same position
 * 3. bool ahead(T_Container*, T_Index&, T_Container*, T_Index&): returns true,
 * if the first iterator is ahead the second one. 
 * 4. bool behind(T_Container* , T_Index&, T_Container*, T_Index&): returns true,
 * if the first iterator is behind the second one.
 * The functions ahead and behind are only avail, if the iterator is random 
 * accessable. 
 * To use the default Accessor you need to spezify one trait for each function:
 * 1. get: pmacc::traits::accessor::Get<T_Container, T_Component, T_Index>
 * 2. ahead: pmacc::traits::accessor::Ahead<T_Container, T_Component, T_Index>
 * 3. equal: pmacc::traits::accessor::Equal<T_Container, T_Component, T_Index>
 * 4. behind: pmacc::traits::accessor::Behind<T_Container, T_Component, T_Index>
 * @tparam T_Container The container over which you like to iterate. 
 * @tparam T_Component The type of the container component. 
 * @tparam T_Index Type of the index to get access to the value of the iterator 
 * position.
 * @tparam T_ContainerCategory Type of default access parameter
 * @tparam T_Get Trait to define, how to get the first element 
 * @tparam T_Equal Trait to define, when two iterators are at the same position
 * @tparam T_Ahead Trait to define, when the first iterator is ahead the second 
 * one. Only needed if the iterator is random accessable.
 * @tparam T_Behind Trait to define, when the first iterator is behind the second
 * one. Only needed if the iteartor is random accessable.
 * @tparam isRandomAccessable true, if the container is random accessable, false
 * otherwise
 */
template<
    typename T_Container,
    typename T_Component,
    typename T_Index,
    typename T_ContainerCategory,
    typename T_Get = pmacc::traits::accessor::Get<
        T_Container, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Ahead = pmacc::traits::accessor::Ahead<
        T_Container, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Equal = pmacc::traits::accessor::Equal<
        T_Container, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Behind = pmacc::traits::accessor::Behind<
        T_Container, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    bool isRandomAccessable = not details::UndefinedAhead<T_Ahead>::value
>
struct Accessor
{
    using ContainerType = T_Container;
    using ContainerPtr = ContainerType*;
    using ComponentType = T_Component;
    using ComponentPtr = ComponentType*;
    using ComponentRef = ComponentType&;
    using ContainerCategory = T_ContainerCategory;
    using IndexType = T_Index;
    
    HDINLINE Accessor() = default;
    
    HDINLINE Accessor(
        Accessor const &
    ) = default;
    
    HDINLINE Accessor(
        Accessor &&
    ) = default;
    
    /**
     * @brief this function dereference a container and a index to get the 
     * component
     * @param containerPtr pointer to a container
     * @param idx current position of the iterator
     * @return component at the idx position
     */
    HDINLINE 
    auto
    get(
        ContainerPtr containerPtr,
        IndexType & idx
    )
    -> ComponentRef
    {
        assert(containerPtr != nullptr);
        
        return _get(
            containerPtr, 
            idx
        );
    }
    
    
    /**
     * @brief this function compares two iterator positions. 
     * @param containerPtr1 pointer to the container of the first iterator
     * @param idx1 current position of the first iterator 
     * @param containerPtr2 pointer to the container of the second iterator
     * @param idx2 current position of the second iterator
     * @return true if both iterators are at the same position, false otherwise
     */
    HDINLINE
    auto
    equal(
        ContainerPtr const containerPtr1,
        IndexType const & index1,
        ContainerPtr const containerPtr2,
        IndexType const & index2
    )
    -> bool
    {
        assert(containerPtr1 != nullptr);
        assert(containerPtr2 != nullptr);
        
        return _equal(
            containerPtr1, 
            index1, 
            containerPtr2, 
            index2
        );
    }
    
    /**
     * @brief this function compares two iterator positions. 
     * @param containerPtr1 pointer to the container of the first iterator
     * @param idx1 current position of the first iterator 
     * @param containerPtr2 pointer to the container of the second iterator
     * @param idx2 current position of the second iterator
     * @return true if both iterators are on the same container and the first 
     * index is ahead the second one.
     */
    template<bool T = isRandomAccessable>
    HDINLINE 
    auto
    greater(ContainerPtr const containerPtr1,
          IndexType const & index1,
          ContainerPtr const containerPtr2,
          IndexType const & index2)
    -> typename std::enable_if<
        T == true,
        bool
    >::type
    {
        assert(containerPtr1 != nullptr);
        assert(containerPtr2 != nullptr);
        
        return _ahead(
            containerPtr1, 
            index1, 
            containerPtr2, 
            index2
        );
    }
    
    /**
     * @brief this function compares two iterator positions. 
     * @param containerPtr1 pointer to the container of the first iterator
     * @param idx1 current position of the first iterator 
     * @param containerPtr2 pointer to the container of the second iterator
     * @param idx2 current position of the second iterator
     * @return true if both iterators are on the same container and the first 
     * index is behind the second one.
     */
    template<bool T = isRandomAccessable>
    HDINLINE 
    auto
    lesser(ContainerPtr const containerPtr1,
          IndexType const & index1,
          ContainerPtr const containerPtr2,
          IndexType const & index2)
    -> typename std::enable_if<
        T == true,
        bool
    >::type
    {
        assert(containerPtr1 != nullptr);
        assert(containerPtr2 != nullptr);
        
        return _behind(
            containerPtr1, 
            index1, 
            containerPtr2,
            index2
        );
    }
    
    T_Get _get;
    T_Ahead _ahead;
    T_Equal _equal;
    T_Behind _behind;
} ;

/**
 * @brief the accessor prescription.
 */
template<>
struct Accessor<
    details::UndefinedType, 
    details::UndefinedType,
    details::UndefinedType,
    details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType
>
{
    
    HDINLINE Accessor() = default;
    HDINLINE Accessor( Accessor const & ) = default;
    HDINLINE Accessor( Accessor && ) = default;
} ;

namespace details 
{



/**
 * @brief This function use an accessor prescription and container template to
 * create a accessor. To use this function several pmacc::traits must be defined.
 * 1. IndexType
 * 2. ContainerCategory
 * 3. ComponentType
 * 4. Get
 * 5. Ahead
 * 6. Equal
 * 7. Behind
 * @param T_Accessor The accessor prescription. It is only virtual and not needed
 * @tparam T_Container. Type of the container.
 * @return An accessor with the functionallity given by the pmacc::traits.
 */
template<
    typename T_Container,
    typename T_ContainerNoRef = typename std::decay<T_Container>::type,
    typename T_Index = typename pmacc::traits::IndexType<T_ContainerNoRef>::type,
    typename T_ContainerCategory =
        typename pmacc::traits::ContainerCategory<
            typename std::decay<T_Container>::type
        >::type,
    typename T_Component =
        typename pmacc::traits::ComponentType<T_ContainerNoRef>::type,
    typename T_Get = pmacc::traits::accessor::Get<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Ahead = pmacc::traits::accessor::Ahead<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Equal = pmacc::traits::accessor::Equal<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Behind = pmacc::traits::accessor::Behind<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >, 
    typename T_Accessor
>
HDINLINE
auto 
makeAccessor(
    T_Accessor&&
)
-> pmacc::Accessor<
    T_ContainerNoRef,
    T_Component,
    T_Index,
    T_ContainerCategory,
    T_Get,
    T_Ahead,
    T_Equal,
    T_Behind
>
{
    using AccessorType = pmacc::Accessor<
        T_ContainerNoRef,
        T_Component,
        T_Index,
        T_ContainerCategory
    > ;
    auto && accessor = AccessorType();
    return accessor;
}

/**
 * @brief This function use a container template to
 * create a accessor. To use this function several pmacc::traits must be defined.
 * 1. IndexType
 * 2. ContainerCategory
 * 3. ComponentType
 * 4. Get
 * 5. Ahead
 * 6. Equal
 * 7. Behind
 * @tparam T_Container. Type of the container.
 * @return An accessor with the functionallity given by the pmacc::traits.
 */
template<
    typename T_Container,
    typename T_ContainerNoRef = typename std::decay<T_Container>::type,
    typename T_Index = typename pmacc::traits::IndexType<T_ContainerNoRef>::type,
    typename T_ContainerCategory = typename pmacc::traits::ContainerCategory<typename std::decay<T_Container>::type>::type,
    typename T_Component = typename pmacc::traits::ComponentType<T_ContainerNoRef>::type,
    typename T_Get = pmacc::traits::accessor::Get<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Ahead = pmacc::traits::accessor::Ahead<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Equal = pmacc::traits::accessor::Equal<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Behind = pmacc::traits::accessor::Behind<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >
>
auto 
HDINLINE
makeAccessor()
-> pmacc::Accessor<
    T_ContainerNoRef,
    T_Component,
    T_Index,
    T_ContainerCategory,
    T_Get,
    T_Ahead,
    T_Equal,
    T_Behind>
{
    using ResultType =  pmacc::Accessor<
        T_ContainerNoRef,
        T_Component,
        T_Index,
        T_ContainerCategory,
        T_Get,
        T_Ahead,
        T_Equal,
        T_Behind
    >                                          ;
        
    return ResultType();
}

} // namespace details


/**
 * @brief This function use a container template to
 * create a accessor. To use this function several pmacc::traits must be defined.
 * 1. IndexType
 * 2. ContainerCategory
 * 3. ComponentType
 * 4. Get
 * 5. Ahead
 * 6. Equal
 * 7. Behind
 * @param T_Container. The container
 * @return An accessor with the functionallity given by the pmacc::traits.
 */
template<
    typename T_Container,
    typename T_ContainerNoRef = typename std::decay<T_Container>::type,
    typename T_Index = typename pmacc::traits::IndexType<T_ContainerNoRef>::type,
    typename T_ContainerCategory = typename pmacc::traits::ContainerCategory<typename std::decay<T_Container>::type>::type,
    typename T_Component = typename pmacc::traits::ComponentType<T_ContainerNoRef>::type,
    typename T_Get = pmacc::traits::accessor::Get<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Ahead = pmacc::traits::accessor::Ahead<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Equal = pmacc::traits::accessor::Equal<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >,
    typename T_Behind = pmacc::traits::accessor::Behind<
        T_ContainerNoRef, 
        T_Component, 
        T_Index, 
        T_ContainerCategory
    >
>
auto 
HDINLINE
makeAccessor(
    T_Container&&
)
-> pmacc::Accessor<
    T_ContainerNoRef,
    T_Component,
    T_Index,
    T_ContainerCategory,
    T_Get,
    T_Ahead,
    T_Equal,
    T_Behind>
{
    using ResultType = pmacc::Accessor<
        T_ContainerNoRef,
        T_Component,
        T_Index,
        T_ContainerCategory,
        T_Get,
        T_Ahead,
        T_Equal,
        T_Behind
    >;
        
    return ResultType();
}






/**
 * @brief creates an accessor prescription.
 */
auto 
HDINLINE
makeAccessor()
-> pmacc::Accessor<
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType>
{
    using ResultType = pmacc::Accessor<
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType
    >;
    return ResultType();
}

/**
 * @brief This function forwards an own accessor. This is a identity function
 * @param Accessor The accessor you like to forward
 * @return the accessor given by as parameter.
 */
template<
    typename T_Accessor>
HDINLINE
auto
makeAccessor(
    T_Accessor && accessor
)
->
decltype(
    pmacc::forward<T_Accessor>(accessor)
)
{
    return pmacc::forward<T_Accessor>(accessor);
}

}// namespace pmacc
