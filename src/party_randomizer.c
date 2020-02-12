#include "global.h"
#include "pokemon.h"
#include "pokedex.h"
#include "constants/moves.h"
#include "random.h"
#include "constants/species.h"

//intended to be included and used in battle_controllers.c during battle setup

//randomizes the entire player's party
static void rndParty(void);
//randomizes a pokemon
static void rndPkmn(struct Pokemon* mon);

static void EncryptBoxMon(struct BoxPokemon *boxMon)
{
    u32 i;
    for (i = 0; i < 12; i++)
    {
        boxMon->secure.raw[i] ^= boxMon->personality;
        boxMon->secure.raw[i] ^= boxMon->otId;
    }
}

static void DecryptBoxMon(struct BoxPokemon *boxMon)
{
    u32 i;
    for (i = 0; i < 12; i++)
    {
        boxMon->secure.raw[i] ^= boxMon->otId;
        boxMon->secure.raw[i] ^= boxMon->personality;
    }
}

#define SUBSTRUCT_CASE(n, v1, v2, v3, v4)                               \
case n:                                                                 \
    {                                                                   \
    union PokemonSubstruct *substructs0 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs1 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs2 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs3 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs4 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs5 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs6 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs7 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs8 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs9 = boxMon->secure.substructs;    \
    union PokemonSubstruct *substructs10 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs11 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs12 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs13 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs14 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs15 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs16 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs17 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs18 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs19 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs20 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs21 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs22 = boxMon->secure.substructs;   \
    union PokemonSubstruct *substructs23 = boxMon->secure.substructs;   \
                                                                        \
        switch (substructType)                                          \
        {                                                               \
        case 0:                                                         \
            substruct = &substructs ## n [v1];                          \
            break;                                                      \
        case 1:                                                         \
            substruct = &substructs ## n [v2];                          \
            break;                                                      \
        case 2:                                                         \
            substruct = &substructs ## n [v3];                          \
            break;                                                      \
        case 3:                                                         \
            substruct = &substructs ## n [v4];                          \
            break;                                                      \
        }                                                               \
        break;                                                          \
    }                                                                   \


static union PokemonSubstruct *GetSubstruct(struct BoxPokemon *boxMon, u32 personality, u8 substructType)
{
    union PokemonSubstruct *substruct = NULL;

    switch (personality % 24)
    {
        SUBSTRUCT_CASE( 0,0,1,2,3)
        SUBSTRUCT_CASE( 1,0,1,3,2)
        SUBSTRUCT_CASE( 2,0,2,1,3)
        SUBSTRUCT_CASE( 3,0,3,1,2)
        SUBSTRUCT_CASE( 4,0,2,3,1)
        SUBSTRUCT_CASE( 5,0,3,2,1)
        SUBSTRUCT_CASE( 6,1,0,2,3)
        SUBSTRUCT_CASE( 7,1,0,3,2)
        SUBSTRUCT_CASE( 8,2,0,1,3)
        SUBSTRUCT_CASE( 9,3,0,1,2)
        SUBSTRUCT_CASE(10,2,0,3,1)
        SUBSTRUCT_CASE(11,3,0,2,1)
        SUBSTRUCT_CASE(12,1,2,0,3)
        SUBSTRUCT_CASE(13,1,3,0,2)
        SUBSTRUCT_CASE(14,2,1,0,3)
        SUBSTRUCT_CASE(15,3,1,0,2)
        SUBSTRUCT_CASE(16,2,3,0,1)
        SUBSTRUCT_CASE(17,3,2,0,1)
        SUBSTRUCT_CASE(18,1,2,3,0)
        SUBSTRUCT_CASE(19,1,3,2,0)
        SUBSTRUCT_CASE(20,2,1,3,0)
        SUBSTRUCT_CASE(21,3,1,2,0)
        SUBSTRUCT_CASE(22,2,3,1,0)
        SUBSTRUCT_CASE(23,3,2,1,0)
    }

    return substruct;
}

static u16 CalculateBoxMonChecksum(struct BoxPokemon *boxMon)
{
    u16 checksum = 0;
    union PokemonSubstruct *substruct0 = GetSubstruct(boxMon, boxMon->personality, 0);
    union PokemonSubstruct *substruct1 = GetSubstruct(boxMon, boxMon->personality, 1);
    union PokemonSubstruct *substruct2 = GetSubstruct(boxMon, boxMon->personality, 2);
    union PokemonSubstruct *substruct3 = GetSubstruct(boxMon, boxMon->personality, 3);
    s32 i;

    for (i = 0; i < 6; i++)
        checksum += substruct0->raw[i];

    for (i = 0; i < 6; i++)
        checksum += substruct1->raw[i];

    for (i = 0; i < 6; i++)
        checksum += substruct2->raw[i];

    for (i = 0; i < 6; i++)
        checksum += substruct3->raw[i];

    return checksum;
}

static void rndParty(void) {
    s32 numMons = CalculatePlayerPartyCount();
    s32 i;
    u16 dexNum;
    u32 personality;
    for (i = 0; i < numMons; i++) {
        rndPkmn(&gPlayerParty[i]);
        dexNum = SpeciesToNationalPokedexNum(GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, 0));
        personality = GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY, 0);
        HandleSetPokedexFlag(dexNum, FLAG_SET_SEEN, personality);
        HandleSetPokedexFlag(dexNum, FLAG_SET_CAUGHT, personality);
    }
}

static void rndPkmn(struct Pokemon* mon) {
    struct PokemonSubstruct2 s2;
    struct PokemonSubstruct2 *substruct2;
    struct PokemonSubstruct3 s3;
    struct PokemonSubstruct3 *substruct3;
    u8 level;
    u32 exp;
    u16 species;
    u8 nextLevel;
    u32 xpa, xpb, xpc, xpd;
    f32 hpr, xpr;
    u16 prevHp;
    u32 status;
    u16 n = Random() % (NUM_SPECIES - 1) + 1;
    

    //store BoxPokemon data
    struct BoxPokemon* box = &mon->box;
    u32 personality = box->personality;
    u8 nickname[POKEMON_NAME_LENGTH];
    s32 i;
    u16 heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    species = GetMonData(mon, MON_DATA_SPECIES, 0);
    //skip eggs
    if (species == SPECIES_EGG || mon->box.isEgg) return;
    status = mon->status;
    GetMonData(mon, MON_DATA_NICKNAME, nickname);
    
    //if we got bad species id (?? pokemon), try to random again. If that doesn't work, just choose one.
    if (n >= SPECIES_OLD_UNOWN_B && n <= SPECIES_OLD_UNOWN_Z) n = Random() % (NUM_SPECIES - 2) + 1;
    if (n >= SPECIES_OLD_UNOWN_B && n <= SPECIES_OLD_UNOWN_Z) n = SPECIES_OLD_UNOWN_B - 1;
    //calculate XP and HP percentages
    level = mon->level;
    exp = GetMonData(mon, MON_DATA_EXP, NULL);
    
    nextLevel = GetMonData(mon, MON_DATA_LEVEL, 0) + 1;
    xpa = gExperienceTables[gBaseStats[species].growthRate][level];
    xpb = gExperienceTables[gBaseStats[species].growthRate][nextLevel];
    xpc = gExperienceTables[gBaseStats[n].growthRate][level];
    xpd = gExperienceTables[gBaseStats[n].growthRate][nextLevel];

    hpr = mon->hp / (f32)mon->maxHP;
    prevHp = mon->hp;

    xpr = (exp - (f32)xpa) / (xpb-xpa);
    //calculate new XP
    exp = xpr * (xpd - xpc) + xpc;

    //store data from substruct 2 and 3
    substruct2 = &(GetSubstruct(&mon->box, mon->box.personality, 2)->type2);
    substruct3 = &(GetSubstruct(&mon->box, mon->box.personality, 3)->type3);
    DecryptBoxMon(&mon->box);
    s2 = *substruct2;
    s3 = *substruct3;

    //create the pokemon, then set XP and HP from the ratios calculated earlier
    CreateMon(mon, n, level, 32, 0, 0, OT_ID_PLAYER_ID, 0);
    SetMonData(mon, MON_DATA_EXP, &exp);
    

    //restore substruct data
    substruct2 = &(GetSubstruct(&mon->box, mon->box.personality, 2)->type2);
    substruct3 = &(GetSubstruct(&mon->box, mon->box.personality, 3)->type3);
    DecryptBoxMon(&mon->box);
    *substruct2 = s2;
    *substruct3 = s3;
    mon->box.checksum = CalculateBoxMonChecksum(&mon->box);
    EncryptBoxMon(&mon->box);

    //restore BoxPokemon data
    box = &mon->box;
    //SetMonData(mon, MON_DATA_PERSONALITY, &personality);
    //box->personality = personality;
    SetMonData(mon, MON_DATA_NICKNAME, nickname);
    SetMonData(mon, MON_DATA_HELD_ITEM, &heldItem);
    personality = 4294967295u;
    SetMonData(mon, MON_DATA_FATEFUL_ENCOUNTER, &personality);
    SetMonData(mon, MON_DATA_OBEDIENCE, &personality);
    

    CalculateMonStats(mon);
    mon->hp = mon->maxHP * hpr + 0.5f;
    if (mon->hp == 0 && prevHp > 0) mon->hp = 1;
    mon->status = status;
}