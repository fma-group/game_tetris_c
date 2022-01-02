#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

struct {
    int width, height;
} screen = {
    .width  = 300,
    .height = 550
};

#define BOARDBLOCK_WIDTH_SIZE screen.width/12

#define BOARDBLOCK_HEIGHT_SIZE screen.height/22

//

typedef struct {
    char* path;
    int width;
    int height;
    int x_max;
} Piece_Struct;

//
Piece_Struct pieces[7] = {
  {.path="assets/O.png", .width=50, .height=50, .x_max=9},
  {.path="assets/Z.png", .width=75, .height=50, .x_max=8},
  {.path="assets/L.png", .width=75, .height=50, .x_max=8},
  {.path="assets/T.png", .width=75, .height=50, .x_max=8},
  {.path="assets/I.png", .width=25, .height=100, .x_max=10},
  {.path="assets/J.png", .width=75, .height=50, .x_max=8},
  {.path="assets/S.png", .width=75, .height=50, .x_max=8}
};

//

typedef struct {
    Texture2D texture;
    Vector2 vector;
    int width;
    int height;
    int rotation;
} Piece_Interface;

Piece_Interface piece_variable;

//

//////////////////functions//////////////////////////

void block_velocity(int* velocity_counter, int velocity, Piece_Interface* block_position)
{
	if (*velocity_counter % velocity == 0) {
		block_position->vector.y += BOARDBLOCK_WIDTH_SIZE;
		*velocity_counter = 0;
	}
	(*velocity_counter)++;
}

//

bool is_colliding__board(Piece_Interface piece, char direction)
{
    Vector2 position = piece.vector;
    int width = piece.width - BOARDBLOCK_WIDTH_SIZE;
	if (direction == 'r') {
        return !(position.x != ((BOARDBLOCK_WIDTH_SIZE)*10)-width);
	} else if (direction == 'l') {
        return !(position.x != ((BOARDBLOCK_WIDTH_SIZE)*1));
	}
}

//

Texture2D load_image(char* path, int resize_width, int resize_height)
{
    Image image = LoadImage(path);
    ImageResize(&image, resize_width, resize_height);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

//

void piece_generator(Piece_Interface* piece_variable)
{
    int random_piece = rand() % 6;
    int x_piece = ((rand() % pieces[random_piece].x_max) + 1)*BOARDBLOCK_WIDTH_SIZE;
    Texture2D texture = load_image(pieces[random_piece].path,
    pieces[random_piece].width,pieces[random_piece].height);
    Vector2 vector = {x_piece, BOARDBLOCK_HEIGHT_SIZE};
    UnloadTexture(piece_variable->texture);
    piece_variable->texture = texture;
    piece_variable->vector = vector;
    piece_variable->width = pieces[random_piece].width;
    piece_variable->height = pieces[random_piece].height;
    piece_variable->rotation = 0;
}

//

void rotation_piece(Piece_Interface* piece_variable)
{
    /* all buggy
    int width = piece_variable->width;
    int height = piece_variable->height;
    switch (piece_variable->rotation) {
        case 0:
            {
                piece_variable->rotation = 90;
                piece_variable->width = height;
                piece_variable->height = width;
                piece_variable->vector->x = piece_variable->vector->x +
            }
            break;
        case 90:
            {
                piece_variable->rotation = 180;
                piece_variable->width = height;
                piece_variable->height = width;
            }
            break;
        case 180:
            {
                piece_variable->rotation = 270;
                piece_variable->width = height;
                piece_variable->height = width;
            }
            break;
        case 270:
            {
                piece_variable->rotation = 0;
                piece_variable->width = height;
                piece_variable->height = width;
            }
            break;
    }
*/
}


///////////////////////////////////////

int main(void)
{
	InitWindow(screen.width, screen.height, "fma tetris");
	SetTargetFPS(60);

    Texture2D board_texture = load_image("assets/Board.png", screen.width, screen.height);
    piece_generator(&piece_variable);
	//
	//
	int velocity_counter = 0;
	int velocity = 10;
    int rotation = 0;
	while (!WindowShouldClose()) {
		//
		if (IsKeyPressed(KEY_RIGHT) &&
			!is_colliding__board(piece_variable, 'r')) {
			piece_variable.vector.x += BOARDBLOCK_WIDTH_SIZE;
		} else if (IsKeyPressed(KEY_LEFT) &&
			!is_colliding__board(piece_variable, 'l')) {
				piece_variable.vector.x -= BOARDBLOCK_WIDTH_SIZE;
		} else if (IsKeyDown(KEY_DOWN)) {
    		piece_variable.vector.y += (BOARDBLOCK_HEIGHT_SIZE);
        } else if (IsKeyDown(KEY_UP) && !rotation) {
            rotation_piece(&piece_variable);
            rotation = 1;
        } else if (IsKeyUp(KEY_UP)) {
            rotation = 0;
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

      //
			DrawTextureEx(piece_variable.texture, piece_variable.vector,piece_variable.rotation,1, WHITE);
			DrawFPS(10, 3);
		EndDrawing();
		//

		block_velocity(&velocity_counter, velocity, &piece_variable);

		if (piece_variable.vector.y >= 550-piece_variable.height)
			piece_generator(&piece_variable);
	}

	UnloadTexture(board_texture);

	CloseWindow();

	return 0;
}
