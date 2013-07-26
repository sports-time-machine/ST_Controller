#ifndef _ST_GAME_RECORD_H_
#define _ST_GAME_RECORD_H_

#include "ofMain.h"

#include "HttpPostThread.h"

class GameRecord {

public:
	std::string game_id;				// DB:records record_id
	std::string player_id;				// DB:records player_id
	std::vector<std::string> partner_ids;	// DB:partners (record_id -> partner_id)
	std::vector<std::string> tags;			// DB:records tags (separated by commma)
	std::string background;				// DB:records background
	std::string color;					// DB:records pattern
	std::string style;
	std::string comment;				// DB:records comment
										// DB:records user_id ????????
	std::string movie_length;					// DB:records movie_length
	std::string movie_path;					// DB:records movie_path
	std::string register_date;					// DB:records register_data
	std::string sound;					// DB:records sound


	GameRecord();

	void setGameId(std::string game_id);
	void setPlayerId(std::string player_id);
	void addPartnerID(std::string partner_id);
	void addTag(std::string tag);
	void setBackground(std::string background);
	void setColor(std::string color);
	void setStyle(std::string style);
	void setComment(std::string commenta);

	// �f�[�^�x�[�X�ɓo�^���邽�߂�json���\�z����
	std::string get_json_string();
	int insert_to_database();

	// CLIENT�ɑ��邽�߂̃��b�Z�[�W���\�z����
	int get_messages(std::vector<std::string> &out);

	HttpPostThread post_t;
	std::string error_content;
};

#endif

	/* 
-- �e�[�u��:`records`
-- �C���f�b�N�X: register_date, delete_flag, modified, created
-- �r���[: v_records ( = SELECT * FROM records WHERE flag = 0)
	`id` bigint unsigned NOT NULL AUTO_INCREMENT COMMENT '�L�^ID',
	`user_id` bigint unsigned COMMENT 'UserID',
	`player_id` varchar(255) COMMENT '�I��ԍ�',
	`record_id` varchar(255) COMMENT '�L�^��QR�R�[�h',
	`movie_path` text COMMENT '����f�[�^�̃p�X',
	`movie_length` text COMMENT '����̒���',
	`register_date` timestamp COMMENT '�L�^���ꂽ����',
	`data` text COMMENT '�L�^���ꂽ�^�C���}�V���̋L�^',
	`tags` text COMMENT '�J���}��؂�Ń^�O�t��',
	`comment` text COMMENT '�R�����g',
	`pattern` varchar(64) COMMENT '�Đ�����Ƃ��̖͗l�Z�b�g�ԍ�',
	`sound` varchar(64) COMMENT '�Đ�����Ƃ��̉��̃Z�b�g�ԍ�',
	`background` varchar(64) COMMENT '�Đ�����Ƃ��̔w�i�Z�b�g�ԍ�',
	`card_image_id` int unsigned COMMENT '�J�[�h�̃X�L�����C���[�W',

	`delete_flag` tinyint(1) NOT NULL DEFAULT 0,
	`modified` datetime NOT NULL,
	`created` datetime NOT NULL,
	`create_user` integer DEFAULT 0,
	PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;


-- �e�[�u��:`partners`
-- �f�t�H���g: `partners` AUTO_INCREMENT = 1;
-- �C���f�b�N�X: delete_flag, modified, created
-- �r���[: v_partners ( = 'SELECT * FROM partners WHERE flag = 0')
	`id` bigint unsigned NOT NULL AUTO_INCREMENT COMMENT '�p�[�g�i�[ID',
	`record_id` bigint unsigned COMMENT '�L�^ID',
	`partner_id` varchar(255) COMMENT '�ꏏ�ɑ����������ID',
	`delete_flag` tinyint(1) NOT NULL DEFAULT 0,
	`modified` datetime NOT NULL,
	`created` datetime NOT NULL,
	`create_user` integer DEFAULT 0,
	PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

*/


