/* Copyright 2018 Sebastian Hahn
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

namespace pmacc
{
namespace traits
{
namespace accessor
{
/**
 * @brief We use this trait to check whether an iterators position is ahead an
 * other iterators position. This means it1 > it2. The trait need the 
 * ()-operator, which has four arguments:
 * 1. A pointer to the container of the first iterator,
 * 2. The index of the first iterator,
 * 3. A pointer to the container of the second iterator 
 * 4. The index of the second iterator
 * This trait is needed in the random access case.
 * @tparam TContainer The container over which the iteartor walks.
 * @tparam TComponent The type of the component of the container.
 * @tparam TIndex The type of the index to get a component out of the container.
 * @tparam TContainerCategory An SFINAE type for categories.
 * @return frue, if the first iterator is ahead the second one, false otherwise
 *
 */
template<
    typename TContainer,
    typename TComponent,
    typename TIndex,
    typename TContainerCategory
>
struct Ahead
{
    HDINLINE
    bool
    operator() (
        TContainer*,
        TIndex&,
        TContainer*,
        TIndex&
    );
    
};




} // namespace accessor
} // namespace traits
    
} // namespace pmacc
