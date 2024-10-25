#include "icb_gui.h"


bool animation_paused = true;  // Animasyonu duraklatmak için yeni flag
HANDLE hThread = NULL;  // Thread handle
HANDLE th = NULL;


int F1, F2;
ICBYTES Map, Corridor, Agent, AgentX3;
ICBYTES AgentCurrent;
int x = 1, y = 10;
int Agent_x = 10;
int Agent_y = 250;
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

void PrintAgent(int x, int y, int state, int Agent_phase)
{
    if (state == 0) //hide
        return;

    else if (state == 1) //durma
    {
        Copy(AgentX3, 1, 127, 55, 109, AgentCurrent);
    }
    else if (state == 2) //animasyon
    {
        if (Agent_phase % 5 == 0) Copy(AgentX3, 53, 127, 62, 109, AgentCurrent);
        if (Agent_phase % 5 == 1) Copy(AgentX3, 112, 127, 67, 109, AgentCurrent);
        if (Agent_phase % 5 == 2) Copy(AgentX3, 175, 127, 54, 109, AgentCurrent);
        if (Agent_phase % 5 == 3) Copy(AgentX3, 230, 127, 56, 109, AgentCurrent);
        if (Agent_phase % 5 == 4) Copy(AgentX3, 283, 127, 68, 109, AgentCurrent);

    }
    else if (state == 3) {

        if (Agent_phase % 4 == 0) {
            Agent_x += 60;
            Agent_y -= 50;
            Copy(AgentX3, 345, 150, 60, 75, AgentCurrent);     //Flip 1
        }
        if (Agent_phase % 4 == 1) {
            Agent_x += 60;
            Agent_y -= 50;
            Copy(AgentX3, 405, 150, 60, 75, AgentCurrent);     //Flip 2

        }
        if (Agent_phase % 4 == 2)
        {
            Agent_x += 60;
            Agent_y += 50;
            Copy(AgentX3, 465, 150, 60, 75, AgentCurrent);     //Flip 3
        }
        if (Agent_phase % 4 == 3) {
            Agent_x += 60;
            Copy(AgentX3, 525, 150, 60, 75, AgentCurrent);     //Flip 4
            Agent_state = 1;
        }
        Agent_phase++;


        Sleep(100);
    }
    PasteNon0(AgentCurrent, x, y, Corridor); // Screen
}


void* ScreenControllerThread(LPVOID lpParam)
{
    while (true)
    {
        Copy(Map, x, y, 800, 450, Corridor);
        // Arkaplan çizilecek

        //Mapteki Objeler Çizilecek

        //Karakterler Çizilecek
        PrintAgent(Agent_x, Agent_y, Agent_state, Agent_phase);
        // Projectilelar Çizilecek

        //Sahneyi Goruntule
        DisplayImage(F1, Corridor);
    }
}

// Animasyon fonksiyonu
void* LoadAgentRun(LPVOID lpParam) {
    Agent_state = 1;
    Sleep(1000);
    while (!animation_paused) { // Animasyon duraklatýlmamýþsa devam et
        int c = 1;
        Copy(Map, x, y, 800, 450, Corridor);
        // Copy(AgentX3, 1, 127, 55, 109, AgentCurrent); //Duran Adam

        for (int i = 0; i < 6; i++) {
            Agent_state = 2;

            if (animation_paused)
                return NULL;

            x += 40;
            Agent_phase++;
            Sleep(150);
            if (x == 401) {
                Agent_phase = 0;
                Agent_state = 3;
                Sleep(150);
            }
            if (Agent_state == 1) {
                break;
            }
        }
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
    DisplayImage(F1, Corridor);

    //BMP Pixel TEST
    //Copy(Map, x, y, 800, 450, Corridor);
    //Copy(AgentX3, 345, 150, 60, 75, AgentCurrent);   //Flip 1
    //Copy(AgentX3, 405, 150, 60, 75, AgentCurrent);   //Flip 2
    //Copy(AgentX3, 465, 150, 60, 75, AgentCurrent     //Flip 3
    //Copy(AgentX3, 525, 150, 60, 75, AgentCurrent     //Flip 4
    //PasteNon0(AgentCurrent, Agent_x, Agent_y, Corridor);
    //DisplayImage(F1, Corridor);
    //Sleep(1000);

    F2 = ICG_FrameThin(10, 5, 20, 20);
    DisplayImage(F2, Agent);

    ICG_Button(544, 30, 160, 55, "Start/Stop Animation", StartStopAnimation);

    // Yeni thread baþlat
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