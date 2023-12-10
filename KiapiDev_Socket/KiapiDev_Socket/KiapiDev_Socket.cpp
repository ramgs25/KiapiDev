#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

int main()
{
	SOCKET		s, cs;
	WSADATA		wsaData;
	SOCKADDR_IN sin, cli_addr;

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("WSAStartup 실패, 에러코드: %d\n", WSAGetLastError());
		return 0;
	}

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET) {
		printf("소켓 생성 실패, 에러코드 : %d\n", WSAGetLastError());
		WSACleanup(); return 0;
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(1234);
	sin.sin_addr.S_un.S_addr = htonl(ADDR_ANY);

	if (bind(s, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf("bind 실패, 에러코드: %d\n", WSAGetLastError());
		closesocket(s); WSACleanup(); return 0;
	}

	if (listen(s, 10) != 0) {
		printf("listen 모드 설정 실패, 에러코드: %d\n", WSAGetLastError());
		closesocket(s); WSACleanup(); return 0;
	}
	printf("서버를 시작합니다\n");
	printf("클라이언트로부터의 접속을 기다리고 있습니다 ...\n");

	int cli_size = sizeof(cli_addr);

	cs = accept(s, (SOCKADDR*)&cli_addr, & cli_size);
	if (cs == INVALID_SOCKET) {
		printf("접속 승인 실패, 에러코드: %d\n", WSAGetLastError());
		closesocket(s); WSACleanup(); return 0;
	}

	printf("클라이언트와 연결되었습니다\n");

	char Buffer[1024];
	recv(cs, Buffer, 1024, 0);
	printf("클라이언트가 보낸 데이터: %s\n", Buffer);

	printf("서버에 보낼 데이터 입력: ");
	scanf("%s", Buffer);
	send(cs, Buffer, strlen(Buffer), 0);

	if (closesocket(cs) != 0 || closesocket(s) != 0) {
		printf("소켓 제거 실패, 에러코드: %d\n", WSAGetLastError());
		WSACleanup(); return 0;
	}

	if (WSACleanup() != 0) {
		printf("WSACleanup 실패, 에러코드: %d\n", WSAGetLastError());
		return 0;
	}

	printf("서버를 종료합니다\n");
	return 0;
}	
