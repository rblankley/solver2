/**
 * @file bitwisemask.cpp
 * @brief Bitwise mask class implementation.
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

#include <cassert>
#include <cstring>

/// Assertion of expression.
#define ASSERT( EXPR )                                      \
    assert( EXPR );                                         \
    if ( !(EXPR) )                                          \
        return false

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for @c bitwiseMask
/**
 * @tparam M  mask type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <class M>
bool test_bitwisemask_impl()
{
    static_assert( M::LENGTH * sizeof(env_word_type) == sizeof(M), "Invalid mask size." );

    const env_word_type zero[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    const env_word_type one[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
    const env_word_type one_inv[8] = { 0, 1, 1, 1, 1, 1, 1, 1 };

    const env_word_type ones[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };

    M mask;
    ASSERT( 0 == std::memcmp( &mask, zero, sizeof(mask) ) );

    mask.add( reinterpret_cast<const M*>(one)[0] );
    ASSERT( 0 == std::memcmp( &mask, one, sizeof(mask) ) );

    mask.subtract( reinterpret_cast<const M*>(ones)[0] );
    ASSERT( 0 == std::memcmp( &mask, zero, sizeof(mask) ) );

    mask |= reinterpret_cast<const M*>(one)[0];
    ASSERT( 0 == std::memcmp( &mask, one, sizeof(mask) ) );

    mask = reinterpret_cast<const M*>(zero)[0];
    ASSERT( 0 == std::memcmp( &mask, zero, sizeof(mask) ) );

    mask.setBit( 0 );
    ASSERT( 0 == std::memcmp( &mask, one, sizeof(mask) ) );

    for ( env_word_type i( M::LENGTH ); i--; )
        mask.setBit( i * ENV_BITS );

    ASSERT( 0 == std::memcmp( &mask, ones, sizeof(mask) ) );

    M mask_a;
    mask_a.add( mask );

    ASSERT( 0 == std::memcmp( &mask_a, ones, sizeof(mask) ) );

    mask_a.subtract( reinterpret_cast<const M*>(ones)[0] );
    ASSERT( 0 == std::memcmp( &mask_a, zero, sizeof(mask) ) );

    M mask_comb( reinterpret_cast<const M*>(one)[0], reinterpret_cast<const M*>(one_inv)[0] );

    ASSERT( 0 == std::memcmp( &mask_comb, ones, sizeof(mask) ) );

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool test_bitwisemask()
{
    // test all variations
    if ( !test_bitwisemask_impl<bitwiseMask<1>>() )
        return false;
    else if ( !test_bitwisemask_impl<bitwiseMask<2>>() )
        return false;
    else if ( !test_bitwisemask_impl<bitwiseMask<4>>() )
        return false;
    else if ( !test_bitwisemask_impl<bitwiseMask<8>>() )
        return false;

    return true;
}
