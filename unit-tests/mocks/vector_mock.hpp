/******************************************************************************************************
 * Plog Copyright (C) 2023                                                                            *
 *                                                                                                    *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the   *
 * authors be held liable for any damages arising from the use of this software.                      *
 *                                                                                                    *
 * Permission is granted to anyone to use this software for any purpose, including commercial         *
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:   *
 *                                                                                                    *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the   *
 * original software. If you use this software in a product, an acknowledgment in the product         *
 * documentation would be appreciated but is not required.                                            *
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being *
 * the original software.                                                                             *
 * 3. This notice may not be removed or altered from any source distribution.                         *
******************************************************************************************************/

#ifndef VECTOR_MOCK_HPP_
#define VECTOR_MOCK_HPP_

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gmock/gmock.h>

extern "C" {
#include "internal/vector.h"
}

/******************************************************************************************************
 * TYPE DEFINITIONS                                                                                   *
 *****************************************************************************************************/

class Vector
{
public:
	virtual ~Vector(void) = default;

	virtual void vector_init(Vector_t* vector) = 0;
	virtual void vector_clean(Vector_t* vector) = 0;
	virtual gboolean vector_push(Vector_t* vector, const gchar* buffer) = 0;
	virtual void vector_pop(Vector_t* vector, gchar* buffer, gsize buffer_size) = 0;
	virtual gboolean vector_is_empty(const Vector_t* vector) = 0;
};

class VectorMock : public Vector
{
public:
	VectorMock(void)
	{
		s_vectorMock = this;
	}

	virtual ~VectorMock(void)
	{
		s_vectorMock = nullptr;
	}

	MOCK_METHOD1(vector_init, void(Vector_t*));
	MOCK_METHOD1(vector_clean, void(Vector_t*));
	MOCK_METHOD2(vector_push, gboolean(Vector_t*, const gchar*));
	MOCK_METHOD3(vector_pop, void(Vector_t*, gchar*, gsize));
	MOCK_METHOD1(vector_is_empty, gboolean(const Vector_t*));

public:
	static VectorMock* s_vectorMock;
};

/******************************************************************************************************
 * LOCAL VARIABLES                                                                                    *
 *****************************************************************************************************/

VectorMock* VectorMock::s_vectorMock = nullptr;

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

extern "C" {

void vector_init(Vector_t* const vector)
{
	ASSERT_NE(nullptr, VectorMock::s_vectorMock) << "vector_init(): nullptr == VectorMock::s_vectorMock";
	VectorMock::s_vectorMock->vector_init(vector);
}

void vector_clean(Vector_t* const vector)
{
	ASSERT_NE(nullptr, VectorMock::s_vectorMock) << "vector_clean(): nullptr == VectorMock::s_vectorMock";
	VectorMock::s_vectorMock->vector_clean(vector);
}

gboolean vector_push(Vector_t* const vector, const gchar* const buffer)
{
	if (nullptr == VectorMock::s_vectorMock)
	{
		ADD_FAILURE() << "vector_push(): nullptr == VectorMock::s_vectorMock";
		return FALSE;
	}
	return VectorMock::s_vectorMock->vector_push(vector, buffer);
}

void vector_pop(Vector_t* const vector, gchar* const buffer, const gsize buffer_size)
{
	ASSERT_NE(nullptr, VectorMock::s_vectorMock) << "vector_pop(): nullptr == VectorMock::s_vectorMock";
	VectorMock::s_vectorMock->vector_pop(vector, buffer, buffer_size);
}

gboolean vector_is_empty(const Vector_t* const vector)
{
	if (nullptr == VectorMock::s_vectorMock)
	{
		ADD_FAILURE() << "vector_is_empty(): nullptr == VectorMock::s_vectorMock";
		return FALSE;
	}
	return VectorMock::s_vectorMock->vector_is_empty(vector);
}

}

#endif /*< VECTOR_MOCK_HPP_ */
