#!/usr/bin/python
import urllib, urllib2, re, sys, cookielib, popen2

def SGFposToGTP(pos):
    col = ord(pos[0])
    row = ord(pos[1])
    if col >= ord('i'):
        col-=1
    if row >= ord('i'):
        row-=1
    row -= ord('a') - 1
    pos = "{}{}".format(chr(col),row)
    return pos

def GTPposToSGF(pos):
    print "Converting {} to SGF coords:".format(pos)

def generateGTP(SGFdata):
    boardsizeREXP = "SZ\[(\d{1,})\]"
    komiREXP = "KM\[(\d{1,}\.\d{1,})\]"
    blackREXP = ";B\[(.?.?)\]"
    whiteREXP = ";W\[(.?.?)\]"
    komi = 6.5 
    boardsize = 19 
    commands = []
    for line in SGFdata.splitlines():
        result = re.search(blackREXP,line)
        if result:
            move = ""
            if result.groups()[0] is "":
                move = "PASS"
            else:
                move = SGFposToGTP(result.groups()[0])
            commands.append("play b {}".format(move))
        result = re.search(whiteREXP,line)
        if result:
            move = ""
            if result.groups()[0] is "":
                move = "PASS"
            else:
                move = SGFposToGTP(result.groups()[0])
            commands.append("play w {}".format(move))
#        result = re.search(komiREXP,line)
#        if result:
#            commands.append("komi {}".format(result.groups()[0]))
        result = re.search(boardsizeREXP,line)
        if result:
            commands.append("boardsize {}".format(result.groups()[0]))
    return commands

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


#Download and convert SGF to GTP commands
GIDregexp = "sgf.php\?gid=([0-9]{6,})"
gameIDs = re.search(GIDregexp,loginsrc).groups()


if len(gameIDs)>0:
    print "Games found!"    
    for l in gameIDs:
        print "Downloading ID: {}".format(l)
        gameurl = "http://www.dragongoserver.net/sgf.php?gid={}".format(l)
        baseurl = urllib2.Request(gameurl)
        logincon = opener.open(baseurl)
        SGFdata = logincon.read()

        GTPcommands = generateGTP(SGFdata)

        #Run Og-Go A.I.
        r, w= popen2.popen2('../bin/oggoai -ai { ucb t 5 r 20 }')
        print "Started Og-Go A.I."
        w.writelines(GTPcommands)
      #  w.flush()
        print "Wrote"
        r.flush()
        r.flush()
        w.write("genmove b")
       # w.flush()
        w.close()
        print "Wrote to program. Parsing feedback"
        print r.readlines()
        print "DONE"
        r.close()


        

"""        
        #Play move
        gameID = 0
        move_num = 0
        coord_alpha = "aa"
        moveurl = "http://www.dragongoserver.net/game.php?g={}&move={}&a=domove&c={}&nextstatus=true".format(gameID,move_num,coord_alpha)
"""
