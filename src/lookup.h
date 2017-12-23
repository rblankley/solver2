/**
 * @file lookup.h
 * @brief Classes for managing lookup values.
 *
 * The structures in this file are designed to help the solver create index values. All structures
 * should be the same size as a @c lookup_index_type type.
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

#ifndef LOOKUP_H
#define LOOKUP_H

#include "env.h"

/// Test lookup functionality.
/**
 * @return  @c true if all tests passed, @c false otherwise
 */
bool test_lookup();

/// Count number of bits in value.
/**
 * @param[in] value  value to count
 * @return  number of least significant bits set
 */
constexpr env_word_type _numBits( env_word_type value ) {return (( value & 1 ) ? (1 + _numBits( value >> 1 )) : 0);}

/// Generate a mask.
/**
 * @param[in] size  mask size in bits
 * @return  mask of length @p size
 */
constexpr env_word_type _generateMask( env_word_type size ) {return (size ? ((_generateMask( size - 1 ) << 1) | 1) : 0);}

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Lookup index type.
typedef env_word_type lookup_index_type;

/// Lookup implementation.
/**
 * Provides implementation for the lookup classes. The @p _LR and @p _TB parameters represent bits
 * in the left/right and top/bottom edges (respectfully). From these values masking is setup which
 * is used by the solver logic.
 *
 * @tparam VALUE  structure to hold edge lookup values
 * @tparam _LR  bits in left and right edges
 * @tparam _TB  bits in top and bottom edges
 */
template <class VALUE, env_word_type _LR, env_word_type _TB>
struct lookup
{
    typedef lookup<VALUE, _LR, _TB> _Myt;                   ///< My type.

    typedef VALUE value_type;                               ///< Value type.

    // ========================================================================
    // Constants
    // ========================================================================

    /// Left edge.
    static const lookup_index_type LEFT_EDGE = _generateMask( _LR );

    /// Number of bits in left edge.
    static const env_word_type LEFT_EDGE_BITS = _numBits( LEFT_EDGE );

    /// Top edge.
    static const lookup_index_type TOP_EDGE = _generateMask( _TB );

    /// Number of bits in top edge.
    static const env_word_type TOP_EDGE_BITS = _numBits( TOP_EDGE );

    /// Left edge mask.
    static const lookup_index_type MASK_LEFT_EDGE = LEFT_EDGE;

    /// Inverted left edge mask.
    static const lookup_index_type MASK_LEFT_EDGE_INV = ~MASK_LEFT_EDGE;

    /// Top edge mask.
    static const lookup_index_type MASK_TOP_EDGE = TOP_EDGE << LEFT_EDGE_BITS;

    /// Inverted top edge mask.
    static const lookup_index_type MASK_TOP_EDGE_INV = ~MASK_TOP_EDGE;

    /// Maximum number of indices.
    static const env_word_type MAX_INDICES = ((MASK_LEFT_EDGE | MASK_TOP_EDGE) + 1) << 2;

    // ========================================================================
    // Member Data
    // ========================================================================

    /// Edge lookup values.
    value_type val;

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    lookup() {setValue( 0 );}

    /// Constructor.
    /**
     * @param[in] value  initial value
     */
    lookup( lookup_index_type value ) {setValue( value );}

    /// Constructor.
    /**
     * @param[in] other  lookup value to duplicate
     */
    lookup( const _Myt &other ) {setValue( other.value() );}

    // ========================================================================
    // Operators
    // ========================================================================

    /// Assignment operator.
    /**
     * @param[in] value  value to assign
     * @return  reference to @c this
     */
    _Myt & operator = ( lookup_index_type value )
    {
        setValue( value );
        return *this;
    }

    /// Assignment operator.
    /**
     * @param[in] other  value to assign
     * @return  reference to @c this
     */
    _Myt & operator = ( const _Myt &other )
    {
        setValue( other.value() );
        return *this;
    }

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve value.
    /**
     * @return  value
     */
    inline lookup_index_type value() const
    {
        return ( reinterpret_cast<const lookup_index_type*>( this )[0] );
    }

    /// Set value.
    /**
     * @param[in] value  value to assign
     */
    inline void setValue( lookup_index_type value )
    {
        reinterpret_cast<lookup_index_type*>( this )[0] = value;
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////

// 32 edges namespace
namespace e32
{
    /// 1x1 rotation lookup value.
    struct lookup_1x1_val
    {
        lookup_index_type left              : 5;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 5;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 1x1 rotation lookup.
    typedef lookup<lookup_1x1_val, 5, 5> lookup_1x1;

    /// 1x2 rotation lookup value.
    struct lookup_1x2_val
    {
        lookup_index_type left              : 10;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 5;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 1x2 rotation lookup.
    typedef lookup<lookup_1x2_val, 10, 5> lookup_1x2;

    /// 2x1 rotation lookup value.
    struct lookup_2x1_val
    {
        lookup_index_type left              : 5;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 10;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x1 rotation lookup.
    typedef lookup<lookup_2x1_val, 5, 10> lookup_2x1;

    /// 2x2 rotation lookup value.
    struct lookup_2x2_val
    {
        lookup_index_type left              : 10;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 10;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x2 rotation lookup.
    typedef lookup<lookup_2x2_val, 10, 10> lookup_2x2;

    /// 2x4 rotation lookup value.
    struct lookup_2x4_val
    {
        lookup_index_type left              : 20;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 10;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x4 rotation lookup.
    typedef lookup<lookup_2x4_val, 20, 10> lookup_2x4;

    /// 4x2 rotation lookup value.
    struct lookup_4x2_val
    {
        lookup_index_type left              : 10;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 20;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x2 rotation lookup.
    typedef lookup<lookup_4x2_val, 10, 20> lookup_4x2;

#if (64 <= _ENV_BITS)

    /// 4x4 rotation lookup value.
    struct lookup_4x4_val
    {
        lookup_index_type left              : 20;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 20;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x4 rotation lookup.
    typedef lookup<lookup_4x4_val, 20, 20> lookup_4x4;

    /// 4x8 rotation lookup value.
    struct lookup_4x8_val
    {
        lookup_index_type left              : 40;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 20;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x8 rotation lookup.
    typedef lookup<lookup_4x8_val, 40, 20> lookup_4x8;

    /// 8x4 rotation lookup value.
    struct lookup_8x4_val
    {
        lookup_index_type left              : 20;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 40;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 8x4 rotation lookup.
    typedef lookup<lookup_8x4_val, 20, 40> lookup_8x4;

#endif

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

// 16 edges namespace
namespace e16
{
    /// 1x1 rotation lookup value.
    struct lookup_1x1_val
    {
        lookup_index_type left              : 4;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 4;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 1x1 rotation lookup.
    typedef lookup<lookup_1x1_val, 4, 4> lookup_1x1;

    /// 1x2 rotation lookup value.
    struct lookup_1x2_val
    {
        lookup_index_type left              : 8;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 4;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 1x2 rotation lookup.
    typedef lookup<lookup_1x2_val, 8, 4> lookup_1x2;

    /// 2x1 rotation lookup value.
    struct lookup_2x1_val
    {
        lookup_index_type left              : 4;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 8;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x1 rotation lookup.
    typedef lookup<lookup_2x1_val, 4, 8> lookup_2x1;

    /// 2x2 rotation lookup value.
    struct lookup_2x2_val
    {
        lookup_index_type left              : 8;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 8;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x2 rotation lookup.
    typedef lookup<lookup_2x2_val, 8, 8> lookup_2x2;

    /// 2x4 rotation lookup value.
    struct lookup_2x4_val
    {
        lookup_index_type left              : 16;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 8;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x4 rotation lookup.
    typedef lookup<lookup_2x4_val, 16, 8> lookup_2x4;

    /// 4x2 rotation lookup value.
    struct lookup_4x2_val
    {
        lookup_index_type left              : 8;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 16;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x2 rotation lookup.
    typedef lookup<lookup_4x2_val, 8, 16> lookup_4x2;

#if (64 <= _ENV_BITS)

    /// 4x4 rotation lookup value.
    struct lookup_4x4_val
    {
        lookup_index_type left              : 16;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 16;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x4 rotation lookup.
    typedef lookup<lookup_4x4_val, 16, 16> lookup_4x4;

    /// 4x8 rotation lookup value.
    struct lookup_4x8_val
    {
        lookup_index_type left              : 32;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 16;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x8 rotation lookup.
    typedef lookup<lookup_4x8_val, 32, 16> lookup_4x8;

    /// 8x4 rotation lookup value.
    struct lookup_8x4_val
    {
        lookup_index_type left              : 16;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 32;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 8x4 rotation lookup.
    typedef lookup<lookup_8x4_val, 16, 32> lookup_8x4;

#endif

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

// 8 edges namespace
namespace e8
{
    /// 1x1 rotation lookup value.
    struct lookup_1x1_val
    {
        lookup_index_type left              : 3;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 3;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 1x1 rotation lookup.
    typedef lookup<lookup_1x1_val, 3, 3> lookup_1x1;

    /// 1x2 rotation lookup value.
    struct lookup_1x2_val
    {
        lookup_index_type left              : 6;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 3;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 1x2 rotation lookup.
    typedef lookup<lookup_1x2_val, 6, 3> lookup_1x2;

    /// 2x1 rotation lookup value.
    struct lookup_2x1_val
    {
        lookup_index_type left              : 3;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 6;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x1 rotation lookup.
    typedef lookup<lookup_2x1_val, 3, 6> lookup_2x1;

    /// 2x2 rotation lookup value.
    struct lookup_2x2_val
    {
        lookup_index_type left              : 6;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 6;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x2 rotation lookup.
    typedef lookup<lookup_2x2_val, 6, 6> lookup_2x2;

    /// 2x4 rotation lookup value.
    struct lookup_2x4_val
    {
        lookup_index_type left              : 12;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 6;            ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 2x4 rotation lookup.
    typedef lookup<lookup_2x4_val, 12, 6> lookup_2x4;

    /// 4x2 rotation lookup value.
    struct lookup_4x2_val
    {
        lookup_index_type left              : 6;            ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 12;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x2 rotation lookup.
    typedef lookup<lookup_4x2_val, 6, 12> lookup_4x2;

    /// 4x4 rotation lookup value.
    struct lookup_4x4_val
    {
        lookup_index_type left              : 12;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 12;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x4 rotation lookup.
    typedef lookup<lookup_4x4_val, 12, 12> lookup_4x4;

#if (64 <= _ENV_BITS)

    /// 4x8 rotation lookup value.
    struct lookup_4x8_val
    {
        lookup_index_type left              : 24;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 12;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 4x8 rotation lookup.
    typedef lookup<lookup_4x8_val, 24, 12> lookup_4x8;

    /// 8x4 rotation lookup value.
    struct lookup_8x4_val
    {
        lookup_index_type left              : 12;           ///< Left edge value (0 for border, all bits for any edge).
        lookup_index_type top               : 24;           ///< Top edge value (0 for border, all bits for any edge).
        lookup_index_type right             : 1;            ///< Right edge value (0 for border, 1 for any edge).
        lookup_index_type bottom            : 1;            ///< Bottom edge value (0 for border, 1 for any edge).
    };

    /// 8x4 rotation lookup.
    typedef lookup<lookup_8x4_val, 12, 24> lookup_8x4;

#endif

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // LOOKUP_H

