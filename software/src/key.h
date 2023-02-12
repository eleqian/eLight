#ifndef __KEY_H__
#define __KEY_H__

typedef enum {
    KEY_EVENT_NONE,
    KEY_EVENT_PRESS,
    KEY_EVENT_SHORT,
    KEY_EVENT_SHORT_2,
    KEY_EVENT_SHORT_3,
    KEY_EVENT_SHORT_4,
    KEY_EVENT_LONG,
    KEY_EVENT_LONG_REPEAT,
    KEY_EVENT_MAX
} key_event_e;

void key_init(void);
void key_task(void);
key_event_e key_get_event(void);

#endif
