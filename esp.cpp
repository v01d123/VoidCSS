#include "esp.h"

void Esp::CrossHairByLix( void )
{
	int *iTeamIndex = (int*) ((DWORD)VoidCSS.g_pMyPlayer->BaseEnt() +(DWORD)0x90 );

	DWORD dwGetTeamColor = CHEAT_WHITE;

	if( *iTeamIndex == 2 ) //Terrorist
		dwGetTeamColor = CHEAT_RED;

	if( *iTeamIndex == 3 )// Counter-Terrorist
		dwGetTeamColor = CHEAT_BLUE;
	
	//Draw Fov 5 x 2
	VoidCSS.g_pMatSystemSurface->DrawColoredCircle((sScreenSize.m_iWidth / 2), (sScreenSize.m_iHeight / 2),10.0f,RED(CHEAT_BLACK),GREEN(CHEAT_BLACK),BLUE(CHEAT_BLACK),ALPHA(CHEAT_BLACK) );
	//CROSS
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2), (sScreenSize.m_iHeight / 2) - 5, 1,11, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 5, (sScreenSize.m_iHeight / 2), 11,1, CHEAT_BLACK );
	//Boxes
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 1, (sScreenSize.m_iHeight / 2)  - 6, 3,1, CHEAT_BLACK);
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 1, (sScreenSize.m_iHeight / 2)  + 6, 3,1, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 1, (sScreenSize.m_iHeight / 2)  - 8, 3,1, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 1, (sScreenSize.m_iHeight / 2)  + 8, 3,1, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 2, (sScreenSize.m_iHeight / 2)  - 8, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 2, (sScreenSize.m_iHeight / 2)  + 6, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) + 2, (sScreenSize.m_iHeight / 2)  - 8, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) + 2, (sScreenSize.m_iHeight / 2)  + 6, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 6, (sScreenSize.m_iHeight / 2)  - 1, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) + 6, (sScreenSize.m_iHeight / 2)  - 1, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 8, (sScreenSize.m_iHeight / 2)  - 1, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) + 8, (sScreenSize.m_iHeight / 2)  - 1, 1,3, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 8, (sScreenSize.m_iHeight / 2)  - 2, 3,1, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) + 6, (sScreenSize.m_iHeight / 2)  - 2, 3,1, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 8, (sScreenSize.m_iHeight / 2)  + 2, 3,1, CHEAT_BLACK );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) + 6, (sScreenSize.m_iHeight / 2)  + 2, 3,1, CHEAT_BLACK );
	//LIL BLOCK
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 7, (sScreenSize.m_iHeight / 2)  - 1, 1,3, dwGetTeamColor );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) + 7, (sScreenSize.m_iHeight / 2)  - 1, 1,3, dwGetTeamColor );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 1, (sScreenSize.m_iHeight / 2)  - 7, 3,1, dwGetTeamColor );
	VoidCSS.g_pNeeded->FillRGBA( (sScreenSize.m_iWidth / 2) - 1, (sScreenSize.m_iHeight / 2)  + 7, 3,1, dwGetTeamColor );
}

void Esp::DrawMap( void )
{

	Vector vWorldPosition;	//see my name
	for ( int index = 0; index < VoidCSS.g_pEntList->GetHighestEntityIndex(); index++  )
	{
		//if ( index == VoidCSS.g_pEngClient->GetLocalPlayer() )
		//	continue;

		IClientEntity* ClientEntity =VoidCSS.g_pEntList->GetClientEntity( index );

		if (	ClientEntity == NULL
			||	ClientEntity->IsDormant() )
			continue;

		CBaseEntity* pBaseEntity = ClientEntity->GetBaseEntity();

		if (	pBaseEntity == NULL 
			||	pBaseEntity->IsDormant() || !pBaseEntity->IsAlive() || !pBaseEntity->IsPlayer())
			continue;

		int *iTeamIndex = (int*) ((DWORD)pBaseEntity +(DWORD)0x90 );

		DWORD dwGetTeamColor = CHEAT_WHITE;

		if ( VoidCSS.g_pAimbot->GetVisible( VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition(), pBaseEntity->EyePosition(), pBaseEntity ) )//visible
		{
			if( *iTeamIndex == 2 ) //Terrorist
				dwGetTeamColor = CHEAT_RED;

			if( *iTeamIndex == 3 )// Counter-Terrorist
				dwGetTeamColor = CHEAT_BLUE;

			if ( VoidCSS.g_pAimbot->iGetTarget() == index )//Aimed Guy
				dwGetTeamColor = CHEAT_PURPLE;
		}
		else//invisible (hidden guy)
		{
			if( *iTeamIndex == 2 ) //Terrorist
				dwGetTeamColor = CHEAT_ORANGE;

			if( *iTeamIndex == 3 )// Counter-Terrorist
				dwGetTeamColor = CHEAT_GREEN;

			if ( VoidCSS.g_pAimbot->iGetTarget() == index )//Aimed Guy
				dwGetTeamColor = CHEAT_BLACK;
		}
		
		VoidCSS.g_pNeeded->GetWorldSpaceCenter( pBaseEntity , vWorldPosition );
		int x = vWorldPosition.x / 10 + 400;
		int y = vWorldPosition.y / 10 + 400;
		VoidCSS.ColorBox(x - 5, y - 5,
								10, 10,
								RED(dwGetTeamColor), GREEN(dwGetTeamColor), BLUE(dwGetTeamColor), 255);
	}	
}

void Esp::DrawEsp( void )
{
	if (!VoidCSS.s_espbox)
		return;

	if ( !VoidCSS.g_pMyPlayer->BaseEnt() )
		return;

	player_info_t pInfo;	//Get Shit info About players
	Vector vScreen;			//see my name
	Vector vWorldPosition;	//see my name
	Vector vPlayerHead;		//see my name
	Vector vPlayersDistance;

	for ( int index = 0; index < VoidCSS.g_pEntList->GetHighestEntityIndex(); index++  )
	{
		if ( index == VoidCSS.g_pEngClient->GetLocalPlayer() )
			continue;

		IClientEntity* ClientEntity =VoidCSS.g_pEntList->GetClientEntity( index );

		if (	ClientEntity == NULL
			||	ClientEntity->IsDormant() )
			continue;

		CBaseEntity* pBaseEntity = ClientEntity->GetBaseEntity();

		if (	pBaseEntity == NULL 
			||	pBaseEntity->IsDormant() )
			continue;

		int *iTeamIndex = (int*) ((DWORD)pBaseEntity +(DWORD)0x90 );

		DWORD dwGetTeamColor = CHEAT_WHITE;

		if ( VoidCSS.g_pAimbot->GetVisible( VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition(), pBaseEntity->EyePosition(), pBaseEntity ) )//visible
		{
			if( *iTeamIndex == 2 ) //Terrorist
				dwGetTeamColor = CHEAT_RED;

			if( *iTeamIndex == 3 )// Counter-Terrorist
				dwGetTeamColor = CHEAT_BLUE;

			if ( VoidCSS.g_pAimbot->iGetTarget() == index )//Aimed Guy
				dwGetTeamColor = CHEAT_PURPLE;
		}
		else//invisible (hidden guy)
		{
			if( *iTeamIndex == 2 ) //Terrorist
				dwGetTeamColor = CHEAT_ORANGE;

			if( *iTeamIndex == 3 )// Counter-Terrorist
				dwGetTeamColor = CHEAT_GREEN;

			if ( VoidCSS.g_pAimbot->iGetTarget() == index )//Aimed Guy
				dwGetTeamColor = CHEAT_BLACK;
		}

		//get position
		VoidCSS.g_pNeeded->GetWorldSpaceCenter( pBaseEntity , vWorldPosition );
		
		//set position of player on screen
		if ( VoidCSS.g_pNeeded->WorldToScreen( vWorldPosition, vScreen ) )
		{
			//get player info
			if ( VoidCSS.g_pEngClient->GetPlayerInfo( index, &pInfo ) )
			{
				//Get Life State
				int *lifestate = (int*) ( ( DWORD )pBaseEntity + ( DWORD )0x87 );
				
				//Get Health
				int *pdwHealth = (int*) ( (DWORD)pBaseEntity + (DWORD)0xDA4 );
				
				//Draw if Alive
				if( *lifestate  == LIFE_ALIVE )
				{
					if ( VoidCSS.g_pNeeded->WorldToScreen( pBaseEntity->EyePosition(), vPlayerHead ) )
					{
						try
						{
							vPlayersDistance = VoidCSS.g_pMyPlayer->BaseEnt()->EyePosition() - pBaseEntity->EyePosition();
							vec_t t = SQUAREVECTOR(vPlayersDistance);
							float yyy = sqrt(t) / 100 * 0.2;
							//VoidCSS.g_pDebugoverlay->AddEntityTextOverlay(index, 1, 0.001, 255, 0, 0, 255, "%f", yyy);
						
							//Set Bounding Box color
							//VoidCSS.g_pMatSystemSurface->DrawSetColor( RGBA(dwGetTeamColor) );

							//Draw Bounding Box
							//VoidCSS.g_pMatSystemSurface->DrawOutlinedRect( vPlayerHead.x - 20, vPlayerHead.y - 20, vPlayerHead.x + 20, vPlayerHead.y + 60 );
							VoidCSS.ColorBox(vPlayerHead.x - 20 / yyy, vPlayerHead.y - 10 / yyy,
								40 / yyy, 80 / yyy,
								RED(dwGetTeamColor), GREEN(dwGetTeamColor), BLUE(dwGetTeamColor), 255);
						}
						catch(...){}
					}
					//Draw ESP Name
					//VoidCSS.g_pNeeded->DrawString( vScreen.x, vScreen.y, dwGetTeamColor, "%s", pInfo.name );
					
					//Draw ESP Health
					//VoidCSS.g_pNeeded->DrawString( vScreen.x, vScreen.y + 16 , dwGetTeamColor, "%i", *pdwHealth );
				}
			}
		}
	}
}