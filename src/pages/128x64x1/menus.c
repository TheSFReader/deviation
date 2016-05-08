/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef OVERRIDE_PLACEMENT
#include "common.h"
#include "pages.h"
#include "gui/gui.h"
#include "config/model.h"
#include "config/tx.h"

enum {
    LABELNUM_X        = 0,
    LABELNUM_WIDTH    = 16,
    LABEL_X           = 17,
    LABEL_WIDTH       = 0,
};
#endif //OVERRIDE_PLACEMENT

static u8 current_menu_id;

static struct menu_obj * const gui = &gui_objs.u.menu;

static struct menu_page * const mp = &pagemem.u.menu_page;
static u16 current_selected[3] = {0, 0, 0};  // 0 is used for main menu, 1& 2 are used for sub menu

static unsigned action_cb(u32 button, unsigned flags, void *data);
static const char *idx_string_cb(guiObject_t *obj, const void *data);
static void menu_press_cb(guiObject_t *obj, s8 press_type, const void *data);
static const char *menu_name_cb(guiObject_t *obj, const void *data);

static int menu_get_next_rowidx(unsigned *i)
{
    for (; *i < PAGEID_LAST; (*i)++) {
        unsigned menu = 0xffff;
        #define PAGEDEF(_id, _init, _event, _exit, _menu, _name) \
        case _id: menu = _menu; break;
        switch(*i) {
            #include "pagelist.h"
        }
        #undef PAGEDEF
        if ((menu & 0xf0) != current_menu_id)
            continue;
        if ((menu & Model.mixer_mode) != Model.mixer_mode)
            continue;
        return menu;
    }
    return -1;
}
static guiObject_t *getobj_cb(int relrow, int col, void *data)
{
    (void)col;
    (void)data;
    return  (guiObject_t *)&gui->name[relrow];
}
static int row_cb(int absrow, int relrow, int y, void *data)
{
    (void)data;
    labelDesc.style = LABEL_LEFT;
    int idx = 0;
    unsigned i = 0;
    while(1) {
        menu_get_next_rowidx(&i);
        if (i >= PAGEID_LAST)
            break;
        if (idx == absrow) {
            GUI_CreateLabelBox(&gui->idx[relrow], LABELNUM_X, y,
                LABELNUM_WIDTH, LINE_HEIGHT,  &DEFAULT_FONT, idx_string_cb, NULL, (void *)(absrow+ 1L));
            GUI_CreateLabelBox(&gui->name[relrow], LABEL_X, y,
                LABEL_WIDTH, LINE_HEIGHT, &labelDesc, menu_name_cb, menu_press_cb, (const void *)(long)i);
            break;
        }
        idx++;
        i++;
    }
    return 1;
}

void PAGE_MenuInit(int page)
{
    int id = 0;
    PAGE_SetModal(0);
    PAGE_SetActionCB(action_cb);
    PAGE_RemoveAllObjects();
    //pagelist.h maps the 'page' variable to a given menu_id
    //page == -1 : Go to previous menu
    //page == 1 << 4 : Go to main menu
    //page == 2 << 4 : Go to model menu
    //page == 3 << 4 : Go to Tx menu
    if (page != -1)
        current_menu_id = (u8)page;
    switch(page) {
        case 1 << 4: id = PAGEID_MENU; break;
        case 2 << 4: id = PAGEID_MODELMNU; break;
        case 3 << 4: id = PAGEID_TXMENU; break;
    }
    PAGE_ShowHeader(_tr(PAGE_GetName(id)));
    mp->current_selected = &current_selected[(current_menu_id >> 4) - 1];

    unsigned idx = 0;
    unsigned i = 0;
    while(1) {
        menu_get_next_rowidx(&i);
        if (i >= PAGEID_LAST)
            break;
        idx++;
        i++;
    }
    GUI_CreateScrollable(&gui->scrollable, 0, HEADER_HEIGHT, LCD_WIDTH, LCD_HEIGHT - HEADER_HEIGHT,
                     LINE_SPACE, idx, row_cb, getobj_cb, NULL, NULL);

    GUI_SetSelected(GUI_ShowScrollableRowOffset(&gui->scrollable, *mp->current_selected));
}

void PAGE_ModelMenuInit(int page)
{
    (void)page;
    PAGE_ChangeByID(PAGEID_MENU, 2 << 4);
}
void PAGE_TxMenuInit(int page)
{
    (void)page;
    PAGE_ChangeByID(PAGEID_MENU, 3 << 4);
}
void PAGE_MenuExit()
{
    *mp->current_selected = GUI_ScrollableGetObjRowOffset(&gui->scrollable, GUI_GetSelected());
}

const char *menu_name_cb(guiObject_t *obj, const void *data)
{
    (void)obj;
    long i = (long)data;
    return PAGE_GetName(i);
}

const char *idx_string_cb(guiObject_t *obj, const void *data)
{
    (void)obj;
    u8 idx = (long)data;
    sprintf(tempstring, "%d.", idx);
    return tempstring;
}

static unsigned action_cb(u32 button, unsigned flags, void *data)
{
    (void)data;
    if ((flags & BUTTON_PRESS) || (flags & BUTTON_LONGPRESS)) {
        if (CHAN_ButtonIsPressed(button, BUT_EXIT)) {
            if (current_menu_id == 1 << 4) {
                PAGE_ChangeByID(PAGEID_MAIN, 0);
            } else {
                PAGE_ChangeByID(PAGEID_MENU, 1 << 4);
            }
        } else {
            // only one callback can handle a button press, so we don't handle BUT_ENTER here, let it handled by press cb
            return 0;
        }
    }
    return 1;
}

void menu_press_cb(guiObject_t *obj, s8 press_type, const void *data)
{
    (void)obj;
    if (press_type == -1) {
        long i = (long)data;
        PAGE_ChangeByID(i, 0);
    }
}
