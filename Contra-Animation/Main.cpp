#include "icb_gui.h"


bool animation_paused = true;  // Animasyonu duraklatmak için yeni flag
HANDLE hThread = NULL;  // Thread handle
HANDLE th = NULL;


int F1, F2;
ICBYTES Map, Corridor, Agent, AgentX3;
ICBYTES BlueAgentCurrent;
ICBYTES RedAgentCurrent;
int x = 1, y = 10;
int BlueAgentX = 10;
int BlueAgentY = 250;
int RedAgentX = 30;
int RedAgentY = 250;
int Agent_state = 0;
int Agent_phase = 0;

void _WaitThread(HANDLE thread);
void _CreateThread(HANDLE thread, void* threadMain);

void ICGUI_Create() {
    ICG_MWSize(842, 672);
    ICG_MWTitle("Contra");
}

// 0 hide
// 1 stand
// 2 run
//currentScene

void SetState(int &oldState,int newState) {
    if (oldState != newState) {
    oldState = newState;
    Agent_phase = 0;
    }
    else {
        Agent_phase++;
    }
}

void PrintAgent(int x, int y, int state, int Agent_phase)
{
    if (state == 0) //hide
        return;

    else if (state == 1) //durma
    {
        Copy(AgentX3, 1, 20, 75, 109, BlueAgentCurrent);
    }
    else if (state == 2) //ko?ma
    {
            if (Agent_phase % 6 == 0) Copy(AgentX3, 1, 127, 55, 109, BlueAgentCurrent);
            if (Agent_phase % 6 == 1) Copy(AgentX3, 53, 127, 62, 109, BlueAgentCurrent);;
            if (Agent_phase % 6 == 2) Copy(AgentX3, 112, 127, 67, 109, BlueAgentCurrent);
            if (Agent_phase % 6 == 3) Copy(AgentX3, 175, 127, 54, 109, BlueAgentCurrent);
            //Contra takımına selam
            if (Agent_phase % 6 == 4) Copy(AgentX3, 230, 127, 56, 109, BlueAgentCurrent);
            if (Agent_phase % 6 == 5) Copy(AgentX3, 283, 127, 68, 109, BlueAgentCurrent);

    }
    else if (state == 3) { //takla
        if (Agent_phase % 4 == 0) { 
            Copy(AgentX3, 345, 150, 60, 75, BlueAgentCurrent);
            Copy(AgentX3, 832, 150, 60, 75, RedAgentCurrent);     //Red Flip 1
        }
        else if (Agent_phase % 4 == 1) { 
            Copy(AgentX3, 405, 150, 60, 75, BlueAgentCurrent);
            Copy(AgentX3, 772, 150, 60, 75, RedAgentCurrent);     //Red Flip 2
        }
        else if (Agent_phase % 4 == 2) { 
            Copy(AgentX3, 465, 150, 60, 75, BlueAgentCurrent);
            Copy(AgentX3, 712, 150, 60, 75, RedAgentCurrent);     //Red Flip 3
        }
        else if (Agent_phase % 4 == 3) { 
            Copy(AgentX3, 525, 150, 60, 75, BlueAgentCurrent);
            Copy(AgentX3, 652, 150, 60, 75, RedAgentCurrent);     //Red Flip 4
        }
    }
    
    //Copy(AgentX3, 832, 150, 60, 75, BlueAgentCurrent);     //Red Flip 1
    //Copy(AgentX3, 772, 150, 60, 75, BlueAgentCurrent);     //Red Flip 2
    //Copy(AgentX3, 712, 150, 60, 75, BlueAgentCurrent);     //Red Flip 3
    //Copy(AgentX3, 652, 150, 60, 75, BlueAgentCurrent);     //Red Flip 4

    //Copy(AgentX3, 237, 77, 100, 50, BlueAgentCurrent); //Mavi Eğilen 1
    //Copy(AgentX3, 337, 80, 100, 50, BlueAgentCurrent); // Mavi Eğilen 2 (Ateş etme)

    //Copy(AgentX3, 1160, 20, 75, 109, BlueAgentCurrent); // Kırmızı Duran
    //Copy(AgentX3, 1088, 20, 75, 109, BlueAgentCurrent); // Kırmız Duran (Ateş etme)

    //Paste(BlueAgentCurrent, BlueAgentX, BlueAgentY, Corridor);
    PasteNon0(BlueAgentCurrent, x, y, Corridor); // Screen
    PasteNon0(RedAgentCurrent, x+200, y, Corridor); // Screen
}

void* ScreenControllerThread(LPVOID lpParam)
{
    while (true)
    {
        Copy(Map, x, y, 800, 450, Corridor);
        // Arkaplan çizilecek

        //Mapteki Objeler Çizilecek

        //Karakterler Çizilecek
        PrintAgent(BlueAgentX, BlueAgentY, Agent_state, Agent_phase);
        // Projectilelar Çizilecek

        //Sahneyi Goruntule
        DisplayImage(F1, Corridor);
    }
}

// Animasyon fonksiyonu
void* LoadAgentRun(LPVOID lpParam) {
    SetState(Agent_state, 1);
    Sleep(1000);
    while (!animation_paused) { // Animasyon duraklatýlmamýþsa devam et
        x = 1; y = 10;BlueAgentX = 10;BlueAgentY = 250;
        int c = 1;
        Copy(Map, x, y, 800, 450, Corridor);

        SetState(Agent_state, 2);
        for (int i = 0; i < 12; i++) { //walk
            if (animation_paused)
                return NULL;
            x += 40;
            Sleep(150);
            Agent_phase++;
        }
        SetState(Agent_state, 3);
        for (int i = 0; i < 4; i++) {
            switch (Agent_phase) {
            case 0:
                BlueAgentX += 60;
                BlueAgentY -= 50;
                break;
            case 1:
                BlueAgentX += 60;
                BlueAgentY -= 50;
                break;
            case 2:
                BlueAgentX += 60;
                BlueAgentY += 50;
                break;
            case 3:
                BlueAgentX += 60;
                BlueAgentY -= 50;
                Agent_state = 1;
                break;
            }
            Agent_phase++;
            Sleep(250);
        }
        //Extra walk
        //SetState(Agent_state, 2);
        //for (int i = 0; i < 8; i++) { // walk
        //    if (animation_paused)
        //        return NULL;
        //    x += 40;
        //    Agent_phase++;
        //    Sleep(150);
        //}
        SetState(Agent_state, 1);
        Sleep(500);
    }
    return 0;
}

// Animasyonu baþlat ve durdur
void StartStopAnimation() {

    //
    animation_paused = !animation_paused;

    // Eðer daha önce bir thread baþlatýlmýþsa, önce onu sonlandýr
    _WaitThread(hThread);

    // Yeni thread baþlat
    _CreateThread(hThread, LoadAgentRun);
}

void ICGUI_main() {
    F1 = ICG_FrameThin(10, 130, 700, 430);
    ReadImage("sprites/mapShort.bmp", Map);
    Copy(Map, x, y, 800, 450, Corridor);
    DisplayImage(F1, Corridor);

    ReadImage("sprites/sprites.bmp", Agent);
    MagnifyX3(Agent, AgentX3);
   // DisplayImage(F1, Corridor);

    //BMP Pixel TEST
    Copy(Map, x, y, 800, 450, Corridor);
    //Copy(AgentX3, 345, 150, 60, 75, BlueAgentCurrent);   //Flip 1
    //Copy(AgentX3, 405, 150, 60, 75, BlueAgentCurrent);   //Flip 2
    //Copy(AgentX3, 465, 150, 60, 75, BlueAgentCurrent     //Flip 3
    //Copy(AgentX3, 525, 150, 60, 75, BlueAgentCurrent     //Flip 4
    //Copy(AgentX3, 525, 150, 60, 75, BlueAgentCurrent);     //Flip 4
    //PasteNon0(BlueAgentCurrent, BlueAgentX, BlueAgentY, Corridor);
    //DisplayImage(F1, Corridor);
    //Sleep(1000);

    F2 = ICG_FrameThin(10, 5, 20, 20);
    DisplayImage(F2, Agent);

    ICG_Button(544, 30, 160, 55, "Start/Stop Animation", StartStopAnimation);

    // Test
    _CreateThread(th, ScreenControllerThread);
}

void _WaitThread(HANDLE thread)
{
    if (thread != NULL) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    thread = NULL;
}

void _CreateThread(HANDLE thread, void* threadMain)
{
    thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadMain, NULL, 0, NULL);
}