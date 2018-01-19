/* Copyright 2013-2018 Rene Widera, Alexander Grund, Sebastian Hahn
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

#include "pmacc/types.hpp"

#include <boost/mpl/map.hpp>
#include <boost/mpl/list.hpp>
#include "pmacc/math/MapTuple.hpp"


#include <boost/type_traits.hpp>

#include "pmacc/particles/boostExtension/InheritLinearly.hpp"
#include "pmacc/particles/memory/dataTypes/Particle.hpp"
#include "pmacc/particles/frame_types.hpp"
#include "pmacc/compileTime/conversion/SeqToMap.hpp"
#include "pmacc/compileTime/conversion/OperateOnSeq.hpp"
#include <boost/utility/result_of.hpp>
#include <boost/mpl/find.hpp>
#include <boost/type_traits/is_same.hpp>

#include "pmacc/compileTime/GetKeyFromAlias.hpp"

#include "pmacc/traits/HasIdentifier.hpp"
#include "pmacc/traits/HasFlag.hpp"
#include "pmacc/traits/GetFlagType.hpp"
#include <boost/mpl/contains.hpp>

#include "pmacc/particles/ParticleDescription.hpp"
#include <boost/mpl/string.hpp>

#include "pmacc/traits/iterator/Componenttype.hpp"
#include "pmacc/traits/iterator/HasConstantSize.hpp"
#include "pmacc/traits/iterator/NumberElements.hpp"
#include "pmacc/traits/iterator/ContainerCategory.hpp"
#include "pmacc/memory/iterator/categorie/ArrayLike.hpp"

namespace pmacc
{

namespace pmath = pmacc::math;

/** Frame is a storage for arbitrary number >0 of Particles with attributes
 *
 * @tparam T_CreatePairOperator unary template operator to create a boost pair
 *                              from single type ( pair<name,dataType> )
 *                              @see MapTupel
 * @tparam T_ValueTypeSeq sequence with value_identifier
 * @tparam T_MethodsList sequence of classes with particle methods
 *                       (e.g. calculate mass, gamma, ...)
 * @tparam T_Flags sequence with identifiers to add flags on a frame
 *                 (e.g. useSolverXY, calcRadiation, ...)
 */
template<
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct Frame;

template<
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct Frame :
    public InheritLinearly<typename T_ParticleDescription::MethodsList>,
    protected pmath::MapTuple<typename SeqToMap<
        typename T_ParticleDescription::ValueTypeSeq,
        T_CreatePairOperator>::type, pmath::AlignedData
    >,
    public InheritLinearly<
        typename OperateOnSeq<
            typename T_ParticleDescription::FrameExtensionList,
            bmpl::apply1<
                bmpl::_1,
                Frame<T_CreatePairOperator,T_ParticleDescription>
            >
        >::type
    >
{
    using ParticleDescription =  T_ParticleDescription ;
    using Name = typename ParticleDescription::Name;
    using SuperCellSize = typename ParticleDescription::SuperCellSize ;
    using ValueTypeSeq = typename ParticleDescription::ValueTypeSeq ;
    using MethodsList = typename ParticleDescription::MethodsList ;
    using FlagList = typename ParticleDescription::FlagsList ;
    using FrameExtensionList = typename ParticleDescription::FrameExtensionList ;
    using ThisType = Frame<
        T_CreatePairOperator,
        ParticleDescription
    > ;
    /* definition of the MapTupel where we inherit from*/
    using BaseType = pmath::MapTuple<
        typename SeqToMap<
            ValueTypeSeq,
            T_CreatePairOperator
        >::type,
        pmath::AlignedData
    >;

    /* type of a single particle*/
    using ParticleType = pmacc::Particle<ThisType>;

    /* define boost result_of results
     * normaly result_of defines operator() result, in this case we define the result for
     * operator[]
     */
    template<class> struct result;

    /* const operator[]*/
    template<
        typename F,
        typename T_Key>
    struct result<const F(T_Key)>
    {

        using Key = typename GetKeyFromAlias<
            ValueTypeSeq,
            T_Key,
            errorHandlerPolicies::ThrowValueNotFound
        >::type ;
        using type = typename boost::result_of<const BaseType(Key)>::type ;
    };

    /* non const operator[]*/
    template<
        typename F,
        typename T_Key>
    struct result< F(T_Key)>
    {
        using Key = typename GetKeyFromAlias<
            ValueTypeSeq,
            T_Key,
            errorHandlerPolicies::ThrowValueNotFound
        >::type;
        using type = typename boost::result_of< BaseType(Key)>::type;
    };

    /** access the Nth particle*/
    HDINLINE
    auto
    operator[](const uint32_t idx)
    -> ParticleType
    {
        return ParticleType(*this, idx);
    }

    /** access the Nth particle*/
    HDINLINE
    auto
    operator[](const uint32_t idx)
    const
    -> const ParticleType
    {
        return ParticleType(*this, idx);
    }

    /** access attribute with a identifier
     *
     * @param T_Key instance of identifier type
     *              (can be an alias, value_identifier or any other class)
     * @return result of operator[] of MapTupel
     */
    template<typename T_Key >
    HDINLINE
    auto
    getIdentifier(const T_Key)
    -> typename boost::result_of < ThisType(T_Key)>::type
    {
        using Key = typename GetKeyFromAlias<
            ValueTypeSeq,
            T_Key
        >::type;
        return BaseType::operator[](Key());
    }

    /** const version of method getIdentifier(const T_Key) */
    template<typename T_Key >
    HDINLINE
    auto
    getIdentifier(const T_Key)
    const
    -> typename boost::result_of < const ThisType(T_Key)>::type
    {
        using Key = typename GetKeyFromAlias<
            ValueTypeSeq,
            T_Key
        >::type;

        return BaseType::operator[](Key());
    }

    HINLINE
    static auto
    getName()
    ->
    std::string
    {
        return std::string(boost::mpl::c_str<Name>::value);
    }

}; // struct frame

namespace traits
{

/**
@brief number of elements within a frame
@todo Here we need to specify the number of particles in frames
*/
template<
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct NumberElements<pmacc::Frame<
    T_CreatePairOperator,
    T_ParticleDescription
> >
{
    using ContainerType = pmacc::Frame<
        T_CreatePairOperator,
        T_ParticleDescription
    >;


    HDINLINE
    auto
    operator()(ContainerType*)
    -> uint_fast32_t
    {
        return 256;
    }
};

template<
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct HasConstantSize<pmacc::Frame<
    T_CreatePairOperator,
    T_ParticleDescription
> >
{
    const static bool value = true;
};

template<
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct ContainerCategory<pmacc::Frame<
    T_CreatePairOperator,
    T_ParticleDescription
> >
{
    typedef pmacc::container::categorie::ArrayLike type;
};

template<
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct ComponentType<pmacc::Frame<
    T_CreatePairOperator,
    T_ParticleDescription
> >
{
    using FrameType =pmacc::Frame<T_CreatePairOperator, T_ParticleDescription>;
    using type = typename FrameType::ParticleType;
};

template<
    typename T_IdentifierName,
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct HasIdentifier<
    pmacc::Frame<
        T_CreatePairOperator,
        T_ParticleDescription
    >,
    T_IdentifierName
>
{
private:
    using FrameType = pmacc::Frame<
        T_CreatePairOperator,
        T_ParticleDescription>;
public:
    using ValueTypeSeq = typename FrameType::ValueTypeSeq ;
    /* if T_IdentifierName is void_ than we have no T_IdentifierName in our Sequence.
     * check is also valid if T_Key is a alias
     */
    using SolvedAliasName = typename GetKeyFromAlias<
        ValueTypeSeq,
        T_IdentifierName
    >::type;

    using type = bmpl::contains<
        ValueTypeSeq,
        SolvedAliasName
    >;
};

template<
    typename T_IdentifierName,
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct HasFlag<
    pmacc::Frame<
         T_CreatePairOperator,
         T_ParticleDescription
    >,
    T_IdentifierName
>
{
private:
    using FrameType = pmacc::Frame<
        T_CreatePairOperator,
        T_ParticleDescription
    >;
    using SolvedAliasName =typename GetFlagType<
        FrameType,
        T_IdentifierName
    >::type;
    using FlagList = typename FrameType::FlagList;
public:

    using type =  bmpl::contains<
        FlagList,
        SolvedAliasName
    > ;
};

template<
    typename T_IdentifierName,
    typename T_CreatePairOperator,
    typename T_ParticleDescription
>
struct GetFlagType<
    pmacc::Frame<
         T_CreatePairOperator,
         T_ParticleDescription
    >,
    T_IdentifierName
>
{
private:
    using FrameType =  pmacc::Frame<
        T_CreatePairOperator,
        T_ParticleDescription
    >;
    using FlagList =  typename FrameType::FlagList;
public:

    using type = typename GetKeyFromAlias<FlagList, T_IdentifierName>::type;
};

} //namespace traits

}//namespace pmacc
