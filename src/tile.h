/**
 * @file tile.h
 * @brief Classes for managing tile (rotation) values.
 *
 * The structures in this file are tile rotations. The tiles are made up with multiple pieces
 * placed together (the mask shows what pieces). We have the right and bottom edges first (at index
 * zero [0]) to speed up placement of the tiles.
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

#ifndef TILE_H
#define TILE_H

#include "lookup.h"

#include <cstring>
#include <iostream>

/// Test tile functionality.
bool test_tile();

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Tile implementation.
/**
 * Provides implementation for the tile classes.
 *
 * @tparam VALUE  structure to hold tile values
 * @tparam L  lookup type
 * @tparam M  mask type
 */
template <class VALUE, class L, class M>
struct tile
{
    typedef tile<VALUE, L, M> _Myt;                         ///< My type.

    typedef VALUE value_type;                               ///< Value type.

    typedef L lookup_type;                                  ///< Lookup type.
    typedef M mask_type;                                    ///< Mask type.

    // ========================================================================
    // Member Data
    // ========================================================================

    /// Tile values.
    value_type val;

    /// Bit mask of what pieces make up this tile.
    mask_type mask;

    /// Random value (used for randomizing order).
    env_word_type random;

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    tile()
    {
        zero();
    }

    /// Constructor.
    /**
     * @param[in] other  tile to duplicate
     */
    tile( const _Myt &other )
    {
        copy( other );
    }

    // ========================================================================
    // Operators
    // ========================================================================

    /// Assignment operator.
    /**
     * @param[in] other  value to assign
     * @return  reference to @c this
     */
    _Myt & operator = ( const _Myt &other )
    {
        copy( other );
        return *this;
    }

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve tile lookup value.
    /**
     * @return  lookup value
     */
    inline lookup_index_type lookup() const
    {
        return reinterpret_cast<const lookup_index_type*>( this )[0];
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Copy tile.
    /**
     * @param[in] other  tile to copy
     */
    void copy( const _Myt &other )
    {
        std::memcpy( this, &other, sizeof(_Myt) );
    }

    /// Print tile.
    void print() const
    {
        std::cout << "[ "; mask.print(); std::cout << "]" << std::endl;
    }

    /// Zero out tile.
    void zero()
    {
        std::memset( this, 0, sizeof(_Myt) );
    }

};

/// Functor for sorting tiles by edge.
struct sortTilesByEdge
{
    /// Comparison functor.
    /**
     * Compares two tiles by edge value.
     *
     * @tparam T  tile type
     * @param[in] lhs  tile to compare
     * @param[in] rhs  tile to compare
     * @return  @c true if @p lhs < @p rhs, @c false otherwise
     */
    template <class T>
    bool operator () ( const T &lhs, const T &rhs )
    {
        const lookup_index_type *lhsEdges( reinterpret_cast<const lookup_index_type*>( &lhs ) );
        const lookup_index_type *rhsEdges( reinterpret_cast<const lookup_index_type*>( &rhs ) );

        if ( lhsEdges[1] == rhsEdges[1] )
            return ( lhsEdges[0] < rhsEdges[0] );

        return ( lhsEdges[1] < rhsEdges[1] );
    }

};

/// Functor for sorting tiles by random value.
struct sortTilesByRandom
{
    /// Comparison functor.
    /**
     * Compares two tiles by random value. Used for sorting tiles randomly.
     *
     * @tparam T  tile type
     * @param[in] lhs  tile to compare
     * @param[in] rhs  tile to compare
     * @return  @c true if @p lhs < @p rhs, @c false otherwise
     */
    template <class T>
    bool operator () ( const T *lhs, const T *rhs )
    {
        return ( lhs->random < rhs->random );
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////

// 32 edges namespace
namespace e32
{
    /// 1x1 tile value.
    struct tile_1x1_val
    {
        lookup_index_type right             : 5;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 5;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 22;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 54;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 5;            ///< Left edge value (0 for border).
        lookup_index_type top               : 5;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 22;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 54;           ///< Reserved (do not use).
#endif
    };

    /// 1x1 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_1x1 = tile<tile_1x1_val, lookup_1x1, M>;

    /// 1x2 tile value.
    struct tile_1x2_val
    {
        lookup_index_type right             : 10;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 5;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 17;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 49;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 10;           ///< Left edge value (0 for border).
        lookup_index_type top               : 5;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 17;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 49;           ///< Reserved (do not use).
#endif
    };

    /// 1x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_1x2 = tile<tile_1x2_val, lookup_1x2, M>;

    /// 2x1 tile value.
    struct tile_2x1_val
    {
        lookup_index_type right             : 5;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 10;           ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 17;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 49;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 5;            ///< Left edge value (0 for border).
        lookup_index_type top               : 10;           ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 17;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 49;           ///< Reserved (do not use).
#endif
    };

    /// 2x1 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x1 = tile<tile_2x1_val, lookup_2x1, M>;

    /// 2x2 tile value.
    struct tile_2x2_val
    {
        lookup_index_type right             : 10;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 10;           ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 12;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 44;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 10;           ///< Left edge value (0 for border).
        lookup_index_type top               : 10;           ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 12;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 44;           ///< Reserved (do not use).
#endif
    };

    /// 2x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x2 = tile<tile_2x2_val, lookup_2x2, M>;

    /// 2x4 tile value.
    struct tile_2x4_val
    {
        lookup_index_type right             : 20;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 10;           ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 2;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 34;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 20;           ///< Left edge value (0 for border).
        lookup_index_type top               : 10;           ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 2;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 34;           ///< Reserved (do not use).
#endif
    };

    /// 2x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x4 = tile<tile_2x4_val, lookup_2x4, M>;

    /// 4x2 tile value.
    struct tile_4x2_val
    {
        lookup_index_type right             : 10;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 20;           ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 2;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 34;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 10;           ///< Left edge value (0 for border).
        lookup_index_type top               : 20;           ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 2;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 34;           ///< Reserved (do not use).
#endif
    };

    /// 4x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x2 = tile<tile_4x2_val, lookup_4x2, M>;

#if (64 == _ENV_BITS)

    /// 4x4 tile value.
    struct tile_4x4_val
    {
        lookup_index_type right             : 20;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 20;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 24;           ///< Reserved (do not use).

        lookup_index_type left              : 20;           ///< Left edge value (0 for border).
        lookup_index_type top               : 20;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 24;           ///< Reserved (do not use).
    };

    /// 4x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x4 = tile<tile_4x4_val, lookup_4x4, M>;

    /// 4x8 tile value.
    struct tile_4x8_val
    {
        lookup_index_type right             : 40;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 20;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 4;            ///< Reserved (do not use).

        lookup_index_type left              : 40;           ///< Left edge value (0 for border).
        lookup_index_type top               : 20;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 4;            ///< Reserved (do not use).
    };

    /// 4x8 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x8 = tile<tile_4x8_val, lookup_4x8, M>;

    /// 8x4 tile value.
    struct tile_8x4_val
    {
        lookup_index_type right             : 20;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 40;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 4;            ///< Reserved (do not use).

        lookup_index_type left              : 20;           ///< Left edge value (0 for border).
        lookup_index_type top               : 40;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 4;            ///< Reserved (do not use).
    };

    /// 8x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_8x4 = tile<tile_8x4_val, lookup_8x4, M>;

#endif

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

// 16 edges namespace
namespace e16
{
    /// 1x1 tile value.
    struct tile_1x1_val
    {
        lookup_index_type right             : 4;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 4;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 24;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 56;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 4;            ///< Left edge value (0 for border).
        lookup_index_type top               : 4;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 24;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 56;           ///< Reserved (do not use).
#endif
    };

    /// 1x1 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_1x1 = tile<tile_1x1_val, lookup_1x1, M>;

    /// 1x2 tile value.
    struct tile_1x2_val
    {
        lookup_index_type right             : 8;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 4;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 20;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 52;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 8;            ///< Left edge value (0 for border).
        lookup_index_type top               : 4;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 20;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 52;           ///< Reserved (do not use).
#endif
    };

    /// 1x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_1x2 = tile<tile_1x2_val, lookup_1x2, M>;

    /// 2x1 tile value.
    struct tile_2x1_val
    {
        lookup_index_type right             : 4;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 8;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 20;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 52;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 4;            ///< Left edge value (0 for border).
        lookup_index_type top               : 8;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 20;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 52;           ///< Reserved (do not use).
#endif
    };

    /// 2x1 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x1 = tile<tile_2x1_val, lookup_2x1, M>;

    /// 2x2 tile value.
    struct tile_2x2_val
    {
        lookup_index_type right             : 8;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 8;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 16;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 48;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 8;            ///< Left edge value (0 for border).
        lookup_index_type top               : 8;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 16;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 48;           ///< Reserved (do not use).
#endif
    };

    /// 2x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x2 = tile<tile_2x2_val, lookup_2x2, M>;

    /// 2x4 tile value.
    struct tile_2x4_val
    {
        lookup_index_type right             : 16;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 8;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 8;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 40;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 16;           ///< Left edge value (0 for border).
        lookup_index_type top               : 8;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 8;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 40;           ///< Reserved (do not use).
#endif
    };

    /// 2x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x4 = tile<tile_2x4_val, lookup_2x4, M>;

    /// 4x2 tile value.
    struct tile_4x2_val
    {
        lookup_index_type right             : 8;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 16;           ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 8;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 40;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 8;            ///< Left edge value (0 for border).
        lookup_index_type top               : 16;           ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 8;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 40;           ///< Reserved (do not use).
#endif
    };

    /// 4x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x2 = tile<tile_4x2_val, lookup_4x2, M>;

#if (64 == _ENV_BITS)

    /// 4x4 tile value.
    struct tile_4x4_val
    {
        lookup_index_type right             : 16;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 16;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 32;           ///< Reserved (do not use).

        lookup_index_type left              : 16;           ///< Left edge value (0 for border).
        lookup_index_type top               : 16;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 32;           ///< Reserved (do not use).
    };

    /// 4x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x4 = tile<tile_4x4_val, lookup_4x4, M>;

    /// 4x8 tile value.
    struct tile_4x8_val
    {
        lookup_index_type right             : 32;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 16;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 16;           ///< Reserved (do not use).

        lookup_index_type left              : 32;           ///< Left edge value (0 for border).
        lookup_index_type top               : 16;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 16;           ///< Reserved (do not use).
    };

    /// 4x8 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x8 = tile<tile_4x8_val, lookup_4x8, M>;

    /// 8x4 tile value.
    struct tile_8x4_val
    {
        lookup_index_type right             : 16;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 32;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 16;           ///< Reserved (do not use).

        lookup_index_type left              : 16;           ///< Left edge value (0 for border).
        lookup_index_type top               : 32;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 16;           ///< Reserved (do not use).
    };

    /// 8x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_8x4 = tile<tile_8x4_val, lookup_8x4, M>;

#endif

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

// 8 edges namespace
namespace e8
{
    /// 1x1 tile value.
    struct tile_1x1_val
    {
        lookup_index_type right             : 3;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 3;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 26;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 58;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 3;            ///< Left edge value (0 for border).
        lookup_index_type top               : 3;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 26;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 58;           ///< Reserved (do not use).
#endif
    };

    /// 1x1 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_1x1 = tile<tile_1x1_val, lookup_1x1, M>;

    /// 1x2 tile value.
    struct tile_1x2_val
    {
        lookup_index_type right             : 6;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 3;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 23;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 55;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 6;            ///< Left edge value (0 for border).
        lookup_index_type top               : 3;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 23;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 55;           ///< Reserved (do not use).
#endif
    };

    /// 1x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_1x2 = tile<tile_1x2_val, lookup_1x2, M>;

    /// 2x1 tile value.
    struct tile_2x1_val
    {
        lookup_index_type right             : 3;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 6;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 23;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 55;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 3;            ///< Left edge value (0 for border).
        lookup_index_type top               : 6;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 23;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 55;           ///< Reserved (do not use).
#endif
    };

    /// 2x1 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x1 = tile<tile_2x1_val, lookup_2x1, M>;

    /// 2x2 tile value.
    struct tile_2x2_val
    {
        lookup_index_type right             : 6;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 6;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 20;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 52;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 6;            ///< Left edge value (0 for border).
        lookup_index_type top               : 6;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 20;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 52;           ///< Reserved (do not use).
#endif
    };

    /// 2x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x2 = tile<tile_2x2_val, lookup_2x2, M>;

    /// 2x4 tile value.
    struct tile_2x4_val
    {
        lookup_index_type right             : 12;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 6;            ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 14;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 46;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 12;           ///< Left edge value (0 for border).
        lookup_index_type top               : 6;            ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 14;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 46;           ///< Reserved (do not use).
#endif
    };

    /// 2x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_2x4 = tile<tile_2x4_val, lookup_2x4, M>;

    /// 4x2 tile value.
    struct tile_4x2_val
    {
        lookup_index_type right             : 6;            ///< Right edge value (0 for border).
        lookup_index_type bottom            : 12;           ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 14;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 46;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 6;            ///< Left edge value (0 for border).
        lookup_index_type top               : 12;           ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 14;           ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 46;           ///< Reserved (do not use).
#endif
    };

    /// 4x2 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x2 = tile<tile_4x2_val, lookup_4x2, M>;

    /// 4x4 tile value.
    struct tile_4x4_val
    {
        lookup_index_type right             : 12;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 12;           ///< Bottom edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_0       : 8;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_0       : 40;           ///< Reserved (do not use).
#endif

        lookup_index_type left              : 12;           ///< Left edge value (0 for border).
        lookup_index_type top               : 12;           ///< Top edge value (0 for border).
#if (32 == _ENV_BITS)
        lookup_index_type _reserved_1       : 8;            ///< Reserved (do not use).
#elif (64 == _ENV_BITS)
        lookup_index_type _reserved_1       : 40;           ///< Reserved (do not use).
#endif
    };

    /// 4x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x4 = tile<tile_4x4_val, lookup_4x4, M>;

#if (64 == _ENV_BITS)

    /// 4x8 tile value.
    struct tile_4x8_val
    {
        lookup_index_type right             : 24;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 12;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 28;           ///< Reserved (do not use).

        lookup_index_type left              : 24;           ///< Left edge value (0 for border).
        lookup_index_type top               : 12;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 28;           ///< Reserved (do not use).
    };

    /// 4x8 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_4x8 = tile<tile_4x8_val, lookup_4x8, M>;

    /// 8x4 tile value.
    struct tile_8x4_val
    {
        lookup_index_type right             : 12;           ///< Right edge value (0 for border).
        lookup_index_type bottom            : 24;           ///< Bottom edge value (0 for border).
        lookup_index_type _reserved_0       : 28;           ///< Reserved (do not use).

        lookup_index_type left              : 12;           ///< Left edge value (0 for border).
        lookup_index_type top               : 24;           ///< Top edge value (0 for border).
        lookup_index_type _reserved_1       : 28;           ///< Reserved (do not use).
    };

    /// 8x4 tile.
    /**
     * @tparam M  mask type
     */
    template <typename M>
    using tile_8x4 = tile<tile_8x4_val, lookup_8x4, M>;

#endif

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TILE_H

