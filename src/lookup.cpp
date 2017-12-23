/**
 * @file lookup.cpp
 * @brief Classes for managing lookup values implementation.
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

#include "lookup.h"

#include <cassert>

/// Assertion of expression.
#define ASSERT( EXPR )                                      \
    assert( EXPR );                                         \
    if ( !(EXPR) )                                          \
        return false

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for lookup values.
/**
 * @tparam L  lookup type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <class L>
bool test_lookup_impl()
{
    static_assert( sizeof(lookup_index_type) == sizeof(L), "Invalid lookup size." );
    static_assert( 0 == (L::MASK_LEFT_EDGE & L::MASK_TOP_EDGE), "Bad lookup edge values." );

    const lookup_index_type index(1);

    const lookup_index_type zero( 0 );
    const lookup_index_type allbits( ~zero );

    lookup_index_type *lookup_ptr;

    L lookup;
    ASSERT( 0 == lookup.value() );

    lookup.val.left = 1;
    ASSERT( index == lookup.value() );

    L lookup_x( index );
    ASSERT( index == lookup_x.value() );

    L lookup_y( lookup_x );
    ASSERT( index == lookup_y.value() );

    L lookup_test;
    lookup_test.val.left = L::LEFT_EDGE;
    lookup_test.val.top = L::TOP_EDGE;

    ASSERT( lookup_test.value() == (L::MASK_LEFT_EDGE | L::MASK_TOP_EDGE) );

    L lookup_r;
    lookup_r.val.right = 1;

    lookup_ptr = reinterpret_cast<lookup_index_type*>( &lookup_test );
    lookup_ptr[0] += 1;

    ASSERT( lookup_test.value() == lookup_r.value() );

    L lookup_b;
    lookup_b.val.bottom = 1;

    lookup_ptr = reinterpret_cast<lookup_index_type*>( &lookup_test );
    lookup_ptr[0] <<= 1;

    ASSERT( lookup_test.value() == lookup_b.value() );

    L lookup_ind;
    lookup_ind.val.left = L::LEFT_EDGE;
    lookup_ind.val.top = L::TOP_EDGE;
    lookup_ind.val.right = 1;
    lookup_ind.val.bottom = 1;

    ASSERT( lookup_ind.value() == (L::MAX_INDICES - 1) );

    L lookup_l_xor;
    lookup_l_xor.val.left = L::LEFT_EDGE;

    ASSERT( L::MASK_LEFT_EDGE_INV == (lookup_l_xor.value() ^ allbits) );

    L lookup_t_xor;
    lookup_t_xor.val.top = L::TOP_EDGE;

    ASSERT( L::MASK_TOP_EDGE_INV == (lookup_t_xor.value() ^ allbits) );

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool test_lookup()
{
    // test all variations
    if ( !test_lookup_impl<e32::lookup_1x1>() )
        return false;
    else if ( !test_lookup_impl<e32::lookup_1x2>() )
        return false;
    else if ( !test_lookup_impl<e32::lookup_2x1>() )
        return false;
    else if ( !test_lookup_impl<e32::lookup_2x2>() )
        return false;
    else if ( !test_lookup_impl<e32::lookup_2x4>() )
        return false;
    else if ( !test_lookup_impl<e32::lookup_4x2>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_lookup_impl<e32::lookup_4x4>() )
        return false;
    else if ( !test_lookup_impl<e32::lookup_4x8>() )
        return false;
    else if ( !test_lookup_impl<e32::lookup_8x4>() )
        return false;
#endif

    if ( !test_lookup_impl<e16::lookup_1x1>() )
        return false;
    else if ( !test_lookup_impl<e16::lookup_1x2>() )
        return false;
    else if ( !test_lookup_impl<e16::lookup_2x1>() )
        return false;
    else if ( !test_lookup_impl<e16::lookup_2x2>() )
        return false;
    else if ( !test_lookup_impl<e16::lookup_2x4>() )
        return false;
    else if ( !test_lookup_impl<e16::lookup_4x2>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_lookup_impl<e16::lookup_4x4>() )
        return false;
    else if ( !test_lookup_impl<e16::lookup_4x8>() )
        return false;
    else if ( !test_lookup_impl<e16::lookup_8x4>() )
        return false;
#endif

    if ( !test_lookup_impl<e8::lookup_1x1>() )
        return false;
    else if ( !test_lookup_impl<e8::lookup_1x2>() )
        return false;
    else if ( !test_lookup_impl<e8::lookup_2x1>() )
        return false;
    else if ( !test_lookup_impl<e8::lookup_2x2>() )
        return false;
    else if ( !test_lookup_impl<e8::lookup_2x4>() )
        return false;
    else if ( !test_lookup_impl<e8::lookup_4x2>() )
        return false;
    else if ( !test_lookup_impl<e8::lookup_4x4>() )
        return false;
#if (64 <= _ENV_BITS)
    else if ( !test_lookup_impl<e8::lookup_4x8>() )
        return false;
    else if ( !test_lookup_impl<e8::lookup_8x4>() )
        return false;
#endif

    return true;
}
