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
 * @file vector_test.c
 * @author Gaina Stefan
 * @date 15.12.2023
 * @brief This file unit-tests vector.c.
 * @details Current coverage report:
 * Line coverage: 100.0% (42/42)
 * Functions:     100.0% (5/5)
 * Branches:      100.0% (8/8)
 * @todo N/A.
 * @bug No known bugs.
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES
 *****************************************************************************************************/

#include <gtest/gtest.h>

#include "glib_mock.hpp"
#include "internal/vector.h"

/******************************************************************************************************
 * MACROS
 *****************************************************************************************************/

/** ***************************************************************************************************
 * @brief Dummy address to pass the != NULL check.
 *****************************************************************************************************/
#define NOT_NULL (void*)1

/******************************************************************************************************
 * TEST CLASS
 *****************************************************************************************************/

class VectorTest : public testing::Test
{
public:
	VectorTest(void)
		: glibMock{}
	{
	}

	~VectorTest(void) = default;

protected:
	void SetUp(void) override
	{
	}

	void TearDown(void) override
	{
	}

public:
	GlibMock glibMock;
};

/******************************************************************************************************
 * vector_init
 *****************************************************************************************************/

TEST_F(VectorTest, vector_init_success)
{
	Vector_t vector = {};

	vector_init(&vector);
	ASSERT_EQ(TRUE, vector_is_empty(&vector)) << "The vector is not empty after initialization!";

	EXPECT_CALL(glibMock, g_free(NULL));
	vector_clean(&vector);
	ASSERT_EQ(TRUE, vector_is_empty(&vector)) << "The vector is not empty after clean!";
}

/******************************************************************************************************
 * vector_push
 *****************************************************************************************************/

TEST_F(VectorTest, vector_push_tryMalloc_fail)
{
	Vector_t vector = {};

	vector_init(&vector);

	EXPECT_CALL(glibMock, g_try_malloc(testing::_)) /**/
		.WillOnce(testing::Return((gpointer)NULL));
	ASSERT_EQ(FALSE, vector_push(&vector, "LINE1")) << "Successfully pushed line into vector even though memory allocation failed!";

	EXPECT_CALL(glibMock, g_free(NULL));
	vector_clean(&vector);
}

TEST_F(VectorTest, vector_push_tryRealloc_fail)
{
	Vector_t vector = {};

	vector_init(&vector);

	EXPECT_CALL(glibMock, g_try_malloc(testing::_)) /**/
		.WillOnce(testing::Return((gpointer)NOT_NULL));
	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_)) /**/
		.WillOnce(testing::Return((gpointer)NULL));
	EXPECT_CALL(glibMock, g_free(testing::_)) /**/
		.Times(2);
	ASSERT_EQ(FALSE, vector_push(&vector, "LINE1")) << "Successfully pushed line into vector even though memory reallocation failed!";

	vector_clean(&vector);
}

TEST_F(VectorTest, vector_push_success)
{
	Vector_t vector		 = {};
	gchar	 buffer1[64] = {};
	gchar	 buffer2[64] = {};

	vector_init(&vector);

	EXPECT_CALL(glibMock, g_try_malloc(testing::_)) /**/
		.WillOnce(testing::Return((gpointer)buffer1));
	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_)) /**/
		.WillOnce(testing::Return((gpointer)buffer2));
	ASSERT_EQ(TRUE, vector_push(&vector, "LINE")) << "Failed to push line into vector!";
	ASSERT_EQ(FALSE, vector_is_empty(&vector)) << "The vector is empty after successfully pushing a line!";

	EXPECT_CALL(glibMock, g_free(testing::_)) /**/
		.Times(2);
	vector_clean(&vector);
}

/******************************************************************************************************
 * vector_pop
 *****************************************************************************************************/

TEST_F(VectorTest, vector_pop_success)
{
	static constexpr const gchar* const INPUT_STRING = "LINE";

	Vector_t vector		 = {};
	gchar	 buffer1[64] = {};
	gchar	 buffer2[64] = {};
	gchar	 buffer3[64] = {};

	vector_init(&vector);

	EXPECT_CALL(glibMock, g_try_malloc(testing::_)) /**/
		.WillOnce(testing::Return((gpointer)buffer1));
	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_)) /**/
		.WillOnce(testing::Return((gpointer)buffer2));
	ASSERT_EQ(TRUE, vector_push(&vector, INPUT_STRING)) << "Failed to push line into vector!";
	ASSERT_EQ(FALSE, vector_is_empty(&vector)) << "The vector is empty after successfully pushing a line!";

	EXPECT_CALL(glibMock, g_try_malloc(testing::_)) /**/
		.WillOnce(testing::Return((gpointer)buffer3));
	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_)) /**/
		.WillOnce(testing::Return((gpointer)buffer2));
	ASSERT_EQ(TRUE, vector_push(&vector, INPUT_STRING)) << "Failed to push line into vector!";
	ASSERT_EQ(FALSE, vector_is_empty(&vector)) << "The vector is empty after successfully pushing a line!";

	EXPECT_CALL(glibMock, g_try_realloc(testing::_, testing::_)) /**/
		.WillOnce(testing::Return((gpointer)buffer2));
	EXPECT_CALL(glibMock, g_free(testing::_)) /**/
		.Times(3);
	vector_pop(&vector, buffer1, sizeof(buffer1));
	ASSERT_EQ(0, strcmp(buffer1, INPUT_STRING)) << "Popped string is not matching!";
	ASSERT_EQ(FALSE, vector_is_empty(&vector)) << "The vector is not empty after popping!";

	vector_clean(&vector);
}
