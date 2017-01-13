# TCP-IP_Communication
Matlab调用C++程序进行数据传输
1、VC中编写Mex函数

　　新建一个win32 dll 空项目。

2、添加源文件Test.cpp，编写MEX函数，MEX函数编写方法见附录。

3、配置项目属性　　

　　打开项目属性配置页，C++ -> 附加包含目录 加入MATLAB安装目录下的 \extern\include 路径。

　　连接器 -> 附加库目录 加入MATLAB的 \extern\lib\win32\microsoft 路径。

　　连接器 -> 输入 -> 附加依赖项 输入libmx.lib libeng.lib libmat.lib libmex.lib 这四个lib文件。

　　在Linker-General-Output File:改成

　　　　　　　　　　　　　　　　　　　.\Debug\(ProjectName).mexw32（64位系统相应改成64）

　　Source Files- Add - New Item新建模块定义文件Test.def

　　并为其添加内容

　　　　　　　　　　LIBRARY ;"Test"
　　　　　　　　　　EXPORTS mexFunction

　　在Linker-Input-Module Definition File添加：Test.def

4、在matlab中cd到vs项目目录（例：cd F:\project\vs\communication\communication\），输入mex XXX.cpp

生成一个sendData.mexw64，在matlab中直接调用XXX.mexw64就行。


附录：

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

参考：

1、http://www.cnblogs.com/lukylu/p/4042306.html

2、http://blog.csdn.net/junmuzi/article/details/49338307

3、http://blog.csdn.net/carson2005/article/details/24656999
