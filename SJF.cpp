#include<stdio.h>
#include<iostream>
#include<string>
#define MAX 100

using namespace std;

class Process
{
public:
	string m_ProcessID;//���̺�
	double m_CreationTime;//����ʱ��
	double m_ExecutionTime;//ִ��ʱ��
	double m_StartTime;//��ʼʱ��
	double m_CompletionTime();//���ʱ��
	double m_TurnaroundTime();//��תʱ��
	double m_CarryingTurnaroundTime();//��Ȩ��תʱ��
};
double Process::m_CompletionTime()//ͨ����Ա���� �ڴ���ʱ�õ� ���ʱ�� = ��ʼʱ�� + ִ��ʱ��
{
	return this->m_StartTime + this->m_ExecutionTime;
}

double Process::m_TurnaroundTime()//��Ա���� ��תʱ�� = ���ʱ�� - ����ʱ��
{
	return this->m_CompletionTime() - this->m_CreationTime;
}

double Process::m_CarryingTurnaroundTime()//��Ա���� ��Ȩ��תʱ�� = ��תʱ�� / ִ��ʱ��
{
	return this->m_TurnaroundTime() / this->m_ExecutionTime;
}


class ProcessBook//���̼�
{
public:
	Process m_ProcessArray[MAX];//����100�����̵�����
	int m_ProcessCount;//��������
};

void showMenu();//��ʾ�˵�
void initProcessBook(ProcessBook* pb);//��ʼ�����̼�
void addProcess(ProcessBook* pb);//��ӽ���
void showProcess(ProcessBook* pb);//��ʾ����
void SJFSort(ProcessBook* pb);//����ҵ��������
void SJF(ProcessBook* pb);//����ҵ���ȵ���
void showResult(ProcessBook *pb);//������

int main(void)
{
	ProcessBook pb;
	initProcessBook(&pb);
	while (true)
	{

		showMenu();

		int select = 0;
		cout << endl << "�����������";
		cin >> select;
		switch (select)
		{
		case 0:
			cout << "�ѳɹ��˳�";
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
			cout << "��������ȷָ��";
			system("pause");
			system("cls");
			break;
		}
	}
	system("pause");
	return 0;
}

void showMenu()
{
	cout << "\t***************************************\t" << endl;
	cout << "\t**************���̵��ȳ���*************\t" << endl;
	cout << "\t**************0���˳�����**************\t" << endl;
	cout << "\t**************1����ӽ���**************\t" << endl;
	cout << "\t**************2����ʾ����**************\t" << endl;
	cout << "\t*************3������ҵ����*************\t" << endl;
	cout << "\t***************************************\t" << endl;
}

void initProcessBook(ProcessBook* pb)
{
	pb->m_ProcessCount = 0;
}

void addProcess(ProcessBook* pb)
{
	cout << "��ӽ���" << endl;
	cout << "��������̺�:";
	cin >> pb->m_ProcessArray[pb->m_ProcessCount].m_ProcessID;//
	cout << "��������̴���ʱ�䣺";
	cin >> pb->m_ProcessArray[pb->m_ProcessCount].m_CreationTime;//
	cout << "���������ִ��ʱ�䣺";
	cin >> pb->m_ProcessArray[pb->m_ProcessCount].m_ExecutionTime;//
	pb->m_ProcessCount++;
	cout << "��" << pb->m_ProcessCount << "��������ӳɹ�" << endl;
	system("pause");
	system("cls");
}

void showProcess(ProcessBook* pb)
{
	if (pb->m_ProcessCount == 0)
	{
		cout << "���޽���" << endl;
	}
	else
	{
		cout << "���н�������" << endl;
		cout << "���̺�\t����ʱ��\t����ʱ��" << endl;
		for (int i = 0; i < pb->m_ProcessCount; i++)//�����������̼� �������
		{
			cout << pb->m_ProcessArray[i].m_ProcessID << "\t" << pb->m_ProcessArray[i].m_CreationTime << "\t\t"
				<< pb->m_ProcessArray[i].m_ExecutionTime << endl;
		}
	}
	cout << endl;
	system("pause");
	system("cls");
}



void executionTimeSort(ProcessBook* pb) { // ������ִ��ʱ����������
	// ð������: ÿ���ҵ���i��pb->m_ProcessArray[i].m_ProcessCount - 1�����ȼ���ߵĽ��̣��ŵ�pb->m_ProcessArray[i]
	for (int i = 0; i < pb->m_ProcessCount - 1; i++) { // �����̴�����ִ��ʱ������������� 
		for (int j = i + 1; j < pb->m_ProcessCount; j++) {
			if (pb->m_ProcessArray[i].m_CreationTime > pb->m_ProcessArray[j].m_CreationTime
				|| (pb->m_ProcessArray[i].m_CreationTime == pb->m_ProcessArray[j].m_CreationTime
					&& pb->m_ProcessArray[i].m_ExecutionTime > pb->m_ProcessArray[j].m_ExecutionTime)) {
				Process temp = pb->m_ProcessArray[i]; // ����i��j��ӦԪ�ص�˳�� 
				pb->m_ProcessArray[i] = pb->m_ProcessArray[j];
				pb->m_ProcessArray[j] = temp;
			}
		}
	}

	for (int i = 0; i < pb->m_ProcessCount - 1; i++) {
		if (i == 0) { // ����������һ�����̵Ĵ���ʱ����̣����Դ���ʱ����ǿ�ʼʱ�� 
			pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;
		}
		else { // ����һ�����̣�������̵Ŀ�ʼʱ������һ�����̵����ʱ����Ǵ˽��̵Ĵ���ʱ�������
			// �����һ�����̵����ʱ�����/���ڴ˽��̵Ĵ���ʱ�䣬��ô��һ�����̵����ʱ����Ǵ˽��̵Ŀ�ʼʱ��
			// ��֮�� ��һ�����̵����ʱ��С�ڴ˽��̵Ĵ���ʱ�䣬��ô�˽��̵Ĵ���ʱ����ǿ�ʼʱ��
			if (pb->m_ProcessArray[i - 1].m_CompletionTime() >= pb->m_ProcessArray[i].m_CreationTime) {
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i - 1].m_CompletionTime();
			}
			else {
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;
			}
		}

		// ��һ�����ҵ�����pb->m_ProcessCount[i]���ʱ����Щ�����Ѵ��� 
		int count = 0; // ͳ��pb->m_ProcessCount[i]�������ʱ�м������̴���

		// �ӽ���pb->m_ProcessCount[i+1]��ʼѰ�� 
		for (int j = i + 1; j < pb->m_ProcessCount; j++) { // ������[j]����ʱ��С�ڽ���[i]���ʱ��ʱ���м�¼ 
			if (pb->m_ProcessArray[j].m_CreationTime <= pb->m_ProcessArray[i].m_CompletionTime()) {
				count++;
			}
			else { // ������[j]����ʱ��С�ڽ���[i]���ʱ��������ѭ�� 
				break;
			}
		}

		// �ڶ��������Ѵ����Ľ������ҵ�ִ��ʱ����̵Ľ��� 
		double min = pb->m_ProcessArray[i + 1].m_ExecutionTime;	// �������[i+1]��ִ��ʱ����� 
		int next = i + 1; // ��¼��̽��̵��±꣬��ֵΪi+1 

		for (int k = i + 1; k < i + count; k++) { // k��ȡֵ��i+1��i+count���������ʱ�����Ѵ����Ľ��� 
			if (pb->m_ProcessArray[k + 1].m_ExecutionTime < min) { // ������Щ�����ҵ���̽��̵��±�next 
				min = pb->m_ProcessArray[k + 1].m_ExecutionTime;
				next = k + 1;
			}
		}

		// ����̽��̽��������ʱ���ڵ���һ������ 
		Process temp = pb->m_ProcessArray[i + 1];
		pb->m_ProcessArray[i + 1] = pb->m_ProcessArray[next];
		pb->m_ProcessArray[next] = temp;
	}
}

void showResult(ProcessBook* pb)
{
	cout << endl << endl;
	cout << "���̵���˳�����£�" << pb->m_ProcessArray[0].m_ProcessID;
	for (int i = 1; i < pb->m_ProcessCount; i++)
	{
		cout << "->";
		cout << pb->m_ProcessArray[i].m_ProcessID;
	}

	cout << endl << endl << "���̵����㷨��Ľ����" << endl;
	cout << "���̺�\t����ʱ��\t����ʱ��\t��ʼʱ��\t���ʱ��\t��תʱ��\t��Ȩ��תʱ��" << endl;
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
	ave_TurnaroundTime /= pb->m_ProcessCount;//ͨ����Ա����������ѭ����֪�ܵ���תʱ��ʹ�Ȩ��תʱ�� 
	ave_CarryingTurnaroundTime /= pb->m_ProcessCount;//ֻ������ܵĽ������Ϳɵó�ƽ����תʱ���ƽ����Ȩ��תʱ��

	cout << "ƽ����תʱ�䣺" << ave_TurnaroundTime << endl;
	cout << "ƽ����Ȩ��תʱ�䣺" << ave_CarryingTurnaroundTime << endl;
	cout << "\t***************************************\t" << endl;

}
void SJF(ProcessBook* pb)
{
	executionTimeSort(pb);
	cout << endl;
	cout << "���̺�\t����ʱ��\t����ʱ��" << endl;
	//�����������Ƿ���Ч
	//for (int i = 0; i < pb->m_PrpcessCount; i++)//�����������̼� �������
	//{
	//	cout << pb->m_ProcessArray[i].m_ProcessID << "\t" << pb->m_ProcessArray[i].m_CreationTime << "\t\t"
	//		<< pb->m_ProcessArray[i].m_ExcutionTime << endl;
	//}
	for (int i = 0; i < pb->m_ProcessCount; i++)
	{
		if (i == 0)
		{
			pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;//��һ������ ��ʼʱ�丳ֵΪ����ʱ��
		}
		else
		{
			if (pb->m_ProcessArray[i - 1].m_CompletionTime() >= pb->m_ProcessArray[i].m_CreationTime)//��������ǰһ���������ʱ��Ⱥ�һ�����̵Ĵ���ʱ���
			{
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i - 1].m_CompletionTime();//����һ�����̵Ŀ�ʼʱ�丳ֵΪǰһ�����̵����ʱ��
			}
			else
			{
				pb->m_ProcessArray[i].m_StartTime = pb->m_ProcessArray[i].m_CreationTime;//���򽫺�һ�����̵Ŀ�ʼʱ�丳ֵΪ����ʱ��
			}
		}
	}
	showResult(pb);
	cout << endl;
	system("pause");
	system("cls");
}


