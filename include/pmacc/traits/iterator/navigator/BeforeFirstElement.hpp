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
 * @brief This trait is used as the element before the first one. The trait has
 * two functions: 
 * 1. test(Container*, Index, ContainerSizeFunction): returns true if the 
 * current element is before the first one, false otherwise
 * 2. set(Container*, Index, ContainerSizeFunction): Set the iterator to the
 * element before the first one. 
 * Both function has three arguments:
 * 1. Container*: A pointer to the container, over which you itearte
 * 2. Index: The current position of the iterator
 * 3. ContainerSizeFunction: If the number of elements within the container is
 * needed, this argument can be used. Call ContainerSizeFunction(Container*) to 
 * get the number of elements. This could be an expensiv operation.
 * @tparam T_Container The container over which the iteartor walks.
 * @tparam T_Index The type of the index to get a component out of the container.
 * @tparam T_ContainerCategory An SFINAE type for categories.
 * @tparam T_SizeFunction This is used to give a function, which calculate the 
 * size of the container, to the trait. It is a template of the function, not of
 * the trait.
 */
template<
    typename T_Container,
    typename T_Index,
    typename T_Offset,
    typename T_ContainerCategory
>
struct BeforeFirstElement
{
    template<typename T_SizeFunction>
    HDINLINE
    auto
    set(
        T_Container*,
        T_Index&,
        T_Offset&,
        T_SizeFunction&
    )
    const
    -> void;

    template<typename T_SizeFunction>
    HDINLINE
    auto
    set(
        T_Container*,
        T_Index&,
        T_Offset&,
        T_SizeFunction&
    )
    const
    -> bool ;
    
};

}// namespace navigator

} // namespace traits
    
} // namespace pmacc

