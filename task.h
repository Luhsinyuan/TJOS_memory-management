#pragma once
#include"iostream"

using namespace std;

class Task {
private:
	int task_num;
	bool is_proposer;
	int size;
public:
	Task(int num, bool proposer, int length)
	{
		task_num = num;
		is_proposer = proposer;
		size = length;
	}
	int get_task_num()
	{
		return task_num;
	}
	bool get_is_proposer()
	{
		return is_proposer;
	}
	int get_size()
	{
		return size;
	}
	void set_task_num(int num)
	{
		task_num = num;
		return;
	}
	void set_is_proposer(bool proposer)
	{
		is_proposer = proposer;
		return;
	}
	void set_size(int length)
	{
		size = length;
		return;
	}
};