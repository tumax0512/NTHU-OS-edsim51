#include "table.hpp"
#include "stdio.h"
#include <pthread.h> 
Table::Table(int n) {
    // TODO: implement table constructor (value, mutex, cond)
    // 初始化信號量的值為 n
    value = n;

    // 初始化互斥鎖和條件變數
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
}

void Table::wait() {
    // TODO: implement semaphore wait
    // 鎖住互斥鎖，確保同一時間只有一個執行緒能夠進入這段程式碼
    pthread_mutex_lock(&mutex);

    // 如果信號量的值 <= 0，代表已經有 4 個哲學家正在用餐，需要等待
    while (value <= 0) {
        // 等待條件變數的激發，並且釋放互斥鎖
        pthread_cond_wait(&cond, &mutex);
    }

    // 減少信號量的值，代表有一個哲學家正在用餐
    value--;

    // 釋放互斥鎖
    pthread_mutex_unlock(&mutex);
}

void Table::signal() {
    // TODO: implement semaphore signal
    // 鎖住互斥鎖，確保同一時間只有一個執行緒能夠進入這段程式碼
    pthread_mutex_lock(&mutex);

    // 增加信號量的值，代表有一個哲學家用餐完畢
    value++;

    // 激發條件變數，通知其他等待中的執行緒有空位可以用餐
    pthread_cond_signal(&cond);

    // 釋放互斥鎖
    pthread_mutex_unlock(&mutex);
}

Table::~Table() {
    // TODO: implement table destructor (mutex, cond)
    // 刪除互斥鎖和條件變數
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}