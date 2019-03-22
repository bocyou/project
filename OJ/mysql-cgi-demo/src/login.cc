#include <unencode.hh>
#include <mysql_utils.hh>

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>


using std::unordered_map;
using std::string;
using std::atol;
using std::fgets;
using std::printf;


void put_headers() {
    printf("Content-type: text/html; charset=utf-8\r\n");
    printf("\r\n");
}


unordered_map<string, string> get_params() {
    char *lenchp = getenv("CONTENT_LENGTH");
    if (lenchp == nullptr) {
        printf("<p>错误</p>");
        exit(EXIT_FAILURE);
    }
    long len = atol(lenchp);
    char *content = new char[len + 1];
    fgets(content, len + 1, stdin);

    return unencode(content);
}


void check_params(unordered_map<string, string> const& params) {
    if (params.find("username") == params.cend()) {
        printf("<p>必须指定用户名</p>\n");
        exit(EXIT_FAILURE);
    }

    if (params.find("password") == params.cend()) {
        printf("<p>必须指定密码</p>\n");
        exit(EXIT_FAILURE);
    }
}


long get_user_id(MYSQL *con, unordered_map<string, string> params) {
    // 有 SQL注入风险
    string query = "SELECT id FROM users WHERE username = '"
                 + params["username"]
                 + "' AND PASSWORD = PASSWORD('"
                 + params["password"]
                 + "')";

    if (mysql_query(con, query.c_str())) {
        finish_with_error(con);
    } 


    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
        finish_with_error(con);
    }

    auto nums = mysql_num_rows(result);
    if (nums == 0) {
        mysql_free_result(result);
        return -1;
    }

    MYSQL_ROW row;
    long id = -1;
    while ((row = mysql_fetch_row(result))) {
        id = atol(row[0]);
    }

    // 释放结果集
    mysql_free_result(result);

    return id;
}


int main() {
    put_headers();
    
    auto params = get_params();
    check_params(params);

    auto con = get_mysql();

    auto user_id = get_user_id(con, params);
    if (user_id == -1) {
        printf("<p>用户名或者密码不正确</p>\n");
        mysql_close(con);
        exit(EXIT_FAILURE);
    }

    printf("<p>登陆成功，用户 id: %ld</p>\n", user_id);

    mysql_close(con);
}
