//-- Utils - Funcs - Stocks
stock PlayerName(playerid)
{
	new name[24];
	GetPlayerName(playerid, name, 24);
	return name;
}

stock IsValidName(const nickname[])
{
  static Regex:regex;
  if (!regex) regex = Regex_New("[A-Z][a-z]+_[A-Z][a-z]+");
  return Regex_Check(nickname, regex);
}

stock IsValidEmail(const email[])
{
  static Regex:regex;
  if (!regex) regex = Regex_New("^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");
  return Regex_Check(email, regex);
}

stock KickReason(playerid, reason[30])
{
	printf("%s", reason);
	Kick(playerid);
}

stock DelayedKick(playerid, time = 500)
{
	SetTimerEx("_KickPlayerDelayed", time, false, "d", playerid);
	return 1;
}

//---------------
forward OnLoginTimeout(playerid);
public OnLoginTimeout(playerid)
{
	DataPlayer[playerid][LoginTimer] = 0;
	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Login", "You have been kicked for taking too long to login successfully to your account.", "Okay", "");
	DelayedKick(playerid);
	return 1;
}

forward _KickPlayerDelayed(playerid);
public _KickPlayerDelayed(playerid)
{
	Kick(playerid);
	return 1;
}