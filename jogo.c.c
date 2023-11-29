#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

//Se nao tiver a fonte de texto instalada devera mudar o caminho da fonte para uma de sua preferencia

const int WIDTH = 640;
const int HEIGHT = 480;
const int BALL_SIZE = 10;

typedef struct Ball {
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} Ball;

typedef struct Player {
    float yPosition;
} Player;

//tela

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 75;
const int PLAYER_MARGIN = 10;

const float PLAYER_MOVE_SPEED = 150.0f;

bool served = false;
Ball ball;

Player player1;
Player player2;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font;

int player1Score = 0;
int player2Score = 0;
int winner = 0;

void OpenBlueWindow(void) {
    SDL_Window* blueWindow = SDL_CreateWindow("Blue Window",
                                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                             200, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer* blueRenderer = SDL_CreateRenderer(blueWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(blueRenderer, 0, 0, 255, 255);
    SDL_RenderClear(blueRenderer);

    //GANHADOR
    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font* winnerFont = TTF_OpenFont("/home/sdl2/Downloads/roboto/Roboto-Black.ttf", 20); //fonte
    if (!winnerFont) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }

    char winnerText[] = "GANHADOR!!";
    SDL_Surface* winnerSurface = TTF_RenderText_Solid(winnerFont, winnerText, textColor);
    SDL_Texture* winnerTexture = SDL_CreateTextureFromSurface(blueRenderer, winnerSurface);

    SDL_Rect winnerRect = {.x = 50, .y = 50, .w = winnerSurface->w, .h = winnerSurface->h};
    SDL_RenderCopy(blueRenderer, winnerTexture, NULL, &winnerRect);

    SDL_RenderPresent(blueRenderer);
    SDL_Delay(2000);


    SDL_FreeSurface(winnerSurface);
    SDL_DestroyTexture(winnerTexture);
    TTF_CloseFont(winnerFont);

    SDL_DestroyRenderer(blueRenderer);
    SDL_DestroyWindow(blueWindow);
}

//GANHADOR

void OpenRedWindow(void) {
    SDL_Window* redWindow = SDL_CreateWindow("Red Window",
                                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                            200, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer* redRenderer = SDL_CreateRenderer(redWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(redRenderer, 255, 0, 0, 255);
    SDL_RenderClear(redRenderer);


    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font* winnerFont = TTF_OpenFont("/home/sdl2/Downloads/roboto/Roboto-Black.ttf", 20);
    if (!winnerFont) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }

    char winnerText[] = "GANHADOR!!";
    SDL_Surface* winnerSurface = TTF_RenderText_Solid(winnerFont, winnerText, textColor);
    SDL_Texture* winnerTexture = SDL_CreateTextureFromSurface(redRenderer, winnerSurface);

    SDL_Rect winnerRect = {.x = 50, .y = 50, .w = winnerSurface->w, .h = winnerSurface->h};
    SDL_RenderCopy(redRenderer, winnerTexture, NULL, &winnerRect);

    SDL_RenderPresent(redRenderer);
    SDL_Delay(2000);


    SDL_FreeSurface(winnerSurface);
    SDL_DestroyTexture(winnerTexture);
    TTF_CloseFont(winnerFont);

    SDL_DestroyRenderer(redRenderer);
    SDL_DestroyWindow(redWindow);
}

bool Initialize(void);
void Update(float);
void Shutdown(void);

Ball MakeBall(int size);
void UpdateBall(Ball* ball, float elapsed);
void RenderBall(const Ball*);

Player MakePlayers(void);
void UpdatePlayers(float elapsed);
void RenderPlayers(void);

void CheckScore(void);
void RenderScores(void);

int main(int argc, const char* argv[]) {
    srand((unsigned int)time(NULL));
    atexit(Shutdown);

    if (!Initialize()) {
        exit(1);
    }

    bool quit = false;
    SDL_Event event;

    Uint32 lastTick = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        Uint32 curTick = SDL_GetTicks();
        Uint32 diff = curTick - lastTick;
        float elapsed = diff / 1000.0f;
        Update(elapsed);

        RenderScores();

        lastTick = curTick;
    }

    SDL_Quit();
}

bool Initialize(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("PING PONG",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        return false;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", TTF_GetError()); //FONTE
        return false;
    }

    font = TTF_OpenFont("/home/sdl2/Downloads/roboto/Roboto-Black.ttf", 15);

    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return false;
    }

    ball = MakeBall(BALL_SIZE);
    player1 = MakePlayers();
    player2 = MakePlayers();

    return true;
}

void Update(float elapsed) {
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);

    UpdateBall(&ball, elapsed);
    RenderBall(&ball);

    UpdatePlayers(elapsed);
    RenderPlayers();
    RenderScores();

    SDL_RenderPresent(renderer);
}

void Shutdown(void) {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

bool CoinFlip(void) {
    return rand() % 2 == 1 ? true : false;
}

Ball MakeBall(int size) {
    const float SPEED = 120;

    Ball ball = {
        .x = WIDTH / 2 - size / 2,
        .y = HEIGHT / 2 - size / 2,
        .size = size,
        .xSpeed = SPEED * (CoinFlip() ? 1 : -1),
        .ySpeed = SPEED * (CoinFlip() ? 1 : -1),
    };

    return ball;
}

void RenderBall(const Ball* ball) {
    int size = ball->size;
    int halfSize = size / 2;

    SDL_Rect rect = {
        .x = ball->x - halfSize,
        .y = ball->y - halfSize,
        .w = size,
        .h = size,
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //COR DA BOLA
    SDL_RenderFillRect(renderer, &rect);
}

void UpdateBall(Ball* ball, float elapsed) {
    if (!served) {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        return;
    }

    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;
    CheckScore();

    if (ball->x < BALL_SIZE / 2) {
        ball->xSpeed = fabs(ball->xSpeed);
        served = false;
        OpenRedWindow();  // tela do ganhador vermelho
    }

    if (ball->x > WIDTH - BALL_SIZE / 2) {
        ball->xSpeed = -fabs(ball->xSpeed);
        served = false;
        OpenBlueWindow();  // tela do ganhador azul
    }

    if (ball->y < BALL_SIZE / 2) {
        ball->ySpeed = fabs(ball->ySpeed);
    }

    if (ball->y > HEIGHT - BALL_SIZE / 2) {
        ball->ySpeed = -fabs(ball->ySpeed);
    }
}

Player MakePlayers(void) {
    Player player = {
        .yPosition = HEIGHT / 2,
    };

    return player;
}

void UpdatePlayers(float elapsed) {
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_SPACE]) {
        served = true;
    }

    if (keyboardState[SDL_SCANCODE_W]) {
        player1.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }

    if (keyboardState[SDL_SCANCODE_S]) {
        player1.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }

    if (keyboardState[SDL_SCANCODE_UP]) {
        player2.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }

    if (keyboardState[SDL_SCANCODE_DOWN]) {
        player2.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }

    SDL_Rect ballRect = {
        .x = ball.x - ball.size / 2,
        .y = ball.y - ball.size / 2,
        .w = ball.size,
        .h = ball.size,
    };

    SDL_Rect player1Rect = {
        .x = PLAYER_MARGIN,
        .y = (int)(player1.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &player1Rect)) {
        ball.xSpeed = fabs(ball.xSpeed);
    }

    SDL_Rect player2Rect = {
        .x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = (int)(player2.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };

    if (SDL_HasIntersection(&ballRect, &player2Rect)) {
        ball.xSpeed = -fabs(ball.xSpeed);
    }
}

void RenderPlayers(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //COR DOS JOGADORES

    SDL_Rect player1Rect = {
        .x = PLAYER_MARGIN,
        .y = (int)(player1.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };

    SDL_RenderFillRect(renderer, &player1Rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //COR DOS JOGADORES

    SDL_Rect player2Rect = {
        .x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = (int)(player2.yPosition) - PLAYER_HEIGHT / 2,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };

    SDL_RenderFillRect(renderer, &player2Rect);
}

//contagem de pontos

void CheckScore(void) {
    if (ball.x < 0) {
        player2Score++;
        printf("Player 2 score: %d\n", player2Score);
        served = false;
        ball = MakeBall(BALL_SIZE);
    } else if (ball.x > WIDTH) {
        player1Score++;
        printf("Player 1 Score: %d\n", player1Score);
        served = false;
        ball = MakeBall(BALL_SIZE);
    }
}

void RenderScores(void) {
    SDL_Color textColor = {255, 255, 255, 255}; //COR DO SCORE
    char player1ScoreText[20];
    char player2ScoreText[20];

    sprintf(player1ScoreText, "Player 1: %d", player1Score);
    sprintf(player2ScoreText, "Player 2 %d", player2Score);

    SDL_Surface* player1Surface = TTF_RenderText_Solid(font, player1ScoreText, textColor);
    SDL_Surface* player2Surface = TTF_RenderText_Solid(font, player2ScoreText, textColor);

    SDL_Texture* player1Texture = SDL_CreateTextureFromSurface(renderer, player1Surface);
    SDL_Texture* player2Texture = SDL_CreateTextureFromSurface(renderer, player2Surface);

    SDL_Rect player1Rect = {.x = 20, .y = 20, .w = player1Surface->w, .h = player1Surface->h};
    SDL_Rect player2Rect = {.x = WIDTH - 20 - player2Surface->w, .y = 20,
        .w = player2Surface->w, .h = player2Surface->h};

    SDL_RenderCopy(renderer, player1Texture, NULL, &player1Rect);
    SDL_RenderCopy(renderer, player2Texture, NULL, &player2Rect);

    SDL_FreeSurface(player1Surface);
    SDL_FreeSurface(player2Surface);
    SDL_DestroyTexture(player1Texture);
    SDL_DestroyTexture(player2Texture);
}
