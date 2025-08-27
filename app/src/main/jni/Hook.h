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

int dmg = 1, defense = 1, attacksp = 1, sp_recovery = 1;
bool frozen, deploy, autowin, noCardCost, freeDeploy, noRespawnTime, onehit, godMode, unlimitUnit, freezeLifePoint, maxLifePoint, preMaxLifePoint;
uintptr_t sideType, m_owner, respawnState;

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

struct ObscuredInt {
    int currentCryptoKey;    // 0x10
    int hiddenValue;         // 0x14
    bool inited;             // 0x18
    char pad_0x19[0x3];      // padding để nhảy tới 0x1C
    int fakeValue;           // 0x1C
    bool fakeValueActive;    // 0x20
    char pad_0x21[0x3];      // padding để nhảy tới 0x24
};

typedef ObscuredInt (*OpImplicit_t)(int value);
static OpImplicit_t ObscuredInt_OpImplicit;
ObscuredInt IntToObscuredInt(int value) {
    if(!ObscuredInt_OpImplicit) {
        const char* args[1] = { "System.Int32" };
        void* addr = Il2Cpp::GetMethodOffset(
                "ThirdPartyAssembly.dll",
                "CodeStage.AntiCheat.ObscuredTypes",
                "ObscuredInt",
                "op_Implicit",
                (char**)args,
                1
        );
        ObscuredInt_OpImplicit = (OpImplicit_t)addr;
    }
    return ObscuredInt_OpImplicit(value);
}

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

FP (*_get_attackTime)(void *instance);
FP get_attackTime(void *instance) {
    if (instance != NULL && attacksp > 1)
    {
        SideType side = *(SideType *)((uintptr_t) instance + sideType);
        if (side == SideType::ALLY)
        {
            FP attackTime;
            attackTime._serializedValue = _get_attackTime(instance)._serializedValue / attacksp;
            return attackTime;
        }
    }
    return _get_attackTime(instance);
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
    if(instance != NULL && deploy && value < 0){
        return false;
    }
    return _ModifyCost(instance, value, source, side, forceToDisplayNumber, forceToDisplayNegativeNumber);
}

void DoFinishGame(void *instance, GameResult result, bool silent) {
    void (*_DoFinishGame)(void *, GameResult, bool) =
    (void (*)(void *, GameResult, bool))(
            GetMethodOffset(
                    oxorany("Assembly-CSharp.dll"),
                    oxorany("Torappu.Battle"),
                    oxorany("BattleController"),
                    oxorany("_DoFinishGame"),
                    2
            )
    );

    if (_DoFinishGame != NULL) {
        _DoFinishGame(instance, result, silent);
    }
}

void DoSetLifePoint(void *instance, int value, int side) {
    void (*_DoSetLifePoint)(void *, int, int) =
    (void (*)(void *, int, int))(
            GetMethodOffset(
                    oxorany("Assembly-CSharp.dll"),
                    oxorany("Torappu.Battle"),
                    oxorany("BattleController"),
                    oxorany("SetLifePoint"),
                    2
            )
    );
    if(_DoSetLifePoint) {
        _DoSetLifePoint(instance,value,side);
    }
}

void (*_BattleController)(void *instance);
void BattleController(void *instance){
	if (instance != NULL){
		if (autowin) DoFinishGame(instance, GameResult::WIN, true);
        if(maxLifePoint != preMaxLifePoint) {
            if(maxLifePoint) {
                DoSetLifePoint(instance,999,0);
            }
            preMaxLifePoint = maxLifePoint;
        }
	}
	_BattleController(instance);
}

void (*_SkillPointController)(void *instance, FP value, bool force);
void SkillPointController(void *instance, FP value, bool force) {
    if(instance != NULL && sp_recovery > 1) {
        void* owner = *(void**)((uintptr_t)instance + m_owner);
        if(owner != NULL) {
            SideType side = *(SideType*)((uintptr_t)owner + sideType);
            if(side == SideType::ALLY) {
                FP newSpRecoveryRate;
                newSpRecoveryRate._serializedValue = value._serializedValue * sp_recovery;
                _SkillPointController(instance, newSpRecoveryRate, force);
                return;
            }
        }
    }
    _SkillPointController(instance, value, force);
}

bool (*_get_readyToSpawn)(void *instance);
bool get_readyToSpawn(void *instance) {
    if(instance != NULL && freeDeploy) {
        return true;
    }
    return _get_readyToSpawn(instance);
}

int (*_get_cost)(void *instance);
int get_cost(void *instance) {
    if(instance != NULL && noCardCost) {
        return 0;
    }
    return _get_cost(instance);
}

void (*_OnCardListChanged)(void *instance, void *card);
void OnCardListChanged(void *instance, void *card) {
    if (instance != NULL && freeDeploy) {
        return;
    }
    _OnCardListChanged(instance, card);
}

void (*_CardController)(void *instance, void *deltaTime);
void CardController(void *instance, void *deltaTime) {
    if (instance != NULL && noRespawnTime) {
        *(bool *)((uintptr_t)instance + respawnState) = true;
    }
    return _CardController(instance, deltaTime);
}

FP (*_get_hp)(void *instance);
FP get_hp(void *instance) {
    if(instance != NULL && onehit) {
        SideType side = *(SideType *)((uintptr_t) instance + sideType);
        if (side == SideType::ENEMY) {
            FP newValue;
            newValue._serializedValue = _get_hp(instance)._serializedValue / INT_MAX;
            return newValue;
        }
    }
    return _get_hp(instance);
}

typedef FP (*OpImplicitDouble_t)(double value);
static OpImplicitDouble_t op_Implicit_Double;
FP DoubleToFP(double value) {
    if (!op_Implicit_Double) {
        const char* args[1] = { "System.Double" };
        void* addr = Il2Cpp::GetMethodOffset(
                "Torappu.Common.dll",
                "Torappu",
                "FP",
                "op_Implicit",
                (char**)args,
                1
        );
        op_Implicit_Double = (OpImplicitDouble_t)addr;
    }
    return op_Implicit_Double(value);
}

void (*_set_hp)(void *instance, FP value);
void set_hp(void *instance, FP value) {
    if(instance != NULL && godMode) {
        SideType side = *(SideType *)((uintptr_t) instance + sideType);
        if (side == SideType::ALLY) {
            FP newVal = DoubleToFP(456);
            return _set_hp(instance,newVal);
        }
    }
    _set_hp(instance,value);
}

bool (*_ModifyLifePoint)(void *instance, int value, void *source, int side, bool isReach);
bool ModifyLifePoint(void *instance, int value, void *source, int side, bool isReach) {
    if(instance != NULL && freezeLifePoint) {
        return false;
    }
    return _ModifyLifePoint(instance,value,source,side,isReach);
}

bool (*_get_dontOccupyDeployCnt)(void *instance);
bool get_dontOccupyDeployCnt(void *instance) {
    if(instance != NULL && unlimitUnit) {
        return true;
    }
    return _get_dontOccupyDeployCnt(instance);
}