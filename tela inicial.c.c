#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* renderizarTexto(SDL_Renderer* renderer, TTF_Font* fonte, const char* texto, SDL_Color cor);
void exibirNovaJanela(SDL_Renderer* renderer, TTF_Font* fonte, const char* titulo, const char* texto);

const char* textoNaJanela = "";

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Falha na inicialização do SDL: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Falha na inicialização do SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window* janela = SDL_CreateWindow("Menu do Jogo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!janela) {
        fprintf(stderr, "Falha na criação da janela: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Falha na criação do renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(janela);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* fonte = TTF_OpenFont("/home/sdl2/Downloads/roboto/Roboto-Light.ttf", 36);
    if (!fonte) {
        fprintf(stderr, "Falha ao carregar a fonte: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(janela);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Color corBotao = {255, 255, 255}; // Branco
    SDL_Color corTexto = {0, 0, 0}; // Preto

    int sair = 0;
    SDL_Event evento;

    while (!sair) {
        while (SDL_PollEvent(&evento) != 0) {
            if (evento.type == SDL_QUIT) {
                sair = 1;
            }

            if (evento.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 250 && mouseX <= 550 && mouseY >= 200 && mouseY <= 250) {
                    // Botão "Jogar"
                    exibirNovaJanela(renderer, fonte, "Jogar", "Nova Janela Jogar");
                } else if (mouseX >= 250 && mouseX <= 550 && mouseY >= 300 && mouseY <= 350) {
                    textoNaJanela = " Regras do Ping Pong\n \n \n 1. Objetivo: Marcar pontos ao fazer a bola colidir com a margem inimiga\n \n \n 2. Pontos: Cada vez que a bola toca a margem inimiga, marca um ponto\n \n \n 3. Saque: O jogo inicia com um saque. Apertar a tecla SPACE para sacar\n \n \n 4. Comandos: o jogador 1 deve utilizar as teclas W e S e o jogador 2 deve usar UP e DOWN\n \n \n 5. Ganhador: O primeiro a atingir a margem oposta mais vezes vence o jogo";
                    exibirNovaJanela(renderer, fonte, "Regras", textoNaJanela);
                } else if (mouseX >= 250 && mouseX <= 550 && mouseY >= 400 && mouseY <= 450) {
                    textoNaJanela = "Integrantes do grupo \n \n \n 1.Daniel Costa \n \n \n 2.Guilherme de Oliveira\n \n \n 3.Lethicia Silva\n \n \n 4.Maria Eduarda";
                    exibirNovaJanela(renderer, fonte, "Integrantes", textoNaJanela);
                } else if (mouseX >= 250 && mouseX <= 550 && mouseY >= 500 && mouseY <= 550) {
                    // Botão "Sair"
                    sair = 1;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Cor de fundo preto
        SDL_RenderClear(renderer);

        SDL_Rect botaoJogar = {250, 200, 300, 50};
        SDL_Rect botaoRanking = {250, 300, 300, 50};
        SDL_Rect botaoJogadores = {250, 400, 300, 50};
        SDL_Rect botaoSair = {250, 500, 300, 50};

        SDL_SetRenderDrawColor(renderer, corBotao.r, corBotao.g, corBotao.b, corBotao.a);
        SDL_RenderFillRect(renderer, &botaoJogar);
        SDL_RenderFillRect(renderer, &botaoRanking);
        SDL_RenderFillRect(renderer, &botaoJogadores);
        SDL_RenderFillRect(renderer, &botaoSair);

        SDL_Texture* textoBotaoJogar = renderizarTexto(renderer, fonte, "Jogar", corTexto);
        SDL_Texture* textoBotaoRanking = renderizarTexto(renderer, fonte, "Regras", corTexto);
        SDL_Texture* textoBotaoJogadores = renderizarTexto(renderer, fonte, "Integrantes", corTexto);
        SDL_Texture* textoBotaoSair = renderizarTexto(renderer, fonte, "Sair", corTexto);

        int botaoJogarW, botaoJogarH;
        SDL_QueryTexture(textoBotaoJogar, NULL, NULL, &botaoJogarW, &botaoJogarH);

        SDL_Rect rectTextoBotaoJogar = {botaoJogar.x + (botaoJogar.w - botaoJogarW) / 2, botaoJogar.y + (botaoJogar.h - botaoJogarH) / 2, 0, 0};

        SDL_RenderCopy(renderer, textoBotaoJogar, NULL, &rectTextoBotaoJogar);

        SDL_DestroyTexture(textoBotaoJogar);
        SDL_DestroyTexture(textoBotaoRanking);
        SDL_DestroyTexture(textoBotaoJogadores);
        SDL_DestroyTexture(textoBotaoSair);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(fonte);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(janela);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

SDL_Texture* renderizarTexto(SDL_Renderer* renderer, TTF_Font* fonte, const char* texto, SDL_Color cor) {
    SDL_Surface* superficie = TTF_RenderText_Blended_Wrapped(fonte, texto, cor, 800);
    if (!superficie) {
        fprintf(stderr, "Falha ao renderizar texto: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, superficie);
    if (!textura) {
        fprintf(stderr, "Falha ao criar textura a partir da superfície: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(superficie);

    return textura;
}

void exibirNovaJanela(SDL_Renderer* renderer, TTF_Font* fonte, const char* titulo, const char* texto) {
    SDL_RenderClear(renderer);

    // Define a cor de fundo como preto
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, NULL);

    fonte = TTF_OpenFont("/home/sdl2/Downloads/roboto/Roboto-Light.ttf", 24);
    textoNaJanela = texto;

    SDL_Color corTexto = {255, 255, 255}; // Cor branca para o texto

    SDL_Texture* texturaTexto = renderizarTexto(renderer, fonte, textoNaJanela, corTexto);

    int largura, altura;
    SDL_QueryTexture(texturaTexto, NULL, NULL, &largura, &altura);

    int x = (800 - largura) / 2;
    int y = (600 - altura) / 2;

    SDL_Rect destino = {x, y, largura, altura};
    SDL_RenderCopy(renderer, texturaTexto, NULL, &destino);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texturaTexto);

    SDL_Event evento;
    int sair = 0;
    while (!sair) {
        while (SDL_PollEvent(&evento) != 0) {
            if (evento.type == SDL_QUIT) {
                sair = 1;
            }
        }
    }
}
