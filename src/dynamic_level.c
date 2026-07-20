#include "global.h"
#include "dynamic_level.h"
#include "pokemon.h"
#include "move.h"
#include "random.h"
#include "event_data.h"
#include "constants/pokemon.h"

enum Species DynLvl_Devolve(enum Species species, u8 level, u32 personalityValue, u8 is_wild) {
	if (FlagGet(DYN_LVL_FLAG_NO_DEVOLVE)) return species;
	
	enum Species evoStage[DYN_LVL_MAX_SPECIES] = {species}, override[DYN_LVL_MAX_OVERRIDES], current_evo;
	const struct Evolution *evolutions;
	s32 i, j, k;
	u8 evoMinLvl[DYN_LVL_MAX_SPECIES], lvl_min, lvl_add, lvl_temp, num_overrides = 0, curr_override = 0, result = DYN_LVL_MAX_SPECIES - 1;
	
	for (i = 0; i < DYN_LVL_MAX_OVERRIDES; i++) override[i] = SPECIES_NONE;
	
	// Fill previous evolution stage species
	for (i = 1; i < DYN_LVL_MAX_SPECIES; i++) {
		evoStage[i] = GetSpeciesPreEvolution(evoStage[i - 1]);
		if (evoStage[i] == SPECIES_NONE) {
			// No more preEvos found, abort
			result = i - 1;
			break;
		}
	}
	
	evoMinLvl[result] = MIN_LEVEL;
	
	// Check if each evo stage should evolve, until one doesn't
	for (; result > 0; result--) {
		evoMinLvl[result - 1] = MAX_LEVEL;
		evolutions = GetSpeciesEvolutions(evoStage[result]);
		
		if (num_overrides > curr_override) {
			// Must check minimum evo level for another species as well
			current_evo = override[curr_override];
			curr_override++;
		}
		else current_evo = evoStage[result - 1];
		
		// Calculate minimum level for current evo
		for (i = 0; evolutions[i].method != EVOLUTIONS_END; i++) {
			if (evolutions[i].targetSpecies == current_evo) {
				if (evolutions[i].method == EVO_LEVEL || evolutions[i].method == EVO_LEVEL_BATTLE_ONLY) {
					// Lvl evo. Minimum Lvl is highest between evo Lvl, and preEvo's minimum Lvl +1
					lvl_min = max(evoMinLvl[result] + 1, evolutions[i].param);
					lvl_add = 0;
				}
				else if (evolutions[i].method == EVO_SPLIT_FROM_EVO) {
					// Split evo (Shedinja), check for the other species (Ninjask) after, but only if that species hasn't/won't be checked already
					if (num_overrides == DYN_LVL_MAX_OVERRIDES || evolutions[i].param == evoStage[result - 1]) continue;
					
					lvl_temp = 255;
					for (j = 0; j < num_overrides; j++) {
						if (evolutions[i].param == override[j]) {
							lvl_temp = j;
							break;
						}
					}
					
					if (lvl_temp == 255) {
						override[num_overrides] = evolutions[i].param;
						num_overrides++;
					}
					continue;
				}
				else {
					// Not Lvl dependent, use preEvo's minimum Lvl
					lvl_min = evoMinLvl[result];
					
					// Non Lvl based evos shouldn't happen before Lvl 16, or before 8 Lvls since last evo
					lvl_add = max(8, 16 - lvl_min);
					
					// Add levels based on evo's and preEvo's BSTs
					j = GetSpeciesBaseStatTotal(current_evo);
					lvl_add = max(lvl_add, max((j / 15) - lvl_min, 4 + (j - GetSpeciesBaseStatTotal(evoStage[result]) / 15)));
				}
				
				// Add levels for evos with certain conditions
				for (j = 0; evolutions[i].params != NULL && evolutions[i].params[j].condition != CONDITIONS_END; j++) {
					lvl_temp = 0;
					switch (evolutions[i].params[j].condition) {
						case IF_MIN_FRIENDSHIP:
						if (evolutions[i].params->arg1 > gSpeciesInfo[evoStage[result]].friendship) lvl_temp = (evolutions[i].params[j].arg1 - gSpeciesInfo[evoStage[result]].friendship) / 8;
						else lvl_temp = 0;
						break;
						
						case IF_MIN_BEAUTY:
						case IF_MIN_COOLNESS:
						case IF_MIN_SMARTNESS:
						case IF_MIN_TOUGHNESS:
						case IF_MIN_CUTENESS:
						lvl_temp = evolutions[i].params[j].arg1 / 10;
						break;
						
						case IF_KNOWS_MOVE:
						if (gSpeciesInfo[evoStage[result]].levelUpLearnset == NULL) {
							lvl_temp = 16;
							break;
						}
						
						lvl_temp = 255;
						for (k = 0; gSpeciesInfo[evoStage[result]].levelUpLearnset[k].move != LEVEL_UP_MOVE_END; k++) {
							if (gSpeciesInfo[evoStage[result]].levelUpLearnset[k].move == evolutions[i].params[j].arg1 && gSpeciesInfo[evoStage[result]].levelUpLearnset[k].level >= lvl_min) {
								lvl_temp = min(lvl_temp, gSpeciesInfo[evoStage[result]].levelUpLearnset[k].level - lvl_min);
							}
						}
						if (lvl_temp == 255) lvl_temp = 16; // Move not in learnset at or above minimum level
						break;
						
						case IF_KNOWS_MOVE_TYPE:
						if (gSpeciesInfo[evoStage[result]].levelUpLearnset == NULL) {
							lvl_temp = 16;
							break;
						}
						
						lvl_temp = 255;
						for (k = 0; gSpeciesInfo[evoStage[result]].levelUpLearnset[k].move != LEVEL_UP_MOVE_END; k++) {
							if (gMovesInfo[gSpeciesInfo[evoStage[result]].levelUpLearnset[k].move].type == evolutions[i].params[j].arg1 && gSpeciesInfo[evoStage[result]].levelUpLearnset[k].level >= lvl_min) {
								lvl_temp = min(lvl_temp, gSpeciesInfo[evoStage[result]].levelUpLearnset[k].level - lvl_min);
							}
						}
						if (lvl_temp == 255) lvl_temp = 16; // Move not in learnset at or above minimum level
						break;
					}
					lvl_add = max(lvl_add, lvl_temp);
				}
				
				// If multiple evo methods, use lowest minimum Lvl
				evoMinLvl[result - 1] = min(evoMinLvl[result - 1], lvl_min + lvl_add);
				// assertf(FALSE, "\n\nCurrent level: %d\n\nLvl min: %d\nLvl add: %d", level, lvl_min, lvl_add) {}
			}
		}
		
		if (num_overrides > curr_override) {
			result++;
			continue;
		}
		
		// Check if preEvo should evolve
		if (evoMinLvl[result - 1] > level) return evoStage[result]; // Level below minimum for evo}
		
		// Calculate chance to evolve, based on levels above minimum
		i = level - evoMinLvl[result - 1];
		if (is_wild) {
			// Wild Pokémon
			#if DYN_LVL_WILD_MAX_EVO_CHANCE < 100
			j = min(DYN_LVL_WILD_MAX_EVO_CHANCE, DYN_LVL_WILD_BASE_EVO_CHANCE + DYN_LVL_WILD_EVO_CHANCE_PER_LEVEL * i);
			#else
			j = DYN_LVL_WILD_BASE_EVO_CHANCE + DYN_LVL_WILD_EVO_CHANCE_PER_LEVEL * i;
			#endif
			k = (100 * Random()) >> 16;
		}
		else {
			// Trainer Pokémon
			#if DYN_LVL_TRAINER_MAX_EVO_CHANCE < 100
			j = min(DYN_LVL_TRAINER_MAX_EVO_CHANCE, DYN_LVL_TRAINER_BASE_EVO_CHANCE + DYN_LVL_TRAINER_EVO_CHANCE_PER_LEVEL * i);
			#else
			j = DYN_LVL_TRAINER_BASE_EVO_CHANCE + DYN_LVL_TRAINER_EVO_CHANCE_PER_LEVEL * i;
			#endif
			rng_value_t rng = LocalRandomSeed(personalityValue);
			k = (100 * LocalRandom(&rng)) >> 16;
		}
		
		if (k >= j) return evoStage[result]; // Did not succeed evo
		
		// Evo succeeded, prepare for next evo, if needed
		for (i = 0; i < DYN_LVL_MAX_OVERRIDES; i++) override[i] = SPECIES_NONE;
		num_overrides = curr_override = 0;
	}
	
	// All evolutions succeeded
	return species;
}

u8 DynLvl_LevelUp(u8 level, u8 is_wild) {
	if (FlagGet(DYN_LVL_FLAG_NO_LEVELUP)) return level;
	return min(MAX_LEVEL, level + (is_wild ? VarGet(VAR_WILD_ADDED_LEVELS) : VarGet(VAR_TRAINER_ADDED_LEVELS)));
}
