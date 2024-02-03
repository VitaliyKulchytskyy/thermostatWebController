#include "handlers/ftpHandler.h"


FTPServer g_ftpSrv(SDFS);

bool setupFTP() {
    g_ftpSrv.begin(device_name, password_ftp);
    return true;
}
