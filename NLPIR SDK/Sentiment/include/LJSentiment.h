/***************************************************
*  Copyright (c) 2011. All rights reserved.
*  Procession or use of this file requires a written 
*  license from the author.
*  
*  Created: 2011-05-20
***************************************************/
#ifndef _H_SENTIMENT_DLL
#define _H_SENTIMENT_DLL


#ifdef OS_LINUX
#define SENTIMENT_API extern "C" 
#else
#ifdef LJSENTIMENT_EXPORTS
#define SENTIMENT_API extern "C" __declspec(dllexport)
#else
#define SENTIMENT_API extern "C"  __declspec(dllimport)
#endif
#endif

#define GBK_CODE 0//Ĭ��֧��GBK����
#define UTF8_CODE GBK_CODE+1//UTF8����
#define BIG5_CODE GBK_CODE+2//BIG5����
#define GBK_FANTI_CODE GBK_CODE+3//GBK���룬�������������
/*-----------------------------------------------------------------------
* ���ܣ���ʼ��
* ������sLicenseCode           - [IN]  ����������
* ���أ�true - �ɹ���false - ʧ��
* ��ע���ڽ����д˺�����������������֮ǰ���ã�ֻ��ִ��һ�Σ�
------------------------------------------------------------------------*/
SENTIMENT_API bool ST_Init(const char *sInitDirPath=0,int encode=GBK_CODE,const char* sLicenseCode="");

/*-----------------------------------------------------------------------
* ���ܣ���������۵���Ϣ�ķ����ͳ�ȡ
* ������sTitle           - [IN]  �������±���
*		sContent         - [IN]  ������������
*       sObject			 - [IN]  ���������������
* ���أ��������ַ�����xml��ʽ��
* ��ע���ڽ����д˺�������ִ�ж�Σ�֧�ֶ��߳�ִ��
------------------------------------------------------------------------*/

SENTIMENT_API const char* ST_GetOneObjectResult(const char* sTitle, const char* sContent, const char* sObject);

/*-----------------------------------------------------------------------
* ���ܣ���������۵���Ϣ�ķ����ͳ�ȡ
* ������sTitle           - [IN]  �������±���
*		sContent         - [IN]  ������������
*       sObjectRuleFile	 - [OUT] ����������������ļ�
* ���أ��������ַ�����xml��ʽ��
* ��ע���ڽ����д˺�������ִ�ж�Σ�֧�ֶ��߳�ִ��
------------------------------------------------------------------------*/
SENTIMENT_API const char* ST_GetMultiObjectResult(const char* sTitle, const char* sContent, const char* sObjectRuleFile);

/*-----------------------------------------------------------------------
* ���ܣ��������뵼����дʵ�
* ������sPositiveFile           - [IN]  �����ʵ��ļ�����
*		sNegativeFile         - [IN]   �����ʵ��ļ���
*       bInputOutput	 - [IN] true: ����ʵ䵽ϵͳ�У� false:����ϵͳ�ʵ䵽�ı��ļ�
* ���أ�int: 1:�ɹ���0:ʧ��
* ��ע��
------------------------------------------------------------------------*/
SENTIMENT_API int ST_SentiDictIO(const char* sPositiveFile,const char* sNegativeFile,bool bInputOutput=false);


//�˳����ͷ���Դ�����̽���ǰ��������ͷ���ռ�õ��ڴ���Դ
SENTIMENT_API void ST_Exit();

//������һ�εĴ�����Ϣ
SENTIMENT_API const char* ST_GetLastErrMsg();

#endif //_H_SENTIMENT_DLL
