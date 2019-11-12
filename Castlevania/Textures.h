#pragma once
#include <map>
#include <d3dx9.h>
#include <string>

using namespace std;

/*
	Manage texture database
*/
class CTextures
{
	static CTextures * __instance;

	map<int, LPDIRECT3DTEXTURE9> textures;

public: 
	CTextures();
	void Add(int idTex, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(int idTex);

	static CTextures * GetInstance();
};