#include "icb_gui.h"


bool animation_paused = true;  // Animasyonu duraklatmak için yeni flag
HANDLE hThread = NULL;  // Thread handle

int F1, F2;
ICBYTES Map, Corridor, Agent, AgentX3;
ICBYTES AgentStanding;
int x = 1, y = 10;
int Agent_x = 10;

void _WaitThread(HANDLE thread);
void _CreateThread(HANDLE thread, void* threadMain);

void ICGUI_Create() {
    ICG_MWSize(842, 672);
    ICG_MWTitle("Contra");
}

// Animasyon fonksiyonu
void* LoadAgentRun(LPVOID lpParam) {
    while (!animation_paused) { // Animasyon duraklatýlmamýþsa devam et
        int c = 1;
        Copy(Map, x, y, 800, 450, Corridor);
        // Copy(AgentX3, 1, 127, 55, 109, AgentStanding); //Duran Adam
        Copy(AgentX3, 53, 127, 62, 109, AgentStanding);  // Koþan Adam 1
        PasteNon0(AgentStanding, Agent_x, 250, Corridor);
        DisplayImage(F1, Corridor);
        Sleep(100);

        for (int i = 0; i < 54; i++) {
            if (animation_paused)
                return NULL;

            if (x > 1600) x = 0, Agent_x = 0;
            x += 40;
            Copy(Map, x, y, 800, 450, Corridor);
            //Deniz
            if (c == 1) {
                Copy(AgentX3, 53, 127, 62, 109, AgentStanding);  // Run 1
            }
            else if (c == 2) {
                Copy(AgentX3, 112, 127, 67, 109, AgentStanding);  // Run 2
            }
            else if (c == 3) {
                Copy(AgentX3, 175, 127, 54, 109, AgentStanding);  // Run 3
            }
            else if (c == 4) {
                Copy(AgentX3, 230, 127, 56, 109, AgentStanding);  // Run 4
            }
            else {
                Copy(AgentX3, 283, 127, 68, 109, AgentStanding);  // Run 5
                c = 0;
            }
            c++;
            //Eren
            if (x == 401) {
                Copy(Map, x, y, 800, 450, Corridor);
                Copy(AgentX3, 345, 150, 60, 75, AgentStanding);   //Flip 1
                Sleep(200);
                PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                DisplayImage(F1, Corridor);

                Copy(Map, x, y, 800, 450, Corridor);
                Copy(AgentX3, 405, 150, 60, 75, AgentStanding);   //Flip 2
                PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                DisplayImage(F1, Corridor);
                Sleep(200);


                Copy(Map, x, y, 800, 450, Corridor);
                Copy(AgentX3, 465, 150, 60, 75, AgentStanding);   //Flip 3
                PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                DisplayImage(F1, Corridor);
                Sleep(200);

                Copy(Map, x, y, 800, 450, Corridor);
                Copy(AgentX3, 525, 150, 60, 75, AgentStanding);   //Flip 4
                DisplayImage(F1, Corridor);
                PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                DisplayImage(F1, Corridor);
                Sleep(200);
            }
            c++;
            Agent_x += 3;
            PasteNon0(AgentStanding, Agent_x, 250, Corridor);
            DisplayImage(F1, Corridor);
            Sleep(100);
        }

        Copy(Map, x, y, 800, 450, Corridor);
        Copy(AgentX3, 1, 127, 55, 109, AgentStanding); //Duran Adam
        Agent_x += 3;
        PasteNon0(AgentStanding, Agent_x, 250, Corridor);
        DisplayImage(F1, Corridor);
        animation_paused = true;
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
    //Copy(AgentX3, 345, 150, 60, 75, AgentStanding);   //Flip 1
    //Copy(AgentX3, 405, 150, 60, 75, AgentStanding);   //Flip 2
    //Copy(AgentX3, 465, 150, 60, 75, AgentStanding     //Flip 3
    //Copy(AgentX3, 525, 150, 60, 75, AgentStanding     //Flip 4
    //PasteNon0(AgentStanding, Agent_x, 250, Corridor);
    //DisplayImage(F1, Corridor);
    //Sleep(1000);

    F2 = ICG_FrameThin(10, 5, 20, 20);
    DisplayImage(F2, Agent);

    ICG_Button(544, 30, 160, 55, "Start/Stop Animation", StartStopAnimation);
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