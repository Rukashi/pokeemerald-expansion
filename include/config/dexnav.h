#ifndef GUARD_CONFIG_DEXNAV_H
#define GUARD_CONFIG_DEXNAV_H

#define DEXNAV_ENABLED                TRUE  // Whether or not DexNav is enabled. If TRUE, flags/vars below must all be non-zero
#define USE_DEXNAV_SEARCH_LEVELS      TRUE  /* WARNING: POSSIBLY EXCEEDS SAVEBLOCK SPACE! REQUIRES 1 BYTE PER SPECIES */

// Flag/var defines
#define DN_FLAG_SEARCHING             FLAG_DEXNAV_SEARCHING // Searching for mon
#define DN_FLAG_DEXNAV_GET            FLAG_DEXNAV_GET // DexNav shows in start menu
#define DN_FLAG_DETECTOR_MODE         FLAG_DEXNAV_DETECTOR_MODE // Allow player to find hidden mons
#define DN_VAR_SPECIES                VAR_DEXNAV_SPECIES // Registered DexNav species
#define DN_VAR_STEP_COUNTER           VAR_DEXNAV_STEP_COUNTER // Steps for finding hidden Pokémon

// Search parameters
#define DEXNAV_TIMEOUT                  15  // 15 seconds is the time out. Max of 1092 seconds allowed
#define SNEAKING_PROXIMITY              4   // Tile amount
#define CREEPING_PROXIMITY              2
#define MAX_PROXIMITY                   20

#define DEXNAV_CHAIN_MAX                100 // maximum chain value

// hidden Pokémon options - an approximation of values due to lack of available data
#define HIDDEN_MON_STEP_COUNT       16   // Look for hidden Pokémon every x steps
#define HIDDEN_MON_SEARCH_RATE      10   // x% chance of finding hidden Pokémon every x steps
#define HIDDEN_MON_PROBABILTY       25   // x% chance of finding hidden mon compared to regular encounter data

//// SEARCH PROBABILITIES
// See https://bulbapedia.bulbagarden.net/wiki/DexNav#Benefits
// Chance of encountering egg move at search levels
#define SEARCHLEVEL0_MOVECHANCE         5
#define SEARCHLEVEL5_MOVECHANCE         20
#define SEARCHLEVEL10_MOVECHANCE        35
#define SEARCHLEVEL25_MOVECHANCE        50
#define SEARCHLEVEL50_MOVECHANCE        65
#define SEARCHLEVEL100_MOVECHANCE       80
// Chance of encountering Hidden Abilities at search levels
#define SEARCHLEVEL0_ABILITYCHANCE      0
#define SEARCHLEVEL5_ABILITYCHANCE      10
#define SEARCHLEVEL10_ABILITYCHANCE     20
#define SEARCHLEVEL25_ABILITYCHANCE     30
#define SEARCHLEVEL50_ABILITYCHANCE     35
#define SEARCHLEVEL100_ABILITYCHANCE    40
// Chance of encountering held item
#define SEARCHLEVEL0_ITEM               0
#define SEARCHLEVEL5_ITEM               5
#define SEARCHLEVEL10_ITEM              10
#define SEARCHLEVEL25_ITEM              15
#define SEARCHLEVEL50_ITEM              20
#define SEARCHLEVEL100_ITEM             25
// Chance of encountering one star potential
#define SEARCHLEVEL0_ONESTAR            0
#define SEARCHLEVEL5_ONESTAR            20
#define SEARCHLEVEL10_ONESTAR           40
#define SEARCHLEVEL25_ONESTAR           60
#define SEARCHLEVEL50_ONESTAR           40
#define SEARCHLEVEL100_ONESTAR          20
// Chance of encountering two star potential
#define SEARCHLEVEL0_TWOSTAR            0
#define SEARCHLEVEL5_TWOSTAR            10
#define SEARCHLEVEL10_TWOSTAR           20
#define SEARCHLEVEL25_TWOSTAR           30
#define SEARCHLEVEL50_TWOSTAR           40
#define SEARCHLEVEL100_TWOSTAR          50
// Chance of encountering three star potential
#define SEARCHLEVEL0_THREESTAR          0
#define SEARCHLEVEL5_THREESTAR          0
#define SEARCHLEVEL10_THREESTAR         5
#define SEARCHLEVEL25_THREESTAR         10
#define SEARCHLEVEL50_THREESTAR         20
#define SEARCHLEVEL100_THREESTAR        30

#endif // GUARD_CONFIG_DEXNAV_H
