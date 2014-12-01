/*********************************************************
 *				Database - Final Project				 *
 *					1114606 유수정						 *
 *				봉사활동-봉사자 관리 프로그램				 *
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
			printf("  1. 봉사자 입력\n");
			printf("  2. 봉사자 조회\n");
			printf("  3. 봉사활동 입력\n");
			printf("  4. 봉사활동 조회\n");
			printf("  5. 봉사활동 신청\n");
			printf("  6. 봉사실적 조회\n");
			printf("  7. 봉사내역 조회\n");
			printf("  8. 데이터 삭제\n");
			printf("  0. 종료\n");
			scanf("%d",&i);

			if(i == 1) {
				//Volunteer테이블에 데이터를 삽입한다.
				printf("connection is good!\n");
				printf("봉사자연번 : ");
				scanf("%d", &vno);
				printf("봉사자이름 : ");
				fflush(stdin);
				gets(vname);
				printf("직업 : ");
				scanf("%s", job);
				printf("연락처 : ");
				scanf("%s", contact);
				sprintf(query,"insert into Volunteer(vno, vname, job, contact) values('%d', '%s', '%s', '%s')", vno, vname, job, contact);
				mysql_query (&conn, query);
				printf("등록되었습니다.\n");

				//volunteer 테이블 출력함수
				showPerson(&conn, sql_result, sql_row);
			}

			if(i==2) {
				//volunteer 테이블 출력함수 - 봉사자 조회
				showPerson(&conn, sql_result, sql_row);
			}

			if(i==3) {
				//Work테이블에 데이터를 삽입한다.
				printf("봉사활동 번호 : ");
				scanf("%d", &wno);
				printf("활동내용 : \n");
				fflush(stdin);
				gets(wcontent);
				printf("\n");
				printf("활동시간(4) : ");
				scanf("%d", &dotime);
				printf("날짜(yyyy-mm-dd) : ");
				scanf("%s", date);
				printf("주기(Regular 또는 Irregular) : ");
				scanf("%s", cycle);
				sprintf(query,"insert into work(wno, wcontent, dotime, date, cycle) values('%d', '%s', '%d', '%s', '%s')", wno, wcontent, dotime, date, cycle);
				mysql_query (&conn, query);
				printf("등록되었습니다.\n");

				//work테이블 출력함수
				showWork(&conn, sql_result, sql_row);
			}

			if(i==4) {
				//work테이블 출력함수 - 봉사활동 조회
				showWork(&conn, sql_result, sql_row);
			}

			if(i==5) {
				//특정 봉사자를 특정 봉사활동에 매칭시킨다(봉사활동을 신청한다.)
				printf("봉사자연번 : ");
				scanf("%d", &vno);
				printf("봉사활동 번호 : ");
				scanf("%d", &wno);
				printf("활동여부 : ");
				scanf("%s", state);
				sprintf(query,"insert into Recruiting(vno, wno, state) values('%d', '%d', '%s')", vno, wno, state);
				mysql_query (&conn, query);
				printf("등록되었습니다.\n");
			
				showRecruiting(&conn, sql_result, sql_row);
			}
			if(i==6) {
				//봉사활동 실적을 조회한다.
				printf("실적을 조회할 봉사자의 연번을 입력하세요. : ");
				scanf("%d", &n);
				searchResult(&conn, sql_result, sql_row, n);
				//searchResult_work(&conn, sql_result, sql_row, n);
			}
			if(i==7) {
				//봉사내역을 조회한다.
				printf("봉사내역을 조회할 봉사자의 연번을 입력하세요. : ");
				scanf("%d", &n);
				//searchResult(&conn, sql_result, sql_row, n);
				searchResult_work(&conn, sql_result, sql_row, n);
			}

			if(i==8) {
				//사용자가 삭제할 데이터를 선택하도록 입력 받는다.
				printf("어떤 데이터를 삭제하시겠습니까? (1:봉사자  2:봉사활동  3:신청내역)\n");
				scanf("%d", &d);
			
				if(d == 1) {
					//봉사자 데이터 삭제
					printf("등록취소할 봉사자 연번 입력 : \n");
					scanf("%d", &n);
					sprintf(query,"delete from volunteer where vno='%d'", n);
					mysql_query (&conn, query);
					printf("삭제되었습니다.\n");
					showPerson(&conn, sql_result, sql_row);
				}

				if(d == 2) {
					//봉사활동 데이터 삭제
					printf("등록취소할 봉사활동 번호 입력 : \n");
					scanf("%d", &w);
					sprintf(query,"delete from work where wno='%d'", w);
					mysql_query (&conn, query);
					printf("삭제되었습니다.\n");
					showWork(&conn, sql_result, sql_row);
				}

				if(d == 3) {
					//신청내역 데이터 삭제
					printf("신청을 철회할 봉사자의 연번과 봉사활동 번호 입력 : \n");
					printf("봉사자연번 : ");
					fflush(stdin);
					scanf("%d", &n);
					printf("봉사활동 번호 : ");
					fflush(stdin);
					scanf("%d", &w);
					sprintf(query,"delete from recruiting where vno='%d' and wno='&d'", n, w);
					mysql_query (&conn, query);
					printf("삭제되었습니다.\n");
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
		sql_result = mysql_store_result(connection);			// Result Set에 저장
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set에서 1개씩 배열에 가져옴
		{
			printf("+ %3s   | %10s | %7s | %13s  +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3]);	// 저장된 배열을 출력
		}
		printf("+-----------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set 해제한다
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
		sql_result = mysql_store_result(connection);			// Result Set에 저장
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set에서 1개씩 배열에 가져옴
		{
			printf("+ %3s  |  %s  |  %3s  |  %10s  |  %7s   +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3],sql_row[4]);	// 저장된 배열을 출력
		}
		printf("+----------------------------------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set 해제한다
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
		sql_result = mysql_store_result(connection);			// Result Set에 저장
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set에서 1개씩 배열에 가져옴
		{
			printf("+ %3s   | %3s   |   %7s   +\n",sql_row[0],sql_row[1],sql_row[2]);	// 저장된 배열을 출력
		}
		printf("+------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set 해제한다
	}
}

void searchResult (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row, int n) {
	char query[200];
	int state = 0;
	
	/*
	  봉사자별 총봉사시간과 봉사횟수를 조회하기 위한 질의문
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
		sql_result = mysql_store_result(connection);			// Result Set에 저장
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set에서 1개씩 배열에 가져옴
		{
			printf("+ %3s   | %10s |  %5s      |  %10s times   +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3]);	// 저장된 배열을 출력
		}
		printf("+--------------------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set 해제한다
	}
}

void searchResult_work (MYSQL*connection, MYSQL_RES *sql_result, MYSQL_ROW sql_row, int n) {
	char query[200];
	int state2 = 0;	
	/*이 봉사자의 봉사활동 내역을 조회하기 위한 질의문
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
		sql_result = mysql_store_result(connection);			// Result Set에 저장
		while((sql_row = mysql_fetch_row(sql_result))!= NULL)	// Result Set에서 1개씩 배열에 가져옴
		{
			printf("+ %3s  |  %s  |  %3s  |  %10s  +\n",sql_row[0],sql_row[1],sql_row[2],sql_row[3]);	// 저장된 배열을 출력
		}
		printf("+-------------------------------------------------------+\n");
		mysql_free_result(sql_result);		// Result Set 해제한다
	}
}