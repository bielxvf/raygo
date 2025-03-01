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
    const texture_stone_black = try rl.loadTexture("resources/stone_black.png");
    // const texture_stone_white = try rl.loadTexture("resources/stone_white.png");

    var board: [9][9]u32 = undefined;

    for (&board) |*row| {
        for (row) |*it| {
            it.* = 0;
        }
    }

    while (!rl.windowShouldClose()) {
        // Input

        // Simulation

        // Rendering
        rl.beginDrawing();
        defer rl.endDrawing();
        rl.clearBackground(rl.Color.white);

        const board_x = @as(f32, @floatFromInt(screen_width - texture_board.width)) / 2.0;
        const board_y = @as(f32, @floatFromInt(screen_height - texture_board.height)) / 2.0;
        const board_padding = 31.0;
        const board_cell_width = 48.0;
        rl.drawTextureEx(texture_board, rl.Vector2.init(board_x, board_y), 0.0, 1.0, rl.Color.white);

        const stone_scale = board_cell_width / @as(f32, @floatFromInt(texture_stone_black.width));
        const stone_x = board_x + board_padding - (@as(f32, @floatFromInt(texture_stone_black.width)) / 2.0) * stone_scale;
        const stone_y = board_y + board_padding - (@as(f32, @floatFromInt(texture_stone_black.width)) / 2.0) * stone_scale;
        rl.drawTextureEx(texture_stone_black, rl.Vector2.init(stone_x, stone_y), 0.0, stone_scale, rl.Color.white);
    }
}
