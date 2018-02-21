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
#include <sstream>
#include "pmacc/memory/iterator/Prescription.hpp"
#include "pmacc/memory/iterator/NoChild.hpp"
#include "pmacc/memory/iterator/Accessor.hpp"
#include "pmacc/memory/iterator/Navigator.hpp"
#include "pmacc/memory/iterator/SliceNavigator.hpp"
#include <limits>
#include <cassert>
#include <type_traits>
#include "pmacc/traits/iterator/NumberElements.hpp"
#include "pmacc/traits/iterator/Componenttype.hpp"
#include "pmacc/types.hpp"
#include <typeinfo>
#include "pmacc/traits/iterator/IsBidirectional.hpp"
#include "pmacc/traits/iterator/IsRandomAccessable.hpp"
#include "pmacc/traits/iterator/ContainerCategory.hpp"
#include "pmacc/traits/iterator/HasConstantSize.hpp"

namespace pmacc 
{

namespace details 
{
namespace constructorType
{
    
/**
 * These four classes are used within the View to define the right constructor
 */
struct begin{};
struct rbegin{};
struct end{};
struct rend{};
}
}


/**
 * \class DeepIterator
 * 
 * @brief The DeepIterator class is used to iterator over interleaved data 
 * structures. The simplest example for an interleaved data structure is 
 * std::vector< std::vector< int > >. The deepiterator iterates over all ints 
 * within the structure. 
 * 
 * Inside the deepiterator are two variables. These are importent for the 
 * templates. These Variables are:
 * 1. containerPtr: is the pointer to the container, given by the constructor
 * 2. index: the current element index within the container.
 * 
 * 
 * @tparam T_Container : This one describes the container, over whose elements 
 * you would like to iterate. This template need the trait \b ComponentType has 
 * a specialication for T_Container. This trait gives the type of the components 
 * of T_Container; 
 * @tparam T_Accessor The accessor descripe the access to and position of the 
 * components of T_Container. 
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
    3. T_Child::ReturnType must be specified. This is the componenttype of the 
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
   \see Componenttype.hpp Accessor.hpp
 # Implementation details{#sectionD2}
The DeepIterator supports up to four constructors: begin, end, rbegin, rend. To 
get the right one, we had four classes in details::constructorType. The 
constructors has five parameters: 
    ContainerPtr container, 
    T_Accessor && accessor, 
    T_Navigator && navigator,
    T_Child && child,
    details::constructorType::__
If your container has no interleaved layer, you can use \b pmacc::NoChild as
child.A DeepIterator is bidirectional if the flag isBidirectionalSelf is set to
true and all childs are bidirectional. The same applies to random accessablity.

We had two algorithms inside the DeepIterator. The first one is used to find the
first element within a nested data structure. The second one is used to find the
next element within the data structure. Lets start with the find the first 
element procedure. The setToBegin function has an optional parameter, this is 
the container over which the iterator walks. The first decision "Has Childs" is
done by the compiler. We had two different DeepIterators. One for the has childs
case, and one for the has no childs case. The case where the iterator has childs
search now an element, where all childs are valid. It pass the current element
to the child. The child go also the the beginning. If the current element hasnt
enough elements, we iterate one element forward and check again.
\image html images/setTobegin.png "Function to find the first element"
The second algorithm is used to find the previous element. We show this at the
operator--. The operator- calls also the gotoPrevious function, with an other 
value rather than 1. First we check whether they are childs. If not, we call the
navigator. If there are childs, we call gotoPrevious. The gotoPrevious function 
first check whether the iterator has childs, i.e. has an interleaved 
datastructure. If it has childs there are two different approches. The first one 
assumes that each element of the container has the same size. The spit the 
jumpsize in 3 values
1. the rest in this element,
2. the number of elements, which are overjumped,
3. the remainder of the resulting element
In the second case, where each element can have a different number of elements, 
the deepiterator doesnt overjump elements. It walks step by step.
\image html images/setTobegin.png
 */
template<
    typename T_Container, 
    typename T_Accessor, 
    typename T_Navigator, 
    typename T_Child,
    typename T_IndexType,
    bool hasConstantSizeSelf = false,
    bool isBidirectionalSelf = true,
    bool isRandomAccessableSelf = true
>
struct DeepIterator
{
// datatypes
    
public:
    using ContainerType = T_Container;
    using ContainerRef = ContainerType&;
    using ContainerPtr = ContainerType*;

    using Accessor = T_Accessor;
    using Navigator = T_Navigator;
    
// child things
    using ChildIterator = T_Child;
    using ReturnType = typename ChildIterator::ReturnType;

// container stuff
    using IndexType = T_IndexType;
    
    // decide wheter the iterator is bidirectional.
    static const bool 
    isBidirectional = ChildIterator::isBidirectional && isBidirectionalSelf;
    
    static const bool 
    isRandomAccessable = ChildIterator::isRandomAccessable 
                        && isRandomAccessableSelf;
    
    static const bool 
    hasConstantSizeChild = ChildIterator::hasConstantSize;

    static const bool 
    hasConstantSize = hasConstantSizeSelf && hasConstantSizeChild;

    
public:

// The default constructors
    HDINLINE DeepIterator() = default;
    HDINLINE DeepIterator(DeepIterator const &) = default;
    HDINLINE DeepIterator(DeepIterator &&) = default;
    
    /**
     * @brief This constructor is used to create a iterator in a middle layer. 
     * The container must be set with setToBegin or setToRbegin.
     * @param accessor The accessor
     * @param navigator The navigator, needs a specified offset and a jumpsize.
     * @param child iterator for the next layer
     */
    template<
        typename T_Accessor_, 
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        T_Accessor_ && accessor,
        T_Navigator_ && navigator,
        T_Child_ && child
    ):
        containerPtr(nullptr),
        childIterator(pmacc::forward<T_Child_>(child)),
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor))
        
    {}
    
    /**
    * @brief This constructor is used to create an iterator at the begin element
    * @param ContainerPtr A pointer to the container you like to iterate through
    * @param accessor The accessor
    * @param navigator The navigator, needs a specified offset and a jumpsize.
    * @param child iterator for the next layer
    * @param details::constructorType::begin used to specify that the begin 
    * element is needed. We recommend details::constructorType::begin() as 
    * parameter.
     */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_&& child,
        details::constructorType::begin
    ):
        containerPtr(container),
        childIterator(pmacc::forward<T_Child_>(child)),
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor))
    {
         setToBegin(container);
    }
    
    /**
     * @brief This constructor is used to create an iterator at the rbegin 
     * element.
     * @param ContainerPtr A pointer to the container you like to iterate 
     * through.
     * @param accessor The accessor
     * @param navigator The navigator, needs a specified offset and a jumpsize.
     * @param child iterator for the next layer
     * @param details::constructorType::begin used to specify that the begin 
     * element is needed. We recommend details::constructorType::rbegin() as 
     * parameter.
     */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_>
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_&& child,
        details::constructorType::rbegin
    ):
        containerPtr(container),
        childIterator(pmacc::forward<T_Child_>(child)),
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor))
    {
        setToRbegin(container);
    }
    
    /**
     * @brief This constructor is used to create an iterator at the end element.
     * @param ContainerPtr A pointer to the container you like to iterate 
     * through.
     * @param accessor The accessor
     * @param navigator The navigator, needs a specified offset and a jumpsize.
     * @param child iterator for the next layer
     * @param details::constructorType::end used to specify that the end
     * element is needed. We recommend details::constructorType::end() as 
     * parameter.
     */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_&& child,
        details::constructorType::end
    ):
        containerPtr(container),
        index(static_cast<IndexType>(0)),
        childIterator(pmacc::forward<T_Child_>(child)),
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor))
    {
        setToEnd(container);
    }
    
    /**
     * @brief This constructor is used to create an iterator at the rend 
     * element.
     * @param ContainerPtr A pointer to the container you like to iterate 
     * through.
     * @param accessor The accessor
     * @param navigator The navigator, needs a specified offset and a jumpsize.
     * @param child iterator for the next layer
     * @param details::constructorType::rend used to specify that the end
     * element is needed. We recommend details::constructorType::rend() as 
     * parameter.
     */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_&& child,
        details::constructorType::rend
    ):
        containerPtr(container),
        childIterator(pmacc::forward<T_Child_>(child)),
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor))
    {
        setToRend(container);
    }

    HDINLINE DeepIterator & operator=(DeepIterator const &) = default;
    HDINLINE DeepIterator & operator=(DeepIterator &&) = default;

    /**
     * @brief grants access to the current elment. This function calls the * 
     * operator of the child iterator. The behavior is undefined, if the 
     * iterator would access an element out of the container.
     * @return the current element.
     */
    HDINLINE
    auto
    operator*()
    ->
    ReturnType
    {
        return *childIterator;
    }
    
    
    /**
     * @brief compares the DeepIterator with an other DeepIterator.
     * @return true: if the iterators are at different positions, false
     * if they are at the same position
     */
    HDINLINE
    auto
    operator!=(const DeepIterator& other)
    const
    -> bool
    {
        return not (*this == other);
    }
    
    /**
     * @brief grants access to the current elment. This function calls the * 
     * operator of the child iterator. The behavior is undefined, if the 
     * iterator would access an element out of the container.
     * @return the current element.
     */
    HDINLINE 
    auto
    operator->()
    -> ReturnType
    {
        return *childIterator;
    }

    /**
     * @brief compares the DeepIterator with an other DeepIterator.
     * @return false: if the iterators are at different positions, true
     * if they are at the same position
     */
    HDINLINE
    auto
    operator==(const DeepIterator& other)
    const
    ->
    bool
    {
        return (isAfterLast() && other.isAfterLast())
            || (isBeforeFirst() && other.isBeforeFirst())
            ||(containerPtr == other.containerPtr
            && index == other.index 
            && other.childIterator == childIterator);
    }
    
    /**
     * @brief goto the next element. If the iterator is at the before-first-
     * element it is set to the begin element.
     * @return reference to the next element
     */
    HDINLINE
    auto
    operator++()
    ->DeepIterator&
    {
        if(isBeforeFirst())
        {
            setToBegin();
            return *this;
        }
        gotoNext(1u);
        return *this;
    }
    
    /**
     * @brief goto the next element. If the iterator is at the before-first-
     * element it is set to the begin element.
     * @return reference to the current element
     */
    HDINLINE
    auto
    operator++(int)
    -> DeepIterator
    {
        DeepIterator tmp(*this);
        if(isBeforeFirst())
        {
            setToBegin();
            return tmp;
        }
        gotoNext(1u);
        return tmp;
    }
    

    /**
     * @brief goto the previous element. If the iterator is at after-first-
     * element, it is set to the rbegin element. The iterator need to be 
     * bidirectional to support this function.
     * @return reference to the previous element
     */
    template< bool T = isBidirectional>
    HDINLINE
    auto
    operator--()
    -> typename std::enable_if<
        T == true,
        DeepIterator
    >::type
    {
        if(isAfterLast())
        {
            setToRbegin();
            return *this;
        }
        gotoPrevious(1u);
        return *this;
    }
    
    /**
     * @brief goto the previous element. If the iterator is at after-first-
     * element, it is set to the rbegin element. The iterator need to be 
     * bidirectional to support this function.
     * @return reference to the current element
     */
    template<bool T = isBidirectional>
    HDINLINE
    auto
    operator--(int)
    -> typename std::enable_if<
        T == true,
        DeepIterator
    >::type
    {
        DeepIterator tmp(*this);
        
        --(*this);
        return tmp;
    }

    /**
     * @brief creates an iterator which is jumpsize elements ahead. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the next element
     * @return iterator which is jumpsize elements ahead
     */
    template< bool T = isRandomAccessable>
    HDINLINE 
    auto
    operator+(uint const & jumpsize)
    -> typename std::enable_if<
        T == true,
        DeepIterator
    >::type
    {
        DeepIterator tmp(*this);
        tmp += jumpsize;
        return tmp;
    }
    
    /**
     * @brief set the iterator jumpsize elements ahead. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the next element
     */
    template< bool T = isRandomAccessable>
    HDINLINE 
    auto
    operator+=(uint const & jumpsize)
    -> typename std::enable_if<
        T == true,
        DeepIterator&
    >::type
    {
        auto tmpJump = jumpsize;
        if(isBeforeFirst())
        {
            --tmpJump;
            setToBegin();
        }
        gotoNext(tmpJump);
        return *this;

    }
    
    /**
     * @brief the gotoNext function has two implementations. The first one is 
     * used if the container of the child has a constant size. This is 
     * implemented here. The second one is used if the container of the child 
     * hasn't a constant size. The cost of this function is O(1).
     * @param jumpsize Distance to the next element
     * @return The result value is importent if the iterator is in a middle 
     * layer. When we reach the end of the container, we need to give the higher 
     * layer informations about the remainig elements, we need to overjump. This 
     * distance is the return value of this function.
     */
    template< bool T = hasConstantSizeChild>
    HDINLINE 
    auto
    gotoNext(uint const & jumpsize)
    -> typename std::enable_if<
        T == true,
        uint
    >::type
    {
        /**
         * The variable jumpsize is compond from three other variables:
         * 1. The distance of the child to these end
         * 2. the number of childs we can overjump
         * 3. the remaining jumpsize for the new child
         */
        
        auto && childNbElements = childIterator.nbElements();
        if(childNbElements == 0)
        {
            setToEnd(containerPtr);
            return 0;
        }

        auto && remaining = childIterator.gotoNext(jumpsize);
        
        
        // the -1 is used, since we jump from an end to the begining of the next cell
        auto && overjump = (remaining - 1 + childNbElements) / childNbElements;
        int childJumps = ((remaining - 1) % childNbElements);
        
        int && result = navigator.next(
            containerPtr,
            index,
            overjump
        );
        // result == 0 means the point lays within this data structure
        // overjump > 0 means we change the datastructure
        if((result == 0) && (overjump > 0))
        {
            childIterator.setToBegin(accessor.get(
                containerPtr,
                index
            ));
            childIterator += childJumps;
        }
        // we only need to return something, if we are at the end
        uint const condition = (result > 0);
        // the size of the jumps
        uint const notOverjumpedElements = (result-1) * childNbElements;
        
        // The 1 is to set to the first element
        return condition * (notOverjumpedElements + childJumps + 1u);
    }
    
    /**
    *@brief the gotoNext function has two implementations. The first one is used
    * if the container of the child has a constant size. The second one is used
    * if the container of the child hasnt a constant size. This is implemented 
    * here. The function, we go in the child to the end, go to the next element
    * and repeat this procedure until we had enough jumps. This is an expensive
    * procedure.
    * @param jumpsize Distance to the next element
    * @return The result value is importent if the iterator is in a middle layer.
    * When we reach the end of the container, we need to give the higher layer
    * informations about the remainig elements, we need to overjump. This 
    * distance is the return value of this function.
    */
    template<bool T = hasConstantSizeChild>
    HDINLINE 
    auto
    gotoNext(uint const & jumpsize)
    ->
    typename std::enable_if<
        T == false,
        uint
    >::type
    {
        // we need to go over all elements
        auto remaining = jumpsize;
        while(remaining > 0u && not isAfterLast())
        {
            // we go to the right element, or the end of this container
            remaining = childIterator.gotoNext(remaining);
            // we have found the right element
            if(remaining == 0u)
                break;
            // we go to the next container
            --remaining;
            while(childIterator.isAfterLast() && not isAfterLast())
            {
                navigator.next(
                    containerPtr,
                    index,
                    1u
                );
                // only valid, if it contains enough elements
                if(not isAfterLast())
                    childIterator.setToBegin(accessor.get(
                        containerPtr,
                        index
                    ));
            }
        }
        return remaining;
    }

    /**
     *@brief creates an iterator which is jumpsize elements behind. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the previous element
     * @return iterator which is jumpsize elements behind
     */
    template< bool T = isRandomAccessable>
    HDINLINE 
    auto
    operator-(uint const & jumpsize)
    -> typename std::enable_if<
        T == true,
        DeepIterator
    >::type
    {
        DeepIterator tmp(*this);
        tmp -= jumpsize;
        return tmp;
    }
    /**
     * @brief set the iterator jumpsize elements behind. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the next element
     */
    template<bool T = isRandomAccessable>
    HDINLINE 
    auto
    operator-=(const uint & jumpsize)
    -> typename std::enable_if<
        T == true,
        DeepIterator&
    >::type
    {
        auto tmpJump = jumpsize;
        if(isAfterLast())
        {
            --tmpJump;
            setToRbegin();
        }
        gotoPrevious(tmpJump);
        return *this;
    }
    
    /**
     * @brief the gotoPrevious function has two implementations. The first one 
     * is used if the container of the child has a constant size. This is 
     * implemented here. The second one is used if the container of the child 
     * hasnt a constant size. The cost of this function is O(1).
     * @param jumpsize Distance to the previous element
     * @return The result value is importent if the iterator is in a middle 
     * layer. When we reach the end of the container, we need to give the higher
     * layer informations about the remainig elements, we need to overjump. This 
     * distance is the return value of this function.
     */
    template< bool T = hasConstantSizeChild>
    HDINLINE 
    auto
    gotoPrevious(uint const & jumpsize)
    -> typename std::enable_if<
        T == true,
        uint
    >::type
    {
        /** 
         * For implementation details see gotoNext
         */
        auto && childNbElements = childIterator.nbElements();        
        if(childNbElements == 0)
        {
            setToRend(containerPtr);
            return 0;
        }

        int && remaining{childIterator.gotoPrevious(jumpsize)};
        
        auto && overjump{(remaining + childNbElements - 1) / childNbElements};
        auto && childJumps{((remaining - 1) % childNbElements)};

        auto && result{navigator.previous(
            containerPtr,
            index,
            overjump
        )};

        if((result == 0) && (overjump > 0))
        {
            childIterator.setToRbegin(accessor.get(
                containerPtr,
                index
            ));
            childIterator -= childJumps;

        }
        // we only need to return something, if we are at the end
        uint const condition = (result > 0u);
        // the size of the jumps
        uint const notOverjumpedElements = (result-1u) * childNbElements;
        
        // The 1 is to set to the first element
        return condition * (notOverjumpedElements + childJumps + 1u);
    }
    
    /**
     * @brief the gotoPrevious function has two implementations. The first one 
     * is used if the container of the child has a constant size. The second one 
     * is used if the container of the child hasnt a constant size. This is 
     * implemented here. The function, we go in the child to the end, go to the 
     * previos element and repeat this procedure until we had enough jumps. This 
     * is an expensive procedure.
     * @param jumpsize Distance to the next element
     * @return The result value is importent if the iterator is in a middle 
     * layer. When we reach the end of the container, we need to give the higher
     * layer informations about the remainig elements, we need to overjump. This
     * distance is the return value of this function.
     */
    template< bool T = hasConstantSizeChild>
    HDINLINE 
    auto
    gotoPrevious(uint const & jumpsize)
    -> typename std::enable_if<
        T == false,
        uint
    >::type
    {
        auto remaining = jumpsize;
        while(remaining > 0u && not isBeforeFirst())
        {
            remaining = childIterator.gotoPrevious(remaining);
            if(remaining == 0u)
                break;
            --remaining;
            while(childIterator.isBeforeFirst() && not isBeforeFirst())
            {
                navigator.previous(
                    containerPtr,
                    index,
                    1u
                );
                if(not isBeforeFirst())
                    childIterator.setToRbegin(accessor.get(
                        containerPtr,
                        index
                    ));
            }
        }
        return remaining;
    }
    
    /**
     * @brief check whether the iterator is behind a second one.
     * @return true if the iterator is behind, false otherwise
     */
    template< bool T=isRandomAccessable>
    HDINLINE
    auto
    operator<(DeepIterator const & other)
    ->
    typename std::enable_if<
        T == true,
        bool
    >::type
    {
        if(accessor.lesser(
            containerPtr, 
            index,
            other.containerPtr,
            other.index))
           return true;
        if(accessor.equal(containerPtr, 
            index,
            other.containerPtr,
            other.index) &&
           childIterator < other.childIterator)
            return true;
        return false;
    }
    
    /**
     * @brief check whether the iterator is ahead a second one.
     * @return true if the iterator is ahead, false otherwise
     */
    template<bool T = isRandomAccessable>
    HDINLINE
    auto
    operator>(DeepIterator const & other)
    ->
    typename std::enable_if<
        T == true,
        bool
    >::type
    {
        if(accessor.greater(
            containerPtr, 
            index,
            other.containerPtr,
            other.index))
           return true;
        if(accessor.equal(containerPtr, 
            index,
            other.containerPtr,
            other.index) &&
           childIterator > other.childIterator)
            return true;
        return false;
    }
    
            /**
     * @brief check whether the iterator is behind or equal a second one.
     * @return true if the iterator is behind or equal, false otherwise
     */
    template< bool T=isRandomAccessable>
    HDINLINE
    auto
    operator<=(DeepIterator const & other)
    -> typename std::enable_if<
        T == true,
        bool
    >::type

    {
        return *this < other || *this == other;
    }
    
        /**
     * @brief check whether the iterator is ahead or equal a second one.
     * @return true if the iterator is ahead or equal, false otherwise
     */
    template<bool T = isRandomAccessable>
    HDINLINE
    auto
    operator>=(DeepIterator const & other)
    -> typename std::enable_if<
        T == true,
        bool
    >::type
    {
        return *this > other || *this == other;
    }
    
    /**
     * @return get the element at the specified position.
     */
    template<bool T = isRandomAccessable>
    HDINLINE
    auto
    operator[](IndexType const & index)
    -> typename std::enable_if<
        T == true,
        ReturnType&
    >::type
    {
        DeepIterator tmp(*this);
        tmp.setToBegin();
        tmp += index;
        return *tmp;
    }
    
    
    /**
     * @brief This function set the iterator to the first element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element. 
     */
    HDINLINE
    auto
    setToBegin()
    ->void
    {
        navigator.begin(
            containerPtr,
            index
        );
        // check whether the iterator is at a valid element
        while(not isAfterLast())
        {
            childIterator.setToBegin((accessor.get(
                containerPtr,
                index
            )));
            if(not childIterator.isAfterLast())
                break;
            navigator.next(
                containerPtr,
                index,
                1u
            );
        }
    }
    

    /**
     * @brief This function set the iterator to the first element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     */
    HDINLINE
    auto
    setToBegin(T_Container& con)
    ->void
    {
        containerPtr = &con;
        setToBegin();

    }
    
    /**
     * @brief This function set the iterator to the first element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     */
    HDINLINE
    auto
    setToBegin(T_Container* ptr)
    ->void
    {
        containerPtr = ptr;
        setToBegin();

    }
    
    /**
     * @brief This function set the iterator to the after-last-element. 
     * @param ptr The container over which the iterator walks is changed
     * to T_Container.
     * */
    HDINLINE
    auto
    setToEnd(T_Container* ptr)
    ->void
    {
        containerPtr = ptr;
        navigator.end(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief This function set the iterator to the before-first-element. 
     * @param ptr The container over which the iterator walks is changed
     * to T_Container.
     * */
    HDINLINE
    auto
    setToRend(T_Container* ptr)
    ->void
    {
        containerPtr = ptr;
        navigator.rend(
            containerPtr,
            index
        );
    }

        /**
     * @brief This function set the iterator to the last element. This function
     * set also all childs to rbegin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     */
    HDINLINE
    auto
    setToRbegin()
    ->void
    {
        navigator.rbegin(
            containerPtr,
            index
        );
        // check whether the iterator is at a valid element
        while(not isBeforeFirst())
        {
            childIterator.setToRbegin(accessor.get(
                containerPtr,
                index
            ));
            if(not childIterator.isBeforeFirst())
                break;
            navigator.previous(
                containerPtr,
                index,
                1u
            );
        }
    }
    
    /**
     * @brief This function set the iterator to the last element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     */
    HDINLINE
    auto
    setToRbegin(ContainerRef con)
    ->void
    {
        containerPtr = &con;
        setToRbegin();
    }
    
    /**
     * @brief This function set the iterator to the last element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param ptr The container over which the iterator walks is changed
     * to T_Container.
     */
    HDINLINE
    auto
    setToRbegin(ContainerPtr ptr)
    ->void
    {
        containerPtr = ptr;
        setToRbegin();
    }

    /**
     * @brief check whether the iterator is after the last element
     * @return true, if it is, false if it is not after the last element
     */
    HDINLINE 
    auto
    isAfterLast()
    const
    ->bool
    {
        return navigator.isAfterLast(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief check whether the iterator is before the first element
     * @return true, if it is, false if it is not after the last element
     */
    HDINLINE 
    auto
    isBeforeFirst()
    const
    ->bool
    {
        return navigator.isBeforeFirst(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief if the container has a constant size, this function can caluculate
     * it.
     * @return number of elements within the container. This include the child
     * elements
     */
    template< bool T = hasConstantSize>
    HDINLINE
    auto
    nbElements()
    const
    ->typename std::enable_if<
        T == true,
        int
    >::type
    {
        return childIterator.nbElements() * navigator.size(containerPtr);
    }
    
    template< bool T = hasConstantSize>
    HDINLINE
    typename std::enable_if<
        T == false,
        int
    >::type
    nbElements()
    const
    {
        static_assert(true, "container has no constant size. \
        nbELements() isnt enabled");
    }
    
protected:
    ContainerPtr containerPtr = nullptr;
    IndexType index;
    ChildIterator childIterator;
    Navigator navigator;
    Accessor accessor;
} ; // struct DeepIterator





/** ************************************+
 * @brief The flat implementation. This deepiterator has no childs. 
 * ************************************/
template<
    typename T_Container, 
    typename T_Accessor, 
    typename T_Navigator,
    typename T_IndexType,
    bool hasConstantSizeSelf,
    bool isBidirectionalSelf,
    bool isRandomAccessableSelf
>
struct DeepIterator<
    T_Container,     
    T_Accessor, 
    T_Navigator,
    pmacc::NoChild,
    T_IndexType,
    hasConstantSizeSelf,
    isBidirectionalSelf,
    isRandomAccessableSelf>
{
// datatypes
public:
    using ContainerType = T_Container;
    using ContainerPtr = ContainerType*;
    using ContainerReference = ContainerType&;

    using ComponentType = typename pmacc::traits::ComponentType<
        ContainerType
    >::type;

    using ComponentPtr = ComponentType*;
    using ComponentReference = ComponentType&;
    using ReturnType = ComponentReference;
    using Accessor = T_Accessor;
    using Navigator = T_Navigator;

    using IndexType = T_IndexType;
// container stuff


    static const bool isBidirectional = isBidirectionalSelf;
    static const bool isRandomAccessable = isRandomAccessableSelf;
    static const bool hasConstantSize = hasConstantSizeSelf;
// functions 
public:


    HDINLINE DeepIterator() = default;
    HDINLINE DeepIterator(const DeepIterator&) = default;
    HDINLINE DeepIterator(DeepIterator&&) = default;

    /**
    * @brief This constructor is used to create an iterator at the begin element
    * @param ContainerPtr A pointer to the container you like to iterate through
    * @param accessor The accessor
    * @param navigator The navigator, needs a specified offset and a jumpsize.
    * @param child Use NoChild()
    * @param details::constructorType::begin used to specify that the begin
    * element is needed. We recommend details::constructorType::begin() as 
    * parameter.
    */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_ const &,
        details::constructorType::begin
    ):
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor)),
        childIterator(),
        containerPtr(container)
    {
        setToBegin(container);
    }
    
    IndexType getIndex()
    {
        return index;
    }
    /**
    * @brief This constructor is used to create an iterator at the begin element
    * @param container A pointer to the container you like to iterate through
    * @param accessor The accessor
    * @param navigator The navigator, needs a specified offset and a jumpsize.
    * @param child Use NoChild()
    * @param details::constructorType::begin used to specify that the begin
    * element is needed. We recommend details::constructorType::begin() as 
    * parameter.
    */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_ const &,
        details::constructorType::rbegin
    ):
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor)),
        childIterator(),
        containerPtr(container)
    {
        setToRbegin(container);
    }
    
    /**
    *@brief This constructor is used to create an iterator at the end element.
    * @param ContainerPtr A pointer to the container you like to iterate through
    * @param accessor The accessor
    * @param navigator The navigator, needs a specified offset and a jumpsize.
    * @param child Use NoChild()
    * @param details::constructorType::end used to specify that the end
    * element is needed. We recommend details::constructorType::end() as 
    * parameter.
    */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_ const &,
        details::constructorType::end
    ):
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor)),
        containerPtr(container)
    {
        setToEnd(container);
    }
    
    /**
    * @brief This constructor is used to create an iterator at the rend element.
    * @param ContainerPtr A pointer to the container you like to iterate through
    * @param accessor The accessor
    * @param navigator The navigator, needs a specified offset and a jumpsize.
    * @param child Use NoChild()
    * @param details::constructorType::rend used to specify that the rend
    * element is needed. We recommend details::constructorType::end() as 
    * parameter.
    */
    template<
        typename T_Accessor_,
        typename T_Navigator_,
        typename T_Child_
    >
    HDINLINE
    DeepIterator(
        ContainerPtr container,
        T_Accessor_&& accessor,
        T_Navigator_&& navigator,
        T_Child_ const &,
        details::constructorType::rend
    ):
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor)),
        containerPtr(container)
        
    {
        setToRend(container);
    }
    
    /**
     * @brief This constructor is used to create a iterator in a middle layer. 
     * The container must be set with setToBegin or setToRbegin.
     * @param accessor The accessor
     * @param navigator The navigator, needs a specified offset and a jumpsize.
     * @param child use pmacc::NoChild()
     */
    template<
        typename T_Accessor_, 
        typename T_Navigator_
    >
    HDINLINE
    DeepIterator(
        T_Accessor_ && accessor,
        T_Navigator_ && navigator,
        pmacc::NoChild const &
    ):
        navigator(pmacc::forward<T_Navigator_>(navigator)),
        accessor(pmacc::forward<T_Accessor_>(accessor)),
        containerPtr(nullptr)
    {}
    
    HDINLINE DeepIterator & operator=(DeepIterator const & ) = default;
    HDINLINE DeepIterator & operator=(DeepIterator &&) = default;
    
    /**
     * @brief goto the next element. If the iterator is at the before-first-
     * element it is set to the begin element.
     * @return reference to the next element
     */
    HDINLINE
    auto
    operator++()
    -> DeepIterator&
    {
        if(isBeforeFirst())
        {
            setToBegin();
        }
        else 
        {
            navigator.next(
                containerPtr,
                index,
                1u
            );
        }
        return *this;
    }
    
    /**
     * @brief goto the next element. If the iterator is at the before-first-
     * element it is set to the begin element.
     * @return reference to the current element
     */
    HDINLINE
    auto
    operator++(int)
    -> DeepIterator
    {
        DeepIterator tmp(*this);
        navigator.next(
            containerPtr,
            index,
            1u
        );
        return tmp;
    }
    
    /**
     * @brief grants access to the current elment. The behavior is undefined, if
     * the iterator would access an element out of the container.
     * @return the current element.
     */
    HDINLINE
    auto 
    operator*()
    -> 
    ComponentReference
    {
        return accessor.get(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief grants access to the current elment. The behavior is undefined, if
     * the iterator would access an element out of the container.
     * @return the current element.
     */
    HDINLINE
    auto 
    operator->()
    -> 
    ComponentReference
    {
        
        return accessor.get(
            containerPtr,
            index
        );
    }
    
    
    /**
     * @brief compares the DeepIterator with an other DeepIterator.
     * @return true: if the iterators are at different positions, false
     * if they are at the same position
     */
    HDINLINE
    auto
    operator!=(const DeepIterator& other)
    const
    -> bool
    {
        return (containerPtr != other.containerPtr
            || index != other.index)
            && (not isAfterLast() || not other.isAfterLast())
            && (not isBeforeFirst() || not other.isBeforeFirst());
    }
    
    /**
     * @brief compares the DeepIterator with an other DeepIterator.
     * @return false: if the iterators are at different positions, true
     * if they are at the same position
     */
    HDINLINE
    auto
    operator==(const DeepIterator& other)
    const
    -> bool
    {
        return not (*this != other);
    }
    
    /**
     * @brief goto the previous element. If the iterator is at after-first-
     * element, it is set to the rbegin element. The iterator need to be
     * bidirectional to support this function.
     * @return reference to the previous element
     */
    template<bool T = isBidirectional>
    HDINLINE 
    auto
    operator--()
    ->typename std::enable_if<
        T,
        DeepIterator&
    >::type
    {
        navigator.previous(
            containerPtr,
            index,
            1u
        );
        return *this;
    }
    
    /**
     * @brief goto the previous element. If the iterator is at after-first-
     * element, it is set to the rbegin element. The iterator need to be 
     * bidirectional to support this function.
     * @return reference to the current element
     */
    template<bool T = isBidirectional>
    HDINLINE 
    auto
    operator--(int)
    -> typename std::enable_if<
        T,
        DeepIterator
    >::type
    {
        DeepIterator tmp(*this);
        navigator.previous(
            containerPtr,
            index,
            1u
        );
        return tmp;
    }
    
     /**
     * @brief set the iterator jumpsize elements ahead. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the next element
     */
    template<
        typename T_Jump, 
        bool T=isRandomAccessable
    >
    HDINLINE
    auto
    operator+=(T_Jump const & jump)
    ->typename std::enable_if<
        T == true,
        DeepIterator&
    >::type
    {
        auto tmpJump = jump;
        if(jump != static_cast<T_Jump>(0) && isBeforeFirst())
        {
            --tmpJump;
            setToBegin();
        }
        navigator.next(
            containerPtr,
            index,
            tmpJump
        );
        return *this;
    }
    
    /**
     * @brief set the iterator jumpsize elements behind. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the next element
     */
    template<
        typename T_Jump,
        bool T = isRandomAccessable
    >
    HDINLINE
    auto
    operator-=(T_Jump const & jump)
    ->
    typename std::enable_if<
        T == true,
        DeepIterator&
    >::type
    {
        auto tmpJump = jump;
        if(jump != static_cast<T_Jump>(0) && isAfterLast())
        {
            --tmpJump;
            setToRbegin();
        }
        navigator.previous(
            containerPtr,
            index,
            tmpJump
        );
        return *this;
    }
    
    /**
     * @brief creates an iterator which is jumpsize elements ahead. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the next element
     * @return iterator which is jumpsize elements ahead
     */
    template<
        typename T_Jump,
        bool T=isRandomAccessable
    >
    HDINLINE
    auto
    operator+(T_Jump const & jump)
    ->
    typename std::enable_if<
        T == true,
        DeepIterator
    >::type
    {
        DeepIterator tmp = *this;
        tmp+=jump;
        return tmp;
    }
    
    /**
     *@brief creates an iterator which is jumpsize elements behind. The iterator 
     * need to be random access to support this function.
     * @param jumpsize distance to the previos element
     * @return iterator which is jumpsize elements behind
     */
    template<
        typename T_Jump,
        bool T = isRandomAccessable
    >
    HDINLINE
    auto
    operator-(T_Jump const & jump)
    ->typename std::enable_if<
        T == true,
        DeepIterator
    >::type
    {
        DeepIterator tmp = *this;
        tmp-=jump;
        return tmp;
    }
    
        /**
     * @brief check whether the iterator is behind a second one.
     * @return true if the iterator is behind, false otherwise
     */
    template< bool T=isRandomAccessable>
    HDINLINE
    auto
    operator<(DeepIterator const & other)
    -> typename std::enable_if<
        T == true,
        bool
    >::type
    {
        return accessor.lesser(
            containerPtr, 
            index,
            other.containerPtr,
            other.index
        );
    }
    
    /**
     * @brief check whether the iterator is ahead a second one.
     * @return true if the iterator is ahead, false otherwise
     */
    template<bool T = isRandomAccessable>
    HDINLINE
    auto
    operator>(DeepIterator const & other)
    -> typename std::enable_if<
        T == true,
        bool
    >::type
    {
        return accessor.greater(
            containerPtr,  
            index,
            other.containerPtr,
            other.index
        );
    }
    
    /**
     * @brief check whether the iterator is behind or equal a second one.
     * @return true if the iterator is behind or equal, false otherwise
     */
    template< bool T=isRandomAccessable>
    HDINLINE
    auto
    operator<=(DeepIterator const & other)
    ->
    typename std::enable_if<
        T == true,
        bool
    >::type
    {

        return *this < other || *this == other;
    }
    
    /**
     * @brief check whether the iterator is ahead or equal a second one.
     * @return true if the iterator is ahead or equal, false otherwise
     */
    template<bool T = isRandomAccessable>
    HDINLINE
    auto
    operator>=(DeepIterator const & other)
    ->
    typename std::enable_if<
        T == true,
        bool
    >::type
    {
        return *this > other || *this == other;
    }
    
    /**
     * @return get the element at the specified position.
     */
    template<bool T = isRandomAccessable>
    HDINLINE
    auto
    operator[](IndexType const & index)
    -> typename std::enable_if<
        T == true,
        ComponentReference
    >::type
    {
        return accessor.get(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief check whether the iterator is after the last element
     * @return true, if it is, false if it is not after the last element
     */
    HDINLINE
    auto
    isAfterLast()
    const
    -> bool
    {
        return navigator.isAfterLast(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief check whether the iterator is before the first element
     * @return true, if it is, false if it is not after the last element
     */
    HDINLINE 
    auto
    isBeforeFirst()
    const
    -> bool
    {
        return navigator.isBeforeFirst(
            containerPtr,
            index
        );
    }
    


    /**
     * @brief This function set the iterator to the first element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     */
    HDINLINE 
    auto
    setToBegin()
    -> void
    {
        navigator.begin(
            containerPtr,
            index
        );
    }
    
        /**
     * @brief This function set the iterator to the first element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     */
    HDINLINE 
    auto
    setToBegin(ContainerPtr con)
    -> void
    {
        containerPtr = con;
        navigator.begin(
            containerPtr,
            index
        );
    }
    
        /**
     * @brief This function set the iterator to the first element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to con.
     */
    HDINLINE 
    auto
    setToBegin(ContainerReference con)
    -> void
    {
        containerPtr = &con;
        navigator.begin(
            containerPtr,
            index
        );
    }
    
       /**
     * @brief This function set the iterator to the after-last-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     * */
    HDINLINE
    auto
    setToEnd(ContainerPtr con)
    -> void
    {
        containerPtr = con;
        navigator.end(
            containerPtr,
            index
        );
    }
    
        /**
     * @brief This function set the iterator to the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     * */
    HDINLINE
    auto
    setToRend(ContainerPtr con)
    -> void
    {
        containerPtr = con;
        navigator.rend(
            containerPtr,
            index
        );
    }
    
        /**
     * @brief This function set the iterator to the last element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     */
    HDINLINE
    auto
    setToRbegin(ContainerReference con)
    -> void
    {
        containerPtr = &con;
        navigator.rbegin(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief This function set the iterator to the last element. This function
     * set also all childs to the begin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     * @param con The container over which the iterator walks is changed
     * to T_Container.
     */
    HDINLINE
    auto
    setToRbegin(ContainerPtr con)
    -> void
    {
        containerPtr = con;
        navigator.rbegin(
            containerPtr,
            index
        );
    }
    
    /**
     * @brief This function set the iterator to the last element. This function
     * set also all childs to rbegin. If the container hasnt enough elements
     * it should be set to the after-last-element or the before-first-element. 
     */
    HDINLINE
    auto
    setToRbegin()
    -> void
    {
        navigator.rbegin(
            containerPtr,
            index
        );
    }
    
           /**
     * @brief goto a successor element
     * @param jumpsize Distance to the successor element
     * @return The result value is importent if the iterator is in a middle 
     * layer. When we reach the end of the container, we need to give the higher
     * layer informations about the remainig elements, we need to overjump. This
     * distance is the return value of this function.
     */
    HDINLINE 
    auto 
    gotoNext(uint const & steps)
    ->
    uint
    {
        return navigator.next(
            containerPtr,
            index,
            steps
        );
    }
    
        /**
     * @brief goto a previos element
     * @param jumpsize Distance to the previous element
     * @return The result value is importent if the iterator is in a middle 
     * layer. When we reach the end of the container, we need to give the higher 
     * layer informations about the remainig elements, we need to overjump. This 
     * distance is the return value of this function.
     */
    HDINLINE
    auto 
    gotoPrevious(uint const & steps)
    ->
    uint
    {

        return navigator.previous(
            containerPtr,
            index,
            steps
        );
    }
    
        /**
     * @brief if the container has a constant size, this function can caluculate
     * it.
     * @return number of elements within the container. This include the child
     * elements
     */
    template< bool T = hasConstantSize>
    HDINLINE
    auto
    nbElements()
    const
    -> typename std::enable_if<
        T,
        int
    >::type
    {
        return navigator.size(containerPtr);
    }

protected:   
    Navigator navigator;
    Accessor accessor;
    pmacc::NoChild childIterator;
    

    ContainerType* containerPtr;
    IndexType index;

private:
} ; // struct DeepIterator


namespace details 
{
/**
 * @brief This function is used in makeView. The function is a identity function
 * for pmacc::NoChild
 */
template<
    typename T_Container,
    typename T_Child,
// SFIANE Part
    typename T_ChildNoRef = typename std::decay<T_Child>::type>
HDINLINE
auto
makeIterator(
    T_Child && child, 
    typename std::enable_if<
        std::is_same<
            T_ChildNoRef, 
            pmacc::NoChild
        >::value
    >::type* = nullptr)
->
pmacc::NoChild
{
    return child;
}



/**
 * @brief bind an an iterator concept to an containertype. The concept has no 
 * child.
 * @tparam T_Container type of the container
 * @param concept an iterator concept
 * 
 */
template<
    typename T_Container,
    typename TPrescription,
    typename TPrescriptionNoRef =typename std::decay<TPrescription>::type, 
    typename T_ContainerNoRef = typename std::decay<T_Container>::type, 
    typename T_ContainerCategoryType = typename traits::ContainerCategory<
        T_ContainerNoRef
    >::type,
    typename T_IndexType = typename pmacc::traits::IndexType<
        T_ContainerNoRef
    >::type,
    bool isBidirectional = pmacc::traits::IsBidirectional<
        T_Container,
        T_ContainerCategoryType
    >::value,
    bool isRandomAccessable = pmacc::traits::IsRandomAccessable<
        T_Container,
        T_ContainerCategoryType
    >::value,
    bool hasConstantSize = traits::HasConstantSize<T_Container>::value>
HDINLINE
auto 
makeIterator (
    TPrescription && concept,
    typename std::enable_if<
        not std::is_same<
            T_ContainerNoRef, 
            pmacc::NoChild
        >::value
    >::type* = nullptr)
->
DeepIterator<
    T_Container,
    decltype(makeAccessor<T_Container>(
            pmacc::forward<TPrescription>(concept).accessor
    )),
    decltype(makeNavigator<T_Container>(
            pmacc::forward<TPrescription>(concept).navigator
    )),
    decltype(makeIterator<typename traits::ComponentType<T_Container>::type>(
        pmacc::forward<TPrescription>(concept).child
    )),
    T_IndexType,
        hasConstantSize,
        isBidirectional,
        isRandomAccessable
>
{
    using ContainerType = T_Container;

    using AccessorType = decltype(makeAccessor<ContainerType>(
        pmacc::forward<TPrescription>(concept).accessor
    ));
    using NavigatorType = decltype(makeNavigator<ContainerType>(
        pmacc::forward<TPrescription>(concept).navigator
    ));
    using ChildType = decltype(
        makeIterator<typename traits::ComponentType<T_Container>::type>(
            pmacc::forward<TPrescription>(concept).child
        )
    );


    using Iterator = DeepIterator<
        ContainerType,
        AccessorType,
        NavigatorType,
        ChildType,
        T_IndexType,
        hasConstantSize,
        isBidirectional,
        isRandomAccessable>;
 
    return Iterator(
        makeAccessor<ContainerType>(
            pmacc::forward<TPrescription>(concept).accessor
        ),
        makeNavigator<ContainerType>(
            pmacc::forward<TPrescription>(concept).navigator
        ),
        makeIterator<typename traits::ComponentType<T_Container>::type>(
            pmacc::forward<TPrescription>(concept).child
        )
    );
}

} // namespace details


/**
 * @brief Bind a container to a virtual iterator.  
 * @param con The container you like to inspect
 * @param iteratorPrescription A virtual iterator, which describes the behavior 
 * of the iterator
 * @return An Iterator. It is set to the first element.
 */
template<
    typename T_Container,
    typename T_ContainerNoRef = typename std::decay<T_Container>::type,
    typename T_Accessor,
    typename T_Navigator,
    typename T_Child,
    typename T_ContainerCategoryType
        = typename traits::ContainerCategory<T_ContainerNoRef>::type,
    typename IndexType 
        = typename pmacc::traits::IndexType<T_ContainerNoRef>::type,
    bool isBidirectional 
        = pmacc::traits::IsBidirectional<
            T_ContainerNoRef,
            T_ContainerCategoryType
        >::value,
    bool isRandomAccessable 
        = pmacc::traits::IsRandomAccessable<
            T_ContainerNoRef,
            T_ContainerCategoryType
        >::value,
    bool hasConstantSize = traits::HasConstantSize<T_ContainerNoRef>::value
>
HDINLINE 
auto
makeIterator(
    T_Container && container,
    pmacc::details::IteratorPrescription<
        T_Accessor,
        T_Navigator,
        T_Child> && concept)
-> 
DeepIterator<
        typename std::decay<T_Container>::type,
        decltype(details::makeAccessor(container, concept.accessor)),
        decltype(details::makeNavigator(container, concept.navigator)),
        decltype(details::makeIterator<
            typename traits::ComponentType<
                typename std::decay<T_Container>::type>::type>(
                    concept.childIterator
                )
        ),
        IndexType,
        hasConstantSize,
        isBidirectional,
        isRandomAccessable>         
{
    using ContainerType = typename std::decay<T_Container>::type;
    using ComponentType = typename traits::ComponentType<ContainerType>::type;

    using AccessorType = decltype(details::makeAccessor(
        container, 
        concept.accessor
    ));
    using NavigatorType = decltype(details::makeNavigator(
        container, 
        concept.navigator
    )) ;
    using ChildType = decltype(details::makeIterator<ComponentType>(
        concept.childIterator
    ));
    

    using Iterator = DeepIterator<
        ContainerType,
        AccessorType,
        NavigatorType,
        ChildType,
        IndexType,
        hasConstantSize,
        isBidirectional,
        isRandomAccessable
    >;
    
    return Iterator(
        container, 
        details::makeAccessor<ContainerType>(),
        details::makeNavigator<ContainerType>(concept.navigator),
        details::makeIterator<ComponentType>(concept.childIterator)
    );
}

} // namespace pmacc
