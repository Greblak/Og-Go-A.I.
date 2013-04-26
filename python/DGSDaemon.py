#!/usr/bin/python
import urllib, urllib2, re, sys, cookielib

username = 'oggoai'
password = 'uiauia'

#Login
cookiejar = cookielib.CookieJar()
opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookiejar))
urlwithlogin = "http://www.dragongoserver.net/login.php?userid={}&passwd={}".format(username,password)

baseurl = urllib2.Request(urlwithlogin)
logincon = opener.open(baseurl)
loginsrc = logincon.read()

#Ensuring login successful
if("OG-Go A.I." not in loginsrc):
    sys.exit()

print 'Login successful! Continuing with script.'
urlwithlogin = "http://www.dragongoserver.net/status.php"

baseurl = urllib2.Request(urlwithlogin)
logincon = opener.open(baseurl)
loginsrc = logincon.read()

GIDregexp = "sgf.php\?gid=([0-9]{6,})"
gameIDs = re.search(GIDregexp,loginsrc).groups()

if len(gameIDs)>0:
    print "Games found!"    
    for l in gameIDs:
        print "Downloading ID: {}".format(l)
        gameurl = "http://www.dragongoserver.net/sgf.php?gid={}".format(l)
        baseurl = urllib2.Request(gameurl)
        logincon = opener.open(baseurl)
        loginsrc = logincon.read()
        print loginsrc



    

