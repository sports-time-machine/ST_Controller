
#include "GameRecord.h"

#include "PSL/PSL.h"

#include "testApp.h"

#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include <iostream>

using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;

#include "Poco/TextConverter.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UTF16Encoding.h"

#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"

GameRecord::GameRecord()
{
	game_id = "";
	player_id = "";
	background = "";
	color = "";
	style = "";
	comment = "";
}

void GameRecord::setGameId(std::string game_id)
{
	this->game_id = game_id;
}

void GameRecord::setPlayerId(std::string player_id)
{
	this->player_id = player_id;
}

void GameRecord::addPartnerID(std::string partner_id)
{
	partner_ids.push_back(partner_id);
}

void GameRecord::addTag(std::string tag)
{
	tags.push_back(tag);
}

void GameRecord::setBackground(std::string background)
{
	this->background = background;
}

void GameRecord::setColor(std::string color)
{
	this->color = color;
}

void GameRecord::setStyle(std::string style)
{
	this->style = style;
}

void GameRecord::setComment(std::string comment)
{
	this->comment = comment;
}


std::string GameRecord::get_json_string()
{
	char md5src[1024]; 
	char buffer[1024]; 

	//char register_date[64] = "2013-06-28 00:11:22";
	Poco::LocalDateTime local_date_time;
	std::string register_date = Poco::DateTimeFormatter::format(local_date_time, "%y-%m-%d %H:%M:%S");
	cout << register_date << endl;

    MD5Engine md5;
    DigestOutputStream ostr(md5);

	sprintf_s(md5src, "%s, %s, %s", player_id.c_str(), game_id.c_str(), register_date.c_str());
	ostr << md5src;
    ostr.flush();
    const DigestEngine::Digest& digest = md5.digest(); // obtain result
    std::string md5hex = DigestEngine::digestToHex(digest);

	std::string json;

	sprintf_s(buffer, "{\"User\":{\"username\":\"\", \"player_id\":\"%s\"},", player_id.c_str());
	json += buffer;

	sprintf_s(buffer, "\"Record\":{\"player_id\":\"%s\", \"record_id\":\"%s\", \"movie_path\":\"%s\", \"movie_length\":\"%s\",",
		player_id.c_str(), game_id.c_str(), "" /*movie_path*/, "" /*movie_length*/);
	json += buffer;

	sprintf(buffer, "\"register_date\":\"%s\", \"data\":\"\", \"comment\":\"\", ", register_date.c_str());
	json += buffer;

	json += std::string("\"tags\":\"");
	if (tags.size() > 0) {
		std::vector<std::string>::iterator it = tags.begin();
		json += (*it++);
		while (it != tags.end()) {
			json += std::string(", ")+(*it++);
		}
	}
	json += std::string("\", ");

	sprintf(buffer, "\"md5hex\":\"%s\", \"pattern\":\"%s\", \"sound\":\"%s\", \"background\":\"%s\"},",
		md5hex.c_str(), color.c_str(), sound.c_str(), background.c_str());
	json += buffer;

	json += std::string("\"Partner\":[");
	if (partner_ids.size() > 0) {
		std::vector<std::string>::iterator it = partner_ids.begin();
		json += std::string("{\"partner_id\":\"")+(*it++)+"\"}";
		while (it != partner_ids.end()) {
			json += std::string(", {\"partner_id\":\"")+(*it++)+"\"}";
		}
	}
	json += std::string("], ");

	sprintf(buffer, "\"Image\":[]}");
	json += buffer;


	return json;
}

#include "HttpPostThread.h"

using namespace PSL;

int GameRecord::insert_to_database()
{
	testApp* app = (testApp*)ofGetAppPtr();
	variable str = app->psl->get("databse_api");
	std::string api_url = str;

	Poco::UTF8Encoding utf8;
	Poco::UTF16Encoding utf16;

	std::string json = get_json_string();
	std::string json_enc;

	//Poco::TextConverter converter(utf16, utf8);
	//converter.convert(json, json_enc);

	post_t.post(api_url, json.c_str());
	//post_t.post(api, json_enc.c_str());
	return  0;
}

int GameRecord::get_messages(std::vector<std::string> &out)
{
	if (background != "")
		out.push_back(std::string("BACKGROUND ")+background);
	if (color != "")
		out.push_back(std::string("PLAYER-COLOR ")+color);
	if (style != "")
		out.push_back(std::string("PLAYER-STYLE ")+style);
	if (comment != "")
		out.push_back(std::string("COMMENT ")+comment);	
	if (partner_ids.size() > 0) {
		std::vector<std::string>::iterator it = partner_ids.begin();
		while (it != partner_ids.end())
			out.push_back(std::string("PARTNER ")+(*it++));
	}
	/*
	if (tags.size() > 0) {
		std::vector<std::string>::iterator it = tags.begin();
		while (it != tags.end())
			ret.push_back(std::string("TAGS ")+(*it++));
	}
	*/
	out.push_back(std::string("IDENT ")+player_id+" "+game_id);
	return 0;
}

