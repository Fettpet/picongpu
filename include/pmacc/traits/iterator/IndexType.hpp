/**
 * @author Sebastian Hahn t.hahn < at > pmacc.de
 * @brief This trait is used to decide the indextype. The indextype is used to 
 * specify the position to get the current component out of the container.
 */
#pragma once

namespace pmacc
{

namespace traits
{

template<
    typename TContainer, 
    typename SFINAE = void>
struct IndexType
{
    typedef int_fast32_t type; 
};


} // namespace traits

} // namespace pmacc
