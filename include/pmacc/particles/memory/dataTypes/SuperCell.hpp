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

}//namespace traits

} //end namespace pmacc
