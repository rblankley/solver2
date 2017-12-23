/**
 * @file placement.cpp
 * @brief Tile placement class implementation.
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
#include "placement.h"
#include "tile.h"

#include <cassert>

/// Assertion of expression.
#define ASSERT( EXPR )                                      \
    assert( EXPR );                                         \
    if ( !(EXPR) )                                          \
        return false

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for @c placement.
/**
 * @tparam T  tile type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <class T>
bool test_placement_impl()
{
    typedef placement<T> placement_type;
    typedef typename placement_type::tile_type tile_type;
    typedef typename placement_type::lookup_type lookup_type;

    // one lookup and seven pointers
    static_assert( sizeof(lookup_type) + 7 * sizeof(void*) == sizeof(placement_type), "Invalid placement size." );

    placement_type p[3];
    p[0].right = &p[1];
    p[0].bottom = &p[2];

    const lookup_type *p0( reinterpret_cast<const lookup_type*>( &p[0] ) );
    const lookup_type *p1( reinterpret_cast<const lookup_type*>( &p[1] ) );
    const lookup_type *p2( reinterpret_cast<const lookup_type*>( &p[2] ) );

    ASSERT( 0 == p[0].lookup );
    ASSERT( 0 == p[1].lookup );
    ASSERT( 0 == p[2].lookup );

    tile_type t1;
    t1.val.left = 1;
    t1.val.top = 2;
    t1.val.right = 3;
    t1.val.bottom = 4;

    p[0].place( &t1 );
    ASSERT( &t1 == p[0].tile );

    ASSERT( 0 == p0->val.left );
    ASSERT( 0 == p0->val.top );
    ASSERT( 0 == p0->val.right );
    ASSERT( 0 == p0->val.bottom );

    ASSERT( 3 == p1->val.left );
    ASSERT( 0 == p1->val.top );
    ASSERT( 0 == p1->val.right );
    ASSERT( 0 == p1->val.bottom );

    ASSERT( 0 == p2->val.left );
    ASSERT( 4 == p2->val.top );
    ASSERT( 0 == p2->val.right );
    ASSERT( 0 == p2->val.bottom );

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for @c placement.
/**
 * @tparam T  tile type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <template <class> class T>
bool test_placement_all_masks()
{
    // test all variations
    if ( !test_placement_impl<T<bitwiseMask<1>>>() )
        return false;
    else if ( !test_placement_impl<T<bitwiseMask<2>>>() )
        return false;
    else if ( !test_placement_impl<T<bitwiseMask<4>>>() )
        return false;
    else if ( !test_placement_impl<T<bitwiseMask<8>>>() )
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool test_placement()
{
    // test all variations
    if ( !test_placement_all_masks<e32::tile_1x1>() )
        return false;
    else if ( !test_placement_all_masks<e32::tile_1x2>() )
        return false;
    else if ( !test_placement_all_masks<e32::tile_2x2>() )
        return false;
    else if ( !test_placement_all_masks<e32::tile_2x4>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_placement_all_masks<e32::tile_4x4>() )
        return false;
    else if ( !test_placement_all_masks<e32::tile_4x8>() )
        return false;
#endif

    if ( !test_placement_all_masks<e16::tile_1x1>() )
        return false;
    else if ( !test_placement_all_masks<e16::tile_1x2>() )
        return false;
    else if ( !test_placement_all_masks<e16::tile_2x2>() )
        return false;
    else if ( !test_placement_all_masks<e16::tile_2x4>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_placement_all_masks<e16::tile_4x4>() )
        return false;
    else if ( !test_placement_all_masks<e16::tile_4x8>() )
        return false;
#endif

    if ( !test_placement_all_masks<e8::tile_1x1>() )
        return false;
    else if ( !test_placement_all_masks<e8::tile_1x2>() )
        return false;
    else if ( !test_placement_all_masks<e8::tile_2x2>() )
        return false;
    else if ( !test_placement_all_masks<e8::tile_2x4>() )
        return false;
    else if ( !test_placement_all_masks<e8::tile_4x4>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_placement_all_masks<e8::tile_4x8>() )
        return false;
#endif

    return true;
}

