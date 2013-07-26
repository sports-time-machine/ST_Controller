
#include "UDPReceiverThread.h"
#include "testApp.h"

UDPReceiverThread::UDPReceiverThread() {
}

void UDPReceiverThread::setup(testApp *app, int port) {
	this->app = app;
	this->port = port;
}
	
void UDPReceiverThread::threadedFunction() {
	receiver.Create();
	receiver.Bind(port);

	printf_s("UDPReceiverThread was started. (linstening port %d)\n", port);

	while (isThreadRunning()) {
		int len = receiver.Receive(buf, UDP_PACKET_BUF_SIZE);
		receiver.GetRemoteAddr(address);

		// chomp
		while (len>0 && isspace(buf[len-1]))
		{
			--len;
		}

		buf[len] = 0;	// �k���I�[�łȂ��̂Ŗ����Ƀk����u���B
		printf_s("from %s: msg=[%s]\n", address, buf);

#if 0
		char *argv[40];
		int argc;
		char *ptr = buf;
		do {
			ptr = tokenize(ptr, argv, argc);
			//for (int i=0; i<argc; i++) printf_s("argv[%d]: %s\n", i, argv[i]);
			app->dispatchMessage(argv, argc, address);
		} while (ptr != NULL);
#else
		app->msg.lock();
		app->msg.request = true;
		app->msg.done    = false;
		app->msg.received_message.clear();
		app->msg.address = address;

		app->msg.received_message.push_back(buf);
		app->msg.unlock();

		while (app->msg.done==false)
		{
			Sleep(1);
		}
#endif
	}

	receiver.Close();
	printf_s("UDPReceiverThread was stoped\n");
}

// ��������󔒂ŋ�؂��� argv/argc �`���ŕԂ��B
// �����s����ꍇ�́A���̍s�̓���Ԃ��B����ȏ�s��������� NULL ��Ԃ��B
char* tokenize(char *buf, char **argv, int &argc) {
	char *ptr = buf;
	argc = 0;
	argv[argc] = ptr;
	while (true) {
	//printf_s("debug: %c\n", *ptr);
		char c = *ptr;
		switch (c) {
		case ' ':
			*ptr = 0;	// �P��I�[���k���ɂ���
			if (argv[argc] != ptr) argc++;
			argv[argc] = ++ptr;
			break;
		case '\n':
			*ptr = 0;	// ���s�������k���ɂ���
			argc++;
			return ++ptr;
		case 0:
			argc++;
			return NULL;
		default:
			++ptr;
		}
	}
}

