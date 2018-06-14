//
// Created by jmpews on 2018/6/14.
//

#include "Singleton.h"

pthread_mutex_t Singleton::mutex_;

template <class T>
T *Singleton::GetInstance() {
    if (instance_ == NULL) {
        pthread_mutex_lock(&mutex_);
        if (instance_ == NULL) {
            instance_ = new T();
        }
        pthread_mutex_unlock(&mutex_);
    }
    return instance_;
}