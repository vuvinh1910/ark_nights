#pragma once

#if defined(__aarch64__)
	auto RETURN = "CO 03 5F D6";
	auto NOP = "1F 20 03 D5";
	auto TRUE = "20 00 80 D2 CO 03 5F D6";
	auto FALSE = "00 00 80 D2 CO 03 5F D6";
	auto window_name = OBFUSCATE("Tuấn Meta | @ZTRChannel [X64]");
#else
	auto RETURN = "1E FF 2F E1";
	auto NOP = "00 F0 20 E3";
	auto TRUE = "01 00 A0 E3 1E FF 2F E1";
	auto FALSE = "00 00 A0 E3 1E FF 2F E1";
	auto window_name = OBFUSCATE("Tuấn Meta | @ZTRChannel [X32]");
#endif

int dmg = 1, defense = 1, attacksp = 1;
bool frozen, deploy, autowin;

enum class GameResult
{
	NOT_YET = 0,
	WIN = 1,
	LOSE = 2
};

enum class SideType
{
	NONE = 0, // 0x0
	ALLY = 1, // 0x0
	ENEMY = 2, // 0x0
	BOTH_ALLY_AND_ENEMY = 3, // 0x0
	NEUTRAL = 4 // 0x0
};

struct FP {
	long _serializedValue; //0x10
};

uintptr_t sideType;

FP (*_get_atk)(void *instance);
FP get_atk(void *instance) {
	if (instance != NULL)
	{
		SideType side = *(SideType *)((uintptr_t) instance + sideType);
		if (dmg > 1 && side == SideType::ALLY)
		{
			FP damage;
			damage._serializedValue = _get_atk(instance)._serializedValue * dmg;
			return damage;
		}
		
		if (defense > 1 && side == SideType::ENEMY)
		{
			FP def;
			def._serializedValue = _get_atk(instance)._serializedValue / defense;
			return def;
		}
	}
	return _get_atk(instance);
}

FP (*_get_attackSpeed)(void *instance);
FP get_attackSpeed(void *instance) {
	if (instance != NULL && attacksp > 1)
	{
		SideType side = *(SideType *)((uintptr_t) instance + sideType);
		if (side == SideType::ALLY)
		{
			FP speed;
			speed._serializedValue = _get_attackSpeed(instance)._serializedValue * attacksp;
			return speed;
		}
	}
	return _get_attackSpeed(instance);
}

bool (*_get_isFrozen)(void *instance);
bool get_isFrozen(void *instance) {
	if (instance != NULL && frozen)
	{
		SideType side = *(SideType *)((uintptr_t) instance + sideType);
		if (side == SideType::ENEMY)
		{
			return true;
		}
	}
	return _get_isFrozen(instance);
}

bool (*_ModifyCost)(void *instance, int value, void *source, void *side, bool forceToDisplayNumber, bool forceToDisplayNegativeNumber);
bool ModifyCost(void *instance, int value, void *source, void *side, bool forceToDisplayNumber, bool forceToDisplayNegativeNumber){
	if(instance != NULL && deploy){
		return false;
	}
	return _ModifyCost(instance, value, source, side, forceToDisplayNumber, forceToDisplayNegativeNumber);
}

void DoFinishGame(void *instance, GameResult result, bool silent) {
	void (*_DoFinishGame)(void *instance, GameResult result, bool silent) = (void (*)(void *, GameResult, bool))(GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("BattleController") , oxorany("_DoFinishGame"), 2));
	return _DoFinishGame(instance, result, silent);
}

void (*_BattleController)(void *instance);
void BattleController(void *instance){
	if (instance != NULL){
		if (autowin) DoFinishGame(instance, GameResult::WIN, true);
	}
	_BattleController(instance);
}