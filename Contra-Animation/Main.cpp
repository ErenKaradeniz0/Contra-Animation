#include "icb_gui.h"
bool animation_paused = true;  // Animasyonu duraklatmak için yeni flag
HANDLE hThread = NULL;  // Thread handle
HANDLE th = NULL;

int F1, F2;
ICBYTES Map, Corridor, AgentBMP, AgentBMPX3;
ICBYTES BlueAgentCurrent;
ICBYTES RedAgentCurrent;
int x = 1, y = 10;

enum AgentState {
    HIDE = 0,
    STAND,
    RUN,
    FLIP,
    SHOOT,
    DEATH,
    CROUCH
};

struct Agent {
    int x;
    int y;
    int phase;
    AgentState State;
};

// Agent türünden iki değişken oluşturuyoruz
Agent BlueAgent = { 10, 250, 0, HIDE };
Agent RedAgent = { 830, 200, 0, HIDE };
void _WaitThread(HANDLE thread);
void _CreateThread(HANDLE thread, void* threadMain);

void ICGUI_Create() {
    ICG_MWSize(842, 672);
    ICG_MWTitle("Contra");
}



void SetState(Agent& agent, AgentState newState) {
    if (agent.State != newState) {
        agent.State = newState;
    }
    else {
        agent.phase++;
    }
    }

void PrintBlueAgent(int x, int y, AgentState state, int phase)
{

    switch (state) {
    case HIDE: // saklanma
        return;
    case STAND: // durma
        Copy(AgentBMPX3, 1, 20, 75, 109, BlueAgentCurrent);
        break;

    case RUN: // koşma
        switch (phase % 6) {
        case 0: Copy(AgentBMPX3, 1, 127, 55, 109, BlueAgentCurrent); break;
        case 1: Copy(AgentBMPX3, 53, 127, 62, 109, BlueAgentCurrent); break;
        case 2: Copy(AgentBMPX3, 112, 127, 67, 109, BlueAgentCurrent); break;
        case 3: Copy(AgentBMPX3, 175, 127, 54, 109, BlueAgentCurrent); break;
        case 4: Copy(AgentBMPX3, 230, 127, 56, 109, BlueAgentCurrent); break;
        case 5: Copy(AgentBMPX3, 283, 127, 68, 109, BlueAgentCurrent); break;
        }
        break;

    case FLIP: // takla
        switch (phase % 4) {
        case 0: Copy(AgentBMPX3, 345, 150, 60, 75, BlueAgentCurrent); break;
        case 1: Copy(AgentBMPX3, 405, 150, 60, 75, BlueAgentCurrent); break;
        case 2: Copy(AgentBMPX3, 465, 150, 60, 75, BlueAgentCurrent); break;
        case 3: Copy(AgentBMPX3, 525, 150, 60, 75, BlueAgentCurrent); break;
        }
        break;

    case SHOOT: // ateş etme
        break;

    case DEATH: // ölüm
        break;

    case CROUCH: // eğilme
        break;

    default:
        break;
    }

    PasteNon0(BlueAgentCurrent, BlueAgent.x, BlueAgent.y, Corridor); // Screen
}


void PrintRedAgent(int x, int y, int state, int phase) {

    switch (state) {
    case HIDE:
        return;

    case STAND: // durma
        Copy(AgentBMPX3, 1160, 20, 75, 109, RedAgentCurrent); // Kırmızı Duran
        break;

    case RUN: // koşma
        // Kırmızı koşma yok
        break;

    case FLIP: // takla
        switch (phase % 4) {
        case 0: Copy(AgentBMPX3, 712, 150, 60, 75, RedAgentCurrent); break; // Red Flip 3
        case 1: Copy(AgentBMPX3, 652, 150, 60, 75, RedAgentCurrent); break; // Red Flip 4
        case 2: Copy(AgentBMPX3, 772, 150, 60, 75, RedAgentCurrent); break; // Red Flip 2
        case 3: Copy(AgentBMPX3, 1160, 20, 75, 109, RedAgentCurrent); break; // Red Flip 1
        }
        break;

    case SHOOT: // ateş etme
        break;

    case DEATH: // ölüm
        break;

    case CROUCH: // eğilme
        break;

    default:
        break;
    }


    //Copy(AgentBMPX3, 237, 77, 100, 50, BlueAgentCurrent); //Mavi Eğilen 1
    //Copy(AgentBMPX3, 337, 80, 100, 50, BlueAgentCurrent); // Mavi Eğilen 2 (Ateş etme)

    //Copy(AgentBMPX3, 1160, 20, 75, 109, BlueAgentCurrent); // Kırmızı Duran
    //Copy(AgentBMPX3, 1088, 20, 75, 109, BlueAgentCurrent); // Kırmız Duran (Ateş etme)

    //Paste(BlueAgentCurrent, BlueAgent.x, BlueAgent.y, Corridor);
    PasteNon0(RedAgentCurrent, RedAgent.x, RedAgent.y, Corridor); // Screen
}
void* ScreenControllerThread(LPVOID lpParam)
{
    while (true)
    {
        Copy(Map, x, y, 800, 450, Corridor);
        // Arkaplan çizilecek

        //Mapteki Objeler Çizilecek

        //Karakterler Çizilecek
        PrintBlueAgent(BlueAgent.x, BlueAgent.y, BlueAgent.State, BlueAgent.phase);
        PrintRedAgent(RedAgent.x, RedAgent.y, RedAgent.State, RedAgent.phase);
        // Projectilelar Çizilecek

        //Sahneyi Goruntule
        DisplayImage(F1, Corridor);
    }
}

// Animasyon fonksiyonu
void* LoadAgentRun(LPVOID lpParam) {
    SetState(BlueAgent, STAND);
    Sleep(1000);
    while (!animation_paused) { // Animasyon duraklatýlmamýþsa devam et
        x = 1; y = 10;
        BlueAgent.x = 10;BlueAgent.y = 250;
        RedAgent.x = 830;RedAgent.y = 200;
        int c = 1;
        Copy(Map, x, y, 800, 450, Corridor);

        SetState(BlueAgent, RUN);
        for (int i = 0; i < 12; i++) { //walk
            if (animation_paused)
                return NULL;
            x += 40;
            Sleep(150);
            BlueAgent.phase++;
        }
        SetState(BlueAgent,FLIP);
        SetState(RedAgent, FLIP);
        for (int i = 0; i < 4; i++) {
            switch (BlueAgent.phase % 4) {
            case 0:
                BlueAgent.x += 60;
                BlueAgent.y -= 50;

                RedAgent.x -= 70;
                break;
            case 1:
                BlueAgent.x += 60;
                BlueAgent.y -= 50;

                RedAgent.x -= 70;
                break;
            case 2:
                BlueAgent.x += 60;
                BlueAgent.y += 50;

                RedAgent.y -= 50;
                RedAgent.x -= 70;
                break;
            case 3:
                BlueAgent.x += 60;
                BlueAgent.y -= 50;
                SetState(BlueAgent, STAND);
                SetState(RedAgent, STAND);
                break;
            }
            BlueAgent.phase++;
            RedAgent.phase++;

            Sleep(250);
        }
        //Extra walk
        //SetState(BlueAgent.State, 2);
        //for (int i = 0; i < 8; i++) { // walk
        //    if (animation_paused)
        //        return NULL;
        //    x += 40;
        //    BlueAgent.phase++;
        //    Sleep(150);
        //}
        SetState(BlueAgent, STAND);
        SetState(RedAgent, STAND);
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

    ReadImage("sprites/sprites.bmp", AgentBMP);
    MagnifyX3(AgentBMP, AgentBMPX3);
   // DisplayImage(F1, Corridor);

    //BMP Pixel TEST
    Copy(Map, x, y, 800, 450, Corridor);
    //Copy(AgentBMPX3, 345, 150, 60, 75, BlueAgentCurrent);   //Flip 1
    //Copy(AgentBMPX3, 405, 150, 60, 75, BlueAgentCurrent);   //Flip 2
    //Copy(AgentBMPX3, 465, 150, 60, 75, BlueAgentCurrent     //Flip 3
    //Copy(AgentBMPX3, 525, 150, 60, 75, BlueAgentCurrent     //Flip 4
    //Copy(AgentBMPX3, 525, 150, 60, 75, BlueAgentCurrent);     //Flip 4
    //PasteNon0(BlueAgentCurrent, BlueAgent.x, BlueAgent.y, Corridor);
    //DisplayImage(F1, Corridor);
    //Sleep(1000);

    F2 = ICG_FrameThin(10, 5, 20, 20);
    DisplayImage(F2, AgentBMP);

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