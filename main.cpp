#include <iostream>
#include <raylib.h>
using namespace std;

enum GameState{
    GAME_ACTIVE,
    GAME_ROUND_FINISHED
};

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 130, 255};
Color Light_Green = Color{130, 200, 180, 255};
Color Yellow = Color{240, 210, 90, 255};

int player_score = 0;
int cpu_score = 0;
int Round = 1;

GameState gameState = GAME_ACTIVE;


//to make the ball move:
class Ball{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() const{
        //Drawing the circle:
        DrawCircle(x, y, radius, Yellow);
    }

    //update to make the ball mov
    void Update() {
        x += speed_x;
        y += speed_y;

        //to reverse the ball when touch the screen edge:
        if (y + radius >= GetScreenHeight() || y - radius <= 0){
            //reverse y-axis:
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) //cpu wins:
        {
            cpu_score++;
            ResetBall();
        }
        if (x - radius <= 0){
            player_score++;
            ResetBall();
        }
    }

    void ResetBall(){
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }

};

//use player's keyboard to control the ball:
class Paddle{
protected:
    void LimitMovement(){
        if (y <= 0){
            y = 0;
        }
        if (y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }
public:
    float x, y;
    float width, height;
    float speed;

    //paddle draw method:
    void Draw(){
        //DrawRectangle(x,y,width, height, WHITE);
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.7, 0, WHITE);
    }

    void Update(){
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)){
            y = y + speed;
        }
        //call protected function:
        LimitMovement();

    }

};

class CpuPaddle : public Paddle{
public:
    void Update(int ball_y){
        if (y + height / 2 > ball_y){
            y = y - speed;
        }
        if (y + height / 2 <= ball_y){
            y = y + speed;
        }

        LimitMovement();
    }

};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main(){
    //cout << "Starting the Game!" << endl;

    //InitWindow(screen, screen_height, "My Pong Game");

    const int screen_width = 1280;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "My Pong Game");
    SetTargetFPS(60);

    Texture2D logo = LoadTexture("resources/raylib_logo_card.png");
    int fadeOutTime = 90;
    int fadeInTime = 180;
    Color tint = WHITE;

    SetTargetFPS(60);

    for (int i = 0; i < fadeInTime; i++) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float alpha = 1.0f;
        if (i < fadeOutTime) {

            alpha = (float)i / (float)fadeOutTime;
        } else {

            alpha = 1.0f - ((float)i - (float)fadeOutTime) / (float)(fadeInTime - fadeOutTime);
        }

        tint = Fade(WHITE, alpha);

        DrawTexture(logo, screen_width / 2 - logo.width / 2, screen_height / 2 - logo.height / 2, tint);

        EndDrawing();
    }

    UnloadTexture(logo);


    //initialized the ball object:
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    //init player:
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 9;

    //init cpu:
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 5;

    Rectangle startButton = { screen_width / 2 - 100, screen_height / 2, 200, 50 };
    bool gameStarted = false;



    while (!WindowShouldClose()) {

        if (!gameStarted) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("Beat Ai Danny", startButton.x -100, startButton.y -140, 60, BLACK);

            //start buttom:
            DrawRectangleRec(startButton, LIGHTGRAY);
            DrawText("Start", startButton.x + 70, startButton.y + 15, 20, BLACK);

            //if click
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), startButton)) {
                gameStarted = true; // change game status:
            }

            EndDrawing();
        } else {

            BeginDrawing();
            ClearBackground(Dark_Green);

            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);

            if (gameState == GAME_ACTIVE) {
                ball.Update();
                player.Update();
                cpu.Update(ball.y);

                if (player_score >= 10 || cpu_score >= 10) {
                    //cout << "Round " << Round << " Finished!" << endl;
                    gameState = GAME_ROUND_FINISHED;
                }

                ball.Draw();
                player.Draw();
                cpu.Draw();

                //DrawText(TextFormat("Player Score: %i", player_score), 10, 10, 20, BLACK);
                //DrawText(TextFormat("CPU Score: %i", cpu_score), screen_width - 150, 10, 20, BLACK);
            } else if (gameState == GAME_ROUND_FINISHED) {
                const char *winnerText = player_score >= 10 ? "You Wins!" : "Ohh...Danny Wins!";
                int winnerTextWidth = MeasureText(winnerText, 20);
                int promptTextWidth = MeasureText("Press 'N' for next round or 'Q' to quit.", 20);
                int promptTextHeight = 20;
                //DrawRectangle(screen_width / 2 - promptTextWidth / 2 - 10, screen_height / 2 + 20, promptTextWidth + 20, promptTextHeight + 10, LIGHTGRAY);
                DrawText(winnerText, screen_width / 2 - winnerTextWidth / 2, screen_height / 2 - 10, 20, RED);
                DrawText("Press 'N' for next round or 'Q' to quit.", screen_width / 2 - promptTextWidth / 2,
                         screen_height / 2 + 20, 20, BLACK);

                if (IsKeyPressed(KEY_N)) {
                    gameState = GAME_ACTIVE;
                    player_score = 0;
                    cpu_score = 0;
                    Round++;
                    ball.ResetBall();
                } else if (IsKeyPressed(KEY_Q)) {
                    break;
                }
            }



            //check for collections:
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                        Rectangle{player.x, player.y, player.width, player.height})) {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                        Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
                ball.speed_x *= -1;
            }


            //to clear the ball trace before drawing the next ball, or otherwise it will show wrong:
            //ClearBackground(Dark_Green);
            //DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            //DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);

            //Drawing the line ihe middle of the screen:
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

            //call the draw function:
            //ball.Draw();

            //call cpu:
            cpu.Draw();

            //Drawing the paddle: rec1, rec2
            //DrawRectangle(10,(screen_height / 2 - 60),25, 120, WHITE);
            //DrawRectangle(screen_width - 35,(screen_height / 2 - 60),25, 120, WHITE);
            player.Draw();

            DrawText(TextFormat("Danny"), (screen_width / 4 - 50), 730, 40, WHITE);
            DrawText(TextFormat("You"), (3 * screen_width / 4 - 70), 730, 40, WHITE);

            DrawText(TextFormat("%i", cpu_score), (screen_width / 4 - 20), 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), (3 * screen_width / 4 - 20), 20, 80, WHITE);

            EndDrawing();

        }
    }

    CloseWindow();
    return 0;
}