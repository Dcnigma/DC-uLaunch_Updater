#include <unistd.h>
#include <switch.h>

#include "menu.h"
#include "touch.h"
#include "util.h"

#define APP_VERSION "uLaunch Updater: 0.0.5"
#define UL_VERSION "uLaunch version: 0.1"

void refreshScreen()
{
    clearRenderer();

    // app version.
    drawText(fntMedium, 40, 40, SDL_GetColour(white), APP_VERSION);

    // system version.
    drawText(fntSmall, 25, 150, SDL_GetColour(white), getSysVersion());
    //uLaunch version
    drawText(fntSmall, 25, 190, SDL_GetColour(white), getAmsVersion());
    // atmosphere version.
    drawText(fntSmall, 25, 230, SDL_GetColour(purple), UL_VERSION);

    //drawText(fntMedium, 120, 225, SDL_GetColour(white), "Menu Here"); // menu options
    drawButton(fntButton, BUTTON_A, 970, 672, SDL_GetColour(white));
    drawText(fntSmall, 1010, 675, SDL_GetColour(white), "Select");
    drawButton(fntButton, BUTTON_PLUS, 1145, 672, SDL_GetColour(white));
    drawText(fntSmall, 1185, 675, SDL_GetColour(white), "Exit");
}

void printOptionList(int cursor)
{
    refreshScreen();

    char *option_list[]      = {    "Install uLaunch?", \
                                    "De-Install uLaunch", \
                                    "Get uLaunch Themes-showcase (discord)", \
                                    "Disable or Enabled Offline mode", \
                                    "Update app", \
                                    "Reboot (reboot to payload)" };

    char *description_list[] = {    "Warning this is for Atmosphere!", \
                                    "De-Install uLaunch for Atmosphere!", \
                                    "Get All Themes for uLaunch (Updated 11-17-19)", \
                                    "Disable or Enabled Offline mode", \
                                    "Update app and removes old version", \
                                    "Reboots switch (recommended after updating or De-install)" };
//Add
    SDL_Texture *textureArray[] = { ams_icon, ams_plus_icon, hekate_icon, offline_icon, app_icon, reboot_icon };

    for (int i=0, nl=0; i < (CURSOR_LIST_MAX+1); i++, nl+=NEWLINE)
    {
        if (cursor != i) drawText(fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(white), option_list[i]);
        else
        {
            // icon for the option selected.
            drawImage(textureArray[i], 125, 350);
            // highlight box.
            drawShape(SDL_GetColour(black), 530, (FIRST_LINE + nl - HIGHLIGHT_BOX_MIN), 700, HIGHLIGHT_BOX_MAX);
            // option text.
            drawText(fntSmall, 550, FIRST_LINE+nl, SDL_GetColour(white), option_list[i]);
            // description.
            drawText(fntSmall, 25, 675, SDL_GetColour(white), description_list[i]);
        }
    }
}

void popUpBox(TTF_Font *font, int x, int y, SDL_Colour colour, char *text)
{
    // outline. box
    drawShape(SDL_GetColour(black), (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box. Background color
    drawShape(SDL_GetColour(dark_grey), SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
    // text to draw.
    drawText(font, x, y, colour, text);
}

int yesNoBox(int mode, int x, int y, char *question)
{
    printOptionList(mode);
    popUpBox(fntMedium, x, y, SDL_GetColour(white), question);
    // highlight box.
    drawShape(SDL_GetColour(black), 380, 410, 175, 65);
    drawShape(SDL_GetColour(black), 700, 410, 190, 65);
    // option text.
    drawButton(fntButtonBig, BUTTON_B, 410, 425, SDL_GetColour(white));
    drawText(fntMedium, 455, 425, SDL_GetColour(white), "No");
    drawButton(fntButtonBig, BUTTON_A, 725, 425, SDL_GetColour(white));
    drawText(fntMedium, 770, 425, SDL_GetColour(white), "Yes");

    updateRenderer();

    int res = 0;
    int touch_lock = OFF;
    touchPosition touch;
    u32 tch = 0;
    u32 touch_count = hidTouchCount();

    // check if the user is already touching the screen.
    if (touch_count > 0) touch_lock = ON;

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        hidTouchRead(&touch, tch);
        touch_count = hidTouchCount();

        if (touch_count == 0) touch_lock = OFF;

        if (touch_count > 0 && touch_lock == OFF)
            res = touch_yes_no_option(touch.px, touch.py);

        if (kDown & KEY_A || res == YES)
            return YES;

        if (kDown & KEY_B || res == NO)
            return NO;
    }
}

void errorBox(int x, int y, char *errorText)
{
    popUpBox(fntMedium, x, y, SDL_GetColour(white), errorText);
    drawImageScale(error_icon, 570, 340, 128, 128);
    updateRenderer();

    sleep(3);
}
