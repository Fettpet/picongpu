#pragma once
/**
 * @author Sebastian Hahn t.hahn < at > pmacc.de
 * @brief Each container category need to specify one range type. This is the 
 * type of the distance between two iterator points
 * 
 */

namespace pmacc 
{
namespace traits
{
template<typename TContainerCategorie, typename SFIANE = void>
struct RangeType;
}// namespace traits
}// namespace pmacc


