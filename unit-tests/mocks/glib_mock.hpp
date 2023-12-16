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

#ifndef GLIB_MOCK_HPP_
#define GLIB_MOCK_HPP_

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gmock/gmock.h>
#include <glib.h>

/******************************************************************************************************
 * TYPE DEFINITIONS                                                                                   *
 *****************************************************************************************************/

class Glib
{
public:
	virtual ~Glib(void) = default;

	virtual gpointer g_try_realloc(gpointer mem, gsize n_bytes) = 0;
	virtual GThread* g_thread_try_new(const gchar* name, GThreadFunc func, gpointer data, GError** error) = 0;
	virtual void g_free(gpointer mem) = 0;
	virtual gpointer g_thread_join(GThread* thread) = 0;
	virtual void g_thread_exit(gpointer retval) = 0;
	virtual gpointer g_try_malloc(gsize n_bytes) = 0;
	virtual void g_cond_wait(GCond* cond, GMutex* mutex) = 0;
};

class GlibMock : public Glib
{
public:
	GlibMock(void)
	{
		s_glibMock = this;
	}

	virtual ~GlibMock(void)
	{
		s_glibMock = nullptr;
	}

	MOCK_METHOD2(g_try_realloc, gpointer(gpointer, gsize));
	MOCK_METHOD4(g_thread_try_new, GThread*(const gchar*, GThreadFunc, gpointer, GError**));
	MOCK_METHOD1(g_free, void(gpointer));
	MOCK_METHOD1(g_thread_join, gpointer(GThread*));
	MOCK_METHOD1(g_thread_exit, void(gpointer));
	MOCK_METHOD1(g_try_malloc, gpointer(gsize n_bytes));
	MOCK_METHOD2(g_cond_wait, void(GCond*, GMutex*));

public:
	static GlibMock* s_glibMock;
};

/******************************************************************************************************
 * LOCAL VARIABLES                                                                                    *
 *****************************************************************************************************/

GlibMock* GlibMock::s_glibMock = nullptr;

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

extern "C" {

gpointer g_try_realloc(gpointer const mem, const gsize n_bytes)
{
	if (nullptr == GlibMock::s_glibMock)
	{
		ADD_FAILURE() << "g_try_realloc(): nullptr == GlibMock::s_glibMock";
		return nullptr;
	}
	return GlibMock::s_glibMock->g_try_realloc(mem, n_bytes);
}

GThread* g_thread_try_new(const gchar* const name, GThreadFunc const func, gpointer const data, GError** const error)
{
	if (nullptr == GlibMock::s_glibMock)
	{
		ADD_FAILURE() << "g_thread_try_new(): nullptr == GlibMock::s_glibMock";
		return nullptr;
	}
	return GlibMock::s_glibMock->g_thread_try_new(name, func, data, error);
}

void g_free(gpointer const mem)
{
	ASSERT_NE(nullptr, GlibMock::s_glibMock) << "g_free(): nullptr == GlibMock::s_glibMock";
	GlibMock::s_glibMock->g_free(mem);
}

gpointer g_thread_join(GThread* const thread)
{
	if (nullptr == GlibMock::s_glibMock)
	{
		ADD_FAILURE() << "g_thread_join(): nullptr == GlibMock::s_glibMock";
		return nullptr;
	}
	return GlibMock::s_glibMock->g_thread_join(thread);
}

void g_thread_exit(gpointer const retval)
{
	ASSERT_NE(nullptr, GlibMock::s_glibMock) << "g_thread_exit(): nullptr == GlibMock::s_glibMock";
	GlibMock::s_glibMock->g_thread_exit(retval);
}

gpointer g_try_malloc(const gsize n_bytes)
{
	if (nullptr == GlibMock::s_glibMock)
	{
		ADD_FAILURE() << "g_try_malloc(): nullptr == GlibMock::s_glibMock";
		return nullptr;
	}
	return GlibMock::s_glibMock->g_try_malloc(n_bytes);
}

void g_cond_wait(GCond* const cond, GMutex* const mutex)
{
	ASSERT_NE(nullptr, GlibMock::s_glibMock) << "g_cond_wait(): nullptr == GlibMock::s_glibMock";
	GlibMock::s_glibMock->g_cond_wait(cond, mutex);
}

}

#endif /*< GLIB_MOCK_HPP_ */
