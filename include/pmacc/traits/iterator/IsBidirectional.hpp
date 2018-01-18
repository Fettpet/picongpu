/**
 * @author Sebastian Hahn t.hahn < at > pmacc.de
 * @brief A container is bidirectional it is possible to go to the previous element.
 * The deepiterator has the functions --it and it-- if it is bidirectional.
 */
#pragma once

namespace pmacc 
{
namespace traits
{
template<typename TContainerCategorie, typename SFIANE = void>
struct IsBidirectional;

}// namespace traits
}// namespace pmacc


