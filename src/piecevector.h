/**
 * @file piecevector.h
 * @brief Vector of pieces.
 *
 * @section Copyright
 * Copyright (C) 2015-2016 Randy Blankley
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

#ifndef PIECEVECTOR_H
#define PIECEVECTOR_H

#include "piece.h"
#include "tile.h"

#include <algorithm>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Piece vector.
class pieceVector : public std::vector<piece>
{
    typedef pieceVector _Myt;                               ///< My type.
    typedef std::vector<piece> _Mybase;                     ///< My base type.

public:

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    pieceVector();

    /// Constructor.
    /**
     * @param[in] other  object to copy
     */
    pieceVector( const _Myt &other );

    // ========================================================================
    // Properties
    // ========================================================================

    /// How many edges are configured.
    /**
     * @return  number of edges
     */
    virtual size_type edges_count() const;

    // ========================================================================
    // Methods
    // ========================================================================

    /// Generate tile arrays and indices from pieces.
    /**
     * Creates indexed arrays of 1x1 tiles.
     *
     * @tparam L  tile array lookup type
     * @param[out] lookup  tile lookups
     */
    template <class L>
    void generateLookups( L &lookup ) const
    {
        typedef typename L::tile_array_type tile_array_type;

        env_word_type corners( 0 );
        env_word_type pieceNum( 0 );

        // iterate pieces
        for ( const value_type &p : (*this) )
        {
            tile_array_type t;
            t.resize( 4 );

            // set piece num
            t[3].mask.setBit( pieceNum++ );
            t[0].mask = t[1].mask = t[2].mask = t[3].mask;

            // generate rotations
            t[0].val.left = t[1].val.top = t[2].val.right = t[3].val.bottom = p.left;
            t[1].val.left = t[2].val.top = t[3].val.right = t[0].val.bottom = p.top;
            t[2].val.left = t[3].val.top = t[0].val.right = t[1].val.bottom = p.right;
            t[3].val.left = t[0].val.top = t[1].val.right = t[2].val.bottom = p.bottom;

            // sort tiles
            std::sort( t.begin(), t.end(), sortTilesByEdge() );

            // corner piece
            if (( 0 == t[0].val.left ) && ( 0 == t[0].val.top ))
            {
                // only want one rotation of first corner
                if ( 0 == corners++ )
                    lookup.insert( t.begin(), t.begin() + 1 );
                else
                    lookup.insert( t.begin() + 1, t.end() );
            }

            // opposite edges same
            else if (( t[0].val.left == t[0].val.right ) && ( t[0].val.top == t[0].val.bottom ))
            {
                // all edges same
                if ( t[0].val.left == t[0].val.top )
                    lookup.insert( t.begin(), t.begin() + 1 );
                else
                    lookup.insert( t.begin(), t.begin() + 2 );
            }

            // need all rotations
            else
            {
                lookup.insert( t.begin(), t.end() );
            }
        }
    }

    /// Load pieces from file.
    /**
     * @param[in] filename  filename to load
     * @return  @c true upon success, @c false otherwise
     */
    virtual bool load( const std::string &filename );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // PIECEVECTOR_H
