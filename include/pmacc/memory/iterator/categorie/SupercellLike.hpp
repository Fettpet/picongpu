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
            tmp = tmp->previousFrame;
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
            tmp = tmp->nextFrame;
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
        idx = container->firstFrame;
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
            idx = idx->nextFrame;
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

        index = containerPtr->lastFrame;


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
        TRange const &,
        TRange const & jumpsize,
        TContainerSize&)
    {
        TRange i = 0;
        for(i = 0; i<jumpsize; ++i)
        {
            idx = idx->previousFrame;
            if(idx == nullptr)
                return jumpsize - i;
        }

        return jumpsize - i;
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
            tmp = tmp->previousFrame;
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

