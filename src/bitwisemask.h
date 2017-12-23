/**
 * @file bitwisemask.h
 * @brief Bitwise mask class.
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

#ifndef BITWISEMASK_H
#define BITWISEMASK_H

#if _WIN32 || _WIN64
#pragma warning( push )
#pragma warning( disable : 4800 )
#endif

#include "env.h"

#include <iostream>

/// Test bitwise mask functionality.
/**
 * @return  @c true if all tests passed, @c false otherwise
 */
bool test_bitwisemask();

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Bitwise masking class.
/**
 * @tparam _N  number of words in mask
 */
template <env_word_type _N>
struct bitwiseMask
{
    typedef bitwiseMask<_N> _Myt;                           ///< My type.

    // ========================================================================
    // Constants
    // ========================================================================

    /// Mask length (in words).
    static const env_word_type LENGTH = _N;

    // ========================================================================
    // Member Data
    // ========================================================================

    /// Raw mask value.
    env_word_type bits[LENGTH];

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    bitwiseMask()
    {
        zero();
    }

    /// Constructor.
    /**
     * @param[in] other  object to copy
     */
    bitwiseMask( const _Myt &other )
    {
        copy( other );
    }

    /// Constructor.
    /**
     * Initialize to the OR of other1 and other2.
     * @param[in] other1  object to copy
     * @param[in] other2  object to copy
     */
    bitwiseMask( const _Myt &other1, const _Myt &other2 )
    {
        copyCombine( other1, other2 );
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

    /// Bitwise OR operator.
    /**
     * @param[in] other  object to OR against
     * @return  reference to this
     */
    _Myt & operator |= ( const _Myt &other )
    {
        add( other );
        return *this;
    }

    // ========================================================================
    // Properties
    // ========================================================================

    /// Set specific bit in mask.
    /**
     * @param[in] bit  bit to set where 0 <= @p bit < MAX_PIECES
     */
    void setBit( env_word_type bit )
    {
        env_word_type n( 0 );

        while ( ENV_BITS <= bit )
        {
            bit -= ENV_BITS;
            ++n;
        }

        env_word_type value( 1 );
        value <<= bit;

        bits[n] |= value;
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Add bits to mask.
    /**
     * @param[in] other  mask to add
     */
    void add( const _Myt &other );

    /// Copy mask.
    /**
     * @param[in] other  mask to copy
     */
    void copy( const _Myt &other )
    {
        for ( env_word_type n( LENGTH ); n--; )
            bits[n] = other.bits[n];
    }

    /// Copy combine masks.
    /**
     * @param[in] other1  mask to copy
     * @param[in] other2  mask to copy
     */
    void copyCombine( const _Myt &other1, const _Myt &other2 );

    /// Print bits.
    void print() const
    {
        env_word_type bit( 1 );

        for ( env_word_type n( 0 ); n < LENGTH; ++n )
        {
            env_word_type value( 1 );

            do
            {
                if ( bits[n] & value )
                    std::cout << bit << " ";

                ++bit;

            } while ( value <<= 1 );
        }
    }

    /// Subtract bits from mask.
    /**
     * @param[in] other  mask to remove
     */
    void subtract( const _Myt &other );

    /// Zero out mask.
    void zero()
    {
        for ( env_word_type n( LENGTH ); n--; )
            bits[n] = 0;
    }

};

/// Add bits to mask implementation.
template <>
inline void bitwiseMask<1>::add( const bitwiseMask<1> &other )
{
    bits[0] |= other.bits[0];
}

/// Add bits to mask implementation.
template <>
inline void bitwiseMask<2>::add( const bitwiseMask<2> &other )
{
    // unrolled loop
    bits[0] |= other.bits[0];
    bits[1] |= other.bits[1];
}

/// Add bits to mask implementation.
template <>
inline void bitwiseMask<4>::add( const bitwiseMask<4> &other )
{
    // unrolled loop
    bits[0] |= other.bits[0];
    bits[1] |= other.bits[1];
    bits[2] |= other.bits[2];
    bits[3] |= other.bits[3];
}

/// Add bits to mask implementation.
template <>
inline void bitwiseMask<8>::add( const bitwiseMask<8> &other )
{
    // unrolled loop
    bits[0] |= other.bits[0];
    bits[1] |= other.bits[1];
    bits[2] |= other.bits[2];
    bits[3] |= other.bits[3];
    bits[4] |= other.bits[4];
    bits[5] |= other.bits[5];
    bits[6] |= other.bits[6];
    bits[7] |= other.bits[7];
}

/// Copy combine masks implementation.
template <>
inline void bitwiseMask<1>::copyCombine( const bitwiseMask<1> &other1, const bitwiseMask<1> &other2 )
{
    bits[0] = other1.bits[0] | other2.bits[0];
}

/// Copy combine masks implementation.
template <>
inline void bitwiseMask<2>::copyCombine( const bitwiseMask<2> &other1, const bitwiseMask<2> &other2 )
{
    // unrolled loop
    bits[0] = other1.bits[0] | other2.bits[0];
    bits[1] = other1.bits[1] | other2.bits[1];
}

/// Copy combine masks implementation.
template <>
inline void bitwiseMask<4>::copyCombine( const bitwiseMask<4> &other1, const bitwiseMask<4> &other2 )
{
    // unrolled loop
    bits[0] = other1.bits[0] | other2.bits[0];
    bits[1] = other1.bits[1] | other2.bits[1];
    bits[2] = other1.bits[2] | other2.bits[2];
    bits[3] = other1.bits[3] | other2.bits[3];
}

/// Copy combine masks implementation.
template <>
inline void bitwiseMask<8>::copyCombine( const bitwiseMask<8> &other1, const bitwiseMask<8> &other2 )
{
    // unrolled loop
    bits[0] = other1.bits[0] | other2.bits[0];
    bits[1] = other1.bits[1] | other2.bits[1];
    bits[2] = other1.bits[2] | other2.bits[2];
    bits[3] = other1.bits[3] | other2.bits[3];
    bits[4] = other1.bits[4] | other2.bits[4];
    bits[5] = other1.bits[5] | other2.bits[5];
    bits[6] = other1.bits[6] | other2.bits[6];
    bits[7] = other1.bits[7] | other2.bits[7];
}

/// Subtract bits from mask implemenation.
template <>
inline void bitwiseMask<1>::subtract( const bitwiseMask<1> &other )
{
    bits[0] &= ~other.bits[0];
}

/// Subtract bits from mask implemenation.
template <>
inline void bitwiseMask<2>::subtract( const bitwiseMask<2> &other )
{
    // unrolled loop
    bits[0] &= ~other.bits[0];
    bits[1] &= ~other.bits[1];
}

/// Subtract bits from mask implemenation.
template <>
inline void bitwiseMask<4>::subtract( const bitwiseMask<4> &other )
{
    // unrolled loop
    bits[0] &= ~other.bits[0];
    bits[1] &= ~other.bits[1];
    bits[2] &= ~other.bits[2];
    bits[3] &= ~other.bits[3];
}

/// Subtract bits from mask implemenation.
template <>
inline void bitwiseMask<8>::subtract( const bitwiseMask<8> &other )
{
    // unrolled loop
    bits[0] &= ~other.bits[0];
    bits[1] &= ~other.bits[1];
    bits[2] &= ~other.bits[2];
    bits[3] &= ~other.bits[3];
    bits[4] &= ~other.bits[4];
    bits[5] &= ~other.bits[5];
    bits[6] &= ~other.bits[6];
    bits[7] &= ~other.bits[7];
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Compare one mask to another.
/**
 * @param[in] lhs  mask to compare
 * @param[in] rhs  mask to compare
 * @return  @c true if mask share a bit, @c false otherwise
 */
inline bool operator & ( const bitwiseMask<1> &lhs, const bitwiseMask<1> &rhs )
{
    return ( lhs.bits[0] & rhs.bits[0] );
}

/// Compare one mask to another.
/**
 * @param[in] lhs  mask to compare
 * @param[in] rhs  mask to compare
 * @return  @c true if mask share a bit, @c false otherwise
 */
inline bool operator & ( const bitwiseMask<2> &lhs, const bitwiseMask<2> &rhs )
{
    // unrolled loop
    return (( lhs.bits[0] & rhs.bits[0] ) ||
            ( lhs.bits[1] & rhs.bits[1] ));
}

/// Compare one mask to another.
/**
 * @param[in] lhs  mask to compare
 * @param[in] rhs  mask to compare
 * @return  @c true if mask share a bit, @c false otherwise
 */
inline bool operator & ( const bitwiseMask<4> &lhs, const bitwiseMask<4> &rhs )
{
    // unrolled loop
    return (( lhs.bits[0] & rhs.bits[0] ) ||
            ( lhs.bits[1] & rhs.bits[1] ) ||
            ( lhs.bits[2] & rhs.bits[2] ) ||
            ( lhs.bits[3] & rhs.bits[3] ));
}

/// Compare one mask to another.
/**
 * @param[in] lhs  mask to compare
 * @param[in] rhs  mask to compare
 * @return  @c true if mask share a bit, @c false otherwise
 */
inline bool operator & ( const bitwiseMask<8> &lhs, const bitwiseMask<8> &rhs )
{
    // unrolled loop
    return (( lhs.bits[0] & rhs.bits[0] ) ||
            ( lhs.bits[1] & rhs.bits[1] ) ||
            ( lhs.bits[2] & rhs.bits[2] ) ||
            ( lhs.bits[3] & rhs.bits[3] ) ||
            ( lhs.bits[4] & rhs.bits[4] ) ||
            ( lhs.bits[5] & rhs.bits[5] ) ||
            ( lhs.bits[6] & rhs.bits[6] ) ||
            ( lhs.bits[7] & rhs.bits[7] ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32 || _WIN64
#pragma warning( pop )
#endif

#endif // BITWISEMASK_H

