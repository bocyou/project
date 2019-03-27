#!/usr/bin/python
#coding:utf-8

import urllib2
import re
import random
from bs4 import BeautifulSoup

ua_list = ["Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
        "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
        "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.0 Safari/536.3",
        "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.24 (KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24",
        "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/535.24 (KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24"
        ]

def openPage(url):

    user_agent = random.choice(ua_list)

    req = urllib2.Request(url)
    req.add_header('User-Agent', user_agent)

    rsp = urllib2.urlopen(req)
    data = rsp.read()
    return data

def test1():
    url = "http://www.nmc.cn/publish/forecast/ASN.html"
    print openPage(url)


def parseMainPage(page):
    soup = BeautifulSoup(page, "html.parser")

    return soup


# /publish/forecast/ASN/xian.html
# /publish/forecast/ASN/chengcheng.html

def getCityInfo():
    url = "http://www.nmc.cn/publish/forecast/ASN.html"
    page = openPage(url)
    # print parseMainPage(page)
    soup = parseMainPage(page)

    cityList = {}
    for link in soup.find_all('div', class_='cname') :
        # print link.find_all('a')
        for ui in link.find_all(name='a'):
            # cityList.fromkeys(ui.get('href'), ui.get_text())
            cityList["http://www.nmc.cn" + ui.get('href')] = ui.get_text()
    
    return cityList

def getCityHtml():
    cityUrl = getCityInfo()

    for key, val in cityUrl.items():
        pageHtml = openPage(key)
        fd = open("./wwwroot/html/"+val+".html", mode="w")
        fd.write(pageHtml)
        fd.close()


def test2():
    # getCityInfo()
    getCityHtml()

if __name__ == "__main__":
    # test1()
    test2()
