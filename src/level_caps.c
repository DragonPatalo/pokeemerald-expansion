#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "level_caps.h"
#include "pokemon.h"


u32 GetCurrentLevelCap(void)
{
    static const u32 sLevelCapFlagMap[][2] =
    {
        {FLAG_BADGE01_GET, 15},                                     // 0x7
        {FLAG_DEFEATED_RIVAL_RUSTBORO, 17},                         // 0xD3
        {FLAG_BADGE02_GET, 21},                                     // 0x8
        {FLAG_DEFEATED_RIVAL_ROUTE_110, 27},                        // 0x4EF
        {FLAG_BADGE03_GET, 31},                                     // 0x9
        {FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, 35},                   // 0x8B
        {FLAG_BADGE04_GET, 39},                                     // 0xA
        {FLAG_BADGE05_GET, 45},                                     // 0xB
        {FLAG_RECEIVED_HM_FLY, 50}, // Defeat Rival on Route 119    // 
        {FLAG_BADGE06_GET, 54},                                     // 0xC
        //RIVAL LILYCOVE  58                                          //
        //MATT AQUA HIDEOUT 60                                        //
        {FLAG_BADGE07_GET, 64},                                     // 0xD
        //MAXIE/TABITHA SPACE CENTER 67                               //
        //ARCHIE SEA FLR CAVERN 69                                    // 0xE
        {FLAG_BADGE08_GET, 72},                                     //
        {FLAG_IS_CHAMPION, 80},                                     //
    };

    u32 i;

    if (B_LEVEL_CAP_TYPE == LEVEL_CAP_FLAG_LIST)
    {
        for (i = 0; i < ARRAY_COUNT(sLevelCapFlagMap); i++)
        {
            if (!FlagGet(sLevelCapFlagMap[i][0]))
                return sLevelCapFlagMap[i][1];
        }
    }
    else if (B_LEVEL_CAP_TYPE == LEVEL_CAP_VARIABLE)
    {
        return VarGet(B_LEVEL_CAP_VARIABLE);
    }

    return MAX_LEVEL;
}

u32 GetSoftLevelCapExpValue(u32 level, u32 expValue)
{
    static const u32 sExpScalingDown[5] = { 4, 8, 16, 32, 64 };
    static const u32 sExpScalingUp[5]   = { 16, 8, 4, 2, 1 };

    u32 levelDifference;
    u32 currentLevelCap = GetCurrentLevelCap();

    if (B_EXP_CAP_TYPE == EXP_CAP_NONE)
        return expValue;

    if (level < currentLevelCap)
    {
        if (B_LEVEL_CAP_EXP_UP)
        {
            levelDifference = currentLevelCap - level;
            if (levelDifference > ARRAY_COUNT(sExpScalingUp))
                return expValue + (expValue / sExpScalingUp[ARRAY_COUNT(sExpScalingUp) - 1]);
            else
                return expValue + (expValue / sExpScalingUp[levelDifference]);
        }
        else
        {
            return expValue;
        }
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_HARD)
    {
        return 0;
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_SOFT)
    {
        levelDifference = level - currentLevelCap;
        if (levelDifference > ARRAY_COUNT(sExpScalingDown))
            return expValue / sExpScalingDown[ARRAY_COUNT(sExpScalingDown) - 1];
        else
            return expValue / sExpScalingDown[levelDifference];
    }
    else
    {
       return expValue;
    }
}
