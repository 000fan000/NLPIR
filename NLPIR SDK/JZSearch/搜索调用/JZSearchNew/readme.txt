
���ܣ��������ӿڵ��õķ�װ


�����б�
search_all();��ѯ��������
search_and(String,String..) �߼� ��
...�������ע��

���ò��裺
 	����jar��
	�½�LJSearch����
	���ö���ĳ�Ա����������set_*��search_*���������й�������
	����run()������ִ������������ֵ����ΪList<Map<String,String>>
ʾ����
		LJSearch a = new LJSearch();
		a.search_all();
		a.set_unique("id");
		List<Map<String,String>> res = a.run();
		System.out.println(res);