#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define AMS_URL     "https://github.com/XorTroll/uLaunch/releases/download/0.1/uLaunch-v0.1.zip"
//Rename to theme
#define HEKATE_URL  "https://www.dropbox.com/s/cwybbymzguv4smy/ulaunch-theme0.1.zip?dl=1"
#define THEME_URL2  "https://www.dropbox.com/s/2z1bdcjwv71bnuu/ulaunch-theme0.2.zip?dl=1"
// Remove or disable
#define EBOOK_URL     "-"
// App download location
#define APP_URL     "https://github.com/Dcnigma/DC-uLaunch_Updater/raw/master/DC-uLaunch_Updater.nro"

// Define how to download 	if (!downloadFile(THEME_URL2, TEMP_FILE, OFF))
int downloadFile(const char *url, const char *output, int api_mode);

#endif
