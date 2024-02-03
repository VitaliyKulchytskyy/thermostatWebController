#pragma once
#include "FTPServer.h"
#include "SDFS.h"
#include "settings.h"


/// FTP Server
extern FTPServer g_ftpSrv;

/**
 * Setup FTP g_webServer.
 * @retval True - the FTP g_webServer is initialized
 * @retval False - the FTP g_webServer isn't initialized
 */
extern bool setupFTP();