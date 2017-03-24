#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int N){
	int* buffer = new int[N]();
	int buffer_size = N;
	int buffer_cnt = 0; //item count
	int buffer_last = N-1; //tail

	pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER; //lock
	pthread_cond_t buffer_full = PTHREAD_COND_INITIALIZER; //keeps track of the number of items on the buffer
	pthread_cond_t buffer_empty = PTHREAD_COND_INITIALIZER; //keeps track of the (N - (number of items on the buffer))

	printf("%d\n", buffer_cnt);
}

int modulo(int a,int m){
	return (a % m + m) %m;
}

void BoundedBuffer::append(int data){
	printf("append called...");
	pthread_mutex_lock(&buffer_lock); //acquire lock 
	printf("lock acquired...");
	while(buffer_cnt > buffer_size){
		pthread_cond_wait(&buffer_empty, &buffer_lock);
		printf("waiting on append...");
			//block thread on condition varaible buffer_empty
			//upon successful return, the buffer_lock will have been locked and owned by the calling thread
	}

	int i = modulo((buffer_last + 1), buffer_size);
	buffer[i] = data;
	buffer_last = i;
	buffer_cnt++;

	pthread_cond_signal(&buffer_full); //unblocks threads blocked on buffer_full (i.e. threads that called remove)
	pthread_mutex_unlock(&buffer_lock); //release lock
}

int BoundedBuffer::remove(){
	printf("remove called...");
	pthread_mutex_lock(&buffer_lock); //acquire lock
	while(buffer_cnt < 1){
		pthread_cond_wait(&buffer_full, &buffer_lock); 
		printf("waiting on remove...");
			//block thread on condition varaible buffer_full (which is signalled by append)
			//upon successful return, the buffer_lock will have been locked and owned by the calling thread
	}

	int i = modulo((buffer_last + 1 - buffer_cnt), buffer_size);
	int a = buffer[i];
	buffer[i] = 0;
	buffer_cnt--;
	
	
	pthread_cond_signal(&buffer_empty); //unblocks threads blocked on buffer_empty (i.e. threads that called append)
	pthread_mutex_unlock(&buffer_lock); //release lock
	return a; 	//return item
}

bool BoundedBuffer::isEmpty(){
	printf("%d\n", buffer_cnt);
	return buffer_cnt == 0;
}

