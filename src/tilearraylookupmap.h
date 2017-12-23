/**
 * @file tilearraylookupmap.h
 * @brief Map of tile arrays indexed by lookup.
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

#ifndef TILEARRAYLOOKUPMAP_H
#define TILEARRAYLOOKUPMAP_H

#if _WIN32 || _WIN64
#pragma warning( push )
#pragma warning( disable : 4800 )
#endif

#include "tile.h"
#include "tilearraylookup.h"

#include <map>

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Map of tile arrays indexed by lookup.
/**
 * @tparam T  tile type
 */
template <class T>
class tileArrayLookupMap : public tileArrayLookup<T>, private std::map<lookup_index_type, std::vector<T*>>
{
    typedef tileArrayLookupMap<T> _Myt;                                     ///< My type.
    typedef tileArrayLookup<T> _Mybase;                                     ///< My base type.

    typedef std::map<lookup_index_type, std::vector<T*>> _Myimpl;           ///< My implementation type.

    typedef typename _Myimpl::const_iterator const_iterator;                ///< Constant iterator type.
    typedef typename _Myimpl::iterator iterator;                            ///< Iterator type.

    typedef typename _Myimpl::value_type value_type;                        ///< Value type.

public:

    typedef typename _Mybase::tile_type tile_type;                          ///< Tile type.

    typedef typename _Mybase::tile_array_type tile_array_type;              ///< Tile array type.
    typedef typename _Mybase::tile_array_ptr_type tile_array_ptr_type;      ///< Tile array pointer type.

    typedef typename _Mybase::lookup_type lookup_type;                      ///< Lookup type.

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    tileArrayLookupMap() :
        _Mybase(),
        _Myimpl()
    {

    }

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve array at index.
    /**
     * @param[in] value  index to retrieve
     * @return  pointer to tile array
     */
    inline const tile_array_ptr_type &at( lookup_index_type value ) const
    {
        const_iterator i;

        if ( _Myimpl::end() == (i = _Myimpl::find( value )) )
            return empty_;

        return i->second;
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
        _Mybase::insert( tile );
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
        _Mybase::insert( first, last );
    }

protected:

    // ========================================================================
    // Methods
    // ========================================================================

    /// Insert or assign tile to lookup value.
    /**
     * @param[in] lookup  lookup value
     * @param[in] tile  pointer to tile
     */
    virtual void insert_or_assign( lookup_index_type lookup, tile_type *tile )
    {
        iterator i;

        // find (or create) lookup array
        if ( _Myimpl::end() != (i = _Myimpl::find( lookup )) )
            i->second.push_back( tile );
        else
        {
            tile_array_ptr_type a;
            a.push_back( tile );

            _Myimpl::insert( value_type( lookup, a ) );
        }
    }

    /// Randomize tile arrays.
    virtual void randomize_arrays()
    {
        // process each array
        for ( auto &i : (*this) )
        {
            // funny how our randomization is a sort...
            std::sort( i.second.begin(), i.second.end(), sortTilesByRandom() );
        }
    }

private:

    tile_array_ptr_type empty_;                             ///< An empty array (for when lookup not found).

    // not implemented
    tileArrayLookupMap( const _Myt & );

    // not implemented
    _Myt & operator == ( const _Myt & );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32 || _WIN64
#pragma warning( pop )
#endif

#endif // TILEARRAYLOOKUPMAP_H

