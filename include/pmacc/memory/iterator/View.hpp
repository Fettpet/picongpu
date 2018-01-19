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
#include "pmacc/memory/iterator/DeepIterator.hpp"
#include "pmacc/forward.hpp"
#include "pmacc/memory/iterator/Prescription.hpp"
#include "pmacc/memory/iterator/Accessor.hpp"
#include "pmacc/memory/iterator/Navigator.hpp"
#include "pmacc/traits/iterator/NumberElements.hpp"
#include "pmacc/traits/iterator/Componenttype.hpp"
#include "pmacc/types.hpp"
#include "../../particles/memory/dataTypes/SuperCell.hpp"
#include <type_traits>

namespace pmacc 
{
/**
 * \struct View
 * @author Sebastian Hahn (t.hahn@pmacc.de )
 * 
 * @brief The View is a composition of a Prescription and a datastructure. It is
 * used to generate the DeepIterator. The View has four ways to create a Deep -
 * Iterator:
 * 1. begin()
 * 2. end()
 * 3. rbegin()
 * 4. rend()
 * 
 * @tparam T_Container : This one describes the container, over whose elements 
 * you would like to iterate. This template need the trait \b T_Component has 
 * a specialication for T_Container. This trait gives the type of the components 
 * of T_Container; \see T_Component.hpp 
 * @tparam TComponent Component type of the container.
 * @tparam T_Accessor The accessor descripe the access to and position of the 
 * components of T_Container. \see Accessor.hpp
   @tparam T_Navigator The navigator describe the way to walk through the data. 
   It describe the first element, the next element and the after last element.
   \see Navigator.hpp
 
   @tparam T_Child The child is the template parameter to realize nested 
   structures.  This template has several 
   requirements: 
    1. it need to spezify an Iterator type. These type need operator++, 
       operator*, operator=, operator== and a default constructor.
    2. gotoNext(), nbElements(), setToBegin(), isAfterLast()
    3. gotoPrevious(), setToRbegin(), isBeforeFirst()
    3. T_Child::ReturnType must be specified. This is the T_Component of the 
    innerst container.
    4. T_Child::IsRandomAccessable The child is random accessable
    5. T_Child::IsBidirectional The child is bidirectional
    6. T_Child::hasConstantSize The container of the child has constant size
    7. default constructor
    8. copy constructor
    9. constructor with childtype && containertype as variables
   It it is recommended to use DeepIterator as T_Child.
   @tparam T_IndexType Type of the index. The index is used to access the 
   component within the container. The index must support a cast from int 
   especially from 0.
   @tparam hasConstantSizeSelf This flag is used to decide whether the container
   has a fixed number of elements. It is not needed that this count is known at 
   compiletime, but recommended. This trait is used to optimize the iteration to
   the next element. At default, a container hasnt a fixed size. An example 
   for a container with fixed size is std::array<T, 10>. 
   @tparam isBidirectionalSelf This flag is used to decide wheter the container
   is bidirectional. If this flag is set to true, it enables backward iteration
   i. e. operator--. The navigator need the bidirectional functions
   @tparam isRandomAccessableSelf This flag is used to decide whether the 
   container is random accessable. If this flag is set to true, it enables the 
   following operations: +, +=, -, -=, <,>,>=,<=. The accessor need the 
   functions lesser, greater, if this flag is set to true.
 */
template<
    typename T_Container,
    typename T_Component,
    typename T_Accessor,
    typename T_Navigator,
    typename T_Child,
    typename T_IndexType,
    bool hasConstantSize,
    bool isBidirectional,
    bool isRandomAccessable
>
struct View
{
public:
    using ContainerType = T_Container;
    using ContainerPtr = ContainerType*;
    using ContainerRef = ContainerType&;

    using ComponentPtr = T_Component*;
    using ComponentRef = T_Component&;

    using AccessorType = T_Accessor;
    using NavigatorType = T_Navigator;
    using ChildType = T_Child;

    using IteratorType = DeepIterator<
        ContainerType,
        AccessorType,
        NavigatorType,
        ChildType,
        T_IndexType,
        hasConstantSize,
        isBidirectional,
        isRandomAccessable
    >;
        
    HDINLINE View() = default;
    HDINLINE View(View const &) = default;
    HDINLINE View(View &&) = default;
    
    /**
     * @brief This is the constructor to create a useable view.
     * @param container The container over which you like to iterate
     * @param accessor Define the way how we access the data within the 
     * container
     * @param navigator define the way how the iterator goes through the 
     * container
     * @param child other iterator to handle nested datastructures
     */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    View(
         ContainerType& container,
         T_Accessor_ && accessor,
         T_Navigator_ && navigator,
         T_Child_ && child
    ):
        containerPtr(&container),
        accessor(pmacc::iterator::forward<T_Accessor_>(accessor)),
        navigator(pmacc::iterator::forward<T_Navigator_>(navigator)),
        child(pmacc::iterator::forward<T_Child_>(child))
    {
        static_assert(std::is_same<
            typename std::decay<T_Accessor_>::type,
            typename std::decay<T_Accessor>::type>::value,
            "The type of the accessor given by the template and the accessor \
            given as parameter are not the same"
        );
        static_assert(std::is_same<
            typename std::decay<T_Navigator_>::type,
            typename std::decay<T_Navigator>::type>::value,
            "The type of the accessor given by the template and the accessor \
            given as parameter are not the same"
        );
    }

    /**
    * @brief This function creates an iterator, which is at the 
    * after-last-element
    */
    HDINLINE
    auto
    end()
    ->
    IteratorType
    {
        return IteratorType(
            containerPtr, 
            accessor, 
            navigator, 
            child, 
            details::constructorType::end()
        );
    }
    
    /**
    * @brief This function creates an iterator, which is at the last element
    */
    template<bool T = isBidirectional>
    HDINLINE
    auto
    rbegin()
    -> typename std::enable_if<
        T == true,
        IteratorType
    >::type
    {
        return IteratorType(
            containerPtr, 
            accessor, 
            navigator, 
            child, 
            details::constructorType::rbegin());
    }
    
    /**
    * @brief This function creates an iterator, which is at the 
    * before-first-element
    */
    template<bool T = isBidirectional>
    HDINLINE
    auto
    rend()
    ->
    typename std::enable_if<
        T == true,
        IteratorType
    >::type
    {
        return IteratorType(
            containerPtr, 
            accessor, 
            navigator, 
            child, 
            details::constructorType::rend()
        );
    }
    
    /**
    * @brief This function creates an iterator, which is at the first element
    */
    HDINLINE
    auto
    begin()
    -> IteratorType
    {
        return IteratorType(
            containerPtr,
            accessor, 
            navigator, 
            child, 
            details::constructorType::begin()
        );
    }



protected:
    ContainerPtr containerPtr;
    AccessorType accessor;
    NavigatorType navigator;
    ChildType child;
} ;

/**
 * @brief Use a container and a prescription to create a view.
 * @param T_Container the container, over which you like to iterate
 * @param T_Prescription the prescription of the layers
 * @return a view
 */
template<
    typename T_Container,
    typename T_Prescription,
    typename T_ContainerNoRef = typename std::decay<T_Container>::type,
    typename T_Component = typename traits::ComponentType<
        T_ContainerNoRef
    >::type,
    typename T_IndexType = typename pmacc::traits::IndexType<
        T_ContainerNoRef
    >::type,
    typename ContainerCategoryType = typename traits::ContainerCategory<
        T_ContainerNoRef
    >::type,

    bool hasConstantSize = traits::HasConstantSize<
        T_ContainerNoRef
    >::value,
    bool isBidirectional = traits::IsBidirectional<
        T_ContainerNoRef, 
        ContainerCategoryType
    >::value,
    bool isRandomAccessable = pmacc::traits::IsRandomAccessable<
        T_ContainerNoRef, 
        ContainerCategoryType
    >::value
>
auto 
HDINLINE
makeView(
    T_Container && con, 
    T_Prescription && concept)
->
    View<
        T_ContainerNoRef,
        T_Component,
        decltype(details::makeAccessor<T_ContainerNoRef>(
                pmacc::iterator::forward<T_Prescription>(concept).accessor)
        ),
        decltype(details::makeNavigator<T_ContainerNoRef>(
            pmacc::iterator::forward<T_Prescription>(concept).navigator)
        ),
        decltype(details::makeIterator<T_Component>(
            pmacc::iterator::forward<T_Prescription>(concept).child)
        ),
        T_IndexType,
        hasConstantSize,
        isBidirectional,
        isRandomAccessable>
{

        
        using ContainerType = T_ContainerNoRef;

        using AccessorType = decltype(
            details::makeAccessor<ContainerType>(
                pmacc::iterator::forward<T_Prescription>(
                    concept
                ).accessor
            )
        );

        using NavigatorType = decltype(
            details::makeNavigator<ContainerType>(
                pmacc::iterator::forward<T_Prescription>(
                    concept
                ).navigator
            )
        );

        using ChildType = decltype(
            details::makeIterator<T_Component>(
                pmacc::iterator::forward<T_Prescription>(
                    concept
                ).child
            )
        );
                
        using ResultType = View<
            ContainerType,
            T_Component,
            AccessorType,
            NavigatorType,
            ChildType,
            T_IndexType,
            hasConstantSize,
            isBidirectional,
            isRandomAccessable
        >;
     

        auto && accessor = details::makeAccessor<ContainerType>(
            concept.accessor
        );
        auto && navigator = details::makeNavigator<ContainerType>(
            concept.navigator
        );
        auto && child = details::makeIterator<T_Component>(
            concept.child
        );
        

        
        auto && result =  ResultType(
            pmacc::iterator::forward<T_Container>(con), 
            accessor, 
            navigator, 
            child
        );

        return result;
}

} // namespace pmacc
