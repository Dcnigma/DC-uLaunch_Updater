#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define AMS_URL     "https://github.com/XorTroll/uLaunch/releases/download/0.1/uLaunch-v0.1.zip"
#define HEKATE_URL  "https://github.com/Dcnigma/DC-uLaunch_Updater/raw/master/ulaunch-theme.zip"
#define EBOOK_URL     "-"
#define APP_URL     "https://github.com/Dcnigma/DC-uLaunch_Updater/raw/master/DC-uLaunch_Updater.nro"

int downloadFile(const char *url, const char *output, int api_mode);

#endif
