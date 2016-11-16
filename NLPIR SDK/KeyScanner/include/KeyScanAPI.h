/****************************************************************************
*
* �ؼ���ɨ�����ϵͳ Keyword Scan System Copyright (c) 2000-2016
*     Dr. Kevin Zhang (Hua-Ping Zhang)
*
*     All rights reserved.
*
* This file is the confidential and proprietary property of
* Kevin Zhang and the possession or use of this file requires
* a written license from the author.
* Filename:
* Abstract:
*          KeyScan.h: definition of the system API
* Author:   Kevin Zhang
*          Email: pipy_zhang@msn.com kevinzhang@bit.edu.cn
*			Weibo: http://weibo.com/drkevinzhang
*			Homepage: http://ictclas.nlpir.org
* Date:     2013-12-19
*
* Notes:һ�����ùؼ��ʣ�����ɨ��������дʵı��壬�緱����ʽ���м�Ӹ����������
*
****************************************************************************/
#ifndef _H_KS_DLL
#define _H_KS_DLL

#ifdef OS_LINUX
	#define KEYSCANAPI_API  extern "C"
#else
	#ifdef KEYSCANAPI_EXPORTS
		#define KEYSCANAPI_API extern "C" __declspec(dllexport)
	#else
		#define KEYSCANAPI_API extern "C" __declspec(dllimport)
	#endif
#endif


#define GBK_CODE 0//Ĭ��֧��GBK����
#define UTF8_CODE GBK_CODE+1//UTF8����
#define BIG5_CODE GBK_CODE+2//BIG5����
#define GBK_FANTI_CODE GBK_CODE+3//GBK���룬�������������
#define UTF8_FANTI_CODE GBK_CODE+4//UTF8����

#define KS_HANDLE int
/*********************************************************************
 *
 *  Func Name  : Init
 *
 *  Description: Init KS ϵͳ��ʼ��
 *               The function must be invoked before any operation listed as following
 *
 *  Parameters : const char * sInitDirPath:  Path where Data directory stored. ���ݴʵ�洢��·��
 *				 the default value is NULL, it indicates the initial directory is current working directory path
 *				 encode: encoding code;
 *				 sLicenseCode: license code for unlimited usage. common user ignore it
 *  Returns    : success or fail ; ����ԭ��¼ͨ��KS_GetLastErrorMsg
 *  Author     : Kevin Zhang  
 *  History    : 
 *              1.create 2013-6-8
 *********************************************************************/
KEYSCANAPI_API int KS_Init(const char *sInitDirPath = "", int encode = GBK_CODE, const char*sLicenceCode = 0);


/*********************************************************************
*
*  Func Name  : KS_Exit
*
*  Description: Exist KS and free related buffer �˳����ͷ���Դ
*               Exit the program and free memory
*				 The function must be invoked while you needn't any lexical anlysis
*
*  Parameters : None
*
*  Returns    : success or fail
*  Author     : Kevin Zhang
*  History    :
*              1.create 2002-8-6
*********************************************************************/
KEYSCANAPI_API void KS_Exit();

/*********************************************************************
*
*  Func Name  : KS_NewInstance
*
*  Description: New a  KeyScan Instance
*               The function must be invoked before mulitiple keyword scan filter
*
*  Parameters : 
*  Returns    : KS_HANDLE: KeyScan Handle if success; otherwise return -1;
*  Author     : Kevin Zhang
*  History    :
*              1.create 2016-11-15
*********************************************************************/
KEYSCANAPI_API KS_HANDLE KS_NewInstance();

/*********************************************************************
*
*  Func Name  : KS_DeleteInstance
*
*  Description: Delete a  KeyScan Instance with handle
*               The function must be invoked before release a specific classifier
*
*  Parameters : DC_HANDLE , KeyScan Handle
*  Returns    : 
*  Author     : Kevin Zhang
*  History    :
*              1.create 2015-9-22
*********************************************************************/
KEYSCANAPI_API int KS_DeleteInstance(KS_HANDLE handle);

/*********************************************************************
 *
 *  Func Name  : ImportUserDict
 *
 *  Description: Import User-defined dictionary �����û��ʵ�
 *  Parameters : 
 *				sFilename:Text filename for user dictionary 
 *				bool bOverwrite: 
 *				KS_HANDLE handle�� handle of KeyScanner
 *  Returns    : The  number of  lexical entry imported successfully
 *               �ɹ�����Ĵʵ�����
 *  Author     : Kevin Zhang
 *  History    : 
 *              1.create 2014-8-3
 *  �ı��ļ��ĸ�ʽΪ����������������  Ȩ�� (ע�⣬��ඨ��255�����)
 *        ���磺 AV��Ӱ ɫ�� 2
 *				 ϰ��ƽ �漰�쵼�� 1
*********************************************************************/
KEYSCANAPI_API int KS_ImportUserDict(const char *sFilename, bool bOverwrite = false, KS_HANDLE handle=0);
 
/*********************************************************************
 *
 *  Func Name  : KS_Scan
 *
 *  Description: ɨ��������ı�����
 *  Parameters : sContent:�ı�����
 *				KS_HANDLE handle�� handle of KeyScanner
 *  Returns    : const char*: �漰���е����������Ȩ�أ�����Ȩ������
 *				 �磺 ɫ��/10#����/1#
 *				 ""����ʾ��ɨ�����н��
 *  Author     : Kevin Zhang
 *  History    : 
 *              1.create 2014-8-3
 *  ���������ʽΪ�� ���η���/2#FLG/1#���쵼��/1#
*********************************************************************/
KEYSCANAPI_API const char* KS_Scan(const char*sContent, KS_HANDLE handle = 0);

/*********************************************************************
 *
 *  Func Name  : KS_ScanDetail
 *
 *  Description: ɨ��������ı�����
 *  Parameters : sContent:�ı�����
 *				KS_HANDLE handle�� handle of KeyScanner
 *  Returns    : ���ؾ����漰���е����ݣ�����������
 *  Author     : Kevin Zhang
 *  History    : 
 *              1.create 2014-8-3
// ���صĸ�ʽ���£�
// ����ֵ�����ذ�����ɨ���������ݣ�ɨ������ϸ:
//
//		*<class name="���η���" weight=1>ϰ*-��������</class>*(ɢؔͯ�ӣ����Ĳ�ˬ����
//		<class name="FLG" weight=1>��*݆��</class>�ã�
*********************************************************************/
KEYSCANAPI_API const char* KS_ScanDetail(const char*sContent, KS_HANDLE handle = 0);

/*********************************************************************
*
*  Func Name  : KS_ScanFile
*
*  Description: ɨ��������ı��ļ�����
*  Parameters : sFilename:�ı��ļ���
*				KS_HANDLE handle�� handle of KeyScanner
*  Returns    : const char*: �漰���е����������Ȩ�أ�����Ȩ������
*				 �磺 ɫ��/10##����/1##
*				 ""����ʾ��ɨ�����н��
*  Author     : Kevin Zhang
*  History    :
*              1.create 2014-8-3
*  �ı��ļ��ĸ�ʽΪ����������������
*        ���磺 AV��Ӱ ɫ��
*********************************************************************/
KEYSCANAPI_API const char* KS_ScanFile(const char	*sFilename, KS_HANDLE handle = 0);

/*********************************************************************
*
*  Func Name  : KS_ScanFileDetail
*
*  Description: ɨ��������ı��ļ�����
*  Parameters : sFilename:�ı��ļ���
*				KS_HANDLE handle�� handle of KeyScanner
*  Returns    : ���ؾ����漰���е����ݣ�����������
*  Author     : Kevin Zhang
*  History    :
*              1.create 2014-8-3
// ���صĸ�ʽ���£�
// ����ֵ�����ذ�����ɨ���������ݣ�ɨ������ϸ:
//		   *<class name="���η���" weight=1>ϰ*-��������</class>*(ɢؔͯ�ӣ����Ĳ�ˬ����
//		<class name="FLG" weight=1>��*݆��</class>�ã�
*********************************************************************/
KEYSCANAPI_API const char* KS_ScanFileDetail(const char	*sFilename, KS_HANDLE handle = 0);

/*********************************************************************
 *
 *  Func Name  : KS_GetLastErrorMsg
 *
 *  Description: �������������Ϣ
 *				 The function must be invoked while you needn't any lexical anlysis
 *
 *  Parameters : �������������Ϣ
 *
 *  Returns    : success or fail
 *  Author     : Kevin Zhang  
 *  History    : 
 *              1.create 2002-8-6
 *********************************************************************/
KEYSCANAPI_API const char* KS_GetLastErrorMsg();

#endif //_H_KS_DLL

