/******************************************************************************************************
 * @file plog_version.c                                                                               *
 * @date:      @author:                   Reason for change:                                          *
 * 29.06.2023  Gaina Stefan               Initial version.                                            *
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
