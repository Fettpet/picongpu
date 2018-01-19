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
 * @brief This trait is used to set the iterator to the last element. If there
 * are not enough elements (e.g. empty container) The iterator is set to the 
 * AfterLastElement or the BeforeFirstElement. The trait need the operator() 
 * with three arguments:
 * 1. A pointer to the container
 * 2. A reference to the index
 * 3. An offset. This is the distance between the first element and the first 
 * iterator position. 
 * @tparam T_Container The container over which the iteartor walks.
 * @tparam T_Index The type of the index to get a component out of the container.
 * @tparam T_ContainerCategory An SFINAE type for categories.
 * @tparam TOffset Type of the offset. This is a template of the function, not
 * of the trait.
 * @tparam T_SizeFunction Function to get the size of a container. You can use 
 * T_SizeFunction(T_Container*) to get the number of the container. This is a 
 * template of the function.
 */
template<
    typename T_Container,
    typename T_Index,
    typename T_ContainerCategory
>
struct LastElement
{
    template<
        typename T_SizeFunction>
    HDINLINE
    void
    operator() (
        T_Container*,
        T_Index&,
        T_SizeFunction&
    );
};


} // namespace navigator
} // namespace traits
    
} // namespace pmacc


