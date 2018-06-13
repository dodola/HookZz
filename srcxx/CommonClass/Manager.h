//
// Created by z on 2018/6/14.
//

#ifndef HOOKZZ_MANAGER_H
#define HOOKZZ_MANAGER_H

template <typename ItemType>
class CollectionSharedManager {
public:
    std::vector<ItemType> items;
private:
    static CollectionSharedManager *manager;
};
#endif //HOOKZZ_MANAGER_H
