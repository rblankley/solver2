/**
 * @file piecevector.cpp
 * @brief Vector of pieces implementation.
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

#include "piecevector.h"

#include <fstream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////////
pieceVector::pieceVector() :
    _Mybase()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
pieceVector::pieceVector( const _Myt &other ) :
    _Mybase( other )
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
pieceVector::size_type pieceVector::edges_count() const
{
    size_type edge( 0 );

    // find highest edge value
    for ( const value_type &p : (*this) )
    {
        if ( edge < p.left )
            edge = p.left;

        if ( edge < p.top )
            edge = p.top;

        if ( edge < p.right )
            edge = p.right;

        if ( edge < p.bottom )
            edge = p.bottom;
    }

    return (edge + 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool pieceVector::load( const std::string &filename )
{
    std::ifstream infile( filename );
    std::string line;

    // make sure file open
    if ( !infile.is_open() )
        return false;

    // clear existing elements
    clear();

    // process all lines
    while ( std::getline( infile, line ) )
        if (( 7 <= line.length() ) && ( '/' != line[0] ))
        {
            value_type p;

            std::istringstream iss( line );

            env_word_type count( 0 );
            env_word_type value;

            // each line is in the format:
            // 1 2 3 4
            //
            // where the first number is the left edge, the second is the top edge, etc.
            while ( iss >> value )
            {
                if ( 0 == count )
                    p.left = value;
                else if ( 1 == count )
                    p.top = value;
                else if ( 2 == count )
                    p.right = value;
                else if ( 3 == count )
                    p.bottom = value;

                ++count;
            }

            // need at least four edges
            if ( 4 <= count )
                push_back( p );
        }

    return ( 0 < size() );
}
