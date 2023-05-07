#include <YSI_Coding/y_hooks>

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
	{
	//
		
		case PLAYER_LOGIN:
		{
			new hashed_pass[65];
			if(!response) return Kick(playerid);
			SHA256_PassHash(inputtext, DataPlayer[playerid][Salt], hashed_pass, 65);
			if(strcmp(hashed_pass, DataPlayer[playerid][Password]) == 0)
			{
				cache_set_active(DataPlayer[playerid][Cache_ID]);
				AssignPlayerData(playerid);
				cache_delete(DataPlayer[playerid][Cache_ID]);
				DataPlayer[playerid][Cache_ID] = MYSQL_INVALID_CACHE;
				KillTimer(DataPlayer[playerid][LoginTimer]);
				DataPlayer[playerid][LoginTimer] = 0;
				DataPlayer[playerid][IsLoggedIn] = true;
				SetSpawnInfo(playerid, NO_TEAM, DataPlayer[playerid][Skin], DataPlayer[playerid][X_Pos], DataPlayer[playerid][Y_Pos], DataPlayer[playerid][Z_Pos], DataPlayer[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
				SpawnPlayer(playerid);
			}
			else
			{
				DataPlayer[playerid][LoginAttempts]++;
				if(DataPlayer[playerid][LoginAttempts] >= 3)
				{
					ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Ingreso - Cuenta Registrada", "Te has equivocado 3 veces al poner tu contraseña.\nTe hemos expulsado del servidor por seguridad.", "Entiendo", "");
					DelayedKick(playerid);
				}
				else ShowPlayerDialog(playerid, PLAYER_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Contraseña incorrecta!\nPor favor introduce la correcta:", "Ingresar", "Salir");
			}
		}
		case REG_PLAYER_PASS:
		{
			
			if(!response) return Kick(playerid);
			if(strlen(inputtext) <= 5) return ShowPlayerDialog(playerid, REG_PLAYER_PASS, DIALOG_STYLE_PASSWORD, "Ingreso - Registro de cuenta", "Tu contraseña no puede ser menor a 5 caracteres!\nPor favor crea otra contraseña:", "Registrar", "Cancelar");
			for(new i = 0; i < 16; i++) DataPlayer[playerid][Salt][i] = random(94) + 33;
			SHA256_PassHash(inputtext, DataPlayer[playerid][Salt], DataPlayer[playerid][Password], 65);
			ShowPlayerDialog(playerid, REG_PLAYER_EMAIL, DIALOG_STYLE_PASSWORD, "{FFFFFF} Ingreso - {F5E800}Registro de cuenta.", "{F5E800}Genial{FFFFFF}, para continuar ingresa un correo electronico:", "Continuar", "Cancelar");			
		}
		case REG_PLAYER_EMAIL:
		{
			new query[180], emailtmp[60];
			if(!IsValidEmail(inputtext)) return ShowPlayerDialog(playerid, REG_PLAYER_EMAIL, DIALOG_STYLE_PASSWORD, "{FFFFFF} Ingreso - {F5E800}Registro de cuenta.", "{FFFFFF}Este correo no es{F5E800} valido{FFFFFF},ingresa otro correo electronico:", "Continuar", "Cancelar");
			mysql_format(handle_db, query, sizeof query, "SELECT * FROM `users` WHERE `Email` = '%e' LIMIT 1", inputtext);
			mysql_tquery(handle_db, query, "CheckAvailableMail", "dd", playerid, g_MysqlRaceCheck[playerid]);
			format(emailtmp, sizeof(emailtmp), inputtext);
			DataPlayer[playerid][Email] = emailtmp;
			ShowPlayerDialog(playerid, REG_PLAYER_GENDER, DIALOG_STYLE_LIST, "{FFFFFF}Registro - Selecciona el {F5E800}genero {FFFFFF}de tu personaje.", "{FFFFFF}Hombre.\nMujer.", "Aceptar", "Cancelar");
		}
		case REG_PLAYER_GENDER:
		{
			switch(listitem)
			{
				case 0:
				{
					new query[221];
					DataPlayer[playerid][Gender] = 0; // Man
					DataPlayer[playerid][Skin] = 250;
					SetPlayerSkin(playerid, DataPlayer[playerid][Skin]);
					mysql_format(handle_db, query, sizeof query, "INSERT INTO `users` (`Name`, `Password`, `Salt`, `Email`, `Gender`) VALUES ('%e', '%s', '%e', '%e', '%d')", PlayerName(playerid), DataPlayer[playerid][Password], DataPlayer[playerid][Salt], DataPlayer[playerid][Email], DataPlayer[playerid][Gender]);
					mysql_tquery(handle_db, query, "OnPlayerRegister", "d", playerid);
				}
				case 1:
				{
					new query[221];
					DataPlayer[playerid][Gender] = 1; // Woman
					DataPlayer[playerid][Skin] = 191;
					SetPlayerSkin(playerid, DataPlayer[playerid][Skin]);					
					mysql_format(handle_db, query, sizeof query, "INSERT INTO `users` (`Name`, `Password`, `Salt`, `Email`, `Gender`) VALUES ('%e', '%s', '%e', '%e', '%d')", PlayerName(playerid), DataPlayer[playerid][Password], DataPlayer[playerid][Salt], DataPlayer[playerid][Email], DataPlayer[playerid][Gender]);
					mysql_tquery(handle_db, query, "OnPlayerRegister", "d", playerid);
				}
			}
		}
		default: return 0;
	}
	return 1;
}