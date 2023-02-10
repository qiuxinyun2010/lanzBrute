#include <stdio.h>
#include <requests.h>
#include <string.h>
using namespace requests;
const char* url = "http://wwta.lanzoub.com/ikLuO0iwztid";

static http_headers get_sign_headers;

static http_headers headers;
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
    printf("%p %p %d\n", start_p,end_p,len);
    memmove(ret,start_p,len);
    ret[len] = '\0';
    // printf("%s\n",msg);
    // printf("%s\n",ret);
    return ret;
}

int main() {
    get_sign_headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36 Edg/109.0.1518.78";
    
    
    char sign[100] ={0};
    get_sign(sign);
    printf("%s\n",sign);
    // free(sign);
    Request req(new HttpRequest);
    req->method = HTTP_POST;
    req->url = "https://wwta.lanzoub.com/ajaxm.php";
    req->timeout = 5; // 10min
    req->content_type = MULTIPART_FORM_DATA;

    headers["Host"] = "wwta.lanzoub.com";
    headers["Content-Type"] = "application/x-www-form-urlencoded";
    headers["Accept"] = "application/json, text/javascript, */*";
    headers["Origin"] = "https://wwta.lanzoub.com";
    headers["X-Requested-With"] = "XMLHttpRequest";
    headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36";
    headers["Sec-Fetch-Site"] = "same-origin";
    headers["Cookie"] = "codelen=1; ai_user=FWg/iqzvP3hpX3jVbUYNDB|2023-01-03T11:39:57.357Z; pc_ad1=1";
    req->headers = headers;

    req->SetFormData("p", "1234");
    req->SetFormData("action", "downprocess");
    req->SetFormData("sign", sign);
    

    auto resp = request(req);
        if (resp == NULL) {
        // return NULL;
        printf("request failed!\n");
    } 
    auto msg = resp->body.c_str();
    printf("%d %s\r\n", resp->status_code, resp->status_message());
    printf("%s\n",msg);
    // headers = {
    //     "Host": "wwta.lanzoub.com",
    //     "Connection": "keep-alive",
    //     "Accept": "application/json, text/javascript, */*",
    //     "Sec-Fetch-Dest": "empty",
    //     "X-Requested-With": "XMLHttpRequest",
    //     "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36",
    //     "Content-Type": "application/x-www-form-urlencoded",
    //     "Origin": "https://wwta.lanzoub.com",
    //     "Sec-Fetch-Site": "same-origin",
    //     "Sec-Fetch-Mode": "cors",
    //     "Accept-Encoding": "gzip, deflate, br",
    //     "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8,ja;q=0.7",
    //     "Cookie": "codelen=1; ai_user=FWg/iqzvP3hpX3jVbUYNDB|2023-01-03T11:39:57.357Z; pc_ad1=1",
    // }

    return 0;
}
