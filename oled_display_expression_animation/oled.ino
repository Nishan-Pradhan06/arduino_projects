#include <Arduino.h>
#include <U8g2lib.h>

// Driver for the 1.3" I2C OLED (SH1106)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void drawFace(void (*eyeFunc)(), void (*mouthFunc)())
{
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    eyeFunc();
    mouthFunc();
    u8g2.sendBuffer();
}

// --- Mouth Styles ---
void mouthFlat() { u8g2.drawBox(54, 48, 20, 3); }
void mouthSmall() { u8g2.drawBox(58, 50, 12, 2); }
void mouthCurved() { u8g2.drawRFrame(54, 48, 20, 6, 3); }
void mouthO() { u8g2.drawCircle(64, 52, 6); }
void mouthSad() { u8g2.drawFrame(58, 50, 12, 4); } // Simplified sad arc

// --- Eye Expressions from image_56e938.jpg ---

// 1. Normal/Staring
void eyesNormal()
{
    u8g2.drawDisc(40, 30, 14);
    u8g2.drawDisc(88, 30, 14);
    u8g2.setDrawColor(0);
    u8g2.drawDisc(44, 28, 5);
    u8g2.drawDisc(92, 28, 5);
}

// 2. Curiously Looking Right
void eyesLookRight()
{
    u8g2.drawDisc(40, 30, 14);
    u8g2.drawDisc(88, 30, 14);
    u8g2.setDrawColor(0);
    u8g2.drawDisc(48, 28, 5);
    u8g2.drawDisc(96, 28, 5);
}

// 3. Blinking
void eyesBlinking()
{
    u8g2.drawBox(25, 30, 30, 6);
    u8g2.drawBox(73, 30, 30, 6);
}

// 4. Heart Eyes
void eyesHeart()
{
    // Left Heart
    u8g2.drawDisc(35, 28, 7);
    u8g2.drawDisc(45, 28, 7);
    u8g2.drawTriangle(28, 30, 52, 30, 40, 45);
    // Right Heart
    u8g2.drawDisc(83, 28, 7);
    u8g2.drawDisc(93, 28, 7);
    u8g2.drawTriangle(76, 30, 100, 30, 88, 45);
}

// 5. Sad/Droopy
void eyesSad()
{
    u8g2.drawDisc(40, 35, 12);
    u8g2.drawDisc(88, 35, 12);
    u8g2.setDrawColor(0);
    u8g2.drawBox(20, 20, 100, 15); // Cut off tops
}

// 6. Looking Left
void eyesLookLeft()
{
    u8g2.drawDisc(40, 30, 14);
    u8g2.drawDisc(88, 30, 14);
    u8g2.setDrawColor(0);
    u8g2.drawDisc(32, 28, 5);
    u8g2.drawDisc(80, 28, 5);
}

// 7. Angry
void eyesAngry()
{
    u8g2.drawDisc(40, 35, 13);
    u8g2.drawDisc(88, 35, 13);
    u8g2.setDrawColor(0);
    u8g2.drawLine(20, 20, 55, 35);
    u8g2.drawLine(108, 20, 73, 35); // Sharp brow lines
}

// 8. Sleeping/Inactive
void eyesSleeping()
{
    u8g2.drawBox(30, 35, 25, 3);
    u8g2.drawBox(73, 35, 25, 3);
}

// 9. Winking
void eyesWinking()
{
    u8g2.drawDisc(40, 30, 14);
    u8g2.drawRFrame(73, 30, 30, 8, 2);
    u8g2.setDrawColor(0);
    u8g2.drawDisc(44, 28, 5);
}

// 10. Shocked/O-Face
void eyesShocked()
{
    u8g2.drawCircle(40, 30, 14);
    u8g2.drawCircle(88, 30, 14);
    u8g2.drawDisc(40, 30, 5);
    u8g2.drawDisc(88, 30, 5);
}

void setup() { u8g2.begin(); }

void loop()
{
    drawFace(eyesNormal, mouthFlat);
    delay(2000); // 1
    drawFace(eyesLookRight, mouthCurved);
    delay(2000); // 2
    drawFace(eyesBlinking, mouthSmall);
    delay(500); // 3
    drawFace(eyesHeart, mouthCurved);
    delay(2000); // 4
    drawFace(eyesSad, mouthSad);
    delay(2000); // 5
    drawFace(eyesLookLeft, mouthFlat);
    delay(2000); // 6
    drawFace(eyesAngry, mouthSad);
    delay(2000); // 7
    drawFace(eyesSleeping, mouthFlat);
    delay(2000); // 8
    drawFace(eyesWinking, mouthCurved);
    delay(2000); // 9
    drawFace(eyesShocked, mouthO);
    delay(2000); // 10
}