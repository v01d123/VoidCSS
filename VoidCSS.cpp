#include "voidcss.h"

BaseHook	VoidCSS;

BaseHook::BaseHook()
{
	VoidCSS.g_ini =						new CIniFile();
	ReadData();
	VoidCSS.s_ShowMenu = false;
	WriteData();
	//VoidCSS.g_pGameConsole =			new CConsole();
	VoidCSS.g_pDrawzLayer =				new CDrawLayer();
	VoidCSS.g_pMyPlayer =				new LocalPlayer();
	VoidCSS.g_pNeeded =					new Needed();	
	VoidCSS.g_pEsp =					new Esp();
	
	VoidCSS.g_pAimbot =					new cCAimbot();
	//VoidCSS.g_pNoSpread =				new cNoSpread();
}

void BaseHook::ReadData(void)
{
	VoidCSS.s_hMenuBool = 0;
	VoidCSS.s_hMenuInt = 1;
	VoidCSS.g_ini->Load("C:\\CSSHack\\cl_dll\\bin\\setings.ini");

	VoidCSS.s_aimbot = atoi(VoidCSS.g_ini->GetKeyValue("aim", "aimbot").c_str());
	VoidCSS.s_aimfov = atof(VoidCSS.g_ini->GetKeyValue("aim", "aimbotfov").c_str());
	VoidCSS.s_autoaim = atoi(VoidCSS.g_ini->GetKeyValue("aim", "autoaim").c_str());
	VoidCSS.s_assshot = atoi(VoidCSS.g_ini->GetKeyValue("ass", "assshot").c_str());
	VoidCSS.s_wallhack = atoi(VoidCSS.g_ini->GetKeyValue("wh", "wallhack").c_str());
	VoidCSS.s_espbox = atoi(VoidCSS.g_ini->GetKeyValue("esp", "box").c_str());
	VoidCSS.s_espbox = 0;
}


void BaseHook::WriteData(void)
{
	VoidCSS.g_ini->SetKeyValue("aim", "aimbot", "1");
	VoidCSS.g_ini->SetKeyValue("aim", "aimbotfov", "15");
	VoidCSS.g_ini->SetKeyValue("aim", "autoaim", "1");
	VoidCSS.g_ini->SetKeyValue("ass", "assshot", "1");
	VoidCSS.g_ini->SetKeyValue("wh", "wallhack", "1");
	VoidCSS.g_ini->SetKeyValue("esp", "box", "0");

	VoidCSS.g_ini->Save("C:\\CSSHack\\cl_dll\\bin\\setings.ini");
}

void BaseHook::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
    D3DXVECTOR2 vLine[2];
    VoidCSS.pLine->SetWidth(w);
    VoidCSS.pLine->SetAntialias( false );
    VoidCSS.pLine->SetGLLines( true );
    
    vLine[0].x = x + w/2;
    vLine[0].y = y;
    vLine[1].x = x + w/2;
    vLine[1].y = y + h;  

    VoidCSS.pLine->Begin( );
    VoidCSS.pLine->Draw( vLine, 2, D3DCOLOR_RGBA( r, g, b, a ) );
    VoidCSS.pLine->End( );
}

void BaseHook::ColorBox(int x, int y, int w, int h, int r, int g, int b, int a)
{
    FillRGBA(x-2,y-2,w+4,2,r,g,b,a);
    FillRGBA(x-2,y,2,h,r,g,b,a);
    FillRGBA(x+w,y,2,h,r,g,b,a);
    FillRGBA(x-2,y+h,w+4,2,r,g,b,a);
	
}