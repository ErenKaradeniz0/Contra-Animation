#include<windows.h>
#include"icb_gui.h"

//#pragma comment (lib,"winmm.lib")

int F1, F2;
ICBYTES Corridor, Agent;
ICBYTES AgentStanding;//Ajan ayakta dururken 
//AJAN KOÞMA KARELERÝ ALTTAKÝ RESÝM DÝZÝNÝNE YÜKLENÝYOR
ICBYTES AgentRun[8]; // <--Agent Running sequence will be uploaded here
<<<<<<< Updated upstream
=======


int x = 120, y = 10;

int Agent_x = 10;

>>>>>>> Stashed changes
void ICGUI_Create()
{
	ICG_MWSize(768, 672);
	ICG_MWTitle("Contra");
}

void LoadAgentRun()
{
<<<<<<< Updated upstream
	Copy(Agent, 1, 1, 26, 44, AgentStanding);
	PasteNon0(AgentStanding, 10, 58, Corridor);
	ICBYTES cordinat{ {45, 9, 13,30},{71,9,16,30},{100,8,20,30},{130,8,23,30},{160,7,25,30},{189,8,22,30},
		{218,8,25,31} };
=======
	
	
	//Copy(Map, x1, 100, x2, 370, Corridor);
	
	//Copy(Agent, 70, 9, 84, 34, AgentStanding);

	
	Copy(AgentX3, 1, 127, 55, 109, AgentStanding); //Koþan 1
	//Copy(AgentX3, 53, 127, 62, 109, AgentStanding); //Koþan 2
	//Copy(AgentX3, 112, 127, 67, 109, AgentStanding); //Koþan 3



	PasteNon0(AgentStanding, 10, 250, Corridor);
	DisplayImage(F1, Corridor);

	Sleep(1000);
	
	
	//MagnifyX3(AgentStanding, TV);
	//SetPixelsX2(Agent, 0, 0, 0xcccccc, AgentStanding);
	//Impress12x20(AgentStanding, 0, 0, "x3", 0x000000);
	
	
	
	
	
	//Paste(TV, 10, 200, Corridor);
	
	//ICBYTES cordinat{ {45, 9, 13,30},{71,9,16,30},{100,8,20,30},{130,8,23,30},{160,7,25,30},{189,8,22,30},
		//{218,8,25,31} };
>>>>>>> Stashed changes
	//for (int i = 1; i <= cordinat.Y(); i++)
	//{
	//	Copy(Agent, cordinat.I(1,i), cordinat.I(2, i), cordinat.I(3, i), cordinat.I(4, i), AgentRun[i-1]);
	//	PasteNon0(AgentRun[i-1], 33*i, 58, Corridor);
	//}
<<<<<<< Updated upstream
	ICBYTES TV;
	MagnifyX3(Corridor, TV);
	DisplayImage(F1, TV);
=======
	
	
	//DisplayImage(F1, AgentStanding);
	
	

	
	
	
	int c = 0;
	for (int i = 0; i < 50; i++) {
		x += 40;
		Copy(Map, x, y, 800, 450, Corridor);
		if (c%2==0) {
			Copy(AgentX3, 53, 127, 62, 109, AgentStanding); //Koþan 2
			
		}
		else {
			Copy(AgentX3, 112, 127, 67, 109, AgentStanding); //Koþan 3
		}
		c++;
		Agent_x += 3;
		//Paste(AgentStanding, Agent_x, 250, Corridor);
		PasteNon0(AgentStanding, Agent_x, 250, Corridor);
		DisplayImage(F1, Corridor);
		Sleep(30);
	}
	
	
	

	DisplayImage(F1, Corridor);

	
	//DisplayImage(F1, TV);
	
	
>>>>>>> Stashed changes

}

//void MakeAgentRun(void *)
//{
//	int xcor=1, ycor=58;
//	ICBYTES TV,back;
//	int step[] = { 5,5,5,5,5,8,8 };
//	ReadImage("impossible_mission_elevator.bmp", Corridor);
//	Copy(Corridor, xcor, ycor, 25, 32, back);
//	PasteNon0(AgentStanding, xcor, ycor, Corridor);
//	MagnifyX3(Corridor, TV);
//	DisplayImage(F1, TV);
//	PlaySound("Another_Visitor.wav", NULL, SND_SYNC);
//	Paste(back, xcor, ycor, Corridor);
//	int i = 0;
//	while(true)
//	{
//		Copy(Corridor, xcor, ycor, 25, 32,back);
//		PasteNon0(AgentRun[i%7], xcor, ycor, Corridor);
//		MagnifyX3(Corridor, TV);
//		DisplayImage(F1, TV);
//#ifdef _DEBUG
//		Sleep(20);//DEBUG MODU YAVAÞ OLDUÐU ÝÇÝN DAHA AZ BEKLETÝYORUZ
//#else
//		Sleep(60); //Release mode is fast so we delay more
//#endif
//		Paste(back, xcor, ycor, Corridor);
//		if(i%7==4)PlaySound("Step.wav", NULL, SND_ASYNC);
//		xcor += step[i%7];
//		i++;
//		if (i > 52) {
//			xcor = 1; i = 0;
//		}
//	}
//}


void ICGUI_main()
{
	F1 = ICG_FrameThin(5, 140, 450, 430);
	ICG_Button(400, 5, 160, 55, "Start", LoadAgentRun);
	//ICG_TButton(580, 5, 160, 55, "Stop", NULL,NULL);
	ReadImage("sprites/map.bmp", Corridor);
	DisplayImage(F1, Corridor);
	ReadImage("sprites/sprites.bmp", Agent);

	//Show Sprite
	//F2 = ICG_FrameThin(5, 5, 20, 20);
	//DisplayImage(F2, Agent);
}