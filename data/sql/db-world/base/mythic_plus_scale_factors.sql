-- 最后更新：2021/08/15
-- 描述：用于规范跨扩展地下城难度的Mythic+地下城的比例因子
DROP TABLE IF EXISTS mythic_plus_scale_factors;
CREATE TABLE IF NOT EXISTS mythic_plus_scale_factors (
    mapId SMALLINT PRIMARY KEY,
    dmg_bonus INT,
    spell_bonus INT,
    hp_bonus INT,
    difficulty  INT,
    max INT
);

-- 1. 60级前地下城 (13 dmg_bonus, 2 hp_bonus, max 23, difficulty 3)
INSERT INTO mythic_plus_scale_factors (mapId, dmg_bonus, spell_bonus, hp_bonus, difficulty, max)
VALUES
    (389, 22, 19,2, 3, 23),  -- 怒焰裂谷
    (43,  19, 18,2, 3, 23),  -- 哀嚎洞穴
    (36,  19, 19,2, 3, 23),  -- 死亡矿井
    (33,  19, 19,2, 3, 23),  -- 影牙城堡
    (34,  19, 19,2, 3, 23),  -- 监狱
    (48,  19, 19,2, 3, 23),  -- 黑暗深渊
    (90,  19, 19,2, 3, 23),  -- 诺莫瑞根
    (47,  19, 19,2, 3, 23),  -- 剃刀沼泽
    (189, 19, 19,2, 3, 23),  -- 血色修道院（墓地）
    (129, 19, 19,2, 3, 23),  -- 剃刀高地
    (70,  19, 19,2, 3, 23),  -- 奥达曼
    (209, 19, 19,2, 3, 23),  -- 祖尔法拉克
    (349, 19, 19,2, 3, 23)   -- 玛拉顿
ON DUPLICATE KEY UPDATE mapId = mapId;

-- 2. 经典旧世60级地下城 (15 dmg_bonus, 3 hp_bonus, max 25, difficulty 3)
INSERT INTO mythic_plus_scale_factors (mapId, dmg_bonus, spell_bonus, hp_bonus, difficulty, max)
VALUES
    (289, 17, 20,3, 3, 25),  -- 通灵学院
    (109, 17, 20,3, 3, 25),  -- 沉没的神庙
    (329, 17, 20,3, 3, 25),  -- 斯坦索姆
    (229, 17, 20,3, 3, 25),  -- 黑石塔（下层）
    (230, 17, 20,3, 3, 25),  -- 黑石塔（上层）
    (429, 17, 20,3, 3, 25),  -- 厄运之槌
    (269, 17, 20,3, 3, 25)   -- 阿塔哈卡神庙
ON DUPLICATE KEY UPDATE mapId = mapId;

-- 3. 燃烧的远征70级前地下城 (15 dmg_bonus, 4 hp_bonus, max 26, difficulty 3)
INSERT INTO mythic_plus_scale_factors (mapId, dmg_bonus, spell_bonus, hp_bonus, difficulty, max)
VALUES
    (542, 16, 14,4, 3, 26),  -- 地狱火堡垒：血熔炉
    (543, 16, 14,4, 3, 26),  -- 地狱火堡垒：城墙
    (545, 16, 14,4, 3, 26),  -- 盘牙水库：蒸汽地窟
    (546, 16, 14,4, 3, 26),  -- 盘牙水库：幽暗沼泽
    (547, 16, 14,4, 3, 26),  -- 盘牙水库：幽暗沼泽
    (557, 16, 14,4, 3, 26),  -- 奥金顿：法力陵墓
    (558, 16, 14,4, 3, 26),  -- 奥金顿：奥金尼地穴
    (560, 16, 14,4, 3, 26)   -- 时光之穴：旧希尔斯布莱德丘陵
ON DUPLICATE KEY UPDATE mapId = mapId;

-- 4. 燃烧的远征70级地下城 (14 dmg_bonus, 4 hp_bonus, max 29, difficulty 3)
INSERT INTO mythic_plus_scale_factors (mapId, dmg_bonus, spell_bonus, hp_bonus, difficulty, max)
VALUES
    (540, 14, 13,4, 3, 29),  -- 破碎大厅
    (556, 14, 13,4, 3, 29),  -- 奥金顿：塞泰克大厅
    (555, 14, 13,4, 3, 29),  -- 奥金顿：暗影迷宫
    (553, 14, 13,4, 3, 29),  -- 风暴要塞：植物学
    (554, 14, 13,4, 3, 29),  -- 风暴要塞：生态船
    (552, 14, 13,4, 3, 29),  -- 风暴要塞：禁魔监狱
    (585, 14, 13,4, 3, 29)   -- 魔导师平台
ON DUPLICATE KEY UPDATE mapId = mapId;

-- 5. 巫妖王之怒80级前地下城 (17 dmg_bonus, 3 hp_bonus, max 30, difficulty 3)
INSERT INTO mythic_plus_scale_factors (mapId, dmg_bonus, spell_bonus, hp_bonus, difficulty, max)
VALUES
    (574, 19, 12,3, 3, 30),  -- 乌特加德城堡
    (619, 19, 12,3, 3, 30),  -- 安卡赫特：古代王国
    (576, 19, 12,3, 3, 30),  -- 魔枢
    (600, 19, 12,3, 3, 30),  -- 达克萨隆要塞
    (601, 19, 12,3, 3, 30),  -- 艾卓-尼鲁布
    (608, 19, 12,3, 3, 30)   -- 紫罗兰监狱
ON DUPLICATE KEY UPDATE mapId = mapId;

-- 6. 巫妖王之怒80级地下城 (19 dmg_bonus, 4 hp_bonus, max 33, difficulty 3)
INSERT INTO mythic_plus_scale_factors (mapId, dmg_bonus, spell_bonus, hp_bonus, difficulty, max)
VALUES
    (595, 19, 13,5, 3, 33),  -- 净化斯坦索姆
    (604, 19, 13,5, 3, 33),  -- 古达克
    (599, 19, 13,5, 3, 33),  -- 石之大厅
    (602, 19, 13,5, 3, 33),  -- 闪电大厅
    (578, 19, 13,5, 3, 33),  -- 奥核之眼
    (650, 19, 13,5, 3, 33),  -- 冠军的试炼
    (632, 19, 13,5, 3, 33),  -- 灵魂洪炉
    (658, 19, 13,5, 3, 33),  -- 破碎大厅
    (668, 19, 13,5, 3, 33)   -- 映像大厅
ON DUPLICATE KEY UPDATE mapId = mapId;
