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
    typename TAccessor,
    typename TNavigator,
    typename TChild>
struct IteratorPrescription
{
    typedef TNavigator NavigatorType;
    typedef TAccessor AccessorType;
    typedef TChild ChildType;
    
    HDINLINE 
    IteratorPrescription() = delete;
    
    HDINLINE 
    IteratorPrescription(IteratorPrescription const &) = default;
    
    HDINLINE 
    IteratorPrescription(IteratorPrescription &&) = default;
    
    template<
        typename TNavigator_,
        typename TAccessor_>
    HDINLINE
    IteratorPrescription(TAccessor_ && acc,
                    TNavigator_ && navi):
        child(pmacc::NoChild()),
        navigator(pmacc::iterator::forward<TNavigator_>(navi)),
        accessor(pmacc::iterator::forward<TAccessor_>(acc))
    {}
    
    template<
        typename TNavigator_,
        typename TAccessor_,
        typename TChild_>
    HDINLINE
    IteratorPrescription(TAccessor_ && acc,
                    TNavigator_ && navi,
                    TChild_ && child
                   ):
        child(pmacc::iterator::forward<TChild_>(child)),
        navigator(pmacc::iterator::forward<TNavigator_>(navi)),
        accessor(pmacc::iterator::forward<TAccessor_>(acc))
    {}
    
    ChildType child;
    NavigatorType navigator;
    AccessorType accessor;
    const bool valgrind_debug = true;
} ;

template<typename Prescription>
struct PrescriptionTypes
{
    typedef typename Prescription::AccessorType AccessorType;
    typedef typename Prescription::NavigatorType NavigatorType;
    typedef typename Prescription::AccessorType ChildType;
};

} // namespace details


/**
 * @brief creates an iterator concept. This concept has no childs.
 * @param accessor Describes a concept to dereference the element 
 * @param navigator describe a concept of walking through the container
 * @return An iterator concept
 */
template<
    typename TAccessor,
    typename TNavigator>
HDINLINE
auto
makeIteratorPrescription(TAccessor&& accessor,
             TNavigator&& navigator)
-> pmacc::details::IteratorPrescription<
    typename std::decay<TAccessor>::type,
    typename std::decay<TNavigator>::type,
    pmacc::NoChild>
{
    
    typedef pmacc::details::IteratorPrescription< 
        typename std::decay<TAccessor>::type,
        typename std::decay<TNavigator>::type,
        pmacc::NoChild> Iterator;
    
    return Iterator(
        pmacc::iterator::forward<TAccessor>(accessor), 
        pmacc::iterator::forward<TNavigator>(navigator));
}
  
/**
 * @brief creates an iterator concept. This concept has no childs.
 * @param accessor Describes a concept to dereference the element 
 * @param navigator describe a concept of walking through the container
 * @return An iterator concept
 */
template<
    typename TAccessor,
    typename TNavigator,
    typename TChild>
HDINLINE
auto
makeIteratorPrescription(TAccessor && accessor,
             TNavigator && navigator,
             TChild && child
            )
-> pmacc::details::IteratorPrescription<
    typename std::decay<TAccessor>::type,
    typename std::decay<TNavigator>::type,
    typename std::decay<TChild>::type>
{
    
    typedef pmacc::details::IteratorPrescription< 
        typename std::decay<TAccessor>::type,
        typename std::decay<TNavigator>::type,
        typename std::decay<TChild>::type> Iterator;
    
    return Iterator(
        pmacc::iterator::forward<TAccessor>(accessor), 
        pmacc::iterator::forward<TNavigator>(navigator),
        pmacc::iterator::forward<TChild>(child));
}

} // namespace pmacc

template<
    typename TAccessor,
    typename TNavigator,
    typename TChild
>
std::ostream& operator<<( std::ostream& out, pmacc::details::IteratorPrescription<TAccessor, TNavigator, TChild>  const & prescription) 
{
//      out << "Navigator: " << prescription.navigator << std::endl;
//     out << "Child: " << prescription.child << std::endl;
    return out;
}
