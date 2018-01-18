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

#include "pmacc/traits/iterator/Componenttype.hpp"
#include "pmacc/traits/iterator/ContainerCategory.hpp"
#include "pmacc/traits/iterator/HasConstantSize.hpp"
#include "pmacc/traits/iterator/IsBidirectional.hpp"
#include "pmacc/traits/iterator/IsRandomAccessable.hpp"
#include "pmacc/traits/iterator/IndexType.hpp"
#include "pmacc/traits/iterator/RangeType.hpp"
#include "pmacc/traits/iterator/NumberElements.hpp"

#include "pmacc/traits/iterator/navigator/AfterLastElement.hpp"
#include "pmacc/traits/iterator/navigator/BeforeFirstElement.hpp"
#include "pmacc/traits/iterator/navigator/LastElement.hpp"
#include "pmacc/traits/iterator/navigator/NextElement.hpp"
#include "pmacc/traits/iterator/navigator/PreviousElement.hpp"
#include "pmacc/traits/iterator/navigator/FirstElement.hpp"

#include "pmacc/traits/iterator/accessor/Ahead.hpp"
#include "pmacc/traits/iterator/accessor/Behind.hpp"
#include "pmacc/traits/iterator/accessor/Equal.hpp"
#include "pmacc/traits/iterator/accessor/Get.hpp"

#include "pmacc/traits/iterator/ContainerCategory.hpp"
