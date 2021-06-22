#pragma once
#include"iostream"

using namespace std;

class Block {
private:
	int start_place;			//��ʼ��ַ
	int length;					//�ڴ�鳤��
	int task_num;				//��������ţ�-1��ʾδ������
	int total_order;            //�ÿ��������
public:
	Block(int start, int size, int num, int order = 0)
	{
		start_place = start;
		length = size;
		task_num = num;
		total_order = order;
	}
	int get_total_order()
	{
		return total_order;
	}
	void set_total_order(int order)
	{
		total_order = order;
		return;
	}
	int get_start_place()
	{
		return start_place;
	}
	int get_length()
	{
		return length;
	}
	int get_task_num()
	{
		return task_num;
	}
	void set_start_place(int start)
	{
		start_place = start;
		return;
	}
	void set_length(int size)
	{
		length = size;
		return;
	}
	void set_task_num(int num)
	{
		task_num = num;
		return;
	}
};