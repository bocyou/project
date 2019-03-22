#include <mysql_config.hh>
#include <mysql_utils.hh>


MYSQL * get_mysql() {
    // 创建 MYSQL 结构
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        finish_with_error(con);
    }

    // 连接数据库
    if (mysql_real_connect(
        con, host, user, password, database, 0, NULL, 0) == NULL) {

        finish_with_error(con);
    }

    // 设置客户端字符编码为 utf8
    if (mysql_set_character_set(con, "utf8") != 0) {
        finish_with_error(con);
    }

    return con;
}
