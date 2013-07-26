#pragma warning(disable:4996)//unsafe function
#include "PSL/PSL.h"

#include "ofMain.h"
#include "ofxUDPManager.h"
#include "UDPReceiverThread.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include <iostream>

#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UTF16Encoding.h"

#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"

