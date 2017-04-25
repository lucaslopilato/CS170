/**********
 *Read Write Lock
 *Implementation Based on https://www.cs.ucsb.edu/~cs170/slides/5synchAdv.pdf
 *
 *
 *
 **********/

#include "rwlock.h"

RWLock::RWLock(){
    #ifdef RWLOCK
        //Initialize Lock for Read/Write counters
        pthread_mutex_init(&this->lock, NULL);

        //Initialize Counter Variables
        this->AR = 0; //locks[0]
        this->WR = 0; //locks[1]
        this->AW = 0; //locks[2]
        this->WW = 0; //locks[3]

        //Initialize Condition Variables
        pthread_cond_init(&this->okToRead, NULL);
        pthread_cond_init(&this->okToWrite, NULL);
    #else
        pthread_mutex_init(&this->lock, NULL);
    #endif
}

RWLock::~RWLock(){
    #ifdef RWLOCK
        pthread_mutex_destroy(&this->lock);

        pthread_cond_destroy(&this->okToRead);
        pthread_cond_destroy(&this->okToWrite);
    #else
        pthread_mutex_destroy(&this->lock);
    #endif
}

void RWLock::startRead(){
    #ifdef RWLOCK
        //Enter Queue
        pthread_mutex_lock(&lock);
        WR++;

        //Wait until there are no active or waiting writers
        while((AW + WW) != 0){
            //printf("AW:%d, WW:%d\n", AW, WW);
            pthread_cond_wait(&okToRead, &lock);
        }

        AR++;

        WR--;
        pthread_mutex_unlock(&lock);

    #else
        pthread_mutex_lock(&this->lock);
    #endif

}

void RWLock::doneRead(){
    #ifdef RWLOCK

        //Decrement Number of readers
        pthread_mutex_lock(&lock);
        AR--;

        signal();

        //Release Active Readers
        pthread_mutex_unlock(&lock);

    #else
        pthread_mutex_unlock(&this->lock);
    #endif
}

void RWLock::startWrite(){
    #ifdef RWLOCK

        //Enter Queue
        pthread_mutex_lock(&lock);
        WW++;

        while((AW + AR) != 0){
            //printf("AW:%d, AR:%d\n", AW, AR);
            pthread_cond_wait(&okToWrite, &lock);
        }

        //Increment Active Writers
        AW++;

        //Exit Queue
        WW--;
        pthread_mutex_unlock(&lock);

    #else
        pthread_mutex_lock(&this->lock);
    #endif
}
void RWLock::doneWrite(){
    #ifdef RWLOCK

        //Decrement Number of Active Writers
        pthread_mutex_lock(&lock);
        AW--;

        signal();

        pthread_mutex_unlock(&lock);        

    #else
        pthread_mutex_unlock(&this->lock);
    #endif
}

//Signal other threads
void RWLock::signal(){
    #ifdef RWLOCK

    if(WW > 0 && (AW + AR) == 0){
        //printf("Signalling ok to write\n");
        pthread_cond_signal(&okToWrite);
    }
    else if(AW == 0){
        //printf("Signalling ok to read\n");
        pthread_cond_signal(&okToRead);
    }

    #else
    //printf("Not working\n");

    #endif
}
