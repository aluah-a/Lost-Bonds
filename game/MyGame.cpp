#include "stdafx.h"
#include "MyGame.h"
#include <cmath>

CMyGame::CMyGame(void) : 
	startScreen(400,300,"startScreen.bmp",0)
	// to initialise more sprites here use a comma-separated list
{

}

CMyGame::~CMyGame(void)
{
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	// do not run game logic when in menu mode
	if (IsMenuMode()) return;

	Uint32 t = GetTime();

	// TODO: add the game update code here

	if (damagecounter > 0)
	{
		damagecounter--;
	}

	if (player.GetY() < 0) GameOver();
	PlayerControl();
	SlimeControl();
	forestnpcControl();


	// loads backgrounds 
	if (level == 1 && player.GetX() > 2300)
	{
		SetupLevel2();
		level = 2;
		player.SetPosition(100, player.GetY());
	}
	else if (level == 2 && player.GetX() > 2300)
	{
		SetupLevel3();
		level = 3;
		player.SetPosition(100, player.GetY());

	}
	else if (level == 3 && player.GetX() > 2300)
	{
		SetupLevel4();
		level = 4;
		player.SetPosition(100, player.GetY());
	}
	
	// loops the levels together so that you can walk to previous level
	
	if (level == 1 && player.GetX() < 50)
	{
		SetupLevel3();
		level = 3;
		player.SetPosition(2300, player.GetY());

	}
	else if (level == 3 && player.GetX() < 50)
	{
		SetupLevel2();
		level = 2;
		player.SetPosition(2300, player.GetY());
	}
	else if (level == 2 && player.GetX() < 50)
	{
		SetupLevel1();
		level = 1;
		player.SetPosition(2300, player.GetY());
	}

	//text for player
	if (level == 1 && player.GetX() <= 1300 && player.GetX() >= 1200)
	{
		textbox.SetImage("textboxplayer");
		textbox.SetPosition(player.GetX() + 100, player.GetY() + 200);
	}
	if (level == 1 && player.GetX() <= 1400 && player.GetX() >= 1300)
	{
		textbox.SetImage("textboxplayer1");
		textbox.SetPosition(player.GetX() + 100, player.GetY() + 200);
	}

	if (timer > 0)
	{
		timer--;
	}

	




	//goblin battle + big
	if (level == 2)
	{
		Bodyguard1.SetImage("guard");
		Bodyguard2.SetImage("guard");
		
		Bodyguard1.SetPosition(1750, 100 );
		Bodyguard2.SetPosition(1600, 150);
		//follows player
		swampmonster.SetDirection(player.GetPosition() - swampmonster.GetPosition());
		

		

		goblin1.SetSpeed(50);
		goblin1.SetDirection(player.GetPosition() - goblin1.GetPosition());	
		


		goblin.SetSpeed(50);
		goblin.SetDirection(player.GetPosition() - goblin.GetPosition());

	
		
	}
	//projectiles for big
	if (timer == 0 && level == 2)
	{


		for (CSprite* bolt : boltList)
		{
			if (bolt->GetY() < 0 || bolt->GetY() > 600 || bolt->GetX() < 0 || bolt->GetX() > 2400) bolt->Delete();

			bolt->SetDirection(player.GetPosition() - swampmonster.GetPosition());
		}
		boltList.delete_if(deleted);

		if (cooldown > 0)
		{
			cooldown--;
		}


		//creates bolt

		if (cooldown == 0)
		{
			CSprite* newbolt = new CSprite(swampmonster.GetX() + 15, swampmonster.GetY(), "bolt.png", GetTime());
			newbolt->SetDirection(player.GetPosition() - newbolt->GetPosition());
			newbolt->SetSpeed(500);
			boltList.push_back(newbolt);
			cooldown = 500;
		}

	}
	
	
	

	

	//removes sprites from levels they aren't on

	if (level != 1)
	{
		slime.Delete();
		forestnpc.Delete();
		textbox.Delete();

	}
	else 
	{
		slime.UnDelete();
		forestnpc.UnDelete();
		textbox.UnDelete();
	}

	//shows text
	if (player.HitTest(&forestnpc) == true)
	{
		forestnpc.SetSpeed(0);
		textbox.SetImage("textbox");
		textbox.SetPosition(player.GetX() +100, player.GetY() + 200);
		player.SetSpeed(0);

		
		
	}
	else if (player.HitTest(&forestnpc) == false)
		{
			forestnpc.SetSpeed(50);

		}	
	



	//removes enemies from level if their health gets to 0
	if (goblin.GetHealth() == 0)
	{
		
		goblin.Delete();

	}
	 if (swampmonster.GetHealth() == 0)
	 {	 
		 heart.SetPosition(swampmonster.GetPosition());
		 heart.SetImage("heart");

		swampmonster.Delete();
		boltList.delete_all();
	}
	 if (goblin1.GetHealth() <= 0)
	 {
		 goblin1.Delete();
	 }
	 if (Bodyguard1.GetHealth() == 0)
	 {
		 Bodyguard1.Delete();
	 }
	 if (Bodyguard2.GetHealth() == 0)
	 {
		 Bodyguard2.Delete();
	 }
	 if (Bug.GetHealth() == 0)
	 {
		 Bug.Delete();
		 bulletList.delete_all();
	 }
	 if (enemy.GetHealth() == 0)
	 {
		 enemy.Delete();
	 }

	 //projectiles for the player - rocks

	if (cooldown2 > 0 )
	{
		if (cooldown2 == 1)
		{
			CSprite* newrock = new CSprite(player.GetX()+15, player.GetY(), "Rock1.png", GetTime());
			newrock->SetDirection(90);
			newrock->SetSpeed(500);
			rockList.push_back(newrock);
		}
		cooldown2--;
	}
	
	//delets projectiles outside level 
	for (CSprite* rock : rockList)
	{
		if (rock->GetY() < 0 || rock->GetY() > 600 || rock->GetX() < 0 || rock->GetX() > 2400) rock->Delete();

		rock->SetDirection(90);
	}
	rockList.delete_if(deleted);

	//hit tests for rocks
	for (CSprite* rock : rockList)
	{
		if (rock->HitTest(&goblin) == true)
		{
			goblin.SetHealth(goblin.GetHealth() - 10);
			
			rock->Delete();
			

		}
		else if (rock->HitTest(&swampmonster) == true)
		{
			swampmonster.SetHealth(swampmonster.GetHealth() - 10);
			rock->Delete();
		}
		else if (rock->HitTest(&goblin1) == true)
		{

			goblin1.SetHealth(goblin1.GetHealth() - 10);
			rock->Delete();
		}
		else if (rock->HitTest(&Bodyguard1) == true)
		{

			Bodyguard1.SetHealth(Bodyguard1.GetHealth() - 10);
			rock->Delete();
		}
		else if (rock->HitTest(&Bodyguard2) == true)
		{

			Bodyguard2.SetHealth(Bodyguard2.GetHealth() - 10);
			rock->Delete();
		}
		else if (rock->HitTest(&Bug) == true)
		{
			Bug.SetHealth(Bug.GetHealth() - 10);
			rock->Delete();
		}
		else if (rock->HitTest(&enemy) == true)
		{
			enemy.SetHealth(enemy.GetHealth() - 10);
			rock->Delete();
		}

	}
	rockList.delete_if(deleted);

	//hit test on bolts to player
	for (CSprite* bolt : boltList)
	{
		if (bolt->HitTest(&player) == true)
		{
			player.SetHealth(player.GetHealth() - 20);

			bolt->Delete();

		}

	}
	boltList.delete_if(deleted);

	//frees ally when all enemies are dead
	if (goblin.IsDeleted() == true && goblin1.IsDeleted() == true && swampmonster.IsDeleted() == true && Bodyguard1.IsDeleted() == true && Bodyguard1.IsDeleted() == true && level == 2)
	{
		textbox.UnDelete();
		textbox.SetImage("ally");
		textbox.SetPosition(player.GetX() + 100, player.GetY() + 200);

	}
	
	//timer
	if (timer2 > 0)
	{
		timer2--;
	}
	
//level 3 mechanics
	if (level == 3)
	{
		ally.Delete();
		textbox.Delete();
		enemy.SetDirection(player.GetPosition() - enemy.GetPosition());

		Bug.SetDirection(player.GetPosition() - Bug.GetPosition());
		ally2.SetPosition(450, 100);
		if (timer2 == 0 && level == 3)
		{


			for (CSprite* bullet : bulletList)
			{
				if (bullet->GetY() < 0 || bullet->GetY() > 600 || bullet->GetX() < 0 || bullet->GetX() > 2400) bullet->Delete();

				bullet->SetDirection(player.GetPosition() - Bug.GetPosition());
			}
			bulletList.delete_if(deleted);




			//bullet shots after 100ms

			if (timer2 == 0)
			{
				CSprite* bullet = new CSprite(Bug.GetX() + 15, swampmonster.GetY(), "Bullet1.png", GetTime());
				bullet->SetDirection(player.GetPosition() - bullet->GetPosition());
				bullet->SetSpeed(500);
				bulletList.push_back(bullet);
				timer2 = 100;



			}

			
		}
	}
	// level4

	if (level == 4)
	{
		ally.UnDelete();
		
	
		ally.SetPosition(450, 100);
		ally2.SetPosition(400, 100);

	
		textbox.SetImage("ally2");
		textbox.SetPosition(player.GetX() + 100, player.GetY() + 200);

		
		textbox.UnDelete();
		textbox.SetImage("ally2text");
		textbox.SetPosition(player.GetX() + 100, player.GetY() + 200);
		
		if (player.GetX() > 600)
		{
			GameOver();

		}
	}


	



	//bullet hit test
	for (CSprite* bullet : bulletList)
	{
		if (bullet->HitTest(&player) == true)
		{
			player.SetHealth(player.GetHealth() - 10);

			bullet->Delete();

		}

	}
	bulletList.delete_if(deleted);

	//gameover when level enemies are cleared
	if (Bug.IsDeleted() && enemy.IsDeleted())
	{
		
	}

	if (Bug.IsDeleted() == true)
	{
		bulletList.delete_all();
	}
	


	if (heart.HitTest(&player))
	{
		player.SetHealth(150);
		heart.Delete();
	}



	//colisions with player - takes away 10 HP - enemies who dont have ranged weapon
	if (slime.HitTest(&player) == true || enemy.HitTest(&player) || Bug.HitTest(&player) || goblin.HitTest(&player) || goblin1.HitTest(&player))
	{
		if (damagecounter == 0)
		{
			player.SetHealth(player.GetHealth() - 10);
			//slime.SetX(player.GetX()-35);
			//float angle = atan2(slime.GetY() - player.GetX());
			damagecounter = 60;
			
		}



	}
	//player death if health is 0

	if (player.GetHealth() == 0)
	{
		{
			player.SetAnimation("die");

			GameOver();
			PauseGame();

		}
	}



	// updating all sprites
	player.Update(t);
	
	slime.Update(t);
	background.Update(t);
	forestnpc.Update(t);
	textbox.Update(t);
	goblin.Update(t);
	goblin1.Update(t);
	swampmonster.Update(t);
	Bodyguard1.Update(t);
	Bodyguard2.Update(t);
	Bodyguard3.Update(t);
	ally.Update(t);
	ally2.Update(t);
	Bug.Update(t);
	enemy.Update(t);
	heart.Update(t);

	
	for (CSprite* rock : rockList) rock->Update(GetTime());
	for (CSprite* bolt : boltList) bolt->Update(GetTime());
	for (CSprite* bullet : bulletList) bullet->Update(GetTime());

	rockList.delete_if(deleted);

	

}


void CMyGame::OnDraw(CGraphics* g)
{
	// draw menu when in menu mode
	if (IsMenuMode())
	{
		startScreen.Draw(g);
		playbutton.Draw(g);
		playbutton.SetPosition(400 ,500);
		return;
	}



	// ----- scrolling -------------------------------
    // game world (background image) is of size 2400x600
	int leftScreenLimit = 300;
	int rightScreenLimit = 1900; // 2400-800+300
	int scrolloffset = 0;
	
	// do not scroll if left to the left limit
	if (player.GetX() < leftScreenLimit)
	{
		scrolloffset = 0;
		g->SetScrollPos(0, 0);  
	}
    // we scroll the whole game world according to the player position
	// unless we reached the left or right screen limits
	else if (player.GetX() >= leftScreenLimit && player.GetX() <= rightScreenLimit)
	{
		scrolloffset = leftScreenLimit - player.GetX();
		g->SetScrollPos(leftScreenLimit - player.GetX(), 0);
	}
	// we stop scrolling with the player once we have reached the right limit
	// and remain in that fixedscrolling position at the right limit
	else if (player.GetX() > rightScreenLimit)
	{
		scrolloffset = leftScreenLimit - rightScreenLimit;
		g->SetScrollPos(leftScreenLimit-rightScreenLimit, 0);
	}



	
	//draws all sprites
	background.Draw(g);
	slime.Draw(g);
	player.Draw(g);
	forestnpc.Draw(g);
	textbox.Draw(g);
	swampmonster.Draw(g);
	goblin.Draw(g);
	goblin1.Draw(g);
	Bodyguard1.Draw(g);
	Bodyguard2.Draw(g);
	Bodyguard3.Draw(g);
	ally.Draw(g);
	ally2.Draw(g);
	Bug.Draw(g);
	enemy.Draw(g);
	heart.Draw(g);

	for (CSprite* rock : rockList) rock->Draw(g);
	for (CSprite* bolt : boltList) bolt->Draw(g);
	for (CSprite* bullet : bulletList) bullet->Draw(g);

	// don't scroll the overlay screen

	*g << font(28) << color(CColor::Green()) << xy(player.GetX()-20, player.GetY()+30) << player.GetHealth();
	if (goblin.IsDeleted() == false && level == 2)
	{
		*g << font(28) << color(CColor::Green()) << xy(goblin.GetX() - 20, goblin.GetY() + 30) << goblin.GetHealth();
	}
	if (swampmonster.IsDeleted() == false && level == 2)
	{
		*g << font(28) << color(CColor::Green()) << xy(swampmonster.GetX() - 20, swampmonster.GetY() + 30) << swampmonster.GetHealth();
	}
	if (goblin1.IsDeleted() == false && level == 2)
	{
		*g << font(28) << color(CColor::Green()) << xy(goblin1.GetX() - 20, goblin1.GetY() + 30) << goblin1.GetHealth();
	}
	if (Bodyguard1.IsDeleted() == false && level == 2)
	{
		*g << font(28) << color(CColor::Green()) << xy(Bodyguard1.GetX() - 20, Bodyguard1.GetY() + 30) << Bodyguard1.GetHealth();
	}	
	if (Bodyguard2.IsDeleted() == false && level == 2)
	{
		*g << font(28) << color(CColor::Green()) << xy(Bodyguard2.GetX() - 20, Bodyguard2.GetY() + 30) << Bodyguard2.GetHealth();
	}
	if (Bug.IsDeleted() == false && level == 3)
	{
		*g << font(28) << color(CColor::Green()) << xy(Bug.GetX() - 20, Bug.GetY() + 30) << Bug.GetHealth();
	}
	if (enemy.IsDeleted() == false && level == 3)
	{
		*g << font(28) << color(CColor::Green()) << xy(enemy.GetX() - 20, Bug.GetY() + 30) << enemy.GetHealth();
	}



	 g->SetScrollPos(0, 0);
	 if (IsGameOverMode())  *g << font(40) << color(CColor::Green()) << xy(400, 300) << "Game Over";
	
	 
}

void CMyGame::PlayerControl()
{

	float hwalkSpeed = 500; // horizontal walking speed
	float vwalkSpeed = 500; // vertical walking speed

	float lowerlimit = 30;  // setting the limits
	float upperlimit = 230;
	float leftlimit = 30;
	float rightlimit = 2400-30;
	
	enum animation_state { NONE, STANDLEFT, STANDRIGHT, WALKLEFT, WALKRIGHT, STANDUP, STANDDOWN, WALKUP, WALKDOWN, ATTACK};
	/* 
	  player status variable is used to indicate which state the player is in

	  0: None
	  1: standing left looking
	  2: standing right looking
	  3: walking left
	  4: walking right
	  5:standing DOWN looking
	  6:standing UP looking
	  7:walking DOWN
	  8:walking UP
	*/

	// preserving the current status
    int old_animation_status = player.GetStatus();

    
	// resetting the player to standing each time and then
	// changing this according to state and keyboard input
    player.SetMotion(0, 0);
	if (player.GetStatus()==WALKLEFT) player.SetStatus(STANDLEFT);
	if (player.GetStatus()==WALKRIGHT) player.SetStatus(STANDRIGHT);
	if (player.GetStatus() == ATTACK) player.SetStatus(STANDRIGHT);

	// setting the speeds and animation status according to the keyboard controls
	if (IsKeyDown(SDLK_RIGHT)) { player.SetXVelocity(hwalkSpeed); player.SetStatus(WALKRIGHT);}
	if (IsKeyDown(SDLK_LEFT)) { player.SetXVelocity(-hwalkSpeed); player.SetStatus(WALKLEFT); }
	if (IsKeyDown(SDLK_UP)) { player.SetYVelocity(vwalkSpeed); player.SetStatus(WALKUP); }
	if (IsKeyDown(SDLK_DOWN)) { player.SetYVelocity(-vwalkSpeed); player.SetStatus(WALKDOWN); }
	
	
	


	if (IsKeyDown(SDLK_UP))
	{
		if (player.GetStatus() == STANDUP) player.SetStatus(WALKUP);
		player.SetYVelocity(hwalkSpeed);
	}
	if (IsKeyDown(SDLK_DOWN))
	{
		if (player.GetStatus() == STANDDOWN) player.SetStatus(WALKDOWN);
		player.SetYVelocity(-hwalkSpeed);
	}
	if (IsKeyDown(SDLK_x))
	{
		player.SetStatus(ATTACK);

	}


	// changing animation sequences if different from previous status
	if (player.GetStatus() != old_animation_status)
	{
		if (player.GetStatus() == WALKLEFT) player.SetAnimation("walkleft", 10);
		if (player.GetStatus() == WALKRIGHT) player.SetAnimation("walkright", 10);
		if (player.GetStatus() == STANDRIGHT) player.SetAnimation("standright" ,4);
		if (player.GetStatus() == STANDLEFT) player.SetAnimation("standleft", 4);
		if (player.GetStatus() == ATTACK) player.SetAnimation("throw", 6);
	}

	// setting the player movement limits
	if (player.GetY() > upperlimit) player.SetY(upperlimit);
	if (player.GetY() < lowerlimit) player.SetY(lowerlimit);
	if (player.GetX() < leftlimit) player.SetX(leftlimit);
	if (player.GetX() > rightlimit) player.SetX(rightlimit);

	player.Update(GetTime());





	
}

void CMyGame::SlimeControl()
{

}
void CMyGame::heartC()
{

}


void CMyGame::forestnpcControl()
{
	if (forestnpc.GetX() >= (2000))
	{
		forestnpc.SetDirection(270);
		forestnpc.SetSpeed(50);
		forestnpc.SetAnimation("walkleft");
	}
	else if (forestnpc.GetX() <= (1700))
	{
		forestnpc.SetDirection(90);
		forestnpc.SetSpeed(50);
		forestnpc.SetAnimation("walkright");

	}


}

void CMyGame::forestnpcspeech()
{
	
}

void CMyGame::GoblinControl()
{
	goblin.SetPosition(1000, 200);
	//follows player	
	
	goblin.SetDirection(player.GetPosition() - goblin.GetPosition());

	goblin.SetAnimation("walkleft");

}

void CMyGame::GoblinControl1()
{
	//follows 	goblin1.SetAnimation("walkleft");
	goblin1.SetPosition(1300, 200);


	goblin1.SetAnimation("walkleft");
	goblin1.SetDirection(player.GetPosition() - goblin1.GetPosition());

	
	
}

void CMyGame::GoblinControl12()
{

}

void CMyGame::allyControl()
{
	
	ally.SetPosition(1600, 100);
	ally.SetAnimation("walkleft");
	
	
	

}
void CMyGame::ally2Control()
{
	
	
    ally2.SetPosition(1600, 50);
	ally2.SetAnimation("walkleft");


}

void CMyGame::bookControl()
{
	
}

void CMyGame::bigControl()
{
	swampmonster.SetPosition(1200, 200);
	swampmonster.SetDirection(player.GetPosition() - swampmonster.GetPosition());
	swampmonster.SetAnimation("walkleft");
	
	swampmonster.SetSpeed(25);
	
}

void CMyGame::BugControl()
{
	Bug.SetPosition(1200, 200);
	Bug.SetDirection(player.GetPosition() - swampmonster.GetPosition());
	Bug.SetAnimation("walkleft");

	Bug.SetSpeed(55);


}

void CMyGame::enemyControl()
{
	enemy.SetPosition(1800, 200);
	enemy.SetDirection(player.GetPosition() - enemy.GetPosition());
	enemy.SetAnimation("walkleft");

	enemy.SetSpeed(55);

}



/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{   
	// loading the game map


	// configuring the animations for the player sprite
	player.LoadImage("right.png", "walkright", 6, 1, 0, 0, CColor::Black());
	player.AddImage("right.png", "walkright", 6, 1, 0, 0, 5, 0, CColor::Black());

	player.LoadImage("left.png", "walkleft", 6, 1, 3, 0, CColor::Black());
	player.AddImage("left.png", "walkleft", 6, 1, 5, 0, 0, 0,CColor::Black());

	player.LoadImage("standingright.png", "standright", 4, 1, 0, 0, CColor::Black());
	player.AddImage("standingright.png", "standright", 4, 1, 0, 0, 3, 0, CColor::Black());

	player.LoadImage("standingleft.png", "standleft", 4, 1, 3, 0, CColor::Black());
	player.AddImage("standingleft.png", "standleft", 4, 1, 3, 0, 0, 0, CColor::Black());

	player.LoadImage("rockthrow.png", "throw", 6, 1, 0, 0, CColor::Black());
	player.AddImage("rockthrow.png", "throw", 6, 1, 0, 0, 5, 0, CColor::Black());

	player.LoadImage("die.png", "die", 8, 1, 0, 0, CColor::Black());
	player.AddImage("die.png", "die", 8, 1, 0, 0, 7, 0, CColor::Black());


	//GOBLIN LOAD
	 
	goblin.LoadImage("goblinleft.png", "standleft", 8, 1, 0, 0, CColor::Black());
	goblin.AddImage("goblinleft.png", "walkleft", 8, 1, 0, 0, 3, 0, CColor::Black());

	goblin.LoadImage("goblinright.png", "standright", 8, 1, 0, 0, CColor::Black());
	goblin.AddImage("goblinright.png", "walkright", 8, 1, 0, 0, 7, 0, CColor::Black());


	goblin1.LoadImage("goblinleft.png", "standleft", 8, 1, 0, 0, CColor::Black());
	goblin1.AddImage("goblinleft.png", "walkleft", 8, 1, 0, 0, 3, 0, CColor::Black());

	goblin1.LoadImage("goblinright.png", "standright", 8, 1, 0, 0, CColor::Black());
	goblin1.AddImage("goblinright.png", "walkright", 8, 1, 0, 0, 7, 0, CColor::Black());

	//slime animation
	
	slime.LoadImage("slime.bmp", "bounce", 6, 1, 0, 0, CColor::White());
	slime.AddImage("slime.bmp", "bounce", 6, 1, 0, 0, 5, 0, CColor::White());

	//forestnpc

	forestnpc.LoadImage("npcright.png", "standright", 4, 1, 2, 0, CColor::Black());
	forestnpc.AddImage("npcright.png", "walkright", 4, 1, 1, 0, 3, 0, CColor::Black());

	forestnpc.LoadImage("npcleft.png", "standleft", 4, 1, 2, 0, CColor::Black());
	forestnpc.AddImage("npcleft.png", "walkleft", 4, 1, 0, 0, 3, 0, CColor::Black());

	//text bubble for forest npc
	textbox.LoadImage("textbox.png", "textbox");
	textbox.AddImage("textbox.png", "textbox");
	textbox.LoadImage("textbox1.png", "ally2text");
	textbox.AddImage("textbox1.png", "ally2text");


	//text for ally
	textbox.LoadImage("allytext.png", "ally");
	textbox.AddImage("allytext.png", "ally");
	textbox.LoadImage("ally2text.png", "ally2");
	textbox.AddImage("ally2text.png", "ally2");

	//text for player
	textbox.LoadImage("textboxplayer.png", "textboxplayer");
	textbox.AddImage("textboxplayer.png", "textboxplayer");
	textbox.LoadImage("textboxplayer2.png", "textboxplayer1");
	textbox.AddImage("textboxplayer2.png", "textboxplayer1");

	
	player.SetImage("standright");
	player.SetStatus(2);
	
	//	slime.SetImage("bounce");

	bmusic.Volume(10);
	bmusic.Play("kirby.wav", -1);

	//boss 2
	swampmonster.LoadImage("Biwalk.png", "standleft", 6, 1, 0, 0, CColor::Black());
	swampmonster.AddImage("Biwalk.png", "walkleft", 6, 1, 0, 0, 5, 0, CColor::Black());


	Bodyguard1.LoadImage("Bodyguard.png", "guard");
	Bodyguard1.AddImage("Bodyguard.png", "guard");
	Bodyguard2.LoadImage("Bodyguard.png", "guard");
	Bodyguard2.AddImage("Bodyguard.png", "guard");
	Bodyguard3.LoadImage("Bodyguard.png", "guard");
	Bodyguard3.AddImage("Bodyguard.png", "guard");

	ally.LoadImage("right2.png", "walkright", 6, 1, 0, 0, CColor::Black());
	ally.AddImage("right2.png", "walkright", 6, 1, 5, 0, 0, 0, CColor::Black());

	ally.LoadImage("left2.png", "walkleft", 6, 1, 3, 0, CColor::Black());
	ally.AddImage("left2.png", "walkleft", 6, 1, 5, 0, 0, 0, CColor::Black());


	ally2.LoadImage("right3.png", "walkleft", 6, 1, 3, 0, CColor::Black());
	ally2.AddImage("right3.png", "walkleft", 6, 1, 5, 0, 0, 0, CColor::Black());

	heart.LoadImage("heart.png","heart");
	heart.AddImage("heart.png", "heart");
	
	Bug.LoadImage("Bugwalk.png", "standleft", 4, 1, 0, 0, CColor::Black());
	Bug.AddImage("Bugwalk.png", "walkleft", 4, 1, 0, 0, 3, 0, CColor::Black());

	enemy.LoadImage("LWalk.png", "standleft", 7, 1, 0, 0, CColor::Black());
	enemy.AddImage("LWalk.png", "walkleft", 7, 1, 0, 0, 6, 0, CColor::Black());


}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{   

	SetupLevel1();
	
}

void CMyGame::MainMenu()
{


}

void CMyGame::SetupLevel1()
{
	background.LoadImage("bg.png", "map1", CColor::Black());
	background.SetImage("map1"); background.SetPosition(1200, 300);



}


// setting up a level 
void CMyGame::SetupLevel2()
{

	background.LoadImage("bg2.jpg", "map2", CColor::Black());
	background.SetImage("map2"); background.SetPosition(1200, 300);
	GoblinControl1();
	GoblinControl();
	bigControl();
	allyControl();


}

void CMyGame::SetupLevel3()
{

	background.LoadImage("bg2.jpg", "map2", CColor::Black());
	background.SetImage("map2"); background.SetPosition(1200, 300);
	BugControl();
	enemyControl();
	ally2Control();
}

void CMyGame::SetupLevel4()
{
	background.LoadImage("bg4.png", "map4", CColor::Black());
	background.SetImage("map4"); background.SetPosition(1200, 300);
	allyControl();
	ally2Control();

}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
    player.SetPosition(100, 220);
	player.SetMotion(0, 0);
	player.SetAnimation("standright");
	player.SetStatus(2);

	
	slime.SetAnimation("bounce");
	slime.SetPosition(1000, 50);

	player.SetHealth(150);

	forestnpc.SetPosition(1650, 100);
	forestnpc.SetDirection(90);
	forestnpc.SetSpeed(50);
	forestnpc.SetAnimation("walkright1");

	textbox.LoadImage("textbox.png");	
	goblin.SetHealth(100);

	goblin1.SetHealth(100);
	swampmonster.SetHealth(150);
	enemy.SetHealth(100);

	Bodyguard1.SetHealth(50);
	Bodyguard2.SetHealth(50);

	Bug.SetHealth(150);
	enemy.SetHealth(50);

	rockList.delete_all();
	heartC();
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{	
	level = 1;
}

// called when the game is over
void CMyGame::OnGameOver()
{

	
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();

	if (sym == SDLK_F2)
		NewGame();

	if (sym == SDLK_F1)
	{
		GameOver();
	}


	if (sym == SDLK_x && cooldown2 == 0)
	{
		cooldown2 = 20;

	}
	
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{

}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	// start game with a left mouse button click
	if (IsMenuMode()) StartGame();
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
