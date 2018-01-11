/* Copyright 2013-2017 Heiko Burau, Rene Widera
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

template <class TYPE>
class SuperCell
{
public:

    HDINLINE SuperCell() :
    firstFramePtr(nullptr),
    lastFramePtr(nullptr),
    mustShiftVal(false),
    sizeLastFrame(0)
    {
    }

    HDINLINE TYPE* FirstFramePtr()
    {
        return firstFramePtr;
    }

    HDINLINE TYPE* LastFramePtr()
    {
        return lastFramePtr;
    }

    HDINLINE const TYPE* FirstFramePtr() const
    {
        return firstFramePtr;
    }

    HDINLINE const TYPE* LastFramePtr() const
    {
        return lastFramePtr;
    }

    HDINLINE bool mustShift()
    {
        return mustShiftVal;
    }

    HDINLINE void setMustShift(bool value)
    {
        mustShiftVal = value;
    }

    HDINLINE lcellId_t getSizeLastFrame()
    {
        return sizeLastFrame;
    }

    HDINLINE void setSizeLastFrame(lcellId_t size)
    {
        sizeLastFrame = size;
    }


private:
    PMACC_ALIGN(mustShiftVal, bool);
    PMACC_ALIGN(sizeLastFrame, lcellId_t);
public:
    PMACC_ALIGN(firstFramePtr, TYPE*);
    PMACC_ALIGN(lastFramePtr, TYPE*);
};

namespace traits 
{
/**
@brief number of elements within a frame
@todo Here we need to specify the number of particles in frames
*/
template<
typename T>
struct NumberElements<pmacc::SuperCell<T> >
{
    typedef pmacc::SuperCell<T> ContainerType;

    uint_fast32_t
    HDINLINE
    operator()(ContainerType* container)
    {
        auto result = 0;
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
typename T>
struct HasConstantSize<pmacc::SuperCell<T> >
{
    const static bool value = false;
};

template<
    typename T_Container>
struct ContainerCategory<pmacc::SuperCell<T_Container> >
{
    typedef pmacc::traits::categorie::SupercellLike type;
};

template<
typename T>
struct ComponentType<pmacc::SuperCell<T> >
{
    typedef T type;
};

}//namespace traits

} //end namespace pmacc
