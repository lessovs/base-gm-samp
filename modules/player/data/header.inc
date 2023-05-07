// player data
enum E_PLAYER
{
	ID,
	Name[MAX_PLAYER_NAME],
	Password[65], 
	Salt[17],
	Email[60],
	Gender,
	Age,
	Skin,
	Float: X_Pos,
	Float: Y_Pos,
	Float: Z_Pos,
	Float: A_Pos,
	//
	Cache: Cache_ID,
	bool: IsLoggedIn,
	LoginAttempts,
	LoginTimer,
	//
	Admin,
	//
};
new DataPlayer[MAX_PLAYERS][E_PLAYER];