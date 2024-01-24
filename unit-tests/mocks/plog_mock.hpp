/******************************************************************************************************
 * Plog Copyright (C) 2024                                                                            *
 *                                                                                                    *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the   *
 * authors be held liable for any damages arising from the use of this software.                      *
 *                                                                                                    *
 * Permission is granted to anyone to use this software for any purpose, including commercial         *
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:   *
 *                                                                                                    *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the   *
 *    original software. If you use this software in a product, an acknowledgment in the product      *
 *    documentation would be appreciated but is not required.                                         *
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being *
 *    the original software.                                                                          *
 * 3. This notice may not be removed or altered from any source distribution.                         *
 *****************************************************************************************************/

#ifndef PLOG_MOCK_HPP_
#define PLOG_MOCK_HPP_

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gmock/gmock.h>

#include "plog.h"

/******************************************************************************************************
 * MACROS                                                                                             *
 *****************************************************************************************************/

#undef plog_fatal
#define plog_fatal(format, ...)

#undef plog_error
#define plog_error(format, ...)

#undef plog_warn
#define plog_warn(format, ...)

#undef plog_info
#define plog_info(format, ...)

#undef plog_debug
#define plog_debug(format, ...)

#undef plog_trace
#define plog_trace(format, ...)

#undef plog_verbose
#define plog_verbose(format, ...)

/******************************************************************************************************
 * TYPE DEFINITIONS                                                                                   *
 *****************************************************************************************************/

class Plog
{
public:
	virtual ~Plog(void) = default;

	virtual gboolean plog_init(const gchar* file_name) = 0;
	virtual void plog_deinit(void) = 0;
	virtual void plog_set_severity_level(guint8 severity_level_mask) = 0;
	virtual guint8 plog_get_severity_level(void) = 0;
	virtual void plog_set_file_size(gsize file_size) = 0;
	virtual gsize plog_get_file_size(void) = 0;
	virtual void plog_set_file_count(guint8 file_count) = 0;
	virtual guint8 plog_get_file_count(void) = 0;
	virtual void plog_set_terminal_mode(gboolean terminal_mode) = 0;
	virtual gboolean plog_get_terminal_mode(void) = 0;
	virtual gboolean plog_set_buffer_size(gsize buffer_size) = 0;
	virtual gsize plog_get_buffer_size(void) = 0;
};

class PlogMock : public Plog
{
public:
	PlogMock(void)
	{
		plogMock = this;
	}

	virtual ~PlogMock(void)
	{
		plogMock = nullptr;
	}

	MOCK_METHOD1(plog_init, gboolean(const gchar*));
	MOCK_METHOD0(plog_deinit, void(void));
	MOCK_METHOD1(plog_set_severity_level, void(guint8));
	MOCK_METHOD0(plog_get_severity_level, guint8(void));
	MOCK_METHOD1(plog_set_file_size, void(gsize));
	MOCK_METHOD0(plog_get_file_size, gsize(void));
	MOCK_METHOD1(plog_set_file_count, void(guint8));
	MOCK_METHOD0(plog_get_file_count, guint8(void));
	MOCK_METHOD1(plog_set_terminal_mode, void(gboolean));
	MOCK_METHOD0(plog_get_terminal_mode, gboolean(void));
	MOCK_METHOD1(plog_set_buffer_size, gboolean(gsize));
	MOCK_METHOD0(plog_get_buffer_size, gsize(void));

public:
	static PlogMock* plogMock;
};

/******************************************************************************************************
 * LOCAL VARIABLES                                                                                    *
 *****************************************************************************************************/

PlogMock* PlogMock::plogMock = nullptr;

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

extern "C" {

gboolean plog_init(const gchar* const file_name)
{
	if (nullptr == PlogMock::plogMock)
	{
		ADD_FAILURE() << "plog_init(): nullptr == PlogMock::plogMock";
		return FALSE;
	}
	return PlogMock::plogMock->plog_init(file_name);
}

void plog_deinit(void)
{
	ASSERT_NE(nullptr, PlogMock::plogMock) << "plog_deinit(): nullptr == PlogMock::plogMock";
	PlogMock::plogMock->plog_deinit();
}

void plog_set_severity_level(const guint8 severity_level_mask)
{
	ASSERT_NE(nullptr, PlogMock::plogMock) << "plog_set_severity_level(): nullptr == PlogMock::plogMock";
	PlogMock::plogMock->plog_set_severity_level(severity_level_mask);
}

guint8 plog_get_severity_level(void)
{
	if (nullptr == PlogMock::plogMock)
	{
		ADD_FAILURE() << "plog_get_severity_level(): nullptr == PlogMock::plogMock";
		return 0U;
	}
	return PlogMock::plogMock->plog_get_severity_level();
}

void plog_set_file_size(const gsize file_size)
{
	ASSERT_NE(nullptr, PlogMock::plogMock) << "plog_set_file_size(): nullptr == PlogMock::plogMock";
	PlogMock::plogMock->plog_set_file_size(file_size);
}

gsize plog_get_file_size(void)
{
	if (nullptr == PlogMock::plogMock)
	{
		ADD_FAILURE() << "plog_get_file_size(): nullptr == PlogMock::plogMock";
		return 0UL;
	}
	return PlogMock::plogMock->plog_get_file_size();
}

void plog_set_file_count(const guint8 file_count)
{
	ASSERT_NE(nullptr, PlogMock::plogMock) << "plog_set_file_count(): nullptr == PlogMock::plogMock";
	PlogMock::plogMock->plog_set_file_count(file_count);
}

guint8 plog_get_file_count(void)
{
	if (nullptr == PlogMock::plogMock)
	{
		ADD_FAILURE() << "plog_get_file_count(): nullptr == PlogMock::plogMock";
		return 0U;
	}
	return PlogMock::plogMock->plog_get_file_count();
}

void plog_set_terminal_mode(const gboolean terminal_mode)
{
	ASSERT_NE(nullptr, PlogMock::plogMock) << "plog_set_terminal_mode(): nullptr == PlogMock::plogMock";
	PlogMock::plogMock->plog_set_terminal_mode(terminal_mode);
}

gboolean plog_get_terminal_mode(void)
{
	if (nullptr == PlogMock::plogMock)
	{
		ADD_FAILURE() << "plog_get_terminal_mode(): nullptr == PlogMock::plogMock";
		return FALSE;
	}
	return PlogMock::plogMock->plog_get_terminal_mode();
}

gboolean plog_set_buffer_size(const gsize buffer_size)
{
	if (nullptr == PlogMock::plogMock)
	{
		ADD_FAILURE() << "plog_set_buffer_size(): nullptr == PlogMock::plogMock";
		return FALSE;
	}
	return PlogMock::plogMock->plog_set_buffer_size(buffer_size);
}

gsize plog_get_buffer_size(void)
{
	if (nullptr == PlogMock::plogMock)
	{
		ADD_FAILURE() << "plog_get_buffer_size(): nullptr == PlogMock::plogMock";
		return 0UL;
	}
	return PlogMock::plogMock->plog_get_buffer_size();
}

void plog_internal(guint8 severity_bit, const gchar* severity_tag, const gchar* function_name, const gchar* format, ...)
{
}

}

#endif /*< PLOG_MOCK_HPP_ */
