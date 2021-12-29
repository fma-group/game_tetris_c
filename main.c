#include <raylib.h>
#include <stdio.h>

struct {
    int width, height;
} screen = {
    .width  = 300,
    .height = 550
};

#define BOARDBLOCK_WIDTH_SIZE screen.width/12
#define BOARDBLOCK_HEIGHT_SIZE screen.height/22

//////////////////functions//////////////////////////

void block_velocity(int* velocity_counter, int velocity, Vector2* block_position)
{
	if (*velocity_counter % velocity == 0) {
		block_position->y += BOARDBLOCK_WIDTH_SIZE;
		*velocity_counter = 0;
	}
	(*velocity_counter)++;
}

//

bool is_colliding__board(Vector2 position, char direction)
{
	if (direction == 'r') {
		return !(position.x != (BOARDBLOCK_WIDTH_SIZE)*9);
	} else if (direction == 'l') {
		return !(position.x != (BOARDBLOCK_WIDTH_SIZE)*1);
	}
}

//

Texture2D load_images(char* path, int resize_width, int resize_height)
{
	Image image = LoadImage(path);
	ImageResize(&image, resize_width, resize_height);
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	return texture;
}

///////////////////////////////////////////////////////////////

int main(void)
{
	InitWindow(screen.width, screen.height, "fma tetris");
	SetTargetFPS(60);

	Texture2D board_texture = load_images("assets/Board.png", screen.width, screen.height);
	//

	Vector2 block_position = {
		(BOARDBLOCK_WIDTH_SIZE) * 5,
		(BOARDBLOCK_HEIGHT_SIZE) - 100
	};
	Texture2D green_block = load_images("assets/Green.png",
		(BOARDBLOCK_WIDTH_SIZE)*2,
		(BOARDBLOCK_HEIGHT_SIZE)*2
	);

	//
	int velocity_counter = 0;
	int velocity = 10;

	while (!WindowShouldClose()) {
		//
		if (IsKeyPressed(KEY_RIGHT) &&
			!is_colliding__board(block_position, 'r')) {
			block_position.x += BOARDBLOCK_WIDTH_SIZE;
		} else if (IsKeyPressed(KEY_LEFT) &&
			!is_colliding__board(block_position, 'l')) {
				block_position.x -= BOARDBLOCK_WIDTH_SIZE;
		} else if (IsKeyDown(KEY_DOWN)) {
    		block_position.y += (BOARDBLOCK_HEIGHT_SIZE);
    }

		//
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawTexture(
				board_texture,
				screen.width/2 - board_texture.width/2,
				screen.height/2 - board_texture.height/2,
				WHITE
			);

			DrawTextureV(green_block, block_position, WHITE);
			DrawFPS(10, 3);
		EndDrawing();
		//

		block_velocity(&velocity_counter, velocity, &block_position);

		if (block_position.y >= 550)
			block_position.y = -25.0;
	}

	UnloadTexture(board_texture);
	UnloadTexture(green_block);

	CloseWindow();

	return 0;
}
