#ifndef macros_h
#define macros_h

#define DrawTextUTF8(text, posX, posY, fontSize, color) DrawTextEx(font, text, (Vector2) { posX, posY }, fontSize, 1.0f, color)
#define MeasureTextUTF8(text, fontSize) MeasureTextEx(font, text, fontSize, 1.0f)

#endif

