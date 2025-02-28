const std = @import("std");

const rl = @import("raylib");

pub fn main() !void {

    const screen_width = 800;
    const screen_height = 450;

    rl.initWindow(screen_width, screen_height, "Raygo");
    defer rl.closeWindow();
    rl.setTargetFPS(60);

    while (!rl.windowShouldClose()) {
        // Input

        // Simulation

        // Rendering
        rl.beginDrawing();
        defer rl.endDrawing();

        rl.clearBackground(rl.Color.white);

        rl.drawText("Hello from Zig!", 190, 200, 20, rl.Color.light_gray);
    }
}
