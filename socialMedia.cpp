struct Post{
    int pID, timeStamp, like;
    bool expired;
    Post* next;
    bool operator>(const Post& b) const{
        if(b.expired){
            return timeStamp > b.timeStamp;
        }else {
            if (like == b.like)
                return timeStamp > b.timeStamp;
            return like > b.like;
        }
    }
};

struct User{
    int followId[1001];
    int followCnt;
    Post* myPost;
};

Post post[100001];
User users[1001];

inline void makeFollow(int myId, int followId){
    users[myId].followId[users[myId].followCnt++] = followId;
}

inline void insertPost(Post*& head, Post*& newPost){
    newPost->next=head;
    head=newPost;
}

inline Post* getNewPost(int pid, int stamp){
    post[pid].pID = pid, post[pid].timeStamp = stamp, post[pid].like = 0, post[pid].next = 0, post[pid].expired = false;
    return &post[pid];
}

inline Post* findPost(int pid){
    return &post[pid];
}

inline User* getUser(int uid){
    return &users[uid];
}

struct PQ {
    Post* heap[1001];
    int heapSize = 0;
    inline bool isEmpty(){
        return heapSize == 0;
    }
    inline void heapInit() {
        heapSize = 0;
    }
    inline int heapPush(Post* value) {
        if (heapSize + 1 > MAX_SIZE) {
            //printf("queue is full!");
            return 0;
        }

        heap[heapSize] = value;

        int current = heapSize;
        while (current > 0 && *heap[current] > *heap[(current - 1) / 2]) {
            Post* temp = heap[(current - 1) / 2];
            heap[(current - 1) / 2] = heap[current];
            heap[current] = temp;
            current = (current - 1) / 2;
        }

        heapSize = heapSize + 1;

        return 1;
    }
    inline int heapPop(Post** value) {
        if (heapSize <= 0) {
            return -1;
        }

        *value = heap[0];
        heapSize = heapSize - 1;

        heap[0] = heap[heapSize];

        int current = 0;
        while (current * 2 + 1 < heapSize) {
            int child;
            if (current * 2 + 2 == heapSize) {
                child = current * 2 + 1;
            } else {
                child = *heap[current * 2 + 1] > *heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
            }

            if (*heap[current] > *heap[child]) {
                break;
            }

            Post* temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;

            current = child;
        }
        return 1;
    }
};

PQ validPQ, expiredPQ;

void init(int N){
    for(int i=0; i<=N; i++) {
        users[i].followCnt = 0;
        users[i].myPost = 0;
    }
}

void follow(int uID1, int uID2, int timestamp){
    makeFollow(uID1, uID2);
}

void makePost(int uID, int pID, int timestamp){
    register User* u = getUser(uID);
    register Post* newPost = getNewPost(pID, timestamp);
    insertPost(u->myPost, newPost);
}

void like(int pID, int timestamp){
    register Post* post = findPost(pID);
    post->like++;
}

void getFeed(int uID, int timestamp, int pIDList[]){
    validPQ.heapInit(), expiredPQ.heapInit();
    register User* u = getUser(uID);
    register Post* p = u->myPost;
    register int expiredN = 0;
    while(p!=0){
        if(p->timeStamp>=timestamp-1000){
            validPQ.heapPush(p);
        }
        else {
            if (expiredN > 10) {
                break;
            }
            else {
                expiredN++;
                p->expired = true;
                expiredPQ.heapPush(p);
            }
        }
        p=p->next;
    }

    for(int i=0; i<u->followCnt; i++){
        p = getUser(u->followId[i])->myPost;
        expiredN=0;
        while(p!=0){
            if(p->timeStamp>=timestamp-1000){
                validPQ.heapPush(p);
            } else{
                if(expiredN>10){
                    break;
                }else{
                    expiredN++;
                    p->expired = true;
                    expiredPQ.heapPush(p);
                }
            }
            p = p->next;
        }
    }

    register int retCnt=0;

    while(retCnt<10) {
        register Post *popped;
        if(!validPQ.isEmpty()) {
            validPQ.heapPop(&popped);
            pIDList[retCnt++] = popped->pID;
        }else{
            if(!expiredPQ.isEmpty()){
                expiredPQ.heapPop(&popped);
                pIDList[retCnt++]=popped->pID;
            }else{
                break;
            }
        }
    }
}