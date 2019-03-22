# mysql-cgi-demo

1. 使用 [mysql c api](https://dev.mysql.com/doc/refman/8.0/en/c-api-functions.html)

2. 使用 C++ 实现

3. 场景是用户注册/登陆

4. 缺陷

    1. 没有考虑 SQL 注入

    2. 没有考虑 用户 id 的加密安全问题

    3. 没有实现 session 功能
