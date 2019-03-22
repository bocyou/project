#pragma once

#include <mysql.h>
#include <cstdio>
#include <cstdlib>

static inline void finish_with_error(MYSQL *con) {
    std::printf("<p>MySQL 执行错误: %s</p>\n", mysql_error(con));
    if (con) {
        mysql_close(con);
    }
    std::exit(EXIT_FAILURE);
}


MYSQL * get_mysql();
