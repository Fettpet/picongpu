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

namespace pmacc 
{
namespace traits
{
/**
 * @brief Each container categorie need to determine if it is a random accessable.
 * A container categorie is random accessable if there is a way to overjump some
 * values easily. Mostly a container is random accessable if the operator [] is
 * overloaded. 
 */
template<typename TContainerCategorie, typename SFIANE = void>
struct IsRandomAccessable;


}// namespace traits
}// namespace pmacc

