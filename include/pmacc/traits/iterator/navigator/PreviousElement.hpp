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
namespace navigator
{
    /**
 * @brief This trait is used to set the index to the previous element. The trait
 * need the operator() with four arguments:
 * 1. A pointer to the container
 * 2. A reference to the index
 * 3. Jumpsize: Distance between the current and previous element.
 * 4. T_ContainerSize: Trait to get the size of the container. 
 * 
 * @result If the jumpsize is greater than the remaining elements in the
 * container, the result is the "unjumped" elements, i.e
 *  -1 * [Hypotetical positon]. 
 * 
 * @tparam T_Container The container over which the iteartor walks.
 * @tparam T_Index The type of the index to get a component out of the container.
 * @tparam T_ContainerCategory An SFINAE type for categories.
 * @tparam T_Range Type of the offset. This is a template of the function, not
 * of the trait.
 */
template<
    typename T_Container,
    typename T_Index,
    typename T_Range,
    typename T_ContainerCategory
>
struct PreviousElement;


} // namespace navigator

} // namespace traits
    
} // namespace pmacc


