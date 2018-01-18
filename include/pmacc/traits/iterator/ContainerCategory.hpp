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
 * \struct ContainerCategory
 * @author Sebastian Hahn (t.hahn < at > pmacc) 
 * @brief This trait is used to specify a categorie of a container. Currently are
 * three categories implemented. They are in the folder Iterator/Categorie. If 
 * a container has no categorie, you need to specify the following traits to use
 * the DeepIterator:
 * The first four are needed for the accesor:
 * 1. pmacc::traits::accessor::Ahead
 * 2. pmacc::traits::accessor::Behind
 * 3. pmacc::traits::accessor::Equal
 * 4. pmacc::traits::accessor::Get
 * The next six are needed for the navigator
 * 5. pmacc::traits::navigator::AfterLastElement
 * 6. pmacc::traits::navigator::BeforeFirstElement
 * 7. pmacc::traits::navigator::FirstElement
 * 8. pmacc::traits::navigator::LastElement 
 * 9. pmacc::traits::navigator::NextElement
 * 10. pmacc::traits::navigator::PreviousElement 
 * If a container isnt bidirectional you doesnt need 1, 2, 6, 8, 10.
 * If a container isnt randon accessable you doesnt need 1,2
 */ 
template<typename T>
struct ContainerCategory
{
    typedef T type;
};

}// namespace traits
    
}// namespace pmacc
