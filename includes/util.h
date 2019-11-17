#ifndef _UTIL_H_
#define _UTIL_H_

#define ROOT                    "/"
#define APP_PATH                "/switch/Ulaunch-update/"
#define AMS_OUTPUT              "/atmosphere/TitleUpdate.zip"
#define HEKATE_OUTPUT           "/switch/Ulaunch-update/ulaunch-theme.zip"
#define APP_OUTPUT              "/switch/Ulaunch-update/DC-uLaunch_Updater.nro"
#define OLD_APP_PATH            "/switch/DC-uLaunch_Updater.nro"
//Remove ebook in clean up!
#define EBOOK_OUTPUT		         "/switch/DC-Switch/ebook.zip"




#define YES                     10
#define NO                      20
#define ON                      1
#define OFF                     0

void writeSysVersion();                                                 // writes the sys version.
void writeAmsVersion();                                                 // writes the ams version with hash.
char *getSysVersion();                                                  // returns sys version.
char *getAmsVersion();                                                  // returns ams version.

void copyFile(char *src, char *dest);                                   // basic copy file. Use malloc if you need dynamic mem
int parseSearch(char *phare_string, char *filter, char* new_string);    // hacky way to parse a file for a string
int update_dcapps(char *url, char *output, int mode);               	// update either ams or hekate
int update_ebooks(char *url, char *output, int mode);               	// update either ams or hekate
int update_amiibo(char *url, char *output, int mode);               	// update either amiibo
void update_app();                                                      // update the app

#endif
