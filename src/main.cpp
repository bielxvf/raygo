#include <vector>

#include <raylib.h>

#define PROGRAM "raygo"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum {
    MAIN_MENU = 0,
    PLAY_OFFLINE,
} Game_Screen;

class Button {
    public:
        Button(Font font, const char* text, Vector2 pos, Color bg_color, Color fg_color)
        {
            m_font = font;
            Vector2 btn_dims = MeasureTextEx(font, text, (float) font.baseSize, 1.0);
            m_text = text;
            m_rec = {
                .x = pos.x - (btn_dims.x + 15.0) / 2.0,
                .y = pos.y,
                .width = btn_dims.x + 15.0,
                .height = btn_dims.y + 15.0,
            };
            m_fg_color = fg_color;
            m_bg_color = bg_color;
        }

        ~Button()
        {

        }

        Rectangle get_rec()
        {
            return m_rec;
        }

        void draw() {
            DrawRectangleRec(m_rec, m_bg_color);
            DrawTextEx(m_font, m_text, { .x = m_rec.x + 15.0/2.0, .y = m_rec.y + 15.0/2.0 }, (float) m_font.baseSize, 1.0, m_fg_color);
        }

        bool is_pressed(int button)
        {
            if (IsMouseButtonPressed(button)) {
                Vector2 mouse_position = GetMousePosition();
                if (mouse_position.x >= m_rec.x &&
                        mouse_position.x <= m_rec.x + m_rec.width &&
                        mouse_position.y >= m_rec.y &&
                        mouse_position.y <= m_rec.y + m_rec.height) {
                    return true;
                }
            }
            return false;
        }
        
    private:
        Font m_font;
        const char* m_text;
        Rectangle m_rec;
        Color m_fg_color;
        Color m_bg_color;
};

typedef enum {
    STONE_EMPTY = 0,
    STONE_BLACK,
    STONE_WHITE,
} Stone;

class Board {
    public:
        Board(size_t size, Vector2 pos, Texture2D board_texture, Texture2D stone_black_texture, Texture2D stone_white_texture)
        {
            m_board.resize(size, std::vector(size, STONE_EMPTY));
            m_board_texture = board_texture;
            m_stone_black_texture = stone_black_texture;
            m_stone_white_texture = stone_white_texture;
            m_move = 0;
        }

        ~Board()
        {

        }

        void draw()
        {
            float i_want_width = 50.0 * 10.0;
            float board_x = (SCREEN_WIDTH - i_want_width) / 2.0;
            float board_y = (SCREEN_HEIGHT - i_want_width) / 2.0;
            DrawTextureEx(m_board_texture,
                    { board_x, board_y },
                    0.0,
                    i_want_width / (float) m_board_texture.width,
                    WHITE);

            float i_want_stone_width = 50.0;
            for (size_t i = 0; i < 9; i++) {
                for (size_t j = 0; j < 9; j++) {
                    switch (m_board[i][j]) {
                    case STONE_BLACK: {
                        DrawTextureEx(m_stone_black_texture,
                            { .x = 7.5 + board_x + (i % 9) * (i_want_stone_width + 4.25), .y = 7.5 + board_y + (j % 9) * (i_want_stone_width + 4.25) },
                            0.0,
                            i_want_stone_width / m_stone_black_texture.width,
                            WHITE);
                    } break;
                    case STONE_WHITE: {
                        DrawTextureEx(m_stone_white_texture,
                                { .x = 7.5 + board_x + (i % 9) * (i_want_stone_width + 4.25), .y = 7.5 + board_y + (j % 9) * (i_want_stone_width + 4.25) },
                                0.0,
                                i_want_stone_width / m_stone_white_texture.width,
                                WHITE);
                    } break;
                    case STONE_EMPTY: {
                    } break;
                    }
                }
            }
        }

        size_t remove_captured()
        {

        }

    private:
        std::vector<std::vector<Stone>> m_board;
        Texture2D m_board_texture;
        Texture2D m_stone_black_texture;
        Texture2D m_stone_white_texture;
        size_t m_move;
};

int main(int argc, char** argv)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROGRAM);
    SetTargetFPS(60);
    Game_Screen current_screen = MAIN_MENU;
    // TODO: Get the executable working dir then work out this path, also think abt where it should be "installed"
    Font font = LoadFont("../resources/dejavu.fnt");
    if (font.texture.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load font!");
    }

    // TODO: Get the executable working dir then work out this path, also think abt where it should be "installed"
    Texture2D texture_board = LoadTexture("../resources/Blank_Go_board_9x9.png");
    Texture2D texture_stone_black = LoadTexture("../resources/stone_black.png");
    Texture2D texture_stone_white = LoadTexture("../resources/stone_white.png");

    Board board(9, { 100, 100 }, texture_board, texture_stone_black, texture_stone_white);

    Button title(font, "Raygo", { .x = SCREEN_WIDTH / 2.0, .y = 100 }, WHITE, BLACK);
    Button btn_main_menu(font, "Main menu", { .x = 100, .y = 100 }, RED, BLACK);
    Button btn_play_offline(font, "Play offline", { .x = SCREEN_WIDTH/2.0, .y = SCREEN_HEIGHT/2.0 }, RED, BLACK);

    size_t score[2] = {};
    size_t move = 0;

    while (!WindowShouldClose()) {
        // INPUT
        switch (current_screen) {
        case MAIN_MENU: {
            if (btn_play_offline.is_pressed(MOUSE_BUTTON_LEFT)) {
                current_screen = PLAY_OFFLINE;
            }
        } break;
        case PLAY_OFFLINE: {            
            if (btn_main_menu.is_pressed(MOUSE_BUTTON_LEFT)) {
                current_screen = MAIN_MENU;
            } else if (board.is_pressed()) { // @TODO: Check for clicks on the board
                move++;
            }
        } break;
        }

        // SIMULATION
        score[move % 2] = board.remove_captured();

        // RENDERING
	    BeginDrawing();
	    ClearBackground(SKYBLUE);

        switch (current_screen) {
        case MAIN_MENU: {
            title.draw();
            btn_play_offline.draw();
        } break;
        case PLAY_OFFLINE: {
            btn_main_menu.draw();
            board.draw();
        } break;
        }

	    EndDrawing();
    }

    UnloadFont(font);
    UnloadTexture(texture_board);
    UnloadTexture(texture_stone_black);
    UnloadTexture(texture_stone_white);
    CloseWindow();
    return 0;
}
