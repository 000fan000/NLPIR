#ifdef _WIN64
#pragma comment(lib, "../lib/win64/LJSentiment.lib")
#else
#pragma comment(lib, "../lib/win32/LJSentiment.lib")
#endif

#include <cstdio>
#include <string>
#include "LJSentiment.h"

using namespace std;

const string rootDir = "../";

int main(void)
{
	if (!ST_Init(rootDir.c_str(), 0, ""))
	{
		printf("��ʼ��ʧ��");
		printf("�����������...");
		getchar();
		return 0;
	}

	/**
	* ���Ե�����дʵ�
	*/
	if (ST_SentiDictIO((rootDir + "test/positive.txt").c_str(), (rootDir + "test/negative.txt").c_str(), false) == 1)
	{
		printf("�ʵ䵼���ɹ����뵽��Ӧ��Ŀ¼����");
	}
	else
	{
		printf("�ʵ䵼��ʧ��");
	}

	/**
	* ���Ե�����дʵ�
	*/
	if (ST_SentiDictIO((rootDir + "test/pos.txt").c_str(), (rootDir + "test/neg.txt").c_str(), true) == 1)
	{
		printf("��ϲ���ʵ䵼��ɹ�");
	}
	else
	{
		printf("�ʵ䵼��ʧ��");
	}

	printf("=====================================================================================");
	string sTitle = "���¸��跴˼��������ˮ�����������ܸ���";
	string sContent = "�ݡ��������������������Ѿ�֤ʵ1��9�����׶����ʻ������˵ľ��ǵ�����Ա�����ҵ���������Ա������˱����������������������Ŀǰ�������������������飬�����¸��롰���ˡ������ٴ��������ǵ����顣\n������Ϊʲô��ƵƵ���ִ����¼�������һ�������Ǻ����������顣����̨�ϣ����¸ٰ��Լ�����ηŵ���ô�ͣ���������������ף�Ϊʲôһ����̨������ƺ���ȫ�ǻ���һ������?\n���¸ٲ��������ڣ������Ļ�˵�Լ�����һ����������������Ա��������Щ�������ڵ���ͳһ����Ϸ��Ϊ�������ġ���������һ�г��Ͼ��Լ������Ŀ�������̺��ڿ���Щ��ν�ġ�������������Ա�����¸ٳ��������Ĳ����ĸ��������ţ��������Լ���ֲ�и��Ŭ���������ǹ����ǵ������������ǵ���̨����Ĵ������������ԣ����ڳ�������ӡ���ʵ������ʱ���һֱ����Ҫ˵�ף��������ף�������Ա��������!��";

	printf("����Ϊ��\n%s\n", sTitle.c_str());
	printf("����Ϊ��\n%s\n", sContent.c_str());

	printf(ST_GetOneObjectResult(sTitle.c_str(), sContent.c_str(), "���¸�"));
	printf(ST_GetMultiObjectResult(sTitle.c_str(), sContent.c_str(), "stConduct.xml"));


	ST_Exit();

	printf("����");
	printf("�����������...");
	getchar();
	return 0;
}