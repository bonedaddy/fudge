void print_insertfill(struct ring *ring, struct event_header *header, struct element_fill *fill);
void print_insertmouse(struct ring *ring, struct event_header *header, struct element_mouse *mouse);
void print_insertpanel(struct ring *ring, struct event_header *header, struct element_panel *panel);
void print_inserttext(struct ring *ring, struct event_header *header, struct element_text *text, unsigned int count);
void print_insertwindow(struct ring *ring, struct event_header *header, struct element_window *window);
void print_removefill(struct ring *ring, struct event_header *header, struct element_fill *fill);
void print_removemouse(struct ring *ring, struct event_header *header, struct element_mouse *mouse);
void print_removepanel(struct ring *ring, struct event_header *header, struct element_panel *panel);
void print_removetext(struct ring *ring, struct event_header *header, struct element_text *text);
void print_removewindow(struct ring *ring, struct event_header *header, struct element_window *window);
