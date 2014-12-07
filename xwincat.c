// xwincat: Log X11 window positions

#include <xcb/xcb.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

const char * DEFAULT_DISPLAY    = ":0";
const char * DEFAULT_PRINT_TIME = false;

int printUsage() {
    printf("\
USAGE: xwincat [-display <display>] [-time]\n\
    display  target X display      (default %s)\n\
    time     also print timestamps (default %s)\n",
        DEFAULT_DISPLAY,
        (DEFAULT_PRINT_TIME ? "yes" : "no") );
    exit(0);
}

int main(int argc, char * argv[]) {

    const char * hostname    = DEFAULT_DISPLAY;
    bool         printTimes  = DEFAULT_PRINT_TIME;

    // Get arguments
    for (int i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-help"))     printUsage();
        else if (!strcmp(argv[i], "-display"))  hostname    = argv[++i];
        else if (!strcmp(argv[i], "-time"))     printTimes  = true;
        else { printf("Unexpected argument `%s`\n", argv[i]); printUsage(); }
    }

    // Set up X
    int screenNum; 
    xcb_connection_t * connection = xcb_connect(hostname, &screenNum);

    const xcb_setup_t * setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    for (int i = 0; i < screenNum; ++i) {
        xcb_screen_next(&iter);
    }
    xcb_screen_t * screen = iter.data;

    printf("screen width:%d height:%d\n",
            screen->width_in_pixels,
            screen->height_in_pixels);

    const static uint32_t values[] = { XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY };
    xcb_void_cookie_t cookie = xcb_change_window_attributes_checked(connection,
            screen->root, XCB_CW_EVENT_MASK, values);
    xcb_generic_error_t * error = xcb_request_check(connection, cookie); 
    xcb_flush(connection);
    if (NULL != error) {
        fprintf(stderr, "Can't get SUBSTRUCTURE_NOTIFY (Error code %d)\n",
                error->error_code);
        xcb_disconnect(connection);
        exit(1);
    }

    while(1) {
        xcb_generic_event_t * ev = xcb_wait_for_event(connection);
        switch (ev->response_type & ~0x80) {
            case XCB_CONFIGURE_NOTIFY: {
                long timestamp = 0;
                if (printTimes) timestamp = (long)time(NULL);

                xcb_configure_notify_event_t * e =
                    (xcb_configure_notify_event_t *)ev;

                printf("window %d %d %d %d %d",
                        e->window,
                        e->x, e->y,
                        e->width, e->height);
                if (printTimes) printf(" %ld", timestamp);
                printf("\n");
                break;
            }
        }
    }
}
