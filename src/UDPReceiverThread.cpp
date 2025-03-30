#include "UDPReceiverThread.h"
#include "testApp.h"

UDPReceiverThread::UDPReceiverThread() = default;

void UDPReceiverThread::setup(testApp* app, const int port)
{
    this->app = app;
    this->port = port;
}

void UDPReceiverThread::threadedFunction()
{
    receiver.Create();
    receiver.Bind(port);

    printf_s("UDPReceiverThread was started. (listening port %d)\n", port);

    while (isThreadRunning())
    {
        int len = receiver.Receive(buf, UDP_PACKET_BUF_SIZE);
        int remotePort;
        receiver.GetRemoteAddr(address, remotePort);

        // chomp
        while (len > 0 && isspace(buf[len - 1]))
        {
            --len;
        }

        buf[len] = 0; // ヌル終端でないので末尾にヌルを置く。
        printf_s("from %s: msg=[%s]\n", address.c_str(), buf);

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
        app->msg.done = false;
        app->msg.received_message.clear();
        app->msg.address = address;

        app->msg.received_message.push_back(buf);
        app->msg.unlock();

        while (app->msg.done == false)
        {
            Sleep(1);
        }
#endif
    }

    receiver.Close();
    printf_s("UDPReceiverThread was stopped\n");
}

// 文字列を空白で区切って argv/argc 形式で返す。
// 複数行ある場合は、次の行の頭を返す。それ以上行が無ければ NULL を返す。
char* tokenize(char* buf, char** argv, int& argc)
{
    char* ptr = buf;
    argc = 0;
    argv[argc] = ptr;
    while (true)
    {
        //printf_s("debug: %c\n", *ptr);
        switch (*ptr)
        {
        case ' ':
            *ptr = 0; // 単語終端をヌルにする
            if (argv[argc] != ptr) argc++;
            argv[argc] = ++ptr;
            break;
        case '\n':
            *ptr = 0; // 改行文字をヌルにする
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
