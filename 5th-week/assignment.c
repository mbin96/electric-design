#include <stdio.h>
#include <stdlib.h>
//winAPI를 사용할수있도록 windows.h 인클루드
#include <windows.h>

//시리얼로 보낼 캐릭터로 된 숫자좌표 글로벌변수
char point[5];

//시리얼로 보낼 캐릭터로 된 숫자좌표 생성 함수
char * cursorToChar(int a){
    //각 자리수를 구해 char '0'(48)과 더한다(즉 char형 ascii 숫자로 변경해줌 ) 
    point[0] = (a / 1000) + '0';			//10^3자리
    point[1] = ((a % 1000) / 100) + '0';	//10^2자리
    point[2] = ((a % 100) /10) + '0';		//10^1자리
    point[3] = (a % 10) + '0';				//1의 자리
    point[4] = '\0';
	//만든 char 리턴해줌
    return point;
}


int main()
{
	//시리얼 핸들 열기
	HANDLE hSerial;

	//변수 설정
	unsigned long bytes_read;
    unsigned long bytes_written;
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-dcb 참고
	//시리얼의 컨트롤 설정의 스트럭스 값을 선언하며 초기화
	DCB dcbSerialParams = { 0 };
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-commtimeouts 참고
	//시리얼 디바이스의 타임아웃 스트럭트값을 선언하며 초기화
	COMMTIMEOUTS timeouts = { 0 };

	// https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-createfilea 참고
	hSerial = CreateFile("COM1", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// VSPE 세팅을 했음에도 불구하고, COM port를 여는 과정에 문제가 생긴다면, L"COM2"에서 L을 지우고 다시 빌드하세요.)
	//핸들을 여는데 실패한경우 에러메세지 설정
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		//포트가 안열린경우
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("ERROR!! COM1 PORT NOT FOUND\n");
		}
		printf("ERROR!! FAIIED TO ACCESS TO COM2 SUCCESSFULLY\n");
	}

	//스트럭트의 길이 설정
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getcommstate 참고
	//serial디바이스의 컨트롤 셋팅을 참조하여 참조하는데실패하면 에러메세지를 출력한다.
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		printf("ERROR!! FAIIED TO GET SERIAL PARAMETERS\n");
	}

	//시리얼 설정을 지정한다.
	//비트레이트 57600bps로 설정
	dcbSerialParams.BaudRate = CBR_57600;
	//sizeof(byte)==8설정
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.StopBits = ONESTOPBIT;
	
	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-setcommstate 참고
	//DCB에 따라 시리얼 통신을 설정한다. 실패하면 에러메세지를 출력한다.
	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		printf("ERROR, Setting serial port state");
	}

	//타임아웃에 대한 설정 수행
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	//포인터를 받아올 캐릭터 배열을 설정
    char mouseInputX[5]={0}, mouseInputY[5]={0};
    //포인트 변수
	POINT a;
	//핸들 변수 선언
    HANDLE hIn;
	//인풋을 저장할 rec변수 선언
    INPUT_RECORD rec;
    DWORD dw;
	//STD_INPUT_HANDLE을 핸들변수에 가져옴
    hIn = GetStdHandle(STD_INPUT_HANDLE);

	// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-setcommtimeouts 참고
	//시리얼 통신에 대한 타임 아웃 파라메터를 설정하고 예외가 발생하면 에러메세지를 출력하고 종료.
	if (!SetCommTimeouts(hSerial, &timeouts))
	{
		printf("ERROR!! Time Out\n");
		return 1;
	}
	else
	{
        ////////////////////////시리얼 통신 함수의 시작/////////////////
        while (1)
		{
			//콘솔 입력을 기록한다.
			ReadConsoleInput(hIn, &rec, 1, &dw);

			//rec에 더블클릭이 기록되면 내부 실행
			if (rec.EventType == MOUSE_EVENT){            
                if (rec.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK){//더블클릭 감지
					//프린트
					printf("Double click detacted");

					//핸들리셋(일반적으로 없어도 되지만 알수없는 이유로 가비지 값의 시리얼 입력이 발생하여 핸들을 초기화하여 해결했습니다.)
					CloseHandle(hSerial);
					hSerial = CreateFile("COM1", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					
					//상대방 콘솔에서 시리얼을 통해 (char) x좌표 입력받기
					WriteFile(hSerial, "input x>", 9, &bytes_written, NULL);
					for (int i=0;i<5;i++){
						//상대방 콘솔에서 좌표를 한글자씩 받아서mouseInput에 저장한다.
						ReadFile(hSerial, &mouseInputX[i], 1, &bytes_read, NULL);
						//상대방 콘솔에 입력받은 좌표를 출력해서 보여준다.
						WriteFile(hSerial, &mouseInputX[i], 1, &bytes_written, NULL);
						//상대방측에서 엔터를 입력받으면 좌표를 그만 입력받는다.
						if(mouseInputX[i]=='\r'){
							WriteFile(hSerial, "\r\n", 2, &bytes_written, NULL);
							break;
						}
					}

					//상대방 콘솔에서 시리얼을 통해 (char) y좌표 입력받기
					WriteFile(hSerial, "input y>", 9, &bytes_written, NULL);
					for (int i=0;i<5;i++){
						//상동
						ReadFile(hSerial, &mouseInputY[i], 1, &bytes_read, NULL);
						WriteFile(hSerial, &mouseInputY[i], 1, &bytes_written, NULL);
						if(mouseInputY[i]=='\r'){
							WriteFile(hSerial, "\r\n", 2, &bytes_written, NULL);
							break;
						}
					}

					//입력한 좌표를 통해 윈도우의 마우스 커서를 설정한다.
					SetCursorPos(atoi(mouseInputX),atoi(mouseInputY));
				}
            }

			//윈도우 커서값을 a포인터에 가져온다.
            GetCursorPos(&a);
            
			//받아온 커서값을 시리얼로 출력한다.
			//출력할때는 cursorToChar함수를 통해서 (int)좌표를 char로 바꿔서넘겨준다.
			WriteFile(hSerial, cursorToChar(a.x), 5, &bytes_written, NULL);
            WriteFile(hSerial, " ", 1, &bytes_written, NULL);//x와 y사이 공백
			WriteFile(hSerial, cursorToChar(a.y), 5, &bytes_written, NULL);
			//커서를 다 출력하면 엔터한다.
			WriteFile(hSerial, "\r\n", 2, &bytes_written, NULL);
        }
	}

	//프로그램 종료시 핸들 반납
	CloseHandle(hSerial);
	return 0;
}