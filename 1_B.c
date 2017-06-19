//다른 프로세스의 스택영역을 변경하는 프로그램
//mem파일 내부에서 문자열을 자동으로 찾아내는 부분을 구현하지 못하였기 때문에
//stack영역 내 문자열의 주소를 알 경우 직접 입력해 변경하는 형식으로 제작했습니다.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ADDR_PHASE 16 //스택영역 시작점에서 이동시킬 주소값

void main()
{
	int i;
	int pid;
	char str_search[100], str_change[100]; //찾을 문자열, 바꿀 문자열
	char maps_path[100], mem_path[100]; //mem과 maps의 경로를 저장할 변수
	char temp[300]; //문자열 처리용 임시 변수
	char *temp2; //문자열 처리용 임시 포인터 변수
	char sline[6][50]; //heap정보를 저장할 문자열 배열
	unsigned long addr_start, addr_end; //heap영역 주소 시작값과 끝값을 저장하는 함수

	printf("문자열을 변경할 프로세스의 PID를 입력하세요 : ");
	scanf("%d", &pid);
	printf("프로세스 내에서 찾을 문자열을 입력하세요 : ");
	scanf("%s", str_search); //이 프로그램에선 해당 변수가 동작하는 부분이 없습니다.
	printf("어떤 문자열로 바꾸실지 입력하세요 : ");
	scanf("%s", str_change);
	printf("\n");

	sprintf(maps_path, "/proc/%d/maps", pid); //입력받은 pid로 maps의 경로 생성
	sprintf(mem_path, "/proc/%d/mem", pid); //입력받은 pid로 mem의 경로 생성

	printf("[*] maps : %s \n", maps_path);
	printf("[*] mem : %s \n", mem_path);

	FILE *maps = fopen(maps_path, "r"); //maps 파일 오픈
	FILE *mem = fopen(mem_path, "r+b"); //mem 파일 오픈

	//maps가 정상적으로 오픈되었는지 검사
	if(maps == NULL)
	{
		printf("maps파일 오픈 실패입니다. \n");
		fclose(maps);
		fclose(mem);
		return;
	}
	//mem이 정상적으로 오픈되었는지 검사
	if(mem == NULL)
	{
		printf("mem파일 오픈 실패입니다. \n");
		fclose(maps);
		fclose(mem);
		return;
	}

	//maps파일의 stack영역을 나타내는 라인 검색
	while(!feof(maps))
	{
		fgets(temp, sizeof(temp), maps);
		
		if(strstr(temp, "[stack]") != NULL)
		{
			printf("[*] Found [stack] : \n");
			break;
		}
		else
			continue;

		printf("스택을 찾지 못했습니다. \n");
		fclose(maps);
		fclose(mem);
		return;
	}

	//stack영역 정보 저장
	temp2 = strtok(temp, " ");
	i=0;
	while(temp2 != NULL)
	{
		strcpy(sline[i], temp2);
		i++;
		temp2 = strtok(NULL, " ");
	}

	printf("\tpathname = %s", sline[5]);
	printf("\taddresses = %s \n", sline[0]);
	printf("\tpermisions = %s \n", sline[1]);
	printf("\toffset = %s \n", sline[2]);
	printf("\tinode = %s \n", sline[4]);

	//stack영역의 시작주소와 끝주소를 저장
	temp2 = strtok(sline[0], "-");
	addr_start = strtoul(temp2, NULL, 16);
	temp2 = strtok(NULL, "-");
	addr_end = strtoul(temp2, NULL, 16);

	printf("\tAddr start [%x] | Addr end [%x] \n", addr_start, addr_end);
	
	//파일스트림을 stack영역의 문자열이 있는 주소로 이동
	//이 프로그램에서는 문자열이 있는 주소를 따로 구해주어야 합니다.
	fseek(mem, addr_start+ADDR_PHASE, SEEK_SET);

	//mem파일에 문자열 overwrite
	fwrite(str_change, strlen(str_change), 1, mem);

	printf("[*] Writing '%s' at %x \n", str_change, addr_start+ADDR_PHASE);

	fclose(maps);
	fclose(mem);
}
