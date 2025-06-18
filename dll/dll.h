/* Copyright (C) 2019 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "steam_client.h"

#ifdef STEAMCLIENT_DLL
#define STEAMAPI_API static
#define STEAMCLIENT_API S_API_EXPORT
#else
#define STEAMAPI_API S_API_EXPORT
#define STEAMCLIENT_API static
#endif


#define PRINT_DEBUG_ENTRY() PRINT_DEBUG("")

enum ESteamAPIInitResult
{
	k_ESteamAPIInitResult_OK = 0,
	k_ESteamAPIInitResult_FailedGeneric = 1, // Some other failure
	k_ESteamAPIInitResult_NoSteamClient = 2, // We cannot connect to Steam, steam probably isn't running
	k_ESteamAPIInitResult_VersionMismatch = 3, // Steam client appears to be out of date
};

const int k_cchMaxSteamErrMsg = 1024;
typedef char SteamErrMsg[ k_cchMaxSteamErrMsg ];

// See "Initializing the Steamworks SDK" above for how to choose an init method.
// Same usage as SteamAPI_InitEx(), however does not verify ISteam* interfaces are
// supported by the user's client and is exported from the dll
S_API ESteamAPIInitResult S_CALLTYPE SteamAPI_InitFlat( SteamErrMsg *pOutErrMsg );

S_API ESteamAPIInitResult S_CALLTYPE SteamInternal_SteamAPI_Init( const char *pszInternalCheckInterfaceVersions, SteamErrMsg *pOutErrMsg );

// Internal implementation of SteamAPI_InitEx.  This is done in a way that checks
// all of the versions of interfaces from headers being compiled into this code.
// If you are not using any of the C++ interfaces and do not need this version checking
// (for example if you are only using the "flat" interfaces, which have a different type
// of version checking), you can pass a NULL interface version string.
inline ESteamAPIInitResult SteamAPI_InitEx( SteamErrMsg *pOutErrMsg )
{
	const char *pszInternalCheckInterfaceVersions = 
		STEAMUTILS_INTERFACE_VERSION "\0"
		STEAMNETWORKINGUTILS_INTERFACE_VERSION "\0"

		STEAMAPPLIST_INTERFACE_VERSION "\0"
		STEAMAPPS_INTERFACE_VERSION "\0"
		STEAMCONTROLLER_INTERFACE_VERSION "\0"
		STEAMFRIENDS_INTERFACE_VERSION "\0"
		STEAMGAMESEARCH_INTERFACE_VERSION "\0"
		STEAMHTMLSURFACE_INTERFACE_VERSION "\0"
		STEAMHTTP_INTERFACE_VERSION "\0"
		STEAMINPUT_INTERFACE_VERSION "\0"
		STEAMINVENTORY_INTERFACE_VERSION "\0"
		STEAMMATCHMAKINGSERVERS_INTERFACE_VERSION "\0"
		STEAMMATCHMAKING_INTERFACE_VERSION "\0"
		STEAMMUSICREMOTE_INTERFACE_VERSION "\0"
		STEAMMUSIC_INTERFACE_VERSION "\0"
		STEAMNETWORKINGMESSAGES_INTERFACE_VERSION "\0"
		STEAMNETWORKINGSOCKETS_INTERFACE_VERSION "\0"
		STEAMNETWORKING_INTERFACE_VERSION "\0"
		STEAMPARENTALSETTINGS_INTERFACE_VERSION "\0"
		STEAMPARTIES_INTERFACE_VERSION "\0"
		STEAMREMOTEPLAY_INTERFACE_VERSION "\0"
		STEAMREMOTESTORAGE_INTERFACE_VERSION "\0"
		STEAMSCREENSHOTS_INTERFACE_VERSION "\0"
		STEAMUGC_INTERFACE_VERSION "\0"
		STEAMUSERSTATS_INTERFACE_VERSION "\0"
		STEAMUSER_INTERFACE_VERSION "\0"
		STEAMVIDEO_INTERFACE_VERSION "\0"

		"\0";

	return SteamInternal_SteamAPI_Init( pszInternalCheckInterfaceVersions, pOutErrMsg );
}

Steam_Client *get_steam_client();
bool steamclient_has_ipv6_functions();

HSteamUser flat_hsteamuser();
HSteamPipe flat_hsteampipe();
HSteamUser flat_gs_hsteamuser();
HSteamPipe flat_gs_hsteampipe();
