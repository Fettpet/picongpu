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
    typedef char one;
    typedef long two;

    template <typename C> static one test( decltype(&C::UNDEFINED) ) ;
    template <typename C> static two test(...);    

public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
}; // class UndefinedAhead

struct UndefinedType;
} // namespace details
     
     

/**
 * \struct Accessor
 * 
 * @brief The Accesor is used to describe the current position of the iterator. 
 * The Accessor use several functions: 
 * 1. TComponent& get(TContaier*, TIndex&): returns the current value at the 
 * iterator position
 * 2. bool equal(TContainer*, TIndex&, TContainer*, TIndex&): returns true,
 * if the iterator is at the same position
 * 3. bool ahead(TContainer*, TIndex&, TContainer*, TIndex&): returns true,
 * if the first iterator is ahead the second one. 
 * 4. bool behind(TContainer* , TIndex&, TContainer*, TIndex&): returns true,
 * if the first iterator is behind the second one.
 * The functions ahead and behind are only avail, if the iterator is random 
 * accessable. 
 * To use the default Accessor you need to spezify one trait for each function:
 * 1. get: pmacc::traits::accessor::Get<TContainer, TComponent, TIndex>
 * 2. ahead: pmacc::traits::accessor::Ahead<TContainer, TComponent, TIndex>
 * 3. equal: pmacc::traits::accessor::Equal<TContainer, TComponent, TIndex>
 * 4. behind: pmacc::traits::accessor::Behind<TContainer, TComponent, TIndex>
 * @tparam TContainer The container over which you like to iterate. 
 * @tparam TComponent The type of the container component. 
 * @tparam TIndex Type of the index to get access to the value of the iterator 
 * position.
 * @tparam TContainerCategory Type of default access parameter
 * @tparam TGet Trait to define, how to get the first element 
 * @tparam TEqual Trait to define, when two iterators are at the same position
 * @tparam TAhead Trait to define, when the first iterator is ahead the second 
 * one. Only needed if the iterator is random accessable.
 * @tparam TBehind Trait to define, when the first iterator is behind the second
 * one. Only needed if the iteartor is random accessable.
 * @tparam isRandomAccessable true, if the container is random accessable, false
 * otherwise
 */
template<
    typename TContainer,
    typename TComponent,
    typename TIndex,
    typename TContainerCategory,
    typename TGet = pmacc::traits::accessor::Get<TContainer, TComponent, TIndex, TContainerCategory>,
    typename TAhead = pmacc::traits::accessor::Ahead<TContainer, TComponent, TIndex, TContainerCategory>,
    typename TEqual = pmacc::traits::accessor::Equal<TContainer, TComponent, TIndex, TContainerCategory>,
    typename TBehind = pmacc::traits::accessor::Behind<TContainer, TComponent, TIndex, TContainerCategory>,
    bool isRandomAccessable = not details::UndefinedAhead<TAhead>::value >
struct Accessor
{
    typedef TContainer                                              ContainerType;
    typedef ContainerType*                                          ContainerPtr;
    typedef TComponent                                              ComponentType;
    typedef ComponentType*                                          ComponentPtr;
    typedef ComponentType&                                          ComponentRef;
    typedef TContainerCategory                                      ContainerCategory;
    typedef TIndex                                                  IndexType;
    
    HDINLINE Accessor() = default;
    HDINLINE Accessor(Accessor const &) = default;
    HDINLINE Accessor(Accessor &&) = default;
    
    /**
     * @brief this function dereference a container and a index to get the 
     * component
     * @param containerPtr pointer to a container
     * @param idx current position of the iterator
     * @return component at the idx position
     */
    HDINLINE 
    ComponentRef
    get(ContainerPtr containerPtr,
        IndexType & idx)
    {
        assert(containerPtr != nullptr);
        return _get(containerPtr, idx);
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
    bool
    equal(ContainerPtr const containerPtr1,
          IndexType const & index1,
          ContainerPtr const containerPtr2,
          IndexType const & index2)
    {
        assert(containerPtr1 != nullptr);
        assert(containerPtr2 != nullptr);
        return _equal(containerPtr1, index1, containerPtr2, index2);
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
    typename std::enable_if<T == true, bool>::type
    greater(ContainerPtr const containerPtr1,
          IndexType const & index1,
          ContainerPtr const containerPtr2,
          IndexType const & index2)
    {
        assert(containerPtr1 != nullptr);
        assert(containerPtr2 != nullptr);
        return _ahead(containerPtr1, index1, containerPtr2, index2);
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
    typename std::enable_if<T == true, bool>::type
    lesser(ContainerPtr const containerPtr1,
          IndexType const & index1,
          ContainerPtr const containerPtr2,
          IndexType const & index2)
    {
        assert(containerPtr1 != nullptr);
        assert(containerPtr2 != nullptr);
        return _behind(containerPtr1, index1, containerPtr2, index2);
    }
    
    
    
     TGet _get;
     TAhead _ahead;
     TEqual _equal;
     TBehind _behind;
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
    pmacc::details::UndefinedType>
{
    typedef details::UndefinedType ContainerType;
    
    HDINLINE Accessor() = default;
    HDINLINE Accessor(Accessor const &) = default;
    HDINLINE Accessor(Accessor &&) = default;
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
 * @param TAccessor The accessor prescription. It is only virtual and not needed
 * @tparam TContainer. Type of the container.
 * @return An accessor with the functionallity given by the pmacc::traits.
 */
template<
    typename TContainer,
    typename TContainerNoRef = typename std::decay<TContainer>::type,
    typename TIndex = typename pmacc::traits::IndexType<TContainerNoRef>::type,
    typename TContainerCategory = typename pmacc::traits::ContainerCategory<typename std::decay<TContainer>::type>::type,
    typename TComponent = typename pmacc::traits::ComponentType<TContainerNoRef>::type,
    typename TGet = pmacc::traits::accessor::Get<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TAhead = pmacc::traits::accessor::Ahead<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TEqual = pmacc::traits::accessor::Equal<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TBehind = pmacc::traits::accessor::Behind<TContainerNoRef, TComponent, TIndex, TContainerCategory>, 
    typename TAccessor>
HDINLINE
auto 
makeAccessor(TAccessor&&)
-> pmacc::Accessor<
    TContainerNoRef,
    TComponent,
    TIndex,
    TContainerCategory,
    TGet,
    TAhead,
    TEqual,
    TBehind>
{
    typedef pmacc::Accessor<
        TContainerNoRef,
        TComponent,
        TIndex,
        TContainerCategory> AccessorType;
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
 * @tparam TContainer. Type of the container.
 * @return An accessor with the functionallity given by the pmacc::traits.
 */
template<
    typename TContainer,
    typename TContainerNoRef = typename std::decay<TContainer>::type,
    typename TIndex = typename pmacc::traits::IndexType<TContainerNoRef>::type,
    typename TContainerCategory = typename pmacc::traits::ContainerCategory<typename std::decay<TContainer>::type>::type,
    typename TComponent = typename pmacc::traits::ComponentType<TContainerNoRef>::type,
    typename TGet = pmacc::traits::accessor::Get<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TAhead = pmacc::traits::accessor::Ahead<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TEqual = pmacc::traits::accessor::Equal<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TBehind = pmacc::traits::accessor::Behind<TContainerNoRef, TComponent, TIndex, TContainerCategory> >
auto 
HDINLINE
makeAccessor()
-> pmacc::Accessor<
    TContainerNoRef,
    TComponent,
    TIndex,
    TContainerCategory,
    TGet,
    TAhead,
    TEqual,
    TBehind>
{
    typedef pmacc::Accessor<
        TContainerNoRef,
        TComponent,
        TIndex,
        TContainerCategory,
        TGet,
        TAhead,
        TEqual,
        TBehind>                                          ResultType;
        
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
 * @param TContainer. The container
 * @return An accessor with the functionallity given by the pmacc::traits.
 */
template<
    typename TContainer,
    typename TContainerNoRef = typename std::decay<TContainer>::type,
    typename TIndex = typename pmacc::traits::IndexType<TContainerNoRef>::type,
    typename TContainerCategory = typename pmacc::traits::ContainerCategory<typename std::decay<TContainer>::type>::type,
    typename TComponent = typename pmacc::traits::ComponentType<TContainerNoRef>::type,
    typename TGet = pmacc::traits::accessor::Get<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TAhead = pmacc::traits::accessor::Ahead<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TEqual = pmacc::traits::accessor::Equal<TContainerNoRef, TComponent, TIndex, TContainerCategory>,
    typename TBehind = pmacc::traits::accessor::Behind<TContainerNoRef, TComponent, TIndex, TContainerCategory> >
    
auto 
HDINLINE
makeAccessor(TContainer&&)
-> pmacc::Accessor<
    TContainerNoRef,
    TComponent,
    TIndex,
    TContainerCategory,
    TGet,
    TAhead,
    TEqual,
    TBehind>
{
    typedef pmacc::Accessor<
        TContainerNoRef,
        TComponent,
        TIndex,
        TContainerCategory,
        TGet,
        TAhead,
        TEqual,
        TBehind>                                         ResultType;
        
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
    typedef pmacc::Accessor<
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType>                           ResultType;
    return ResultType();
}

/**
 * @brief This function forwards an own accessor. This is a identity function
 * @param Accessor The accessor you like to forward
 * @return the accessor given by as parameter.
 */
template<
    typename TAccessor>
HDINLINE
auto
makeAccessor(TAccessor && accessor)
->
decltype(pmacc::forward<TAccessor>(accessor))
{
    return pmacc::forward<TAccessor>(accessor);
}

}// namespace pmacc
