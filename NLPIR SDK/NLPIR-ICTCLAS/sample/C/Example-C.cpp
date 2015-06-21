// win_cDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "../include/NLPIR.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef OS_LINUX
#ifdef NLPIR_STATIC
#pragma comment(lib, "../../../bin/ICTCLAS2013/NLPIRStatic.lib")
#else

#ifndef WIN64
#pragma comment(lib, "../../../bin/ICTCLAS2013/NLPIR.lib")
#else
#pragma comment(lib, "../../../bin/ICTCLAS2013/x64/NLPIR.lib")
#endif
#endif

#else
#include<pthread.h>
#endif


#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <time.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef OS_LINUX
#include <io.h>
#include <process.h>
#include <direct.h>
#include <assert.h>
#include <conio.h>
#define makeDirectory _mkdir
#pragma warning(disable:4786)
#define PATH_DELEMETER  "\\"
#define LONG64U __int64
#define DATE_DELEMETER  "/"
//#include <windows.h>
#define SLEEP(x) Sleep(x*1000)
#else
#include <dirent.h>
#include <ctype.h>
#include <unistd.h> 
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#define makeDirectory(x) mkdir(x, 0777)    // �ⲽ����룬�����ļ���ֻ��
//#define stricmp strcasecmp
//#define strnicmp strncasecmp
#define Sleep(x) sleep(x/1000)
#define min(a,b) (((a)<(b)) ? (a) : (b))
#define _stricmp(X,Y) strcasecmp((X),(Y))
#define stricmp(X,Y) strcasecmp((X),(Y))
#define strnicmp(X,Y,Z) strncasecmp((X),(Y),(Z))
#define _fstat(X,Y)     fstat((X),(Y))
#define _fileno(X)     fileno((X))
#define _stat           stat
#define _getcwd         getcwd
#define _off_t          off_t
#define PATH_DELEMETER  "/"
#define DATE_DELEMETER  "/"
#define LONG64U long long
#include <unistd.h>
#define SLEEP(x) sleep(x)
#endif

void SplitGBK(const char *sInput);
void SplitBIG5();
void SplitUTF8();
void testNewWord(int code);
long ReadFile(const char *sFilename,char **pBuffer);
void testRandomString()
{
	if (!NLPIR_Init("../../", UTF8_CODE)) 
	{ 
		return ; 
	} 
	unsigned char buf[15] = {0}; 
	srand((unsigned)time(NULL)); 
	for (int i = 0; i < 1000000; i++) 
	{ 
		memset(buf, '\0', 15); 
		printf("%d\n", sizeof(buf)); 
		for (int j = 0; j < sizeof(buf) - 2; j++) 
		{ 
			unsigned int nt = abs(rand()%254); 
			buf[j] = nt; 
		} 
		printf("content[%s], len[%d]\n", buf, strlen((const char*)buf)); 
		NLPIR_ParagraphProcess((char*)buf, 0); 
	} 

	NLPIR_Exit();
}
int testBug()
{
	if(NLPIR_Init("../../")) 
	{ 
		for(int j=0; j<3000; ++j) 
		{ 
			if (j==1963)
			{
				int nDebug=1;
			}
			std::string str = "Я��ת�����������鷶Χ��������������ͬ���ߡ�"; 
			int nCount = 0; 
			const result_t *pVecResult = NLPIR_ParagraphProcessA(str.c_str(), &nCount); 
			//����pVecResult��nCount����ؼ��ʵ� 
			for(int i=0; i<nCount; ++i) 
			{ 
				char szKey[16] = { 0 }; 
				memcpy(szKey, str.c_str() + pVecResult[i].start, (pVecResult[i].length < 16) ? pVecResult[i].length : 16 - 1); 
				//cout<<szKey<<endl; 
			} 

			printf("%d\r",j);
		} 
		NLPIR_Exit(); 
	} 
	else 
	{ 
		//cout<<"nlpir init failed."<<endl; 
		printf("nlpir init failed.\n");
	} 

	return 0; 
}
/*********************************************************************
 *
 *  Func Name  : Read(const char *sFilename,char *pBuffer)
 *  Description: 
 *        Read file to a buffer and return the file size
 *              
 *
 *  Parameters : 
 *               sFilename: filename;
 *               pBuffer: buffer
 *
 *  Returns    : _off_t file size
 *  Author     : Kevin Zhang  
 *  History    : 
 *              1.create 2003-11-28
 *********************************************************************/
long ReadFile(const char *sFilename,char **pBuffer)
{
	FILE *fp;
    struct _stat buf;
    if((fp=fopen(sFilename,"rb"))==NULL)
	   return 0;//read file false
    int nRet=_fstat( _fileno(fp), &buf );//Get file information
    if(nRet != 0)//File length of the lexicon file
	{
	   return 0;
	}
    *pBuffer = new char[buf.st_size+1];
    //The buffer for file
    if(*pBuffer == 0)
	{
	   return 0;
	}
    fread(*pBuffer, buf.st_size, 1, fp);//Read 
	(*pBuffer)[buf.st_size]=0;
    fclose(fp);
	return buf.st_size;
}


void testStrSplitSpeed()
{
	//��ʼ���ִ����
	printf("���յ�ƪ���½��д���\n");
	if(!NLPIR_Init("../../",GBK_CODE))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	printf("Success Init!\n");
	char sInput[1000] = "�ȵǻ���һȰҵ���С�����ʱ�����������ڡ���Խ�ϡ������ձ�����ý��2��18�ձ���";


	clock_t  lStart,lEnd;
	long lTime=0;
	int nLine=0;


	int nSize=strlen(sInput),nTotalSize=0;
	int i=1;
	double fTimeTotal=0.0,fTime=0.0;//Time cost
	double fSpeed,fSpeendMin=100000000.0;
	lStart=clock();//Record the time
	for (int i=0;i<10000;i++)
	{
		const char *pResult=NLPIR_ParagraphProcess(sInput);
	}

	lEnd=clock();//Record the time
	lTime=lEnd-lStart;
	fTime=(double)lTime/(double)CLOCKS_PER_SEC;//Time cost
	fSpeed=(double)nSize*10000/(double)fTime;
		
	printf("speed=%.2lfBytes/sec,length=%ld,time=%.6f\n",fSpeed,nSize*10000,fTime);

	NLPIR_Exit();
}
void testBatchSpeed(const char *sListFilename)
{
	//��ʼ���ִ����
	printf("���յ�ƪ���½��д���\n");
	if(!NLPIR_Init("../../",UTF8_CODE))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	printf("Success Init!\n");

	FILE *fpListFile=fopen(sListFilename,"rb");
	char sInputFile[1025]="../test/test.TXT",sResultFile[1024];

	if (fpListFile==NULL)
	{
		printf("Error open %s\n",sListFilename);
		return ;
	}
	clock_t  lStart,lEnd;
	long lTime=0;
	int nLine=0;

	
	int nSize=0,nTotalSize=0;
	int i=1;
	double fTimeTotal=0.0,fTime=0.0;//Time cost
	double fSpeed,fSpeendMin=100000000.0;

	while(fgets(sInputFile,1024,fpListFile))
	{
		nLine=strlen(sInputFile);
		while(nLine>0&&(sInputFile[nLine-1]=='\r'||sInputFile[nLine-1]=='\n'))
		{
			nLine--;
		}
		sInputFile[nLine]=0;
		//sscanf(sResultFile,sInputFile)
		char *pText=0;
		nSize=ReadFile(sInputFile,&pText);
		if (nSize<=0)
		{
			printf("Error open %s\n",sInputFile);
			continue ;
		}
		nTotalSize+=nSize;
		lStart=clock();//Record the time
		int nCount=NLPIR_GetParagraphProcessAWordCount(pText);

		if (nCount>0)
		{
			result_t *pResult=new result_t[nCount];
			NLPIR_ParagraphProcessAW(nCount,pResult);
			
			//for (int i=0;i<nCount;i++)
			{
				//printf("\nstart=%d length=%d\n",pResult[i].start,pResult[i].length);
			}
			delete [] pResult;
		}
		lEnd=clock();//Record the time
		lTime=lEnd-lStart;
		fTime=(double)lTime/(double)CLOCKS_PER_SEC;//Time cost
		fTimeTotal+=lTime;
		fSpeed=(double)nSize/(double)fTime;
		if (nSize>0&&fSpeed<fSpeendMin)
		{
			fSpeendMin=fSpeed;
			strcpy(sResultFile,sInputFile);
		}
		printf("%d\r",i++);
	}
	
	fclose(fpListFile);
	fTimeTotal=fTimeTotal/CLOCKS_PER_SEC;
	fSpeed=(double)nTotalSize/fTimeTotal;
	printf("speed=%.2lfBytes/sec,length=%ld,time=%.6f\n",fSpeed,nTotalSize,fTimeTotal);
	fSpeed=(double)i/(double)fTimeTotal;
	printf("speed=%.2lfdocs/sec,docs=%ld,time=%.6f\n",fSpeed,i,fTimeTotal);

	printf("MinSpeed=%.2lfBytes/sec,file=%s\n",fSpeendMin,sResultFile);
	NLPIR_Exit();
}
void testSpeed(int nCode)
{
	//��ʼ���ִ����
	printf("���յ�ƪ���½��д���\n");
	if(!NLPIR_Init("../../",nCode))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	printf("Success Init!\n");
	char sInputFile[1024]="../test/test.TXT",sResultFile[1024];
	if (nCode==UTF8_CODE)
	{
		strcpy(sInputFile,"../test/test-utf8.TXT");
	}
	strcpy(sResultFile,sInputFile);
	strcat(sResultFile,"_result.txt");
	clock_t  lStart,lEnd;
	long lTime=0;
	int nLine=0;
	FILE *fpResult=fopen(sResultFile,"wt");
	if (fpResult==NULL)
	{
		printf("Error open %s\n",sResultFile);
	}
	lStart=clock();//Record the time
	char *pText=0;
	int nSize=ReadFile(sInputFile,&pText);
	if (nSize<=0)
	{
		printf("Error open %s\n",sInputFile);
		return ;
	}
	int nCount=NLPIR_GetParagraphProcessAWordCount(pText);
	
	if (nCount>0)
	{
			result_t *pResult=new result_t[nCount];
			NLPIR_ParagraphProcessAW(nCount,pResult);
			lEnd=clock();//Record the time
			for (int i=0;i<nCount;i++)
			{
				fprintf(fpResult,"\"\nstart=%d length=%d word=\"",pResult[i].start,pResult[i].length);
				fwrite(pText+pResult[i].start,sizeof(char),pResult[i].length,fpResult);
			}
			fclose(fpResult);
			delete [] pResult;
	}
	lTime=lEnd-lStart;

	double fTime=(double)lTime/(double)CLOCKS_PER_SEC;//Time cost
	double fSpeed=(double)nSize/(double)fTime;
	printf("speed=%.2lfBytes/s,length=%ld,time=%.2f\n",fSpeed,nSize,fTime);


	strcat(sResultFile,"_file");
	lStart=clock();//Record the time
	NLPIR_FileProcess(sInputFile,sResultFile,0);
	lEnd=clock();//Record the time
	lTime=lEnd-lStart;
	fTime=(double)lTime/(double)CLOCKS_PER_SEC;//Time cost
	fSpeed=(double)nSize/(double)fTime;

	printf("NLPIR_FileProcess speed=%.2lfBytes/s,length=%ld,time=%.2f\n",fSpeed,nSize,fTime);

	NLPIR_Exit();
}

void testNewWord(int nCode)
{
	//��ʼ���ִ����
	if(!NLPIR_Init("../../",nCode))//��������һ��Ŀ¼�£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	char sInputFile[1024]="../test/test.TXT",sResultFile[1024];
	if (nCode==UTF8_CODE)
	{
		strcpy(sInputFile,"../test/test-utf8.TXT");
	}

	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ���Թؼ�����ȡ����
	//////////////////////////////////////////////////////////////////////////
	const char *sResult=NLPIR_GetFileKeyWords(sInputFile,50,true);//���ı��ļ�����ȡ�ؼ���
	FILE *fp=fopen("Result.txt","wb");
	fprintf(fp,sResult);//�����д���ļ�
	fclose(fp);
	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ���Դӵ����ļ�����ȡ�´ʵĹ���
	//////////////////////////////////////////////////////////////////////////
	sResult=NLPIR_GetFileNewWords(sInputFile);//���ı��ļ�����ȡ�´�
	fp=fopen("ResultNew.txt","wb");//�����д���ļ�
	fprintf(fp,sResult);
	fclose(fp);



	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ���Դӵ����ļ�����ȡ�´ʵĹ���
	//////////////////////////////////////////////////////////////////////////
	NLPIR_NWI_Start();//�´�ʶ��ʼ
	NLPIR_NWI_AddFile(sInputFile);//�������������ļ������Բ���ѭ������NLPIR_NWI_AddFile����NLPIR_NWI_AddMem
	NLPIR_NWI_Complete();//�´�ʶ�����ļ�����
	const char *pNewWordlist=NLPIR_NWI_GetResult();//��ȡ�������������ı��ļ���ʶ����´ʽ��
	printf("ʶ������´�Ϊ��%s\n",pNewWordlist);//��ӡ����´�ʶ����
	
	strcpy(sResultFile,sInputFile);
	strcat(sResultFile,"_result1.txt");
	clock_t  lStart,lEnd;
	long lTime=0;
	int nLine=0;

	lStart=clock();//Record the time
	double speed=NLPIR_FileProcess(sInputFile,sResultFile,0);//��ͨ���ı��ļ��ִʹ���
	lEnd=clock();//Record the time
	lTime+=lEnd-lStart;

	float fTime=(float)lTime/(float)CLOCKS_PER_SEC;//Time cost
	printf("speed=%.2lfKB/s,time=%.2f\n",speed,fTime);
	
	NLPIR_NWI_Result2UserDict();//���ϴ��´�ʶ��Ľ����Ϊ�û��ʵ䵼��ϵͳ�У������ķִʽ��Ϊ����Ӧ�ִʽ��

	strcpy(sResultFile,sInputFile);
	strcat(sResultFile,"_result2.txt");
	NLPIR_FileProcess(sInputFile,sResultFile);//�ٴε���ͬ���ĺ��������Ƿִʽ�����Զ���Ӧ�´ʽ��
	
	NLPIR_Exit();//ʶ����ɣ�ϵͳ�˳����ͷ���Դ
}

void SplitEnglish(const char *sInput)
{//�ִ���ʾ

	//��ʼ���ִ����
	if(!NLPIR_Init("../../"))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	//���ļ����зִ�

	const char *sResult = ICTCLAS_ParagraphProcess(sInput, 1); 
	printf("%s\n", sResult); 

	int nCount1=ICTCLAS_ImportUserDict("userdictgbk.txt");
	sResult = ICTCLAS_ParagraphProcess(sInput, 1); 
	printf("%s\n", sResult); 
	sResult = NLPIR_GetKeyWords(sInput,100,true); 
	printf("%s\n", sResult); 
	NLPIR_Exit();
	return ;
//	NLPIR_ReleaseBuf(sResult);
	NLPIR_FileProcess("D:\\NLPIR\\test\\english\\sample3.txt","D:\\NLPIR\\test\\english\\sample3_result.txt",1);
	NLPIR_FileProcess("D:\\NLPIR\\test\\english\\sample4.txt","D:\\NLPIR\\test\\english\\sample4_result.txt",1);
	NLPIR_FileProcess("D:\\NLPIR\\test\\english\\input_test.txt","D:\\NLPIR\\test\\english\\input_test_result.txt",1);
	NLPIR_FileProcess("D:\\NLPIR\\test\\english\\sample1.txt","D:\\NLPIR\\test\\english\\sample1_result.txt",1);
	NLPIR_FileProcess("D:\\NLPIR\\test\\english\\sample2.txt","D:\\NLPIR\\test\\english\\sample2_result.txt",1);
	NLPIR_FileProcess("D:\\NLPIR\\test\\english\\sample5.txt","D:\\NLPIR\\test\\english\\sample5_result.txt",1);
	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ����NLPIR_ParagraphProcessA����
	//////////////////////////////////////////////////////////////////////////
	int nCount;
	const result_t *pResult=NLPIR_ParagraphProcessA(sInput,&nCount);
	//�ִʵĸ߼�����
	//�ú�������Ϊ�������ַ���������ִʽ����result_t�ṹ�����飬nCoutΪ�����С
	//���ص���������ϵͳ����ά�����û�ֱ�ӵ��ü���

	printf("nCount=%d\n",nCount);//�ִʽ����Ŀ

	int i=1;
	char *sWhichDic;
	char sWord[100];
	for(i=0;i<nCount;i++)
	{//�����Ӧ�ִʽ��������
		strncpy(sWord,sInput+pResult[i].start,pResult[i].length);
		sWord[pResult[i].length]=0;
		printf("No.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d, UserDefine:%d, Word:%s\n",
			i+1, pResult[i].start, pResult[i].length, pResult[i].iPOS, pResult[i].word_ID, pResult[i].word_type,
			sWord);
	}


	//�ͷŷִ������Դ
	NLPIR_Exit();
}
void SplitGBK(const char *sInput)
{//�ִ���ʾ

	//��ʼ���ִ����
	if(!NLPIR_Init("d:/nlpir"))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	printf("ICTCLAS INIT Success!\n");
	FILE *fp=fopen("result.txt","wt");
	const char *pKey=NLPIR_GetFileKeyWords("D:\\BIT\\��Ŀ\\2015\\anq\\���鸦\\content\\content.txt",200,true);
	fprintf(fp,"Key is:\n%s\n",pKey);
	printf("NLPIR_GetFileKeyWords Success!\n");
	fflush(fp);

	pKey=NLPIR_FileWordFreqStat("D:\\BIT\\��Ŀ\\2015\\anq\\���鸦\\content\\content.txt");
	fprintf(fp,"Freq is:%s\n",pKey);
	printf("NLPIR_FileWordFreqStat Success!\n");
	fflush(fp);
	fclose(fp);


	/*
	char sWord[100]="����";
	const char* pResult,*pFinerResult;
	int bExit=NLPIR_IsWord(sWord);
	pResult=NLPIR_GetWordPOS(sWord);
	pFinerResult=NLPIR_FinerSegment(sWord);
	printf("sWord=%s,NLPIR_IsWord=%d,NLPIR_GetWordPOS=%s,NLPIR_FinerSegment=%s\n",sWord,bExit,pResult,pFinerResult);
	
	strcpy(sWord,"������");
	bExit=NLPIR_IsWord(sWord);
	pResult=NLPIR_GetWordPOS(sWord);
	pFinerResult=NLPIR_FinerSegment(sWord);
	printf("sWord=%s,NLPIR_IsWord=%d,NLPIR_GetWordPOS=%s,NLPIR_FinerSegment=%s\n",sWord,bExit,pResult,pFinerResult);


	strcpy(sWord,"�л����񹲺͹�");
	bExit=NLPIR_IsWord(sWord);
	pResult=NLPIR_GetWordPOS(sWord);
	pFinerResult=NLPIR_FinerSegment(sWord);
	printf("sWord=%s,NLPIR_IsWord=%d,NLPIR_GetWordPOS=%s,NLPIR_FinerSegment=%s\n",sWord,bExit,pResult,pFinerResult);
	
	strcpy(sWord,"�л����񹲺͹�1949�������");
	pFinerResult=NLPIR_ParagraphProcess(sWord);
	printf("%s",pFinerResult);

	//NLPIR_Exit();
	//return ;
	*/
	//char sSentence[5000]="�𾴵�ȫ��ͨ�ͻ������ã���ӭ��¼�����ƶ�����Ӫҵ���� ��ֱ�ӻظ���ţ�\r\n0.�ѿ�ͨҵ���ѯ\n1.���������\n2.�����Ż�\n3.����ҵ��\n4.������ѯ�����\n5.��ͥ�ͼ���ҵ��\n6.ȫ��ͨ�����ײ�\n7.�ͻ����������\n���ͨ����DHLL�����ֶһ���������3305����ͨ5Ԫ��30MB�������Ӱ���������Ч����111���黰�ѣ���192������������113�����ײͣ����й��ƶ�";//"Bill Clinton��������ͳ������û������������ѧ��ϲ����Сβ��";
	char sSentence[5000]="113�����ײͣ����й��ƶ�";//"Bill Clinton��������ͳ������û������������ѧ��ϲ����Сβ��";
	strcpy(sSentence,"Waitingforyousolongtime!600056!\n");
	const char * sResult;
	//ICTCLAS_AddUserWord("���� ns");
	sResult=ICTCLAS_ParagraphProcess(sSentence, 1); 
	printf("%s",sResult);
	sResult=NLPIR_FinerSegment(sSentence); 
	printf("%s",sResult);


	sResult = NLPIR_FileWordFreqStat("testGBK.txt");
	printf("%s\n", sResult);

	sResult = NLPIR_GetEngWordOrign("driving");
	printf("%s\n", sResult);
	sResult = NLPIR_GetEngWordOrign("drove");
	printf("%s\n", sResult);
	sResult = NLPIR_GetEngWordOrign("drives");
	printf("%s\n", sResult);
	sResult = NLPIR_GetEngWordOrign("driven");
	printf("%s\n", sResult);
/*
	NLPIR_Exit();
	return ;

	

	NLPIR_SetPOSmap(ICT_POS_MAP_SECOND);//���ô��Ա�ע���ϵ����ͣ�Ĭ��Ϊ������������ע��ϵ
	NLPIR_FileProcess("D:/NLPIR/feedback/1.txt","D:/NLPIR/feedback/1-result.txt");
	const char* sample1 ="��Ϥ���ʼ��ܾ��ѽ������й�����ٴ�ͨ��������Ҫ��������ǿ���仪���׵Ĳ�����Դ�����估�ִ��Ȼ��ڵĹܿش�ʩ����Ч�����仪���ױ�δ���ҹ�ũҵ����ȫ��������׼��ת����Ʒϵ��Ⱦ��";// "���ĳɳ��ķ���ѧϰ�����й�֤ȯ�г�";
	const char* sResult1 = ICTCLAS_ParagraphProcess(""); 
	printf("%s\n", sResult1); 
	for (int i=0;i<100;i++)
	{
		sResult1 = ICTCLAS_ParagraphProcess(sample1); 
		printf("%d,%s\n", i,sResult1); 
	}

	printf("%d\n", ICTCLAS_AddUserWord("Ҫ��������ǿ���� n"));
	printf("%d\n", ICTCLAS_AddUserWord("�����׵Ĳ�����Դ  n"));



	sResult1 = ICTCLAS_ParagraphProcess(sample1, 1); 
	 printf("%s\n", sResult1); 
	 printf("%d\n", ICTCLAS_DelUsrWord("Ҫ��������ǿ����"));
	 sResult1 = ICTCLAS_ParagraphProcess(sample1, 1); 
	 printf("%s\n", sResult1); 
	 printf("%d\n", ICTCLAS_AddUserWord("Ҫ��������ǿ���� n"));
	 sResult1 = ICTCLAS_ParagraphProcess(sample1, 1); 
	 printf("%s\n", sResult1); 
	 
	 sResult1 = NLPIR_GetFileKeyWords("D:\\NLPIR\\feedback\\keyword2.txt",50,false); 
	 printf("%s\n", sResult1); 
	
	 char *pText=0;
	 ReadFile("D:\\NLPIR\\feedback\\keyword2.txt",&pText);
	 sResult1 = NLPIR_GetKeyWords(pText,50,false); 
	 printf("%s\n", sResult1); 
	 delete [] pText;

	 sResult1 = NLPIR_GetKeyWords(sample1,10,false); 
	 printf("%s\n", sResult1); 


	 const char* sample2 = "������Ƶ��������";
	 const char* sResult2 = ICTCLAS_ParagraphProcess(sample2);
	 printf("%s\n", sResult2); 
	 printf("%d\n", ICTCLAS_AddUserWord("���� ag")); 
	 sResult2 = ICTCLAS_ParagraphProcess(sample2, 1); 
	 printf("%s\n", sResult2);



	 */
	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ����NLPIR_ParagraphProcessA����
	//////////////////////////////////////////////////////////////////////////
	int nCount;
	//strcpy(sSentence,"�ų���ָ�����й�֤ȯ�г����µ��������ԭ�����羭�÷�չ����ĵ����ͷ���ͨ�ɵ��������⣬�Լ��г������ı仯�ȡ�");
//	strcpy(sSentence,"drivingmotor The fuel for which gas and oil are used in general is determined at the same time as determination of the driving motor , taking into account the supplying condition, environmental properties, cost, etc.");
	strcpy(sSentence,"driving  motor , Bill  Clinton taking into account the supplying condition, environmental properties, cost, etc.");

	

	const result_t *pResult=NLPIR_ParagraphProcessA(sSentence,&nCount);
	//�ִʵĸ߼�����
	//�ú�������Ϊ�������ַ���������ִʽ����result_t�ṹ�����飬nCoutΪ�����С
	//���ص���������ϵͳ����ά�����û�ֱ�ӵ��ü���
	printf("nCount=%d\n",nCount);//�ִʽ����Ŀ

	int i=1;
	char *sWhichDic;
	char sWord[100];
	for(i=0;i<nCount;i++)
	{//�����Ӧ�ִʽ��������
		sWhichDic="";
		switch (pResult[i].word_type)
		{
		case 0:
			sWhichDic = "���Ĵʵ�";
			break;
		case 1:
			sWhichDic = "�û��ʵ�";
			break;
		case 2:
			sWhichDic = "רҵ�ʵ�";
			break;
		default:
			break;
		}
		strncpy(sWord,sSentence+pResult[i].start,pResult[i].length);
		sWord[pResult[i].length]=0;
		printf("No.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d, UserDefine:%s, Word:%s,Weight:%d\n",
			i+1, pResult[i].start, pResult[i].length, pResult[i].iPOS, pResult[i].word_ID, sWhichDic,
			sWord,pResult[i].weight );
	}
	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ�����û��ʵ书��
	//////////////////////////////////////////////////////////////////////////
	//�����û��ʵ�ǰ
	
	printf("δ�����û��ʵ䣺\n");
	sResult = NLPIR_ParagraphProcess(sSentence, 1);
	printf("%s\n", sResult);

	//�����û��ʵ��
	printf("\n�����û��ʵ��\n");
	nCount = NLPIR_ImportUserDict("userdictgbk.txt");//userdic.txt������ǰ���û��ʵ�
	//�����û��ʵ�
	printf("����%d���û��ʡ�\n", nCount);
	sResult = NLPIR_ParagraphProcess(sSentence, 1);
	printf("%s\n", sResult);
	NLPIR_Exit();
	return;
	//����ڶ����û��ʵ��
	printf("\n�����û��ʵ��\n");
	nCount = NLPIR_ImportUserDict("userdictgbk.txt",false);//userdictgbk.txt������ǰ���û��ʵ�
	//�����û��ʵ�
	printf("����%d���û��ʡ�\n", nCount);
	sResult = NLPIR_ParagraphProcess(sSentence, 1);
	printf("%s\n", sResult);

	strcpy(sSentence,"������Ƶ��ݸ��������");
	sResult = NLPIR_ParagraphProcess(sSentence, 1);
	printf("%s\n", sResult);

	//��̬����û���
	printf("\n��̬����û��ʺ�\n");
	NLPIR_AddUserWord("����   ag");
	sResult = NLPIR_ParagraphProcess(sSentence, 1);
	printf("%s\n", sResult);


	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ���Զ��ļ����зִʹ���
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//���濪ʼ���Ի����ĳ��ù���NLPIR_ParagraphProcess����
	//////////////////////////////////////////////////////////////////////////
	while(_stricmp(sSentence,"q")!=0)
	{
		sResult = NLPIR_ParagraphProcess(sSentence);
		//����������Ĺ��ܣ�����һ���ַ���������ִʽ���ַ�����
		//�������Ҫ��ע�����õ�ʱ�����Ϊ�� NLPIR_ParagraphProcess(sSentence,0)
		printf("%s\nInput string now('q' to quit)!\n", sResult);// 
		gets(sSentence);
	}
	//���ļ����зִ�
	NLPIR_FileProcess("test2.txt","test2_result.txt",1);
	NLPIR_FileProcess("testGBK.txt","testGBK_result.txt",1);

	sResult=NLPIR_GetFileNewWords("D:\\NLPIR\\test\\��˿��һ����ͷ�ĵ���.TXT");
	//�ͷŷִ������Դ
	NLPIR_Exit();
}

void SplitGBK_Fanti(const char *sInput)
{//�ִ���ʾ

	//��ʼ���ִ����
	if(!NLPIR_Init("",GBK_FANTI_CODE))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}

	NLPIR_SetPOSmap(ICT_POS_MAP_SECOND);

	char sSentence[5000]="ICTCLAS�ڹ���ר������֯�������л����˵�һ�����ڵ�һ��������Ĵ����о�����SigHan��֯�������ж�����˶����һ����ICTCLAS�ڇ��Ȍ��ҽM�M�����u�y�л�ӫ@���˵�һ�����ڵ�һ�Ç��H����̎���о��C��SigHan�M�����u�y�ж��@���˶�헵�һ����";
	const char * sResult;


	int nCount;
	NLPIR_ParagraphProcessA(sSentence,&nCount);
	printf("nCount=%d\n",nCount);
	int count = NLPIR_GetParagraphProcessAWordCount(sSentence);
	const result_t *pResult=NLPIR_ParagraphProcessA(sSentence,&nCount);

	int i=1;
	char *sWhichDic;
	for(i=0;i<nCount;i++)
	{
		sWhichDic="";
		switch (pResult[i].word_type)
		{
		case 0:
			sWhichDic = "���Ĵʵ�";
			break;
		case 1:
			sWhichDic = "�û��ʵ�";
			break;
		case 2:
			sWhichDic = "רҵ�ʵ�";
			break;
		default:
			break;
		}
		printf("No.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d, UserDefine:%s, Word:%s\n",
			i+1, pResult[i].start, pResult[i].length, pResult[i].iPOS, pResult[i].word_ID, sWhichDic,sSentence+pResult[i].start );
	}
	while(_stricmp(sSentence,"q")!=0)
	{
		sResult = NLPIR_ParagraphProcess(sSentence,0);
		printf("%s\nInput string now('q' to quit)!\n", sResult);// 
		scanf("%s",sSentence);
	}
	
	//�����û��ʵ�ǰ
	printf("δ�����û��ʵ䣺\n");
	sResult = NLPIR_ParagraphProcess(sInput, 0);
	printf("%s\n", sResult);

	//�����û��ʵ��
	printf("\n�����û��ʵ��\n");
	nCount = NLPIR_ImportUserDict("userdic.txt");//userdic.txt������ǰ���û��ʵ�
	//�����û��ʵ�
	NLPIR_SaveTheUsrDic();
	printf("����%d���û��ʡ�\n", nCount);
	
	sResult = NLPIR_ParagraphProcess(sInput, 1);
	printf("%s\n", sResult);

	//��̬����û���
	printf("\n��̬����û��ʺ�\n");
	NLPIR_AddUserWord("�����ѧԺ   xueyuan");
	NLPIR_SaveTheUsrDic();
	sResult = NLPIR_ParagraphProcess(sInput, 1);
	printf("%s\n", sResult);


	//���ļ����зִ�
	NLPIR_FileProcess("test2.txt","test2_result.txt",1);
	NLPIR_FileProcess("testGBK.txt","testGBK_result.txt",1);


	//�ͷŷִ������Դ
	NLPIR_Exit();
}
void SplitBIG5()
{
	//��ʼ���ִ����
	if(!NLPIR_Init("../../",BIG5_CODE))//�����ڵ�ǰ·���£�����ΪBIG5����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	NLPIR_FileProcess("testBIG.txt","testBIG_result.txt");
	NLPIR_Exit();
}

void SplitFantiUTF8()
{
	//��ʼ���ִ����
	if(!NLPIR_Init("../../",UTF8_FANTI_CODE))//�����ڵ�ǰ·���£�����ΪBIG5����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	NLPIR_FileProcess("testFantiUTF.txt","testFantiUTF_result.txt");
	NLPIR_Exit();
}
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
void SplitUTF8()
{
	/*if(!NLPIR_Init("D:/NLPIR",UTF8_CODE))//�����ڵ�ǰ·���£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	NLPIR_FileProcess("testUTF.txt","testUTF_result.txt");
	NLPIR_FileProcess("luan.txt","luan_result.txt");
	NLPIR_FileProcess("ting.txt","ting_result.txt");
	//NLPIR_
	NLPIR_Exit();
	return;
	int nnCount=0;
	ifstream fin("weibo2.csv");
	string strInput;

	while(getline(fin,strInput))
	{
		nnCount++;
		const char *sResult = NLPIR_ParagraphProcess(strInput.c_str());
		//if (nnCount%1000==0)
		{
			printf("nCount=%d\r",nnCount);
		}
	}
	NLPIR_Exit();
	return ;*/
	/*
	FILE *fpSource,*fpResult;//The file pointer of read and write
	char sLine[4097];

	fpSource=fopen("D:/NLPIR/feedback/weibo1.txt","rb");
	fpResult=fopen("D:/NLPIR/feedback/weibo1_result.txt","wb");
	
	//m_bEnglishText=IsEnglishTextFile(sSrcFilename);
	clock_t  lStart,lEnd;
	long lTime=0;
	int nLine=0;
	const char *pResult;
	;
	while (fgets(sLine,4096,fpSource))
	{
		fputs(sLine,fpResult);
		//fwrite(sLine,strlen(sLine),1,fpResult);
		nLine+=1;
#ifdef _DEBUG
		printf("%d\r",nLine);
#endif
	}
	//printf("Size=%ldBytes,Time=%6fs,Speed=%6fKB/s\n",buf.st_size,fTime,fSpeed/1000);
	fclose(fpSource);
	fclose(fpResult);
	*/
	//��ʼ���ִ����
	if(!NLPIR_Init("D:/NLPIR",UTF8_CODE))//�����ڵ�ǰ·���£�����ΪUTF8����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	//NLPIR_FileProcess("D:\\NLPIR\\feedback\\huawei\\microBlog.txt","D:\\NLPIR\\feedback\\huawei\\microBlogLicense.txt");
	NLPIR_FileProcess("D:\\NLPIR\\test\\1.txt","D:\\NLPIR\\test\\1-result.res");
	const char *sResult=NLPIR_GetFileNewWords("D:\\NLPIR\\test\\testNewwordUTF8.TXT");
	//�ͷŷִ������Դ

	NLPIR_Exit();
	return ;


	/**
	std::cout << NLPIR_ParagraphProcess("^^", 0) << std::endl; 
	std::cout << NLPIR_ParagraphProcess("^^a", 0) <<std::endl; 
	std::cout << NLPIR_ParagraphProcess("b^^a", 0) <<std::endl; 
	std::cout << NLPIR_ParagraphProcess("b^^^a", 0) <<std::endl; 
	std::cout << NLPIR_ParagraphProcess("bcd^^^a", 0) <<std::endl; 

	std::cout << NLPIR_ParagraphProcess("A B C D", 0) << std::endl;
	std::cout << NLPIR_ParagraphProcess("def", 0) << std::endl;
	std::cout << NLPIR_ParagraphProcess("A B C D", 0) << std::endl;
	std::cout << NLPIR_ParagraphProcess("def", 0) << std::endl;
	std::cout << NLPIR_ParagraphProcess("   ", 0) << std::endl;
	std::cout << NLPIR_ParagraphProcess("ghi", 0) << std::endl;
	std::cout << NLPIR_ParagraphProcess("a1 b1 c1", 0) << std::endl;
	//NLPIR_FileProcess("D:/NLPIR/feedback/weibo2.txt","D:/NLPIR/feedback/weibo2.csv.txt");
	printf("Start now!\n");
��*/
	int nCount=NLPIR_ImportUserDict("dict.txt");

	NLPIR_FileProcess("testUtf.txt","testUtf8-result.txt");

	printf("Start now!\n");
	//const char *sResult=NLPIR_GetFileKeyWords("D:\\NLPIR\\feedback\\huawei\\5341\\5341\\��Ůְ���Ͷ������ر�涨���ؿ�\\2012\\6\\18951.txt");
	//const char *sResultNew=NLPIR_GetFileNewWords("D:\\NLPIR\\feedback\\huawei\\5341\\5341\\��Ůְ���Ͷ������ر�涨���ؿ�\\2012\\6\\18951.txt");
	FILE *fpResult=fopen("Result.txt","wb");
	//const char *sResult;	
	sResult=NLPIR_GetFileKeyWords("testUtf.txt",1000,true);
	fprintf(fpResult,"Key Result is\n%s\n",sResult);
	nCount=NLPIR_ImportKeyBlackList("keyblack.txt");
	sResult=NLPIR_GetFileKeyWords("testUtf.txt",1000,true);
	fprintf(fpResult,"Key Result ��after blacklist imported�� is\n%s\n",sResult);
	//const char *sResultNew=NLPIR_GetFileNewWords("weibo.txt",1000,true);
	//fprintf(fpResult,"New Result is\n%s\n",sResultNew);
	fclose(fpResult);

	//NLPIR_FileProcess("D:/NLPIR/feedback/weibo1.txt","D:/NLPIR/feedback/weibo1-result.txt");
	//NLPIR_FileProcess("D:/NLPIR/feedback/weibo1.txt","D:/NLPIR/feedback/weibo1-result.txt");
	NLPIR_FileProcess("D:/NLPIR/feedback/input-utf8.txt","D:/NLPIR/feedback/input-utf8-result.txt");
/*	
	const char *pMessage=NLPIR_GetLastErrorMsg();
	char *pText=0;
    int nSize=ReadFile("testUTF.txt",&pText);
 	const char *pKeyResult=NLPIR_GetKeyWords(pText);
	const char *pNewResult=NLPIR_GetNewWords(pText);
	const char *pResult=NLPIR_ParagraphProcess(pText,1);
	delete [] pText;
	//NLPIR_Exit();
	//return ;
	FILE *fpU=fopen("testUTF_result2.txt","wt");
	if (fpU==NULL)
	{
		printf("Error Open TestUTF8-bigfile.txt\n");
		NLPIR_Exit();
		return ;
	}
	fprintf(fpU,"%s",pResult);
	fclose(fpU);
//	NLPIR_ReleaseBuf(pResult);
	//return;

	FILE *fp=fopen("D:\\NLPIR\\test\\utf-8-offset.txt","rb");
	if (fp==NULL)
	{
		printf("Error Open TestUTF8-bigfile.txt\n");
		NLPIR_Exit();
		return ;
	}
	FILE *fpResult=fopen("utf-8-offset-result.txt","wt");
	char sLine[10241];
	int i,nCount,nDocID=1;
	result_t res[1000];
	//while (fgets(sLine,10240,fp))
	{
		CICTCLAS *pICTCLAS=new CICTCLAS;
		fseek(fp,0,SEEK_END);
		int nSize=ftell(fp);

		fseek(fp,0,SEEK_SET);
		fread(sLine,nSize,1,fp);
		sLine[nSize]=0;
		const result_t *pResult=pICTCLAS->ParagraphProcessA(sLine,&nCount);
	    i=1;

		for(i=0;i<500&&i<nCount;i++)
		{
 			fprintf(fpResult,"\nNo.%d:start:%d, length:%d,POS_ID:%d,Word_ID:%d\n",
 				i+1, pResult[i].start, pResult[i].length, pResult[i].iPOS, pResult[i].word_ID);
			fwrite(sLine+pResult[i].start,sizeof(char),pResult[i].length,fpResult);
		}
		delete pICTCLAS;
		fclose(fpResult);
		printf("Processed docs %d\r",nDocID++);
	}
	*/
	NLPIR_Exit();
}
void ImportUserDict(const char *sFilename)
{
	//��ʼ���ִ����
	if(!NLPIR_Init("../../"))//��������һ��Ŀ¼�£�Ĭ��ΪGBK����ķִ�
	{
		printf("ICTCLAS INIT FAILED!\n");
		return ;
	}
	int nCount=NLPIR_ImportUserDict(sFilename);//�ٴε���ͬ���ĺ��������Ƿִʽ�����Զ���Ӧ�´ʽ��
	printf("Import %d user defined items from file %s\n",nCount,sFilename);
	NLPIR_Exit();//ʶ����ɣ�ϵͳ�˳����ͷ���Դ
}
void testLinuxMac()
{

	int	m_length=0;
	char sMacAddr[255][13];
	char sTemp[13];	
	int i,j,nMacCount=0;


	char *pText=0,*pCur,*pStart;//,sChar[4]
	int nLen=ReadFile("temp.mac",&pText);
	//sChar[3]=0;
	i=0;
	if (nLen>0)
	{//HWaddr 00:30:48:24:21:AB
		//sChar[3]=0;
		i=0;
		if (nLen>0)
		{//HWaddr 00:30:48:24:21:AB
			pStart=strstr(pText,":");
			while (pStart!=0&&pStart-pText>3&&pStart+3-pText<nLen)
			{
				while (pStart!=0&&pStart-pText>3&&pStart+12-pText<nLen&&
						(*(pStart+12)!=':'||*(pStart+3)!=':'||*(pStart+6)!=':'||*(pStart+9)!=':')
					   )
				{//Lookup the start position:00:0C:29:BF:F0:85
					pStart=strstr(pStart+3,":");
				}
				if (pStart!=0&&pStart-pText>3&&pStart+3-pText<nLen&&*(pStart+3)==':')
				{//Locate a mac address
					pCur=pStart-2;
					//printf("pCur=%s\n",pCur);
					for (j=0;j<6;j++)
					{
						sMacAddr[i][j*2]=pCur[j*3];
						sMacAddr[i][j*2+1]=pCur[j*3+1];
					}
					sMacAddr[i][12]=0;
					i++;
				}
				if (pStart!=0)
				{
					pStart=strstr(pCur+17,":");
				}
			}	
		}
	}


}
int main(int argc,char *argv[])
{
//	testLinuxMac();
//	testRandomString();
//	return 1;
	//SplitFantiUTF8();
	char *pFile;
	if (argc<2)
	{
		SplitGBK("");
		return 1;
	}
	switch(argv[1][0])
	{

	case 'b'://thread
	case 'B'://thread
		testBatchSpeed(argv[1]);
		break;
	case 'e'://thread
	case 'E'://thread
		SplitEnglish("Beijing Institute of Technology from the year of 2010 in China!");//Dr. Kevin Zhang worked in 
		break;
	case 'f'://thread
	case 'F'://thread
		SplitBIG5();
		break;
	case 'g'://thread
	case 'G'://thread
		SplitGBK("Test");
		break;
	case 'i'://import user dictionary
	case 'I'://
		pFile="../../Data/Userdict.txt";
		if (argc>2)
		{
			pFile=argv[2];
		}
		ImportUserDict(pFile);
		break;
	case 's'://thread
	case 'S'://thread
		testSpeed(0);
		break;
	case 'm'://thread
	case 'M'://thread
		//testThread();
		break;
	case 'n'://thread
	case 'N'://thread
		testNewWord(0);
		break;
	case 'u'://thread
	case 'U'://thread
		SplitUTF8();
		break;
	}
	return 0;
}	