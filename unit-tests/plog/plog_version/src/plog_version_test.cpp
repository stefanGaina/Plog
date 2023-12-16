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

/******************************************************************************************************
 * @file plog_version_test.cpp                                                                        *
 * @date:      @author:                   Reason for change:                                          *
 * 15.12.2023  Gaina Stefan               Initial version.                                            *
 * @details This file unit-tests plog_version.c.                                                      *
 * @todo N/A.                                                                                         *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gtest/gtest.h>

#include "plog_version.h"

/******************************************************************************************************
 * vector_init                                                                                        *
 *****************************************************************************************************/

TEST(PlogVersionTest, plog_get_version_success)
{
	const plog_Version_t version = plog_get_version();

	ASSERT_EQ(PLOG_VERSION_MAJOR, version.major) << "Incorrect major version!";
	ASSERT_EQ(PLOG_VERSION_MINOR, version.minor) << "Incorrect minor version!";
	ASSERT_EQ(PLOG_VERSION_PATCH, version.patch) << "Incorrect patch version!";
}
