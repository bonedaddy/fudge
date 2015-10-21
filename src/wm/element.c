#include <abi.h>
#include <fudge.h>
#include "box.h"
#include "element.h"

void element_init(struct element *element, unsigned int type, unsigned int count)
{

    element->id = (unsigned int)element;
    element->type = type;
    element->count = count;
    element->z = 1;

}

void element_initmouse(struct element_mouse *mouse)
{

    element_init(&mouse->base, ELEMENT_TYPE_MOUSE, 0);

    mouse->base.z = 2;

}

void element_initpanel(struct element_panel *panel)
{

    element_init(&panel->base, ELEMENT_TYPE_PANEL, sizeof (struct element_panelheader));

}

void element_inittext(struct element_text *text, unsigned int type)
{

    element_init(&text->base, ELEMENT_TYPE_TEXT, sizeof (struct element_textheader));

    text->header.type = type;

}

void element_initwindow(struct element_window *window)
{

    element_init(&window->base, ELEMENT_TYPE_WINDOW, sizeof (struct element_windowheader));

}
