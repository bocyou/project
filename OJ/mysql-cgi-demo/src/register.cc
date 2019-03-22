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


long insert_user(MYSQL *con, unordered_map<string, string> params) {
    // 有 SQL注入风险
    string query = "INSERT INTO users (username, password) VALUES ('"
                 + params["username"]
                 + "', PASSWORD('"
                 + params["password"]
                 + "'))";

    if (mysql_query(con, query.c_str())) {
        finish_with_error(con);
    } 

    mysql_use_result(con);

    return (long)mysql_insert_id(con);
}


int main() {
    put_headers();
    
    auto params = get_params();
    check_params(params);

    auto con = get_mysql();

    auto user_id = insert_user(con, params);

    printf("<p>注册成功，用户 id: %ld</p>\n", user_id);

    mysql_close(con);
}
