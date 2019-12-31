#ifndef _UTIL_H_
#define _UTIL_H_

#define ROOT                    "/"
#define APP_PATH                "/switch/DC-uLaunch-updater"
#define OLDDER_APP_PATH         "/switch/Ulaunch-update"
#define AMS_OUTPUT              "/atmosphere/TitleUpdate.zip"
//Changed because of Fault in HB appstore.
#define HEKATE_OUTPUT           "/switch/DC-uLaunch-updater/ulaunch-theme.zip"
#define APP_OUTPUT              "/switch/DC-uLaunch-updater/DC-uLaunch_Updater.nro"
#define OLD_APP_PATH            "/switch/DC-uLaunch_Updater.nro"
//Remove ebook in clean up!
#define EBOOK_OUTPUT		        "/switch/DC-uLaunch-updater/ebook.zip"
#define BACKUP_OUTPUT           "/atmosphere/Title/"



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
int update_dcapps(char *url, char *output, int mode);               	  // update either ams or hekate
int update_ebooks(char *url, char *output, int mode);               	  // update either ams or hekate
int update_amiibo(char *url, char *output, int mode);               	  // update either amiibo
void disable_app();                                                     // disable_app ulaunch offline
void Enable_app();                                                      // Enable_app ulaunch offline
void update_app();                                                      // update the app
void removeold_app();
#endif
