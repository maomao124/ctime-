#include<iostream>
#include<stdio.h>
#include<string.h>
#include <Windows.h>
#include<time.h>
#include <graphics.h>
#include <conio.h>
#include<algorithm>
#include<stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <ddraw.h>
#include<iomanip>
#include <Psapi.h>
#include<vector>
#include<io.h>
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>
#include <sapi.h>                                             // 语音头文件
#include <sphelper.h>                                     // 语音识别头文件
#include <thread>                                           //多线程
#include"resource1.h"
                                                                        //vs2019自定义控制台代码模板 by mao
                                                                        //作者QQ1296193245
using namespace std;
#pragma warning(disable : 4996)
#pragma comment (lib,"ddraw.lib")
#pragma comment(lib,"sapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"Winmm.lib")                     // 引用 Windows Multimedia API
#pragma intrinsic(__rdtsc)
unsigned __int64 t1, t2, t3, t4;
double run_time;                                                   //微秒级算法时间计时器
LARGE_INTEGER time_start;	                                //开始时间
LARGE_INTEGER time_over;	                                //结束时间
double dqFreq;		                                               //计时器频率
LARGE_INTEGER f;	                                               //计时器频率
double tt;
long double totaltime;

inline void runstart()
{
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	t1 = __rdtsc();
	QueryPerformanceCounter(&time_start);	       //计时开始
}
inline void runend()
{
	QueryPerformanceCounter(&time_over);	        //计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	t2 = __rdtsc();
	t3 = t2 - t1;
}
void ShowProcessMemoryUsageInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
	float memoryUsage_K = pmc.WorkingSetSize / 1024.0;
	std::cout << std::fixed << setprecision(0) << "程序内存使用率：" << memoryUsage_K << " KB = " << setprecision(2) << memoryUsage_M << " MB" << std::endl;
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
{
	__int64 a = static_cast<long long>(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	__int64 b = static_cast<long long>(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return   (b - a);
}
void rundisplay()
{
	cout << endl;
	cout << "-------------------------------------------------------" << endl;
	if (t3 < 10000)
	{
		cout << "CPU脉冲数：" << (t3) << endl;
	}
	else if (t3 < 100000000 && t3 >= 10000)
	{
		tt = t3;
		cout << "CPU脉冲数：" << (tt) / 10000 << "万" << endl;
	}
	else if (t3 > 100000000000)
	{
		t4 = t3;
		t4 = t3 / 10000000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10 << "亿" << endl;
	}
	else
	{
		t4 = t3;
		t4 = t3 / 10000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10000 << "亿" << endl;
	}
	if (run_time > 10000)
	{
		cout << "算法运行时间：" << run_time / 1000 << "秒" << endl;
	}
	else if (run_time < 1)
	{
		cout << "算法运行时间：" << run_time * 1000 << "微秒" << endl;
	}
	else
	{
		cout << "算法运行时间：" << run_time << "毫秒" << endl;
	}
	totaltime = clock();
	cout << "程序运行时间：" << totaltime / 1000 << "秒" << endl;
	if (run_time > 0.4)
	{
		double frequency = t3 / run_time / 1000;
		cout << "CPU频率：" << frequency << "MHZ" << endl;
	}
	ShowProcessMemoryUsageInfo();
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)tchData, &dwSize);
		if (lRet == ERROR_SUCCESS)
		{
			cout << "CPU 名字：";
			wcout << tchData << endl;
		}
		else
		{
			cout << "CPU 名字：";
			wcout << L"未知" << endl;
		}
	}
	else
	{
		cout << "CPU 名字：";
		wcout << L"未知" << endl;
	}
	RegCloseKey(hKey);
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	double totalphy = statex.ullTotalPhys / 1024 / 1024;
	totalphy = totalphy / 1024;
	cout << "总物理内存：    " << setprecision(4) << left << setw(7) << totalphy << " GB" << endl;
	double totalvir = statex.ullTotalPageFile / 1024 / 1024;
	totalvir = totalvir / 1024;
	cout << "总虚拟内存：    " << setw(7) << totalvir - totalphy << " GB" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	//   光标位置   
	COORD     cursorPos;
	//   标准输出句柄   
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	double freephy = statex.ullAvailPhys / 1024 / 1024;
	freephy = freephy / 1024;
	double usephy = (totalphy - freephy) * 100.0 / totalphy;
	cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
	double freetotal = statex.ullAvailPageFile / 1024 / 1024;
	freetotal = freetotal / 1024;
	cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
	double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
	cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
	double usevir = freevir * 100 / (totalvir - totalphy);
	cout << "虚拟内存使用率：";
	printf("%.2f%%", usevir);
	cout << endl;
	cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << 0.00 << "%" << endl;
	cout << "CPU空闲率：    " << setw(6) << 100.0<< "%" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "按任意键退出程序" << endl;
	while (!_kbhit())
	{
		GlobalMemoryStatusEx(&statex);
		WaitForSingleObject(hEvent, 1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);
		float cpu = (kernel + user - idle) * 100.0 / (kernel + user);
		float cpuidle = (idle) * 100.0 / (kernel + user);
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 0;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "内存使用率：" << statex.dwMemoryLoad << "%" << endl;
		double freephy = statex.ullAvailPhys / 1024 / 1024;
		freephy = freephy / 1024;
		double usephy = (totalphy - freephy) * 100.0 / totalphy;
		cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 1;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freetotal = statex.ullAvailPageFile / 1024 / 1024;
		freetotal = freetotal / 1024;
		cout << "总空闲内存：    " << setprecision(3) << freetotal <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 2;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
		cout << "虚拟内存使用量：" << setprecision(3) << freevir <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 3;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double usevir = freevir * 100 / (totalvir - totalphy);
		cout << "虚拟内存使用率：";
		printf("%.2f%%", usevir);
		cout << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 4;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << cpu << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 5;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU空闲率：    " << setw(6) << cpuidle << "%"<<endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 6;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "-------------------------------------------------------" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 7;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "按任意键退出程序" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	cout << endl;
	
}


//**************************************************************//************************非主函数



//**************************************************************//************************

int main()
{

	//*************************************************************************



	//*************************************************************************
	runstart();
	//*************************计时开始***************************************
	
	//有四个与时间相关的类型：clock_t、time_t、size_t 和 tm。类型 clock_t、size_t 和 time_t 能够把系统时间和日期表示为某种整数
	/*
	 struct tm {
  int tm_sec;   // 秒，正常范围从 0 到 59，但允许至 61
  int tm_min;   // 分，范围从 0 到 59
  int tm_hour;  // 小时，范围从 0 到 23
  int tm_mday;  // 一月中的第几天，范围从 1 到 31
  int tm_mon;   // 月，范围从 0 到 11
  int tm_year;  // 自 1900 年起的年数
  int tm_wday;  // 一周中的第几天，范围从 0 到 6，从星期日算起
  int tm_yday;  // 一年中的第几天，范围从 0 到 365，从 1 月 1 日算起
  int tm_isdst; // 夏令时
   };
	*/



	//C 库函数 time_t time(time_t * seconds) 返回自纪元 Epoch（1970 - 01 - 01 00:00 : 00 UTC）起经过的时间，以秒为单位
	//如果 seconds 不为空，则返回值也存储在变量 seconds 中
	time_t seconds;
	seconds = time(NULL);
	printf("自 1970-01-01 起的小时数 = %ld\n", seconds / 3600);


	//C 库函数 char *ctime(const time_t *timer) 返回一个表示当地时间的字符串，当地时间是基于参数 timer
	//返回的字符串格式如下： Www Mmm dd hh : mm:ss yyyy 其中，Www 表示星期几，Mmm 是以字母表示的月份，dd 表示一月中的第几天，hh : mm:ss 表示时间，yyyy 表示年份
	time_t curtime;
	time(&curtime);
	printf("当前时间 = %s", ctime(&curtime));


	//C 库函数 struct tm *localtime(const time_t *timer) 使用 timer 的值来填充 tm 结构。timer 的值被分解为 tm 结构，并用本地时区表示
	time_t rawtime;
	struct tm* info;
	char buffer[80];
	time(&rawtime);
	info = localtime(&rawtime);
	printf("当前的本地时间和日期：%s", asctime(info));


	//C 库函数 clock_t clock(void) 返回程序执行起（一般为程序的开头），处理器时钟所使用的时间。为了获取 CPU 所使用的秒数，您需要除以 CLOCKS_PER_SEC。
	//在 32 位系统中，CLOCKS_PER_SEC 等于 1000000，该函数大约每 72 分钟会返回相同的值。
	clock_t start_t, end_t;
	double total_t;
	int i;
	start_t = clock();
	printf("程序启动，start_t = %ld\n", start_t);
	printf("开始一个大循环，start_t = %ld\n", start_t);
	for (i = 0; i < 10000000; i++)
	{
	}
	end_t = clock();
	printf("大循环结束，end_t = %ld\n", end_t);
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("CPU 占用的总时间：%f\n", total_t);


	//C 库函数 char *asctime(const struct tm *timeptr) 返回一个指向字符串的指针，它代表了结构 struct timeptr 的日期和时间
	struct tm t;
	t.tm_sec = 10;
	t.tm_min = 10;
	t.tm_hour = 6;
	t.tm_mday = 25;
	t.tm_mon = 2;
	t.tm_year = 89;
	t.tm_wday = 6;
	puts(asctime(&t));


	//C 库函数 struct tm *gmtime(const time_t *timer) 使用 timer 的值来填充 tm 结构，并用协调世界时（UTC）也被称为格林尼治标准时间（GMT）表示
#define BST (+1)
#define CCT (+8)
	time_t rawtime2;
	struct tm* info2;
	time(&rawtime2);
	/* 获取 GMT 时间 */
	info2 = gmtime(&rawtime2);
	printf("当前的世界时钟：\n");
	printf("伦敦：%2d:%02d\n", (info2->tm_hour + BST) % 24, info2->tm_min);
	printf("中国：%2d:%02d\n", (info2->tm_hour + CCT) % 24, info2->tm_min);


	//C 库函数 time_t mktime(struct tm *timeptr) 把 timeptr 所指向的结构转换为自 1970 年 1 月 1 日以来持续时间的秒数，发生错误时返回-1
	int ret;
	struct tm info3;
	char buffer3[80];
	info3.tm_year = 2021 - 1900;
	info3.tm_mon = 7 - 1;
	info3.tm_mday = 4;
	info3.tm_hour = 0;
	info3.tm_min = 0;
	info3.tm_sec = 1;
	info3.tm_isdst = -1;
	ret = mktime(&info3);
	cout << ret << "秒       ";
	if (ret == -1) 
	{
		printf("Error: unable to make time using mktime\n");
	}
	else 
	{
		strftime(buffer3, sizeof(buffer3), "%c", &info3);
		printf(buffer3);
	}


	//C 库函数 double difftime(time_t time1, time_t time2) 返回 time1 和 time2 之间相差的秒数 (time1 - time2)
	//这两个时间是在日历时间中指定的，表示了自纪元 Epoch（协调世界时 UTC：1970-01-01 00:00:00）起经过的时间
	time_t start_t2, end_t2;
	double diff_t;
	printf("\n");
	time(&start_t2);
	printf("休眠 2 秒...\n");
	Sleep(2000);
	time(&end_t2);
	diff_t = difftime(end_t2, start_t2);
	printf("执行时间 = %f\n", diff_t);


	//C 库函数 size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr) 根据 format 中定义的格式化规则，格式化结构 timeptr 表示的时间，并把它存储在 str 中
	/*
	  说明符	  替换为	                                                                                                      实例
		% a	 缩写的星期几名称	                                                                                      Sun
		% A 	完整的星期几名称	                                                                                     Sunday
		% b	缩写的月份名称	                                                                                           Mar
		% B	完整的月份名称	                                                                                          March
		% c	日期和时间表示法	                                                                                      Sun Aug 19 02:56 : 02 2012
		% d	一月中的第几天（01 - 31）                                                                          	19
		% H	24 小时格式的小时（00 - 23）                                                                    	14
		% I	12 小时格式的小时（01 - 12）                                                                    	05
		% j	一年中的第几天（001 - 366）                                                                	       231
		% m	十进制数表示的月份（01 - 12）                                                                    	08
		% M	分  （00 - 59）                                                                                             	55
		% p	AM 或 PM 名称                                                                                           	PM
		% S	秒（00 - 61）	                                                                                            02
		% U	一年中的第几周，以第一个星期日作为第一周的第一天（00 - 53）                   	33
		% w	十进制数表示的星期几，星期日表示为 0（0 - 6）	                                            4
		% W	一年中的第几周，以第一个星期一作为第一周的第一天（00 - 53）	                   34
		% x	日期表示法	                                                                                               08 / 19 / 12
		% X	时间表示法                                                                                                  02:50:06
		% y	年份，最后两个数字（00 - 99）                                                                   	01
		% Y	年份	                                                                                                          2012
		% Z	时区的名称或缩写	                                                                                      CDT
		% %一个 % 符号                                                                                                  %
	*/
	time_t rawtime4;
	struct tm* info4;
	char buffer4[80];
	time(&rawtime4);
	info4 = localtime(&rawtime4);
	strftime(buffer4, 80, "%Y-%m-%d %H:%M:%S", info4);
	printf("格式化的日期 & 时间 : |%s|\n", buffer4);

	//*************************计时结束***************************************
	runend();



	_getch();



	rundisplay();                                                                   //计时结果显示
	_getch();
	//closegraph();
	// system("pause");
	//Sleep(10000);
	return 0;
}
