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
#include "pmacc/forward.hpp" 
#include "pmacc/memory/iterator/NoChild.hpp"
#include <boost/core/ignore_unused.hpp>
#include <type_traits>
#include "pmacc/types.hpp"
#include "pmacc/traits/iterator/Componenttype.hpp"
#include "pmacc/traits/iterator/ContainerCategory.hpp"
#include "pmacc/traits/iterator/RangeType.hpp"
#include "pmacc/traits/iterator/navigator/AfterLastElement.hpp"
#include "pmacc/traits/iterator/navigator/BeforeFirstElement.hpp"
#include "pmacc/traits/iterator/navigator/LastElement.hpp"
#include "pmacc/traits/iterator/navigator/NextElement.hpp"
#include "pmacc/traits/iterator/navigator/PreviousElement.hpp"
#include "pmacc/traits/iterator/navigator/FirstElement.hpp"
#include "pmacc/memory/iterator/Categorie.hpp"
#include <cassert>

namespace pmacc 
{
namespace details
{
struct UndefinedType;

template <typename T>
class UndefinedLastElement
{
    using one = char;
    using two = long ;

    template <typename C>
    static
    one
    test( decltype(&C::UNDEFINED) ) ;

    template <typename C>
    static
    two
    test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
}; // class UndefinedAhead

template<typename T>    
struct OffsetRangeType
{
    T v;
    using type = decltype(v());
};

} // namespace details


/**
* \struct Navigator
@brief This is the default implementation of the navigator. The task of the 
navigator is to define the first element, the next element and an after last 
element. If the navigator is bidirectional it need also a last element, a 
previous element and a before first element. 

The navigator has two traits for parallel 
walking through the container. The first one is TOffset. This is used to get the 
distance from the first element of the container to the first element which will
be accessed. This trait can be used to map the thread ID (for example offset 
= threadIdx.x). The second trait is the jumpsize. The jumpsize is the distance
between two iterator elements. The number of threads can be mapped on the jump-
size. With this two traits you can go parallel over all elements and touch each
element only one times. 
 
We had three/six traits for the behaviour of the container. The first three
traits are :
1. define the first element of the container,
2. define a next element of the container,
3. define a after last element of the container.
If the navigator is bidirectional three additional traits are needed
4. define the last element within the container
5. define a previous element of the container
6. define a before first element of the container.
The navigator use this 8 traits to define methodes for parallel iteration though
the container.
 
@tparam TContainer Type of the container,
@tparam TComponent Type of the component of the container.
@tparam TOffset Policy to get the offset. You need to specify the () operator.
@tparam TJumpsize Policy to specify the Jumpsize. It need the operator ().
@tparam TIndex Type of the index. The index is used to specify the iterator 
position.
@tparam TContainerSize Trait to specify the size of a container. It need the 
function operator()(TContainer*). TContainer is a pointer to the container 
instance over which the iterator walks.
@tparam TFirstElement Trait to set the index to the first element. It need the 
function operator()(TContainer*, TIndex&, const TRange). TRange is the result 
type of TOffset's (). TContainer is a pointer to the container 
instance over which the iterator walks. TIndex is used to describe the position.
TRange is the offset.
@tparam TNextElement Trait to set the index to the next element. The trait need 
the function TRange operator()(TContainer*, TIndex&, TRange). The TRange 
parameter is used to handle the jumpsize. The result of this function is the 
remaining jumpsize. A little example. Your container has 10 elements and your
iterator is the the 8 element. Your jumpsize is 5. This means the new position
would be 13. So the result of the function is 3, the remaining jumpsize.
@tparam TAfterLastElement This Trait is used to check whether the iteration is 
after the last element. The function header is 
bool operator()(TContainer*, TIndex&). It returns true, if the end is reached, 
and false otherwise.
@tparam TLastElement This trait gives the last element which the iterator would
access, befor the end is reached, in a forward iteration case. The function 
head is operator()(TContainer*, TIndex&, const TRange). This trait is only 
needed if the navigator is bidirectional. 
@tparam TPreviousElement Trait to set the index to the previous element. The 
trait need the function TRange operator()(TContainer*, TIndex&, TRange). This 
trait is only needed if the navigator is bidirectional. For fourther 
informations see TNextElement.
@tparam TBeforeFirstElement Used to check whether the iterator is before the
first element. The function header is bool operator()(TContainer*, TIndex&). 
It returns true, if the end is reached, and false otherwise.
@tparam isBidirectional Set the navigator to bidirectional (true) or to forward
only (false)
*/
template<
    typename TContainer,
    typename TComponent,
    typename TOffset,
    typename TJumpsize,
    typename TIndex,
    typename TContainerSize,
    typename TRange,
    typename TFirstElement,
    typename TNextElement,
    typename TAfterLastElement,
    typename TLastElement = pmacc::details::UndefinedType,
    typename TPreviousElement = pmacc::details::UndefinedType,
    typename TBeforeFirstElement = pmacc::details::UndefinedType,
    bool isBidirectional = ! details::UndefinedLastElement<TLastElement>::value
>
struct Navigator
{
// define the types 
    using ContainerType = typename std::decay<TContainer>::type;
    using ContainerPtr = ContainerType*;
    using ContainerRef = ContainerType&;
    using ComponentType = TComponent;
    using ComponentPtr = ComponentType*;
    using JumpsizeType = TJumpsize;
    using OffsetType = TOffset;
    using IndexType = TIndex;
    using RangeType = TRange;
    using NumberElements = TContainerSize;
    using FirstElement = TFirstElement;
    using NextElement = TNextElement;
    using AfterLastElement = TAfterLastElement;
    using LastElement = TLastElement;
    using PreviousElement = TPreviousElement;
    using BeforeFirstElement = TBeforeFirstElement;

    
public:
// the default constructors
    HDINLINE Navigator() = default;
    HDINLINE Navigator(Navigator const &) = default;
    HDINLINE Navigator(Navigator &&) = default;
    HDINLINE ~Navigator() = default;
    HDINLINE Navigator& operator=(const Navigator&) = default;
    HDINLINE Navigator& operator=(Navigator&&) = default;

    /**
     * @brief Set the offset and the jumpsize to the given values
       @param offset the distance from the start to the first element
       @param jumpsize distance between two elements
    */
    HDINLINE
    Navigator(
        OffsetType && offset,
        JumpsizeType && jumpsize
    ):
        offset(pmacc::iterator::forward<OffsetType>(offset)),
        jumpsize(pmacc::iterator::forward<JumpsizeType>(jumpsize)),
        containerSize(),
        firstElement(),
        nextElement(),
        afterLastElement(),
        lastElement(),
        previousElement(),
        beforeFirstElement()
    {}
    
    
    /**
     * @brief The function moves the iterator forward to the next element. 
     * @param index in: current position of iterator; out: position of the 
     * iterator after the move.
     * @param containerPtr pointer to the container, over which we iterate
     * @param distance number of elements you like to overjump.
     * @result the distance from the end element to the hypothetical position
     * given by the distance parameter
     */
    HDINLINE
    auto
    next(
        ContainerPtr containerPtr,  
        IndexType & index,
        RangeType distance
    )
    -> RangeType
    {
        assert(containerPtr != nullptr); // containerptr should be valid
        // We jump over distance * jumpsize elements
        auto remainingJumpsize = nextElement(
            containerPtr, 
            index,  
            static_cast<RangeType>(jumpsize() * distance),
            containerSize
        );
        
        // we need the distance from the last element to the current index 
        // position this is a round up
        return static_cast<RangeType>(remainingJumpsize + jumpsize() - 1) 
             / static_cast<RangeType>(jumpsize());
    }
    
    
    /**
     * @brief The function moves the iterator backward to the previous element. 
     * This function is only enabled, if the navigator is bidirectional.
     * @param containerPtr pointer to the container, over which we iterate
     * @param index in: current position of iterator; out: position of the 
     * iterator after the move.
     * @param distance number of elements you like to overjump.
     * @result the distance from the end element to the hypothetical position
     * given by the distance parameter
     */
    template< bool T=isBidirectional>
    HDINLINE
    auto
    previous(
        ContainerPtr containerPtr,  
        IndexType & index,
        RangeType distance
    )
    -> typename std::enable_if<T==true, RangeType>::type
    {
        assert(containerPtr != nullptr); // containerptr should be valid
        // We jump over distance * jumpsize elements
        auto remainingJumpsize = previousElement(
            containerPtr, 
            index,
            offset(),
            static_cast<RangeType>(jumpsize() * distance),
            containerSize
        );
        

        // we need the distance from the last element to the current index 
        // position
        return static_cast<RangeType>(remainingJumpsize + jumpsize() - 1) 
             / static_cast<RangeType>(jumpsize());
    }
    
    /**
     * @brief set the iterator to the first element
     * @param containerPtr pointer to the container, over which we iterate
     * @param index out: first element of the iterator.
     */
    HDINLINE 
    auto
    begin(
        ContainerPtr containerPtr,  
        IndexType & index
    )
    -> void
    {
        assert(containerPtr != nullptr); // containerptr should be valid
        firstElement(
            containerPtr,
            index
        );
        nextElement(
            containerPtr, 
            index,  
            static_cast<RangeType>(offset()),
            containerSize
        );
    }
    
    /**
     * @brief set the iterator to the last element. 
     * @param containerPtr pointer to the container, over which we iterate
     * @param index out: last element of the iterator.
     */

    template< bool T=isBidirectional>
    HDINLINE 
    auto
    rbegin(
        ContainerPtr containerPtr,  
        IndexType & index
    )
    -> void
    {
        assert(containerPtr != nullptr); // containerptr should be valid
        auto nbElementsVar = nbElements(containerPtr);
        // -1 since we dont like to jump outside
        auto nbJumps = (nbElementsVar - offset() - 1) / jumpsize();
        auto lastPosition = nbJumps * jumpsize() + offset();
        // -1 since we need the last position
        auto neededJumps = (nbElementsVar - 1) - lastPosition;

        lastElement(
            containerPtr,
            index,
            containerSize
        );
        previousElement(
            containerPtr, 
            index,
            offset(),
            static_cast<RangeType>(neededJumps),
            containerSize
        );

        
    }
    
    /**
     * @brief set the iterator to the after last element
     * @param containerPtr pointer to the container, over which we iterate
     * @param index out: index of the after last element
     */
    HDINLINE 
    auto
    end(
        ContainerPtr containerPtr,  
        IndexType & index
    )
    -> void
    {
        afterLastElement.set(
            containerPtr,
            index,
            containerSize
        );
    }
    
    /**
     * @brief set the iterator to the last element. It is possible that two 
     * iterators, the first start with begin, the second with last, never meet.
     * @param containerPtr pointer to the container, over which we iterate
     * @param index out: index of the before first element
     */

    template< bool T=isBidirectional>
    HDINLINE
    auto
    rend(
        ContainerPtr containerPtr,  
        IndexType & index
    )
    ->     typename std::enable_if<T==true>::type
    {
        beforeFirstElement.set(
            containerPtr,
            index,
            offset(),
            containerSize
        );
    }
    
    /**
     * @brief check wheter the index is after the last element
     * @param containerPtr pointer to the container, over which we iterate
     * @param index in: current index position
     * @return true, if the index is after the last element; false otherwise
     */
    HDINLINE 
    auto
    isAfterLast(
        ContainerPtr containerPtr,  
        IndexType const & index
    )
    const
    -> bool
    {
        return afterLastElement.test(
            containerPtr,
            index,
            containerSize
        );
    }
    
    /**
     * @brief check wheter the index is before the first element
     * @param containerPtr pointer to the container, over which we iterate
     * @param index in: current index position
     * @return true, if the index is before the first element; false otherwise
     */
    template< bool T=isBidirectional>
    HDINLINE 
    auto
    isBeforeFirst(
        ContainerPtr containerPtr,   
        IndexType const & index
    )
    const
    -> typename std::enable_if<
        T==true,
        bool
    >::type
    {
        return beforeFirstElement.test(
            containerPtr, 
            index, 
            offset(), 
            containerSize
        );
    }
    
    /**
     * @brief this function determine the number of elements within the 
     * container
     * @param containerPtr pointer to the container, you like to know the number
     * of elements
     * @return number of elements within the container
     */
    HDINLINE
    auto
    nbElements(ContainerPtr containerPtr)
    const
    -> RangeType
    {
        assert(containerPtr != nullptr); // containerptr should be valid
        return containerSize(containerPtr);
    }
    
    /**
     * @brief this function determine the number of elements over which the
     * navigator goes. I.e sizeContainer / jumpsize
     * @param containerPtr pointer to the container, you like to know the number
     * of elements
     * @return number of elements the navigator can access
     */
    HDINLINE
    auto
    size(ContainerPtr containerPtr)
    const
    -> RangeType
    {
        assert(containerPtr != nullptr); // containerptr should be valid
        auto const nbElem = nbElements(containerPtr);
        auto const off = offset();
        assert(nbElem >= off);
        return (nbElem - off + jumpsize() - static_cast<RangeType>(1)) 
            / jumpsize();
    }
    
//variables
protected:
    OffsetType offset;
    JumpsizeType jumpsize;
    NumberElements containerSize;
    FirstElement firstElement;
    NextElement nextElement;
    AfterLastElement afterLastElement;
    LastElement lastElement;
    PreviousElement previousElement;
    BeforeFirstElement beforeFirstElement;
} ;


/**
 * @brief This navigator is a concept. It has an offset and a jumpsize.
 */

template<
    typename TOffset,
    typename TJumpsize
>
struct Navigator<
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    TOffset,
    TJumpsize,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    pmacc::details::UndefinedType,
    false>
{
    using ContainerType = pmacc::details::UndefinedType;
    using ContainerPtr = ContainerType*;
    using ContainerRef = ContainerType&;
    using ComponentType = pmacc::details::UndefinedType ;
    using ComponentPtr = ComponentType*;
    using JumpsizeType = TJumpsize;
    using OffsetType = TOffset;
    using IndexType = pmacc::details::UndefinedType ;
    using RangeType = pmacc::details::UndefinedType ;
    using NumberElements = pmacc::details::UndefinedType ;
    using FirstElement = pmacc::details::UndefinedType ;
    using NextElement = pmacc::details::UndefinedType ;
    using AfterLastElement = pmacc::details::UndefinedType ;
    using LastElement = pmacc::details::UndefinedType ;
    using PreviousElement = pmacc::details::UndefinedType ;
    using BeforeFirstElement = pmacc::details::UndefinedType ;
    // the default constructors
    HDINLINE Navigator() = default;
    HDINLINE Navigator(Navigator const &) = default;
    HDINLINE Navigator(Navigator &&) = default;
    HDINLINE ~Navigator() = default;
    
    /**
     * @brief Set the offset and the jumpsize to the given values
       @param offset the distance from the start to the first element
       @param jumpsize distance between two elements
    */
    template<
        typename TOffset_,
        typename TJumpsize_
    >
    HDINLINE
    Navigator(
            TOffset_ && offset, 
            TJumpsize_ && jumpsize
    ):
        offset(pmacc::iterator::forward<TOffset_>(offset)),
        jumpsize(pmacc::iterator::forward<TJumpsize_>(jumpsize))
    {}
    
    OffsetType offset;
    JumpsizeType jumpsize;
} ;


/**
 * @brief creates an navigator concept. It needs an offset and the jumpsize
 * @param offset distance from the begining of the container to the first 
 * position of the iterator 
 * @param jumpsize distance between two elements within the container
 * 
 */
template<
    typename TOffset,
    typename TJumpsize
>
HDINLINE
auto 
makeNavigator(
    TOffset && offset,
    TJumpsize && jumpsize
)
-> 
    pmacc::Navigator<
        details::UndefinedType,
        details::UndefinedType,
        typename std::decay<TOffset>::type,
        typename std::decay<TJumpsize>::type,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        false>
{
    typedef typename std::decay<TOffset>::type OffsetType;
    typedef typename std::decay<TJumpsize>::type JumpsizeType;
    typedef pmacc::Navigator<
        details::UndefinedType,
        details::UndefinedType,
        OffsetType,
        JumpsizeType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        pmacc::details::UndefinedType,
        false> ResultType;
    auto && result = ResultType(
        pmacc::iterator::forward<TOffset>(offset),
        pmacc::iterator::forward<TJumpsize>(jumpsize));
    return result;
}



namespace details
{



    template<typename T>
    struct NavigatorTemplates
    {
        using _T = typename std::decay<T>::type;
        using ContainerType = typename _T::ContainerType;
        using OffsetType = typename _T::OffsetType;
        using JumpsizeType = typename _T::JumpsizeType;
        using IndexType = typename _T::IndexType;
        using RangeType = typename _T::RangeType;
        using NumberElements = typename _T::NumberElements;
        using FirstElement = typename _T::FirstElement;
        using NextElement = typename _T::NextElement;
        using AfterLastElement = typename _T::AfterLastElement;
        using LastElement = typename _T::LastElement;
        using PreviousElement = typename _T::PreviousElement;
        using BeforeFirstElement = typename _T::BeforeFirstElement;
        
    };


template<
    typename TContainer,
    typename TContainerNoRef = typename std::decay<TContainer>::type,
    typename TNavigator,
    typename TOffset = typename details::NavigatorTemplates<
        TNavigator
    >::OffsetType,
    typename TJumpsize = typename details::NavigatorTemplates<
        TNavigator
    >::JumpsizeType,
    typename TComponent = typename pmacc::traits::ComponentType<
        TContainerNoRef
    >::type,
    typename TContainerCategorie = typename pmacc::traits::ContainerCategory<
        TContainerNoRef
    >::type,
    typename TContainerSize = typename pmacc::traits::NumberElements<
        TContainerNoRef
    >,
    typename TIndex = typename pmacc::traits::IndexType<
        TContainerNoRef
    >::type,
    typename TRange = typename std::decay<
        typename OffsetRangeType<TOffset>::type
    >::type,
    typename TFirstElement = typename pmacc::traits::navigator::FirstElement<
        TContainerNoRef, 
        TIndex, 
        TContainerCategorie
    >,
    typename TAfterLastElement = typename pmacc::traits::navigator::AfterLastElement<
        TContainerNoRef, 
        TIndex, 
        TContainerCategorie
    >,
    typename TNextElement = typename pmacc::traits::navigator::NextElement<
        TContainerNoRef, 
        TIndex, 
        TRange, 
        TContainerCategorie
    >,
    typename TLastElement = typename pmacc::traits::navigator::LastElement<
        TContainerNoRef, 
        TIndex, 
        TContainerCategorie>,
    typename TPreviousElement = typename pmacc::traits::navigator::PreviousElement<
        TContainerNoRef, 
        TIndex, 
        TRange, 
        TContainerCategorie
    >,
    typename TBeforeFirstElement = typename pmacc::traits::navigator::BeforeFirstElement<
        TContainerNoRef, 
        TIndex, 
        TRange, 
        TContainerCategorie
    >,
    bool isBidirectional = not std::is_same<
        TLastElement, 
        pmacc::details::UndefinedType
    >::value
>
auto
HDINLINE
makeNavigator(TNavigator && navi)
->
pmacc::Navigator<
    TContainerNoRef,
    TComponent,
    TOffset,
    TJumpsize,
    TIndex,
    TContainerSize,
    TRange,
    TFirstElement,
    TNextElement,
    TAfterLastElement,
    TLastElement,
    TPreviousElement,
    TBeforeFirstElement,
    isBidirectional
>
{
    using ResultType = pmacc::Navigator<
        TContainerNoRef,
        TComponent,
        TOffset,
        TJumpsize,
        TIndex,
        TContainerSize,
        TRange,
        TFirstElement,
        TNextElement,
        TAfterLastElement,
        TLastElement,
        TPreviousElement,
        TBeforeFirstElement,
        isBidirectional
    > ;
        

    auto && result = ResultType(
        pmacc::iterator::forward<TOffset>(navi.offset),
        pmacc::iterator::forward<TJumpsize>(navi.jumpsize)
    );

    return result;
}


} // namespace details


/**
 * @brief creates an iterator
 * @tparam container type of the container
 * @param offset distance from the start of the container to the first element 
 * of the iterator
 * @param jumpsize distance between to elements within the container
 */

template<
    typename TContainer,
    typename TContainerNoRef = typename std::decay<TContainer>::type,
    typename TOffset,
    typename TJumpsize,
    typename TComponent = typename pmacc::traits::ComponentType<
        TContainerNoRef
    >::type,
    typename TContainerCategorie = typename pmacc::traits::ContainerCategory<
        TContainerNoRef
    >::type,
    typename TContainerSize = typename pmacc::traits::NumberElements<
        TContainerNoRef
    >::type,
    typename TIndex = typename pmacc::traits::IndexType<
        TContainerNoRef
    >::type,
    typename TRange = decltype(TOffset::operator()()),
    typename TFirstElement = typename pmacc::traits::navigator::FirstElement<
        TContainerNoRef, 
        TIndex, 
        TContainerCategorie
    >::type,
    typename TAfterLastElement = typename pmacc::traits::navigator::AfterLastElement<
        TContainerNoRef, 
        TIndex, 
        TContainerCategorie
    >::type,
    typename TNextElement = typename pmacc::traits::navigator::NextElement<
        TContainerNoRef, 
        TIndex, 
        TRange, 
        TContainerCategorie
    >::type,
    typename TLastElement = typename pmacc::traits::navigator::LastElement<
        TContainerNoRef, 
        TIndex, 
        TContainerCategorie
    >::type,
    typename TPreviousElement = typename pmacc::traits::navigator::PreviousElement<
        TContainerNoRef, 
        TIndex, 
        TRange, 
        TContainerCategorie
    >::type,
    typename TBeforeFirstElement = typename pmacc::traits::navigator::BeforeFirstElement<
        TContainerNoRef, 
        TIndex, 
        TRange, 
        TContainerCategorie
    >::type,
    bool isBidirectional = not std::is_same<
        TLastElement, 
        pmacc::details::UndefinedType
    >::value
>
auto 
HDINLINE
makeNavigator(
    TOffset && offset,
    TJumpsize && jumpsize
)
-> 
    pmacc::Navigator<
        TContainerNoRef,
        TComponent,
        TOffset,
        TJumpsize,
        TIndex,
        TContainerSize,
        TRange,
        TFirstElement,
        TNextElement,
        TAfterLastElement,
        TLastElement,
        TPreviousElement,
        TBeforeFirstElement,
        isBidirectional
    >
{

    using ResultType =  pmacc::Navigator<
        TContainerNoRef,
        TComponent,
        TOffset,
        TJumpsize,
        TIndex,
        TContainerSize,
        TRange,
        TFirstElement,
        TNextElement,
        TAfterLastElement,
        TLastElement,
        TPreviousElement,
        TBeforeFirstElement,
        isBidirectional
    > ;
    auto && result = ResultType(
        pmacc::iterator::forward<TOffset>(offset),
        pmacc::iterator::forward<TJumpsize>(jumpsize)
    );
    
    return result;
}

}// namespace pmacc

