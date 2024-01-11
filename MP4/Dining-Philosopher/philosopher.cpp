#include "philosopher.hpp"
#include "fork.hpp"
#include <pthread.h> 
pthread_t tid[5];
Fork fk[5];
int beUsed[5]={0,0,0,0,0};
int i;
Philosopher::Philosopher(int id, Fork *leftFork, Fork *rightFork, Table *table) :id(id), cancelled(false), leftFork(leftFork), rightFork(rightFork), table(table) {
    srand((unsigned) time(&t1));
    
}

void Philosopher::start() {
    // TODO: start a philosopher thread
   
    pthread_create(&tid[i], NULL,run, this);
}

int Philosopher::join() {
    // TODO: join a philosopher thread
    
    
    pthread_join(tid[i],NULL);
	  
    return 0;
}

int Philosopher::cancel() {
    // TODO: cancel a philosopher thread
    int i ;
    cancelled = true;
    
    pthread_cancel(tid[i]);
    
    return 0;
}

void Philosopher::think() {
    int thinkTime = rand() % MAXTHINKTIME + MINTHINKTIME;
    sleep(thinkTime);
    // printf("Philosopher %d is thinking for %d seconds.\n", id, thinkTime);
}

void Philosopher::eat() {
    // printf("Philosopher %d is eating.\n", id);
    sleep(EATTIME);
}

void Philosopher::pickup(int id) {
    // TODO: implement the pickup interface, the philosopher needs to pick up the left fork first, then the right fork
    
    int left,right;
    if(id == 4){
      left = id;
		  right = 0;
	  }
    else{
		  left = id;
		  right = id+1;
	  }
    fk[left].wait(); //拿到左手的筷子。
    beUsed[left]=1;
		printf("philosopher %d fetches fork %d\n",id,left);
   
		while(beUsed[right==1]){//右邊已被拿走 拿右手的筷子失败
			fk[left].signal();//右手筷子被拿走，放下左手的筷子。
      beUsed[left]=0;
			continue;
		}
    printf("philosopher %d fetches fork %d\n",id,right);

}

void Philosopher::putdown(int id) {
    // TODO: implement the putdown interface, the philosopher needs to put down the left fork first, then the right fork
    int left,right;
    if(id == 4){
      left = 0;
		  right = id;
	  }
    else{
		  left = id;
		  right = id+1;
	  }
    fk[right].signal(); //放下右手的筷子。
		beUsed[right]=0;
    printf("philosopher %d release chopstick %d\n",id,right);
    
		fk[left].signal(); //放下左手的筷子。
		beUsed[left]=0;
    printf("philosopher %d release chopstick %d\n",id,left);

}

void Philosopher::enter() {
    // TODO: implement the enter interface, the philosopher needs to join the table first
    table->wait();
}

void Philosopher::leave() {
    // TODO: implement the leave interface, the philosopher needs to let the table know that he has left
    table->signal();
}

void* Philosopher::run(void* arg) {
    // TODO: complete the philosopher thread routine. 
    Philosopher* p = static_cast<Philosopher*>(arg);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    //給好的
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    while (!p->cancelled) {
        p->think();
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

        p->eat();

        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    }
    

    return NULL;
}