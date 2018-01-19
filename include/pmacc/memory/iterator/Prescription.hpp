/* Copyright 2013-2018 Sebastian Hahn
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
#include "pmacc/forward.hpp"
#include "pmacc/memory/iterator/Navigator.hpp"

namespace pmacc 
{

namespace details 
{
    /**
 * @brief A prescription consists of an accessor, a navigator and optional a 
 * child. A prescription decribes an abstract way to iterate through the data. 
 * The navigator and the accessor are not bound to a container. A prescription 
 * can be created with the makeIteratorPrescription function. 
 * 
 */

template<
    typename T_Accessor,
    typename T_Navigator,
    typename T_Child>
struct IteratorPrescription
{
    typedef T_Navigator NavigatorType;
    typedef T_Accessor AccessorType;
    typedef T_Child ChildType;
    
    HDINLINE 
    IteratorPrescription() = delete;
    
    HDINLINE 
    IteratorPrescription(IteratorPrescription const &) = default;
    
    HDINLINE 
    IteratorPrescription(IteratorPrescription &&) = default;
    
    template<
        typename T_Navigator_,
        typename T_Accessor_>
    HDINLINE
    IteratorPrescription(T_Accessor_ && acc,
                    T_Navigator_ && navi):
        child(pmacc::NoChild()),
        navigator(pmacc::iterator::forward<T_Navigator_>(navi)),
        accessor(pmacc::iterator::forward<T_Accessor_>(acc))
    {}
    
    template<
        typename T_Navigator_,
        typename T_Accessor_,
        typename T_Child_>
    HDINLINE
    IteratorPrescription(T_Accessor_ && acc,
                    T_Navigator_ && navi,
                    T_Child_ && child
                   ):
        child(pmacc::iterator::forward<T_Child_>(child)),
        navigator(pmacc::iterator::forward<T_Navigator_>(navi)),
        accessor(pmacc::iterator::forward<T_Accessor_>(acc))
    {}
    
    ChildType child;
    NavigatorType navigator;
    AccessorType accessor;
} ;

template<typename T_Prescription>
struct PrescriptionTypes
{
    typedef typename T_Prescription::AccessorType AccessorType;
    typedef typename T_Prescription::NavigatorType NavigatorType;
    typedef typename T_Prescription::AccessorType ChildType;
};

} // namespace details


/**
 * @brief creates an iterator concept. This concept has no childs.
 * @param accessor Describes a concept to dereference the element 
 * @param navigator describe a concept of walking through the container
 * @return An iterator concept
 */
template<
    typename T_Accessor,
    typename T_Navigator>
HDINLINE
auto
makeIteratorPrescription(T_Accessor&& accessor,
             T_Navigator&& navigator)
-> pmacc::details::IteratorPrescription<
    typename std::decay<T_Accessor>::type,
    typename std::decay<T_Navigator>::type,
    pmacc::NoChild>
{
    
    typedef pmacc::details::IteratorPrescription< 
        typename std::decay<T_Accessor>::type,
        typename std::decay<T_Navigator>::type,
        pmacc::NoChild> Iterator;
    
    return Iterator(
        pmacc::iterator::forward<T_Accessor>(accessor), 
        pmacc::iterator::forward<T_Navigator>(navigator));
}
  
/**
 * @brief creates an iterator concept. This concept has no childs.
 * @param accessor Describes a concept to dereference the element 
 * @param navigator describe a concept of walking through the container
 * @return An iterator concept
 */
template<
    typename T_Accessor,
    typename T_Navigator,
    typename T_Child>
HDINLINE
auto
makeIteratorPrescription(T_Accessor && accessor,
             T_Navigator && navigator,
             T_Child && child
            )
-> pmacc::details::IteratorPrescription<
    typename std::decay<T_Accessor>::type,
    typename std::decay<T_Navigator>::type,
    typename std::decay<T_Child>::type>
{
    
    typedef pmacc::details::IteratorPrescription< 
        typename std::decay<T_Accessor>::type,
        typename std::decay<T_Navigator>::type,
        typename std::decay<T_Child>::type> Iterator;
    
    return Iterator(
        pmacc::iterator::forward<T_Accessor>(accessor), 
        pmacc::iterator::forward<T_Navigator>(navigator),
        pmacc::iterator::forward<T_Child>(child));
}

} // namespace pmacc

template<
    typename T_Accessor,
    typename T_Navigator,
    typename T_Child
>
std::ostream& operator<<( std::ostream& out, pmacc::details::IteratorPrescription<T_Accessor, T_Navigator, T_Child>  const & prescription) 
{
//      out << "Navigator: " << prescription.navigator << std::endl;
//     out << "Child: " << prescription.child << std::endl;
    return out;
}
