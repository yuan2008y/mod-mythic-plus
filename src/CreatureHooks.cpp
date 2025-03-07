#include "CreatureHooks.h"
#include "MythicPlus.h"
#include "MpLogger.h"

void CreatureHooks::RegisterJustDied(uint32 entry, CreatureHook<Creature*, Unit*> callback) {
    (*_JustDiedHandlers)[entry].push_back(callback);
}

void CreatureHooks::RegisterOnSpawn(uint32 entry, CreatureHook<Creature*>callback) {
    (*_OnSpawnHandlers)[entry].push_back(callback);
}

void CreatureHooks::RegisterOnAddToInstance(uint32 entry, CreatureHook<Creature*> callback) {
    (*_OnAddToInstanceHandlers)[entry].push_back(callback);
}

// Call health events if the creature's health is at or below the percentage
// void CheckHealthEvents(Creature* creature) {
//     uint32 entry = creature->GetEntry();
//     uint32 currentHealthPct = creature->GetHealthPct();

//     if (_healthPercentEvents->contains(entry)) {
//         for (const auto& [percent, callbacks] : _healthPercentEvents->at(entry)) {
//             if (currentHealthPct <= percent) {
//                 for (auto& callback : callbacks) {
//                     callback(creature); // Trigger custom behavior
//                 }
//             }
//         }
//     }
// }

void CreatureHooks::JustDied(Creature* creature, Unit* killer) {
    if(!creature) {
        MpLogger::debug("JustDied() called with nullptr for creature");
        return;
    }

    if(!killer) {
        MpLogger::debug("JustDied() called with nullptr for killer");
        return;
    }


    uint32 entry = creature->GetEntry();
    if (_JustDiedHandlers->contains(entry)) {
        for (auto& callback : _JustDiedHandlers->at(entry)) {
            MpLogger::debug("JustDied() called for creature: {}", entry);
            callback(creature, killer);
        }
    }
}

void CreatureHooks::JustSpawned(Creature* creature) {
    if(!creature) {
        MpLogger::debug("JustSpawned() called with nullptr for creature");
        return;
    }

    uint32 entry = creature->GetEntry();
    MpInstanceData* instanceData = sMpDataStore->GetInstanceData(creature->GetMapId(), creature->GetInstanceId());

    if(instanceData) {
        sMythicPlus->AddScaledCreature(creature, instanceData);
    }

    if (_OnSpawnHandlers->contains(entry)) {
        for (auto& callback : _OnSpawnHandlers->at(entry)) {
            callback(creature);
            MpLogger::debug("JustSpawned() called in CreatureHook: {}", entry);
        }
    }
}

void CreatureHooks::AddToInstance(Creature* creature) {
    uint32 entry = creature->GetEntry();

    if (_OnAddToInstanceHandlers->contains(entry)) {
        for (auto& callback : _OnAddToInstanceHandlers->at(entry)) {
            callback(creature);
            MpLogger::debug("AddedToInstance() called in CreatureHook: {}", entry);
        }
    }
}
