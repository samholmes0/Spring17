#include "ProducerConsumer.h"
#include "BoundedBuffer.h"

BoundedBuffer* buffer = new BoundedBuffer(5);
int items_cap; //max items
int items_created = 0; //items created
int p_time;
int c_time;

pthread_mutex_t inc = PTHREAD_MUTEX_INITIALIZER; 

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
// The constructor for the ProducerConsumer class takes several inputs:
// items: the total number of data items that should be produced by all the producer threads collectively before they are done.

	if (p < 1 || c < 1) {
		printf ("Number of producers and consumers must be at least 1.");
		exit (1);
	}

	items_cap = items;
	p_time = psleep;
	c_time = csleep;

	pthread_t pid[p+c];

	int i;
	for (i=0; i < p; i++){
		pthread_create(&pid[i], NULL, producer, (void *)&pid[i]);
	}
	for (i=p; i < p+c; i++){
		pthread_create(&pid[i], NULL, consumer, (void *)&pid[i]);
	}	
	for(i=0; i < p+c; i++){
		pthread_join(pid[i], NULL); 
	}


}

void* producer(void* threadID){
	while (true) { 	

		if (items_created == items_cap){
			break;	
		}

		usleep(p_time);

		int a = 1;
		buffer -> append(1);
		
		pthread_mutex_lock(&inc) ;
		items_created++;
		pthread_mutex_unlock(&inc);
	}

}

void* consumer(void* threadID){
	while ( !(buffer -> isEmpty()) ) {
		usleep(c_time);
		int a = buffer -> remove();
	}
}
