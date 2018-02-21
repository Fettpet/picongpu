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
#include "pmacc/traits/iterator/IsBidirectional.hpp"
#include "pmacc/traits/iterator/IsRandomAccessable.hpp"
#include "pmacc/traits/iterator/accessor/Ahead.hpp"
#include "pmacc/traits/iterator/accessor/Behind.hpp"
#include "pmacc/traits/iterator/accessor/Equal.hpp"
#include "pmacc/traits/iterator/accessor/Get.hpp"
#include "pmacc/traits/iterator/navigator/AfterLastElement.hpp"
#include "pmacc/traits/iterator/navigator/BeforeFirstElement.hpp"
#include "pmacc/traits/iterator/navigator/LastElement.hpp"
#include "pmacc/traits/iterator/navigator/NextElement.hpp"
#include "pmacc/traits/iterator/navigator/PreviousElement.hpp"
#include "pmacc/traits/iterator/navigator/FirstElement.hpp"

namespace pmacc
{
namespace traits 
{
namespace categorie
{
/**
 * A container is supercell like if the following conditions hold
 * 1. The container has two public variables: firstFramePtr, and lastFramePtr. The 
 * variables firstFramePtr and lastFramePtr are pointers of the index type.
 * 2. The index type has two public variables: nextFrame.ptr, previousFrame.ptr. Both 
 * are pointers of other index types. 
 * 3. con->firstFramePtr->previousFrame.ptr == nullptr
 * 4. con->lastFramePtr->nextFrame.ptr == nullptr
 * It is possible, that more conditions exists.
 */
struct SupercellLike;
}



template<typename T>
struct IsBidirectional<
    T, 
    pmacc::traits::categorie::SupercellLike>
{
    static const bool value = true;
} ;    

template<
    typename T>
struct IsRandomAccessable<
    T, 
    pmacc::traits::categorie::SupercellLike>
{
    static const bool value = true;
} ;
namespace accessor
{

/**
 * @brief get the value of the element, at the iterator positions. \see Get.hpp
 */
template<
    typename T,
    typename TComponent,
    typename TIndex>
struct Get<
    T, 
    TComponent, 
    TIndex, 
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    HDINLINE
    TComponent&
    operator() (TContainer*, TIndex& idx)
    {
        return *idx;
    }
} ;    

/**
 * @brief check if both iterators are at the same element. \see Equal.hpp
 */
template<
    typename T,
    typename TComponent,
    typename TIndex>
struct Equal<
    T,
    TComponent, 
    TIndex, 
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    HDINLINE
    bool
    operator() (TContainer* con1, TIndex const & idx1, TContainer* con2, TIndex const & idx2)
    {
        return con1 == con2 && idx1 == idx2;
    }
} ;

 /**
 * @brief Check if the iterator one is ahead the second one. \see Ahead.hpp
 */
template<
    typename T,
    typename TComponent,
    typename TIndex>
struct Ahead<
    T,
    TComponent, 
    TIndex, 
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    HDINLINE
    bool
    operator() (TContainer* con1, TIndex const & idx1, TContainer* con2, TIndex const & idx2)
    {
        if(con1 != con2)
            return false;
        
        TIndex tmp = idx1;
        while(tmp != nullptr)
        {
            tmp = tmp->previousFrame.ptr;
            if(tmp == idx2) 
                return true;
           
        }
        return false;
    }
} ;



/**
 * @brief check wheter the iterator 1 is behind the second one. \see Behind.hpp
 */
template<
    typename T,
    typename TComponent,
    typename TIndex>
struct Behind<
    T,
    TComponent, 
    TIndex, 
    pmacc::traits::categorie::SupercellLike
    >
{
    typedef T TContainer;
    HDINLINE
    bool
    operator() (TContainer*, TIndex const & idx1, TContainer*, TIndex const & idx2)
    {
        TIndex tmp = idx1;
        while(tmp != nullptr)
        {
            tmp = tmp->nextFrame.ptr;
            if(tmp == idx2) 
                return true;
            
        }
        return false;
    }
} ;

} // namespace accessor
    
    
namespace navigator
{

/**
 * @brief Implementation to get the first element. \see FirstElement.hpp
 */
template<
    typename T,
    typename TIndex>
struct FirstElement<
    T,
    TIndex, 
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    HDINLINE
    void
    operator() (TContainer* container, TIndex& idx)
    {
        idx = container->firstFramePtr;
    }
} ;
/**
 * @brief Implementation to get the next element. For futher details \see 
 * NExtElement.hpp
 */
template<
    typename T,
    typename TIndex,
    typename TRange>
struct NextElement<
    T,
    TIndex,
    TRange,
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    template<
        typename TContainerSize>
    HDINLINE
    TRange
    operator() (
        TContainer*, 
        TIndex& idx, 
        TRange const & range,
        TContainerSize&)
    {
        TRange i = 0;
        for(i = 0; i<range; ++i)
        {
            idx = idx->nextFrame.ptr;
            if(idx == nullptr)
                break;
        }
        return range - i;
    }
} ;
/**
 * @brief Implementation to check whether the iterator is after the last element.
 * \see AfterLastElement.hpp
 */
template<
    typename T,
    typename TIndex>
struct AfterLastElement<
    T,
    TIndex, 
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    template<typename TRangeFunction>
    HDINLINE
    bool
    test(TContainer*, TIndex const & idx, TRangeFunction const &)
    const
    {
        return idx == nullptr;
    }
    
    template<typename TRangeFunction>
    HDINLINE
    void
    set(TContainer*, TIndex & idx, TRangeFunction const &)
    const
    {
        idx = nullptr;
    }
} ;

/**
 * @brief Set the iterator to the last element. \see LastElement.hpp
 */
template<
    typename T,
    typename TIndex>
struct LastElement<
    T,
    TIndex,
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    template<typename TSizeFunction>
    HDINLINE
    void
    operator() (
        TContainer* containerPtr, 
        TIndex& index, 
        TSizeFunction &&)
    {

        index = containerPtr->lastFramePtr;


    }
} ;

/**
 * @brief Implementation to get the next element. For futher details \see 
 * NExtElement.hpp
 */
template<
    typename T,
    typename TIndex,
    typename TRange>
struct PreviousElement<
    T,
    TIndex,
    TRange,
    pmacc::traits::categorie::SupercellLike>
{
    
    typedef T TContainer;
    template<
        typename TContainerSize>
    HDINLINE
    TRange
    operator() (
        TContainer*, 
        TIndex& idx, 
        TRange const & offset,
        TRange const & jumpsize,
        TContainerSize&)
    {
        for(TRange i = static_cast<TRange>(0); i<jumpsize; ++i)
        {
            idx = idx->previousFrame.ptr;
            if(idx == nullptr)
                return jumpsize - i;
        }
        // check the offset
        TIndex buffer = idx;
        for(TRange i = static_cast<TRange>(0); i<offset; ++i)
        {
            buffer = buffer->previousFrame.ptr;
            if(buffer == nullptr)
            {
                idx = nullptr;
                return offset - i;
            }
        }

        return 0;
    }
} ;

/**
 * @brief Implementation to check whether the iterator is before the fist 
 * element. \see BeforeFirstElement.hpp
 */
template<
    typename T,
    typename TIndex,
    typename TRange>
struct BeforeFirstElement<
    T,
    TIndex, 
    TRange,
    pmacc::traits::categorie::SupercellLike>
{
    typedef T TContainer;
    
    template<typename TRangeFunction>
    HDINLINE
    bool
    test(TContainer*, TIndex const & idx, TRange const & offset, TRangeFunction&)
    const
    {
        TIndex tmp = idx;
        for(TRange i = static_cast<TRange>(0); i < offset; ++i)
        { 
            if(tmp == nullptr)
                return true;
            tmp = tmp->previousFrame.ptr;
        }
        return tmp == nullptr;
    }
    

    template<typename TRangeFunction>
    HDINLINE
    void
    set(TContainer*, TIndex & idx,TRange const &, TRangeFunction&)
    const
    {
        idx = nullptr;
    }
} ;
}
    
} // namespace traits

}// namespace pmacc

