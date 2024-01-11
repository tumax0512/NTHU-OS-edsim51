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
    // 等待互斥鎖
    pthread_mutex_lock(&mutex);
    while (isUsed) {
        // 如果叉子已被使用，等待條件變數
        pthread_cond_wait(&cond, &mutex);
        
    }
    // 將叉子標記為使用中
    isUsed = true;
    // 釋放互斥鎖
    pthread_mutex_unlock(&mutex);

}

void Fork::signal() {

    // TODO: implement semaphore signal
    // 等待互斥鎖
    pthread_mutex_lock(&mutex);
    // 將叉子標記為未使用
    isUsed = false;
    // 發送條件變數信號，通知等待的哲學家
    pthread_cond_signal(&cond);
    // 釋放互斥鎖
    pthread_mutex_unlock(&mutex);
}

Fork::~Fork() {
    // TODO: implement fork destructor (mutex, cond)
     // 釋放互斥鎖和條件變數
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}