#include "icb_gui.h"

// Thread'ler arasý güvenli flag deðiþkeni
bool thread_running = false;
bool animation_paused = false;  // Animasyonu duraklatmak için yeni flag
HANDLE hThread = NULL;  // Thread handle

int F1, F2;
ICBYTES Map, Corridor, Agent, AgentX3;
ICBYTES AgentStanding;
int x = 1, y = 10;
int Agent_x = 10;

void ICGUI_Create() {
    ICG_MWSize(842, 672);
    ICG_MWTitle("Contra");
}

// Animasyon fonksiyonu
DWORD WINAPI LoadAgentRun(LPVOID lpParam) {
    while (thread_running) { //kapalý
        if (!animation_paused) {  // Animasyon duraklatýlmamýþsa devam et
            int c = 1;
            Copy(Map, x, y, 800, 450, Corridor);
           // Copy(AgentX3, 1, 127, 55, 109, AgentStanding); //Duran Adam
            Copy(AgentX3, 53, 127, 62, 109, AgentStanding);  // Koþan Adam 1
            PasteNon0(AgentStanding, Agent_x, 250, Corridor);
            DisplayImage(F1, Corridor);
            Sleep(100);

            for (int i = 0; i < 54 && thread_running && !animation_paused; i++) {
                if (x > 1600) x = 0, Agent_x = 0;
                x += 40;
                Copy(Map, x, y, 800, 450, Corridor);
                //Deniz
                if (c == 1) {
                    Copy(AgentX3, 53, 127, 62, 109, AgentStanding);  // Koþan Adam 1
                }
                else if(c==2){
                    Copy(AgentX3, 112, 127, 67, 109, AgentStanding);  // Koþan Adam 2
                }
                else if (c == 3) {
                    Copy(AgentX3, 175, 127, 54, 109, AgentStanding);  // Koþan Adam 3
                }
                else if (c == 4) {
                    Copy(AgentX3, 230, 127, 56, 109, AgentStanding);  // Koþan Adam 4
                }
                else {
                    Copy(AgentX3, 283, 127, 68, 109, AgentStanding);  // Koþan Adam 5
                    c = 0;
                }
                c++;
                //Eren
                if (x == 401) {
                    Copy(AgentX3, 345, 150, 60, 75, AgentStanding);   //Flip 1
                    Sleep(200);
                    PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                    DisplayImage(F1, Corridor);

                    Copy(AgentX3, 405, 150, 60, 75, AgentStanding);   //Flip 2
                    PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                    DisplayImage(F1, Corridor);
                    Sleep(200);

                    Copy(AgentX3, 465, 150, 60, 75, AgentStanding);   //Flip 3
                    PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                    DisplayImage(F1, Corridor);
                    Sleep(200);

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
        else {
            Sleep(100);  // Thread'i meþgul etmemek için kýsa bir bekleme
        }
    }
    return 0;
}

// Animasyonu baþlat ve durdur
void StartStopAnimation() {
    if (!thread_running) {
        // Thread henüz çalýþmýyorsa baþlat
        thread_running = true;
        animation_paused = false;

        // Eðer daha önce bir thread baþlatýlmýþsa, önce onu sonlandýr
        if (hThread != NULL) {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
        }

        // Yeni thread baþlat
        hThread = CreateThread(NULL, 0, LoadAgentRun, NULL, 0, NULL);
        if (hThread == NULL) {
            ICG_printf("Thread baþlatýlamadý!");
        }
    }
    else {
        // Thread çalýþýyorsa, animasyonu duraklat/devam ettir
        animation_paused = !animation_paused;
    }
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