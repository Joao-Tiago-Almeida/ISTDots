/**
*Este projeto foi realizado no âmbito da cadeira de programação de 1ºano , referente ao curso MEEC 2ºsemestre 2017-2018.
*Autor do projeto: João Tiago Almeida 90119 MEEC.
*Data de realização: de 1.abril.2018 até 15.abril.2018.
*O projeto consiste na elaboração de um jogo aproximado do jogo oficial "DOTS".
*O jogo tem como objetivo a aprendizagem por parte dos alunos da linguagem básica de programação #C.

*O jogo consiste na união de partículas de um tabuleiro. Para isso o jogador deverá pressionar o botão do rato
e unir as partículas da mesma cor. Para tal o jogador pode realizar ligações entre partículas que se encontrem
numa posição diretamente em cima, em baixo, à esquerda ou à direita da partícula selecionada.
*/


//CÓDIGO PARA COMPILAR NO TERMINAL
//gcc ISTDots.c -g -I/usr/local/include -Wall -pedantic -std=c99 -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o ISTDots

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>           // biblioteca das strings
#include <math.h>             // biblioteca de matemática
#include <locale.h>           //biblioteca para os acentos

#define MAX(a,b)    (((a)>(b))?(a):(b))
#define M_PI 3.14159265358979323846264338327950288
#define STRING_SIZE 100       // max size for some strings
#define TABLE_SIZE 850        // main game space size
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size
#define MAX_BOARD_POS 15      // maximum size of the board
#define MARGIN 5

#define MAX_NAME_SIZE 9         // tamanho da srting( 8 espaços para o nome do jogador + 1 espaço para o '\0')
#define MIN_BOARD_SIZE 5        // tamanho minimo do tabuleiro
#define MAX_BOARD_SIZE 15       // tamanho máximo do tabuliero
#define MAX_MOVES 99            // número máximo de jogadas
#define MAX_PIECES 99           // número máximo de partículas
#define NO_COLOR 6              // número que não coresponde a cor  SILVER
#define NR_MAX_LIGACOES 225     // número máximo de ligações, se ligarmos todos as peças
#define MAX_COLORS 7            // 5 + freeze + black
#define COLOR_FREEZE 5          //cor de congelamento
#define TEXT_FILE "stats.txt"   //nome do ficheiro com as estatisticas do jogo
#define MAX_PLAYS 200           //número máximo de jogadas consecutivas
#define WINDOW_POSX_INFO 200    // initial position of the window info : x
#define WINDOW_POSY_INFO 200    // initial position of the window info : y
#define jogo_congelado 2        // não detetar qualquer evento do mouse
#define vitoria 1               // o jogador ganhou o jogo
#define play 0                  // o jogador está a jogar
#define derrota -1              // o jogador perdeu o jogo
#define saida -2                // o jogador saiu do programa
#define desistencia -3          // o jogador desisitiu do jogo


// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer **, TTF_Font ** ,  char[]);
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int, char []);
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
int RenderTable(int, int, int [], TTF_Font *, SDL_Surface **, SDL_Renderer *, int [][MAX_BOARD_POS]);
void ProcessMouseEvent(int , int , int [], int , int *, int * );
void RenderPoints(int [][MAX_BOARD_POS], int, int, int [], int, SDL_Renderer *, int, int, int [2][NR_MAX_LIGACOES], int, int, int, int , int, int);
void RenderStats( SDL_Renderer *, TTF_Font *, int [], int , int );
void filledCircleRGBA(SDL_Renderer * , int , int , int , int , int , int );

void receberParametros1(int*, int*, char[]);
void receberParametros2(int*, int[] , int*);
void resetParametros(int*, int*, int*, int [], int*);
void gerarCores(int, int, int, int [][MAX_BOARD_POS], int, int);
void limparTabuleiro(int, int, int [][MAX_BOARD_POS]);
int memoriaCores(int, int, int, int [][MAX_BOARD_POS]);
int jogada(int [2][NR_MAX_LIGACOES], int, int, int*, int, int, int, int);
void eliminacaoPecas(int [2][NR_MAX_LIGACOES], int [][MAX_BOARD_POS], int);
int comparacao(int [2][NR_MAX_LIGACOES], int, int, int, int, int, int, int);
void baixarPecas(int, int, int, int [][MAX_BOARD_POS]);
int formasFechadasCheck(int [2][NR_MAX_LIGACOES], int);
void formasFechadasDo(int, int, int, int [][MAX_BOARD_POS]);
void resetJogada(int [2][NR_MAX_LIGACOES], int*, int*);
void quadradoQuadrado(int [][MAX_BOARD_POS], int ,int, int, int [2][NR_MAX_LIGACOES]);
int checkShuffle(int, int, int [][MAX_BOARD_POS]);
void doShuffle(int[], int[][MAX_BOARD_POS], int, int, int, int, int);
void freezeFA(int [][MAX_BOARD_POS], int*, int*, int, int);
void fixFreezeFA(int [][MAX_BOARD_POS], int, int);
void quadradosAndFA(int [][MAX_BOARD_POS], int, int, int, int, int*, int, int, int [2][NR_MAX_LIGACOES]);
void calculos(int*, int [], int, int, int, int, int [][MAX_BOARD_POS], int [], int [], int [], int*);
void particulasCor(int, int , int , int , int [][MAX_BOARD_POS], int []);
int somaVetor(int []);
void backup( int*, int[], int, int []);
void RenderWIN(SDL_Renderer *, TTF_Font *, TTF_Font *);
void RenderLOSE(SDL_Renderer *, TTF_Font *, TTF_Font *);
void RenderSHUFFLE(SDL_Renderer *, TTF_Font *, TTF_Font *);
void Estatisticas(char[], int, int, int[], int, int [][MAX_PLAYS]);
void saveStats(int, int , int, int[][MAX_PLAYS], int);
void RenderINFO(SDL_Renderer *, TTF_Font *);

// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Joao Almeida";
const char myNumber[] = "IST 90119";
const int colors[3][MAX_COLORS] = {{255, 93, 255, 255, 152, 0, 192},{255, 138, 215, 140, 255, 255, 192},{255, 168, 0, 0, 152, 255, 192}};

/**
 * main function: entry point of the program
 */
int main( void )
{
  //ALUNO

    int check0 = 0, check1 = 0, check2 = 0, check3 = 0;                 //checkpoints para a jogada válida
    int quadradosflag = 0;                                              //formafechada
    int matrixJogada[2][NR_MAX_LIGACOES] = {{0}};                       //matriz de casas validas em cada jogada
    int jogadaValida = 0;                                               //jogada atual
    int bloco = 0;                                                      //número de blocos da mesma cor que vão desaparecer -1 , minhoca
    int x1 = 0, y1 = 0;                                                 //posições da primeira particula no tabuleiro (x1, y1)
    int xn = 0, yn = 0;                                                 //posições da seguinte partícula no tabuleiro (xn, yn)
    int vetorCor[MAX_COLORS] = {0,0,0,0,0,0,0};                         //número de partículas por cor antes de gerar cores, fim da jogada
    int vetorCor0[MAX_COLORS] = {0,0,0,0,0,0,0};                        //número de partículas por cor depois de gerar cores, início da jogada
    int vetorJogada[MAX_COLORS] = {0,0,0,0,0,0,0};                      //cores eliminadas na jogada
    int moveColor = 0;                                                  // cor de cada jogada
    int mTamanho = 0, nTamanho = 0, nrCores = 0, jogadasTotais = 0;     //parametros a pedir ao jogador
    int objetivosPecas[MAX_COLORS] = {0,0,0,0,0,0,0};                   //parametro a pedir ao jogador
    char nome[MAX_NAME_SIZE];                                           //nome ou nickname do jogador
    int xfreeze = -1, yfreeze = -1;                                     //inicializações das coordenadas da casa congelada
    int coresBACKUP[MAX_COLORS] = {0,0,0,0,0,0,0};                      //vetor backup de objetivosPecas[]
    int jogadasBACKUP = 0;                                              //numero backup de jogadasTotais
    int win = jogo_congelado;                                           // variavel de vitoria, derrota ou jogo e variantes
    int jogos = 0;                                                      // variável do número de jogos
    int matrixStats[2][MAX_PLAYS] = {{-1}};                             //matriz das estatisticas


    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *AppleGaramond = NULL;
    TTF_Font *Urban = NULL;
    SDL_Surface *imgs[2];
    SDL_Event event;
    int delay = 0;
    int stay = 1;       //true
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px = 0, board_size_px[2] = {0};
    int board_pos_x = 0, board_pos_y = 0;
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    int pt_x = 0, pt_y = 0;




    srand(1234);

    receberParametros1(&nTamanho, &mTamanho, nome);
    receberParametros2(&nrCores, objetivosPecas, &jogadasTotais);
    backup(&jogadasBACKUP, coresBACKUP, jogadasTotais, objetivosPecas);
    limparTabuleiro(nTamanho, mTamanho, board);
    resetJogada(matrixJogada, &bloco, &moveColor);

    board_pos_x = nTamanho;
    board_pos_y = mTamanho;

    // initialize graphics
    InitEverything(width, height, &AppleGaramond, imgs, &window, &renderer, &Urban, nome);

    while(stay)
    {

        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                exit(EXIT_FAILURE);
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_n:
                          limparTabuleiro(nTamanho, mTamanho, board);
                          freezeFA(board, &xfreeze, &yfreeze, nTamanho, mTamanho);
                          gerarCores(nrCores, nTamanho, mTamanho, board, quadradosflag, moveColor);
                          //cálculo do vetor partículas por cor antes da jogada
                          particulasCor(nTamanho, mTamanho, xfreeze, yfreeze, board, vetorCor0);
                          backup(&jogadasTotais, objetivosPecas, jogadasBACKUP, coresBACKUP);
                          jogos++;
                          if(win == jogo_congelado){
                            win = play;
                          }else if(win == play){
                            win = desistencia;
                          }
                        break;
                    case SDLK_q:
                        win = saida;
                        stay=0;
                    default:
                        break;
                }
            }
            else if ( event.type == SDL_MOUSEBUTTONDOWN && win != jogo_congelado )
            {
                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                particulasCor(nTamanho, mTamanho, xfreeze, yfreeze, board, vetorCor0);
                check0 = 1;
                moveColor = board[pt_x][pt_y];
                x1 = pt_x;
                y1 = pt_y;
            }
            else if ( event.type == SDL_MOUSEBUTTONUP && win != jogo_congelado)
            {
                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                check0 = 0;

                quadradosflag = formasFechadasCheck(matrixJogada, bloco);
                if(quadradosflag){
                    formasFechadasDo(moveColor, nTamanho, mTamanho, board);
                    quadradoQuadrado(board, nTamanho, mTamanho, bloco, matrixJogada);
                }else{
                    eliminacaoPecas(matrixJogada, board, bloco);
                }
                calculos(&jogadasTotais, objetivosPecas , nTamanho, mTamanho, xfreeze, yfreeze, board, vetorCor, vetorCor0, vetorJogada, &win);
                baixarPecas(nrCores, nTamanho, mTamanho, board);
                fixFreezeFA(board, xfreeze, yfreeze);
                gerarCores(nrCores, nTamanho, mTamanho, board, quadradosflag, moveColor);
                check2 = checkShuffle(nTamanho, mTamanho, board);
                if(check2){
                  RenderSHUFFLE(renderer, Urban, AppleGaramond);
                  SDL_RenderPresent(renderer);
                  SDL_Delay(1000);
                  doShuffle(vetorCor, board, nTamanho, mTamanho, nrCores, xfreeze, yfreeze);
                }
                resetJogada(matrixJogada, &bloco, &moveColor);
            }
            else if ( event.type == SDL_MOUSEMOTION && win != jogo_congelado)
            {
                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                if(check0){
                    check1 = memoriaCores(moveColor, pt_x, pt_y, board);
                    if(check1){
                        jogadaValida = jogada(matrixJogada, pt_x, pt_y, &bloco, x1, y1, xn, yn);
                        if(jogadaValida != -1){
                            check3 = comparacao(matrixJogada, bloco, x1, y1, xn, yn, pt_x, pt_y);
                            if(check3 == 1){
                                bloco++;
                            }else if (check3 == -1){
                              bloco--;
                              for (int k = bloco; k < NR_MAX_LIGACOES; k++){
                                  matrixJogada[0][k] =-1;
                                  matrixJogada[1][k] =-1;
                                }
                            }
                            xn = pt_x;
                            yn = pt_y;
                        }
                    }
                }
                quadradosflag = formasFechadasCheck(matrixJogada, bloco);
            }
        }
        // render game table
        square_size_px = RenderTable( board_pos_x, board_pos_y, board_size_px, AppleGaramond, imgs, renderer, board);
        // render board
        RenderPoints(board, board_pos_x, board_pos_y, board_size_px, square_size_px, renderer, pt_x,
                    pt_y, matrixJogada, bloco, win, nTamanho, mTamanho, moveColor, quadradosflag);
        // render in the screen all the stats
        RenderStats( renderer , AppleGaramond, objetivosPecas, nrCores, jogadasTotais );
        //render an instrucion message
        RenderINFO(renderer, AppleGaramond);
        // render in the screen a winnig message and decides the state of the current game
        if(win == vitoria || win == derrota || win == desistencia){
            if(win == vitoria){
              RenderWIN(renderer, Urban, AppleGaramond);
              saveStats(jogadasTotais, jogadasBACKUP, jogos, matrixStats, win);
            }else{
              RenderLOSE(renderer, Urban, AppleGaramond);
              saveStats(jogadasTotais, jogadasBACKUP, jogos, matrixStats, win);
            }

            if(win == desistencia){
              win = play;
            }else{
              win = jogo_congelado;
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(2500);
        }
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );

    }
    Estatisticas(nome, win, jogadasTotais, objetivosPecas, jogos, matrixStats);
    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(AppleGaramond);
    TTF_CloseFont(Urban);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();



    return EXIT_SUCCESS;
}

/**
 * ProcessMouseEvent: gets the square pos based on the click positions !
 * \param _mouse_pos_x position of the click on pixel coordinates
 * \param _mouse_pos_y position of the click on pixel coordinates
 * \param _board_size_px size of the board !
 * \param _square_size_px size of each square
 * \param _pt_x square nr
 * \param _pt_y square nr
 */
void ProcessMouseEvent(int _mouse_pos_x, int _mouse_pos_y, int _board_size_px[], int _square_size_px,
        int *_pt_x, int *_pt_y )
{
    // corner of the board
    int x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    int y_corner = (TABLE_SIZE - _board_size_px[1] - 15);

    // verify if valid cordinates
    if (_mouse_pos_x < x_corner || _mouse_pos_y < y_corner || _mouse_pos_x > (x_corner + _board_size_px[0])
        || _mouse_pos_y > (y_corner + _board_size_px[1]) )
    {
        *_pt_x = -1;
        *_pt_y = -1;
        return;
    }

    // computes the square where the mouse position is
    _mouse_pos_x = _mouse_pos_x - x_corner;
    _mouse_pos_y = _mouse_pos_y - y_corner;

    *_pt_x = _mouse_pos_x / (_square_size_px + SQUARE_SEPARATOR);
    *_pt_y = _mouse_pos_y / (_square_size_px + SQUARE_SEPARATOR);
}

/**
 * RenderPoints: renders the board
 * \param _board 2D array with integers representing board colors
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _square_size_px size of each square
 * \param _board_size_px size of the board
 * \param _renderer renderer to handle all rendering in a window
 * \param _pt_x posição do rato em xx
 * \param _pt_y posição do rato em yy
 * \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
 * \param _bloco linha onde se escreve na matrix_jogada
 * \param _win flag do resultado do jogo
 * \param n_tamanho número de colunas do tabuleiro
 * \param m_tamanho número de linhas do tabuleiro
 * \param _board matriz 2D com números que representam cores no tabuleiro
  * \param move_color cor da jogada
 */
void RenderPoints(int _board[][MAX_BOARD_POS], int _board_pos_x, int _board_pos_y,
        int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer,
        int _pt_x, int _pt_y , int matrix_jogada[2][NR_MAX_LIGACOES], int _bloco, int _win,
        int n_tamanho, int m_tamanho, int move_color , int quadrados_flag)
{
    int clr, x_corner, y_corner, circleX, circleY, circleR;

    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px[1] - 15);

    float raio = ((rand() % 100) / 500.0f) + 0.25f;
    // renders the squares where the dots will appear
    for ( int i = 0; i < _board_pos_x; i++ ){
        for ( int j = 0; j < _board_pos_y; j++ ){
          //jogo parado
          if(_win == jogo_congelado){
              circleR = (int)(_square_size_px*0.40f);
          //comportamento das bolas durante o jogo
          }else{
              //quando há forma fechada todos a partículas da cor da jogada mexem
              if(quadrados_flag){
                  if(_board[i][j] == move_color){
                    circleR = (int)(_square_size_px*raio*(float)_bloco*0.10f);
                }else{
                  circleR = (int)(_square_size_px*0.40f);
                }
              //identificação da partícula em que o rato se encontra (fora da jogada), pelo aumento e diminuição do raio
              }else if((i == _pt_x && j == _pt_y) && _board[_pt_x][_pt_y] != COLOR_FREEZE){
                  circleR = (int)(_square_size_px*raio);    // no quadrado em que o rato se encontra
              }else{
                  circleR = (int)(_square_size_px*0.40f);
              }
              //partículas pertencentes à jogada ficam mais pequenas e aumentam com o aumentar da jogada
              for( int k = 0; k < NR_MAX_LIGACOES; k++){
                if(matrix_jogada[0][k] == i && matrix_jogada[1][k] == j){
                  circleR = (int)(_square_size_px*raio*(float)sqrt(_bloco)*0.10f);
                }
              }
          }
            // define the size and copy the image to display
            circleX = x_corner + (i+1)*SQUARE_SEPARATOR + i*(_square_size_px)+(_square_size_px>>1);
            circleY = y_corner + (j+1)*SQUARE_SEPARATOR + j*(_square_size_px)+(_square_size_px>>1);
            // draw a circle
            clr = _board[i][j];
            filledCircleRGBA(_renderer, circleX, circleY, circleR, colors[0][clr], colors[1][clr], colors[2][clr]);
        }
    }
}

/**
 * filledCircleRGBA: renders a filled circle
 * \param _circleX x pos
 * \param _circleY y pos
 * \param _circleR radius
 * \param _r red
 * \param _g gree
 * \param _b blue
 */
void filledCircleRGBA(SDL_Renderer * _renderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);

    SDL_SetRenderDrawColor(_renderer, _r, _g, _b, 255);

    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));
            SDL_RenderDrawPoint(_renderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(_renderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(_renderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(_renderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
}

/*
 * Shows some information about the game:
 * - Goals of the game
 * - Number of moves
 * \param _renderer renderer to handle all rendering in a window
 * \param _font font to display the scores
 * \param _objetivos_pecas goals of the game
 * \param nr_cores number of colors
 * \param jpgadas_totais number of moves remaining
 */
void RenderStats( SDL_Renderer *_renderer, TTF_Font *_font, int objetivos_pecas[], int nr_cores, int jogadas_totais)
{
    SDL_Color white = {255, 255, 255 };
    int xx = 120;

    if(jogadas_totais == 0){
      SDL_SetRenderDrawColor(_renderer, 200, 0, 0, 255 );
    }else{
      SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255 );
    }
    SDL_Rect jogadas = {65, 65, 50, 40};
    SDL_RenderFillRect(_renderer, &jogadas);
    char njogadas[3];
    sprintf(njogadas, "%d", jogadas_totais);
    RenderText(80, 70, njogadas, _font, &white, _renderer);

    for(int i = 0; i < nr_cores; i++){
      if(objetivos_pecas[i] == 0){
        SDL_SetRenderDrawColor(_renderer, 0, 200, 0, 255 );
      }else{
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255 );
      }
      SDL_Rect rect = {xx + 50, 65, 60, 40};
      SDL_RenderFillRect(_renderer, &rect);
      char ncores[3];
      sprintf(ncores, "%d", objetivos_pecas[i]);
      RenderText(xx + 90, 70, ncores, _font, &white, _renderer);
      filledCircleRGBA(_renderer, xx + 65, 80, 10, colors[0][i], colors[1][i], colors[2][i]);

      xx += 140;
    }


}

/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _board_size_px size of the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 * \param _board matriz 2D com números que representam cores no tabuleiro
 */
int RenderTable( int _board_pos_x, int _board_pos_y, int _board_size_px[],
        TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer , int _board[MAX_BOARD_POS][MAX_BOARD_POS])
{
    SDL_Color blue = { 0, 191, 255 };             // nome do aluno e número
    SDL_Color brown = { 210, 105, 20 };           // cor das bordas do TABULEIRO
    SDL_Color freeze = { 0, 255, 255 };           // cor do bloco congelado
    SDL_Color silver = { 192, 192, 192 };         // cor do fundo
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size, square_size_px, max_pos;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );    //malha lateral

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _font, &blue, _renderer);

    // this renders the student number
    RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _font, &blue, _renderer);

    // compute and adjust the size of the table and squares
    max_pos = MAX(_board_pos_x, _board_pos_y);
    board_size = (int)(BOARD_SIZE_PER*TABLE_SIZE);
    square_size_px = (board_size - (max_pos+1)*SQUARE_SEPARATOR) / max_pos;
    _board_size_px[0] = _board_pos_x*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;
    _board_size_px[1] = _board_pos_y*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;

    // renders the entire board background
    SDL_SetRenderDrawColor(_renderer, brown.r, brown.g, brown.b, brown.a );
    board.x = (TABLE_SIZE - _board_size_px[0]) >> 1;
    board.y = (TABLE_SIZE - _board_size_px[1] - 15);
    board.w = _board_size_px[0];
    board.h = _board_size_px[1];
    SDL_RenderFillRect(_renderer, &board);

    // renders the squares where the numbers will appear
    // iterate over all squares
    for ( int i = 0; i < _board_pos_x; i++ )
    {
        for ( int j = 0; j < _board_pos_y; j++ )
        {
              board_square.x = board.x + (i+1)*SQUARE_SEPARATOR + i*square_size_px;
              board_square.y = board.y + (j+1)*SQUARE_SEPARATOR + j*square_size_px;
              board_square.w = square_size_px;
              board_square.h = square_size_px;
            if(_board[i][j]  != COLOR_FREEZE){
                SDL_SetRenderDrawColor(_renderer, silver.r, silver.g, silver.b, silver.a );
                SDL_RenderFillRect(_renderer, &board_square);
            }else{
                SDL_SetRenderDrawColor(_renderer, freeze.r, freeze.g, freeze.b, freeze.a );
                SDL_RenderFillRect(_renderer, &board_square);
            }
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // return for later use
    return square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 * \param _nome nome do jogador
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer, TTF_Font **_font2, char _nome[MAX_NAME_SIZE])
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height, _nome);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("AppleGaramond.ttf", 20);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // criada uma nova fonte ≠ tamanho de letra
    // this opens (loads) a font file and sets a size
    *_font2 = TTF_OpenFont("Urban.ttf", 70);
    if(!*_font2)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _nome nome do jogador
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height , char _nome[MAX_NAME_SIZE])
{
    SDL_Window *window;
    char nome[9];
    sprintf(nome, "%s", _nome);
    // init window
    window = SDL_CreateWindow( nome, WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}

/**
* receberParametros1: recebe informação sobre a dimensão do tabuleiro e o nome de utilizador
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param _nome nome do jogador
* \param _jogos número de jogos iniciados
*/
void receberParametros1(int* n_tamanho, int* m_tamanho, char _nome[MAX_NAME_SIZE]){

    char buf[STRING_SIZE];

    printf("\nIntroduza:\n\n1. A altura do tabuleiro (m linhas)(5-15): ");
    fgets(buf, STRING_SIZE, stdin);
    sscanf(buf, "%d", m_tamanho);
    while(*m_tamanho<MIN_BOARD_SIZE || *m_tamanho>MAX_BOARD_SIZE){
        printf("ERRO! O tabuleiro deverá ter entre 5 a 15 linhas, inclusive!\nVolte a introduzir o número de linhas (m): ");
        fgets(buf, STRING_SIZE, stdin);
        sscanf(buf, "%d", m_tamanho);
    }
    printf("\n2. A largura do tabuleiro (n colunas)(5-15): ");
    fgets(buf, STRING_SIZE, stdin);
    sscanf(buf, "%d", n_tamanho);
    while(*n_tamanho<MIN_BOARD_SIZE || *n_tamanho>MAX_BOARD_SIZE){
        printf("ERRO! O tabuleiro deverá ter entre 5 a 15 colunas, inclusive!\nVolte a introduzir o número de colunas (n): ");
        fgets(buf, STRING_SIZE, stdin);
        sscanf(buf, "%d", n_tamanho);
    }
      printf("\n3. O nome do utilizador (máx 8 carateres) , sem espaços: ");
      fgets(buf, STRING_SIZE, stdin);
      sscanf(buf, "%s", _nome);
      while(strlen(_nome) >= MAX_NAME_SIZE || strlen(_nome) == 0){
          printf("ERRO! O nome de utilizador tem de ter entre 1 a 8 caracteres!\nVolte a introduzir o nome: ");
          fgets(buf, STRING_SIZE, stdin);
          sscanf(buf, "%s", _nome);
      }
}

/**
* receberParametros2: recebe informação sobre as cores e objetivos para o jogo
* \param nr_cores número de cores no tabuleiro
* \param objetivos_pecas objetivos a alcaçar por cor
* \param jogadas_totais número máximo de jogadas
* \param _jogos número de jogos iniciados
*/
void receberParametros2(int* nr_cores, int objetivos_pecas[], int* jogadas_totais){

      char buf[STRING_SIZE];

     printf("\n4. O número de cores (2-5): ");
     fgets(buf, STRING_SIZE, stdin);
     sscanf(buf, "%d", nr_cores);
     while(*nr_cores < 2 || 5 < *nr_cores){
         printf("ERRO! O tabuleiro deverá ter entre 2 a 5 cores!\nVolte a introduzir o número de cores: ");
         fgets(buf, STRING_SIZE, stdin);
         sscanf(buf, "%d", nr_cores);
     }
     printf("\n5. Os objetivos de pontos por cor (2-99):\n");
     for(int i = 0 ; i < *nr_cores ; i++){
         printf("Cor %d: ", i+1);
         fgets(buf, STRING_SIZE, stdin);
         sscanf(buf, "%d", &objetivos_pecas[i]);
     }
     for(int i = 0 ; i < *nr_cores ; i++){
         while(objetivos_pecas[i] < 2 || MAX_PIECES < objetivos_pecas[i]){

             printf("ERRO! O número de peças por cor deverá ser entre 2 a 99! Volte a introduzir o objetivo para a cor %d: ", i+1);
             fgets(buf, STRING_SIZE, stdin);
             sscanf(buf, "%d", &objetivos_pecas[i]);
         }
     }
     printf("\n6. O número de jogados totais (1-99): ");
     fgets(buf, STRING_SIZE, stdin);
     sscanf(buf, "%d", jogadas_totais);
     while(*jogadas_totais < 1 || MAX_MOVES < *jogadas_totais){
         printf("ERRO! O número de jogadas totais deverá ser entre 1 a 99 por jogo!\nVolte a introduzir o número de jogadas por jogo: ");
         fgets(buf, STRING_SIZE, stdin);
         sscanf(buf, "%d", jogadas_totais);
     }
}

/**
* limparTabuleiro: remove a cor do tabuleiro
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param _board matriz 2D com números que representam cores no tabuleiro
*/
void limparTabuleiro(int n_tamanho, int m_tamanho, int _board[MAX_BOARD_POS][MAX_BOARD_POS]){

    for(int i = 0; i<m_tamanho; i++){
        for(int j = 0; j<n_tamanho; j++){

            _board[j][i] = NO_COLOR;
        }
    }
}

/**
* gerarCores gera cor no tabuleiro numa posição sem cor
* \param nr_cores número de cores no tabuleiro
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param _board matriz 2D com números que representam cores no tabuleiro
* \param quadrados_flag indica se a jogada anterior foi de forma fechada ou não
* \param move_color é a cor da jogada
*/
void gerarCores(int nr_cores, int n_tamanho, int m_tamanho, int _board[MAX_BOARD_POS][MAX_BOARD_POS], int quadrados_flag, int move_color){

    for(int i = 0; i<m_tamanho; i++){
        for(int j = 0; j<n_tamanho; j++){
            //gerar cor onde apenas onde não se encontra qualquer cor
            if(_board[j][i] == NO_COLOR && _board[j][i] != COLOR_FREEZE){
                if(quadrados_flag == 0){
                    _board[j][i] = (rand() % nr_cores);
                }else{
                    do {
                      _board[j][i] = (rand() % nr_cores);
                    } while(_board[j][i] == move_color);
                }
            }
        }
    }
}

/**
* memoriaCores: compara a cor da posição atual do rato com a cor inicial da jogada
* \param move_color é a cor da jogada
* \param _pt_x posição do rato em xx
* \param _pt_y posição do rato em yy
* \param _board matriz 2D com números que representam cores no tabuleiro
*/
int memoriaCores(int move_color, int _pt_x, int _pt_y, int board[MAX_BOARD_POS][MAX_BOARD_POS]){

    //cor da partícula == cor da jogada => 1; cor da partícula != cor da jogada => 0
    if(move_color != board[_pt_x][_pt_y]){
      return 0;
    }
    return 1;
}

/**
* jogada: escreve na matrix_jogada e previne jogadas na diagonal
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
* \param _pt_x posição do rato em xx
* \param _pt_y posição do rato em yy
* \param _bloco linha onde se escreve na matrix_jogada
* \param _x1 coordenada xx da 1ª partícula válida da jogada
* \param _y1 coordenada yy da 1ª partícula válida da jogada
* \param _xn coordenada xx da última partícula válida da jogada
* \param _yn coordenada yy da última partícula válida da jogada
*/
int jogada(int matrix_jogada[2][NR_MAX_LIGACOES], int _pt_x, int _pt_y,  int *_bloco, int _x1, int _y1, int _xn, int _yn){

     // 1ªpartíclua => 1; nova partíclua => 1; mesma partícula ou partícula errada => -1
        //guardar a posição da partícula inicial da jogada
        if((_x1 == _pt_x && _y1 == _pt_y) && matrix_jogada[0][2] + matrix_jogada[0][2] == -2){
            *_bloco = 0;
            matrix_jogada[0][*_bloco] = _pt_x;
            matrix_jogada[1][*_bloco] = _pt_y;
            return 0;

        //guardar as posições das partículas seguintes
        //impedir a diagonal
        }else if(((abs(_xn - _pt_x) == 1) && ((_yn - _pt_y) == 0)) || ((abs(_yn - _pt_y) == 1) && (_xn - _pt_x) == 0)){

            matrix_jogada[0][*_bloco] = _pt_x;
            matrix_jogada[1][*_bloco] = _pt_y;
            return 1;
        }
    return -1;
}

/**
* eliminacaoPecas: remove as cores das partículas se for uma forma aberta
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
* \param _board matriz 2D com números que representam cores no tabuleiro
* \param _bloco linha onde se escreve na matrix_jogada
*/
void eliminacaoPecas(int matrix_jogada[2][NR_MAX_LIGACOES], int board[MAX_BOARD_POS][MAX_BOARD_POS], int _bloco){

    if(_bloco-1 > 0){
        for(int i = 0; i<_bloco; i++){
            board[matrix_jogada[0][i]][matrix_jogada[1][i]] = NO_COLOR;
        }
    }
}

/**
* comparacao: controlo do sentido do movimento
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
* \param _bloco linha onde se escreve na matrix_jogada
* \param _x1 coordenada xx da 1ª partícula válida da jogada
* \param _y1 coordenada yy da 1ª partícula válida da jogada
* \param _xn coordenada xx da última partícula válida da jogada
* \param _yn coordenada yy da última partícula válida da jogada
* \param _pt_x posição do rato em xx
* \param _pt_y posição do rato em yy
*/
int comparacao(int matrix_jogada[2][NR_MAX_LIGACOES], int _bloco,  int _x1, int _y1, int _xn, int _yn, int _pt_x, int _pt_y){

    // não contraria o sentido do movimento anterior => 1; contraria o sentido do movimento anterior => -1

    //quando retrocede ao ponto inicial e não é realizado um quadrado
    //se a segunda posição da matriz das coordenadas de pontos estiver preenchida, significa que já houve pelo menos um houve movimento
    //se há realização de quadrado pelo menos a terceira posição da matriz das coordenadas de pontos tem de estar preenchida
    if((_x1 == _pt_x && _y1 == _pt_y) && matrix_jogada[0][1] + matrix_jogada[0][1] != -2 && matrix_jogada[0][2] + matrix_jogada[0][2] == -2){
        return -1;

    }else if(abs(_xn - _pt_x) == 1 || abs(_yn - _pt_y) == 1){

        if(matrix_jogada[0][_bloco] == matrix_jogada[0][_bloco-2] && matrix_jogada[1][_bloco] == matrix_jogada[1][_bloco-2]){
            return -1;
        }
    }
    return 1;
}

/**
* baixarPecas: move as partículas para baixo até encontrar outra partícula com cor (efeito cascata) e gera outras sem cor em cima
* \param nr_cores é o número de cores no tabuleiro
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param _board matriz 2D com números que representam cores no tabuleiro
*/
void baixarPecas(int nr_cores, int n_tamanho, int m_tamanho, int _board[MAX_BOARD_POS][MAX_BOARD_POS]){

    for(int i=1; i<m_tamanho; i++){
        for(int j=0; j<n_tamanho; j++){
            //se encontrar partícula sem cor após a remoção de cores
            if(_board[j][i] == NO_COLOR ){
                //efeito cascata
                //sobe na coluna para que o bloco congelado venha até ao mais acima possível
                for(int k = i; k>0; k--){               //k-1 representa a linha superior a i
                    if(_board[j][k] != COLOR_FREEZE){
                        //caso a partícula estudada estiver na segunda linha e a partícula de cima for congelada, é removida
                        if(_board[j][k-1] == COLOR_FREEZE && i == 1){
                            _board[j][k] = NO_COLOR;
                        /*caso a partícula estudada estiver após a segunda linha e a partícula de cima for congelada,
                        a partícula de cima da partícula congelada cai duas posições*/
                        }else if(_board[j][k-1] == COLOR_FREEZE && i > 1) {
                            _board[j][k] = _board[j][k-2];
                        //queda das peças, toma o valor da bola superior
                        }else{
                            _board[j][k] = _board[j][k-1];
                        }
                    }
                }
                _board[j][0] = NO_COLOR;    //partícula de cima s/ cor, se nessa linha houve pelo menos uma queda
            }
        }
    }
}

/**
* formasFechadasCheck: identifica o tipo de forma realizada
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
* \param _bloco linha onde se escreve na matrix_jogada
*/
int formasFechadasCheck(int matrix_jogada[2][NR_MAX_LIGACOES], int _bloco){

    //a função não altera o jogo => 0; a forma realizada é fechada, alterando o jogo => 1;
    for(int k = 0; k < _bloco-3; k++){
        if(matrix_jogada[0][_bloco-1] == matrix_jogada[0][k] && matrix_jogada[1][_bloco-1] == matrix_jogada[1][k]){

        return 1;
        }
    }
    return 0;
}

/**
* formasFechadasDo: remove as todas as partículas da cor inicial se for uma forma fechada
* \param move_color cor da jogada
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param _board matriz 2D com números que representam cores no tabuleiro
*/
void formasFechadasDo(int move_color, int n_tamanho, int m_tamanho, int _board[MAX_BOARD_POS][MAX_BOARD_POS]){

      for(int i = 0; i < m_tamanho; i++){
          for(int j = 0; j < n_tamanho; j++){

              if(_board[j][i] == move_color){
                  _board[j][i] = NO_COLOR;
              }
          }
      }
}

/**
* resetJogada: reinicia a matrix_jogada a {-1}, o _bloco a {0} e a cor da jogada a {-1}
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
* \param _bloco linha onde se escreve na matrix_jogada
*/
void resetJogada(int matrix_jogada[2][NR_MAX_LIGACOES], int *_bloco, int* move_color){

    for(int i = 0; i < NR_MAX_LIGACOES; i++){
        matrix_jogada[0][i] =-1;
        matrix_jogada[1][i] =-1;
    }
    *_bloco = 0;
    *move_color = -1;
}

/**
* quadradoQuadrado: análise dos três casos possíveis com a partícula congelada dentro de uma forma fechada
* \param _board matriz 2D com números que representam cores no tabuleiro
* \param _board 2D array with integers representing board colors
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param _bloco linha onde se escreve na matrix_jogada
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
*/
void quadradoQuadrado(int _board[][MAX_BOARD_POS], int n_tamanho, int m_tamanho, int _bloco, int matrix_jogada[2][NR_MAX_LIGACOES]){

    int _blocof = 0;
        while(matrix_jogada[0][_blocof] != -1){
            _blocof++;
        }
    for(int i = 1; i < m_tamanho-1; i++){
        for(int j = 1; j < n_tamanho; j++){
           //percorrer possivel fronteira de baixo
            for(int ii=i+1; ii < m_tamanho; ii++){
                //percorrer a fronteira dda direita
                for(int jj=j+1; jj < n_tamanho; jj++){
                    //caso a partícula tiver partículas da jogada em cima e à esquerda e não estiver congelada
                    if(_board[j-1][i] == NO_COLOR && _board[j][i-1] == NO_COLOR && _board[jj][i] == NO_COLOR
                       && _board[j][ii] == NO_COLOR && _board[j][i] != NO_COLOR && _board[j][i] != COLOR_FREEZE){
                        quadradosAndFA(_board, j-1, i-1, j, i, &_blocof, jj, ii, matrix_jogada);

                    //caso a partícula tiver uma partícula da jogada em cima e a da esquerda estiver congelada
                    }else if(_board[j-1][i] == COLOR_FREEZE && _board[j][i-1] == NO_COLOR && _board[jj][i] == NO_COLOR
                             && _board[j][ii] == NO_COLOR && _board[j][i] != NO_COLOR){
                        quadradosAndFA(_board, j-2, i-1, j, i, &_blocof, jj, ii, matrix_jogada);

                    //se a partícula tievr uma partícula da jogada à esquerda e a de cima estiver congelada
                    }else if(_board[j-1][i] == NO_COLOR && _board[j][i-1] == COLOR_FREEZE && _board[jj][i] == NO_COLOR
                             && _board[j][ii] == NO_COLOR && _board[j][i] != NO_COLOR){
                        quadradosAndFA(_board, j-1, i-2, j, i, &_blocof, jj, ii, matrix_jogada);
                    }
                }
            }
        }
    }
}

/**
* checkShuffle: verifica se há jogadas possíveis
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param _board matriz 2D com números que representam cores no tabuleiro
*/
int checkShuffle(int n_tamanho, int m_tamanho, int _board[][MAX_BOARD_POS]){

  //não há shuffle => 0; há shuffle => 1;
    //estudo do tabuleiro à excepção da primeira linha e primeira coluna
    for(int i = 1; i < m_tamanho; i++){
        for(int j = 1; j < n_tamanho; j++){
            if(_board[j][i] == _board[j-1][i] || _board[j][i] == _board[j][i-1]){
                return 0;
            }
        }
    }
    //estudo da primeira coluna do tabuleiro
    for(int j = 0; j < n_tamanho; j++){
        if(_board[j][0] == _board[j-1][0]){
            return 0;
        }
    }
    //estudo da primeira linha do tabuleiro
    for(int i = 0; i < n_tamanho; i++){
        if(_board[0][i] == _board[0][i-1]){
            return 0;
        }
    }
    return 1;
}

/**
* doShuffle: embaralhamento das partículas no tabuleiro
* \param _vetorCor vetor com o número de partículas por cor após a jogada
* \param _board matriz 2D com números que representam cores no tabuleiro
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param nr_cores número de cores no tabuleiro
* \param x_freeze coordenada xx da partícula congelada
* \param y_freeze coordenada yy da partícula congelada
*/
void doShuffle(int _vetorCor[], int _board[][MAX_BOARD_POS], int n_tamanho, int m_tamanho, int nr_cores, int x_freeze, int y_freeze){

  //cálculo do vetor partículas por cor após a jogada
  particulasCor(n_tamanho, m_tamanho, x_freeze, y_freeze, _board, _vetorCor);

    for(int i = 0; i < m_tamanho; i++){
        for(int j = 0; j < n_tamanho; j++){

            if(i != y_freeze || j != x_freeze){
                int cor_gerada = (rand() % nr_cores);
                //caso a cor gerada já tiver sido totalmente distribuida, é gerada uma nova
                while(_vetorCor[cor_gerada] == 0){
                    cor_gerada = (rand() % nr_cores);
                }
                _board[j][i] = cor_gerada;
                // decremento no stock de cor disponíveis
                if(_board[j][i] == 0){
                    _vetorCor[0]--;
                }else if(_board[j][i] == 1){
                    _vetorCor[1]--;
                }else if(_board[j][i] == 2){
                    _vetorCor[2]--;
                }else if(_board[j][i] == 3){
                    _vetorCor[3]--;
                }else if(_board[j][i] == 4){
                    _vetorCor[4]--;
                }
            }
        }
    }
}

/**
* freezeFA: gera a partícula congelada
* \param _board matriz 2D com números que representam cores no tabuleiro
* \param x_freeze coordenada xx da partícula congelada
* \param y_freeze coordenada yy da partícula congelada
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
*/
void freezeFA(int _board[][MAX_BOARD_POS], int *_xfreeze, int *_yfreeze, int n_tamanho, int m_tamanho){

    *_xfreeze = (rand() % n_tamanho);
    *_yfreeze = (rand() % m_tamanho);

    _board[*_xfreeze][*_yfreeze] = COLOR_FREEZE;

}

/**
* fixFreezeFA: fixa a partícula congelada
* \param _board matriz 2D com números que representam cores no tabuleiro
* \param x_freeze coordenada xx da partícula congelada
* \param y_freeze coordenada yy da partícula congelada
*/
void fixFreezeFA(int _board[][MAX_BOARD_POS], int _xfreeze, int _yfreeze){

    _board[_xfreeze][_yfreeze] = COLOR_FREEZE;
}

/**
* quadradosAndFA: remove a cor das partículas dentro da forma fechada
  //apenas para partículas de cor diferente da inicial
* \param f_board matriz 2D com números que representam cores no tabuleiro
* \_x coordenada xx da partícula anterior de referência à direita
* \_y coordenada yy da partícula anterior de referência por cima
* \_j coordenada xx da partícula a eliminar
* \_i coordenada yy da partícula a eliminar
* f_blocof linha onde se escreve na matrix_jogada
* \_jj coordenada xx da partícula anterior de referência à esquerda
* \_ii coordenada yy da partícula anterior de referência por por baixo
* \f_matrix_jogada guarda as coordenas (x,y) das partículas válidas
*/
void quadradosAndFA(int f_board[][MAX_BOARD_POS], int _x, int _y, int _j, int _i, int *f_blocof,
                    int _jj, int _ii, int f_matrix_jogada[2][NR_MAX_LIGACOES]){

    // caso haja fronteiras sem cor
    if(f_board[_x][_i] == NO_COLOR && f_board[_j][_y] == NO_COLOR && f_board[_jj][_i] == NO_COLOR && f_board[_j][_ii] == NO_COLOR && f_board[_j][_i] != NO_COLOR){
        for(int k = 0; k < NR_MAX_LIGACOES; k++){
            //caso a coordenada da casa esquerda pertencer à matriz jogada
            if(f_matrix_jogada[0][k] == _x && f_matrix_jogada[1][k] == _i){
                for(int kk = 0; kk < NR_MAX_LIGACOES; kk++){
                    //caso a coordenada da casa de cima pertence à matriz jogada
                    if(f_matrix_jogada[0][kk] == _j && f_matrix_jogada[1][kk] == _y){
                        //f_bloco+1 para deixar uma linha de espaço entre os pontos da jogada e os pontos bonus a eliminnar
                        f_board[_j][_i] = NO_COLOR;
                        f_matrix_jogada[0][*f_blocof+1] = _j;
                        f_matrix_jogada[1][*f_blocof+1] = _i;
                        (*f_blocof)++;
                    }
                }
            }
        }
    }
}

/**
* calculos: atualização das estatisticas de jogos
* \param jogadas_totais é o número máximo de jogadas
* \param objetivos_pecas representa os objetivos a alcaçar por cor
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho úmero de linhas do tabuleiro
* \param x_freeze coordenada xx da partícula congelada
* \param y_freeze coordenada yy da partícula congelada
* \param _board matriz 2D com números que representam cores no tabuleiro
* \param _vetorCor vetor com o número de partículas por cor após a jogada
* \param _vetorCor0 vetor com o número de partículas por cor antes da jogada
* \param vetor_jogada representa o vetor com o número de partículas por cor eliminadas na jogadas
* \param _win flag do resultado do jogo
*/
void calculos(int *jogadas_totais, int objetivos_pecas[], int n_tamanho, int m_tamanho, int  x_freeze, int y_freeze,
              int _board[][MAX_BOARD_POS], int _vetorCor[], int _vetorCor0[], int vetor_jogada[], int *_win){

  //cálculo do vetor partículas por cor após a jogada
  particulasCor(n_tamanho, m_tamanho, x_freeze, y_freeze, _board, _vetorCor);

  //Vetor das cores eliminadas da jogada
  for(int i = 0; i < MAX_COLORS; i++){
    vetor_jogada[i] =  _vetorCor0[i] -  _vetorCor[i];
  }
  //Vetor dos objetivos em falta para o ganhar o jogo
  for(int i = 0; i < MAX_COLORS; i++){
    objetivos_pecas[i] = objetivos_pecas[i] - vetor_jogada[i];
    if(objetivos_pecas[i] < 0){
        objetivos_pecas[i] = 0;
    }
  }


  //jogadas restantes
  if(somaVetor(vetor_jogada) != 0){
    *jogadas_totais = *jogadas_totais - 1;
  }
  if(somaVetor(objetivos_pecas) == 0){
    *_win = vitoria;
  }else if(*jogadas_totais == 0){
    *_win = derrota;
  }
}

/**
* particulasCor: cálculo do vetor que contêm o número de partículas de cada cor do jogo
* \param f_n_tamanho número de colunas do tabuleiro
* \param f_m_tamanho número de linhas do tabuleiro
* \param f_x_freeze coordenada xx da partícula congelada
* \param f_y_freeze coordenada yy da partícula congelada
* \param f_board matriz 2D com números que representam cores no tabuleiro
* \param f_vetorCor vetor de partículas por Cor a calcular
*/
void particulasCor(int f_n_tamanho, int f_m_tamanho, int f_xfreeze, int f_yfreeze, int f_board[MAX_BOARD_POS][MAX_BOARD_POS], int f_vetorCor[]){

  //cores do tabuleiro a entrar no vetor
  int cor0 = 0, cor1 = 0, cor2 = 0, cor3 = 0, cor4 = 0, CONGELADA = 0;

  for(int i = 0; i < f_m_tamanho; i++){
    for(int j = 0; j < f_n_tamanho; j++){
        if(i != f_yfreeze || j != f_xfreeze){
            if(f_board[j][i] == 0){
                cor0++;
            }else if(f_board[j][i] == 1){
                cor1++;
            }else if(f_board[j][i] == 2){
                cor2++;
            }else if(f_board[j][i] == 3){
                cor3++;
            }else if(f_board[j][i] == 4){
                cor4++;
            }
        }else{
          CONGELADA++;
        }
    }
  }
  f_vetorCor[0] = cor0;
  f_vetorCor[1] = cor1;
  f_vetorCor[2] = cor2;
  f_vetorCor[3] = cor3;
  f_vetorCor[4] = cor4;
  f_vetorCor[COLOR_FREEZE] = CONGELADA;

}

/**
* somaVetor: cálculo da soma de todos os elementos de um vetor
* \param vetor vetor
*/
int somaVetor(int vetor[]){

  //soma de todos os elementos do vetor => soma
  int soma = 0;
  for(int i = 0; i < MAX_COLORS; i++){
      soma = soma + vetor[i];
  }
  return soma;
}

/**
* backup: copia para um vetor backup e para um inteiro respetivos valores
* \param backupINT inteiro backup
* \param backupVECT vetor backup
* \param mainINT inteiro a guardar
* |param mainVECT vetor a guardar
*/
void backup(int *backupINT, int backupVECT[MAX_COLORS], int mainINT ,int mainVECT[MAX_COLORS]){

  *backupINT = mainINT;

  for(int i = 0; i < MAX_COLORS; i++){

    backupVECT[i] = mainVECT[i];
  }
}

/**
* RenderWIN: faz render uma mensagem de vit'oria :"BENFICA"
* \param _renderer renderer to handle all rendering in a window
* \param _font and _font2 fonts to display the scores
*/
void RenderWIN( SDL_Renderer *_renderer, TTF_Font *_font, TTF_Font *_font2){


  SDL_Color red = {255, 0, 0 };

  SDL_SetRenderDrawColor(_renderer, 220, 220, 220, 255 );
  char njogadas[10] = "BENFICA!!";
  char mensagem[22] = "Ganhou o jogo! ;)";

  SDL_Rect jogadas = {240, 290, 300, 100};
  SDL_RenderFillRect(_renderer, &jogadas);
  RenderText(250, 300, njogadas, _font, &red, _renderer);
  RenderText(295, 365, mensagem, _font2, &red, _renderer);

}

/**
* RenderLOSE: faz render uma mensagem de derrota :"SPORTING"
* \param _renderer renderer to handle all rendering in a window
* \param _font and _font2 fonts to display the scores
*/
void RenderLOSE( SDL_Renderer *_renderer, TTF_Font *_font, TTF_Font *_font2){

  // cor de escrita
  SDL_Color green = {0, 255, 0 };

  SDL_SetRenderDrawColor(_renderer, 220, 220, 220, 255 );
  char njogadas[11] = "SPORTING!!";
  char mensagem[22] = "Nao ganhou o jogo! :(";

  SDL_Rect jogadas = {240, 290, 330, 100};
  SDL_RenderFillRect(_renderer, &jogadas);
  RenderText(250, 300, njogadas, _font, &green, _renderer);
  RenderText(300, 365, mensagem, _font2, &green, _renderer);

}

/**
* RenderSHUFFLE: faz render uma mensagem de realização de embaralhamento no tabuleiro
* \param _renderer renderer to handle all rendering in a window
* \param _font and _font2 fonts to display the scores
*/
void RenderSHUFFLE( SDL_Renderer *_renderer, TTF_Font *_font, TTF_Font *_font2){

   // cor de escrita
  SDL_Color blue = {0, 0, 255 };

  SDL_SetRenderDrawColor(_renderer, 150, 150, 150, 255 );
  char shuffle[10] = "SHUTTLE!!";
  char mensagem[28] = "Nenhuma jogada possivel! :/";

  SDL_Rect jogadas = {240, 290, 300, 100};
  SDL_RenderFillRect(_renderer, &jogadas);
  RenderText(250, 300, shuffle, _font, &blue, _renderer);
  RenderText(295, 365, mensagem, _font2, &blue, _renderer);

}

/**
* Estatisticas: escrita no ficheiro
* \param _nome nome do jogador
* \param _win flag do resultado do jogo
* \param jogadas_totais é o número máximo de jogadas
* \param objetivos_pecas representa os objetivos a alcaçar por cor
* \param _jogos número de jogos iniciados
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
*/
void Estatisticas(char _nome[MAX_NAME_SIZE], int _win, int jogadas_totais, int objetivos_pecas[], int _jogos, int matrix_stats[2][MAX_PLAYS]){

  FILE *file;
  file = fopen(TEXT_FILE, "w");
  int vitorias = 0, derrotas = 0;
  if(file == NULL){
    printf("\nUnable to create the file: "TEXT_FILE"\n");
    exit(EXIT_FAILURE);
  }
  if(_jogos > 0){
      fprintf(file, "%s \t começou o jogo %d vezes e no ...\n", _nome, _jogos-1);
      for(int i = 0; i < _jogos; i++){    //no último jogo o utilizador sai
        if(matrix_stats[0][i] == 1){
          fprintf(file, "jogo %d ganhou, realizando %d jogadas!\n",i, matrix_stats[1][i]);
          vitorias++;
        }else if(matrix_stats[0][i] == 0){
          fprintf(file, "jogo %d perdeu, realizando %d jogadas!",i, matrix_stats[1][i]);
            if(matrix_stats[1][i] == 0){
              fputs( "\tTecla 'N'\n", file);
            }else{
              fputs( "\n", file);
            }
          derrotas++;
        }
      }
    fprintf(file,"\a\n As estatisticas do jogador foram:\n\t %d V\t e\t %d D , resultando num saldo de: \t%d . ", vitorias, derrotas, vitorias-derrotas);
    fprintf(file,"\a\n\n%s\t saiu do jogo\n", _nome);
  }else{
    fprintf(file,"\a%s\t não chegou a iniciar qualquer jogo\n", _nome);
  }
  fclose(file);
}

/**
* saveStats: informação guardada ao longo dos jogos iniciados
* \param jogadas_totais número máximo de jogadas
* \param jogadas_backup número de jogadas iniciais
* \param _jogos número de jogos iniciados
* \param matrix_jogada guarda as coordenas (x,y) das partículas válidas
* \param _win flag do resultado do jogo
*/
void saveStats(int jogadas_totais, int jogadas_backup, int _jogos, int matrixStats[2][MAX_PLAYS], int _win){

    int temp = jogadas_backup - jogadas_totais;

    if(_win == vitoria){
        matrixStats[0][_jogos-1] = 1;
        matrixStats[1][_jogos-1] = temp;
    }else if(_win == derrota || _win == desistencia){
        matrixStats[0][_jogos-1] = 0;
        matrixStats[1][_jogos-1] = temp;
    }
}

/**
* RenderINFO: faz render uma mensagem de informação sobre o jogo
* \param _renderer renderer to handle all rendering in a window
* \param _font font to display the scores
*/
void RenderINFO( SDL_Renderer *_renderer, TTF_Font *_font){

  SDL_Color white = {255, 255, 255 };

  SDL_SetRenderDrawColor(_renderer, 20, 20, 20, 255);
  const char n_key[83] = "PRESSIONA 'N' PARA UM JOGO COM OS PARAMETROS INICIAIS (em qualquer altura do jogo)";
  const char q_key[63] = "PRESSIONA 'Q' PARA SAIRES DO JOGO (em qualquer altura do jogo)";
  SDL_Rect info = {35, 120, 800, 80};
  SDL_RenderFillRect(_renderer, &info);
  RenderText(50, 135, n_key, _font, &white, _renderer);
  RenderText(50, 165, q_key, _font, &white, _renderer);
}

/**
* resetParametros: reinicializa a {0} todos os parametros de jogo
* \param n_tamanho número de colunas do tabuleiro
* \param m_tamanho número de linhas do tabuleiro
* \param nr_cores número de cores no tabuleiro
* \param objetivos_pecas objetivos a alcaçar por cor
* \param jogadas_totais número máximo de jogadas
*/
void resetParametros(int* n_tamanho, int* m_tamanho, int* nr_cores, int objetivos_pecas[], int* jogadas_totais){

    *n_tamanho = 0;
    *m_tamanho = 0;
    *nr_cores = 0;
    for(int i = 0; i < MAX_COLORS; i++){
        objetivos_pecas[i] = 0;
    }
    *jogadas_totais = 0;
}
