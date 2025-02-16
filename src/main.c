#include <stddef.h>
#include <stdio.h>

#include <raylib.h>

#define PROGRAM "raygo"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum {
    MAIN_MENU = 0,
    PLAY_OFFLINE,
} Game_Screen;

typedef enum {
    STONE_EMPTY = 0,
    STONE_BLACK,
    STONE_WHITE,
} Stone;

typedef struct {
    const char* text;
    Rectangle rec;
    Color fg_color;
    Color bg_color;
} Button;

Color WOOD = (Color){ .r = 0x92, .g = 0x66, .b = 0x3E, .a = 0xFF};

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

    Texture2D texture_board = LoadTexture("../resources/Blank_Go_board_9x9.png");
    Texture2D texture_stone_black = LoadTexture("../resources/stone_black.png");
    Texture2D texture_stone_white = LoadTexture("../resources/stone_white.png");

    Stone board[9][9] = {0};
    for (size_t i = 0; i < 9; i++) {
        for (size_t j = 0; j < 9; j++) {
                board[i][j] = STONE_EMPTY;
        }
    }

    Button title = Button_init(font, "Raygo", (Vector2){ .x = SCREEN_WIDTH / 2.0, .y = 100 }, WHITE, BLACK);
    Button btn_main_menu = Button_init(font, "Main menu", (Vector2){ .x = 100, .y = 100 }, RED, BLACK);
    Button btn_play_offline = Button_init(font, "Play offline", (Vector2){ .x = SCREEN_WIDTH/2.0, .y = SCREEN_HEIGHT/2.0 }, RED, BLACK);

    int key = 0;
    int second_key = 0;
    int move = 0;
    while (!WindowShouldClose()) {
        // float dt = GetFrameTime();

        // INPUT
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
                } else if (false) { // @TODO: Check for clicks on the board
                }
            }

            if (key == 0) {
                key = GetKeyPressed();
            }
            if (key >= '1' && key <= '9') {
                if (second_key == 0) {
                    second_key = GetKeyPressed();
                    if (second_key >= '1' && second_key <= '9') {
                        board[key - '1'][(9 - (second_key - '0')) % 9] = move % 2 == 0 ? STONE_BLACK : STONE_WHITE;
                        key = 0;
                        second_key = 0;
                        move++;
                    }
                }

            }
        } break;
        }


        // SIMULATION
        // @TODO: Check dead stones and remove
        for (size_t i = 0; i < 9; i++) {
            for (size_t j = 0; j < 9; j++) {
                if (board[i + 1][j] != STONE_EMPTY && board[i+1][j] == STONE_BLACK) {
                    board[i][j] = STONE_EMPTY;
                }
            }
        }

        // RENDERING
	    BeginDrawing();
	    ClearBackground(SKYBLUE);
        switch (current_screen) {
        case MAIN_MENU: {
            Button_draw(font, title);
            Button_draw(font, btn_play_offline);
        } break;
        case PLAY_OFFLINE: {
            Button_draw(font, btn_main_menu);

            float i_want_width = 50.0 * 10.0;
            float board_x = (SCREEN_WIDTH - i_want_width) / 2.0;
            float board_y = (SCREEN_HEIGHT - i_want_width) / 2.0;
            DrawTextureEx(texture_board,
                    (Vector2){ .x = board_x, .y = board_y },
                    0.0,
                    i_want_width / (float) texture_board.width,
                    WHITE);

            float i_want_stone_width = 50.0;
            for (size_t i = 0; i < 9; i++) {
                for (size_t j = 0; j < 9; j++) {
                    switch (board[i][j]) {
                    case STONE_BLACK: {
                        DrawTextureEx(texture_stone_black,
                                (Vector2){ .x = 7.5 + board_x + (i % 9) * (i_want_stone_width + 4.25), .y = 7.5 + board_y + (j % 9) * (i_want_stone_width + 4.25) },
                                0.0,
                                i_want_stone_width / texture_stone_black.width,
                                WHITE);
                    } break;
                    case STONE_WHITE: {
                        DrawTextureEx(texture_stone_white,
                                (Vector2){ .x = 7.5 + board_x + (i % 9) * (i_want_stone_width + 4.25), .y = 7.5 + board_y + (j % 9) * (i_want_stone_width + 4.25) },
                                0.0,
                                i_want_stone_width / texture_stone_white.width,
                                WHITE);
                    } break;
                    }
                }
            }

        } break;
        }
	    EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
