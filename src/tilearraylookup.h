/**
 * @file tilearraylookup.h
 * @brief Tile array lookup base class.
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

#ifndef TILEARRAYLOOKUP_H
#define TILEARRAYLOOKUP_H

#include "lookup.h"

#include <algorithm>
#include <array>
#include <list>
#include <mutex>
#include <random>

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Tile array lookup base class.
/**
 * @tparam T  tile type
 */
template <class T>
class tileArrayLookup
{
    typedef tileArrayLookup _Myt;                           ///< My type.

public:
    typedef T tile_type;                                    ///< Tile type.

    typedef std::vector<tile_type> tile_array_type;         ///< Tile array type.
    typedef std::vector<tile_type*> tile_array_ptr_type;    ///< Tile array pointer type.

    typedef typename T::lookup_type lookup_type;            ///< Lookup type.

    // ========================================================================
    // DTOR
    // ========================================================================

    /// Destructor.
    virtual ~tileArrayLookup()
    {
    }

    // ========================================================================
    // Properties
    // ========================================================================

    /// Calculate number of tiles.
    /**
     * @return  number of tiles
     */
    virtual env_word_type tiles() const
    {
        return tiles_.size();
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Insert tile.
    /**
     * @param[in] tile  tile to insert
     */
    virtual void insert( const tile_type &tile )
    {
        static const lookup_index_type MASK_UL_CORNER( lookup_type::MASK_LEFT_EDGE | lookup_type::MASK_TOP_EDGE );

        bool left;
        bool top;

        // lock object
        std::lock_guard<decltype(m_)> guard( m_ );

        // add tile to list
        tiles_.push_back( tile );

        // compute lookup value
        lookup_type index( reinterpret_cast<const lookup_index_type*>( &tile )[1] );
        index.val.right = (tile.val.right ? 1 : 0);
        index.val.bottom = (tile.val.bottom ? 1 : 0);

        // find (or create) lookup array
        insert_or_assign( index.value(), &tiles_.back() );

        // check left edge
        if ( (left = (lookup_type::MASK_LEFT_EDGE & index.value())) )
        {
            // find (or create) lookup array
            insert_or_assign( lookup_type::MASK_LEFT_EDGE | index.value(), &tiles_.back() );
        }

        // check top edge
        if ( (top = (lookup_type::MASK_TOP_EDGE & index.value())) )
        {
            // find (or create) lookup array
            insert_or_assign( lookup_type::MASK_TOP_EDGE | index.value(), &tiles_.back() );
        }

        // check corner
        if (( left ) && ( top ))
        {
            // find (or create) lookup array
            insert_or_assign( MASK_UL_CORNER | index.value(), &tiles_.back() );
        }
    }

    /// Insert tiles.
    /**
     * Insert tiles in range [first, last) into lookup.
     *
     * @param[in] first  tile iterator of begin element
     * @param[in] last  tile iterator of end element
     */
    virtual void insert( const typename tile_array_type::const_iterator &first, const typename tile_array_type::const_iterator &last )
    {
        // insert each tile
        for ( auto i( first ); i != last; ++i )
            insert( *i );
    }

    /// Randomize tiles.
    virtual void randomize()
    {
        rng_type rng( generateSeededRandNumGen() );

        // set random number
        std::uniform_int_distribution<env_word_type> dist;

        for ( auto &t : tiles_ )
            t.random = dist( rng );

        // randomize each array
        randomize_arrays();
    }

protected:

    std::list<T> tiles_;                                    ///< List of tiles.

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    tileArrayLookup()
    {
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Insert or assign tile to lookup value.
    /**
     * @param[in] lookup  lookup value
     * @param[in] tile  pointer to tile
     */
    virtual void insert_or_assign( lookup_index_type lookup, tile_type *tile ) = 0;

    /// Randomize tile arrays.
    virtual void randomize_arrays() = 0;

private:

    std::mutex m_;                                          ///< Mutex for write access.

#if (32 == _ENV_BITS)
    typedef std::mt19937 rng_type;                          ///< Random number generator type for 32 bit machines.
#elif (64 == _ENV_BITS)
    typedef std::mt19937_64 rng_type;                       ///< Random number generator type for 64 bit machines.
#endif

    /// Generate a seeded random number generator.
    /**
     * @return  seeded random number generator
     */
    static rng_type generateSeededRandNumGen()
    {
        std::random_device rd;

        // calculate number of bits random device can generate
        size_t rd_bits( 0 );

        for ( std::random_device::result_type max( rd.max() ); max & 0x01; max >>= 1 )
            ++rd_bits;

        // generate seed values
        std::array<rng_type::result_type, rng_type::state_size> seed_values;

        size_t rng_bits( rng_type::word_size );

        for ( ;; )
        {
            std::array<rng_type::result_type, rng_type::state_size> seed_values_tmp;
            std::generate( seed_values_tmp.begin(), seed_values_tmp.end(), std::ref( rd ) );

            for ( size_t i( rng_type::state_size ); i--; )
            {
                seed_values[i] <<= rd_bits;
                seed_values[i] |= seed_values_tmp[i];
            }

            if ( rng_bits <= rd_bits )
                break;

            rng_bits -= rd_bits;
        }

        // create a seeded rng
        std::seed_seq seeds( seed_values.begin(), seed_values.end() );

        rng_type rng( seeds );

        // as much as i'd like to trust std::random_device, we can mitigate against bad seed values
        // if we discard some output according to this paper:
        // http://www.iro.umontreal.ca/~lecuyer/myftp/papers/lfsr04.pdf
        rng.discard( 750000 );

        return rng;
    }

    // not implemented
    tileArrayLookup( const _Myt & );

    // not implemented
    _Myt & operator == ( const _Myt & );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TILEARRAYLOOKUP_H

