-- Used for tracking group instance data for mythic runs
DROP TABLE IF EXISTS mp_group_data;
CREATE TABLE mp_group_data (
    groupId    INT UNSIGNED NOT NULL DEFAULT '0',
    difficulty  INT UNSIGNED,
    mapId       INT UNSIGNED,
    instanceId  INT UNSIGNED,
    instanceTimer INT UNSIGNED,
    deaths        INT UNSIGNED,
    PRIMARY KEY (groupId)
);

-- Used for tracking current instance data for players
DROP TABLE IF EXISTS mp_player_instance_data;
CREATE TABLE mp_player_instance_data(
    guid       INT UNSIGNED NOT NULL DEFAULT '0',
    difficulty INT UNSIGNED NOT NULL DEFAULT '3',
    mapId     INT UNSIGNED NOT NULL,
    instanceId INT UNSIGNED,
    deaths     INT UNSIGNED NOT NULL,

    PRIMARY KEY (guid, mapId, instanceId)
);

-- Used for tracking player deaths to specific creatures in mythic runs
DROP TABLE IF EXISTS mp_player_death_stats;
CREATE TABLE mp_player_death_stats(
  guid          INT UNSIGNED NOT NULL DEFAULT '0',
  creatureEntry INT UNSIGNED NOT NULL,
  difficulty    TINYINT UNSIGNED NOT NULL DEFAULT '0',
  numDeaths     INT UNSIGNED NOT NULL DEFAULT '0',
  lastUpdated   TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (guid, creatureEntry, difficulty)
);

-- Used for tracking player runs in mythic dungeons
DROP TABLE IF EXISTS mp_player_runs;
CREATE TABLE mp_player_runs(
    runId       INT UNSIGNED AUTO_INCREMENT,
    guid        INT UNSIGNED NOT NULL DEFAULT '0',
    difficulty  INT UNSIGNED NOT NULL DEFAULT '3',
    mapId       INT UNSIGNED,
    groupDeaths INT UNSIGNED,
    personalDeaths INT UNSIGNED,
    completeTime   INT UNSIGNED,
    botCount       TINYINT UNSIGNED DEFAULT '0',

    PRIMARY KEY (runId),
    INDEX idx_guid  (guid),
    INDEX idx_mapId (mapId)
);

-- Used for tracking player stats in mythic dungeons
DROP TABLE IF EXISTS mp_player_stats;
CREATE TABLE mp_player_stats (
    guid        INT UNSIGNED     NOT NULL DEFAULT '0',
    mapId      INT UNSIGNED     NOT NULL DEFAULT '0',
    difficulty  TINYINT UNSIGNED NOT NULL DEFAULT '0',
    deaths      INT UNSIGNED     DEFAULT '0',
    runs        INT UNSIGNED     DEFAULT '0',
    completions INT UNSIGNED  DEFAULT '0',
    totalTime      INT UNSIGNED  DEFAULT '0',
    bestTime    INT UNSIGNED  DEFAULT '0',
    PRIMARY KEY (guid, mapId, difficulty)
);

-- Used to enable custom stat upgrades from materials and drops in mythic dungeons
DROP TABLE IF EXISTS mp_player_stat_upgrades;
CREATE TABLE mp_player_stat_upgrades
(
    guid       INT UNSIGNED NOT NULL,
    statTypeId INT UNSIGNED NOT NULL,
    bonus      FLOAT        NOT NULL,
    upgradeRank       INT UNSIGNED NOT NULL,
    materialSpent     INT UNSIGNED NOT NULL DEFAULT '0',
    diceSpent         INT UNSIGNED NOT NULL DEFAULT '0',

    PRIMARY KEY (guid, statTypeId)
);