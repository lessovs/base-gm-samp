#include <YSI_Coding/y_hooks>

stock connectDatabase()
{
	handle_db = mysql_connect_file("cfg_db.ini");
	if(handle_db == MYSQL_INVALID_HANDLE || mysql_errno(handle_db) != 0)
	{
		print("MySQL connection failed. Server is shutting down.");
		SendRconCommand("exit"); // close the server if there is no connection
		return 1;
	}
	print("MySQL connection is successful.");

	return 1;
}

hook OnGameModeExit()
{
	mysql_close(handle_db);
	return 1;
}