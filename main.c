#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char* name;
    int width;
    int height;
    int x_max;
} Piece_Struct;

//
Piece_Struct pieces[7] = {
  {.path="assets/O.png", .name="assets/O", .width=50, .height=50, .x_max=9},
  {.path="assets/Z.png", .name="assets/Z", .width=75, .height=50, .x_max=8},
  {.path="assets/L.png", .name="assets/L", .width=75, .height=50, .x_max=8},
  {.path="assets/T.png", .name="assets/T", .width=75, .height=50, .x_max=8},
  {.path="assets/I.png", .name="assets/I", .width=25, .height=100, .x_max=10},
  {.path="assets/J.png", .name="assets/J", .width=75, .height=50, .x_max=8},
  {.path="assets/S.png", .name="assets/S", .width=75, .height=50, .x_max=8}
};

//

typedef struct {
    char* name;
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
    piece_variable->name = pieces[random_piece].name;
}
//

void update_piece_s_function(Piece_Interface** piece_variable, char* buffer,int rotation, int width, int height)
{
    (*piece_variable)->texture = load_image(buffer, height, width);
    (*piece_variable)->width = height;
    (*piece_variable)->height = width;
    if ((*piece_variable)->vector.x + height > (float) (screen.width - BOARDBLOCK_WIDTH_SIZE)){
        (*piece_variable)->vector.x = (screen.width - BOARDBLOCK_WIDTH_SIZE) - height;
    }
    (*piece_variable)->rotation = rotation;
}

//

void update_piece(Piece_Interface* piece_variable, int new_rotation)
{
    char* name_piece = piece_variable->name;
    int width = piece_variable->width;
    int height = piece_variable->height;
    switch (new_rotation) {
        case 0:{
            char buffer[15];
            strcat(strcpy(buffer, name_piece), ".png");
            UnloadTexture(piece_variable->texture);
            update_piece_s_function(&piece_variable, buffer, new_rotation, width, height);
            }
            break;
        case 90:{
            char buffer[15];
            strcat(strcpy(buffer, name_piece), "90.png");
            UnloadTexture(piece_variable->texture);
            update_piece_s_function(&piece_variable, buffer, new_rotation, width, height);
            }
            break;
        case 180:{
            char buffer[15];
            strcat(strcpy(buffer, name_piece), "180.png");
            UnloadTexture(piece_variable->texture);
            update_piece_s_function(&piece_variable, buffer, new_rotation, width, height);
            }
            break;
        case 270:{
            char buffer[15];
            strcat(strcpy(buffer, name_piece), "270.png");
            UnloadTexture(piece_variable->texture);
            update_piece_s_function(&piece_variable, buffer, new_rotation, width, height);
            }
            break;
    }
}

//

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
            update_piece(&piece_variable,
            piece_variable.rotation+90 == 360 ? 0 : piece_variable.rotation+90
            );
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
			DrawTextureEx(piece_variable.texture,
                piece_variable.vector,
                0,
                1,
                WHITE
            );


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

//'-'
