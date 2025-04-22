/*
 Navicat Premium Dump SQL

 Source Server         : 127.0.0.1
 Source Server Type    : MySQL
 Source Server Version : 80040 (8.0.40)
 Source Host           : localhost:3306
 Source Schema         : acore_world

 Target Server Type    : MySQL
 Target Server Version : 80040 (8.0.40)
 File Encoding         : 65001

 Date: 22/04/2025 15:37:11
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for mythic_difficulties
-- ----------------------------
DROP TABLE IF EXISTS `mythic_difficulties`;
CREATE TABLE `mythic_difficulties`  (
  `difficulty_id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `difficulty_name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `dungeon_health` float NOT NULL,
  `dungeon_melee` float NOT NULL,
  `dungeon_base_damage` float NOT NULL,
  `dungeon_spell` float NOT NULL,
  `dungeon_armor` float NOT NULL,
  `dungeon_avg_level` tinyint UNSIGNED NOT NULL,
  `boss_health` float NOT NULL,
  `boss_melee` float NOT NULL,
  `boss_base_damage` float NOT NULL,
  `boss_spell` float NOT NULL,
  `boss_armor` float NOT NULL,
  `boss_avg_level` tinyint UNSIGNED NOT NULL,
  `death_allowance` int UNSIGNED NOT NULL,
  `item_offset` int UNSIGNED NOT NULL,
  PRIMARY KEY (`difficulty_id`) USING BTREE,
  UNIQUE INDEX `name`(`difficulty_name` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 12 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of mythic_difficulties
-- ----------------------------
INSERT INTO `mythic_difficulties` VALUES (3, 'm3', 2, 1.5, 1.8, 2, 1.3, 70, 3, 2.5, 2.8, 3, 2, 75, 30, 10);
INSERT INTO `mythic_difficulties` VALUES (4, 'm4', 2, 1.5, 1.8, 2, 1.3, 70, 3, 2.5, 2.8, 3, 2, 75, 30, 10);
INSERT INTO `mythic_difficulties` VALUES (5, 'm5', 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
INSERT INTO `mythic_difficulties` VALUES (6, 'm7', 2, 1.5, 1.8, 2, 1.3, 70, 3, 2.5, 2.8, 3, 2, 75, 30, 10);

SET FOREIGN_KEY_CHECKS = 1;
