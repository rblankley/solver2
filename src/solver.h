/**
 * @file solver.h
 * @brief Solver type definitions and global include.
 *
 * @section Copyright
 * Copyright (C) 2015-2017 Randy Blankley
 *
 * @section License
 * This file is part of solver.
 *
 * Solver is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with solver.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "bitwisemask.h"
#include "board.h"
#include "boardh.h"
#include "boardv.h"
#include "piecevector.h"
#include "placement.h"
#include "tile.h"
#include "tilearraylookuparray.h"
#include "tilearraylookupmap.h"
#include "tilearraylookupvector.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if (32 == _ENV_BITS)

typedef bitwiseMask<1> mask_32;                             ///< Mask for 32 piece puzzle.
typedef bitwiseMask<2> mask_64;                             ///< Mask for 64 piece puzzle.
typedef bitwiseMask<4> mask_128;                            ///< Mask for 128 piece puzzle.
typedef bitwiseMask<8> mask_256;                            ///< Mask for 256 piece puzzle.

#elif (64 == _ENV_BITS)

typedef bitwiseMask<1> mask_32;                             ///< Mask for 32 piece puzzle.
typedef bitwiseMask<1> mask_64;                             ///< Mask for 64 piece puzzle.
typedef bitwiseMask<2> mask_128;                            ///< Mask for 128 piece puzzle.
typedef bitwiseMask<4> mask_256;                            ///< Mask for 256 piece puzzle.
typedef bitwiseMask<8> mask_512;                            ///< Mask for 512 piece puzzle.

#endif

// 32 edges namespace
namespace e32
{
    /// Solver tile array lookup types (32 edges variant).
    /**
     * @tparam M  mask type
     */
    template <class M>
    struct tileArrayLookups
    {
        typedef tileArrayLookupArray<e32::tile_1x1<M>> lookup_1x1_type;         ///< Tile lookup type for 1x1 tiles.
        typedef tileArrayLookupVector<e32::tile_1x2<M>> lookup_1x2_type;        ///< Tile lookup type for 1x2 tiles.
        typedef tileArrayLookupVector<e32::tile_2x1<M>> lookup_2x1_type;        ///< Tile lookup type for 2x1 tiles.
        typedef tileArrayLookupVector<e32::tile_2x2<M>> lookup_2x2_type;        ///< Tile lookup type for 2x2 tiles.
        typedef tileArrayLookupMap<e32::tile_2x4<M>> lookup_2x4_type;           ///< Tile lookup type for 2x4 tiles.
        typedef tileArrayLookupMap<e32::tile_4x2<M>> lookup_4x2_type;           ///< Tile lookup type for 4x2 tiles.
#if (64 == _ENV_BITS)
        typedef tileArrayLookupMap<e32::tile_4x4<M>> lookup_4x4_type;           ///< Tile lookup type for 4x4 tiles.
        typedef tileArrayLookupMap<e32::tile_4x8<M>> lookup_4x8_type;           ///< Tile lookup type for 4x8 tiles.
        typedef tileArrayLookupMap<e32::tile_8x4<M>> lookup_8x4_type;           ///< Tile lookup type for 8x4 tiles.
#endif
    };

} // namespace

// 16 edges namespace
namespace e16
{
    /// Solver tile array lookup types (16 edges variant).
    /**
     * @tparam M  mask type
     */
    template <class M>
    struct tileArrayLookups
    {
        typedef tileArrayLookupArray<e16::tile_1x1<M>> lookup_1x1_type;         ///< Tile lookup type for 1x1 tiles.
        typedef tileArrayLookupVector<e16::tile_1x2<M>> lookup_1x2_type;        ///< Tile lookup type for 1x2 tiles.
        typedef tileArrayLookupVector<e16::tile_2x1<M>> lookup_2x1_type;        ///< Tile lookup type for 2x1 tiles.
        typedef tileArrayLookupVector<e16::tile_2x2<M>> lookup_2x2_type;        ///< Tile lookup type for 2x2 tiles.
        typedef tileArrayLookupMap<e16::tile_2x4<M>> lookup_2x4_type;           ///< Tile lookup type for 2x4 tiles.
        typedef tileArrayLookupMap<e16::tile_4x2<M>> lookup_4x2_type;           ///< Tile lookup type for 4x2 tiles.
#if (64 == _ENV_BITS)
        typedef tileArrayLookupMap<e16::tile_4x4<M>> lookup_4x4_type;           ///< Tile lookup type for 4x4 tiles.
        typedef tileArrayLookupMap<e16::tile_4x8<M>> lookup_4x8_type;           ///< Tile lookup type for 4x8 tiles.
        typedef tileArrayLookupMap<e16::tile_8x4<M>> lookup_8x4_type;           ///< Tile lookup type for 8x4 tiles.
#endif
    };

} // namespace

// 8 edges namespace
namespace e8
{
    /// Solver tile array lookup types (8 edges variant).
    /**
     * @tparam M  mask type
     */
    template <class M>
    struct tileArrayLookups
    {
        typedef tileArrayLookupArray<e8::tile_1x1<M>> lookup_1x1_type;          ///< Tile lookup type for 1x1 tiles.
        typedef tileArrayLookupArray<e8::tile_1x2<M>> lookup_1x2_type;          ///< Tile lookup type for 1x2 tiles.
        typedef tileArrayLookupArray<e8::tile_2x1<M>> lookup_2x1_type;          ///< Tile lookup type for 2x1 tiles.
        typedef tileArrayLookupVector<e8::tile_2x2<M>> lookup_2x2_type;         ///< Tile lookup type for 2x2 tiles.
        typedef tileArrayLookupVector<e8::tile_2x4<M>> lookup_2x4_type;         ///< Tile lookup type for 2x4 tiles.
        typedef tileArrayLookupVector<e8::tile_4x2<M>> lookup_4x2_type;         ///< Tile lookup type for 4x2 tiles.
        typedef tileArrayLookupMap<e8::tile_4x4<M>> lookup_4x4_type;            ///< Tile lookup type for 4x4 tiles.
#if (64 == _ENV_BITS)
        typedef tileArrayLookupMap<e8::tile_4x8<M>> lookup_4x8_type;            ///< Tile lookup type for 4x8 tiles.
        typedef tileArrayLookupMap<e8::tile_8x4<M>> lookup_8x4_type;            ///< Tile lookup type for 8x4 tiles.
#endif
    };

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // SOLVER_H

