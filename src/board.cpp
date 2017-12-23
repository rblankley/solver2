/**
 * @file board.h
 * @brief Board class implementation.
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
#include "board.h"
#include "boardh.h"
#include "boardv.h"
#include "piecevector.h"
#include "placement.h"
#include "tile.h"
#include "tilearraylookuparray.h"
#include "tilearraylookupmap.h"
#include "tilearraylookupvector.h"

#include <cassert>

/// Assertion of expression.
#define ASSERT( EXPR )                                      \
    assert( EXPR );                                         \
    if ( !(EXPR) )                                          \
        return false

std::mutex abstractBoard::m_;

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for @c board.
/**
 * @tparam L1  lookup 1x1 type
 * @tparam L2V  lookup 1x2 type
 * @tparam L2H  lookup 2x1 type
 * @tparam L4  lookup 2x2 type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <class L1, class L2V, class L2H, class L4>
bool test_board_impl( env_word_type w, env_word_type h, const pieceVector &pieces )
{
    //
    // 1x1
    //

    L1 *lookups_1x1( new L1 );

    pieces.generateLookups( *lookups_1x1 );

    ASSERT( 58 == lookups_1x1->tiles() );

    // solve single thread
    {
        board<L1> b( w, h, lookups_1x1 );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    // solve threaded
    {
        board<L1> b( w, h, lookups_1x1 );
        b.setThreaded( true );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    //
    // 1x2
    //

    L2V *lookups_1x2( new L2V );
    L2V *lookups_1x2_t( new L2V );

    // solve single thread
    {
        verticalBoard<L1, L2V> b_1x2( lookups_1x1, lookups_1x2 );
        b_1x2.solve();

        ASSERT( 316 == lookups_1x2->tiles() );

        board<L2V> b( w, h/2, lookups_1x2 );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    // solve threaded
    {
        verticalBoard<L1, L2V> b_1x2( lookups_1x1, lookups_1x2_t );
        b_1x2.setThreaded( true );
        b_1x2.solve();

        ASSERT( 316 == lookups_1x2_t->tiles() );

        board<L2V> b( w, h/2, lookups_1x2_t );
        b.setThreaded( true );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    //
    // 2x1
    //

    L2H *lookups_2x1( new L2H );
    L2H *lookups_2x1_t( new L2H );

    // solve single thread
    {
        horizontalBoard<L1, L2H> b_2x1( lookups_1x1, lookups_2x1 );
        b_2x1.solve();

        ASSERT( 316 == lookups_2x1->tiles() );

        board<L2H> b( w/2, h, lookups_2x1 );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    // solve threaded
    {
        horizontalBoard<L1, L2H> b_2x1( lookups_1x1, lookups_2x1_t );
        b_2x1.setThreaded( true );
        b_2x1.solve();

        ASSERT( 316 == lookups_2x1_t->tiles() );

        board<L2H> b( w/2, h, lookups_2x1_t );
        b.setThreaded( true );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    //
    // 2x2
    //

    L4 *lookups_2x2( new L4 );
    L4 *lookups_2x2_t( new L4 );

    // solve single thread
    {
        verticalBoard<L2H, L4> b_2x2( lookups_2x1, lookups_2x2 );
        b_2x2.solve();

        ASSERT( 3472 == lookups_2x2->tiles() );

        board<L4> b( w/2, h/2, lookups_2x2 );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    // solve threaded
    {
        horizontalBoard<L2V, L4> b_2x2( lookups_1x2_t, lookups_2x2_t );
        b_2x2.setThreaded( true );
        b_2x2.solve();

        ASSERT( 3472 == lookups_2x2_t->tiles() );

        board<L4> b( w/2, h/2, lookups_2x2_t );
        b.setThreaded( true );
        b.solve();

        ASSERT( 640 == b.solutions() );
    }

    // cleanup
    delete lookups_2x2_t;
    delete lookups_2x2;

    delete lookups_2x1_t;
    delete lookups_2x1;

    delete lookups_1x2_t;
    delete lookups_1x2;
    
    delete lookups_1x1;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for @c board.
/**
 * @tparam L  lookup type
 * @tparam M  mask type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <template <class> class L, class M>
bool test_board_all_lookups( env_word_type w, env_word_type h, const pieceVector &pieces )
{
    // test all variations
    if ( !test_board_impl<L<e8::tile_1x1<M>>, L<e8::tile_1x2<M>>, L<e8::tile_2x1<M>>, L<e8::tile_2x2<M>>>( w, h, pieces ) )
        return false;
    else if ( !test_board_impl<L<e16::tile_1x1<M>>, L<e16::tile_1x2<M>>, L<e16::tile_2x1<M>>, L<e16::tile_2x2<M>>>( w, h, pieces ) )
        return false;
    else if ( !test_board_impl<L<e32::tile_1x1<M>>, L<e32::tile_1x2<M>>, L<e32::tile_2x1<M>>, L<e32::tile_2x2<M>>>( w, h, pieces ) )
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Test method for @c board.
/**
 * @tparam M  mask type
 * @return  @c true if all tests passed, @c false otherwise
 */
template <class M>
bool test_board_all_tile_array_lookups( env_word_type w, env_word_type h, const pieceVector &pieces )
{
    // test all variations
    if ( !test_board_all_lookups<tileArrayLookupArray, M>( w, h, pieces ) )
        return false;
    else if ( !test_board_all_lookups<tileArrayLookupMap, M>( w, h, pieces ) )
        return false;
    else if ( !test_board_all_lookups<tileArrayLookupVector, M>( w, h, pieces ) )
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool test_board()
{
    pieceVector pieces;

    // generate a 16 piece puzzle
    pieces.push_back( piece( 0, 0, 1, 2 ) );
    pieces.push_back( piece( 1, 0, 2, 3 ) );
    pieces.push_back( piece( 2, 0, 1, 3 ) );
    pieces.push_back( piece( 1, 0, 0, 1 ) );

    pieces.push_back( piece( 0, 2, 3, 2 ) );
    pieces.push_back( piece( 3, 3, 4, 4 ) );
    pieces.push_back( piece( 4, 3, 3, 3 ) );
    pieces.push_back( piece( 3, 1, 0, 1 ) );

    pieces.push_back( piece( 0, 2, 4, 2 ) );
    pieces.push_back( piece( 4, 4, 3, 4 ) );
    pieces.push_back( piece( 3, 3, 4, 4 ) );
    pieces.push_back( piece( 4, 1, 0, 2 ) );

    pieces.push_back( piece( 0, 2, 1, 0 ) );
    pieces.push_back( piece( 1, 4, 1, 0 ) );
    pieces.push_back( piece( 1, 4, 2, 0 ) );
    pieces.push_back( piece( 2, 2, 0, 0 ) );

    // test all variations
    if ( !test_board_all_tile_array_lookups<bitwiseMask<1>>( 4, 4, pieces ) )
        return false;
    else if ( !test_board_all_tile_array_lookups<bitwiseMask<2>>( 4, 4, pieces ) )
        return false;
    else if ( !test_board_all_tile_array_lookups<bitwiseMask<4>>( 4, 4, pieces ) )
        return false;
    else if ( !test_board_all_tile_array_lookups<bitwiseMask<8>>( 4, 4, pieces ) )
        return false;

    return true;
}
