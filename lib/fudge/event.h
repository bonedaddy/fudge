#define EVENT_BROADCAST                 0
#define EVENTS                          64
#define EVENT_ANY                       0x00
#define EVENT_KILL                      0x01
#define EVENT_EMPTY                     0x02
#define EVENT_DATA                      0x03
#define EVENT_FILE                      0x04
#define EVENT_STOP                      0x05
#define EVENT_KEYPRESS                  0x10
#define EVENT_KEYRELEASE                0x11
#define EVENT_MOUSEMOVE                 0x12
#define EVENT_MOUSEPRESS                0x13
#define EVENT_MOUSERELEASE              0x14
#define EVENT_CONSOLEDATA               0x15
#define EVENT_TIMERTICK                 0x16
#define EVENT_VIDEOMODE                 0x17
#define EVENT_BLOCKREQUEST              0x18
#define EVENT_WMMAP                     0x20
#define EVENT_WMUNMAP                   0x21
#define EVENT_WMCONFIGURE               0x22
#define EVENT_WMSHOW                    0x23
#define EVENT_WMHIDE                    0x24
#define EVENT_WMKEYPRESS                0x28
#define EVENT_WMKEYRELEASE              0x29
#define EVENT_WMMOUSEMOVE               0x2A
#define EVENT_WMMOUSEPRESS              0x2B
#define EVENT_WMMOUSERELEASE            0x2C

struct event_route
{

    unsigned int target;
    unsigned int session;

};

struct event_header
{

    unsigned int type;
    unsigned int source;
    unsigned int target;
    unsigned int session;
    unsigned int length;
    unsigned int nroutes;
    struct event_route routes[16];

};

union event_message
{

    struct event_header header;
    char buffer[FUDGE_BSIZE];

};

struct event_file
{

    unsigned char descriptor;

};

struct event_keypress
{

    unsigned char scancode;

};

struct event_keyrelease
{

    unsigned char scancode;

};

struct event_mousepress
{

    unsigned int button;

};

struct event_mouserelease
{

    unsigned int button;

};

struct event_mousemove
{

    char relx;
    char rely;

};

struct event_consoledata
{

    unsigned char data;

};

struct event_timertick
{

    unsigned int counter;

};

struct event_videomode
{

    unsigned int w;
    unsigned int h;
    unsigned int bpp;

};

struct event_blockrequest
{

    unsigned int offset;
    unsigned int count;

};

struct event_wmconfigure
{

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    unsigned int padding;
    unsigned int lineheight;

};

struct event_wmkeypress
{

    unsigned char scancode;

};

struct event_wmkeyrelease
{

    unsigned char scancode;

};

struct event_wmmousepress
{

    unsigned int button;

};

struct event_wmmouserelease
{

    unsigned int button;

};

struct event_wmmousemove
{

    char relx;
    char rely;

};

unsigned int event_avail(struct event_header *header);
unsigned int event_addroute(struct event_header *header, unsigned int target, unsigned int session);
unsigned int event_addfile(struct event_header *header, unsigned int descriptor);
unsigned int event_addblockrequest(struct event_header *header, unsigned int offset, unsigned int count);
unsigned int event_addwmconfigure(struct event_header *header, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int padding, unsigned int lineheight);
unsigned int event_addwmkeypress(struct event_header *header, unsigned char scancode);
unsigned int event_addwmkeyrelease(struct event_header *header, unsigned char scancode);
unsigned int event_addwmmousepress(struct event_header *header, unsigned int button);
unsigned int event_addwmmouserelease(struct event_header *header, unsigned int button);
unsigned int event_addwmmousemove(struct event_header *header, char relx, char rely);
unsigned int event_append(struct event_header *header, unsigned int count, void *buffer);
void event_reset(struct event_header *header);
void event_create(struct event_header *header, unsigned int type);
void event_create2(struct event_header *header, unsigned int type, unsigned int length);
