#pragma	once

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <unistd.h>
#include "../lib/include/mysql.h"

MYSQL *connectMysql();
void selectMysql(MYSQL *my_fd);
void closeMysql(MYSQL *my_fd);
