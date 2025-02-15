#include <raylib.h>

#define PROGRAM "raygo"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum {
    MAIN_MENU = 0,
    PLAY_OFFLINE,
} Game_Screen;

typedef struct {
    const char* text;
    Rectangle rec;
    Color fg_color;
    Color bg_color;
} Button;

Button Button_init(Font font, const char* text, Vector2 pos, Color bg_color, Color fg_color)
{
    Vector2 btn_dims = MeasureTextEx(font, text, (float) font.baseSize, 1.0);
    Button btn =
    {
        .text = text,
        .rec = {
            .width = btn_dims.x + 15.0,
            .x = pos.x - (btn_dims.x + 15.0) / 2.0,
            .height = btn_dims.y + 15.0,
            .y = pos.y,
        },
        .bg_color = bg_color,
        .fg_color = fg_color,
    };

    return btn;
}

void Button_draw(Font font, Button btn)
{
    DrawRectangleRec(btn.rec, btn.bg_color);
    DrawTextEx(font, btn.text, (Vector2){ .x = btn.rec.x + 15.0/2.0, .y = btn.rec.y + 15.0/2.0 }, (float) font.baseSize, 1.0, btn.fg_color);
}

bool is_mouse_on_rec(Rectangle rec)
{
    Vector2 mouse_position = GetMousePosition();
    if (mouse_position.x >= rec.x &&
    mouse_position.x <= rec.x + rec.width &&
    mouse_position.y >= rec.y &&
    mouse_position.y <= rec.y + rec.height) {
        return true;
    }
    return false;
}

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROGRAM);
    SetTargetFPS(60);
    Game_Screen current_screen = MAIN_MENU;
    Font font = LoadFont("../resources/dejavu.fnt");
    if (font.texture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load font!");
    }

    Button title = Button_init(font, "Raygo", (Vector2){ .x = SCREEN_WIDTH / 2, .y = 100 }, WHITE, BLACK);
    Button btn_main_menu = Button_init(font, "Main menu", (Vector2){ .x = 100, .y = 100 }, RED, BLACK);
    Button btn_play_offline = Button_init(font, "Play offline", (Vector2){ .x = SCREEN_WIDTH/2.0, .y = SCREEN_HEIGHT/2.0 }, RED, BLACK);

    while (!WindowShouldClose()) {
        // float dt = GetFrameTime();

        switch (current_screen) {
        case MAIN_MENU: {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (is_mouse_on_rec(btn_play_offline.rec)) {
                    current_screen = PLAY_OFFLINE;
                }
            }
        } break;
        case PLAY_OFFLINE: {            
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (is_mouse_on_rec(btn_main_menu.rec)) {
                    current_screen = MAIN_MENU;
                }
            }
        } break;
        }


	    BeginDrawing();
	    ClearBackground(WHITE);
        switch (current_screen) {
        case MAIN_MENU: {
            Button_draw(font, title);
            Button_draw(font, btn_play_offline);
        } break;
        case PLAY_OFFLINE: {
            Button_draw(font, btn_main_menu);
        } break;
        }
	    EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
