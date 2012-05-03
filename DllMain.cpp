#pragma once
#include "SharedHeaders.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "detours.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h> 
#include <string>
#include <fstream>
#include <strstream>
#include <string> 
#include "Color.h"
#include "igameresources.h"
#include "IClientEntityList.h"
#include "IClientEntity.h"
#include "cdll_int.h"
#include "engine\ivdebugoverlay.h"
//#include "c_baseentity.h"
#include "cbase.h"
#include "cclient.h"
//#include "HudUpdate.h"
//#include "modelrender.h"
//#include "drawmodel.h"
//#include "hook.h"
#include "engine\IVModelRender.h"
#include "vgui_controls\controls.h"
#include "vguimatsurface\IMATSYSTEMSURFACE.H"
#include "ivrenderview.h"
#include "iefx.h"
#include "ienginevgui.h"
#include "C_BaseAnimating.h"
#include "C_BasePlayer.h"
#include "view_shared.h"
#include "iviewrender.h"
#include "sdk.h"


class C_BaseEntity;
HMODULE DLLModule;

#pragma warning(disable:4100)  
#pragma warning(disable:4430)  
#pragma comment(lib,"detours.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")


bool StartDll = 1;
//bool WallHack = 0;
bool WallHackMenu = 0;

bool once = true;
LPD3DXFONT m_font;

//cCAimbot*				g_pAimbot = NULL;
////cNoSpread*				g_pNoSpread = NULL;
//CDrawLayer*				g_pDrawzLayer;




//==========================================================================

//functions
//void fillrgba(int x, int y, int w, int h, int r, int g, int b, int a)
//{
//    D3DXVECTOR2 vLine[2];
//    VoidCSS.pLine->SetWidth(w);
//    VoidCSS.pLine->SetAntialias( false );
//    VoidCSS.pLine->SetGLLines( true );
//    
//    vLine[0].x = x + w/2;
//    vLine[0].y = y;
//    vLine[1].x = x + w/2;
//    vLine[1].y = y + h;  
//
//    VoidCSS.pLine->Begin( );
//    VoidCSS.pLine->Draw( vLine, 2, D3DCOLOR_RGBA( r, g, b, a ) );
//    VoidCSS.pLine->End( );
//}

//A simple Box
void box(int x, int y, int w, int h, int r, int g, int b, int a)
{
    //Top bar
	VoidCSS.FillRGBA(x-2,y-2,w+4,2,255,255,255,200);

    //Left
    VoidCSS.FillRGBA(x-2,y,2,h,255,255,255,200);

    //Right
    VoidCSS.FillRGBA(x+w,y,2,h,255,255,255,200);

    //Bottom
    VoidCSS.FillRGBA(x-2,y+h,w+4,2,255,255,255,200);

    //Background
    VoidCSS.FillRGBA(x,y,w,h,r,g,b,a);
}

void menu(int x, int y, int w, int h, int r, int g, int b, int a, char text[100])
{
    //Title Top bar
    VoidCSS.FillRGBA(x-2,y-27,w+4,2,255,255,255,200);

    //Title Background
    VoidCSS.FillRGBA(x,y-25,w,23,255,0,0,a);

    //Title Left
    VoidCSS.FillRGBA(x-2,y-27,2,h,255,255,255,200);

    //Ttitle Right
    VoidCSS.FillRGBA(x+w,y-27,2,h,255,255,255,200);

    //Ttitle Text
//   zFont->DrawText((char*)text, x+2, y-22);

    //Top bar
    VoidCSS.FillRGBA(x-2,y-2,w+4,2,255,255,255,200);

    //Left
    VoidCSS.FillRGBA(x-2,y,2,h,255,255,255,200);

    //Right
    VoidCSS.FillRGBA(x+w,y,2,h,255,255,255,200);

    //Bottom
    VoidCSS.FillRGBA(x-2,y+h,w+4,2,255,255,255,200);

    //Background
    VoidCSS.FillRGBA(x,y,w,h,r,g,b,a);
}

bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
    for(;*szMask;++szMask,++pData,++bMask)
        if(*szMask=='x' && *pData!=*bMask ) 
            return false;

    return (*szMask) == NULL;
}

DWORD FindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask)
{
    for(DWORD i=0; i < dwLen; i++)
        if( bCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
            return (DWORD)(dwAddress+i);

    return 0;
} 

IGameResources* GetGameResources( )
{
    IGameResources* res;
    static DWORD funcadd = NULL;
    if( !funcadd )
        funcadd = FindPattern( (DWORD) GetModuleHandle( _T("client.dll") ), 0x2680C6, (PBYTE) "\xA1\x00\x00\x00\x00\x85\xC0\x74\x06\x05", "x????xxxxx" );

    typedef IGameResources* (*GGR_t) (void);
    GGR_t GGR = (GGR_t) funcadd;
    return GGR();
} 


void vDrawESP()
{
	//DrawEsp();
    // vars for colour
    int red, green, blue;

	if (VoidCSS.res == NULL)
	{
		VoidCSS.res = GetGameResources();
		return;
	}

    // loop through possible indexes.
    for(int i = 1; i < 33; i++)
    {

        //player_info_t pInfo;
		//p_EngineClient->GetPlayerInfo(i, &pInfo); // get player info

        IClientEntity *pEnt = NULL;
		pEnt = VoidCSS.g_pEntList->GetClientEntity(i);

        if(pEnt == NULL) // no entity found :O
        {
            continue;
        }

        C_BaseEntity *pBasePlayer = NULL;
        pBasePlayer = pEnt->GetBaseEntity();

        if(pBasePlayer == NULL)
        {
            continue;
        }

        if(pBasePlayer->m_iTeamNum == 3) // ct
        {
            red = 0;
            green = 0;
            blue = 255;
        }
        else if(pBasePlayer->m_iTeamNum == 2) // t
        {
            red = 255;
            green = 0;
            blue = 0;
        }

        //if(!pEnt->IsDormant())
        //{
        //    continue;
        //}

        //if(pBasePlayer && pBasePlayer->IsPlayer())
        //{

		//DrawHealthBar(pBasePlayer->m_iTeamNum, 10, 10, res->GetHealth(i));
		
		//g_pDebugoverlay->AddEntityTextOverlay(i, 1, 0.001, red, green, blue, 255, "%s", res->GetHealth(i));
		//}


    }
} 

void __stdcall Hooked_RenderView ( const CViewSetup &view, bool drawViewmodel ) 
{
	//ConMsg(0, "qweqweqwe\n");
	//	//g_oldClient.RenderView( view, nFlags, drawViewmodel );

	//	//IClientEntity *pEnt = NULL;
	//	//pEnt = g_pEntList->GetClientEntity(2);
 // //      C_BaseEntity *pBasePlayer = NULL;
 // //      pBasePlayer = pEnt->GetBaseEntity();
	//	//

	//	//CViewSetup myView;

	//	//g_pBaseCDll->GetPlayerView(myView);
	//	//CViewSetup mirrorView		= myView;

	//	//mirrorView..width			= 300;
	//	//mirrorView.height			= 200;
	//	//mirrorView.x				= 300 ;
	//	//mirrorView.y				= 100 ;;
	//	//mirrorView.angles			= pBasePlayer->EyeAngles();//C_BasePlayer::GetLocalPlayer()->GetBaseEntity()->EyeAngles();
	//	//mirrorView.angles.y			+= 180;
	//	//mirrorView.angles.x			-= mirrorView.angles.x*2;
	//	//mirrorView.fov				= 90.0f;
	//	//g_pBaseCDll->RenderViewEx(mirrorView,0, 0);
	//	//mirrorView.angles.y			-= 180;

	//	//g_oldClient.RenderView( mirrorView , nFlags, false );
	//IClientEntity *pEnt = NULL;
	//	pEnt = g_pEntList->GetClientEntity(1);
	//	if (pEnt == NULL)
	//		return;
	//	C_BaseEntity *pBasePlayer = NULL; 
	//	pBasePlayer = pEnt->GetBaseEntity();
	//	if (pBasePlayer == NULL)
	//		return;

	//	CViewSetup myView;
	//	if (g_pBaseCDll == NULL)
	//		return;
	//	g_pBaseCDll->GetPlayerView(myView);
	//	//g_oldClient.RenderView( myView , 4, true);
	//	CViewSetup mirrorView		= myView;

	//	mirrorView.width			= 300;
	//	mirrorView.height			= 200;
	//	mirrorView.x				= 300 ;
	//	mirrorView.y				= 100 ;;
	//	mirrorView.angles			= pBasePlayer->EyeAngles();//C_BasePlayer::GetLocalPlayer()->GetBaseEntity()->EyeAngles();
	//	mirrorView.angles.y			+= 180;
	//	mirrorView.angles.x			-= mirrorView.angles.x*2;
	//	mirrorView.fov				= 90.0f;
	//	//g_pBaseCDll->RenderViewEx(mirrorView,0, true);
	//	g_oldClient.RenderView(mirrorView, false);
}

void ModelHack()
{
	IMaterial* TTeam1 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\t_arctic\\t_arctic", "Model textures");
	IMaterial* TTeam2 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\t_guerilla\\t_guerilla", "Model textures");
	IMaterial* TTeam3 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\t_leet\\t_leet", "Model textures");
	IMaterial* TTeam4 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\t_phoenix\\t_phoenix", "Model textures");
	IMaterial* CTeam1 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\ct_gign\\ct_gign", "Model textures");
	IMaterial* CTeam2 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\ct_gsg9\\ct_gsg9", "Model textures");
	IMaterial* CTeam3 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\ct_sas\\ct_sas", "Model textures");
	IMaterial* CTeam4 = VoidCSS.g_pMaterialSystem->FindMaterial("models\\player\\ct_urban\\ct_urban", "Model textures");

	IMaterial* SmokeStack		=	VoidCSS.g_pMaterialSystem->FindMaterial( "SmokeStack", "ClientEffect textures" );
	IMaterial* FlashEffect		=	VoidCSS.g_pMaterialSystem->FindMaterial( "effects/flashbang", "ClientEffect textures" );
	IMaterial* FlashOverlay		=	VoidCSS.g_pMaterialSystem->FindMaterial( "effects/flashbang_white", "ClientEffect textures" );
	IMaterial* SmokeParticle	=	VoidCSS.g_pMaterialSystem->FindMaterial( "particle/particle_smokegrenade", "ClientEffect textures" );
	IMaterial* SmokeParticle1	=	VoidCSS.g_pMaterialSystem->FindMaterial( "particle/particle_smokegrenade1", "ClientEffect textures" );
	IMaterial* ScreenSpaceFog	=	VoidCSS.g_pMaterialSystem->FindMaterial( "particle/screenspace_fog", "ClientEffect textures" );

	if(VoidCSS.s_wallhack)
	{

		//C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer(); 

		TTeam1->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );		
		TTeam2->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );		
		CTeam1->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );		
		CTeam2->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );		
		TTeam3->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );		
		TTeam4->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );	
		CTeam3->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
		CTeam4->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );

		FlashEffect->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,	  true );
		FlashOverlay->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,	  true );
		SmokeStack->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,	  true );
		SmokeParticle->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,  true );
		SmokeParticle1->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
		ScreenSpaceFog->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
	}
	else
	{
		TTeam1->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		TTeam2->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		CTeam1->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		CTeam2->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		TTeam3->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		TTeam4->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		CTeam3->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		CTeam4->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );

		FlashEffect->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,	  false );
		FlashOverlay->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,	  false );
		SmokeStack->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,	  false );
		SmokeParticle->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW,  false );
		SmokeParticle1->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, false );
		ScreenSpaceFog->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, false );
		
	}
}

void xDrawModel( int flags, IClientRenderable *cliententity, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld, const matrix3x4_t *pLightingOffset )
{
	ModelHack();
	//try
	//{
	//	
	//	IClientEntity *pEnt = NULL;
	//	pEnt = g_pEntList->GetClientEntity(1);
	//	if (pEnt == NULL)
	//		return;
	//	C_BaseEntity *pBasePlayer = NULL; 
	//	pBasePlayer = pEnt->GetBaseEntity();
	//	if (pBasePlayer == NULL)
	//		return;

	//	CViewSetup myView;
	//	if (g_pBaseCDll == NULL)
	//		return;
	//	g_pBaseCDll->GetPlayerView(myView);
	//	//g_oldClient.RenderView( myView , 4, true);
	//	CViewSetup mirrorView		= myView;

	//	mirrorView.width			= 300;
	//	mirrorView.height			= 200;
	//	mirrorView.x				= 300 ;
	//	mirrorView.y				= 100 ;;
	//	mirrorView.angles			= pBasePlayer->EyeAngles();//C_BasePlayer::GetLocalPlayer()->GetBaseEntity()->EyeAngles();
	//	mirrorView.angles.y			+= 180;
	//	mirrorView.angles.x			-= mirrorView.angles.x*2;
	//	mirrorView.fov				= 90.0f;
	//	//g_pBaseCDll->RenderViewEx(mirrorView,0, true);
	//	g_pBaseCDll->RenderView(mirrorView, false);
	//	//mirrorView.angles.y			-= 180;

	//	//g_oldClient.RenderView( mirrorView, 4, true);
	//}
	//catch(...){}
}


int	__stdcall Hooked_DrawModel( int flags, IClientRenderable *cliententity, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld, const matrix3x4_t *pLightingOffset )
{
	_asm { pushad }
		xDrawModel( flags, cliententity, instance, entity_index, model, origin, angles, skin, body, hitboxset, modelToWorld, pLightingOffset );
	_asm { popad }
	//return g_oldClient2.DrawModel( flags, cliententity, instance, entity_index, model, origin, angles, skin, body, hitboxset, modelToWorld, pLightingOffset );

	return 1;
}

int	__stdcall new_Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals )
{
	//g_AppSysFactory = appSystemFactory;
	VoidCSS.g_pGlobals = pGlobals;

	return VoidCSS.g_oldClient.Init( appSystemFactory, physicsFactory, pGlobals );
}


void __stdcall new_CreateMove ( int sequence_number, float input_sample_frametime, bool active ) 
{
	
	VoidCSS.g_oldClient.CreateMove( sequence_number , input_sample_frametime , active );
	ModelHack();

	if ( VoidCSS.g_pInput )
	{
		CUserCmd* cmd = VoidCSS.g_pInput->GetUserCmd( sequence_number );		

		if ( VoidCSS.g_pEngClient->IsInGame() )
		{
			//Aimbot Norecoil Nospread
			if( cmd->buttons & IN_ATTACK && VoidCSS.s_aimbot)
				VoidCSS.g_pAimbot->AimAtTarget( cmd );
			
			//BunnyHop
			if ( cmd->buttons & IN_JUMP )
			{
				int *iMyFlAgs = (int*) ( (DWORD)VoidCSS.g_pMyPlayer->BaseEnt() + (DWORD)0x2B4 );
				int iFlags = *iMyFlAgs;

				if( !(iFlags &FL_ONGROUND) )
					cmd->buttons &= ~IN_JUMP;
			}
		}
	}

	//VoidCSS.g_pNeeded->FontInit();
	//vgui::VPANEL VisualPanels = VoidCSS.g_pEnginevgui->GetPanel( PANEL_INGAMESCREENS );
	//VoidCSS.g_pDrawzLayer->Create( VisualPanels );  
}

void __stdcall HudTextHook( const char * message )
{
	ConMsg(0, message);
}

void __stdcall new_HudUpdate(bool bActive)
{
    static int NumberOfCalls = 1;
    if (NumberOfCalls <= 1)
    {
		ConMsg(0, "HudUpdateHook Called: %d!\n", NumberOfCalls);
        NumberOfCalls++;

		
		//init panel

    } 
	
    VoidCSS.g_oldClient.HudUpdate(bActive);
}

void Text(LPD3DXFONT Font, int x, int y, int Red, int Green, int Blue, int Alpha, const char *text, ...) 
{
	//setup the uFormat parameters
	D3DCOLOR fontColor = D3DCOLOR_ARGB(Alpha, Red, Green, Blue);

	//setup rectA
	RECT rctA;
	rctA.left=x;
	rctA.top=y;
	rctA.right=rctA.left+1000;
	rctA.bottom=rctA.top+1000;

	va_list va_alist;
	char logbuf[256] = {0};
	va_start (va_alist, text);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), text, va_alist);
	va_end (va_alist);

	//draw the text
	Font->DrawTextA(NULL, logbuf, -1, &rctA, 0, fontColor );
}


//typedef HRESULT (WINAPI* HudUpdate_)(bool bActive);
//HudUpdate_ gHudUpdate;
typedef HRESULT (WINAPI* iPresent) (LPDIRECT3DDEVICE9 nDevice,CONST RECT * pSourceRect,CONST RECT * pDestRect,HWND hDestWindowOverride,CONST RGNDATA * pDirtyRegion);
iPresent cPresent;
typedef HRESULT (WINAPI* iEndScene) (LPDIRECT3DDEVICE9 nDevice);
iEndScene cEndScene;
typedef HRESULT(WINAPI* DrawIndexedPrimitive_)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);
DrawIndexedPrimitive_ pDrawIndexedPrimitive;


HRESULT WINAPI fPresent ( LPDIRECT3DDEVICE9 nDevice,CONST RECT * pSourceRect,CONST RECT * pDestRect,HWND hDestWindowOverride,CONST RGNDATA * pDirtyRegion )
{
	VoidCSS.g_D3Ddev = nDevice;
	//D3DXCreateFontA(nDevice, 22, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_MODERN, "Arial", &Font1);
	return cPresent ( nDevice,pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion );
}

CHAR ttt[MAX_PATH];

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if( FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, 0)) )
	return E_FAIL;

	WORD colour16 = ((WORD)((colour32>>28)&0xF)<<12)
	|(WORD)(((colour32>>20)&0xF)<<8)
	|(WORD)(((colour32>>12)&0xF)<<4)
	|(WORD)(((colour32>>4)&0xF)<<0);

	D3DLOCKED_RECT d3dlr;
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;

	for(int xy=0; xy < 8*8; xy++)
	*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}

LPDIRECT3DTEXTURE9 Red,Yellow,Green,Blue,Purple,Pink,Orange = NULL;

CDXMenu* g_pCMenu;
bool menu_setup = false;
HRESULT WINAPI fEndScene ( LPDIRECT3DDEVICE9 nDevice )
{
	_asm pushad

	try
	{
		if (VoidCSS.s_espbox)
		{
			if (VoidCSS.pLine == NULL)
				D3DXCreateLine(nDevice, &VoidCSS.pLine);
			else
			{
				VoidCSS.g_pEsp->DrawEsp();
				VoidCSS.g_pEsp->DrawMap();
			}
		}

		//g_pSurface->DrawSetColor(255, 123, 123, 255);
		//g_pSurface->DrawOutlinedRect(10, 10, 600, 500);
		if (Red == NULL)
		{
			GenerateTexture(nDevice, &Red, D3DCOLOR_ARGB (255 , 255 , 0 , 0 ));
			GenerateTexture(nDevice, &Yellow, D3DCOLOR_ARGB (255 , 255 , 255 , 0 ));
			GenerateTexture(nDevice, &Green, D3DCOLOR_ARGB (255 , 0 , 255 , 0 ));
			GenerateTexture(nDevice, &Blue, D3DCOLOR_ARGB (255 , 0 , 0 , 255 ));
			GenerateTexture(nDevice, &Purple, D3DCOLOR_ARGB (255 , 102 , 0 , 153 ));
			GenerateTexture(nDevice, &Pink, D3DCOLOR_ARGB (255 , 255 , 20 , 147 ));
			GenerateTexture(nDevice, &Orange, D3DCOLOR_ARGB (255 , 255 , 165 , 0 ));
		}

		D3DVIEWPORT9 viewP;
		nDevice->GetViewport( &viewP );
		DWORD ScreenCenterX = viewP.Width / 2;
		DWORD ScreenCenterY = viewP.Height / 2;
		D3DRECT rec16 = {ScreenCenterX-20, ScreenCenterY, ScreenCenterX+ 20, ScreenCenterY+1};
		D3DRECT rec17 = {ScreenCenterX, ScreenCenterY-20, ScreenCenterX+ 1,ScreenCenterY+20};  
		nDevice->Clear( 1, &rec16, D3DCLEAR_TARGET, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0,  0 );//white
		nDevice->Clear( 1, &rec17, D3DCLEAR_TARGET, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), 0,  0 );

		//Text(Font1,Viewport.Width/2, Viewport.Height/2, 255, 0, 0, 255, "Seregin 4it workeDDD!!!");

		if(menu_setup == false) // Our menu has not been set up yet, lets set it up.
		{
			g_pCMenu = new CDXMenu( nDevice, 10, 160 ); // Create the menu, with pDevice (the device pointer), the 10 and 160 represent the top left corner position of our menu, in this case X 10, Y 160 on the screen.

			// We add the hacks and match them up with their respective bool variables
			g_pCMenu->addHackBool( "WallHack", &VoidCSS.s_wallhack );
			g_pCMenu->addHackBool( "Aimbot", &VoidCSS.s_aimbot );
			g_pCMenu->addHackInt( "AimBotFOV", &VoidCSS.s_aimfov );
			g_pCMenu->addHackBool( "AssShot", &VoidCSS.s_assshot );
			g_pCMenu->addHackBool( "ESPBox", &VoidCSS.s_espbox );

			menu_setup = true; // we finished setting up, so lets set this to true so we dont end up here again.
		}

		if(GetAsyncKeyState( VK_END ) &1) // If we pressed END
		{
			VoidCSS.s_ShowMenu = !VoidCSS.s_ShowMenu;
		}

		g_pCMenu->showMenu( VoidCSS.s_ShowMenu );
		g_pCMenu->updateMenu();
	}
	catch(...){}

	_asm popad
	return cEndScene ( nDevice );
}


HRESULT WINAPI nDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex,UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
{  
	//IDirect3DVertexBuffer9* pStreamData = NULL; 
	//UINT iOffsetInBytes,iStride;  
	//pDevice->GetStreamSource(0,&pStreamData,&iOffsetInBytes,&iStride); 
	//if (iStride == 64 && WallHack == 1)
	//{	
	//	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_NEVER);

	//	//if (NumVertices >= 0 && NumVertices <1000)
	//	//	pDevice->SetTexture(0,Red);

	//	//if (NumVertices >= 1000 && NumVertices <2000)
	//	//	pDevice->SetTexture(0,Yellow);

	//	//if (NumVertices >= 2000  && NumVertices < 3000)
	//	//	pDevice->SetTexture(0,Green);

	//	//if (NumVertices > 3000)
	//	//	pDevice->SetTexture(0,Blue);

	//	//if (NumVertices == 71)
	//	//	pDevice->SetTexture(0,Purple);

	//	//if (NumVertices == 192)
	//	//	pDevice->SetTexture(0,Pink);

	//	//if (NumVertices == 500)
	//	//	pDevice->SetTexture(0,Orange);

	//	//pDevice->SetTexture(0,Red);
	//	//pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	//	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHSRCALPHA);
	//	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);
	//	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	//}
  
	return pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}


int StartHack ();

void SetupVariables()
{
	HMODULE hModuleClient		= NULL;
	HMODULE hModuleEngine		= NULL;
	HMODULE hModuleVGUIFactory	= NULL;
	HMODULE hMaterialSysFactory = NULL;

	while(!hModuleClient)
	{
		hModuleClient		= GetModuleHandle("client.dll");
		hModuleEngine		= GetModuleHandle("engine.dll");
		hModuleVGUIFactory	= GetModuleHandle("vguimatsurface.dll");
		hMaterialSysFactory	= GetModuleHandle("materialsystem.dll");
		Sleep(100);
	}

	VoidCSS.ClientFactory	= (CreateInterfaceFn)GetProcAddress(hModuleClient, "CreateInterface");
	VoidCSS.EngineFactory	= (CreateInterfaceFn)GetProcAddress(hModuleEngine, "CreateInterface");
	VoidCSS.VGUIFactory		= (CreateInterfaceFn)GetProcAddress(hModuleVGUIFactory, "CreateInterface");
	VoidCSS.MaterialSys		= (CreateInterfaceFn)GetProcAddress(hMaterialSysFactory, "CreateInterface");
	

	VoidCSS.g_pdwEngine		= (DWORD*)				VoidCSS.EngineFactory( VENGINE_CLIENT_INTERFACE_VERSION , NULL );
	VoidCSS.g_pdwClient		= (DWORD*)				VoidCSS.ClientFactory( CLIENT_DLL_INTERFACE_VERSION , NULL );
	VoidCSS.g_pdwModelRender = (DWORD*)				VoidCSS.EngineFactory( VENGINE_HUDMODEL_INTERFACE_VERSION, NULL );
	//IEngineVGui* pEngineVgui        = (IEngineVGui*)        EngineFactory( VENGINE_VGUI_VERSION, NULL);  
	//VoidCSS.g_pRender = (IVRenderView*)EngineFactory("VEngineRenderView012", NULL);

	VoidCSS.g_pEntList		= (IClientEntityList*)	VoidCSS.ClientFactory( VCLIENTENTITYLIST_INTERFACE_VERSION , NULL );

	VoidCSS.g_pEngClient	= (IVEngineClient*)		VoidCSS.EngineFactory("VEngineClient012", NULL);
	//bool good = vgui::VGui_InitInterfacesList("vguimatsurface.dll", &VoidCSS.VGUIFactory, 1);  
	VoidCSS.g_pSurface		= (vgui::ISurface *)	VoidCSS.VGUIFactory	( "VGUI_Surface028", NULL ); //VGUI_Surface028
	VoidCSS.g_pMatSystemSurface = ( IMatSystemSurface* )VoidCSS.g_pSurface->QueryInterface( "MatSystemSurface005" ); 
	VoidCSS.g_pDraw			= (IMatSystemSurface*)	VoidCSS.g_pSurface->QueryInterface(MAT_SYSTEM_SURFACE_INTERFACE_VERSION);
	//g_pMaterialSystem = ( IMaterialSystem* )MaterialSys( "VMaterialSystem079", NULL ); 

	VoidCSS.g_pEffects		= (IVEfx*)				VoidCSS.EngineFactory( VENGINE_EFFECTS_INTERFACE_VERSION, NULL );
	VoidCSS.g_pCvar			= (ICvar*)				VoidCSS.EngineFactory( VENGINE_CVAR_INTERFACE_VERSION, NULL );
	VoidCSS.g_pEnginetrace	= (IEngineTrace*)		VoidCSS.EngineFactory( INTERFACEVERSION_ENGINETRACE_CLIENT, NULL );
	VoidCSS.g_pDebugoverlay	= (IVDebugOverlay*)		VoidCSS.EngineFactory( VDEBUG_OVERLAY_INTERFACE_VERSION, NULL );
	VoidCSS.g_pEngineCache	= (IVEngineCache*)		VoidCSS.EngineFactory( VENGINE_CACHE_INTERFACE_VERSION, NULL );
	VoidCSS.g_pModelinfo	= (IVModelInfoClient*)	VoidCSS.EngineFactory( VMODELINFO_CLIENT_INTERFACE_VERSION, NULL );
	VoidCSS.g_pEnginevgui	= (IEngineVGui*)		VoidCSS.EngineFactory( VENGINE_VGUI_VERSION, NULL );
	VoidCSS.g_pModelRender	= (IVModelRender*)		VoidCSS.EngineFactory( VENGINE_HUDMODEL_INTERFACE_VERSION, NULL );
	VoidCSS.g_pRender		= (IVRenderView*)       VoidCSS.EngineFactory( VENGINE_RENDERVIEW_INTERFACE_VERSION, NULL);

	VoidCSS.g_pMaterialSystem = (IMaterialSystem *) VoidCSS.MaterialSys( MATERIAL_SYSTEM_INTERFACE_VERSION, NULL );

	VoidCSS.g_pBaseCDll		= (IBaseClientDLL*)		VoidCSS.ClientFactory(CLIENT_DLL_INTERFACE_VERSION, NULL);
	VoidCSS.g_pMRender		= (IVModelRender*)		VoidCSS.EngineFactory(VENGINE_HUDMODEL_INTERFACE_VERSION, NULL);

	VoidCSS.g_pdwBaseCDll = (PDWORD*)VoidCSS.g_pBaseCDll;
	VoidCSS.g_pdwMRender  = (PDWORD*)VoidCSS.g_pMRender;

	DWORD dwOld;
	
	memcpy( (void*) &VoidCSS.g_oldClient,(void*)*VoidCSS.g_pdwBaseCDll , sizeof(CClient) );
	VoidCSS.gClientFuncs2 = (CClient*)*VoidCSS.g_pdwBaseCDll;

	VirtualProtect( (LPVOID)&((CClient*)*VoidCSS.g_pdwBaseCDll)->Init, 4, PAGE_EXECUTE_READWRITE, &dwOld );
	VoidCSS.gClientFuncs2->Init = &new_Init;

	VirtualProtect( (LPVOID)&((CClient*)*VoidCSS.g_pdwBaseCDll)->CreateMove, 4, PAGE_EXECUTE_READWRITE, &dwOld );
	VoidCSS.gClientFuncs2->CreateMove = &new_CreateMove;

	VirtualProtect( (LPVOID)&((CClient*)*VoidCSS.g_pdwBaseCDll)->HudUpdate, 4, PAGE_EXECUTE_READWRITE, &dwOld );
	//((CClient*)*g_pdwBaseCDll)->HudUpdate = &HudUpdateHook;
	VoidCSS.gClientFuncs2->HudUpdate = &new_HudUpdate;

	if ( VoidCSS.g_pInput == NULL )
	{
		PDWORD pdwAddress = ( PDWORD ) ((( DWORD ) VoidCSS.g_oldClient.CreateMove ) + 0x20 );
		PDWORD pdwTable = ( PDWORD ) *pdwAddress;
		VoidCSS.g_pInput = ( CInput* ) *pdwTable;
	}

	VoidCSS.g_pGlobals = ( CGlobalVarsBase* )0x200D01A4;//0x20365E80
	//PDWORD pdwAddress2 = ( PDWORD ) ((( DWORD ) g_oldClient.CreateMove ) + 0x39 );
	//PDWORD pdwTable2 = ( PDWORD ) *pdwAddress2;
	//g_pGlobals = ( CGlobalVarsBase* ) *pdwTable2;

	////memcpy( (void*) &g_oldClient,(void*)*g_pdwBaseCDll , sizeof(CClient) );
	//VirtualProtect( (LPVOID)&((CClient*)*VoidCSS.g_pdwBaseCDll)->RenderView, 4, PAGE_EXECUTE_READWRITE, &dwOld );
	////((CClient*)*g_pdwBaseCDll)->RenderView = &Hooked_RenderView;
	//VoidCSS.gClientFuncs2->RenderView = & Hooked_RenderView;


	//memcpy( (void*) &g_oldClient2,(void*)*g_pdwMRender , sizeof(cl_modelrendfuncs_s) );
	//VirtualProtect( (LPVOID)&((cl_modelrendfuncs_s*)*g_pdwMRender)->DrawModel, 4, PAGE_EXECUTE_READWRITE, &dwOld );
	//((cl_modelrendfuncs_s*)*g_pdwMRender)->DrawModel = &Hooked_DrawModel;

	//VoidCSS.g_pMyPlayer = new LocalPlayer();
	//VoidCSS.g_pAimbot =	new cCAimbot();
	//g_pNoSpread = new cNoSpread();
}

DWORD MainThread ( LPVOID lpArgs )
{
    HMODULE hClient = NULL;
    for ( ; hClient == NULL ; Sleep(100) )
        hClient = GetModuleHandle("client.dll");

	CreateInterfaceFn IGCCreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandle("gameui.dll"), "CreateInterface");

    IGameConsole *m_pIGameConsole = (IGameConsole *)IGCCreateInterface(GAMECONSOLE_INTERFACE_VERSION, NULL);
    //if (m_pIGameConsole == NULL)
    //    MessageBox(0, "Error: m_pIGameConsole is null!", "Error", MB_OK);
	if (m_pIGameConsole->IsConsoleShown() == false)
        m_pIGameConsole->Show();
	
	ConMsg(0, "Seregin 4it workeDDD!!!\n");

    //Sleep(5000);
    //FreeLibraryAndExitThread(DLLModule, 0);


	//===========================================================================
	//===========================================================================

	SetupVariables();

	DWORD* VTable;
    DWORD D3D9ModuleBase;

    do {
    D3D9ModuleBase = (DWORD)GetModuleHandle(_T("d3d9.dll"));
	Sleep(100);
    } while(!D3D9ModuleBase);

    DWORD DevicePTR = FindPattern(D3D9ModuleBase,0x128000,(PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86","xx????xx????xx");
    memcpy(&VTable,(void*)(DevicePTR+2),4); 
	//FARPROC fp = GetProcAddress(GetModuleHandle("engine.dll"), "CreateInterface");

	VoidCSS.res = GetGameResources( );
	if (VoidCSS.res != NULL)
	{
		const char *p0 =  VoidCSS.res->GetPlayerName(3);
		const char *p1 =  VoidCSS.res->GetPlayerName(4);
		const char *p2 =  VoidCSS.res->GetPlayerName(5);
	}

	pDrawIndexedPrimitive = (DrawIndexedPrimitive_)DetourFunction((PBYTE)VTable[82],(PBYTE)nDrawIndexedPrimitive);
	cPresent = (iPresent) DetourFunction((PBYTE)VTable[17], (PBYTE)fPresent);
	cEndScene = (iEndScene) DetourFunction((PBYTE)VTable[42], (PBYTE)fEndScene);

	for (;;Sleep (500))
	{
		if (GetAsyncKeyState (VK_F12) != 0) 
		{
			Beep (2000,200);
			VoidCSS.s_wallhack = !VoidCSS.s_wallhack;
		}

		if (GetAsyncKeyState (VK_INSERT)!= 0) 
		{
			Beep (3000,200);
			VoidCSS.s_espbox = !VoidCSS.s_espbox;
		}
		
		if (GetAsyncKeyState (VK_DELETE)!= 0) 
		{
			Beep (4000,200);
			VoidCSS.s_aimbot = !VoidCSS.s_aimbot;
		}

		//if (GetAsyncKeyState (VK_HOME) != 0) 
		//{

			//int i = g_pEngClient->GetLocalPlayer();
			//IClientEntity *pEnt = NULL;
			//pEnt = g_pEntList->GetClientEntity(i);
	  //      C_BaseEntity *pBasePlayer = NULL;
			//pBasePlayer = pEnt->GetBaseEntity();
			//
			//pLocalPlayer->SetFOV(pLocalPlayer, 20, 0.2f);
			//VoidCSS.g_pMyPlayer->BaseEnt()->SetGravity(100);
		//}

	}

	return 1;
}


BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved )
{
    if( dwReason == 1 )
    {
        DLLModule = hinstDLL;
        DWORD dwThreadID;

		//SetupModelRenderHooks();
		CreateThread( NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, &dwThreadID);

		//LaunchThread();
	}

    return TRUE;
}