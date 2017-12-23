/**
 * @file piece.h
 * @brief Piece class.
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

#ifndef PIECE_H
#define PIECE_H

#include "env.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Piece.
struct piece
{
    typedef piece _Myt;                                     ///< My type.

    env_word_type left;                                     ///< Left edge value.
    env_word_type top;                                      ///< Top edge value.
    env_word_type right;                                    ///< Right edge value.
    env_word_type bottom;                                   ///< Bottom edge value.

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    piece()
    {
        zero();
    }

    /// Constructor.
    /**
     * @param[in] l  left edge
     * @param[in] t  top edge
     * @param[in] r  right edge
     * @param[in] b  bottom edge
     */
    piece( env_word_type l, env_word_type t, env_word_type r, env_word_type b ) :
        left( l ),
        top( t ),
        right( r ),
        bottom( b )
    {
    }

    /// Constructor.
    /**
     * @param[in] other  object to copy
     */
    piece( const _Myt & other )
    {
        copy( other );
    }

    // ========================================================================
    // Operators
    // ========================================================================

    /// Assignment operator.
    /**
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

    /// Copy piece.
    /**
     * @param[in] other  piece to copy
     */
    void copy( const _Myt &other )
    {
        left = other.left;
        top = other.top;
        right = other.right;
        bottom = other.bottom;
    }

    /// Zero out piece.
    void zero()
    {
        left = top = right = bottom = 0;
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // PIECE_H

