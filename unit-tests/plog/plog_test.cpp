/******************************************************************************************************
 * @file plog_test.cpp                                                                                *
 * @date:      @author:                   Reason for change:                                          *
 * 22.06.2023  Gaina Stefan               Initial version.                                            *
 * @details This file unit-tests plog.c.                                                              *
 * @todo N/A.                                                                                         *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gtest/gtest.h>

#include "plog.h"

/******************************************************************************************************
 * CLASS DEFINITION                                                                                   *
 *****************************************************************************************************/

using namespace ::testing;

class PlogTest : public Test
{
protected:
	void SetUp(void)    override;
	void TearDown(void) override;
};

void PlogTest::SetUp(void)
{
}

void PlogTest::TearDown(void)
{
}

/******************************************************************************************************
 * plog_init                                                                                          *
 *****************************************************************************************************/

TEST_F(PlogTest, plog_init)
{
	plog_init(NULL);
}
