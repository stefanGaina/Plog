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

/******************************************************************************************************
 * @file plog_version.c                                                                               *
 * @date:      @author:                   Reason for change:                                          *
 * 29.06.2023  Gaina Stefan               Initial version.                                            *
 * 08.12.2023  Gaina Stefan               Added copyright comment.                                    *
 * 20.12.2023  Gaina Stefan               Updated copyright.                                          *
 * @details This file implements the interface defined in plog_version.h.                             *
 * @todo N/A.                                                                                         *
 * @bug No known bugs.                                                                                *
 *****************************************************************************************************/

/******************************************************************************************************
 * HEADER FILE INCLUDES                                                                               *
 *****************************************************************************************************/

#include "plog_version.h"

/******************************************************************************************************
 * FUNCTION DEFINITIONS                                                                               *
 *****************************************************************************************************/

plog_Version_t plog_get_version(void)
{
	return (plog_Version_t){ PLOG_VERSION_MAJOR, PLOG_VERSION_MINOR, PLOG_VERSION_PATCH };
}
