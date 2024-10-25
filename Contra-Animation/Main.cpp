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
int BlueAgentState = 0;
int BlueAgentPhase = 0;

int RedAgentX = 830;
int RedAgentY = 200;
int RedAgentState = 0;
int RedAgentPhase = 0;


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
    BlueAgentPhase = 0;
    }
    else {
        BlueAgentPhase++;
    }
}

void PrintBlueAgent(int x, int y, int state, int BlueAgentPhase)
{
    if (state == 0) //hide
        return;

    else if (state == 1) //durma
    {
        Copy(AgentX3, 1, 20, 75, 109, BlueAgentCurrent);
    }
    else if (state == 2) //ko?ma
    {
            if (BlueAgentPhase % 6 == 0) Copy(AgentX3, 1, 127, 55, 109, BlueAgentCurrent);
            if (BlueAgentPhase % 6 == 1) Copy(AgentX3, 53, 127, 62, 109, BlueAgentCurrent);;
            if (BlueAgentPhase % 6 == 2) Copy(AgentX3, 112, 127, 67, 109, BlueAgentCurrent);
            if (BlueAgentPhase % 6 == 3) Copy(AgentX3, 175, 127, 54, 109, BlueAgentCurrent);
            //Contra takımına selam
            if (BlueAgentPhase % 6 == 4) Copy(AgentX3, 230, 127, 56, 109, BlueAgentCurrent);
            if (BlueAgentPhase % 6 == 5) Copy(AgentX3, 283, 127, 68, 109, BlueAgentCurrent);

    }
    else if (state == 3) { //takla
        if (BlueAgentPhase % 4 == 0) { 
            Copy(AgentX3, 345, 150, 60, 75, BlueAgentCurrent);
        }
        else if (BlueAgentPhase % 4 == 1) { 
            Copy(AgentX3, 405, 150, 60, 75, BlueAgentCurrent);
        }
        else if (BlueAgentPhase % 4 == 2) { 
            Copy(AgentX3, 465, 150, 60, 75, BlueAgentCurrent);
        }
        else if (BlueAgentPhase % 4 == 3) { 
            Copy(AgentX3, 525, 150, 60, 75, BlueAgentCurrent);
        }
    }
    PasteNon0(BlueAgentCurrent, BlueAgentX, BlueAgentY, Corridor); // Screen
}


void PrintRedAgent(int x, int y, int state, int RedAgentPhase) {

    if (state == 0) //hide
        return;

    else if (state == 1) //durma
    {
        Copy(AgentX3, 1160, 20, 75, 109, RedAgentCurrent); // Kırmızı Duran
    }
    else if (state == 2) //ko?ma
    {
        //Kırmızı kosma yok
    }
    else if (state == 3) { //takla
        if (RedAgentPhase % 1 == 0) {
            Copy(AgentX3, 712, 150, 60, 75, RedAgentCurrent);     //Red Flip 3
        }
        else if (RedAgentPhase % 2 == 1) {
            Copy(AgentX3, 652, 150, 60, 75, RedAgentCurrent);     //Red Flip 4
        }
        else if (RedAgentPhase % 3 == 2) {
            Copy(AgentX3, 772, 150, 60, 75, RedAgentCurrent);     //Red Flip 2
        }
        else if (RedAgentPhase % 4 == 3) {
            Copy(AgentX3, 1160, 20, 75, 109, RedAgentCurrent);     //Red Flip 1
        }
    }

    //Copy(AgentX3, 237, 77, 100, 50, BlueAgentCurrent); //Mavi Eğilen 1
    //Copy(AgentX3, 337, 80, 100, 50, BlueAgentCurrent); // Mavi Eğilen 2 (Ateş etme)

    //Copy(AgentX3, 1160, 20, 75, 109, BlueAgentCurrent); // Kırmızı Duran
    //Copy(AgentX3, 1088, 20, 75, 109, BlueAgentCurrent); // Kırmız Duran (Ateş etme)

    //Paste(BlueAgentCurrent, BlueAgentX, BlueAgentY, Corridor);
    PasteNon0(RedAgentCurrent, RedAgentX, RedAgentY, Corridor); // Screen
}
void* ScreenControllerThread(LPVOID lpParam)
{
    while (true)
    {
        Copy(Map, x, y, 800, 450, Corridor);
        // Arkaplan çizilecek

        //Mapteki Objeler Çizilecek

        //Karakterler Çizilecek
        PrintBlueAgent(BlueAgentX, BlueAgentY, BlueAgentState, BlueAgentPhase);
        PrintRedAgent(RedAgentX, RedAgentY, RedAgentState, RedAgentPhase);
        // Projectilelar Çizilecek

        //Sahneyi Goruntule
        DisplayImage(F1, Corridor);
    }
}

// Animasyon fonksiyonu
void* LoadAgentRun(LPVOID lpParam) {
    SetState(BlueAgentState, 1);
    Sleep(1000);
    while (!animation_paused) { // Animasyon duraklatýlmamýþsa devam et
        x = 1; y = 10;
        BlueAgentX = 10;BlueAgentY = 250;
        RedAgentX = 830;RedAgentY = 200;
        int c = 1;
        Copy(Map, x, y, 800, 450, Corridor);

        SetState(BlueAgentState, 2);
        for (int i = 0; i < 12; i++) { //walk
            if (animation_paused)
                return NULL;
            x += 40;
            Sleep(150);
            BlueAgentPhase++;
        }
        SetState(BlueAgentState, 3);
        SetState(RedAgentState, 3);
        for (int i = 0; i < 4; i++) {
            switch (BlueAgentPhase) {
            case 0:
                BlueAgentX += 60;
                BlueAgentY -= 50;

                RedAgentX -= 70;
                break;
            case 1:
                BlueAgentX += 60;
                BlueAgentY -= 50;

                RedAgentX -= 70;
                break;
            case 2:
                BlueAgentX += 60;
                BlueAgentY += 50;

                RedAgentX -= 70;
                break;
            case 3:
                BlueAgentX += 60;
                BlueAgentY -= 50;

                RedAgentY -= 50;
                SetState(BlueAgentState, 1);
                SetState(RedAgentState, 1);
                break;
            }
            BlueAgentPhase++;
            RedAgentPhase++;

            Sleep(250);
        }
        //Extra walk
        //SetState(BlueAgentState, 2);
        //for (int i = 0; i < 8; i++) { // walk
        //    if (animation_paused)
        //        return NULL;
        //    x += 40;
        //    BlueAgentPhase++;
        //    Sleep(150);
        //}
        SetState(BlueAgentState, 1);
        SetState(RedAgentState, 1);
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