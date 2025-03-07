#include "CharacterDatabase.h"
#include "MpDataStore.h"
#include "Group.h"
#include "MpLogger.h"
#include "Chat.h"

// Adds an entry for the group difficult to memory and updats database
void MpDataStore::AddGroupData(Group *group, MpGroupData groupData) {
    if(!group) {
        MpLogger::error("AddGroupData called with null group pointer");
        return;
    }

    ObjectGuid guid = group->GetGUID();

    if (!guid) {
        MpLogger::error("AddGroupData called with invalid group GUID");
        return;
    }

    if (!groupData.difficulty) {
        MpLogger::error("AddGroupData called with invalid difficulty");
        return;
    }

    Player* leader = group->GetLeader();
    if(!leader) {
        MpLogger::error("AddGroupData called with null group leader");
        return;
    }

    Map* map = leader->GetMap();
    if (!map) {
        MpLogger::error("AddGroupData called with null map for group leader");
        return;
    }

    auto instance = map->ToInstanceMap();

    // if we already have data override it
    if (_groupData->contains(guid)) {

        MpGroupData existingData = _groupData->at(guid);
        if(groupData.difficulty == MP_DIFFICULTY_HEROIC || groupData.difficulty == MP_DIFFICULTY_NORMAL || groupData.difficulty != existingData.difficulty) {

            // if we set a lower difficulty and we are in an instance we need to kick the group out and reset the instance.
            if(instance && instance->HavePlayers()) {

                instance->Reset(2); // 2 = reset all

                const Map::PlayerList players = map->GetPlayers();
                for (auto itr = players.begin(); itr != players.end(); ++itr) {
                    Player* player = itr->GetSource();

                    if(!player) {
                        MpLogger::error("AddGroupData called with null player in instance");
                    }

                    ChatHandler(player->GetSession()).SendNotification("The group leader has changed the difficulty setting. You have been removed from the instance.");
                }
            }

        }

        _groupData->at(guid) = groupData;

    } else {

        if(instance && instance->HavePlayers()) {

            instance->Reset(2); // 2 = reset all

            const Map::PlayerList players = map->GetPlayers();
            for (auto itr = players.begin(); itr != players.end(); ++itr) {
                Player* player = itr->GetSource();

                if(!player) {
                    MpLogger::error("AddGroupData called with null player in instance");
                }

                ChatHandler(player->GetSession()).SendNotification("The group leader has changed the difficulty setting. You have been removed from the instance.");
            }
        }

        _groupData->emplace(guid, groupData);

    }
    const Group::MemberSlotList members = group->GetMemberSlots();
    for (const auto &memberSlot : members) {
        Player* player = ObjectAccessor::FindPlayer(memberSlot.guid);
        if (player) {

            DBUpdatePlayerInstanceData(player->GetGUID(), groupData.difficulty);
        }
    }
}

MpGroupData* MpDataStore::GetGroupData(Group* group) {

    if (auto it = _groupData->find(group->GetGUID()); it != _groupData->end()) {
        return &(it->second);
    }

    return nullptr;
}

/**
 * This keeps track of instance keys (mapId, instanceId) for a group this can be used to
 * reset instance settings in the instanceData store when a difficulty is changed by the group leader.
 */
void MpDataStore::PushGroupInstanceKey(Group *group, uint32 mapId, uint32 instanceId) {
    ObjectGuid guid = group->GetGUID();

    if (!guid) {
        MpLogger::error("PushGroupInstanceKey called with invalid group GUID");
        return;
    }

    if (!mapId || !instanceId) {
        MpLogger::error("PushGroupInstanceKey called with invalid mapId or instanceId");
        return;
    }

    if (!_groupData->contains(guid)) {
        MpLogger::error("PushGroupInstanceKey called with invalid group GUID");
        return;
    }

    // Need to potentialy reset here?
}

// This clears out any group data from memory and the database
void MpDataStore::RemoveGroupData(Group *group) {
    MpLogger::debug("RemoveGroupData for group {}", group->GetGUID().GetCounter());
    _groupData->erase(group->GetGUID());

    CharacterDatabase.Execute("DELETE FROM group_difficulty WHERE guid = {}) ", group->GetGUID().GetCounter());
}

void MpDataStore::AddPlayerData(ObjectGuid guid, MpPlayerData* pd) {
    MpLogger::debug("AddPlayerData for player {}", guid.GetCounter());
    _playerData->emplace(guid, pd);

    // get the player
    Player* player = ObjectAccessor::FindPlayer(guid);
}

void MpDataStore::RemovePlayerData(ObjectGuid guid) {
    MpLogger::debug("RemovePlayerData for player {}", guid.GetCounter());
    _playerData->erase(guid);
}

void MpDataStore::ResetPlayerData(ObjectGuid guid) {
    MpLogger::debug("ResetPlayerData for player {}", guid.GetCounter());
    _playerData->erase(guid);
}

void MpDataStore::AddInstanceData(uint32 mapId, uint32 instanceId, MpInstanceData instanceSettings) {
    _instanceData->emplace(GetInstanceDataKey(mapId, instanceId), instanceSettings);
}

MpInstanceData* MpDataStore::GetInstanceData(uint32 mapId, uint32 instanceId) {

    if (!_instanceData->contains(GetInstanceDataKey(mapId, instanceId))) {
        return nullptr;
    }

    return &_instanceData->at(GetInstanceDataKey(mapId, instanceId));
}

void MpDataStore::RemoveInstanceData(uint32 mapId, uint32 instanceId) {
    _instanceData->erase(GetInstanceDataKey(mapId, instanceId));
}

void MpDataStore::AddCreatureData(ObjectGuid guid, MpCreatureData creatureData) {
    // MpLogger::debug("AddInstanceCreatureData for creature {}", guid.GetCounter());
    _instanceCreatureData->emplace(guid, creatureData);
}

MpCreatureData* MpDataStore::GetCreatureData(ObjectGuid guid) {
    if (!_instanceCreatureData->contains(guid)) {
        return nullptr;
    }

    return &_instanceCreatureData->at(guid);
}

std::vector<MpCreatureData*> MpDataStore::GetInstanceCreatures(uint32 mapId, uint32 instanceId) {
    std::vector<MpCreatureData*> creatures;
    for (auto& [guid, creatureData] : *_instanceCreatureData) {
        Creature* creature = creatureData.creature;

        if (creature->GetMapId() == mapId && creature->GetInstanceId() == instanceId) {
            creatures.push_back(&creatureData);
        }
    }
    return creatures;
}

 std::vector<MpCreatureData*> MpDataStore::GetUnscaledCreatures(uint32 mapId, uint32 instanceId) {
    std::vector<MpCreatureData*> creatures;
    for (auto& [guid, creatureData] : *_instanceCreatureData) {
        Creature* creature = creatureData.creature;

        if (creature->GetMapId() == mapId && creature->GetInstanceId() == instanceId && !creatureData.IsScaled()) {
            creatures.push_back(&creatureData);
        }
    }
    return creatures;
 }

void MpDataStore::RemoveCreatureData(ObjectGuid guid) {
    // MpLogger::debug("RemoveInstanceCreatureData data for creature {}", guid.GetCounter());
    _instanceCreatureData->erase(guid);
}

MpScaleFactor MpDataStore::GetScaleFactor(int32 mapId, int32 difficulty) const {

    auto key = GetScaleFactorKey(mapId, difficulty);
    if (_scaleFactors->contains(key)) {
        return _scaleFactors->at(key);
    }

    return MpScaleFactor{
        .dmgBonus = 3,
        .healthBonus = 2,
        .maxDamageBonus = 30
    };
}

int32 MpDataStore::GetHealthScaleFactor(int32 mapId, int32 difficulty) const {
    return GetScaleFactor(mapId, difficulty).healthBonus;
}

int32 MpDataStore::GetDamageScaleFactor(int32 mapId, int32 difficulty) const {
    return GetScaleFactor(mapId, difficulty).dmgBonus;
}

int32 MpDataStore::GetSpellScaleFactor(int32 mapId, int32 difficulty) const {
    return GetScaleFactor(mapId, difficulty).spellBonus;
}

int32 MpDataStore::GetMaxDamageScaleFactor(int32 mapId, int32 difficulty) const {
    return GetScaleFactor(mapId, difficulty).maxDamageBonus;
}

void MpDataStore::SetHealthScaleFactor(int32 mapId, int32 difficulty, int32 newValue) {
    auto key = GetScaleFactorKey(mapId, difficulty);
    if (_scaleFactors && _scaleFactors->contains(key)) {
        _scaleFactors->at(key).healthBonus = newValue;
    }
}

void MpDataStore::SetDamageScaleFactor(int32 mapId, int32 difficulty, int32 newValue) {
    auto key = GetScaleFactorKey(mapId, difficulty);

    if (_scaleFactors && _scaleFactors->contains(key)) {
        _scaleFactors->at(key).dmgBonus = newValue;
    }
}

void MpDataStore::SetSpellScaleFactor(int32 mapId, int32 difficulty, int32 newValue) {
    auto key = GetScaleFactorKey(mapId, difficulty);
    if (_scaleFactors && _scaleFactors->contains(key)) {
        _scaleFactors->at(key).spellBonus = newValue;
    }
}

int32 MpDataStore::LoadScaleFactors() {
    _scaleFactors->clear();

    //                                                 0       1          2              3        4        5
    QueryResult result = WorldDatabase.Query("SELECT mapId, dmg_bonus, spell_bonus, hp_bonus, difficulty, max FROM mythic_plus_scale_factors");
    if (!result) {
        MpLogger::error("Failed to load mythic scale factors from database");
        return 0;
    }

    do {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        int32 damageBonus = fields[1].Get<int32>();
        int32 spellBonus = fields[2].Get<int32>();
        int32 healthBonus = fields[3].Get<int32>();
        int32 difficulty = fields[4].Get<int32>();
        int32 maxDamageBonus = fields[5].Get<int32>();

        MpScaleFactor scaleFactor = {
            .dmgBonus = damageBonus,
            .healthBonus = healthBonus,
            .spellBonus = spellBonus,
            .maxDamageBonus = maxDamageBonus
        };

        _scaleFactors->emplace(GetScaleFactorKey(mapId, difficulty), scaleFactor);

    } while (result->NextRow());

    return int32(_scaleFactors->size());
}

/**
 * Database Calls below for storing player data.
 * @todo refactor to use prepared statements
*/
void MpDataStore::DBUpdatePlayerInstanceData(ObjectGuid playerGuid, MpDifficulty difficulty, uint32 mapId, uint32 instanceId, uint32 deaths) {
    if (!playerGuid) {
        MpLogger::error("DBAddPlayerData called with invalid playerData");
        return;
    }

    CharacterDatabase.Execute("REPLACE INTO mp_player_instance_data (guid, difficulty, mapId, instanceId, deaths) VALUES ({},{},{},{},{}) ",
        playerGuid.GetCounter(),
        difficulty,
        mapId,
        instanceId,
        deaths
    );
}

void MpDataStore::DBResetPlayerDeaths(Player* player) {
    if (!player) {
        MpLogger::error("DBUpdateDeaths called with invalid playerId");
        return;
    }

    CharacterDatabase.Execute("UPDATE mp_player_instance_data SET deaths = 0 WHERE guid = {} and mapId = {} and instanceId = {}",
        player->GetGUID().GetCounter(),
        player->GetMapId(),
        player->GetInstanceId()
    );
}

void MpDataStore::DBAddPlayerDeath(Player* player) {
    if (!player) {
        MpLogger::error("DBAddPlayerDeath called with invalid player");
        return;
    }
    CharacterDatabase.Execute("UPDATE mp_player_instance_data SET deaths = deaths + 1 WHERE guid = {} and mapId = {} and instanceId = {}",
        player->GetGUID().GetCounter(),
        player->GetMapId(),
        player->GetInstanceId()
    );
}

// Logs death for player that occurs by a creature directly.
void MpDataStore::DBAddPlayerDeath(Player* player, Creature* creature, MpDifficulty difficulty) {
    if (!player) {
        MpLogger::error("DBAddPlayerDeath called with invalid player");
        return;
    }

    CharacterDatabase.Execute("UPDATE mp_player_instance_data SET deaths = deaths + 1 WHERE guid = {} and mapId = {} and instanceId = {}",
        player->GetGUID().GetCounter(),
        player->GetMapId(),
        player->GetInstanceId()
    );

    CharacterDatabase.Execute(
        "INSERT INTO mp_player_death_stats (guid, creatureEntry, difficulty, numDeaths, lastUpdated) "
        "VALUES ({}, {}, {}, 1, CURRENT_TIMESTAMP) "
        "ON DUPLICATE KEY UPDATE numDeaths = numDeaths + 1, lastUpdated = CURRENT_TIMESTAMP",
        player->GetGUID().GetCounter(),
        creature->GetEntry(),
        difficulty
    );
}

void MpDataStore::DBUpdateGroupData(ObjectGuid groupGuid, MpDifficulty difficulty, uint32 mapId, uint32 instanceId, uint32 deaths) {
    if (!groupGuid) {
        MpLogger::error("DBUpdateGroupData called with invalid groupGuid");
        return;
    }
    CharacterDatabase.Execute("REPLACE INTO mp_group_data (groupId, difficulty, mapId, instanceId, deaths) VALUES ({},{},{},{},{}) ",
        groupGuid.GetCounter(),
        difficulty,
        mapId,
        instanceId,
        deaths
    );
}

void MpDataStore::DBAddGroupDeath(Group* group, uint32 mapId, uint32 instanceId, MpDifficulty difficulty) {
    if (!group) {
        MpLogger::error("DBAddGroupDeath called with invalid group");
        return;
    }

    if(!difficulty) {
        MpLogger::error("DBAddGroupDeath called with invalid difficulty");
        return;
    }

    if(!mapId || !instanceId) {
        MpLogger::error("DBAddGroupDeath called with invalid mapId or instanceId");
        return;
    }

    CharacterDatabase.Execute("UPDATE mp_group_data SET deaths = deaths + 1 WHERE groupId = {} and mapId = {} and instanceId = {} and difficulty = {}",
        group->GetGUID().GetCounter(),
        mapId,
        instanceId,
        difficulty
    );
}

void MpDataStore::DBRemovePlayerData(ObjectGuid playerGuid) {
    if (!playerGuid) {
        MpLogger::error("DBRemovePlayerData called with invalid playerGuid");
        return;
    }

    CharacterDatabase.Execute("DELETE FROM mp_player_instance_data WHERE guid = {} ", playerGuid.GetCounter());
}

void MpDataStore::DBRemovePlayerInstanceData(uint32 instanceId) {
    if (!instanceId) {
        MpLogger::error("DBRemovePlayerInstanceData: missing instanceId to remove player instance ");
        return;
    }

    CharacterDatabase.Execute("DELETE FROM mp_player_instance_data WHERE instanceId = {} ", instanceId);
}


void MpDataStore::DBUpdateGroupTimerDeaths(ObjectGuid groupGuid, uint32 mapId, uint32 instanceId, uint32 timer, uint32 deaths) {
    if (!groupGuid) {
        MpLogger::error("DBUpdateGroupTimerDeaths called with invalid groupGuid");
        return;
    }

    CharacterDatabase.Execute("REPLACE INTO mp_group_data (groupId, mapId, instanceId, instanceTimer, deaths) VALUES ({},{},{},{},{}) ",
        groupGuid.GetCounter(),
        mapId,
        instanceId,
        timer,
        deaths
    );
}

void MpDataStore::DBRemoveGroupData(ObjectGuid groupGuid) {
    if (!groupGuid) {
        MpLogger::error("DBRemoveGroupData called with invalid groupGuid");
        return;
    }

    CharacterDatabase.Execute("DELETE FROM mp_group_data WHERE groupId = {} ", groupGuid.GetCounter());
}

// Remove instance data using the instanceId
void MpDataStore::DBRemoveGroupInstanceData(uint32 instanceId) {
    if (!instanceId) {
        MpLogger::error("DBRemoveGroupData called with invalid groupGuid");
        return;
    }

    CharacterDatabase.Execute("DELETE FROM mp_group_data WHERE instanceId = {} ", instanceId);
}

