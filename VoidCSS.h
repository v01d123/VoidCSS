#ifndef __VOIDCSS_H__
#define __VOIDCSS_H__

#include "sdk.h"
//=====================================
class LocalPlayer;
class CDrawLayer;
class Needed;
class CConsole;
class Esp;
class cNoSpread;
class cCAimbot;
//=====================================
class BaseHook
{
public:
	BaseHook::BaseHook();
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	void ColorBox(int x, int y, int w, int h, int r, int g, int b, int a);
	void ReadData(void);
	void WriteData(void);
	
	int		s_hMenuBool;
	int		s_hMenuInt;
	bool	s_ShowMenu;
	//4eat setings
	bool	s_aimbot;
	int		s_aimfov;
	bool	s_autoaim;
	bool    s_assshot;
	bool	s_wallhack;
	bool	s_espbox;

	CIniFile* g_ini;
	IGameResources* res;

	//CreateInterfaceFn f_Client;						//
	//CreateInterfaceFn Engine_Factory;				//
	IBaseClientDLL*			g_pBaseCDll;
	IVModelRender*			g_pMRender;
	IVEngineClient*			g_pEngClient;
	//IClientEntityList* p_ClientEntityList = NULL;	//
	DWORD orig_func;
	CClient g_oldClient;
	cl_modelrendfuncs_s g_oldClient2;


	LPDIRECT3DDEVICE9 g_D3Ddev;

	LPD3DXFONT Font1;

	//IGameResources* GetGameResources();

	ID3DXLine *pLine;

	//==========================================================================

	CreateInterfaceFn EngineFactory;
	CreateInterfaceFn ClientFactory;
	CreateInterfaceFn VGUIFactory;
	CreateInterfaceFn MaterialSys;

	cl_engfuncs_s			gEngineFuncs;
	CClient* 				gClientFuncs2;
	cl_modelrendfuncs_s		gModelRenderFuncs;

	IClientEntityList*		g_pEntList;
	IMatSystemSurface*		g_pDraw;
	vgui::ISurface*			g_pSurface;
	IVEfx*					g_pEffects;
	ICvar*					g_pCvar;
	IEngineTrace*			g_pEnginetrace;
	IVDebugOverlay*			g_pDebugoverlay;
	IVEngineCache*			g_pEngineCache;
	IVModelInfoClient*		g_pModelinfo;
	IEngineVGui*			g_pEnginevgui;
	CGlobalVarsBase*		g_pGlobals;
	CInput*					g_pInput;
	CUserMessages*			g_pUserMsg;
	vgui::IInputInternal*	g_pInputInt;
	IVModelRender*			g_pModelRender;
	IMaterialSystem*		g_pMaterialSystem;
	IVRenderView*			g_pRender;
	IMatSystemSurface*		g_pMatSystemSurface;

	PDWORD					g_pdwEngine;
	PDWORD					g_pdwClient;
	PDWORD					g_pdwModelRender;
	DWORD					g_dwOrgEngine;
	DWORD					g_dwOrgClient;
	DWORD					g_dwOrgModelRender;

	PDWORD*					g_pdwBaseCDll;
	PDWORD*					g_pdwMRender;

public://cheat class
	
	LocalPlayer*			g_pMyPlayer;		//My Entity class
	CDrawLayer*				g_pDrawzLayer;		//Panel Shit
	Needed*					g_pNeeded;			//Drawing Shit
	CConsole*				g_pGameConsole;		//Game Console
	Esp*					g_pEsp;				//cheat ESP
	cNoSpread*				g_pNoSpread;		//cheat NoSpread
	cCAimbot*				g_pAimbot;			//cheat Aimbot
}; 

extern BaseHook VoidCSS;

class LocalPlayer
{
public:
	//=====================================
	inline IClientEntity *Ent( void ) const
	{
		return VoidCSS.g_pEntList->GetClientEntity( VoidCSS.g_pEngClient->GetLocalPlayer() );
	}
	inline C_BaseEntity *BaseEnt( void ) const
	{
		return Ent()->GetBaseEntity();
	}
	inline IClientRenderable *RenderPlayer( void ) const
	{
		return Ent()->GetClientRenderable();
	}
	//=====================================
};

class CConsole
{
public:
	void Init( void )
	{
		DO_ONCE	
		(	
			VoidCSS.g_pGameConsole = NULL;
			memset(m_szBuf, 0, sizeof(m_szBuf));
		);
		
		if( VoidCSS.g_pGameConsole == NULL )
		{
			CreateInterfaceFn gameUIFactory = (CreateInterfaceFn)GetProcAddress( GetModuleHandle("gameui.dll"), "CreateInterface" );
			//VoidCSS.g_pGameConsole = (IGameConsole *)gameUIFactory("GameConsole003", NULL);
			add_log( "m_pGameConsole: [0x%.8X]", (DWORD)VoidCSS.g_pGameConsole );
		}
	}
	// returns true if console has focus
	//bool IsVisible ( void ){return VoidCSS.g_pGameConsole->IsConsoleVisible();}
	//void Activiate ( void ){VoidCSS.g_pGameConsole->Activate();}
	//void ColorPrint( int r, int g, int b, char* szText ){Color clr(r,g,b,255);VoidCSS.g_pGameConsole->ColorPrintf(clr,szText);}
	//void RandomColorPrint( char* szText ){Color clr((1 + (rand() % 254)),(1 + (rand() % 254)),(1 + (rand() % 254)),255);VoidCSS.g_pGameConsole->ColorPrintf(clr,szText);}
	//void Printf( const char *fmt, ... ){va_list	va_alist;va_start(va_alist, fmt);vsprintf(m_szBuf, fmt, va_alist);va_end(va_alist);/*Game console needs a \n else whatever is printed next is printed on the same line*/strcat(m_szBuf, "\n");VoidCSS.g_pGameConsole->Printf(m_szBuf);}
	//void ErrorPrintf( const char *fmt, ... ){va_list	va_alist;va_start(va_alist, fmt);vsprintf(m_szBuf, fmt, va_alist);va_end(va_alist);strcat(m_szBuf, "\n");Color clr(255,0,0,255);VoidCSS.g_pGameConsole->ColorPrintf(clr,m_szBuf);}
	//int iGetLineCount( void ){return m_iLineCount;}
private:
	// format buffer
	int	m_iLineCount;
	char m_szBuf[3064];
};


#endif