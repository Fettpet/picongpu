/* Copyright 2013-2017 Heiko Burau, Rene Widera
 *
 * This file is part of libPMacc.
 *
 * libPMacc is free software: you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License or
 * the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libPMacc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License and the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and the GNU Lesser General Public License along with libPMacc.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "tag.hpp"
#include "cuSTL/cursor/BufferCursor.hpp"

#include "math/vector/Size_t.hpp"

#include <stdint.h>


namespace PMacc
{
namespace allocator
{

template<typename Type, int T_dim>
struct HostMemAllocator
{
    typedef Type type;
    static constexpr int dim = T_dim;
    typedef cursor::BufferCursor<type, T_dim> Cursor;
    typedef allocator::tag::host tag;

    HDINLINE
    static cursor::BufferCursor<type, T_dim> allocate(const math::Size_t<T_dim>& size);
    template<typename TCursor>
    HDINLINE
    static void deallocate(const TCursor& cursor);
};

template<typename Type>
struct HostMemAllocator<Type, 1>
{
    typedef Type type;
    static constexpr int dim = 1;
    typedef cursor::BufferCursor<type, 1> Cursor;
    typedef allocator::tag::host tag;

    HDINLINE
    static cursor::BufferCursor<type, 1> allocate(const math::Size_t<1>& size);
    template<typename TCursor>
    HDINLINE
    static void deallocate(const TCursor& cursor);
};

} // allocator
} // PMacc

#include "HostMemAllocator.tpp"

