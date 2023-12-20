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
******************************************************************************************************/

#ifndef CONFIGURATION_MOCK_HPP_
#define CONFIGURATION_MOCK_HPP_

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include <gmock/gmock.h>

extern "C" {
#include "internal/configuration.h"
}

/******************************************************************************************************
 * TYPE DEFINITIONS                                                                                   *
 *****************************************************************************************************/

class Configuration
{
public:
	virtual ~Configuration(void) = default;

	virtual gboolean configuration_read(void) = 0;
	virtual void configuration_write(void) = 0;
};

class ConfigurationMock : public Configuration
{
public:
	ConfigurationMock(void)
	{
		s_configurationMock = this;
	}

	virtual ~ConfigurationMock(void)
	{
		s_configurationMock = nullptr;
	}

	MOCK_METHOD0(configuration_read, gboolean(void));
	MOCK_METHOD0(configuration_write, void(void));

public:
	static ConfigurationMock* s_configurationMock;
};

/******************************************************************************************************
 * LOCAL VARIABLES                                                                                    *
 *****************************************************************************************************/

ConfigurationMock* ConfigurationMock::s_configurationMock = nullptr;

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

extern "C" {

gboolean configuration_read(void)
{
	if (nullptr == ConfigurationMock::s_configurationMock)
	{
		ADD_FAILURE() << "configuration_read(): nullptr == ConfigurationMock::s_configurationMock";
		return FALSE;
	}
	return ConfigurationMock::s_configurationMock->configuration_read();
}

void configuration_write(void)
{
	ASSERT_NE(nullptr, ConfigurationMock::s_configurationMock) << "configuration_write(): nullptr == ConfigurationMock::s_configurationMock";
	ConfigurationMock::s_configurationMock->configuration_write();
}

}

#endif /*< CONFIGURATION_MOCK_HPP_ */
