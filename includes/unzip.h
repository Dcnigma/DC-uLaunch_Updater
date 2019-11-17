#ifndef _UNZIP_H_
#define _UNZIP_H_

#define UP_AMS          0
#define UP_AMS_NOINI    1
#define UP_HEKATE       2
#define OFFLINE_MODE    3
#define UP_APP          4
#define REBOOT_PAYLOAD  5


int unzip(const char *output, int mode);

#endif
