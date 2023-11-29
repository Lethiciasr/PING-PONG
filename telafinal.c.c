#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int inicializarSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro ao inicializar a SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("Erro ao inicializar a TTF: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    return 0;
}

TTF_Font* carregarFonte(const char* arquivo, int tamanho) {
    TTF_Font* fonte = TTF_OpenFont(arquivo, tamanho);
    if (!fonte) {
        printf("Erro ao carregar a fonte: %s\n", TTF_GetError());
    }
    return fonte;
}

SDL_Texture* renderizarTexto(SDL_Renderer* renderer, TTF_Font* fonte, const char* texto, SDL_Color cor) {
    SDL_Surface* superficieTexto = TTF_RenderText_Blended(fonte, texto, cor);
    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, superficieTexto);
    SDL_FreeSurface(superficieTexto);
    return textura;
}

void exibirRanking(SDL_Renderer* renderer, TTF_Font* fonte, SDL_Color corTexto) {
    const int windowWidth = 800;
    const int windowHeight = 600;

    SDL_Window* rankingJanela = SDL_CreateWindow("Ranking", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* rankingRenderer = SDL_CreateRenderer(rankingJanela, -1, SDL_RENDERER_ACCELERATED);

    const char* rankingTextos[10] = {
        "1. Guilherme ",
        "2. Daniel",
        "3. Duda",
        "4. Lethicia",
        "5. ",
        "6. ",
        "7. ",
        "8. ",
        "9. ",
        "10. "
    };

    int posY = 50;
    const int verticalSpacing = 20;  

    for (int i = 0; i < 10; ++i) {
        SDL_Texture* texturaRanking = renderizarTexto(rankingRenderer, fonte, rankingTextos[i], corTexto);
        int largura, altura;
        SDL_QueryTexture(texturaRanking, NULL, NULL, &largura, &altura);
        int posX = (windowWidth - largura) / 2;
        SDL_Rect destino = {posX, posY, largura, altura};
        SDL_RenderCopy(rankingRenderer, texturaRanking, NULL, &destino);
        posY += altura + verticalSpacing;  
        SDL_DestroyTexture(texturaRanking);
    }

    SDL_RenderPresent(rankingRenderer);

    int rankingRodando = 1;
    while (rankingRodando) {
        SDL_Event rankingEvento;
        while (SDL_PollEvent(&rankingEvento)) {
            if (rankingEvento.type == SDL_QUIT) {
                rankingRodando = 0;
            }
        }
    }

    SDL_DestroyRenderer(rankingRenderer);
    SDL_DestroyWindow(rankingJanela);
}

int main() {
    if (inicializarSDL() != 0) {
        return 1;
    }

    const int windowWidth = 800;
    const int windowHeight = 600;

    SDL_Window* janela = SDL_CreateWindow("Janela Final", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* fonte = carregarFonte("/home/sdl2/Downloads/roboto/Roboto-Black.ttf", 24);
    SDL_Color corTexto = {255, 255, 255, 255};

    const char* mensagem = "Ganhador: Jogador 1";
    SDL_Texture* texturaMensagem = renderizarTexto(renderer, fonte, mensagem, corTexto);

    const char* mensagemBotao = "Jogar novamente";
    SDL_Texture* texturaBotao = renderizarTexto(renderer, fonte, mensagemBotao, corTexto);

    const char* mensagemRanking = "Ranking";
    SDL_Texture* texturaRanking = renderizarTexto(renderer, fonte, mensagemRanking, corTexto);

    int rodando = 1;

    while (rodando) {
        SDL_Event evento;
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                rodando = 0;
            }
            else if (evento.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                int larguraBotao, alturaBotao;
                SDL_QueryTexture(texturaBotao, NULL, NULL, &larguraBotao, &alturaBotao);
                int xBotao = (windowWidth - larguraBotao) / 2;
                int yBotao = (windowHeight - alturaBotao) / 2 + 50;

                if (mouseX >= xBotao && mouseX <= xBotao + larguraBotao && mouseY >= yBotao && mouseY <= yBotao + alturaBotao) {
                    
                    mensagem = (mensagem[10] == '1') ? "Ganhador: Jogador 2" : "Ganhador: Jogador 1";
                    texturaMensagem = renderizarTexto(renderer, fonte, mensagem, corTexto);
                }

                int larguraRanking, alturaRanking;
                SDL_QueryTexture(texturaRanking, NULL, NULL, &larguraRanking, &alturaRanking);
                int xRanking = (windowWidth - larguraRanking) / 2;
                int yRanking = (windowHeight - alturaRanking) / 2 + 100;

                if (mouseX >= xRanking && mouseX <= xRanking + larguraRanking && mouseY >= yRanking && mouseY <= yRanking + alturaRanking) {
                    exibirRanking(renderer, fonte, corTexto);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int largura, altura;
        SDL_QueryTexture(texturaMensagem, NULL, NULL, &largura, &altura);
        int x = (windowWidth - largura) / 2;
        int y = (windowHeight - altura) / 2;

        SDL_Rect destino = {x, y, largura, altura};
        SDL_RenderCopy(renderer, texturaMensagem, NULL, &destino);

        SDL_QueryTexture(texturaBotao, NULL, NULL, &largura, &altura);
        x = (windowWidth - largura) / 2;
        y = (windowHeight - altura) / 2 + 50;

        destino.x = x;
        destino.y = y;
        destino.w = largura;
        destino.h = altura;
        SDL_RenderCopy(renderer, texturaBotao, NULL, &destino);

        SDL_QueryTexture(texturaRanking, NULL, NULL, &largura, &altura);
        x = (windowWidth - largura) / 2;
        y = (windowHeight - altura) / 2 + 100;

        destino.x = x;
        destino.y = y;
        destino.w = largura;
        destino.h = altura;
        SDL_RenderCopy(renderer, texturaRanking, NULL, &destino);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texturaMensagem);
    SDL_DestroyTexture(texturaBotao);
    SDL_DestroyTexture(texturaRanking);
    TTF_CloseFont(fonte);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(janela);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
