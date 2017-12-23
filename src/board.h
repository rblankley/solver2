/**
 * @file board.h
 * @brief Board class.
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

#ifndef BOARD_H
#define BOARD_H

#if _WIN32 || _WIN64
#pragma warning( push )
#pragma warning( disable : 4800 )
#endif

#include "placement.h"

#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

/// Test board functionality.
bool test_board();

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Abstract board class.
class abstractBoard
{
    typedef abstractBoard _Myt;                             ///< My type.

public:

    static const env_word_type _NO_BORDERS = 0x00;          ///< Bit mask for no borders.
    static const env_word_type _LEFT_BORDER = 0x01;         ///< Bit mask for left border.
    static const env_word_type _TOP_BORDER = 0x02;          ///< Bit mask for top border.
    static const env_word_type _RIGHT_BORDER = 0x04;        ///< Bit mask for right border.
    static const env_word_type _BOTTOM_BORDER = 0x08;       ///< Bit mask for bottom border,

    /// Board type.
    enum type
    {
        typeNormal                  = _LEFT_BORDER | _TOP_BORDER | _RIGHT_BORDER | _BOTTOM_BORDER,      ///< Normal board with all edges a border.
        typeTopLeftCorner           = _TOP_BORDER | _LEFT_BORDER,                                       ///< Top left corner is border edges.
        typeTopEdge                 = _TOP_BORDER,                                                      ///< Only top is border edges.
        typeTopRightCorner          = _TOP_BORDER | _RIGHT_BORDER,                                      ///< Top right corner is border edges.
        typeLeftEdge                = _LEFT_BORDER,                                                     ///< Only left is border edges.
        typeMiddle                  = _NO_BORDERS,                                                      ///< No border edges.
        typeRightEdge               = _RIGHT_BORDER,                                                    ///< Only right is border edges.
        typeBottomLeftCorner        = _BOTTOM_BORDER | _LEFT_BORDER,                                    ///< Bottom left corner is border edges.
        typeBottomEdge              = _BOTTOM_BORDER,                                                   ///< Only bottom is border edges.
        typeBottomRightCorner       = _BOTTOM_BORDER | _RIGHT_BORDER,                                   ///< Bottom right corner is border edges.
    };

    // ========================================================================
    // DTOR
    // ========================================================================

    /// Destructor.
    virtual ~abstractBoard() {}

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve board height.
    /**
     * @return  board height
     */
    virtual env_word_type height() const {return height_;}

    /// Retrieve print option.
    /**
     * @return  @c true to print solution, @c false otherwise
     */
    virtual bool print() const {return print_;}

    /// Retrieve quit option.
    /**
     * @return  @c true to quit when solution found, @c false otherwise
     */
    virtual bool quit() const {return quit_;}

    /// Set print option.
    /**
     * @param[in] value  @c true to print solution, @c false otherwise
     */
    virtual void setPrint( bool value ) {print_ = value;}

    /// Set quit option.
    /**
     * @param[in] value  @c true to quit when solution found, @c false otherwise
     */
    virtual void setQuit( bool value ) {quit_ = value;}

    /// Set solver threaded or not.
    /**
     * @param[in] value  @c true if threaded, @c false otherwise
     */
    virtual void setThreaded( bool value ) {threaded_ = value;}

    /// Set type.
    /**
     * @param[in] value  board type
     */
    virtual void setType( type value ) = 0;

    /// Retrieve number of solutions.
    /**
     * @return  solution count
     */
    virtual env_word_type solutions() const {return solutions_;}

    /// Retrieve solver threaded or not.
    /**
     * @return  @c true if threaded, @c false otherwise
     */
    virtual bool threaded() const {return threaded_;}

    /// Retrieve board width.
    /**
     * @return  board width
     */
    virtual env_word_type width() const {return width_;}

    // ========================================================================
    // Methods
    // ========================================================================

    /// Solve board.
    virtual void solve() = 0;

protected:

    env_word_type width_;                                   ///< Board width.
    env_word_type height_;                                  ///< Board height.

    env_word_type solutions_;                               ///< Number of solutions found.

    bool print_;                                            ///< Print solution when @c true.
    bool quit_;                                             ///< Quit solving after solution found when @c true.
    bool threaded_;                                         ///< Solve using threading when @c true.

    static std::mutex m_;                                   ///< Mutex for printing.

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    /**
     * @param[in] w  width
     * @param[in] h  height
     */
    abstractBoard( env_word_type w, env_word_type h ) :
        width_( w ),
        height_( h ),
        solutions_( 0 ),
        print_( false ),
        quit_( false ),
        threaded_( false )
    {
    }

    /// Constructor.
    /**
     * @param[in] other  object to copy
     */
    abstractBoard( const _Myt &other ) :
        width_( other.width_ ),
        height_( other.height_ ),
        solutions_( 0 ),
        print_( other.print_ ),
        quit_( other.quit_ ),
        threaded_( false )
    {
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Board complete.
    /**
     * This method is invoked when a solution is found.
     */
    virtual void complete() {++solutions_;}

private:

    // not implemented
    _Myt & operator == ( const _Myt & );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Board class.
/**
 * @tparam L  tile array lookup type
 */
template <class L>
class board : public abstractBoard
{
    typedef board<L> _Myt;                                  ///< My type.
    typedef abstractBoard _Mybase;                          ///< My base type.

public:

    typedef L tile_array_lookup_type;                                                       ///< Tile array lookup type.
    typedef typename tile_array_lookup_type::tile_type tile_type;                           ///< Tile type.
    typedef typename tile_array_lookup_type::tile_array_ptr_type tile_array_ptr_type;       ///< Tile array pointer type.
    typedef typename tile_array_lookup_type::lookup_type lookup_type;                       ///< Lookup type.

    typedef typename tile_type::mask_type mask_type;        ///< Mask type.

    typedef placement<tile_type> placement_type;            ///< Placement type.

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    /**
     * @param[in] w  width
     * @param[in] h  height
     * @param[in] lookups  tile lookups
     */
    board( env_word_type w, env_word_type h, const tile_array_lookup_type *lookups ) :
        _Mybase( w, h ),
        lookups_( lookups )
    {
        initialize();
        generateTilePattern();
    }

    /// Constructor.
    /**
     * @param[in] other  object to copy
     */
    board( const _Myt &other ) :
        _Mybase( other ),
        lookups_( other.lookups_ )
    {
        initialize();
        generateTilePattern();

        // copy placement information
        for ( env_word_type x( 0 ); x < width_; ++x )
            for ( env_word_type y( 0 ); y < height_; ++y )
            {
                const placement_type *p0( other.at( x, y ) );

                placement_type *p( at( x, y ) );
                p->lookup = p0->lookup;
                p->tile = p0->tile;
            }
    }

    /// Destructor.
    virtual ~board()
    {
    }

    // ========================================================================
    // Properties
    // ========================================================================

    /// Set type.
    /**
     * @param[in] value  board type
     */
    virtual void setType( type value )
    {
        const bool left( _LEFT_BORDER & value );
        const bool top( _TOP_BORDER & value );
        const bool right( _RIGHT_BORDER & value );
        const bool bottom( _BOTTOM_BORDER & value );

        for ( env_word_type x( 0 ); x < width_; ++x )
            for ( env_word_type y( 0 ); y < height_; ++y )
            {
                lookup_type *pl( reinterpret_cast<lookup_type*>( at( x, y ) ) );

                pl->val.left = ((( 0 == x ) && ( left )) ? 0 : lookup_type::LEFT_EDGE );
                pl->val.top = ((( 0 == y ) && ( top )) ? 0 : lookup_type::TOP_EDGE );
                pl->val.right = ((( x == (width_-1) ) && ( right )) ? 0 : 1 );
                pl->val.bottom = ((( y == (height_-1) ) && ( bottom )) ? 0 : 1 );
            }
    }
    
    // ========================================================================
    // Methods
    // ========================================================================

    /// Solve board.
    virtual void solve()
    {
        placement_type *p( &placements_.at( 0 ) );

        // non-threaded implementation
        if ( !threaded() )
        {
            const mask_type m;

            solve_for( p, m );
        }
        
        // solve using thread on each placement
        else
        {
            const tile_array_ptr_type *tiles( &lookups_->at( p->lookup ) );

            // number of worker threads
            const env_word_type min_workers( 2 * std::thread::hardware_concurrency() );
            const env_word_type max_workers( 4 * std::thread::hardware_concurrency() );

            std::list<_Myt> boards;
            std::list<std::future<void>> work;

            // iterate tiles
            // create a board for each tile and queue up work against it
            for ( env_word_type i( tiles->size() ); i--; )
            {
                const tile_type *t( tiles->at( i ) );

                // generate board
                boards.push_back( *this );

                // schedule work
                _Myt *b( &boards.back() );

                work.push_back( std::async( std::launch::async, &_Myt::solve_for, b, b->at( 0, 0 )->place( t ), t->mask ) );

                // TODO
                // Need a better mechanism for threading... ideally a thread pool of size
                // std::thread::hardware_concurrency() would reside here and handle a queue of
                // pre-generated boards. The problem with below is threads 1-7 could be complete
                // while we are waiting for thread 0 to complete. This is not efficient.

                // cap number of workers
                if ( work.size() < max_workers )
                    continue;

                do
                {
                    const auto &w( work.front() );
                    w.wait();

                    work.pop_front();

                } while ( min_workers < work.size() );

            }

            // wait for completion
            for ( const auto &w : work )
                w.wait();

            // sum solutions
            for ( const auto &b : boards )
                solutions_ += b.solutions();
        }
    }

protected:

    std::vector<placement_type> placements_;                ///< Board placements.
    placement_type junk_;                                   ///< Junk placement, used for edges.

    const tile_array_lookup_type *lookups_;                 ///< Pointer to tile lookups.

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve placement at position.
    /**
     * @param[in] x  x coord
     * @param[in] y  y coord
     * @return  pointer to placement
     */
    placement_type *at( env_word_type x, env_word_type y )
    {
        return &placements_.at( (x * height_) + y );
    }

    /// Retrieve placement at position.
    /**
     * @param[in] x  x coord
     * @param[in] y  y coord
     * @return  pointer to placement
     */
    const placement_type *at( env_word_type x, env_word_type y ) const
    {
        return &placements_.at( (x * height_) + y );
    }

    // ========================================================================
    // Methods
    // ========================================================================

    /// Solve board.
    /**
     * @param[in] p  placement to solve
     * @param[in] mask  mask to use
     */
    void solve_for( placement_type *p, const mask_type &mask )
    {
        const tile_array_ptr_type *tiles( &lookups_->at( p->lookup ) );

        // iterate tiles
        for ( env_word_type i( tiles->size() ); i--; )
        {
            const tile_type *t( tiles->at( i ) );

            // check if this tile can be placed (i.e. all pieces unused)
            if ( mask & t->mask )
                continue;

            // place it!
            placement_type *next;

            if ( (next = p->place( t )) )
            {
                // update our mask and solve next placement
                const mask_type m( mask, t->mask );

                solve_for( next, m );
            }

            // otherwise we are complete!
            else if ( print_ )
            {
                // lock object
                std::lock_guard<decltype(m_)> guard( m_ );

                // print solution
                for ( const placement_type &p : placements_ )
                    p.tile->print();

                // quit!
                if ( quit_ )
                    exit( EXIT_SUCCESS );

                ++solutions_;

                std::cout << std::endl;
            }
            else
            {
                complete();
            }
        }
    }

private:

    /// Initialize placements.
    void initialize()
    {
        placements_.resize( width_ * height_ );

        for ( env_word_type x( 0 ); x < width_; ++x )
            for ( env_word_type y( 0 ); y < height_; ++y )
            {
                placement_type *p( at( x, y ) );

                p->left = (( 0 < x ) ? at( x-1, y ) : &junk_ );
                p->top = (( 0 < y ) ? at( x, y-1 ) : &junk_ );
                p->right = (( x < (width_-1) ) ? at( x+1, y ) : &junk_ );
                p->bottom = (( y < (height_-1) ) ? at( x, y+1 ) : &junk_ );
            }

        setType( typeNormal );
    }

    /// Generate tile pattern.
    void generateTilePattern()
    {
        for ( env_word_type i( width_ * height_ ); i--; )
        {
            placement_type *p( &placements_.at( i ) );

            p->prev = (( 0 < i ) ? &placements_.at( i-1 ) : nullptr );
            p->next = (( i < (width_ * height_) - 1 ) ? &placements_.at( i+1 ) : nullptr );
        }
    }

    // not implemented
    _Myt & operator == ( const _Myt & );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32 || _WIN64
#pragma warning( pop )
#endif

#endif // BOARD_H

