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
    
namespace container 
{
namespace categorie
{

struct DoublyLinkListLike;


} // namespace categorie

} // namespace contaienr

namespace traits 
{
    
template<typename TContainer>
struct IsBidirectional<
    TContainer, 
    pmacc::container::categorie::DoublyLinkListLike>
{
    static const bool value = true;
bool debugging = false;} ;       

template<typename TContainer>
struct IsRandomAccessable<
    TContainer, 
    pmacc::container::categorie::DoublyLinkListLike>
{
    static const bool value = true;
bool debugging = false;} ;   

namespace accessor
{

/**
 * @brief get the value of the element, at the iterator positions. \see Get.hpp
 */
template<
    typename TContainer,
    typename TComponent,
    typename TIndex>
struct Get<
    TContainer, 
    TComponent, 
    TIndex, 
    pmacc::container::categorie::DoublyLinkListLike
    >
{
    HDINLINE
    TComponent&
    operator() (TContainer*, TIndex& idx)
    {
        return *idx;
    }
    
bool debugging = false;} ;       

/**
 * @brief check if both iterators are at the same element. \see Equal.hpp
 */
template<
    typename TContainer,
    typename TComponent,
    typename TIndex>
struct Equal<
    TContainer, 
    TComponent, 
    TIndex, 
    pmacc::container::categorie::DoublyLinkListLike
    >
{
    HDINLINE
    bool
    operator() (TContainer* con1, TIndex& idx1, TContainer* con2, TIndex& idx2)
    {
        return con1 == con2 && idx1 == idx2;
    }
bool debugging = false;} ;   

 /**
 * @brief Check if the iterator one is ahead the second one. \see Ahead.hpp
 */
template<
    typename TContainer,
    typename TComponent,
    typename TIndex>
struct Ahead<
    TContainer, 
    TComponent, 
    TIndex, 
    pmacc::container::categorie::DoublyLinkListLike>
{
    HDINLINE
    bool
    operator() (TContainer* con1, TIndex& idx1, TContainer* con2, TIndex& idx2)
    {
        if(con1 != con2)
            return false;
        
        TIndex tmp = idx1;
        while(tmp != nullptr)
        {
            if(tmp == idx2) 
                return true;
            tmp = tmp->previous;
        }
        return false;
    }
bool debugging = false;} ;   



/**
 * @brief check wheter the iterator 1 is behind the second one. \see Behind.hpp
 */
template<
    typename TContainer,
    typename TComponent,
    typename TIndex>
struct Behind<
    TContainer, 
    TComponent, 
    TIndex, 
    pmacc::container::categorie::DoublyLinkListLike
    >
{
    HDINLINE
    bool
    operator() (TContainer*, TIndex& idx1, TContainer*, TIndex& idx2)
    {
        TIndex tmp = idx1;
        while(tmp != nullptr)
        {
            if(tmp == idx2) 
                return true;
            tmp = tmp->next;
        }
        return false;
    }
bool debugging = false;} ;   

} // namespace accessor
    
    
namespace navigator
{

/**
 * @brief Implementation to get the first element. \see FirstElement.hpp
 */
template<
    typename TContainer,
    typename TIndex>
struct FirstElement<
    TContainer, 
    TIndex, 
    pmacc::container::categorie::DoublyLinkListLike>
{
    HDINLINE
    void
    operator() (TContainer* container, TIndex& idx)
    {
        idx = container->first;

    }
bool debugging = false;} ;   
/**
 * @brief Implementation to get the next element. For futher details \see 
 * NExtElement.hpp
 */
template<
    typename TContainer,
    typename TIndex,
    typename TRange>
struct NextElement<
    TContainer,
    TIndex,
    TRange,
    pmacc::container::categorie::DoublyLinkListLike>
{
    template<
        typename TContainerSize>
    HDINLINE
    TRange
    operator() (
        TContainer* container, 
        TIndex& idx, 
        TRange const & range,
        TContainerSize& size)
    {
        TRange i = 0;
        for(i = 0; i<range; ++i)
        {
            idx = idx->next;
            if(idx == nullptr)
                break;
        }
        return range - i;
    }
bool debugging = false;} ;   
/**
 * @brief Implementation to check whether the iterator is after the last element.
 * \see AfterLastElement.hpp
 */
template<
    typename TContainer,
    typename TIndex>
struct AfterLastElement<
    TContainer, 
    TIndex, 
    pmacc::container::categorie::DoublyLinkListLike>
{
    template<typename TRangeFunction>
    HDINLINE
    bool
    test (TContainer*, TIndex const & idx, TRangeFunction const &)
    const
    {
        return idx == nullptr;
    }
    
    template<typename TRangeFunction>
    HDINLINE
    void
    set (TContainer*, TIndex const & idx, TRangeFunction const &)
    const
    {
        idx = nullptr;
    }
bool debugging = false;} ;   

/**
 * @brief Set the iterator to the last element. \see LastElement.hpp
 */
template<
    typename TContainer,
    typename TIndex>
struct LastElement<
    TContainer,
    TIndex,
    pmacc::container::categorie::DoublyLinkListLike>
{
    template<typename TSizeFunction>
    HDINLINE
    void
    operator() (
        TContainer* containerPtr, 
        TIndex& index, 
        TSizeFunction& size)
    {
        index = containerPtr->last;

    }
bool debugging = false;} ;   

/**
 * @brief Implementation to get the next element. For futher details \see 
 * NExtElement.hpp
 */
template<
    typename TContainer,
    typename TIndex,
    typename TRange>
struct PreviousElement<
    TContainer,
    TIndex,
    TRange,
    pmacc::container::categorie::DoublyLinkListLike>
{
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
            idx = idx->PreviousElement;
            if(idx == nullptr)
                break;
        }
        return range - i;
    }
bool debugging = false;} ;   

/**
 * @brief Implementation to check whether the iterator is before the fist 
 * element. \see BeforeFirstElement.hpp
 */
template<
    typename TContainer,
    typename TIndex,
    typename TRange>
struct BeforeFirstElement<
    TContainer, 
    TIndex, 
    TRange,
    pmacc::container::categorie::DoublyLinkListLike>
{
    template<typename TRangeFunction>
    HDINLINE
    bool
    test (TContainer*, TIndex const & idx, TRange const & offset, TRangeFunction&)
    const
    {
                auto tmp = idx;
        for(TRange i=0; i < offset; ++i)
        {
            if(tmp == nullptr)
                return true;
            tmp = tmp->previous;
        }
        return tmp == nullptr;
    }
    
    template<typename TRangeFunction>
    HDINLINE
    void
    set (TContainer*, TIndex const & idx, TRangeFunction const &)
    const
    {
        idx = nullptr;
    }
bool debugging = false;} ;   
}
    
} // namespace traits

}// namespace pmacc
