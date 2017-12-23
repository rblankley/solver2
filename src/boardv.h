/**
 * @file boardv.h
 * @brief Vertical board class.
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

#ifndef BOARDV_H
#define BOARDV_H

#include "board.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Vertical board class.
/**
 * This board is used to generate larger tiles.
 *
 * @tparam L  tile array lookup type
 * @tparam L0  tile array lookup type
 */
template <class L, class L0>
class verticalBoard : public board<L>
{
    typedef verticalBoard<L, L0> _Myt;                      ///< My type.
    typedef board<L> _Mybase;                               ///< My base type.

public:

    typedef typename _Mybase::tile_array_lookup_type tile_array_lookup_type;        ///< Tile array lookup type.
    typedef typename _Mybase::tile_type tile_type;                                  ///< Tile type.
    typedef typename _Mybase::lookup_type lookup_type;                              ///< Lookup type.

    typedef typename _Mybase::mask_type mask_type;                                  ///< Mask type.

    typedef typename _Mybase::placement_type placement_type;                        ///< Placement type.

    typedef L0 output_tile_array_lookup_type;                                       ///< Output tile array lookup type.
    typedef typename output_tile_array_lookup_type::tile_type output_tile_type;     ///< Output tile type.

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    /**
     * @param[in] lookups  tile lookups
     * @param[in] outputLookups  output tile lookups
     */
    verticalBoard( const tile_array_lookup_type *lookups, output_tile_array_lookup_type *outputLookups ) :
        _Mybase( 1, 2, lookups ),
        outputLookups_( outputLookups )
    {
        p0_ = _Mybase::at( 0, 0 );
        p1_ = _Mybase::at( 0, 1 );
    }

    /// Constructor.
    /**
     * @param[in] other  object to copy
     */
    verticalBoard( const _Myt &other ) :
        _Mybase( other ),
        outputLookups_( other.outputLookups_ )
    {
        p0_ = _Mybase::at( 0, 0 );
        p1_ = _Mybase::at( 0, 1 );
    }

    /// Destructor.
    virtual ~verticalBoard()
    {
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Solve board.
    virtual void solve()
    {
        static const mask_type m;

        std::list<_Myt> boards;

        // solve all board types
        _Mybase::setType( _Mybase::typeTopLeftCorner );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeTopEdge );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeTopRightCorner );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeLeftEdge );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeMiddle );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeRightEdge );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeBottomLeftCorner );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeBottomEdge );
        boards.push_back( *this );

        _Mybase::setType( _Mybase::typeBottomRightCorner );
        boards.push_back( *this );

        // solve!
        if ( !_Mybase::threaded() )
        {
            for ( auto &b : boards )
                b.solve_for( b.p0_, m );
        }
        else
        {
            std::vector<std::future<void>> work;

            // feed the cpu...
            for ( auto &b : boards )
                work.push_back( std::async( std::launch::async, &_Myt::solve_for, &b, b.p0_, m ) );

            // wait for completion
            for ( const auto &w : work )
                w.wait();
        }
    }

protected:

    output_tile_array_lookup_type *outputLookups_;          ///< Output tile lookups.

    placement_type *p0_;                                    ///< Pointer to placement index 0.
    placement_type *p1_;                                    ///< Pointer to placement index 1.

    // ========================================================================
    // Methods
    // ========================================================================

    /// Board complete.
    /**
     * This method is invoked when a solution is found.
     */
    virtual void complete()
    {
        const tile_type *t0( p0_->tile );
        const tile_type *t1( p1_->tile );

        // create new larger tile from placements
        output_tile_type t;

        t.val.left = (t1->val.left << lookup_type::LEFT_EDGE_BITS) | t0->val.left;
        t.val.top = t0->val.top;
        t.val.right = (t1->val.right << lookup_type::LEFT_EDGE_BITS) | t0->val.right;
        t.val.bottom = t1->val.bottom;

        t.mask = t0->mask;
        t.mask |= t1->mask;

        // insert!
        outputLookups_->insert( t );
    }

private:

    // not implemented
    _Myt & operator == ( const _Myt & );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // BOARDV_H

