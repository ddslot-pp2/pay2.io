sudo apt-get install libmysql++-dev
sudo apt-get install libmysqlcppconn-dev

------------------------------------------------------------


CREATE DEFINER=`root`@`localhost` PROCEDURE `insert_user`(IN in_name VARCHAR(40), IN in_password VARCHAR(20), IN in_facebook_id VARCHAR(40))
BEGIN	
    /*DECLARE uid_count INT;*/	

    
    IF (not exists(select name from user_info where name = in_name)) THEN
		/*SELECT "NOT EXIST";*/
		insert into user_info(name, password, facebook_id) values(in_name, in_password, in_facebook_id);
        select uid from user_info where name = in_name;
    ELSE
		SELECT "EXIST";
	END IF;
    
    
END

------------------------------------------------------------


create table user_info(uid BIGINT not null auto_increment primary key, name varchar(40) not null, password varchar(20) not null, facebook_id varchar(40));

ALTER TABLE `pay2`.`user_info` 
CHANGE COLUMN `name` `name` varchar(40) NOT NULL COMMENT '' ;
