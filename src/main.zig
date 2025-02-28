const std = @import("std");

const rl = @import("raylib");

pub fn main() !void {

    const screen_width = 1080;
    const screen_height = 720;

    rl.initWindow(screen_width, screen_height, "Raygo");
    defer rl.closeWindow();
    rl.setTargetFPS(60);

    // TODO: Fix.
    const texture_board = try rl.loadTexture("resources/Blank_Go_board_9x9.png");
    // const texture_stone_black = rl.loadTexture("resources/stone_black.png");
    // const texture_stone_white = rl.loadTexture("resources/stone_white.png");

    while (!rl.windowShouldClose()) {
        // Input

        // Simulation

        // Rendering
        rl.beginDrawing();
        defer rl.endDrawing();
        rl.clearBackground(rl.Color.white);

        // const i_want_width = 50.0 * 10.0;
        // const board_x = (screen_width - i_want_width) / 2.0;
        // const board_y = (screen_height - i_want_width) / 2.0;
        // rl.drawTextureEx(texture_board, (rl.Vector2){ .x = board_x, .y = board_y },
        //     0.0,
        //     i_want_width / @as(f32, @floatFromInt(texture_board.width)),
        //     rl.Color.white);
        rl.drawTextureEx(texture_board, (rl.Vector2){ .x = 0.0, .y = 0.0}, 0.0, 1.0, rl.Color.white);
    }
}
