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

	// データベースに登録するためのjsonを構築する
	std::string get_json_string();
	int insert_to_database();

	// CLIENTに送るためのメッセージを構築する
	int get_messages(std::vector<std::string> &out);

	HttpPostThread post_t;
	std::string error_content;
};

#endif

	/* 
-- テーブル:`records`
-- インデックス: register_date, delete_flag, modified, created
-- ビュー: v_records ( = SELECT * FROM records WHERE flag = 0)
	`id` bigint unsigned NOT NULL AUTO_INCREMENT COMMENT '記録ID',
	`user_id` bigint unsigned COMMENT 'UserID',
	`player_id` varchar(255) COMMENT '選手番号',
	`record_id` varchar(255) COMMENT '記録のQRコード',
	`movie_path` text COMMENT '動画データのパス',
	`movie_length` text COMMENT '動画の長さ',
	`register_date` timestamp COMMENT '記録された日時',
	`data` text COMMENT '記録されたタイムマシンの記録',
	`tags` text COMMENT 'カンマ区切りでタグ付け',
	`comment` text COMMENT 'コメント',
	`pattern` varchar(64) COMMENT '再生するときの模様セット番号',
	`sound` varchar(64) COMMENT '再生するときの音のセット番号',
	`background` varchar(64) COMMENT '再生するときの背景セット番号',
	`card_image_id` int unsigned COMMENT 'カードのスキャンイメージ',

	`delete_flag` tinyint(1) NOT NULL DEFAULT 0,
	`modified` datetime NOT NULL,
	`created` datetime NOT NULL,
	`create_user` integer DEFAULT 0,
	PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;


-- テーブル:`partners`
-- デフォルト: `partners` AUTO_INCREMENT = 1;
-- インデックス: delete_flag, modified, created
-- ビュー: v_partners ( = 'SELECT * FROM partners WHERE flag = 0')
	`id` bigint unsigned NOT NULL AUTO_INCREMENT COMMENT 'パートナーID',
	`record_id` bigint unsigned COMMENT '記録ID',
	`partner_id` varchar(255) COMMENT '一緒に走った相手のID',
	`delete_flag` tinyint(1) NOT NULL DEFAULT 0,
	`modified` datetime NOT NULL,
	`created` datetime NOT NULL,
	`create_user` integer DEFAULT 0,
	PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci;

*/


