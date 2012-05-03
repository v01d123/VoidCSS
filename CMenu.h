

#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include <vector>
#include <iostream>
#include <voidcss.h>
#include <sstream>

class CDXMenu
{
private:
	LPD3DXFONT m_lpFont;
	LPDIRECT3DDEVICE9 m_lpDevice;

	enum D3D_COLOR
	{
		RED,
		BLUE,
		GREEN,
		YELLOW
	};

	struct Hack
	{
		char szName[32];
		bool *identifierBool;
		int	 *identifierInt;
		int	 *type;
	};

	std::vector<Hack> m_vHackList;
	int m_iTopLeftX;
	int m_iTopLeftY;
	int m_iSelection;

	LPD3DXFONT getFont();
	LPDIRECT3DDEVICE9 getDevice();

	void drawRectangle(int x, int y, int width, int height, D3D_COLOR color);
	void drawRectangle(int x, int y, int width, int height, int r, int g, int b);

	void drawBox(int x, int y, int width, int height, D3D_COLOR border, D3D_COLOR color);
	void drawBox(int x, int y, int width, int height, int ra, int ga, int ba, int r, int g, int b);

	void drawText(int x, int y, D3D_COLOR color, const char* szText);
	void drawText(int x, int y, int r, int g, int b, const char * szText);
public:
	CDXMenu( LPDIRECT3DDEVICE9 lpDevice, int topleftX, int topleftY );
	~CDXMenu( );

	void addHackBool(const char* szName, bool* onoffIdentifier);
	void addHackInt(const char* szName, int* onoffIdentifier);
	void showMenu(bool show);

	void updateMenu();
};

