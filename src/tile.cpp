/**
 * @file tile.cpp
 * @brief Classes for managing tile (rotation) values implementation.
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

#include "bitwisemask.h"
#include "tile.h"

#include <cassert>

/// Assertion of expression.
#define ASSERT( EXPR )                                      \
    assert( EXPR );                                         \
    if ( !(EXPR) )                                          \
        return false

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for tile class.
/**
 * @tparam T  tile type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <class T>
bool test_tile_impl()
{
    typedef typename T::lookup_type lookup_type;
    typedef typename T::mask_type mask_type;

    static_assert( 2 * sizeof(lookup_index_type) + sizeof(mask_type) + sizeof(env_word_type) == sizeof(T), "Invalid tile size." );

    const lookup_type lookup_l( lookup_type::MASK_LEFT_EDGE );
    const lookup_type lookup_t( lookup_type::MASK_TOP_EDGE );

    const lookup_index_type lookup( lookup_l.value() | lookup_t.value() );

    T tile;

    const lookup_index_type *tileEdges( reinterpret_cast<const lookup_index_type*>( &tile ) );

    tile.val.left = lookup_type::LEFT_EDGE;
    ASSERT( 0 == tile.lookup() );
    ASSERT( 0 == tileEdges[0] );
    ASSERT( lookup_l.value() == tileEdges[1] );

    tile.val.left = 0;
    tile.val.right = lookup_type::LEFT_EDGE;
    ASSERT( lookup_l.value() == tile.lookup() );
    ASSERT( lookup_l.value() == tileEdges[0] );
    ASSERT( 0== tileEdges[1] );

    tile.val.left = tile.val.right = lookup_type::LEFT_EDGE;
    ASSERT( lookup_l.value() == tile.lookup() );
    ASSERT( lookup_l.value() == tileEdges[0] );
    ASSERT( lookup_l.value() == tileEdges[1] );

    tile.val.right = tile.val.left = lookup_type::LEFT_EDGE;
    ASSERT( lookup_l.value() == tile.lookup() );
    ASSERT( lookup_l.value() == tileEdges[0] );
    ASSERT( lookup_l.value() == tileEdges[1] );

    tile.val.left = tile.val.right = 0;

    tile.val.top = lookup_type::TOP_EDGE;
    ASSERT( 0 == tile.lookup() );
    ASSERT( 0 == tileEdges[0] );
    ASSERT( lookup_t.value() == tileEdges[1] );

    tile.val.top = 0;
    tile.val.bottom = lookup_type::TOP_EDGE;
    ASSERT( lookup_t.value() == tile.lookup() );
    ASSERT( lookup_t.value() == tileEdges[0] );
    ASSERT( 0 == tileEdges[1] );

    tile.val.top = tile.val.bottom = lookup_type::TOP_EDGE;
    ASSERT( lookup_t.value() == tile.lookup() );
    ASSERT( lookup_t.value() == tileEdges[0] );
    ASSERT( lookup_t.value() == tileEdges[1] );

    tile.val.bottom = tile.val.top = lookup_type::TOP_EDGE;
    ASSERT( lookup_t.value() == tile.lookup() );
    ASSERT( lookup_t.value() == tileEdges[0] );
    ASSERT( lookup_t.value() == tileEdges[1] );

    tile.val.left = tile.val.right = lookup_type::LEFT_EDGE;
    ASSERT( lookup == tile.lookup() );
    ASSERT( lookup == tileEdges[0] );
    ASSERT( lookup == tileEdges[1] );

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for tile class.
/**
 * @tparam T  tile type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <template <class> class T>
bool test_tile_all_masks()
{
    // test all variations
    if ( !test_tile_impl<T<bitwiseMask<1>>>() )
        return false;
    else if ( !test_tile_impl<T<bitwiseMask<2>>>() )
        return false;
    else if ( !test_tile_impl<T<bitwiseMask<4>>>() )
        return false;
    else if ( !test_tile_impl<T<bitwiseMask<8>>>() )
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool test_tile()
{
    // test all variations
    if ( !test_tile_all_masks<e32::tile_1x1>() )
        return false;
    else if ( !test_tile_all_masks<e32::tile_1x2>() )
        return false;
    else if ( !test_tile_all_masks<e32::tile_2x1>() )
        return false;
    else if ( !test_tile_all_masks<e32::tile_2x2>() )
        return false;
    else if ( !test_tile_all_masks<e32::tile_2x4>() )
        return false;
    else if ( !test_tile_all_masks<e32::tile_4x2>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_tile_all_masks<e32::tile_4x4>() )
        return false;
    else if ( !test_tile_all_masks<e32::tile_4x8>() )
        return false;
    else if ( !test_tile_all_masks<e32::tile_8x4>() )
        return false;
#endif

    if ( !test_tile_all_masks<e16::tile_1x1>() )
        return false;
    else if ( !test_tile_all_masks<e16::tile_1x2>() )
        return false;
    else if ( !test_tile_all_masks<e16::tile_2x1>() )
        return false;
    else if ( !test_tile_all_masks<e16::tile_2x2>() )
        return false;
    else if ( !test_tile_all_masks<e16::tile_2x4>() )
        return false;
    else if ( !test_tile_all_masks<e16::tile_4x2>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_tile_all_masks<e16::tile_4x4>() )
        return false;
    else if ( !test_tile_all_masks<e16::tile_4x8>() )
        return false;
    else if ( !test_tile_all_masks<e16::tile_8x4>() )
        return false;
#endif

    if ( !test_tile_all_masks<e8::tile_1x1>() )
        return false;
    else if ( !test_tile_all_masks<e8::tile_1x2>() )
        return false;
    else if ( !test_tile_all_masks<e8::tile_2x1>() )
        return false;
    else if ( !test_tile_all_masks<e8::tile_2x2>() )
        return false;
    else if ( !test_tile_all_masks<e8::tile_2x4>() )
        return false;
    else if ( !test_tile_all_masks<e8::tile_4x2>() )
        return false;
    else if ( !test_tile_all_masks<e8::tile_4x4>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_tile_all_masks<e8::tile_4x8>() )
        return false;
    else if ( !test_tile_all_masks<e8::tile_8x4>() )
        return false;
#endif

    return true;
}
