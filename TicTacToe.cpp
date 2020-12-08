#include <Windows.h>
#include <time.h>
#include <stdio.h>

struct LCGrand
{
    unsigned long long seed;

    unsigned long long a;
    unsigned long long m;

    unsigned long long q;
    unsigned long long r;

    unsigned long long t;
};

struct LCGrand get_seed(long long);
unsigned long get_random(struct LCGrand*, int);

struct LCGrand get_seed(long long time)
{
    struct LCGrand rand;

    rand.seed = 2147483645;
    rand.a = 16807;
    rand.m = 2147483647;
    rand.q = 127773;
    rand.r = 2836;
    rand.t = time % 1000;

    return rand;
}

unsigned long get_random(struct LCGrand* rand, int max)
{
    unsigned long long k = 0;

    k = rand->seed / rand->q;
    rand->seed = rand->a * (rand->seed - k * rand->q) - rand->r * k;

    if (rand->seed < 0)
    {
        rand->seed += rand->m;
    }

    return (rand->seed / (rand->t + 1)) % max;
}

int string_length(const wchar_t* str)
{
    const wchar_t* str_temp = str;
    int len = 0;

    while (*str_temp++ != L'\0')
    {
        len++;
    }
    return len;
}


void wstring_copy(wchar_t* source, const wchar_t* str)
{
    for (int i = 0; i < string_length(str) + 1; i++)
    {
        source[i] = str[i];
    }
}

int main()
{
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    SetConsoleTitle(TEXT("TicTacToe Game"));
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
    cfi.dwFontSize.X = 34;
    cfi.dwFontSize.Y = 72;
    wstring_copy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(console, FALSE, &cfi);
    // SetConsoleDisplayMode(console, CONSOLE_FULLSCREEN_MODE, 0);
    ::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
    HANDLE handle_input;
    DWORD prev_mode;
    handle_input = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(handle_input, &prev_mode);
    SetConsoleMode(handle_input, prev_mode & ENABLE_EXTENDED_FLAGS);
    DWORD dwBytesWritten = 0;
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);
    COORD window_size = GetLargestConsoleWindowSize(console);
    SHORT WIDTH = window_size.X;
    SHORT HEIGHT = window_size.Y;
    SetConsoleScreenBufferSize(console, {WIDTH, HEIGHT - 1});
    LCGrand random = get_seed(time(NULL));
    POINT p;
    int human_score = 0, bot_score = 0;
    char* screen = (char*) calloc(HEIGHT * WIDTH, sizeof(char));

    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        screen[i] = ' ';
    }

    bool clicked = false, esc_key = false;
    sprintf(&screen[(HEIGHT / 2 - 1) * WIDTH + (WIDTH - 23) / 2], "WELCOME TO TIC-TAC-TOE");
    sprintf(&screen[(HEIGHT / 2) * WIDTH + (WIDTH - 30) / 2], "PRESS SPACE TO START THE GAME");
    WriteConsoleOutputCharacter(console, screen, WIDTH * HEIGHT, { 0, 0 }, &dwBytesWritten);


    while (!(GetKeyState(0x20) & 0x8000)) {
        esc_key = (GetKeyState(0x1B) & 0x8000);

        if (esc_key)
        {
            SetConsoleMode(handle_input, prev_mode);
            exit(0);
        }
    }

    while (true)
    {
        sprintf(&screen[(WIDTH - 37) / 2], "YOUR SCORE: %d       KIM BOT's SCORE: %d", human_score, bot_score);
        sprintf(&screen[(HEIGHT / 2 - 1) * WIDTH + (WIDTH - 23) / 2], "                      ");
        sprintf(&screen[(HEIGHT / 2) * WIDTH + (WIDTH - 30) / 2], "                             ");
        sprintf(&screen[((HEIGHT - 9) / 2 + 1) * WIDTH + (WIDTH - 17) / 2], "     |     |     ");
        sprintf(&screen[((HEIGHT - 9) / 2 + 2) * WIDTH + (WIDTH - 17) / 2], "     |     |     ");
        sprintf(&screen[((HEIGHT - 9) / 2 + 3) * WIDTH + (WIDTH - 17) / 2], "_____|_____|_____");
        sprintf(&screen[((HEIGHT - 9) / 2 + 4) * WIDTH + (WIDTH - 17) / 2], "     |     |     ");
        sprintf(&screen[((HEIGHT - 9) / 2 + 5) * WIDTH + (WIDTH - 17) / 2], "     |     |     ");
        sprintf(&screen[((HEIGHT - 9) / 2 + 6) * WIDTH + (WIDTH - 17) / 2], "_____|_____|_____");
        sprintf(&screen[((HEIGHT - 9) / 2 + 7) * WIDTH + (WIDTH - 17) / 2], "     |     |     ");
        sprintf(&screen[((HEIGHT - 9) / 2 + 8) * WIDTH + (WIDTH - 17) / 2], "     |     |     ");
        sprintf(&screen[((HEIGHT - 9) / 2 + 9) * WIDTH + (WIDTH - 17) / 2], "     |     |     ");

        WriteConsoleOutputCharacter(console, screen, WIDTH * HEIGHT, { 0, 0 }, &dwBytesWritten);

        bool game_over = false, won = false, draw = false;
        bool moved = false;
        int moves = 0;
        char matrix[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
        bool occupied[9] = { false, false, false, false, false, false, false, false, false };

        while (!game_over)
        {
            esc_key = (GetKeyState(0x1B) & 0x8000);
            clicked = (GetKeyState(0x01) & 0x8000);
            moved = false;

            if (esc_key)
            {
                SetConsoleMode(handle_input, prev_mode);
                exit(0);
            }

            GetCursorPos(&p);

            if (clicked)
            {
                int x = p.x / 36;
                int y = p.y / 72;

                if ((x >= (WIDTH - 17) / 2) && (x <= (WIDTH - 17) / 2 + 3))
                {
                    if ((y >= (HEIGHT - 9) / 2 + 1) && (y <= (HEIGHT - 9) / 2 + 3))
                    {
                        if (!occupied[0])
                        {
                            screen[((HEIGHT - 9) / 2 + 2) * WIDTH + (WIDTH - 17) / 2 + 2] = L'O';
                            occupied[0] = true;
                            matrix[0] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                    else if ((y >= (HEIGHT - 9) / 2 + 4) && (y <= (HEIGHT - 9) / 2 + 6))
                    {
                        if (!occupied[1])
                        {
                            screen[((HEIGHT - 9) / 2 + 5) * WIDTH + (WIDTH - 17) / 2 + 2] = 'O';
                            occupied[1] = true;
                            matrix[1] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                    else if ((y >= (HEIGHT - 9) / 2 + 7) && (y <= (HEIGHT - 9) / 2 + 9))
                    {
                        if (!occupied[2])
                        {
                            screen[((HEIGHT - 9) / 2 + 8) * WIDTH + (WIDTH - 17) / 2 + 2] = 'O';
                            occupied[2] = true;
                            matrix[2] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                }
                else if ((x >= (WIDTH - 17) / 2 + 6) && (x <= (WIDTH - 17) / 2 + 9))
                {
                    if ((y >= (HEIGHT - 9) / 2 + 1) && (y <= (HEIGHT - 9) / 2 + 3))
                    {
                        if (!occupied[3])
                        {
                            screen[((HEIGHT - 9) / 2 + 2) * WIDTH + (WIDTH - 17) / 2 + 8] = 'O';
                            occupied[3] = true;
                            matrix[3] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                    else if ((y >= (HEIGHT - 9) / 2 + 4) && (y <= (HEIGHT - 9) / 2 + 6))
                    {
                        if (!occupied[4])
                        {
                            screen[((HEIGHT - 9) / 2 + 5) * WIDTH + (WIDTH - 17) / 2 + 8] = 'O';
                            occupied[4] = true;
                            matrix[4] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                    else if ((y >= (HEIGHT - 9) / 2 + 7) && (y <= (HEIGHT - 9) / 2 + 9))
                    {
                        if (!occupied[5])
                        {
                            screen[((HEIGHT - 9) / 2 + 8) * WIDTH + (WIDTH - 17) / 2 + 8] = 'O';
                            occupied[5] = true;
                            matrix[5] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                }
                else if ((x >= (WIDTH - 17) / 2 + 10) && (x <= (WIDTH - 17) / 2 + 15))
                {
                    if ((y >= (HEIGHT - 9) / 2 + 1) && (y <= (HEIGHT - 9) / 2 + 3))
                    {
                        if (!occupied[6])
                        {
                            screen[((HEIGHT - 9) / 2 + 2) * WIDTH + (WIDTH - 17) / 2 + 14] = 'O';
                            occupied[6] = true;
                            matrix[6] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                    else if ((y >= (HEIGHT - 9) / 2 + 4) && (y <= (HEIGHT - 9) / 2 + 6))
                    {
                        if (!occupied[7])
                        {
                            screen[((HEIGHT - 9) / 2 + 5) * WIDTH + (WIDTH - 17) / 2 + 14] = 'O';
                            occupied[7] = true;
                            matrix[7] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                    else if ((y >= (HEIGHT - 9) / 2 + 7) && (y <= (HEIGHT - 9) / 2 + 9))
                    {
                        if (!occupied[8])
                        {
                            screen[((HEIGHT - 9) / 2 + 8) * WIDTH + (WIDTH - 17) / 2 + 14] = 'O';
                            occupied[8] = true;
                            matrix[8] = 'O';
                            moved = true;
                            moves++;
                        }
                    }
                }
            }

            if (matrix[0] == matrix[1] && matrix[1] == matrix[2] && matrix[0] == 'O')
            {
                won = true;
                human_score++;
                game_over = true;
            }
            else if (matrix[3] == matrix[4] && matrix[4] == matrix[5] && matrix[3] == 'O')
            {
                won = true;
                human_score++;
                game_over = true;
            }
            else if (matrix[6] == matrix[7] && matrix[7] == matrix[8] && matrix[6] == 'O')
            {
                won = true;
                human_score++;
                game_over = true;
            }
            else if (matrix[0] == matrix[3] && matrix[3] == matrix[6] && matrix[0] == 'O')
            {
                won = true;
                human_score++;
                game_over = true;
            }
            else if (matrix[1] == matrix[4] && matrix[4] == matrix[7] && matrix[1] == 'O')
            {

                won = true;
                human_score++;
                game_over = true;
            }
            else if (matrix[2] == matrix[5] && matrix[5] == matrix[8] && matrix[2] == 'O')
            {
                won = true;
                human_score++;
                game_over = true;
            }
            else if (matrix[0] == matrix[4] && matrix[4] == matrix[8] && matrix[0] == 'O')
            {
                won = true;
                human_score++;
                game_over = true;
            }
            else if (matrix[6] == matrix[4] && matrix[4] == matrix[2] && matrix[6] == 'O')
            {
                won = true;
                human_score++;
                game_over = true;
            }

            WriteConsoleOutputCharacter(console, screen, WIDTH* HEIGHT, { 0, 0 }, & dwBytesWritten);

            if (game_over)
            {
                break;
            }

            if (moves == 9)
            {
                draw = true;
                break;
            }


            if (moved)
            {
                clock_t start_time = clock();
                while (clock() < start_time + 100);
                moved = false;

                int bot_move = get_random(&random, 8);

                while (occupied[bot_move])
                {
                    bot_move++;
                    if (bot_move == 9)
                    {
                        bot_move = 0;
                    }
                }

                if (bot_move == 0)
                {
                    screen[((HEIGHT - 9) / 2 + 2) * WIDTH + (WIDTH - 17) / 2 + 2] = 'X';
                    occupied[0] = true;
                    matrix[0] = 'X';
                    moves++;
                }
                else if (bot_move == 1)
                {
                    screen[((HEIGHT - 9) / 2 + 5) * WIDTH + (WIDTH - 17) / 2 + 2] = 'X';
                    occupied[1] = true;
                    matrix[1] = 'X';
                    moves++;
                }
                else if (bot_move == 2)
                {
                    screen[((HEIGHT - 9) / 2 + 8) * WIDTH + (WIDTH - 17) / 2 + 2] = 'X';
                    occupied[2] = true;
                    matrix[2] = 'X';
                    moves++;
                }
                else if (bot_move == 3)
                {
                    screen[((HEIGHT - 9) / 2 + 2) * WIDTH + (WIDTH - 17) / 2 + 8] = 'X';
                    occupied[3] = true;
                    matrix[3] = 'X';
                    moves++;
                }
                else if (bot_move == 4)
                {
                    screen[((HEIGHT - 9) / 2 + 5) * WIDTH + (WIDTH - 17) / 2 + 8] = 'X';
                    occupied[4] = true;
                    matrix[4] = 'X';
                    moves++;
                }
                else if (bot_move == 5)
                {
                    screen[((HEIGHT - 9) / 2 + 8) * WIDTH + (WIDTH - 17) / 2 + 8] = 'X';
                    occupied[5] = true;
                    matrix[5] = 'X';
                    moves++;
                }
                else if (bot_move == 6)
                {
                    screen[((HEIGHT - 9) / 2 + 2) * WIDTH + (WIDTH - 17) / 2 + 14] = 'X';
                    occupied[6] = true;
                    matrix[6] = 'X';
                    moves++;
                }
                else if (bot_move == 7)
                {
                    screen[((HEIGHT - 9) / 2 + 5) * WIDTH + (WIDTH - 17) / 2 + 14] = 'X';
                    occupied[7] = true;
                    matrix[7] = 'X';
                    moves++;
                }
                else if (bot_move == 8)
                {
                    screen[((HEIGHT - 9) / 2 + 8) * WIDTH + (WIDTH - 17) / 2 + 14] = 'X';
                    occupied[8] = true;
                    matrix[8] = 'X';
                    moves++;
                }

                WriteConsoleOutputCharacter(console, screen, WIDTH* HEIGHT, { 0, 0 }, & dwBytesWritten);

                if (matrix[0] == matrix[1] && matrix[1] == matrix[2] && matrix[0] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
                else if (matrix[3] == matrix[4] && matrix[4] == matrix[5] && matrix[3] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
                else if (matrix[6] == matrix[7] && matrix[7] == matrix[8] && matrix[6] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
                else if (matrix[0] == matrix[3] && matrix[3] == matrix[6] && matrix[0] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
                else if (matrix[1] == matrix[4] && matrix[4] == matrix[7] && matrix[1] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
                else if (matrix[2] == matrix[5] && matrix[5] == matrix[8] && matrix[2] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
                else if (matrix[0] == matrix[4] && matrix[4] == matrix[8] && matrix[0] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
                else if (matrix[6] == matrix[4] && matrix[4] == matrix[2] && matrix[6] == 'X')
                {
                    won = false;
                    bot_score++;
                    game_over = true;
                }
            }
        }

        clock_t start_time = clock();
        while (clock() < start_time + 500);

        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            screen[i] = L' ';
        }

        sprintf(&screen[(HEIGHT / 2 - 1) * WIDTH + (WIDTH - 10) / 2], (draw) ? "IT'S DRAW" : (won) ? "YOU WON " : "YOU LOST");
        sprintf(&screen[(HEIGHT / 2) * WIDTH + (WIDTH - 26) / 2], "PRESS SPACE TO PLAY AGAIN");
        WriteConsoleOutputCharacter(console, screen, WIDTH* HEIGHT, { 0, 0 }, & dwBytesWritten);

        while (!(0x8000 & GetKeyState(0x20)))
        {
            esc_key = (GetKeyState(0x1B) & 0x8000);

            if (esc_key)
            {
                SetConsoleMode(handle_input, prev_mode);
                exit(0);
            }
        }
    }
}
