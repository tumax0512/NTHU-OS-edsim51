#include "fork.hpp"
#include <pthread.h> 
bool isUsed;
Fork::Fork() {
    // TODO: implement fork constructor (value, mutex, cond)
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    isUsed = false;
}

void Fork::wait() {
    // TODO: implement semaphore wait
    // ���ݤ�����
    pthread_mutex_lock(&mutex);
    while (isUsed) {
        // �p�G�e�l�w�Q�ϥΡA���ݱ����ܼ�
        pthread_cond_wait(&cond, &mutex);
        
    }
    // �N�e�l�аO���ϥΤ�
    isUsed = true;
    // ���񤬥���
    pthread_mutex_unlock(&mutex);

}

void Fork::signal() {

    // TODO: implement semaphore signal
    // ���ݤ�����
    pthread_mutex_lock(&mutex);
    // �N�e�l�аO�����ϥ�
    isUsed = false;
    // �o�e�����ܼƫH���A�q�����ݪ����Ǯa
    pthread_cond_signal(&cond);
    // ���񤬥���
    pthread_mutex_unlock(&mutex);
}

Fork::~Fork() {
    // TODO: implement fork destructor (mutex, cond)
     // ���񤬥���M�����ܼ�
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}