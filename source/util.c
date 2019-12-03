#include <stdio.h>
#include <string.h>
#include <switch.h>
#include "util.h"
#include "menu.h"
#include "unzip.h"
#include "download.h"
#include "reboot_payload.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>// check dir
#include <errno.h>// check dir
//need ftw for switch :-D
//#include <ftw.h>

#define TEMP_FILE               "/switch/DC-Ulaunch-update/temp.zip"
#define FILTER_STRING           "browser_download_url\":\""

//uLaunch
//System application (SystemApplicationQHbTarget)
#define TITLE01                 "/atmosphere/titles/01008BB00013C000/exefs.nsp"
//QMenu (LibraryAppletQMenu)
#define TITLE02                 "/atmosphere/titles/010000000000100B/exefs.nsp"
#define TITLE05                 "/atmosphere/titles/010000000000100B/romfs.bin"
#define TITLE06                 "/atmosphere/titles/010000000000100B/fsmitm.flag"
//QDaemon (SystemAppletQDaemon)
#define TITLE03                 "/atmosphere/titles/0100000000001000/exefs.nsp"
//Library applet (LibraryAppletQHbTarget)
//Delete only the exefs.nsp file from 0100000000001000 directory
//(if there is a romfs folder present it could be a normal HOME menu theme)
#define TITLE04                 "/atmosphere/titles/0100000000001001/exefs.nsp"
char g_sysVersion[50];
char g_amsVersion[50];


char *getSysVersion()
{
    return g_sysVersion;
}

char *getAmsVersion()
{
    return g_amsVersion;
}

void writeSysVersion()
{
	Result ret = 0;
	SetSysFirmwareVersion ver;

	if (R_FAILED(ret = setsysGetFirmwareVersion(&ver)))
    {
		printf("GetFirmwareVersion() failed: 0x%x.\n\n", ret);
		return;
	}

    char sysVersionBuffer[20];
	snprintf(sysVersionBuffer, 20, "%u.%u.%u", ver.major, ver.minor, ver.micro);
    snprintf(g_sysVersion, sizeof(g_sysVersion), "Firmware Ver: %s", sysVersionBuffer);
}

void writeAmsVersion()
{
	Result ret = 0;
	u64 ver;
    u64 fullHash;
    SplConfigItem SplConfigItem_ExosphereVersion = (SplConfigItem)65000;
    SplConfigItem SplConfigItem_ExosphereVerHash = (SplConfigItem)65003;

	if (R_FAILED(ret = splGetConfig(SplConfigItem_ExosphereVersion, &ver)))
    {
		printf("SplConfigItem_ExosphereVersion() failed: 0x%x.\n\n", ret);
		return;
	}

    if (R_FAILED(ret = splGetConfig(SplConfigItem_ExosphereVerHash, &fullHash)))
    {
		printf("SplConfigItem_ExosphereVerHash() failed: 0x%x.\n\n", ret);
		return;
	}

    // write only the first 8 char of the hash.
    char shortHash[8];
	snprintf(shortHash, sizeof(shortHash), "%lx", fullHash);

    // write ams version number + hash.
    char amsVersionNum[25];
	snprintf(amsVersionNum, sizeof(amsVersionNum), "%lu.%lu.%lu (%s)", (ver >> 32) & 0xFF,  (ver >> 24) & 0xFF, (ver >> 16) & 0xFF, shortHash);

    // write string + ams version to global variable.
    snprintf(g_amsVersion, sizeof(g_amsVersion), "Atmosphere Ver: %s", amsVersionNum);
}

void copyFile(char *src, char *dest)
{
    FILE *srcfile = fopen(src, "rb");
    FILE *newfile = fopen(dest, "wb");

    if (srcfile && newfile)
    {
        char buffer[10000]; // 10kb per write, which is fast
        size_t bytes;       // size of the file to write (10kb or filesize max)

        while (0 < (bytes = fread(buffer, 1, sizeof(buffer), srcfile)))
        {
            fwrite(buffer, 1, bytes, newfile);
        }
    }
    fclose(srcfile);
    fclose(newfile);
}

int parseSearch(char *parse_string, char *filter, char* new_string)
{
    FILE *fp = fopen(parse_string, "r");

    if (fp)
    {
        char c;
        while ((c = fgetc(fp)) != EOF)
        {
            if (c == *filter)
            {
                for (int i = 0, len = strlen(filter) - 1; c == filter[i]; i++)
                {
                    c = fgetc(fp);
                    if (i == len)
                    {
                        for (int j = 0; c != '\"'; j++)
                        {
                            new_string[j] = c;
                            new_string[j+1] = '\0';
                            c = fgetc(fp);
                        }
                        fclose(fp);
                        remove(parse_string);
                        return 0;
                    }
                }
            }
        }
    }

    errorBox(350, 250, "Failed to find parse url!");
    fclose(fp);
    return 1;
}

int update_dcapps(char *url, char *output, int mode)
{
    if (mode == UP_AMS)
    {
// ask if user wants to install atmosphere as well.

        int res = yesNoBox(mode, 390, 250, "Install uLanch for Atmosphere");

        if (res == YES)
        {

//AMS_URL= TitleUpdate.zip
    if (!downloadFile(AMS_URL, TEMP_FILE, OFF))
    {
//##Uninstalling
      //Make sure you don't remove anything else but the stuff mentioned here, in order to avoid any potential trouble!
      //Remove 0100000000001001, 010000000000100B and 01008BB00013C000 folders from your CFW's titles folder.
      //Remove ONLY the exefs.nsp file from 0100000000001000 folder inside titles (there might be a romfs folder, which could be a
      //normal HOME menu theme/NXTheme)
//###(Optional)
      //if you wish to completely remove uLaunch, you can make a full reset by removing the ulaunch folder
      //in SD root. Note that this will remove any folders or special entries/homebrew added to main menu!
      //(only the accesses, not the homebrew itself)
//Remove titles before install. otherwise update will not work.
      remove(TITLE01);
      remove(TITLE02);
      remove(TITLE03);
      remove(TITLE04);
      remove(TITLE05);
      remove(TITLE06);
	    rename(TEMP_FILE, AMS_OUTPUT);
// AMS_OUTPUT              "/atmosphere/TitleUpdate.zip"
      unzip(AMS_OUTPUT, mode);
//  AMS_OUTPUTZIP    "/SdOut/titles/"
    //copy uLauch
//System application (SystemApplicationQHbTarget)
      mkdir("/atmosphere/titles/01008BB00013C000", 0700);
      rename("/titles/01008BB00013C000/exefs.nsp","/atmosphere/titles/01008BB00013C000/exefs.nsp");
//QMenu (LibraryAppletQMenu)
      mkdir("/atmosphere/titles/010000000000100B", 0700);
      rename("/titles/010000000000100B/exefs.nsp","/atmosphere/titles/010000000000100B/exefs.nsp");
      rename("/titles/010000000000100B/fsmitm.flag","/atmosphere/titles/010000000000100B/fsmitm.flag");
      rename("/titles/010000000000100B/romfs.bin","/atmosphere/titles/010000000000100B/romfs.bin");
//QDaemon (SystemAppletQDaemon)
      mkdir("/atmosphere/titles/0100000000001000", 0700);
      rename("/titles/0100000000001000/exefs.nsp","/atmosphere/titles/0100000000001000/exefs.nsp");
//Library applet (LibraryAppletQHbTarget)
      mkdir("/atmosphere/titles/0100000000001001", 0700);
      rename("/titles/0100000000001001/exefs.nsp","/atmosphere/titles/0100000000001001/exefs.nsp");
//end copy uLauch
//cleanup content of zip (still manual)
      remove("/titles/01008BB00013C000/exefs.nsp");
      remove("/titles/0100000000001000/exefs.nsp");
      remove("/titles/0100000000001001/exefs.nsp");
      remove("/titles/010000000000100B/exefs.nsp");
      remove("/titles/010000000000100B/fsmitm.flag");
      remove("/titles/010000000000100B/romfs.bin");
// Remove can't remove folders. (need to check NX-Shell how to do that)
//FS_RemoveDirRecursive("/SdOut");
//BOOL RemoveDirectory( LPCTSTR "/SdOut" );
      system("rm -r /SdOut");
      rmdir("/titles/0100000000001000");
      rmdir("/titles/01008BB00013C000");
      rmdir("/titles/010000000000100B");
      rmdir("/titles/0100000000001001");
      rmdir("/titles");
//rmdir("/SdOut"); update 0.2 will unpack in to titles.

//end cleanup ulaunch zip


errorBox(400, 250, "Install/Update complete!\nRestart Payload to take effect");

	             }
                return 0;
            }
            return 1;
        }
        return 1;
}
int update_ebooks(char *url, char *output, int mode)
{
    if (mode == UP_AMS_NOINI)
    {
	     remove(TITLE01);
  	   remove(TITLE02);
    	 remove(TITLE03);
       remove(TITLE04);
       remove(TITLE05);
       remove(TITLE06);
       errorBox(400, 250, "Remove complete!\nRestart app to take effect");
	    }

            return 1;
        }




int update_amiibo(char *url, char *output, int mode)
{
    if (mode == UP_HEKATE)
    {
        // ask if user wants to install atmosphere as well.
        int res = yesNoBox(mode, 390, 250, "Install All Themes?");

        if (res == YES)
        {
            // ask if user wants to overwite the atmosphere ini files.
            res = yesNoBox(mode, 355, 250, "Will overwite older themes?");

            if (res == YES)
            {
//Download Themes?
//Pack01
    if (!downloadFile(HEKATE_URL, TEMP_FILE, OFF))
    {
	rename(TEMP_FILE, HEKATE_OUTPUT);
  unzip(HEKATE_OUTPUT, mode);
  {
//Pack02
	if (!downloadFile(THEME_URL2, TEMP_FILE, OFF))
  rename(TEMP_FILE, HEKATE_OUTPUT);
  unzip(HEKATE_OUTPUT, mode);
//Pack03
  if (!downloadFile(THEME_URL3, TEMP_FILE, OFF))
  rename(TEMP_FILE, HEKATE_OUTPUT);
  unzip(HEKATE_OUTPUT, mode);
//Pack04
  if (!downloadFile(THEME_URL4, TEMP_FILE, OFF))
  rename(TEMP_FILE, HEKATE_OUTPUT);
  unzip(HEKATE_OUTPUT, mode);

                }
                return 0;
            }
            return 1;
        }
        return 1;
    }
    return 1;
  }
return 1;
}
void disable_app(char *url, char *output, int mode)
{
  if (mode == OFFLINE_MODE)
  {
      // ask if user wants to install atmosphere as well.
      int res = yesNoBox(mode, 390, 250, "Disable uLaunch?\nMake Off-line Backup!");
    //  errorBox(400, 250, "      Enabled!\nReboot payload to take effect");

      if (res == YES)
      {
        //The easiest way to disable uLaunch would be renaming its main file, exefs.nsp inside 0100000000001000 folder,
        //inside your CFW's titles folder, to something like disabled.nsp (anything different will be enough).
        //To re-enable it again, just rename it back to exefs.nsp.
        rename("/atmosphere/titles/0100000000001000/exefs.nsp","/atmosphere/titles/0100000000001000/disabled.nsp");
        errorBox(400, 250, "Disabled!\nReboot payload to take effect");
        //Oldcode...
        //RENAME System application (SystemApplicationQHbTarget)
        //rename("/atmosphere/titles/01008BB00013C000","/atmosphere/titles/01008BB00013C000BAK");
        //RENAME QMenu (LibraryAppletQMenu)
        //rename("/atmosphere/titles/010000000000100B","/atmosphere/titles/010000000000100BBAK");
        //RENAME QDaemon (SystemAppletQDaemon)
        // create Backup folder otherwise blue atmosphere screen of death
        //mkdir("/atmosphere/titles/0100000000001000BAK", 0700);
        //RENAME Library applet (LibraryAppletQHbTarget)
        //rename("/atmosphere/titles/0100000000001001","/atmosphere/titles/0100000000001001BAK");
        //end copy uLauch

        {
}
  }
    }
      }
void Enable_app(char *url, char *output, int mode)
{
  if (mode == OFFLINE_MODE)
  {
      // ask if user wants to install atmosphere as well.
      int res = yesNoBox(mode, 390, 250, "Enable uLaunch?\nFrom Off-line Backup?");
    //  errorBox(400, 250, "      Enabled!\nReboot payload to take effect");

          if (res == YES)
          {
            rename("/atmosphere/titles/0100000000001000/disabled.nsp","/atmosphere/titles/0100000000001000/exefs.nsp");
            errorBox(400, 250, "Enabled!\nReboot payload to take effect");
            //Oldcode...
            //rename("/atmosphere/titles/01008BB00013C000BAK","/atmosphere/titles/01008BB00013C000");
            //RENAME QMenu (LibraryAppletQMenu)
            //rename("/atmosphere/titles/010000000000100BBAK","/atmosphere/titles/010000000000100B");
            //RENAME QDaemon (SystemAppletQDaemon)
            //RENAME Library applet (LibraryAppletQHbTarget)
            //rename("/atmosphere/titles/0100000000001001BAK","/atmosphere/titles/0100000000001001");
            //end copy uLauch

          }
return ;
        }

      }
      void removeold_app()
      {
        rename("/switch/uLaunch-update","/switch/uLaunch-update-old");
        rmdir("/switch/uLaunch-update-old");
        errorBox(400, 250, "OLD Install folder Renamed\n Please Update complete!\nRestart app to take effect");
      }

void update_app()
{
    // download new nro as a tempfile.
    if (!downloadFile(APP_URL, TEMP_FILE, OFF))
    {
      DIR* dir = opendir(OLDDER_APP_PATH);
      if (dir)
        {
          /* Directory exists. */
          remove("/switch/Ulaunch-update/DC-uLaunch_Updater.nro");
          // remove nro from /switch/.
          //rmdir("/switch/uLaunch-update");
          // rename the downloaded temp_file with the correct nro name.
          rename(TEMP_FILE, APP_OUTPUT);
          closedir(dir);
          remove(TEMP_FILE);
          removeold_app();
          }
    else if (ENOENT == errno) {
          remove(APP_OUTPUT);
          rmdir("/switch/uLaunch-update-old");
          // remove nro from /switch/.
          //remove(OLD_APP_PATH);
          rename(TEMP_FILE, APP_OUTPUT);  /* Directory does not exist. */
          errorBox(400, 250, "Update complete!\nRestart app to take effect");
          remove(TEMP_FILE);
          sleep (3);

          }
    else {
          rmdir("/switch/uLaunch-update-old");  /* opendir() failed for some other reason. */
          }
        // remove current nro file.

        // using errorBox as a message window on this occasion.

    }

}
