/**
 * @file placement.h
 * @brief Tile placement class.
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

#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "lookup.h"

#include <cstring>

/// Test placement functionality.
bool test_placement();

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Placement object.
/**
 * @tparam T  tile type
 */
template <class T>
struct placement
{
    typedef placement<T> _Myt;                              ///< My type.

    typedef T tile_type;                                    ///< Tile type.
    typedef typename tile_type::lookup_type lookup_type;    ///< Lookup type.

    lookup_index_type lookup;                               ///< Lookup value.
    const tile_type *tile;                                  ///< Pointer to tile placed in this placement.

    _Myt *left;                                             ///< Pointer to placement on left.
    _Myt *top;                                              ///< Pointer to placement on top.
    _Myt *right;                                            ///< Pointer to placement on right.
    _Myt *bottom;                                           ///< Pointer to placement on bottom.

    _Myt *prev;                                             ///< Previous placement.
    _Myt *next;                                             ///< Next placement.

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    placement()
    {
        zero();
    }

    /// Constructor.
    /**
     * @warning
     * This method will copy pointers!
     *
     * @param[in] other  object to copy
     */
    placement( const _Myt &other )
    {
        copy( other );
    }

    // ========================================================================
    // Operators
    // ========================================================================

    /// Assignment operator.
    /**
     * @warning
     * This method will copy pointers!
     *
     * @param[in] other  object to copy
     * @return  reference to this
     */
    _Myt & operator = ( const _Myt &other )
    {
        copy( other );
        return *this;
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Copy placement.
    /**
     * @warning
     * This method will copy pointers!
     *
     * @param[in] other  placement to copy
     */
    void copy( const _Myt &other )
    {
        std::memcpy( this, &other, sizeof(_Myt) );
    }

    /// Place a tile.
    /**
     * @param[in] t  tile to place
     * @return  next placement
     */
    inline _Myt *place( const tile_type *t )
    {
        // set placement
        tile = t;

        // update lookups of adjacent placements
        right->lookup &= lookup_type::MASK_LEFT_EDGE_INV;
        right->lookup |= lookup_type::MASK_LEFT_EDGE & t->lookup();

        bottom->lookup &= lookup_type::MASK_TOP_EDGE_INV;
        bottom->lookup |= lookup_type::MASK_TOP_EDGE & t->lookup();

        return next;
    }

    /// Zero out placement.
    void zero()
    {
        std::memset( this, 0, sizeof(_Myt) );
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // PLACEMENT_H

