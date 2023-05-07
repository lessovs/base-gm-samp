#include <YSI_Coding/y_hooks>
//- MySQL - Login and Register User - Forwards
forward OnPlayerRegister(playerid);
forward CheckAvailableMail(playerid);
forward OnPlayerDataLoaded(playerid, race_check);
forward AssignPlayerData(playerid);
forward SavePlayerData(playerid, reason);
//- MySQL - Login and Register User - Callbacks
public OnPlayerRegister(playerid)
{
	DataPlayer[playerid][ID] = cache_insert_id();
	DataPlayer[playerid][IsLoggedIn] = true;
	DataPlayer[playerid][X_Pos] = DEFAULT_POS_X;
	DataPlayer[playerid][Y_Pos] = DEFAULT_POS_Y;
	DataPlayer[playerid][Z_Pos] = DEFAULT_POS_Z;
	DataPlayer[playerid][A_Pos] = DEFAULT_POS_A;
	SetSpawnInfo(playerid, NO_TEAM, DataPlayer[playerid][Skin], DataPlayer[playerid][X_Pos], DataPlayer[playerid][Y_Pos], DataPlayer[playerid][Z_Pos], DataPlayer[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	return 1;
}

public CheckAvailableMail(playerid)
{
	if(cache_num_rows() == 1) return ShowPlayerDialog(playerid, REG_PLAYER_EMAIL, DIALOG_STYLE_PASSWORD, "{FFFFFF} Ingreso - {F5E800}Registro de cuenta.", "{FFFFFF}Este correo ya esta{F5E800}registrado{FFFFFF},ingresa otro correo electronico:", "Aceptar", "Cancelar");
	return 1;
}

public OnPlayerDataLoaded(playerid, race_check)
{
	new str[140];
	if(race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);
	if(cache_num_rows() > 0)
	{
		cache_get_value(0, "Password", DataPlayer[playerid][Password], 65);
		cache_get_value(0, "Salt", DataPlayer[playerid][Salt], 17);
		DataPlayer[playerid][Cache_ID] = cache_save();
		DataPlayer[playerid][LoginTimer] = SetTimerEx("OnLoginTimeout", SECONDS_TO_LOGIN * 1000, false, "d", playerid);
		format(str, sizeof(str), "%s{FFFFFF}Hola {F5E800}%s{FFFFFF}, nos alegra verte de nuevo.\nIngresa tu {F5E800}contraseña{FFFFFF}:", str, PlayerName(playerid));
		ShowPlayerDialog(playerid, PLAYER_LOGIN, DIALOG_STYLE_PASSWORD, "{FFFFFF} Ingreso - {F5E800}Cuenta registrada.", str, "Aceptar", "Cancelar");
	}
	else
	{
		format(str, sizeof(str), "%s{FFFFFF}Hola {F5E800}%s{FFFFFF}, no estas registrado, para continuar.\nCrea una {F5E800}contraseña{FFFFFF}:", str, PlayerName(playerid));
		ShowPlayerDialog(playerid, REG_PLAYER_PASS, DIALOG_STYLE_PASSWORD, "{FFFFFF} Ingreso - Cuenta {F5E800}registrada{FFFFFF}.", str, "Aceptar", "Cancelar");
	}
	return 1;
}

public AssignPlayerData(playerid)
{
	cache_get_value_int(0, "ID", DataPlayer[playerid][ID]);
	cache_get_value_float(0, "PX", DataPlayer[playerid][X_Pos]);
	cache_get_value_float(0, "PY", DataPlayer[playerid][Y_Pos]);
	cache_get_value_float(0, "PZ", DataPlayer[playerid][Z_Pos]);
	cache_get_value_float(0, "PA", DataPlayer[playerid][A_Pos]);
	cache_get_value_int(0, "Skin", DataPlayer[playerid][Skin]);
	cache_get_value_int(0, "Admin", DataPlayer[playerid][Admin]);
	
	return 1;
}

public SavePlayerData(playerid, reason)
{
	new query[520];
	if(DataPlayer[playerid][IsLoggedIn] == false) return 0;
	GetPlayerPos(playerid, DataPlayer[playerid][X_Pos], DataPlayer[playerid][Y_Pos], DataPlayer[playerid][Z_Pos]);
	GetPlayerFacingAngle(playerid, DataPlayer[playerid][A_Pos]);
	mysql_format(handle_db, query, sizeof query, "UPDATE `users` SET `PX` = %f, `PY` = %f, `PZ` = %f, `PA` = %f, `Skin` = %d, `Admin` = %d WHERE `ID` = %d LIMIT 1", DataPlayer[playerid][X_Pos], DataPlayer[playerid][Y_Pos], DataPlayer[playerid][Z_Pos], DataPlayer[playerid][A_Pos], DataPlayer[playerid][Skin], DataPlayer[playerid][Admin], DataPlayer[playerid][ID]);
	//
	mysql_tquery(handle_db, query);
	return 1;
}