#include "raylib.h"
#include "raymath.h"
#include <string>
#include "unistd.h"

int main()
{
    //game window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 960; //width
    windowDimensions[1] = 640; //height

    //initialize the game window
    InitWindow(windowDimensions[0], windowDimensions[1], "Destiny 2: The Adventure");

    //set the game to 60fps
    SetTargetFPS(60);

    //load the map
    Texture2D map = LoadTexture("maps/security.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{1.0f};

    //load the character
    Texture2D texture{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
    Vector2 characterPos{};

    //if character is facing right, value = 1, facing left = -1
    float rightLeft{1.f};
    //animation variables
    float runningTime{};
    int frame{};
    int maxFrames{6};
    float updateTime{1.f/12.f};
    float speed{4.f};
    float width{};
    float height{};
    Vector2 worldPosLastFrame{};
    float scale{4.0f};
    Vector2 velocity{};

    

    
    //begin game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        characterPos = Vector2Scale(characterPos, -1.f);

        //draw the map
        DrawTextureEx(map,mapPos, 0.0, mapScale, WHITE);

        knight.tick(GetFrameTime());
        //check map bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x > map.width * mapScale ||
            knight.getWorldPos().y > map.height * mapScale)
        {
            //knight.undoMovement();
        }
        float position_x{0.0f};
        float position_y{0.0f};
        //check if we are alive or dead
        if (!getAlive()) return;
        
        //check for movement of character
        
        if (IsKeyDown(KEY_A)) position_x -= 1.0;
        if (IsKeyDown(KEY_D)) position_x += 1.0;
        if (IsKeyDown(KEY_W)) velocity.y -= 1.0;
        if (IsKeyDown(KEY_S)) velocity.y += 1.0;

        worldPosLastFrame = worldPos;

        //update animation frame
        runningTime += deltaTime;
        if (runningTime >= updateTime)
        {
            frame++;
            runningTime = 0.f;
            if (frame > maxFrames) frame = 0;
        }

        //if character is not standing still, move the map to show movement
        if (Vector2Length(velocity) != 0.0)
        {
            //set worldPos = worldPos + velocity
            worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
            velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;

            //set the knight sprite to the animated knight_run
            texture = run;
        }
        else
        {
            //set the knight sprite to the idle knight
            texture = idle;
        }
        //set velocity back to zero
        velocity = {};

        //draw the knight
        Rectangle source{frame * width, 0.f, rightLeft * width, height};
        Rectangle dest{getScreenPos().x, getScreenPos().y, scale* width, scale * height};
        DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);

        Vector2 origin{};
        Vector2 offset{};
        float rotation{};

        if (rightLeft > 0.f)
        {
            //character is facing right
            origin = {0.f, weapon.height * scale};
            offset = {35.f, 55.f};
            weaponCollisionRec = {
                getScreenPos().x + offset.x,
                getScreenPos().y + offset.y - weapon.height * scale,
                weapon.width * scale,
                weapon.height * scale
            };
            rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 35.f : 0.f;
        }
        else
        {
            //character is facing left
            origin = {weapon.width * scale, weapon.height * scale};
            offset = {25.f, 55.f};
            rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -35.f : 0.f;
        }

        //draw the sword
        Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft , static_cast<float>(weapon.height)};
        Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
        DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);

        EndDrawing();
    }
    
    //unload textures to release memory
    UnloadTexture(texture);
    UnloadTexture(idle);
    UnloadTexture(run);
    UnloadTexture(map);
    
    //close out window to release memory
    CloseWindow();
    return 0;    
}