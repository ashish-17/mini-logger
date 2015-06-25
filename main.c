#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

void func1() {
    LOG_PROLOG();
    func2();
    LOG_EPILOG();
}

void func2() {
    LOG_PROLOG();
    func3();
    LOG_EPILOG();
}

void func3() {
    LOG_PROLOG();
    LOG_EPILOG();
}

int main() {
    func1();
    func2();
    func3();

    int *p = (int*)malloc(sizeof(int));

    LOG_INFO("%d", 1);
    LOG_INFO("%d %d", 1, 2);
    LOG_INFO("%d %d %d", 1, 2, 3);
    LOG_INFO("%s %d %f", "ashish", 2, 3.2);
    LOG_INFO("%s %d %f %u", "ashish", 2, 3.2, p);

    LOG_WARN("%d", 1);
    LOG_WARN("%d %d", 1, 2);
    LOG_WARN("%d %d %d", 1, 2, 3);
    LOG_WARN("%s %d %f", "ashish", 2, 3.2);
    LOG_WARN("%s %d %f %u", "ashish", 2, 3.2, p);

    LOG_ERROR("%d", 1);
    LOG_ERROR("%d %d", 1, 2);
    LOG_ERROR("%d %d %d", 1, 2, 3);
    LOG_ERROR("%s %d %f", "ashish", 2, 3.2);
    LOG_ERROR("%s %d %f %u", "ashish", 2, 3.2, p);

    LOG_DEBUG("%d", 1);
    LOG_DEBUG("%d %d", 1, 2);
    LOG_DEBUG("%d %d %d", 1, 2, 3);
    LOG_DEBUG("%s %d %f", "ashish", 2, 3.2);
    LOG_DEBUG("%s %d %f %u", "ashish", 2, 3.2, p);
    return 0;
}
