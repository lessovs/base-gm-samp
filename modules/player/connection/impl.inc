#include <YSI_Coding/y_hooks>

hook OnPlayerConnect(playerid)
{
	new query[103];
	if(!IsValidName(PlayerName(playerid)) && strcmp(PlayerName(playerid), ""OWNER"", false) && strcmp(PlayerName(playerid), ""OWNER_TWO"", false))
	{
		Kick(playerid);
	}
	g_MysqlRaceCheck[playerid]++;
	static const empty_player[E_PLAYER];
	DataPlayer[playerid] = empty_player;
	mysql_format(handle_db, query, sizeof query, "SELECT * FROM `users` WHERE `Name` = '%e' LIMIT 1", PlayerName(playerid));
	mysql_tquery(handle_db, query, "OnPlayerDataLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	g_MysqlRaceCheck[playerid]++;

	SavePlayerData(playerid, reason);

	// if the player was kicked (either wrong password or taking too long) during the login part, remove the data from the memory
	if(cache_is_valid(DataPlayer[playerid][Cache_ID]))
	{
		cache_delete(DataPlayer[playerid][Cache_ID]);
		DataPlayer[playerid][Cache_ID] = MYSQL_INVALID_CACHE;
	}

	// if the player was kicked before the time expires (30 seconds), kill the timer
	if(DataPlayer[playerid][LoginTimer])
	{
		KillTimer(DataPlayer[playerid][LoginTimer]);
		DataPlayer[playerid][LoginTimer] = 0;
	}

	// sets "IsLoggedIn" to false when the player disconnects, it prevents from saving the player data twice when "gmx" is used
	DataPlayer[playerid][IsLoggedIn] = false;
	return 1;
}

hook OnPlayerSpawn(playerid)
{
	// spawn the player to their last saved position
	//SetPlayerInterior(playerid, Player[playerid][Interior]);
	SetPlayerPos(playerid, DataPlayer[playerid][X_Pos], DataPlayer[playerid][Y_Pos], DataPlayer[playerid][Z_Pos]);
	SetPlayerFacingAngle(playerid, DataPlayer[playerid][A_Pos]);
	SetPlayerSkin(playerid, DataPlayer[playerid][Skin]);
	SetCameraBehindPlayer(playerid);
	return 1;
}