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
 * @file vector.c
 * @author Gaina Stefan
 * @date 08.12.2023
 * @brief This file implements the interface defined in vector.h.
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include "internal/vector.h"

/******************************************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Explicit data type of the vector for internal usage.
 *****************************************************************************************************/
typedef struct s_PrivateVector_t
{
	gchar** buffer; /**< Dynamic array of strings. */
	gsize	size;	/**< The size of the array.    */
} PrivateVector_t;

/******************************************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************************************/

void vector_init(Vector_t* const public_vector)
{
	PrivateVector_t* const vector = (PrivateVector_t*)public_vector;

	vector->buffer = NULL;
	vector->size   = 0UL;
}

void vector_clean(Vector_t* const public_vector)
{
	PrivateVector_t* const vector = (PrivateVector_t*)public_vector;
	gsize				   index  = 0UL;

	for (; index < vector->size; ++index)
	{
		g_free(vector->buffer[index]);
		vector->buffer[index] = NULL;
	}

	g_free(vector->buffer);
	vector->buffer = NULL;
}

gboolean vector_push(Vector_t* const public_vector, const gchar* const buffer)
{
	PrivateVector_t* const vector			= (PrivateVector_t*)public_vector;
	const gsize			   buffer_size		= strlen(buffer) + 1UL;
	gchar*				   buffer_copy		= NULL;
	gchar**				   buffer_auxiliary = NULL;

	buffer_copy = (gchar*)g_try_malloc(buffer_size);
	if (NULL == buffer_copy)
	{
		return FALSE;
	}

	buffer_auxiliary = (gchar**)g_try_realloc(vector->buffer, (vector->size + 1UL) * sizeof(gchar*));
	if (NULL == buffer_auxiliary)
	{
		g_free(buffer_copy);
		return FALSE;
	}

	(void)g_strlcpy(buffer_copy, buffer, buffer_size);
	vector->buffer				   = buffer_auxiliary;
	vector->buffer[vector->size++] = buffer_copy;

	return TRUE;
}

void vector_pop(Vector_t* const public_vector, gchar* const buffer, const gsize buffer_size)
{
	PrivateVector_t* const vector = (PrivateVector_t*)public_vector;
	gsize				   index  = 0UL;

	(void)g_strlcpy(buffer, vector->buffer[0], buffer_size);
	g_free(vector->buffer[0]);

	for (; index < vector->size - 1UL; ++index)
	{
		vector->buffer[index] = vector->buffer[index + 1UL];
	}

	vector->buffer = (gchar**)g_try_realloc(vector->buffer, --vector->size * sizeof(gchar*));
}

gboolean vector_is_empty(const Vector_t* const public_vector)
{
	const PrivateVector_t* const vector = (const PrivateVector_t*)public_vector;
	return 0UL == vector->size;
}
