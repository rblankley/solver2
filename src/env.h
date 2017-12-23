/**
 * @file env.h
 * @brief Environment setup.
 *
 * @section Copyright
 * Copyright (C) 2015 Randy Blankley
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

#ifndef ENV_H
#define ENV_H

#include <cstdint>

///////////////////////////////////////////////////////////////////////////////////////////////////

// windows
#if _WIN32 || _WIN64

#if _WIN64
#define _ENV_BITS 64
#else
#define _ENV_BITS 32
#endif

// gcc
#elif __GNUC__

#if __x86_64__ || __ppc64__
#define _ENV_BITS 64
#else
#define _ENV_BITS 32
#endif

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

#if (32 == _ENV_BITS )
typedef uint32_t env_word_type;                                             ///< Word type for 32 bit environment.
#elif (64 == _ENV_BITS )
typedef uint64_t env_word_type;                                             ///< Word type for 64 bit environment.
#else
#error Invalid machine setup! See env.h for proper configuration.
#endif

/// Number of bit in an @c env_word_type.
static const env_word_type ENV_BITS = _ENV_BITS;

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // ENV_H

