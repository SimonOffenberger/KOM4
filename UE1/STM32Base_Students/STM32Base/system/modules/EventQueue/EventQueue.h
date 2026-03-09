/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 *
 */
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdint.h>

// Max amouts of events in the queue
#define EVENT_QUEUE_LEN 10

// Define for the EventType No Event
#define EVENT_NO_EVENT 0

// Define for the Return Values
#define EVENT_QUEUE_OP_SUCCESSFUL 0
#define EVENT_QUEUE_FULL 1
#define EVENT_QUEUE_EMPTY 2

// Defines for the different Events
// Defintion of the Event info
#define EVENT_INFO_BUTTON_USER0 0
#define EVENT_INFO_BUTTON_USER1 1
#define EVENT_INFO_BUTTON_WAKEUP 2
#define EVENT_TSC_CALIB_UP_LEFT 3
#define EVENT_TSC_CALIB_UP_RIGHT 4
#define EVENT_TSC_CALIB_LOW_LEFT 5
#define EVENT_TSC_CALIB_LOW_RIGHT 6
#define EVENT_Soft_Key_START 7
#define EVENT_Soft_Key_CLEAR 8
#define EVENT_Soft_Key_MINIMUM 9
#define EVENT_Soft_Key_MAXIMUM 10
#define EVENT_SAMPLING_COMPLETE 11

/*******************************************************************************
Provides information about an event.
*******************************************************************************/
typedef struct Event_s
{
    int32_t EventType; // The type of the event (e.g. Button pressed)
    int32_t X;         // Additional information (meaning depends on event type).
    int32_t Y;         // Additional information (meaning depends on event type).
    int32_t Z;         // Additional information (meaning depends on event type).
} Event_t;

// Defining the Queue structure
typedef struct
{
    Event_t events[EVENT_QUEUE_LEN];
    uint16_t front;
    uint16_t rear;
    uint16_t used;
} EventQueueT;

/*******************************************************************************
Initializes the event queue module.
Must be called exactly once before any other functions.
returns: Zero on success, an error code otherwise.
*******************************************************************************/
int EventQueue_Initialize(void);

/*******************************************************************************
Adds an event to the event queue.
event: A pointer to the event to add.
returns: Zero on success, an error code otherwise.
0: Success
1: Event queue is full; event not added to the queue.
*******************************************************************************/
int EventQueue_Put(Event_t const *event);

/*******************************************************************************
Gets an event from the event queue.
event: A pointer to an event.
returns: Zero on success, an error code otherwise.
0: Success
1: Event queue is empty
*******************************************************************************/
int EventQueue_Get(Event_t *event);

/*******************************************************************************
Clears the event queue.
*******************************************************************************/
void EventQueue_Clear(void);

#endif // EVENT_QUEUE_H
