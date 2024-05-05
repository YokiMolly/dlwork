#include<stdio.h>
#include<iostream>
#include<string>
#define MAX 100

using namespace std;

class Process//进程类
{
public:
	string m_ProcessID;//进程号
	double m_CreationTime;//创建时间
	double m_ExecutionTime;//执行时间
	double m_StartTime;//开始时间
	double m_CompletionTime();//完成时间
	double m_TurnaroundTime();//周转时间
	double m_CarryingTurnaroundTime();//带权周转时间
};
double Process::m_CompletionTime()//通过成员函数 在创建时得到 完成时间 = 开始时间 + 执行时间
{
	return this->m_StartTime + this->m_ExecutionTime;
}

double Process::m_TurnaroundTime()//成员函数 周转时间 = 完成时间 - 创建时间
{
	return this->m_CompletionTime() - this->m_CreationTime;
}

double Process::m_CarryingTurnaroundTime()//成员函数 带权周转时间 = 周转时间 / 执行时间
{
	return this->m_TurnaroundTime() / this->m_ExecutionTime;
}


class ProcessBook//进程集类
{
public:
	Process m_ProcessArray[MAX];//容纳100个进程的数组
	int m_ProcessCount;//进程数量
};

void showMenu();//显示菜单
void initProcessBook(ProcessBook* pb);//初始化进程集
void addProcess(ProcessBook* pb);//添加进程
void showProcess(ProcessBook* pb);//显示进程
void SJFSort(ProcessBook* pb);//短作业优先排序
void SJF(ProcessBook* pb);//短作业优先调度
void showResult(ProcessBook *pb);//输出结果

int main(void)
{
	ProcessBook pb;
	initProcessBook(&pb);
	while (true)
	{

		showMenu();

		int select = 0;
		cout << endl << "请输入操作：";
		cin >> select;
		switch (select)
		{
		case 0:
			cout << "已成功退出";
			return 0;
			break;
		case 1:
			addProcess(&pb);
			break;
		case 2:
			showProcess(&pb);
			break;
		case 3:
			SJF(&pb);
			break;
		default:
			cout << "请输入正确指令";
			system("pause");
			system("cls");
			break;
		}
	}
	system("pause");
	return 0;
}

void showMenu()//主菜单
{
	cout << "\t***************************************\t" << endl;
	cout << "\t**************进程调度程序*************\t" << endl;
	cout << "\t**************0、退出程序**************\t" << endl;
	cout << "\t**************1、添加进程**************\t" << endl;
	cout << "\t**************2、显示进程**************\t" << endl;
	cout << "\t*************3、短作业优先*************\t" << endl;
	cout << "\t***************************************\t" << endl;
}

void initProcessBook(ProcessBook* pb)//初始化进程集类
{
	pb->m_ProcessCount = 0;
}

void addProcess(ProcessBook* pb)//添加进程
{
	cout << "添加进程" << endl;
	cout << "请输入进程号:";
	cin >> pb->m_ProcessArray[pb->m_ProcessCount].m_ProcessID;//
	cout << "请输入进程创建时间：";
	cin >> pb->m_ProcessArray[pb->m_ProcessCount].m_CreationTime;//
	cout << "请输入进程执行时间：";
	cin >> pb->m_ProcessArray[pb->m_ProcessCount].m_ExecutionTime;//
	pb->m_ProcessCount++;
	cout << "第" << pb->m_ProcessCount << "个进程添加成功" << endl;
	system("pause");
	system("cls");
}

void showProcess(ProcessBook* pb)//展示已经输入的进程
{
	if (pb->m_ProcessCount == 0)
	{
		cout << "暂无进程" << endl;
	}
	else
	{
		cout << "所有进程如下" << endl;
		cout << "进程号\t创建时间\t运行时间" << endl;
		for (int i = 0; i < pb->m_ProcessCount; i++)//遍历整个进程集 输出进程
		{
			cout << pb->m_ProcessArray[i].m_ProcessID << "\t" << pb->m_ProcessArray[i].m_CreationTime << "\t\t"
				<< pb->m_ProcessArray[i].m_ExecutionTime << endl;
		}
	}
	cout << endl;
	system("pause");
	system("cls");
}


//这个排序其实是偷的... 算法感觉有点烧脑就直接ctrl cv了... 能看得懂 但自己想可能想不出来...
void executionTimeSort(ProcessBook* pb)//排序算法
{ 
	// 冒泡排序
	for (int i = 0; i < pb->m_ProcessCount - 1; i++) 
	{ // 按进程创建、执行时间进行升序排序 
		//1.这段是先排序出创建第一个排序队列：创建时间从小到大的队列
		for (int j = i + 1; j < pb->m_ProcessCount; j++) 	// 依次检测第i进程与后面所有进程 如果i进程地创建时间大于后面j进程地创建时间 
									//或者进程i进程的的创建时间等于j进程创建时间 并且i进程的执行时间大于j进程的执行时间 那么将i进程与j进程交换排序位置 
									//如此检测完一轮后 i++ 进行后续的排序直到i遍历完最后的进程
		{
			if (pb->m_ProcessArray[i].m_CreationTime > pb->m_ProcessArray[j].m_CreationTime
				|| (pb->m_ProcessArray[i].m_CreationTime == pb->m_ProcessArray[j].m_CreationTime
					&& pb->m_ProcessArray[i].m_ExecutionTime > pb->m_ProcessArray[j].m_ExecutionTime)) 
			{
				Process temp = pb->m_ProcessArray[i]; // 交换i，j对应元素的顺序 
				pb->m_ProcessArray[i] = pb->m_ProcessArray[j];
				pb->m_ProcessArray[j] = temp;
			}
		}
	}

	for (int i = 0; i < pb->m_ProcessCount - 1; i++) //2.这段其实和最下面那个SJF（）是一样的作用 分情况讨论第一次排序完后的各进程开始时间
	{
		if (i == 0) 
		{ // 由于排序后第一个进程的创建时间最短 所以创建时间就是开始时间 
			pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;
		}
		else
		{ // 除第一个进程，其余进程的开始时间由上一个进程的完成时间或是此进程的创建时间决定的
			// 如果上一个进程的完成时间大于/等于此进程的创建时间，那么上一个进程的完成时间就是此进程的开始时间
			// 反之， 上一个进程的完成时间小于此进程的创建时间，那么此进程的创建时间就是开始时间
			if (pb->m_ProcessArray[i - 1].m_CompletionTime() >= pb->m_ProcessArray[i].m_CreationTime)
			{
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i - 1].m_CompletionTime();
			}
			else
			{
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;
			}
		}

		// 第一步：找到进程pb->m_ProcessCount[i]完成时有哪些进程已创建 
		int count = 0; // 统计pb->m_ProcessCount[i]进程完成时有几个进程创建

		// 从进程pb->m_ProcessCount[i+1]开始寻找 
		//3.将第一次排序后 队列中每后一个进程j的创建时间与前一个进程的完成时间i比较 如果后进程j创建时间小于等于前进程i的完成时间 
		//则在count++ 代表后进程j已经被创建了 并用count记录以及被创建的进程数
		for (int j = i + 1; j < pb->m_ProcessCount; j++) { // 当进程[j]创建时间小于进程[i]完成时间时进行记录 
			if (pb->m_ProcessArray[j].m_CreationTime <= pb->m_ProcessArray[i].m_CompletionTime())
			{
				count++;
			}
			else { // 当进程[j]创建时间小于进程[i]完成时间则跳出循环 
				break;
			}
		}

		// 第二步：在已创建的进程中找到执行时间最短的进程 
		double min = pb->m_ProcessArray[i + 1].m_ExecutionTime;	// 假设进程[i+1]的执行时间最短 
		int next = i + 1; // 记录最短进程的下标，初值为i+1 

		//4.遍历已创建进程中的进程
		for (int k = i + 1; k < i + count; k++) 
		{ // k的取值从i+1到i+count即所有完成时间内已创建的进程 
			if (pb->m_ProcessArray[k + 1].m_ExecutionTime < min) 
			{ // 遍历这些进程找到最短进程的下标next 
				min = pb->m_ProcessArray[k + 1].m_ExecutionTime;
				next = k + 1;
			}
		}
		//5.将已创建进程中最短执行时间的进程排序至前一位
		// 将最短进程交换至完成时间内的下一个进程 
		Process temp = pb->m_ProcessArray[i + 1];
		pb->m_ProcessArray[i + 1] = pb->m_ProcessArray[next];
		pb->m_ProcessArray[next] = temp;
	}
}

void showResult(ProcessBook* pb)//结果展示
{
	cout << endl << endl;
	cout << "进程调度顺序如下：" << pb->m_ProcessArray[0].m_ProcessID;
	for (int i = 1; i < pb->m_ProcessCount; i++)
	{
		cout << "->";
		cout << pb->m_ProcessArray[i].m_ProcessID;
	}

	cout << endl << endl << "进程调度算法后的结果：" << endl;
	cout << "进程号\t创建时间\t运行时间\t开始时间\t完成时间\t周转时间\t带权周转时间" << endl;
	for (int i = 0; i < pb->m_ProcessCount ;i++)
	{
		cout << pb->m_ProcessArray[i].m_ProcessID << "\t"
			<< pb->m_ProcessArray[i].m_CreationTime << "\t\t"
			<< pb->m_ProcessArray[i].m_ExecutionTime << "\t\t"
			<< pb->m_ProcessArray[i].m_StartTime << "\t\t"
			<< pb->m_ProcessArray[i].m_CompletionTime() << "\t\t"
			<< pb->m_ProcessArray[i].m_TurnaroundTime() << "\t\t"
			<< pb->m_ProcessArray[i].m_CarryingTurnaroundTime() << "\t\t" << endl;
	}
	double ave_TurnaroundTime = 0;
	double ave_CarryingTurnaroundTime = 0;

	for (int i = 0; i < pb->m_ProcessCount; i++)
	{
		ave_TurnaroundTime += pb->m_ProcessArray[i].m_TurnaroundTime();
		ave_CarryingTurnaroundTime += pb->m_ProcessArray[i].m_CarryingTurnaroundTime();
	}
	ave_TurnaroundTime /= pb->m_ProcessCount;//通过成员函数和上组循环已知总的周转时间和带权周转时间 
	ave_CarryingTurnaroundTime /= pb->m_ProcessCount;//只需除以总的进程数就可得出平均周转时间和平均带权周转时间

	cout << "平均周转时间：" << ave_TurnaroundTime << endl;
	cout << "平均带权周转时间：" << ave_CarryingTurnaroundTime << endl;
	cout << "\t***************************************\t" << endl;

}
void SJF(ProcessBook* pb)//短作业优先调度
{
	executionTimeSort(pb);
	cout << endl;
	cout << "进程号\t创建时间\t运行时间" << endl;
	//测试排序结果是否有效
	//for (int i = 0; i < pb->m_PrpcessCount; i++)//遍历整个进程集 输出进程
	//{
	//	cout << pb->m_ProcessArray[i].m_ProcessID << "\t" << pb->m_ProcessArray[i].m_CreationTime << "\t\t"
	//		<< pb->m_ProcessArray[i].m_ExcutionTime << endl;
	//}
	for (int i = 0; i < pb->m_ProcessCount; i++)
	{
		if (i == 0)
		{
			pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;//第一个进程 开始时间赋值为创建时间
		}
		else
		{
			if (pb->m_ProcessArray[i - 1].m_CompletionTime() >= pb->m_ProcessArray[i].m_CreationTime)//如果排序后前一个进程完成时间比后一个进程的创建时间后
			{
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i - 1].m_CompletionTime();//将后一个进程的开始时间赋值为前一个进程的完成时间
			}
			else
			{
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;//否则将后一个进程的开始时间赋值为创建时间
			}
		}
	}
	showResult(pb);
	cout << endl;
	system("pause");
	system("cls");
}


