//
// Created by jmpews on 2018/6/14.
//

#ifndef HOOKZZ_SINGLETON_H
#define HOOKZZ_SINGLETON_H

#include <pthread.h>

template <class T>
class Singleton {
  private:
    Singleton(){};

  public:
    static T *GetInstance();

  private:
    static T *instance_;
    static pthread_mutex_t mutex_;
};

#endif //HOOKZZ_SINGLETON_H
