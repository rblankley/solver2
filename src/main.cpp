/**
 * @file main.cpp
 * @brief Entry point for the solver application.
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
 *
 * @page description Description of Algorithm
 *
 * This page describes the general process of how this implementation works.
 *
 * @section sec1 Pieces and initial rotations
 *
 * First we load in the pieces from a file. The file format is very basic, one piece per line and
 * each edge separated by a space. Example:
 * ~~~~
 * 0 0 1 2
 * 1 0 0 4
 * 0 2 3 0
 * 3 4 0 0
 * ~~~~
 * 
 * After this we will create an initial rotation list. For this we use the @c tile_1x1 classes. We
 * create one rotation only for the top left corner and then three rotations for the other
 * corners. This is to prevent duplicate solutions from being found.
 *
 * Some simplification is performed in this step. If a piece has all matching sides only one
 * rotation is created. Similarly if a piece has opposite sides matching only two rotations are
 * generated.
 *
 * As an example, this tile will only be rotated once. Note all four rotations would match.
 * ~~~~
 * +-----+
 * |\ 3 /|
 * | \ / |
 * |3 x 3|
 * | / \ |
 * |/ 3 \|
 * +-----+
 * ~~~~
 *
 * Another example, this piece only has two unique rotations.
 * ~~~~
 * +-----+
 * |\ 3 /|
 * | \ / |
 * |5 x 5|
 * | / \ |
 * |/ 3 \|
 * +-----+
 * ~~~~
 *
 * @section sec2 Bridge building
 *
 * Once we have initial rotations, we set about combining them into larger rotations. We do this
 * to speed up the solver process. Why spend time evaluating two (or more) rotations as separate
 * steps when we can accomplish the same thing in only a single step with a larger tile? The code
 * to evaluate a rotation will take the same amount of time no matter how large the rotation.
 *
 * The only drawback to this idea is it consumes more resources. In some cases, a *lot* more
 * resources. Number of rotations will grow exponentially in most cases.
 *
 * For this process we use two boards, a vertical one and a horizontal one.
 *
 * The @c verticalBoard looks as follows:
 * ~~~~
 * +-----+
 * |     |
 * |     |
 * |     |
 * |     |
 * |     |
 * +-----+
 * |     |
 * |     |
 * |     |
 * |     |
 * |     |
 * +-----+
 * ~~~~
 *
 * The @c horizontalBoard looks as follows:
 * ~~~~
 * +-----+-----+
 * |     |     |
 * |     |     |
 * |     |     |
 * |     |     |
 * |     |     |
 * +-----+-----+
 * ~~~~
 *
 * We combine 1x1 rotations into 1x2 rotations using the vertical board. Then we take the 1x2
 * rotations generated from this process and combine them into 2x2 rotations using the horizontal
 * board. And so on and so forth until complete. On a 64 bit system the largest rotation we can
 * generate is 4x8. Chances are you ran out of memory and crashed by then, however.
 *
 * You can limit rotation growth by telling the implementation to stop after reaching a certain
 * rotation size (i.e. 2x2). This is a trial and error process, it will depend on how much memory
 * you have available... you may get more (or less).
 *
 * @section sec3 Indexing
 *
 * Rotations are indexed by their left and top edge value. We also evaluate the right and bottom
 * edge but here we are only interested in whether or not those edges are a border (value of zero)
 * or not.
 *
 * Consider the following rotation:
 * ~~~~
 * +-----+    This rotation would have the following indexed value:
 * |\ 5 /|
 * | \ / |     reserved bit bit x bits x bits
 * |2 x 0|    +--------+---+---+------+------+
 * | / \ |    |        | 1 | 0 |   5  |   2  |
 * |/ 4 \|    +--------+---+---+------+------+
 * +-----+
 * ~~~~
 *
 * Another example:
 * ~~~~
 * +-----+    This rotation would have the following indexed value:
 * |\ 7 /|
 * | \ / |     reserved bit bit x bits x bits
 * |6 x 2|    +--------+---+---+------+------+
 * | / \ |    |        | 0 | 1 |   7  |   6  |
 * |/ 0 \|    +--------+---+---+------+------+
 * +-----+
 * ~~~~
 *
 * In addition to the above, we also generate some special indices for when we want any
 * non-border edge. For this we set all the bits of an edge. We produce three more indices
 * for each tile.
 * ~~~~
 * From this rotation the following indices are also generated:
 * 
 * +-----+     reserved bit bit x bits x bits
 * |\ 2 /|    +--------+---+---+------+------+
 * | \ / |    |        | 1 | 1 |  0xF |   3  |
 * |3 x 2|    +--------+---+---+------+------+
 * | / \ |    |        | 1 | 1 |   2  |  0xF |
 * |/ 4 \|    +--------+---+---+------+------+
 * +-----+    |        | 1 | 1 |  0xF |  0xF |
 *            +--------+---+---+------+------+
 * ~~~~
 *
 * So now when we want a rotation with no borders, for example, we will get back lots of
 * results since multiple rotations match the last index above.
 *
 * @section sec4 Board Creation
 *
 * Boards are created from placements. Each placement has an index lookup that gets updated when
 * rotations are placed and also a pointer to the rotation that resides there. Consider the
 * following board:
 * ~~~~
 * +-----+-----+-----+    For a 3x3 board, this is how the index values would look for each
 * |\ 0 /|\ 0 /|\ 0 /|    placement. The zero (0) value indicates a border and the all bits set
 * | \ / | \ / | \ / |    value represents "any" edge value that is not a border.
 * |0 x 1|F x 1|F x 0|
 * | / \ | / \ | / \ |    In the placements to the left I could query my database for the
 * |/ 1 \|/ 1 \|/ 1 \|    index value and get multiple rotation results back.
 * +-----+-----+-----+
 * |\ F /|\ F /|\ F /|    For example, the middle placement (1,1) has the following index:
 * | \ / | \ / | \ / |
 * |0 x 1|F x 1|F x 0|     reserved bit bit x bits x bits
 * | / \ | / \ | / \ |    +--------+---+---+------+------+
 * |/ 1 \|/ 1 \|/ 1 \|    |        | 1 | 1 |  0xF |  0xF |
 * +-----+-----+-----+    +--------+---+---+------+------+
 * |\ F /|\ F /|\ F /|
 * | \ / | \ / | \ / |    As another example, the bottom left corner (0,2) has this index:
 * |0 x 1|F x 1|F x 0|
 * | / \ | / \ | / \ |     reserved bit bit x bits x bits
 * |/ 0 \|/ 0 \|/ 0 \|    +--------+---+---+------+------+
 * +-----+-----+-----+    |        | 0 | 1 |  0xF |   0 |
 *                        +--------+---+---+------+------+
 * ~~~~
 *
 * The board also has a @c bitwiseMask value which represents all the rotations we have placed. It
 * begins with a value of zero (0) since no pieces have been placed.
 *
 * @section sec5 Solving
 *
 * Now we may set about solving our board. We will start in the upper left corner and proceed
 * downward until we have no more placements, then start back up at the top again in the adjacent
 * column. We will follow this pattern until we run out of placements.
 *
 * In the above example, the upper left corner appears as follows:
 * ~~~~
 *  reserved bit bit x bits x bits
 * +--------+---+---+------+------+
 * |        | 1 | 1 |   0  |   0  |
 * +--------+---+---+------+------+
 * ~~~~
 *
 * So we lookup this index and get lots of rotations back. Suppose the first rotation in the list
 * is made up from pieces 0,4,13,31. We check the mask value and see none of these pieces are used
 * therefore we may proceed with this rotation. We update the mask with the pieces and update our
 * placement with this rotation pointer (so we may print it later, if desired) and finally we
 * update the adjacent indices. Note we only need to update the right and bottom indices.
 *
 * Supposing our rotation looks as follows:
 * ~~~~
 * +-----+
 * |\ 0 /|
 * | \ / |
 * |0 x 6|
 * | / \ |
 * |/ 7 \|
 * +-----+
 * ~~~~
 * 
 * Our modified board would be updated as such:
 * ~~~~
 * +-----+-----+-----+
 * |     |\ 0 /|\ 0 /|    The adjacent placements at (0,1) and (1,0) have been updated with the
 * |     | \ / | \ / |    rotation we placed at (0,0).
 * |     |6 x 1|F x 0|    
 * |     | / \ | / \ |    Notice that in the updated placements we are now looking up a specific
 * |     |/ 1 \|/ 1 \|    edge value instead of the "any" edge value (0xF) and as a result we will
 * +-----+-----+-----+    have less rotations to evaluate.
 * |\ 7 /|\ F /|\ F /|
 * | \ / | \ / | \ / |
 * |0 x 1|F x 1|F x 0|
 * | / \ | / \ | / \ |
 * |/ 1 \|/ 1 \|/ 1 \|
 * +-----+-----+-----+
 * |\ F /|\ F /|\ F /|
 * | \ / | \ / | \ / |
 * |0 x 1|F x 1|F x 0|
 * | / \ | / \ | / \ |
 * |/ 0 \|/ 0 \|/ 0 \|
 * +-----+-----+-----+
 * ~~~~
 *
 * So now we move on to placement (0,1) which has the following index:
 * ~~~~
 *  reserved bit bit x bits x bits
 * +--------+---+---+------+------+
 * |        | 1 | 1 |   7  |   0  |
 * +--------+---+---+------+------+
 * ~~~~
 *
 * We lookup this index and get lots of rotations back. Suppose the first rotation in the list is
 * made up from pieces 2,10,13,23. We check the mask value and see that piece 13 has already been
 * used. No problem, we move on to the next piece until we find one where all the pieces are
 * available for use.
 *
 * If we happen to iterate through all our rotations (i.e. all the pieces are being used) we give
 * up and backtrack.
 *
 * This process is repeated until we have no more placements (the board is solved).
 *
 * @section sec6 Implementation Notes
 *
 * Everything I have said so far applies to any size rotation be it 1x1, 1x2, 2x2, etc. It also
 * applies to any sized board with any number of pieces.
 *
 * Because of this I have chosen to implement this solver using templating. The template code is
 * written generically so the compiler will write the 1x1, 1x2, 2x2, etc. logic automatically. This
 * also results in faster code since if we only need 3 bits to store an edge value (versus 5) or I
 * only have 32 pieces (versus 256) I can optimize things and get faster results. Of course the
 * trade off is a larger binary but who cares.
 */

#include "solver.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

static const std::string version( "2.0.0 rc1" );                            ///< Version string.

env_word_type useTileSize( 1 );                                             ///< Tile size usage.

bool print( false );                                                        ///< Print solution when @c true.
bool quit( false );                                                         ///< Quit solving after solution found when @c true.
bool randomize( false );                                                    ///< Randomizes tiles before solving when @c true.
bool threaded( false );                                                     ///< Solve using threading when @c true.
abstractBoard::type type( abstractBoard::typeNormal );                      ///< Board type to solve.

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Solver function.
/**
 * @param[in] board  board to solve
 */
void solve( abstractBoard *board )
{
    board->solve();
}

/// Solver function.
/**
 * This method sets up board parameters and solves.
 *
 * @param[in] boards  boards to solve
 */
void solve_boards( std::vector<std::shared_ptr<abstractBoard>> &boards )
{
    typedef std::chrono::high_resolution_clock clock_type;

    std::cout << "solving..." << std::endl;

    // setup parameters
    for ( auto &b : boards )
    {
        b->setPrint( print );
        b->setQuit( quit );
        b->setThreaded( threaded );
        b->setType( type );
    }

    // time point of start
    auto start( clock_type::now() );

    // solve!
    if ( !threaded )
    {
        for ( auto &b : boards )
            solve( b.get() );
    }
    else
    {
        std::vector<std::future<void>> work;

        // feed the cpu...
        for ( auto &b : boards )
            work.push_back( std::async( std::launch::async, &solve, b.get() ) );

        // wait for completion
        for ( const auto &w : work )
            w.wait();
    }

    // time point of stop
    auto diff( std::chrono::duration_cast<std::chrono::milliseconds>( clock_type::now() - start ) );

    // sum solution count
    env_word_type solutions( 0 );

    for ( const auto &b : boards )
        solutions += b->solutions();

    std::cout << "found " << solutions << " solutions in " << diff.count() << " ms" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Solver function.
/**
 * This method builds up piece rotations for a smaller board.
 *
 * @tparam A  solver tile array lookup type
 * @param[in] w  board width
 * @param[in] h  board height
 * @param[in] pieces  array of pieces
 */
template <class A>
void solve_for_tile_array_lookups( env_word_type w, env_word_type h, const pieceVector &pieces )
{
    typedef typename A::lookup_1x1_type lookup_1x1_type;
    typedef typename A::lookup_1x2_type lookup_1x2_type;
    typedef typename A::lookup_2x1_type lookup_2x1_type;
    typedef typename A::lookup_2x2_type lookup_2x2_type;
    typedef typename A::lookup_2x4_type lookup_2x4_type;
    typedef typename A::lookup_4x2_type lookup_4x2_type;

    lookup_1x1_type lookups_1x1;
    lookup_1x2_type lookups_1x2;
    lookup_2x1_type lookups_2x1;
    lookup_2x2_type lookups_2x2;
    lookup_2x4_type lookups_2x4;
    lookup_4x2_type lookups_4x2;

#if (64 == _ENV_BITS)
    typedef typename A::lookup_4x4_type lookup_4x4_type;
    typedef typename A::lookup_4x8_type lookup_4x8_type;
    typedef typename A::lookup_8x4_type lookup_8x4_type;

    lookup_4x4_type lookups_4x4;
    lookup_4x8_type lookups_4x8;
    lookup_8x4_type lookups_8x4;
#endif

    const env_word_type len_max( std::max( w, h ) );
    const env_word_type len_min( std::min( w, h ) );

    const bool square( len_max == len_min );

    const bool need_2(( 4 <= len_max ) && ( 0 == (len_max % 2) ) && ( 2 <= useTileSize ));
    const bool need_4(( need_2 ) && ( 4 <= len_min ) && ( 0 == (len_min % 2) ) && ( 4 <= useTileSize ));
    const bool need_8(( need_4 ) && ( 8 <= len_max ) && ( 0 == (len_max % 4) ) && ( 8 <= useTileSize ));
    const bool need_16(( need_8 ) && ( 8 <= len_min ) && ( 0 == (len_min % 4) ) && ( 16 <= useTileSize ));
    const bool need_32(( need_16 ) && ( 16 <= len_max ) && ( 0 == (len_max % 8) ) && ( 32 <= useTileSize ));
    const bool need_64( false );

    // ------------------------------
    // generate rotations and lookups
    // ------------------------------

    // 1x1
    pieces.generateLookups( lookups_1x1 );

    std::cout << "1x1: " << lookups_1x1.tiles() << " rotations" << std::endl;

    // 1x2
    if ( need_2 )
    {
        verticalBoard<lookup_1x1_type, lookup_1x2_type> b_1x2( &lookups_1x1, &lookups_1x2 );
        b_1x2.setThreaded( threaded );
        b_1x2.solve();

        std::cout << "1x2: " << lookups_1x2.tiles() << " rotations" << std::endl;
    }

    // 2x1
    if (( need_2 ) && ( !need_4 ) && ( !square ))
    {
        horizontalBoard<lookup_1x1_type, lookup_2x1_type> b_2x1( &lookups_1x1, &lookups_2x1 );
        b_2x1.setThreaded( threaded );
        b_2x1.solve();

        std::cout << "2x1: " << lookups_2x1.tiles() << " rotations" << std::endl;
    }

    // 2x2
    if ( need_4 )
    {
        horizontalBoard<lookup_1x2_type, lookup_2x2_type> b_2x2( &lookups_1x2, &lookups_2x2 );
        b_2x2.setThreaded( threaded );
        b_2x2.solve();

        std::cout << "2x2: " << lookups_2x2.tiles() << " rotations" << std::endl;
    }

    // 2x4
    if ( need_8 )
    {
        verticalBoard<lookup_2x2_type, lookup_2x4_type> b_2x4( &lookups_2x2, &lookups_2x4 );
        b_2x4.setThreaded( threaded );
        b_2x4.solve();

        std::cout << "2x4: " << lookups_2x4.tiles() << " rotations" << std::endl;
    }

    // 4x2
    if (( need_8 ) && ( !need_16 ) && ( !square ))
    {
        horizontalBoard<lookup_2x2_type, lookup_4x2_type> b_4x2( &lookups_2x2, &lookups_4x2 );
        b_4x2.setThreaded( threaded );
        b_4x2.solve();

        std::cout << "4x2: " << lookups_4x2.tiles() << " rotations" << std::endl;
    }

#if (64 == _ENV_BITS)

    // 4x4
    if ( need_16 )
    {
        horizontalBoard<lookup_2x4_type, lookup_4x4_type> b_4x4( &lookups_2x4, &lookups_4x4 );
        b_4x4.setThreaded( threaded );
        b_4x4.solve();

        std::cout << "4x4: " << lookups_4x4.tiles() << " rotations" << std::endl;
    }

    // 4x8
    if ( need_32 )
    {
        verticalBoard<lookup_4x4_type, lookup_4x8_type> b_4x8( &lookups_4x4, &lookups_4x8 );
        b_4x8.setThreaded( threaded );
        b_4x8.solve();

        std::cout << "4x8: " << lookups_4x8.tiles() << " rotations" << std::endl;
    }

    // 8x4
    if (( need_32 ) && ( !need_64 ) && ( !square ))
    {
        horizontalBoard<lookup_4x4_type, lookup_8x4_type> b_8x4( &lookups_4x4, &lookups_8x4 );
        b_8x4.setThreaded( threaded );
        b_8x4.solve();

        std::cout << "8x4: " << lookups_8x4.tiles() << " rotations" << std::endl;
    }

#endif

    // -------------------
    // randomize rotations
    // -------------------

    if ( randomize )
    {
        std::cout << "randomizing..." << std::endl;

#if (64 == _ENV_BITS)

        if ( lookups_4x8.tiles() )
        {
            lookups_4x8.randomize();
            lookups_8x4.randomize();
        }

        else if ( lookups_4x4.tiles() )
        {
            lookups_4x4.randomize();
        }

        // this else is here on purpose!
        else
#endif

        if ( lookups_2x4.tiles() )
        {
            lookups_2x4.randomize();
            lookups_4x2.randomize();
        }

        else if ( lookups_2x2.tiles() )
        {
            lookups_2x2.randomize();
        }

        else if ( lookups_1x2.tiles() )
        {
            lookups_1x2.randomize();
            lookups_2x1.randomize();
        }

        else
        {
            lookups_1x1.randomize();
        }
    }

    // ------------------------
    // prep board(s) for solver
    // ------------------------

    std::vector<std::shared_ptr<abstractBoard>> boards;

#if (64 == _ENV_BITS)

    if ( lookups_4x8.tiles() )
    {
        boards.push_back( std::make_shared<board<lookup_4x8_type>>( len_min/4, len_max/8, &lookups_4x8 ) );

        if ( !square )
            boards.push_back( std::make_shared<board<lookup_8x4_type>>( len_max/8, len_min/4, &lookups_8x4 ) );
    }

    else if ( lookups_4x4.tiles() )
    {
        boards.push_back( std::make_shared<board<lookup_4x4_type>>( len_min/4, len_max/4, &lookups_4x4 ) );

        if ( !square )
            boards.push_back( std::make_shared<board<lookup_4x4_type>>( len_max/4, len_min/4, &lookups_4x4 ) );
    }

    // this else is here on purpose!
    else
#endif

    if ( lookups_2x4.tiles() )
    {
        boards.push_back( std::make_shared<board<lookup_2x4_type>>( len_min/2, len_max/4, &lookups_2x4 ) );

        if ( !square )
            boards.push_back( std::make_shared<board<lookup_4x2_type>>( len_max/4, len_min/2, &lookups_4x2 ) );
    }

    else if ( lookups_2x2.tiles() )
    {
        boards.push_back( std::make_shared<board<lookup_2x2_type>>( len_min/2, len_max/2, &lookups_2x2 ) );

        if ( !square )
            boards.push_back( std::make_shared<board<lookup_2x2_type>>( len_max/2, len_min/2, &lookups_2x2 ) );
    }

    else if ( lookups_1x2.tiles() )
    {
        boards.push_back( std::make_shared<board<lookup_1x2_type>>( len_min, len_max/2, &lookups_1x2 ) );

        if ( !square )
            boards.push_back( std::make_shared<board<lookup_2x1_type>>( len_max/2, len_min, &lookups_2x1 ) );
    }

    else
    {
        boards.push_back( std::make_shared<board<lookup_1x1_type>>( len_min, len_max, &lookups_1x1 ) );

        if ( !square )
            boards.push_back( std::make_shared<board<lookup_1x1_type>>( len_max, len_min, &lookups_1x1 ) );
    }

    // -----
    // solve
    // -----

    solve_boards( boards );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Solver function.
/**
 * This method determines what mask is used for solving.
 *
 * @tparam M  mask type
 * @param[in] w  board width
 * @param[in] h  board height
 * @param[in] pieces  array of pieces
 */
template <class M>
void solve_for_mask( env_word_type w, env_word_type h, const pieceVector &pieces )
{
    const env_word_type edgesCount( pieces.edges_count() );

    // the 8th (or 16th or 32nd) edge is reserved to mean "any non-zero edge" in the lookup logic
    // so puzzles with 8 edges require a 16 edge solver
    if ( edgesCount < 8 )
        solve_for_tile_array_lookups<e8::tileArrayLookups<M>>( w, h, pieces );
    else if ( edgesCount < 16 )
        solve_for_tile_array_lookups<e16::tileArrayLookups<M>>( w, h, pieces );
    else if ( edgesCount < 32 )
        solve_for_tile_array_lookups<e32::tileArrayLookups<M>>( w, h, pieces );
    else
        std::cout << "too many edges!" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Application entry point.
/**
 * @param[in] argc  argument count
 * @param[in] argv  arguments
 * @return  zero (0) upon success, non-zero upon failure
 */
int main( int argc, char *argv[] )
{
    std::vector<std::string> args;
    std::vector<std::string> options;

    std::string params;

    // parse arguments
    while ( 1 < argc-- )
    {
        const std::string s( argv[argc] );

        if ( 0 == s.find( "--" ) )
            options.push_back( s );
        else if ( 0 == s.find( "-" ) )
            params.append( s.substr( 1 ) );
        else
            args.push_back( s );
    }

    // perform validation tests
    if ( options.end() != std::find( options.begin(), options.end(), "--validate" ) )
    {
        std::cout << "running validation suite... ";

        if (( test_bitwisemask() ) &&
            ( test_lookup() ) &&
            ( test_tile() ) &&
            ( test_placement() ) &&
            ( test_board() ))
        {
            std::cout << "success" << std::endl;
        }
        else
        {
            std::cout << "FAIL!" << std::endl;
        }
    }

    // show version information
    else if ( options.end() != std::find( options.begin(), options.end(), "--version" ) )
    {
        std::cout << "solver " << version << " " << __DATE__ << " " << __TIME__ << std::endl;
        std::cout << "Copyright (C) 2015-2017 Randy Blankley" << std::endl;
        std::cout << "This is free software; see the source for copying conditions. There is NO" << std::endl;
        std::cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
        std::cout << std::endl;
    }

    // show usage
    else if (( 3 != args.size() ) || ( options.end() != std::find( options.begin(), options.end(), "--help" ) ))
    {
        std::cout << "Usage: solver [options] bw bh pieces" << std::endl;
        std::cout << std::endl;
        std::cout << "Required:" << std::endl;
        std::cout << "    bw              board width" << std::endl;
        std::cout << "    bh              board height" << std::endl;
        std::cout << "    pieces          filename where pieces are stored" << std::endl;
        std::cout << std::endl;
        std::cout << "Optional:" << std::endl;
        std::cout << "    --help          show usage" << std::endl;
        std::cout << "    --version       show version information" << std::endl;
        std::cout << "    --validate      run validation suite" << std::endl;
        std::cout << std::endl;
        std::cout << "    --u2            use 1x2 or 2x1 tiles and smaller" << std::endl;
        std::cout << "    --u4            use 2x2 tiles and smaller" << std::endl;
        std::cout << "    --u8            use 2x4 or 4x2 tiles and smaller" << std::endl;
#if (64 == _ENV_BITS)
        std::cout << "    --u16           use 4x4 tiles and smaller" << std::endl;
        std::cout << "    --u32           use 4x8 or 8x4 tiles and smaller" << std::endl;
#endif
        std::cout << std::endl;
        std::cout << "    --bt0           solve a normal board (all edges are borders)" << std::endl;
        std::cout << "    --bt1           solve for bottom left corner only" << std::endl;
        std::cout << "    --bt2           solve for bottom edge only" << std::endl;
        std::cout << "    --bt3           solve for bottom right corner only" << std::endl;
        std::cout << "    --bt4           solve for left edge only" << std::endl;
        std::cout << "    --bt5           solve a board with no borders" << std::endl;
        std::cout << "    --bt6           solve for right edge only" << std::endl;
        std::cout << "    --bt7           solve for top left corner only" << std::endl;
        std::cout << "    --bt8           solve for top edge only" << std::endl;
        std::cout << "    --bt9           solve for top right corner only" << std::endl;
        std::cout << std::endl;
        std::cout << "    -p              print solutions" << std::endl;
        std::cout << "    -q              quit after first solution found" << std::endl;
        std::cout << "    -r              randomize tiles before solving" << std::endl;
        std::cout << "    -t              use threaded solving" << std::endl;
        std::cout << std::endl;
    }

    // solve!
    else
    {
        pieceVector pieces;

        env_word_type bw( std::stoul( args[2] ) );
        env_word_type bh( std::stoul( args[1] ) );

        // validate board width and height
        if (( bw < 2 ) || ( bh < 2 ))
            std::cout << "invalid board size!" << std::endl;

        // attempt to load pieces
        else if ( !pieces.load( args[0] ) )
            std::cout << "failed to load pieces!" << std::endl;

        else
        {
            std::cout << "loaded " << pieces.size() << " pieces" << std::endl;

            // check for tile usage
            if ( options.end() != std::find( options.begin(), options.end(), "--u2" ) )
                useTileSize = 2;
            else if ( options.end() != std::find( options.begin(), options.end(), "--u4" ) )
                useTileSize = 4;
            else if ( options.end() != std::find( options.begin(), options.end(), "--u8" ) )
                useTileSize = 8;
#if (64 == _ENV_BITS)
            else if ( options.end() != std::find( options.begin(), options.end(), "--u16" ) )
                useTileSize = 16;
            else if ( options.end() != std::find( options.begin(), options.end(), "--u32" ) )
                useTileSize = 32;
#endif

            // check for board types
            //
            // the board type numbers match up with the numbers on a keyboard:
            // +---+---+---+
            // | 7 | 8 | 9 |  examples:
            // +---+---+---+  2 would solve a board with only the bottom edge set
            // | 4 | 5 | 6 |  4 would solve a board with only the left edge set
            // +---+---+---+  5 would solve a board with no edges
            // | 1 | 2 | 3 |  9 would solve a board with the top right corner edges set
            // +---+---+---+
            if ( options.end() != std::find( options.begin(), options.end(), "--bt7" ) )
                type = abstractBoard::typeTopLeftCorner;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt8" ) )
                type = abstractBoard::typeTopEdge;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt9" ) )
                type = abstractBoard::typeTopRightCorner;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt4" ) )
                type = abstractBoard::typeLeftEdge;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt5" ) )
                type = abstractBoard::typeMiddle;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt6" ) )
                type = abstractBoard::typeRightEdge;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt1" ) )
                type = abstractBoard::typeBottomLeftCorner;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt2" ) )
                type = abstractBoard::typeBottomEdge;
            else if ( options.end() != std::find( options.begin(), options.end(), "--bt3" ) )
                type = abstractBoard::typeBottomRightCorner;

            // check params
            print = (std::string::npos != params.find( 'p' ));
            quit = (std::string::npos != params.find( 'q' ));
            randomize = (std::string::npos != params.find( 'r' ));
            threaded = (std::string::npos != params.find( 't' ));

            // determine mask size (based on piece count)
            if ( pieces.size() <= 32 )
                solve_for_mask<mask_32>( bw, bh, pieces );
            else if ( pieces.size() <= 64 )
                solve_for_mask<mask_64>( bw, bh, pieces );
            else if ( pieces.size() <= 128 )
                solve_for_mask<mask_128>( bw, bh, pieces );
            else if ( pieces.size() <= 256 )
                solve_for_mask<mask_256>( bw, bh, pieces );
#if (64 == _ENV_BITS)
            else if ( pieces.size() <= 512 )
                solve_for_mask<mask_512>( bw, bh, pieces );
#endif
            else
                std::cout << "too many pieces!" << std::endl;
        }
    }

    return 0;
}
