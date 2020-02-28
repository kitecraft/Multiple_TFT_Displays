#include "QueueItem.h"


Queues::Queues()
{
	DataQueue<DISPLAY_QUEUE_ITEM> displayQueue(20);

    displayQueueMux = portMUX_INITIALIZER_UNLOCKED;
}

Queues::~Queues()
{

}

void Queues::AddItemToDisplayQueue(char key, String value, int screenID)
{
	DISPLAY_QUEUE_ITEM qi;
	qi.key = key;
	qi.value = value;
	qi.screen = screenID;
	
	portENTER_CRITICAL(&displayQueueMux);
	displayQueue.enqueue(qi);
	portEXIT_CRITICAL(&displayQueueMux);
}

