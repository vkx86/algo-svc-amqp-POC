//
// Created by developer on 1/9/22.
//

#ifndef DAGGER_UTILS_H
#define DAGGER_UTILS_H

#include <string>

class Utils {
public:
    static timespec timespec_diff(const timespec &start, const timespec &end);
    static std::string getCurrTime();
};


#endif //DAGGER_UTILS_H
