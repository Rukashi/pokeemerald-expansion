#include "pokemon.h"
#include "constants/flags.h"
#include "constants/vars.h"

#define DYN_LVL_MAX_SPECIES 4
#define DYN_LVL_MAX_OVERRIDES 2

#define DYN_LVL_TRAINER_BASE_EVO_CHANCE 40
#define DYN_LVL_TRAINER_EVO_CHANCE_PER_LEVEL 15
#define DYN_LVL_TRAINER_MAX_EVO_CHANCE 100

#define DYN_LVL_WILD_BASE_EVO_CHANCE 10
#define DYN_LVL_WILD_EVO_CHANCE_PER_LEVEL 5
#define DYN_LVL_WILD_MAX_EVO_CHANCE 50

#define DYN_LVL_FLAG_NO_DEVOLVE FLAG_DYN_LVL_NO_DEVOLVE
#define DYN_LVL_FLAG_NO_LEVELUP FLAG_DYN_LVL_NO_LEVELUP

#define DYN_LVL_VAR_WILD_LEVELS VAR_WILD_ADDED_LEVELS
#define DYN_LVL_VAR_TRAINER_LEVELS VAR_TRAINER_ADDED_LEVELS

enum Species DynLvl_Devolve(enum Species species, u8 level, u32 personalityValue, u8 is_wild);
u8 DynLvl_LevelUp(u8 level, u8 is_wild);