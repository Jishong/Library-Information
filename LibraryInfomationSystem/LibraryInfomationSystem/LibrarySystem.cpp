/*********************************************************
 *				Database - Final Project				 *
 *					1114606 ������						 *
 *				����Ȱ��-������ ���� ���α׷�				 *
 *					High-Level Lanuage					 *
 *********************************************************/

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

const char *host = "localhost";
const char *user = "sj7226";
const char *pw = "1234";
const char *db = "volunteering_management_project";

void showPerson (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row);
void showWork (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row);
void showRecruiting (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row);
void searchResult (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row, int n);
void searchResult_work (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row, int n);

int main(int argc, CHAR* argv[])
{
	MYSQL *connection = NULL;
	MYSQL conn;
	MYSQL_RES *sql_result = NULL;
    MYSQL_ROW sql_row = NULL;

	int vno = NULL;
	char vname[30];
	char job[30];
	char contact[30];
	char query[200];
	int wno = NULL;
	char wcontent[40] = {NULL};
	int dotime = NULL;
	char date[15];
	char cycle[10];
	char state[10];
	int i = 9;
	int n = 0;
	int d = 0;
	int w = 0;
	int rs = 0;

	if(mysql_init(&conn) == NULL)
		printf("mysql_init() error!");
		
	if(mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0))
	{
		while(i != 0)
		{
			printf("  1. ������ �Է�\n");
			printf("  2. ������ ��ȸ\n");
			printf("  3. ����Ȱ�� �Է�\n");
			printf("  4. ����Ȱ�� ��ȸ\n");
			printf("  5. ����Ȱ�� ��û\n");
			printf("  6. ������� ��ȸ\n");
			printf("  7. ���系�� ��ȸ\n");
			printf("  8. ������ ����\n");
			printf("  0. ����\n");
			scanf("%d",&i);

			if(i == 1) {
				//Volunteer���̺� �����͸� �����Ѵ�.
				printf("connection is good!\n");
				printf("�����ڿ��� : ");
				scanf("%d", &vno);
				printf("�������̸� : ");
				fflush(stdin);
				gets(vname);
				printf("���� : ");
				scanf("%s", job);
				printf("����ó : ");
				scanf("%s", contact);
				sprintf(query,"insert into Volunteer(vno, vname, job, contact) values('%d', '%s', '%s', '%s')", vno, vname, job, contact);
				mysql_query (&conn, query);
				printf("��ϵǾ����ϴ�.\n");

				//volunteer ���̺� ����Լ�
				showPerson(&conn, sql_result, sql_row);
			}

			if(i==2) {
				//volunteer ���̺� ����Լ� - ������ ��ȸ
				showPerson(&conn, sql_result, sql_row);
			}

			if(i==3) {
				//Work���̺� �����͸� �����Ѵ�.
				printf("����Ȱ�� ��ȣ : ");
				scanf("%d", &wno);
				printf("Ȱ������ : \n");
				fflush(stdin);
				gets(wcontent);
				printf("\n");
				printf("Ȱ���ð�(4) : ");
				scanf("%d", &dotime);
				printf("��¥(yyyy-mm-dd) : ");
				scanf("%s", date);
				printf("�ֱ�(Regular �Ǵ� Irregular) : ");
				scanf("%s", cycle);
				sprintf(query,"insert into work(wno, wcontent, dotime, date, cycle) values('%d', '%s', '%d', '%s', '%s')", wno, wcontent, dotime, date, cycle);
				mysql_query (&conn, query);
				printf("��ϵǾ����ϴ�.\n");

				//work���̺� ����Լ�
				showWork(&conn, sql_result, sql_row);
			}

			if(i==4) {
				//work���̺� ����Լ� - ����Ȱ�� ��ȸ
				showWork(&conn, sql_result, sql_row);
			}

			if(i==5) {
				//Ư�� �����ڸ� Ư�� ����Ȱ���� ��Ī��Ų��(����Ȱ���� ��û�Ѵ�.)
				printf("�����ڿ��� : ");
				scanf("%d", &vno);
				printf("����Ȱ�� ��ȣ : ");
				scanf("%d", &wno);
				printf("Ȱ������ : ");
				scanf("%s", state);
				sprintf(query,"insert into Recruiting(vno, wno, state) values('%d', '%d', '%s')", vno, wno, state);
				mysql_query (&conn, query);
				printf("��ϵǾ����ϴ�.\n");
			
				showRecruiting(&conn, sql_result, sql_row);
			}
			if(i==6) {
				//����Ȱ�� ������ ��ȸ�Ѵ�.
				printf("������ ��ȸ�� �������� ������ �Է��ϼ���. : ");
				scanf("%d", &n);
				searchResult(&conn, sql_result, sql_row, n);
				//searchResult_work(&conn, sql_result, sql_row, n);
			}
			if(i==7) {
				//���系���� ��ȸ�Ѵ�.
				printf("���系���� ��ȸ�� �������� ������ �Է��ϼ���. : ");
				scanf("%d", &n);
				//searchResult(&conn, sql_result, sql_row, n);
				searchResult_work(&conn, sql_result, sql_row, n);
			}

			if(i==8) {
				//����ڰ� ������ �����͸� �����ϵ��� �Է� �޴´�.
				printf("� �����͸� �����Ͻðڽ��ϱ�? (1:������  2:����Ȱ��  3:��û����)\n");
				scanf("%d", &d);
			
				if(d == 1) {
					//������ ������ ����
					printf("�������� ������ ���� �Է� : \n");
					scanf("%d", &n);
					sprintf(query,"delete from volunteer where vno='%d'", n);
					mysql_query (&conn, query);
					printf("�����Ǿ����ϴ�.\n");
					showPerson(&conn, sql_result, sql_row);
				}

				if(d == 2) {
					//����Ȱ�� ������ ����
					printf("�������� ����Ȱ�� ��ȣ �Է� : \n");
					scanf("%d", &w);
					sprintf(query,"delete from work where wno='%d'", w);
					mysql_query (&conn, query);
					printf("�����Ǿ����ϴ�.\n");
					showWork(&conn, sql_result, sql_row);
				}

				if(d == 3) {
					//��û���� ������ ����
					printf("��û�� öȸ�� �������� ������ ����Ȱ�� ��ȣ �Է� : \n");
					printf("�����ڿ��� : ");
					fflush(stdin);
					scanf("%d", &n);
					printf("����Ȱ�� ��ȣ : ");
					fflush(stdin);
					scanf("%d", &w);
					sprintf(query,"delete from recruiting where vno='%d' and wno='&d'", n, w);
					mysql_query (&conn, query);
					printf("�����Ǿ����ϴ�.\n");
					showRecruiting(&conn, sql_result, sql_row);
				}
			}
		}	
	}
	mysql_free_result(sql_result);
	mysql_close(connection);
	return 0;
}

void showPerson (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row) {
	char *query = "select *from volunteer order by vno";
	int state = 0;

	state = mysql_query(connection, query);
	if(state == 0)
	{
		printf("+-----------------------------------------------+\n");
		printf("+  Vno  |    name    |   job   |     contact    +\n");
		printf("+-----------------------------------------------+\n");
		sql_result = mysql_store_result(connection);			// Result Set�� ����
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set���� 1���� �迭�� ������
		{
			printf("+ %3s   | %10s | %7s | %13s  +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3]);	// ����� �迭�� ���
		}
		printf("+-----------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set �����Ѵ�
	}
}

void showWork (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row) {
	char *query = "select *from work order by wno";
	int state = 0;

	state = mysql_query(connection, query);
	if(state == 0)
	{
		printf("+----------------------------------------------------------------------+\n");
		printf("+  Wno |         Content         |  Time  |    Date    |   Cycle   +\n");
		printf("+----------------------------------------------------------------------+\n");
		sql_result = mysql_store_result(connection);			// Result Set�� ����
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set���� 1���� �迭�� ������
		{
			printf("+ %3s  |  %s  |  %3s  |  %10s  |  %7s   +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3],sql_row[4]);	// ����� �迭�� ���
		}
		printf("+----------------------------------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set �����Ѵ�
	}
}

void showRecruiting (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row) {
	char *query = "select *from recruiting order by vno, wno";
	int state = 0;

	state = mysql_query(connection, query);
	if(state == 0)
	{
		printf("+-----------------------------+\n");
		printf("+  Vno  |  Wno  |   state     +\n");
		printf("+-----------------------------+\n");
		sql_result = mysql_store_result(connection);			// Result Set�� ����
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set���� 1���� �迭�� ������
		{
			printf("+ %3s   | %3s   |   %7s   +\n",sql_row[0],sql_row[1],sql_row[2]);	// ����� �迭�� ���
		}
		printf("+------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set �����Ѵ�
	}
}

void searchResult (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row, int n) {
	char query[200];
	int state = 0;
	
	/*
	  �����ں� �Ѻ���ð��� ����Ƚ���� ��ȸ�ϱ� ���� ���ǹ�
	  SELECT Recruiting.VNO, VName, sum(DoTime), count(Recruiting.WNO)
	  FROM Recruiting, Work, Volunteer
	  WHERE Recruiting.VNO=vno
	  AND Recruiting.WNO=Work.WNO
	  AND Recruiting.VNO=Volunteer.VNO
	  group by VNO;
	*/
	sprintf(query,"SELECT Recruiting.VNO, VName, sum(DoTime), count(Recruiting.WNO) FROM Recruiting, Work, Volunteer WHERE Recruiting.VNO='%d' AND Recruiting.WNO=Work.WNO AND Recruiting.VNO=Volunteer.VNO group by VNO", n);
	state = mysql_query(connection, query);
	if(state == 0)
	{
		printf("+--------------------------------------------------------+\n");
		printf("+  Vno  |    name    |  total hour | num of volunteering +\n");
		printf("+--------------------------------------------------------+\n");
		sql_result = mysql_store_result(connection);			// Result Set�� ����
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set���� 1���� �迭�� ������
		{
			printf("+ %3s   | %10s |  %5s      |  %10s times   +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3]);	// ����� �迭�� ���
		}
		printf("+--------------------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set �����Ѵ�
	}
}

void searchResult_work (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row, int n) {
	char query[200];
	int state2 = 0;	
	/*�� �������� ����Ȱ�� ������ ��ȸ�ϱ� ���� ���ǹ�
	  SELECT Recruiting.wno, wcontent, dotime, date
	  FROM Recruiting, Work
	  WHERE Recruiting.vno=$vno
	  AND Recruiting.wno=Work.wno;
	*/
	sprintf(query,"SELECT Recruiting.wno, wcontent, dotime, date FROM Recruiting, Work WHERE Recruiting.vno='%d' AND Recruiting.wno=Work.wno", n);
	state2 = mysql_query(connection, query);

	if(state2 == 0)
	{
		printf("+------------------------------------------------------+\n");
		printf("+  Wno |         Content         |  Time  |    Date    +\n");
		printf("+------------------------------------------------------+\n");
		sql_result = mysql_store_result(connection);			// Result Set�� ����
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set���� 1���� �迭�� ������
		{
			printf("+ %3s  |  %s  |  %3s  |  %10s  +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3]);	// ����� �迭�� ���
		}
		printf("+-------------------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set �����Ѵ�
	}
}