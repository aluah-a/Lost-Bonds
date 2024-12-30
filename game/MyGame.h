#pragma once

class CMyGame : public CGame
{
	// Define sprites and other instance variables here
	CSprite player;	// player sprite
	CSprite startScreen;
    CSprite background;
	CSpriteList rockList;
	CSpriteList boltList;
	CSpriteList bulletList;
	CSprite Bodyguard1;
	CSprite Bodyguard2;
	CSprite Bodyguard3;
	CSprite ally;
	CSprite forestnpc;
	CSprite slime;
	CSprite playbutton;
	CSprite textbox;
	CSprite swampmonster;
	CSprite goblin;
	CSprite goblin1;
	CSprite Bodyguard;
	CSprite Bug;
	CSprite enemy;
	CSprite heart;
	CSprite ally2;
	CSoundPlayer bmusic;
	


	int level;
	int talk;
    int	damagecounter = 0;
	int	damagecounter2 = 0;
	int cooldown = 0;
	int cooldown2;
	int distance;
	int timer = 60;
	int win;
	int timer2 = 60;
	int crossval;
	int dotval;


	// my own member functions
void PlayerControl();
void SlimeControl();
void forestnpcControl();
void forestnpcspeech();
void GoblinControl();
void GoblinControl12();
void GoblinControl1();
void allyControl();
void ally2Control();
void BugControl();
void bookControl();
void bigControl();
void enemyControl();
void MainMenu();
void SetupLevel1();
void heartC();

void SetupLevel2();
void SetupLevel3();
void SetupLevel4();

bool isBattleArena = false;

public:
	CMyGame(void);
	~CMyGame(void);

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
