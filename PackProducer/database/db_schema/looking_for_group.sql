/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

CREATE DATABASE IF NOT EXISTS `looking_for_group` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;
USE `looking_for_group`;

CREATE TABLE IF NOT EXISTS `user` (
  `usr_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `usr_first_name` varchar(100) NOT NULL,
  `usr_last_name` varchar(100) NOT NULL,
  `usr_username` varchar(150) NOT NULL,
  `usr_email` varchar(150) NOT NULL,
  `usr_avatar` varchar(150) DEFAULT NULL,
  `usr_location` varchar(150) DEFAULT NULL,
  `usr_salt` varchar(100) NOT NULL,
  `usr_password` varchar(255) NOT NULL,
  PRIMARY KEY (`usr_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8mb4;

DELETE FROM `user`;
INSERT INTO `user` (`usr_id`, `usr_first_name`, `usr_last_name`, `usr_username`, `usr_email`, `usr_avatar`, `usr_location`, `usr_salt`, `usr_password`) VALUES
	(1, 'John', 'Cola', 'jcola4', 'jcola4@ncsu.edu', NULL, NULL, '357638792F423F4528472B4B6250655368566D597133743677397A2443264629', '04d0af0083f67d9ee86cd782677b4344a1fa53753f6d0e34200d18bc5ce8d7846019bc0369920eacab6e554d32007702151e1acf79ca738423c6110b966ec645'),
	(2, 'Joe', 'Smith', 'jsmith12', 'jsmith12@ncsu.edu', NULL, NULL, '423F4528482B4D6251655468576D5A7134743777217A24432646294A404E6352', 'e5aaecbba75374831b4ffe81edec29aa67a83d66d8bdd58b2343093b4e841862bebaa4c625e125b83f3f43cef68076d3066734e76229b2c00b21f83d5e73077b'),
  (3, 'Bryanne', 'Cooper', 'bcooper2', 'bcooper2@ncsu.edu', NULL, NULL, '3F4528482B4D6251655468576D5A7134743777217A24432646294A404E635266', '3b40b41889a6e456f6cef7edcf303eb195684d20ed6bffaa28c7c88db598d8bc3a179f79943983ddcd59482bdd4a1b18cf00860126dd03b338406d0a6bf9ff76'),
  (4, 'Oanez', 'Merry', 'omerry', 'omerry@ncsu.edu', NULL, NULL, '51655468576D5A7134743777397A24432646294A404E635266556A586E327235', 'db209ad4fbcbc5fbdbf0e8eab20a878cea5b290b34063de38a9ce34d6dd1549faee1aec557ea4dbf8f8630ad72102f200164d93a403344c00dbc6e89364c45bc'),
  (5, 'Hazel', 'Lilian', 'hlilian', 'hlilian@ncsu.edu', NULL, NULL, '792442264528482B4D6251655468576D5A7134743777217A25432A462D4A404E', '5b694c3f4f2fd2f04b71e9f94e0d90f590dfe2b3434f2ecae0c0bb656ed7138e542a9b4ab3301680a975679cfb79fb126ad31f8ab03e9d73f0a0bf29ce7e3a9a');

CREATE TABLE IF NOT EXISTS `post` (
  `pst_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `pst_authorId` int(11) unsigned NOT NULL,
  `pst_title` varchar(100) NOT NULL,
  `pst_description` varchar(500) NOT NULL,
  `pst_location` varchar(150) DEFAULT NULL,
  `pst_event_datetime` varchar(30) NOT NULL,
  `pst_post_datetime` varchar(30) NOT NULL,
  PRIMARY KEY (`pst_id`),
  KEY `FK_PST_ATR` (`pst_authorId`),
  CONSTRAINT `FK_PST_ATR` FOREIGN KEY (`pst_authorId`) REFERENCES `user` (`usr_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8mb4;

DELETE FROM `post`;
INSERT INTO `post` (`pst_id`, `pst_authorId`, `pst_title`, `pst_description`, `pst_location`, `pst_event_datetime`, `pst_post_datetime`) VALUES
  (1, 1, 'Tour Coca Cola Headquarters', 'Looking for someone to tour the Coca Cola Headquarters with me.', 'Coca Cola Headquarters', '2022-12-12T09:30:00Z', '2022-11-03T12:23:13Z'),
  (2, 5, 'NCSU Football Game', 'Looking for a group of people to join at the home game this saturday', 'Carter-Finley Stadium', '2022-10-22T19:30:00Z', '2022-10-03T15:53:23Z'),
  (3, 2, 'XYZ Concert', 'Looking for anyone who would want to join me in seeing XYZ in concert', 'XYZ Auditorium', '2022-11-26T09:30:00Z', '2022-11-06T16:34:53Z');

CREATE TABLE IF NOT EXISTS `message` (
  `msg_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `msg_senderId` int(11) unsigned NOT NULL,
  `msg_recieverId` int(11) unsigned NOT NULL,
  `msg_text` varchar(500) NOT NULL,
  `msg_datetime` varchar(30) NOT NULL,
  PRIMARY KEY (`msg_id`),
  KEY `FK_MSG_SDR` (`msg_senderId`),
  KEY `FK_MSG_RCP` (`msg_recieverId`),
  CONSTRAINT `FK_MSG_SDR` FOREIGN KEY (`msg_senderId`) REFERENCES `user` (`usr_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_MSG_RCP` FOREIGN KEY (`msg_recieverId`) REFERENCES `user` (`usr_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8mb4;

DELETE FROM `message`;
INSERT INTO `message` (`msg_id`, `msg_senderId`, `msg_recieverId`, `msg_text`, `msg_datetime`) VALUES
  (1, 1, 2, 'Hello! Do you want to go to the event?', '2022-11-04T16:30:03Z'),
  (2, 2, 1, 'Yes! What time is it again?', '2022-11-04T16:45:34Z'),
  (3, 1, 2, '7:30!', '2022-11-04T16:46:23Z');

CREATE TABLE IF NOT EXISTS `tag` (
  `tag_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `tag_name` varchar(100) NOT NULL,
  `tag_icon` varchar(150) NOT NULL,
  PRIMARY KEY (`tag_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8mb4;

DELETE FROM `tag`;
INSERT INTO `tag` (`tag_id`, `tag_name`, `tag_icon`) VALUES
  (1, 'Tickets', 'tickets-icon.png'),
  (2, 'Outing', 'outing-icon.png'),
  (3, 'Concert', 'concert-icon.jpg'),
  (4, 'Movie', 'move-icon.jpg');

CREATE TABLE IF NOT EXISTS `user_tag` (
  `ust_usr_id` int(10) unsigned NOT NULL,
  `ust_tag_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ust_usr_id`,`ust_tag_id`),
  KEY `FK_UST_TAG` (`ust_tag_id`),
  CONSTRAINT `FK_UST_TAG` FOREIGN KEY (`ust_tag_id`) REFERENCES `tag` (`tag_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_UST_USR` FOREIGN KEY (`ust_usr_id`) REFERENCES `user` (`usr_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8mb4;

DELETE FROM `user_tag`;
INSERT INTO `user_tag` (`ust_usr_id`, `ust_tag_id`) VALUES
  (1, 1);

CREATE TABLE IF NOT EXISTS `post_tag` (
  `ptt_pst_id` int(10) unsigned NOT NULL,
  `ptt_tag_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ptt_pst_id`,`ptt_tag_id`),
  KEY `FK_PTT_TAG` (`ptt_tag_id`),
  CONSTRAINT `FK_PTT_TAG` FOREIGN KEY (`ptt_tag_id`) REFERENCES `tag` (`tag_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_PTT_PST` FOREIGN KEY (`ptt_pst_id`) REFERENCES `post` (`pst_id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARACTER SET=utf8mb4;

DELETE FROM `post_tag`;
INSERT INTO `post_tag` (`ptt_pst_id`, `ptt_tag_id`) VALUES
  (1, 1);

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;