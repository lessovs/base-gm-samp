/*
	Gamemode - v 0.1
	Developed by lessov
	UPDATES COMING SOON....
*/

//
#define  	YSI_NO_HEAP_MALLOC
#define 	SSCANF_NO_NICE_FEATURES

//Includes
#include <a_samp>
#include <a_mysql>
#include <Pawn.CMD>
#include <Pawn.Regex>
#include <sscanf2>
#include <streamer>
#include <YSI_Visual/y_dialog>


//-- Modules - headers
#include "modules/server/cfg/header.inc"
#include "modules/data_base/header.inc"
#include "modules/server/utils/header.inc"
#include "modules/player/data/header.inc"
//-- Modules - funcs
#include "modules/server/utils/funcs.inc"
//-- Modules - impls
#include "modules/data_base/impl.inc"
#include "modules/player/connection/impl.inc"
#include "modules/player/login-register/impl.inc"
#include "modules/player/dialogs/impl.inc"


main(){}

public OnGameModeInit()
{
	connectDatabase();
	UsePlayerPedAnims();
	return 1;
}
