#include "icb_gui.h"

// Thread'ler aras� g�venli flag de�i�keni
bool thread_running = false;
bool animation_paused = false;  // Animasyonu duraklatmak i�in yeni flag
HANDLE hThread = NULL;  // Thread handle

int F1, F2;
ICBYTES Map, Corridor, Agent, AgentX3;
ICBYTES AgentStanding;
int x = 120, y = 10;
int Agent_x = 10;

void ICGUI_Create() {
    ICG_MWSize(1300, 672);
    ICG_MWTitle("Contra");
}

// Animasyon fonksiyonu
DWORD WINAPI LoadAgentRun(LPVOID lpParam) {
    while (thread_running) {
        if (!animation_paused) {  // Animasyon duraklat�lmam��sa devam et
            int c = 0;
            Copy(Map, x, y, 800, 450, Corridor);
            Copy(AgentX3, 1, 127, 55, 109, AgentStanding);
            PasteNon0(AgentStanding, Agent_x, 250, Corridor);
            DisplayImage(F1, Corridor);
            Sleep(1000);

            for (int i = 0; i < 50 && thread_running && !animation_paused; i++) {
                x += 40;
                Copy(Map, x, y, 800, 450, Corridor);

                if (c % 2 == 0) {
                    Copy(AgentX3, 53, 127, 62, 109, AgentStanding);  // Ko�an 2
                }
                else {
                    Copy(AgentX3, 112, 127, 67, 109, AgentStanding);  // Ko�an 3
                }
                c++;
                Agent_x += 3;
                PasteNon0(AgentStanding, Agent_x, 250, Corridor);
                DisplayImage(F1, Corridor);
                Sleep(100);
            }

            Copy(Map, x, y, 800, 450, Corridor);
            Copy(AgentX3, 1, 127, 55, 109, AgentStanding);
            Agent_x += 3;
            PasteNon0(AgentStanding, Agent_x, 250, Corridor);
            DisplayImage(F1, Corridor);
            animation_paused = true;
        }
        else {
            Sleep(100);  // Thread'i me�gul etmemek i�in k�sa bir bekleme
        }
    }
    return 0;
}

// Animasyonu ba�lat ve durdur
void StartStopAnimation() {
    if (!thread_running) {
        // Thread hen�z �al��m�yorsa ba�lat
        thread_running = true;
        animation_paused = false;

        // E�er daha �nce bir thread ba�lat�lm��sa, �nce onu sonland�r
        if (hThread != NULL) {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
        }

        // Yeni thread ba�lat
        hThread = CreateThread(NULL, 0, LoadAgentRun, NULL, 0, NULL);
        if (hThread == NULL) {
            ICG_printf("Thread ba�lat�lamad�!");
        }
    }
    else {
        // Thread �al���yorsa, animasyonu duraklat/devam ettir
        animation_paused = !animation_paused;
    }
}

void ICGUI_main() {
    F1 = ICG_FrameThin(450, 100, 700, 430);
    ReadImage("sprites/mapShort.bmp", Map);
    Copy(Map, x, y, 800, 450, Corridor);
    DisplayImage(F1, Corridor);

    ReadImage("sprites/sprites.bmp", Agent);
    MagnifyX3(Agent, AgentX3);
    DisplayImage(F1, Corridor);

    F2 = ICG_FrameThin(5, 5, 20, 20);
    DisplayImage(F2, Agent);

    ICG_Button(750, 5, 160, 55, "Start/Stop Animation", StartStopAnimation);
}