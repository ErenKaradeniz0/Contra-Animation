#include "icb_gui.h"
bool animation_paused = true;  // Animasyonu duraklatmak için yeni flag
HANDLE TAnimation = NULL;  // Thread handle
HANDLE TScreen = NULL;
HANDLE TMusic = NULL;

int F1, F2;
ICBYTES Map, Corridor, AgentBMP, AgentBMPX3;
ICBYTES BlueAgentCurrent;
ICBYTES BlueBulletCurrent;

ICBYTES RedAgentCurrent;
ICBYTES RedAgentBullet;

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

struct Bullet {
    int x;
    int y;
    int speed;
};

Bullet BlueBullet = {300,210,38};

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
    if (agent.State != newState || newState != SHOOT) {
        agent.State = newState;
        agent.phase = 0;
    }
    else {
        agent.phase++;
    }
    }

ICBYTES BlueCoordinates{
    {1, 20, 75, 109},    // Blue Stand
    {1, 127, 55, 109},   // Blue Run (phase 0)
    {53, 127, 62, 109},  // Blue Run (phase 1)
    {112, 127, 67, 109}, // Blue Run (phase 2)
    {175, 127, 54, 109}, // Blue Run (phase 3)
    {230, 127, 56, 109}, // Blue Run (phase 4)
    {283, 127, 68, 109}, // Blue Run (phase 5)
    {345, 150, 60, 75},  // Blue Flip (phase 0)
    {405, 150, 60, 75},  // Blue Flip (phase 1)
    {465, 150, 60, 75},  // Blue Flip (phase 2)
    {525, 150, 60, 75},  // Blue Flip (phase 3)
    {237, 77, 100, 50}   // Blue Crouch
};

void PrintBlueAgent(int x, int y, AgentState state, int phase) {
    if (state == HIDE) {
        return;
    }

    int index = -1;

    switch (state) {
    case STAND: // Durma
        index = 1; // Blue Stand
        break;
    case RUN: // Koşma
        index = 2 + (phase % 6); // 2, 3, 4, 5, 6, 7 based on phase
        break;
    case FLIP: // Takla
        index = 8 + (phase % 4); // 8, 9, 10, 11 based on phase
        break;

    case SHOOT: // Ateş etme
        break;
    case DEATH: // Ölüm
        return;
    case CROUCH: // Eğilme
        index = 12; // Blue Crouch
        break;
    default:
        return;
    }

    if (index >= 0) {
        Copy(AgentBMPX3, BlueCoordinates.I(1, index), BlueCoordinates.I(2, index),
            BlueCoordinates.I(3, index), BlueCoordinates.I(4, index),
            BlueAgentCurrent);
    }
    PasteNon0(BlueAgentCurrent, BlueAgent.x, BlueAgent.y, Corridor); // Screen
}

ICBYTES RedCoordinates{
    {1160, 20, 75, 109}, // Red Stand 
    {712, 150, 60, 75},   // Red Flip 3 (phase 0)
    {652, 150, 60, 75},   // Red Flip 4 (phase 1)
    {772, 150, 60, 75},   // Red Flip 2 (phase 2)
    {1160, 20, 75, 109},   // Red Flip 1 (phase 3)
    {1088, 20, 75, 109}  // Red Stand (Fire) (phase 0)
};

void PrintRedAgent(int x, int y, int state, int phase) {
    if (state == HIDE) {
        return;
    }

    int index = -1;

    switch (state) {
    case STAND: // Durma
        index = 1; // Red Stand
        break;

    case RUN: // Koşma
        // Kırmızı koşma yok
        return;

    case FLIP: // Takla
        index = 2 + (phase % 4); // 2, 3,4,5 based on phase
        break;

    case SHOOT: // Ateş etme
        //index = (phase % 2 == 0) ? 5 : 6; // 5 for Red Stand (Fire), 6 for Red Stand
        break;

    case DEATH: // ölüm
        switch (phase) {
        case 0: Copy(AgentBMPX3, 880, 266, 48, 72, RedAgentCurrent); break; // Red Death 1 +
        case 1: Copy(AgentBMPX3, 932, 282, 74, 48, RedAgentCurrent); break; // Red Death 2
        case 2: Copy(AgentBMPX3, 634, 309, 102, 35, RedAgentCurrent); break; // Red Death 3 +
        }
        break;

    case CROUCH: // Eğilme
        return;

    default:
        return;
    }

    if (index >= 0) {
        Copy(AgentBMPX3, RedCoordinates.I(1, index), RedCoordinates.I(2, index),
            RedCoordinates.I(3, index), RedCoordinates.I(4, index),
            RedAgentCurrent);
    }
    PasteNon0(RedAgentCurrent, RedAgent.x, RedAgent.y, Corridor); // Screen
}

void PrintBullet(Bullet& bullet) {
    if (bullet.x <= 608) {
    Copy(AgentBMPX3, 266, 26, 19, 19, BlueBulletCurrent);
    bullet.x += bullet.speed;
    PasteNon0(BlueBulletCurrent, bullet.x, bullet.y, Corridor);
    Sleep(150);
    }

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
        if(BlueAgent.State == SHOOT)
            PrintBullet(BlueBullet);

        PrintRedAgent(RedAgent.x, RedAgent.y, RedAgent.State, RedAgent.phase);
        // Projectilelar Çizilecek

        //Sahneyi Goruntule
        DisplayImage(F1, Corridor);
    }
}

void* MusicControllerThread(LPVOID lpParam) {
    PlaySound("sound/Intro.wav", NULL, SND_SYNC);
    while(true)
        if(!animation_paused)
            PlaySound("sound/JungleTheme.wav", NULL, SND_SYNC);
}

// Animasyon fonksiyonu
void* LoadAnimation(LPVOID lpParam) {
    SetState(BlueAgent, STAND);
    Sleep(1000);
    while (!animation_paused) { // Animasyon duraklatýlmamýþsa devam et
        //reset scene
        x = 1; y = 10;
        BlueAgent.x = 10;BlueAgent.y = 250;
        RedAgent.x = 830;RedAgent.y = 200;
        BlueBullet.x = 300;

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
                BlueAgent.x += 50;
                BlueAgent.y -= 50;

                RedAgent.x -= 90;
                break;
            case 1:
                BlueAgent.x += 50;
                BlueAgent.y -= 50;

                RedAgent.x -= 90;
                break;
            case 2:
                BlueAgent.x += 50;
                BlueAgent.y += 50;

                RedAgent.x -= 90;
                RedAgent.y -= 50;
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
            int a = 5;
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
        
        SetState(BlueAgent, CROUCH);
        BlueAgent.y += 50;
        Sleep(200);

        SetState(BlueAgent, SHOOT);
        //SetState(RedAgent, SHOOT);
        for (int i = 0; i < 8; i++) {
            // Move up if i is even, down if i is odd
            BlueAgent.y += (i % 2 == 0) ? -3 : 3;
            Sleep(150);
        }

        RedAgent.phase++;
        BlueAgent.phase++;
        Sleep(500);


    }
    return 0;
}

// Animasyonu baþlat ve durdur
void StartStopAnimation() {

    //
    animation_paused = !animation_paused;

    // Eðer daha önce bir thread baþlatýlmýþsa, önce onu sonlandýr
    _WaitThread(TAnimation);

    // Yeni thread baþlat
    _CreateThread(TAnimation, LoadAnimation);
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
    _CreateThread(TScreen, ScreenControllerThread);
    _CreateThread(TMusic, MusicControllerThread);

}

void _WaitThread(HANDLE thread)
{
    if (thread != NULL) {
        WaitForSingleObject(TAnimation, INFINITE);
        CloseHandle(TAnimation);
    }
    thread = NULL;
}

void _CreateThread(HANDLE thread, void* threadMain)
{
    thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadMain, NULL, 0, NULL);
}