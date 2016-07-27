#include <WinSock2.h>  
#include <stdio.h>  
  
#include "mex.h"

#pragma comment(lib, "ws2_32.lib")  
  
struct Package
{
	int packageLen;
	int deviceNo;
	float message;
	wchar_t pend;
};

void sendData(int len, int no, float mess, wchar_t end)
{
	//加载套接字  
    WSADATA wsaData;  
    char buff[1024];  
    memset(buff, 0, sizeof(buff));  
  
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)  
    {  
        printf("Failed to load Winsock");  
        return;  
    }  
  
    SOCKADDR_IN addrSrv;  
    addrSrv.sin_family = AF_INET;  
    addrSrv.sin_port = htons(20101);  
    addrSrv.sin_addr.S_un.S_addr = inet_addr("202.115.13.252");  
  
    //创建套接字  
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);  
    if(SOCKET_ERROR == sockClient)
	{  
        printf("Socket() error:%d", WSAGetLastError());  
        return;  
    }  
  
    //向服务器发出连接请求  
    if(connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET)
	{  
        printf("Connect failed:%d", WSAGetLastError());  
        return;  
    }        

	//封装数据包
    Package package = Package();
	package.packageLen = len;
	package.deviceNo = no;
	package.message = mess;
	package.pend = end;

    //发送数据  
    send(sockClient, (const char*)&(package), sizeof(package), 0); 

	//接收服务器回应  
    recv(sockClient, buff, sizeof(buff), 0);  
    printf("%s\n", buff);  

    //关闭套接字  
    closesocket(sockClient);  
    WSACleanup(); 
}
 
//nlhs 输出参数个数
//plhs 输出参数指针
//nrhs 输入参数个数
//prhs 输入参数指针
void mexFunction( int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[] )
{
	int a;
	int b;
	float c;
	wchar_t d;

	/* 检查参数 */

	if (nrhs != 4) 
	{
		mexErrMsgTxt("Four input argument required.");

	} else if (nlhs > 5) {

		mexErrMsgTxt("Too many output arguments.");

	} else if (!mxIsNumeric(prhs[0])) {

		mexErrMsgTxt("Argument must be numeric.");

	} else if (mxGetNumberOfElements(prhs[0]) != 1 || mxIsComplex(prhs[0])) {

		mexErrMsgTxt("Argument must be non-complex scalar.");
	}

	/* 为输出参数创建变量 */

	plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);


	a = *(mxGetPr(prhs[0]));

    b = *(mxGetPr(prhs[1]));

	c = *(mxGetPr(prhs[2]));

    d = *(mxGetPr(prhs[3]));

	/* 调用sendData 子函数 */

	sendData(a, b, c, d);

}