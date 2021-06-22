#include"Block.h"
#include"Task.h"
#include"vector"
#include"algorithm"


bool cmp1(Block* block1, Block* block2)													//按照地址顺序排序的比较函数
{
	return block1->get_start_place() < block2->get_start_place();
}
bool cmp2(Block* block1, Block* block2)													//按照空间大小排序的比较函数
{
	return block1->get_length() < block2->get_length();
}										
void print_mem(Task* task,vector<Block*> total_list)									//显示当前执行的任务和内存情况
{
	cout << "*******************************" << endl;
	cout << "当前执行的指令：" << endl;
	cout << "作业" << "\t" << "操作" << "\t" << "大小" << endl;
	cout << task->get_task_num() << "\t";
	if (task->get_is_proposer())
		cout << "申请" << "\t";
	else
		cout << "释放" << "\t";
	cout << task->get_size() << endl;
	cout << "********************************" << endl;
	cout << "\n" << "\n" << "\n";
	cout << "********************************" << endl;
	cout << "当前空闲内存块状况：" << endl;
	cout << "起始处" << "\t" << "长度" << "\t" << "任务号" << "\t" << "总排序" << endl;
	for (int i = 0; i < total_list.size(); i++)
	{
		total_list[i]->set_total_order(i + 1);
		if(total_list[i]->get_task_num()==-1)
			cout << total_list[i]->get_start_place() << "\t" << total_list[i]->get_length() << "\t" << total_list[i]->get_task_num() << "\t" << total_list[i]->get_total_order() << endl;
	}
	cout << "\n" << "\n" << "\n";
}							
void Task_init(vector<Task*>& task_list)												//默认任务的初始化，来自PPT
{
	task_list.push_back(new Task(1, true, 130));
	task_list.push_back(new Task(2, true, 60));
	task_list.push_back(new Task(3, true, 100));
	task_list.push_back(new Task(2, false, 60));
	task_list.push_back(new Task(4, true, 200));
	task_list.push_back(new Task(3, false, 100));
	task_list.push_back(new Task(1, false, 130));
	task_list.push_back(new Task(5, true, 140));
	task_list.push_back(new Task(6, true, 60));
	task_list.push_back(new Task(7, true, 50));
	task_list.push_back(new Task(6, false, 60));
}
void proposer(int choose, Task*& task, vector<Block*>& total_list)					//申请函数
{
	vector<Block*> free_block;
	vector<int> free_block_num;
	for (int i = 0; i < total_list.size(); i++)
	{
		if (total_list[i]->get_task_num() == -1)
		{
			free_block.push_back(total_list[i]);
			free_block_num.push_back(i);
		}
	}
	for (int i = free_block_num.size() - 1;i>=0; i--)
	{
		total_list.erase(total_list.begin() + free_block_num[i]);
	}
	if (choose == 2)
		sort(free_block.begin(), free_block.end(), cmp2);

	bool is_find = false;
	for (int i = 0; i < free_block.size(); i++)
	{
		Block* block = free_block[i];
		if (task->get_size() < block->get_length())
		{
			total_list.push_back(new Block(block->get_start_place(), task->get_size(), task->get_task_num()));
			block->set_start_place(block->get_start_place() + task->get_size());
			block->set_length(block->get_length() - task->get_size());
			is_find = true;
			break;
		}
		else if (task->get_size() == block->get_length())
		{
			block->set_task_num(task->get_task_num());
			is_find = true;
			break;
		}
	}
	if (!is_find)
		cout << "内存不够，无法完成该任务！" << endl;
	total_list.insert(total_list.end(), free_block.begin(), free_block.end());
	sort(total_list.begin(), total_list.end(), cmp1);
	print_mem(task,total_list);
}
void free(int choose, Task* task, vector<Block*>& total_list)							//释放函数
{
	int start_address = 0;
	int size = 0;
	int order = 0;
	for (int i = 0; i < total_list.size(); i++)
	{
		Block* block = total_list[i];
		if (block->get_task_num() == task->get_task_num())
		{
			start_address = block->get_start_place();
			size = block->get_length();
			order = block->get_total_order();
			break;
		}
	}
	if (order == total_list.size())														//如果释放最后一块
	{
		if (total_list.size() >= 2 && total_list[total_list.size() - 2]->get_task_num() == -1)                    //倒数第二块是空闲
		{
			total_list[total_list.size() - 2]->set_length(total_list[total_list.size() - 1]->get_length() + size);
			total_list.erase(total_list.begin() + order);
		}
		else
		{
			total_list[order - 1]->set_task_num(-1);
		}
	}
	else if (order == 1)																//如果释放第一块
	{
		if (total_list.size() >= 0 && total_list[1]->get_task_num() == -1)										//第二块是空闲
		{
			total_list[1]->set_start_place(start_address);
			int num = total_list[1]->get_length() + size;
			total_list[1]->set_length(total_list[1]->get_length() + size);
			total_list.erase(total_list.begin());
		}
		else
		{
			total_list[order - 1]->set_task_num(-1);
		}
	}
	else
	{
		if (total_list[order - 2]->get_task_num() == -1 && total_list[order]->get_task_num() == -1)															//上下都是空闲
		{
			total_list[order - 2]->set_length(total_list[order - 2]->get_length() + total_list[order - 1]->get_length() + total_list[order]->get_length());
			total_list.erase(total_list.begin() + order);
			total_list.erase(total_list.begin() + order - 1 );

		}
		else if (total_list[order - 2]->get_task_num() == -1 && total_list[order]->get_task_num() != -1)													//只有上边是空闲
		{
			total_list[order - 2]->set_length(total_list[order - 2]->get_length() + total_list[order - 1]->get_length());
			total_list.erase(total_list.begin() + order - 1);
		}
		else if (total_list[order - 2]->get_task_num() != -1 && total_list[order]->get_task_num() == -1)													//只有下边是空闲
		{
			total_list[order]->set_start_place(total_list[order - 1]->get_start_place());
			total_list[order]->set_length(total_list[order - 1]->get_length() + total_list[order]->get_length());
			total_list.erase(total_list.begin() + order-1);
		}
		else
		{
			total_list[order - 1]->set_task_num(-1);
		}
	}
	sort(total_list.begin(), total_list.end(), cmp1);
	print_mem(task,total_list);
}
void task_solve(vector<Block*>& total_list,int task_choose,int algo_choose)						//处理不同任务来源的函数
{
	if (task_choose == 1)
	{
		while (true)
		{
			int task_num = 0;
			string temp;
			bool is_proposer = false;
			int size = 0;
			cout << "请输入任务指令" << endl;
			cout << "依次输入：作业编号，操作类型和大小(单位为K)，比如：1  申请  140,表示为任务一申请140K的空间(输入-1退出)：" << endl;
			cin >> task_num;
			if (task_num == -1)
				break;
			cin >> temp >> size;
			if (temp == "申请")
				is_proposer = true;
			Task* task = new Task(task_num, is_proposer, size);
			if (task->get_is_proposer() == true)
			{
				proposer(algo_choose, task, total_list);
			}
			else
			{
				free(algo_choose, task, total_list);
			}
		}
	}
	else
	{
		vector<Task*> task_list;
		Task_init(task_list);
		for (int i = 0; i < task_list.size(); i++)
		{
			Task* task = task_list[i];
			if (task->get_is_proposer() == true)
			{
				proposer(algo_choose, task, total_list);
			}
			else
			{
				free(algo_choose, task, total_list);
			}
		}
	}
	return;
}
int main()
{
	vector<Block*> total_list;								//初始化，刚开始内存是空闲的，只有一个640K的空闲空间
	total_list.push_back(new Block(0, 640, -1, 1));

	cout << "动态分区存储管理，请输入算法：" << endl;
	cout << "1.首次适应算法" << endl;
	cout << "2.最佳适应算法" << endl;
	int algo_choose = 0;
	cin >> algo_choose;
	cout << "请输入任务由来：" << endl;
	cout << "1.手动输入" << endl;
	cout << "2.默认任务" << endl;
	int task_choose = 0;
	cin >> task_choose;
	task_solve(total_list,task_choose, algo_choose);
	cout << "任务结束。" << endl;
	system("pause");
	return 0;
}