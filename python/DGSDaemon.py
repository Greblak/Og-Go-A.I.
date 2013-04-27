#!/usr/bin/python
import urllib, urllib2, re, sys, cookielib, popen2,time

def SGFposToGTP(pos,boardsize):
    ##REVERSED! FIX THIS
    col = ord(pos[0])
    row = ord(pos[1]) - ord('a')
    if col >= ord('i'):
        col-=1
    if row >= ord('i'):
        row-=1
    row = boardsize - row
    pos = "{}{}".format(chr(col),row)
    return pos

def GTPposToSGF(pos,boardsize):
    print "Converting {} to SGF coords:".format(pos)
    colGTP = pos[0]
    rowGTP = pos[1:]

    colSGF = colGTP.lower()
    rowSGF = chr(boardsize - int(rowGTP) + ord('a'))

    if ord(colSGF) >= ord('i'):
        colSGF = char(ord(colSGF)+1)
    if ord(rowSGF) >= ord('i'):
        rowSGF = char(ord(rowSGF)+1)
    return "{}{}".format(colSGF,rowSGF)
    
def generateGTP(SGFdata):
    boardsizeREXP = "SZ\[(\d{1,})\]"
    komiREXP = "KM\[(\d{1,}\.\d{1,})\]"
    blackREXP = ";B\[(.?.?)\]"
    whiteREXP = ";W\[(.?.?)\]"
    komi = 6.5 
    boardsize = 19 
    commands = []
    nummoves = 0
    for line in SGFdata.splitlines():
        result = re.search(blackREXP,line)
        if result:
            move = ""
            if result.groups()[0] is "":
                move = "PASS"
            else:
                move = SGFposToGTP(result.groups()[0],boardsize)
            commands.append("play b {}".format(move))
            nummoves += 1
        result = re.search(whiteREXP,line)
        if result:
            move = ""
            if result.groups()[0] is "":
                move = "PASS"
            else:
                move = SGFposToGTP(result.groups()[0],boardsize)
            commands.append("play w {}".format(move))
            nummoves += 1
        result = re.search(komiREXP,line)
        if result:
            komi = float(result.groups()[0])
#            commands.append("komi {}".format(result.groups()[0]))
        result = re.search(boardsizeREXP,line)
        if result:
            boardsize = int(result.groups()[0])
            commands.append("boardsize {}".format(result.groups()[0]))
    return commands,boardsize,nummoves

class Oggoai:
    def __init__(self):
        self.fin, self.fout = popen2.popen2("../bin/oggoai -ai { ucb t 5 r 20 }")
        print self.sendCommand("name")
        print self.sendCommand("version")
    def sendCommand(self, cmd):
        print cmd
        self.fout.write(cmd + "\n")
        self.fout.flush()
    def quit(self):
        self.fout.write("quit")
        self.fout.flush()

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



if re.search(GIDregexp,loginsrc):
    gameIDs = re.search(GIDregexp,loginsrc).groups()
    print "Games found!"    
    for l in gameIDs:
        print "Downloading ID: {}".format(l)
        gameurl = "http://www.dragongoserver.net/sgf.php?gid={}".format(l)
        baseurl = urllib2.Request(gameurl)
        logincon = opener.open(baseurl)
        SGFdata = logincon.read()

        GTPcommands,boardsize,nummoves = generateGTP(SGFdata)
        prg = Oggoai()
        # Prepare process

        
        for cmd in GTPcommands:
            prg.sendCommand(cmd)
     
        time.sleep(7) #Time is not important. Allow slaves to connect prior to genmove command
        genmovecmd = "genmove b"
        prg.sendCommand(genmovecmd)
        prg.sendCommand("quit")
        o = ""
        move = ""
        moveRegexp = "= (.\d{1,2})"
        while True:
            o = prg.fin.readline()
            if re.search(moveRegexp,o):
                print o
                move = GTPposToSGF(re.search(moveRegexp,o).groups()[0],boardsize)
                break
        prg.quit()
        print move

        #Play move
        gameID = l
        move_num = nummoves
        coord_alpha = move
        moveurl = "http://www.dragongoserver.net/game.php?gid={}&move={}&action=domove&coord={}&nextstatus=true".format(gameID,move_num,coord_alpha)
        print "Attempting to play move on: {}".format(moveurl)
        baseurl = urllib2.Request(moveurl)
        opener.open(baseurl)
else:
    print "No games found"
