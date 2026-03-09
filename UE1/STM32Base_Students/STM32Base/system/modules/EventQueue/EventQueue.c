/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#include "EventQueue.h"
#include "CriticalSection.h"

// Module internal definition of the used Queue
static EventQueueT Queue;

/**
 * @brief Function that initializes the global Queue
 * 
 * @return int EVENT_QUEUE_OP_SUCCESSFUL -> Operation was successful
 */
int EventQueue_Initialize(void){
	// reset the index for the status variables
	Queue.front=0;
	Queue.rear=0;
	Queue.used=0;

	return EVENT_QUEUE_OP_SUCCESSFUL;
}
 
/**
 * @brief Function that enqueues an event into the EventQueu
 * @brief This is done by copying the contents of the event into the queue
 * 
 * @param event Event to be enqueued into the Event Queue
 * @return int EVENT_QUEUE_FULL is Queue is full the event is not in the queue
 * @return int EVENT_QUEUE_OP_SUCCESSFUL Event is now in the queue
 */
int EventQueue_Put(Event_t const * event){

	// if queue is ful return
	if(Queue.used >= EVENT_QUEUE_LEN) return EVENT_QUEUE_FULL;
	
	// Enter the Criticals Section -> no Interrupt can preempt this operation
	const uint32_t priMask = CriticalSection_Enter();

	// Copy the values to the Event Queue
	Queue.events[Queue.rear].EventType = event->EventType;
	Queue.events[Queue.rear].X = event->X;
	Queue.events[Queue.rear].Y = event->Y;
	Queue.events[Queue.rear].Z = event->Z;
	
	// increment rear index und used count
	Queue.rear++;
	Queue.used++;

	// Reenable Interrupts
	CriticalSection_Exit(priMask);

	// wrap around in queue
	if(Queue.rear >= EVENT_QUEUE_LEN) Queue.rear = 0;
	
	return EVENT_QUEUE_OP_SUCCESSFUL;
}
	
/**
 * @brief Function that dequeues a element from the queue
 * 
 * @param event Pointer to which the dequeued element is stored to
 * @return int EVENT_QUEUE_EMPTY is Queue empty
 * @return int EVENT_QUEUE_OP_SUCCESSFUL Event is now in the queue
 */
int EventQueue_Get(Event_t * event){
		// if queue is ful return
		if(Queue.used == 0 ) return EVENT_QUEUE_EMPTY;
		
		// Enter Critical Section
		const uint32_t priMask = CriticalSection_Enter();
		
		// Copy the values to the Event Queue
		event->EventType = Queue.events[Queue.front].EventType;
		event->X = Queue.events[Queue.front].X;
		event->Y = Queue.events[Queue.front].Y;
		event->Z = Queue.events[Queue.front].Z;
		
		// increment rear index
		Queue.front++;
		Queue.used--;

		CriticalSection_Exit(priMask);

		// wrap around in queue
		if(Queue.front >= EVENT_QUEUE_LEN) Queue.front = 0;
		
		return EVENT_QUEUE_OP_SUCCESSFUL;
}
	
/**
 * @brief Function that clear the queue of events
 * 
 */
void EventQueue_Clear(void){
	// The queue does not have to be cleared
	// reseting the status varialbes is sufficient
	//while(EventQueue_Get(NULL));
	EventQueue_Initialize();
}