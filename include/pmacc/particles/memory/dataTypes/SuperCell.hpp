/* Copyright 2013-2018 Heiko Burau, Rene Widera, Sebastian Hahn
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

#include "pmacc/types.hpp"
#include "pmacc/traits/iterator/Componenttype.hpp"
#include "pmacc/traits/iterator/HasConstantSize.hpp"
#include "pmacc/traits/iterator/NumberElements.hpp"
#include "pmacc/traits/iterator/ContainerCategory.hpp"
#include "pmacc/traits/iterator/IndexType.hpp"
#include "pmacc/traits/iterator/RangeType.hpp"
#include "pmacc/memory/iterator/categorie/SupercellLike.hpp"

namespace pmacc
{

template <typename T_Frame>
class SuperCell
{
public:

    HDINLINE
    SuperCell() :
        firstFramePtr(nullptr),
        lastFramePtr(nullptr),
        mustShiftVal(false),
        sizeLastFrame(0)
    {
    }

    HDINLINE
    auto
    FirstFramePtr()
    -> T_Frame*
    {
        return firstFramePtr;
    }

    HDINLINE
    auto
    LastFramePtr()
    -> T_Frame*
    {
        return lastFramePtr;
    }

    HDINLINE
    auto
    FirstFramePtr( )
    const
    -> const T_Frame*
    {
        return firstFramePtr;
    }

    HDINLINE
    auto
    LastFramePtr()
    const
    ->  const T_Frame*
    {
        return lastFramePtr;
    }

    HDINLINE
    auto
    mustShift()
    ->bool
    {
        return mustShiftVal;
    }

    HDINLINE
    auto
    setMustShift(bool value)
    -> void
    {
        mustShiftVal = value;
    }

    HDINLINE
    auto
    getSizeLastFrame()
    -> lcellId_t
    {
        return sizeLastFrame;
    }

    HDINLINE
    auto
    setSizeLastFrame(lcellId_t size)
    ->void
    {
        sizeLastFrame = size;
    }


private:
    PMACC_ALIGN(mustShiftVal, bool);
    PMACC_ALIGN(sizeLastFrame, lcellId_t);
public:
    PMACC_ALIGN(firstFramePtr, T_Frame*);
    PMACC_ALIGN(lastFramePtr, T_Frame*);
};

namespace traits 
{
/** 
@brief number of frames within a supercell. At the moment, the supercell hasnt a
varible, where the number of frames is stored. So we start at the first and 
count.
*/
template< typename T>
struct NumberElements<pmacc::SuperCell<T> >
{
private:
    typedef pmacc::SuperCell<T> ContainerType;
public:

    auto
    HDINLINE
    operator()(ContainerType* container)
    const
    -> uint_fast32_t
    {
        uint_fast32_t result = 0;
        auto tmp = container->firstFramePtr;
        while(tmp != nullptr)
        {
            tmp = tmp->previousFrame.ptr;
            ++result;
        }
        return result;
    }
};

template<
    typename T,
    typename SFIANE
>
struct RangeType<
    pmacc::SuperCell<T>, 
    SFIANE
>
{
    using type = uint_fast32_t;
};

template< typename T>
struct HasConstantSize<pmacc::SuperCell<T> >
{
    const static bool value = false;
};

template< typename T_Container>
struct ContainerCategory<pmacc::SuperCell<T_Container> >
{
    typedef pmacc::traits::categorie::SupercellLike type;
};

template< typename T>
struct ComponentType<pmacc::SuperCell<T> >
{
    typedef T type;
};

template< typename T>
struct IndexType<pmacc::SuperCell<T> >
{
    typedef T* type;
};

#if 0
namespace navigator
{
/**
 * @brief implementation to get the first element within a container. For further
 * details \see FirstElement.hpp
 */
template<
    typename T,
    typename T_Index,
    typename SFIANE>
struct FirstElement<
    pmacc::SuperCell<T>, 
    T_Index, 
    SFIANE>
{
    typedef pmacc::SuperCell<T> TContainer;
    
    HDINLINE
    void
    operator() (
        TContainer* container, 
        T_Index& idx
    )
    {
        idx = container->FirstFramePtr();
    }
    
} ;

/**
 * @brief Implementation to get the next element. For futher details \see 
 * NExtElement.hpp
 */
template<
    typename T,
    typename T_Index,
    typename T_Range,
    typename TContainerCategorie>
struct NextElement<
    pmacc::SuperCell<T>,
    T_Index,
    T_Range,
    TContainerCategorie>
{
    typedef pmacc::SuperCell<T> TContainer;
    template<
        typename TContainerSize>
    HDINLINE
    T_Range
    operator() (
        TContainer* container, 
        T_Index& idx, 
        T_Range const & range,
        TContainerSize&)
    {
        T_Range i = static_cast<T_Range>(0);
        for(;i < range; ++i)
        {
            idx = container->next(idx);
            if(not idx.isValid())
                break;
        }
        return range - i;
    }
    
} ;

/**
 * @brief Implementation to check whether the end is reached. For further 
 * informations \see AfterLastElement.hpp
 */
template<
    typename T,
    typename T_Index,
    typename TContainerCategorie
>
struct AfterLastElement<
    pmacc::SuperCell<T>, 
    T_Index, 
    TContainerCategorie>
{
    
    typedef pmacc::SuperCell<T> TContainer;
    
    template<typename TSizeFunction>
    HDINLINE
    bool
    test (TContainer*, T_Index const & idx, TSizeFunction const &)
    const
    {
        return not idx.isValid();
    }
    
    template<typename TSizeFunction, typename TOffset_>
    HDINLINE
    void
    set(TContainer*, T_Index & idx, TOffset_ &&, TSizeFunction const &)
    const
    {
        idx.ptr = nullptr;
    }
    
} ;

/**
 * @brief Implementation of the array like last element trait. For further details
 * \see LastElement.hpp
 */
template<
    typename T,
    typename T_Index,
    typename TContainerCategorie
>
struct LastElement<
    pmacc::SuperCell<T>,
    T_Index,
    TContainerCategorie
>
{
    
    typedef pmacc::SuperCell<T> TContainer;
    
    template<typename TSizeFunction>
    HDINLINE
    void
    operator() (TContainer* conPtr, 
                T_Index& index, 
                TSizeFunction& size)
    {
        index = conPtr->LastFramePtr();
    }
    
} ;

/**
 * @brief The implementation to get the last element in a array like data
 * structure. For futher details \see PreviousElement.hpp
 */
template<
    typename T,
    typename T_Range,
    typename TContainerCategorie,
    typename T_Index
>
struct PreviousElement<
    pmacc::SuperCell<T>,
    T_Index,
    T_Range,
    TContainerCategorie
>
{
    
    typedef pmacc::SuperCell<T> TContainer;
    
    template<typename TSizeFunction>
    HDINLINE
    int
    operator() (
        TContainer* container, 
        T_Index& idx, 
        T_Range const &,
        T_Range const & jumpsize,
        TSizeFunction const & size)
    {
        T_Range i = static_cast<T_Range>(0);
        for(; i<jumpsize; ++i)
        {
            idx = container->previous(idx);
            if(not idx.isValid())
            {
                return jumpsize - i;
            }
        }

        return static_cast<T_Range>(0);
    }
    
} ;

/**
 * @brief Implmentation to get check whether the iterator is on the element 
 * before the first one. \see BeforeFirstElement.hpp
 */
template<
    typename T,
    typename T_Index,
    typename TOffset,
    typename TContainerCategorie
>
struct BeforeFirstElement<
    pmacc::SuperCell<T>, 
    T_Index,
    TOffset,
    TContainerCategorie>
{
    
    typedef pmacc::SuperCell<T> TContainer;
    
    template<typename TSizeFunction, typename TOffset_>
    HDINLINE
    bool
    test (TContainer*, T_Index const & idx, TOffset_ &&, TSizeFunction const &)
    const
    {
        return not idx.isValid();
    }
    
    template<typename TSizeFunction, typename TOffset_>
    HDINLINE
    void
    set(TContainer*, T_Index & idx, TOffset_ &&, TSizeFunction const &)
    const
    {
        idx.ptr = nullptr;
    }
}; // struct BeforeFirstElement
    
} // namespace navigator

namespace accessor
{

/**
 * @brief get the value of the element, at the iterator positions. \see Get.hpp
 */
template<
    typename T,
    typename T_Acc,
    typename T_Component,
    typename TContainerCategorie,
    typename T_Index
>
struct Get<
    pmacc::SuperCell<T>,
    T_Component, 
    T_Index, 
    TContainerCategorie
    >
{
    typedef pmacc::SuperCell<T> TContainer;
    
    HDINLINE
    T_Component&
    operator() (TContainer*, T_Index& idx)
    {
        return *idx;
    }
};    

/**
 * @brief check if both iterators are at the same element. \see Equal.hpp
 */
template<
    typename T,
    typename T_Component,
    typename TContainerCategorie,
    typename T_Index
>
struct Equal<
    pmacc::SuperCell<T>,
    T_Component, 
    T_Index, 
    TContainerCategorie
>
{
    typedef pmacc::SuperCell<T> TContainer;
    HDINLINE
    bool
    operator() (
        TContainer* con1, 
        T_Index& idx1, 
        TContainer* con2, 
        T_Index& idx2
    )
    {
        return con1 == con2 && idx1 == idx2;
    }
};


} // namespace accessor

#endif
}//namespace traits

} //end namespace pmacc
