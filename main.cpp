#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#include <wininet.h>
#include <wTypes.h>
#include <winsock.h>
#include <string.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
void htmtotxt(char *f1,char *f2)
{
        FILE *fin,*fout;
   char sc,*s1,*p1;
        int flag;

   s1=(char *)malloc(10000);
        p1=s1;
fin=fopen(f1,"r");
fout=fopen(f2,"w");
        flag=0;
        while(!feof(fin))
                {
            sc=fgetc(fin);

                        if(sc=='<')
                        {
                                s1=p1;
                                *s1++=sc;
                                while((sc=fgetc(fin))!='>'&&!feof(fin))
                                { *s1=sc; s1++;}
                                flag=1;
                        }
                        if(sc=='>')
                        {
                                s1=p1;
                                *s1++=sc;
                                while((sc=fgetc(fin))!='>'&&!feof(fin))
                                { *s1=sc; s1++;}
                                flag=1;
                        }
                        if(flag==1)
                        {
                                *s1++=sc;
                                *s1='\0';
                                if(fgetc(fin) == '\n')
                                {
                                        fputc('\n',fout);
                                        flag=0;
                                        continue;
                                }
                        }
                        flag=0;
                        fputc(sc,fout);
                }
fclose(fin);
fclose(fout);
remove("fx.dat");

}
void SendString(SOCKET sock,LPCSTR str)
{
    send(sock,str,strlen(str),0);
}
int fun(char d[]){
WSADATA  wsaData;
 SOCKET  sock;
 SOCKADDR_IN tcpaddr;
 hostent * remoteHost;
if(strcmp(d,"")==0||strlen(d)<8||strlen(d)>8){MessageBox(0,"请输入8位学号！","警告",MB_OK);}
 char   host[] = {"www.jiazhaowang.net"};
 int   Ret,l;
 BOOL  done;
 int   port = 80;
 int   chars = 0;
 char  buffer[512];

 if( (Ret = WSAStartup(MAKEWORD(2,2), &wsaData) ) != 0 )
 {
  MessageBox(0,"WSAStartup 初始化错误","警告",MB_OK);
  return 1;
 }

 if( (remoteHost = gethostbyname(host)) == NULL )
 {
  MessageBox(0,"网络错误","警告",MB_OK);
  return 1;
 }

 sock = socket (AF_INET, SOCK_STREAM, 0);
 tcpaddr.sin_family = AF_INET;
 tcpaddr.sin_port = htons( (unsigned short)port );
 tcpaddr.sin_addr.s_addr = *((unsigned long *)*remoteHost->h_addr_list);

 if( connect(sock, (const sockaddr * )&tcpaddr, sizeof(tcpaddr)) )
 {
   MessageBox(0,"网络错误","警告",MB_OK);
  return 1;
 }
char a[] = "GET /yc/getxsfs.asp?studentId=";
char *c = "&year=2013-2014&semester=1 HTTP/1.1\r\n";
       strcat(a,d);
         strcat(a,c);
 SendString(sock,a);
 SendString(sock,"Host:www.jiazhaowang.net\r\n");
 SendString(sock,"User-Agent: Dalvik/1.6.0 (Mincz; U; Android 7.0.4; gc-I9300 Build/IMM76D)");
 SendString(sock,"Connection:Keep-Alive\r\n");
 SendString(sock,"\r\n");
 SendString(sock,"\r\n");
done = FALSE;
 FILE * fp;
 fp = fopen("fx.dat","w+");
    while(!done)
    {
        l = recv(sock,buffer,1,0);
        if(l<0)
            done=TRUE;
  switch(*buffer)
        {
            case '\r':
                break;
            case '\n':
                if(chars==0)
                    done = TRUE;
                chars=0;
                break;
            default:
                chars++;
                break;
        }
    }
 int sum = 0;
 do
 {
  l = recv(sock,buffer,sizeof(buffer)-1,0);

if( l < 0 )
   break;
  sum += l;
  *(buffer + l) = 0;

fputs(buffer,fp);

 } while( l > 0 );
fclose(fp);
char *x="fx.dat";
char *y="data.dat";
htmtotxt(x,y);
 closesocket(sock);

 if( WSACleanup() == SOCKET_ERROR )
 {
MessageBox(0,"网络错误","警告",MB_OK);
 }
 return 0;
}
HINSTANCE hInst;
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
      case IDC_GY:
        MessageBox(hwndDlg, "此软件为兰州理工大学校外查分系统安卓版移植而来，\n安卓原版解释权归红楼梦团队，移植版归移植版作者所有。\n如有疑问请发送邮件至1617292992@qq.com","关于",MB_ICONINFORMATION);
        return true;
        case IDC_CX: char b[256]={0};GetDlgItemText(hwndDlg,IDC_XUEHAO,b,256);
        fun(b);
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
