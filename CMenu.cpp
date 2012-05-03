

#include "CMenu.h"

CDXMenu::CDXMenu(LPDIRECT3DDEVICE9 lpDevice, int topleftX, int topleftY)
{
	m_iSelection = 0;
	m_iTopLeftX = topleftX;
	m_iTopLeftY = topleftY;
	D3DXCreateFont(lpDevice, 14, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &m_lpFont);
	m_lpDevice = lpDevice;
}

CDXMenu::~CDXMenu()
{
	getFont()->Release();
}

LPDIRECT3DDEVICE9 CDXMenu::getDevice()
{
	return m_lpDevice;
}

LPD3DXFONT CDXMenu::getFont()
{
	return m_lpFont;
}

void CDXMenu::drawRectangle(int x, int y, int width, int height, D3D_COLOR color)
{
	D3DCOLOR Color;
	D3DRECT Rectangle = { x, 
						  y, 
						  x + width, 
						  y + height 
						}; 

	switch(color)
	{
	case RED:
		Color = D3DCOLOR_XRGB(255, 0, 0);
		break;
	case BLUE:
		Color = D3DCOLOR_XRGB(0, 0, 255);
		break;
	case GREEN:
		Color = D3DCOLOR_XRGB(0, 255, 0);
		break;
	case YELLOW:
		Color = D3DCOLOR_XRGB(255, 255, 0);
		break;
	default:
		Color = D3DCOLOR_XRGB(255, 0, 0);
		break;
	}

	getDevice()->Clear(1, &Rectangle, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 0,  0); 
}

void CDXMenu::drawRectangle(int x, int y, int width, int height, int r, int g, int b)
{
	D3DCOLOR Color;
	D3DRECT Rectangle = { x, 
						  y, 
						  x + width, 
						  y + height 
						}; 

	Color = D3DCOLOR_XRGB(r, g, b);

	getDevice()->Clear(1, &Rectangle, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 0,  0); 
}

void CDXMenu::drawBox(int x, int y, int width, int height, D3D_COLOR border, D3D_COLOR color)
{
	drawRectangle(x - 1, y - 1, width + 1, height + 1, border); 
	drawRectangle(x, y, width - 1, height - 1, color);
}

void CDXMenu::drawBox(int x, int y, int width, int height, int ra, int ba, int ga, int r, int g, int b)
{
	drawRectangle(x - 1, y - 1, width + 1, height + 1, ra, ba, ga); 
	drawRectangle(x, y, width - 1, height - 1, r, g, b);
}

void CDXMenu::drawText(int x, int y, CDXMenu::D3D_COLOR color, const char * szText)
{
	D3DCOLOR Color;
	RECT Rectangle = {x, y, 1000+x, 1000+y};

	switch(color)
	{
	case RED:
		Color = D3DCOLOR_XRGB(255, 0, 0);
		break;
	case BLUE:
		Color = D3DCOLOR_XRGB(0, 0, 255);
		break;
	case GREEN:
		Color = D3DCOLOR_XRGB(0, 255, 0);
		break;
	case YELLOW:
		Color = D3DCOLOR_XRGB(255, 255, 0);
		break;
	default:
		Color = D3DCOLOR_XRGB(255, 0, 0);
		break;
	}

	getFont()->DrawTextA(NULL, szText, -1, &Rectangle, 0, Color );
}

void CDXMenu::drawText(int x, int y, int r, int g, int b, const char * szText)
{
	D3DCOLOR Color;
	RECT Rectangle = {x, y, 1000+x, 1000+y};
	
	Color = D3DCOLOR_XRGB(r, g, b);

	getFont()->DrawTextA(NULL, szText, -1, &Rectangle, 0, Color );
}

void CDXMenu::addHackBool(const char* szName, bool* onoffIdentifier)
{
	Hack hack;

	if(strlen(szName) > 32)
		return;

	memset(hack.szName,0,32);
	strcpy(hack.szName, szName);

	hack.type = &VoidCSS.s_hMenuBool;
	hack.identifierBool = onoffIdentifier;

	m_vHackList.push_back(hack);
}

void CDXMenu::addHackInt(const char* szName, int* onoffIdentifier)
{
	Hack hack;

	if(strlen(szName) > 32)
		return;

	memset(hack.szName,0,32);
	strcpy(hack.szName, szName);

	hack.type = &VoidCSS.s_hMenuInt;
	hack.identifierInt = onoffIdentifier;

	m_vHackList.push_back(hack);
}

void CDXMenu::showMenu(bool show)
{
	if(!show)
	{
		m_iSelection = 0;
		return;
	}

	for(int i = 0; i < m_vHackList.size(); i++)
	{
		drawBox(m_iTopLeftX, m_iTopLeftY + (i * 16) + 5, 122, 15, 255, 255, 255, 87, 5, 9 );

		Hack hack = (Hack)m_vHackList[i];

		if(m_iSelection == i)
			drawText(m_iTopLeftX+2 ,m_iTopLeftY + (i * 16) + 6, 255,128,64,	hack.szName);
		else
			drawText(m_iTopLeftX+2 ,m_iTopLeftY + (i * 16) + 6, 255,255,255,hack.szName);

		if (*hack.type == 0)
		{
			if(*hack.identifierBool == false)
				drawText(m_iTopLeftX+5+100-12 ,m_iTopLeftY + (i * 16) + 6, 255,128,64,	"OFF");
			else
				drawText(m_iTopLeftX+5+100-12 ,m_iTopLeftY + (i * 16) + 6, 255,255,255,	"ON");
		}

		if (*hack.type == 1)
		{
			std::string s;
			std::stringstream outt;
			outt << *hack.identifierInt;
			s = outt.str();
			//atoi(VoidCSS.g_ini->GetKeyValue("aim", "aimbot").c_str());
			drawText(m_iTopLeftX+5+100-12 ,m_iTopLeftY + (i * 16) + 6, 255,255,255,	s.c_str());
		}

	}
}

void CDXMenu::updateMenu()
{
	if(GetAsyncKeyState(VK_UP) == -32767)
	{
		if(m_iSelection > 0)
			m_iSelection--;
	}
	if(GetAsyncKeyState(VK_DOWN) == -32767)
	{
		if(m_iSelection < 6)
			m_iSelection++;
	}

	if(GetAsyncKeyState(VK_RIGHT) == -32767)
	{
		Hack hack = (Hack)m_vHackList[m_iSelection];
		if (*hack.type == 1)
			*hack.identifierInt = *hack.identifierInt + 1;
		else
			*hack.identifierBool = !*hack.identifierBool;
	}

	if(GetAsyncKeyState(VK_LEFT) == -32767)
	{
		Hack hack = (Hack)m_vHackList[m_iSelection];
		if (*hack.type == 1)
			*hack.identifierInt = *hack.identifierInt - 1;
		else
			*hack.identifierBool = !*hack.identifierBool;
	}
}