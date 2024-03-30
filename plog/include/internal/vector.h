/******************************************************************************************************
 * Plog Copyright (C) 2024
 *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the
 * authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the
 *    original software. If you use this software in a product, an acknowledgment in the product
 *    documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being
 *    the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @file vector.h
 * @author Gaina Stefan
 * @date 08.12.2023
 * @brief This file defines vector data structure that is used internally by Plog and not meant to be
 * public API.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

#ifndef VECTOR_H_
#define VECTOR_H_

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <glib.h>

/******************************************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Opaque data structure for storying copies of strings.
 *****************************************************************************************************/
typedef struct s_Vector_t
{
	gchar dummy[16]; /**< The size of the vector is 16 bytes. */
} Vector_t;

/******************************************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** ***************************************************************************************************
 * @brief Initializes the vector. Do not call any other function before this.
 * @param vector: Vector object.
 * @return void
 *****************************************************************************************************/
extern void vector_init(Vector_t* vector);

/** ***************************************************************************************************
 * @brief Cleans the stored strings and buffer. It is safe to call other functions after this.
 * @param vector: Vector object.
 * @return void
 *****************************************************************************************************/
extern void vector_clean(Vector_t* vector);

/** ***************************************************************************************************
 * @brief Pushes a string (makes a copy) into the vector.
 * @param vector: Vector object.
 * @param buffer: The string that will be copied.
 * @return TRUE - the string has been successfully pushed.
 * @return FALSE - an error has occurred.
 *****************************************************************************************************/
extern gboolean vector_push(Vector_t* vector, const gchar* buffer);

/** ***************************************************************************************************
 * @brief Pops a string from the vector (removes it). It is not safe to call if the vector is empty.
 * @param vector: Vector object.
 * @param[out] buffer: The buffer where the stored string will be copied.
 * @param buffer_size: The size of the buffer (to avoid buffer overflow).
 * @return void
 *****************************************************************************************************/
extern void vector_pop(Vector_t* vector, gchar* buffer, gsize buffer_size);

/** ***************************************************************************************************
 * @brief Queries if the vector currently stores any string.
 * @param vector: Vector object.
 * @return TRUE - the vector does not store any string.
 * @return FALSE - otherwise.
 *****************************************************************************************************/
extern gboolean vector_is_empty(const Vector_t* vector);

#ifdef __cplusplus
}
#endif

#endif /*< VECTOR_H_ */
