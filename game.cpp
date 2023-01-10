#include "game.h"

Game::Game(QGraphicsScene *scene)
{
    board = new MyBoard(scene);
    maxID = 0;
    whitePawnsCounter = 0;
    blackPawnsCounter = 0;

    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if ((i + j) % 2 == 1)
            {
                if (i < BOARD_SIZE / 2 - 1)
                {
                    fields[i][j] = WHITE;
                    addWhitePawn(i, j);
                    ++whitePawnsCounter;
                }
                else if (i > BOARD_SIZE / 2)
                {
                    fields[i][j] = BLACK;
                    addBlackPawn(i, j);
                    ++blackPawnsCounter;
                }
                else
                {
                    fields[i][j] = EMPTY;
                }
            }
            else
            {
                fields[i][j] = BLOCKED;
            }
    isChecked = false;
    whiteTurn = true;
    beatingFlag = false;
    gameOverFlag = false;
}

Game::~Game()
{
    delete board;
}

void Game::squareClicked(int x, int y)
{
    std::pair<int, int> square = board->getSquarePos(x, y);
    // черга білих
    if (whiteTurn)
    {
        // якщо побили білих
        if (beatingFlag)
        {
            if (beat(checked, square))
            {
                lastBeating = 20;
                board->uncheck(checked);
                isChecked = false;
                // перевірка чи можливо побити ще
                if (beatingIsPossible(square))
                {
                    beatingFlag = true;
                    board->check(square);
                    isChecked = true;
                    checked = square;
                }
                else
                {
                    changeTurn();
                    beatingFlag = false;
                    // перевірка чи зміниться на дамку
                    if (square.first == BOARD_SIZE - 1)
                        changeToQueen(square);
                }
                return;
            }
        }
        else
        {
            // поставити прапорець невідмічено
            if (!isChecked && (fields[square.first][square.second] == WHITE || fields[square.first][square.second] == WHITE_QUEEN))
            {
                isChecked = true;
                checked = square;
                board->check(square);
            }
            // знімаємо прапорець
            else if (isChecked && checked == square)
            {
                isChecked = false;
                board->uncheck(square);
            }
            // ставимо інший прапорець
            else if (isChecked && (fields[square.first][square.second] == WHITE || fields[square.first][square.second] == WHITE_QUEEN))
            {
                board->uncheck(checked);
                checked = square;
                board->check(square);
            }
            // намагаємося зробити крок
            else if (isChecked && fields[square.first][square.second] == EMPTY)
            {
                if (move(checked, square))
                {
                    board->uncheck(checked);
                    isChecked = false;
                    changeTurn();
                    // перевірка чи зміниться на дамку
                    if (square.first == BOARD_SIZE - 1)
                        changeToQueen(square);
                    return;
                }
                else if (beat(checked, square))
                {
                    lastBeating = 20;
                    board->uncheck(checked);
                    isChecked = false;
                    // перевірка чи можливо побити ще
                    if (beatingIsPossible(square))
                    {
                        beatingFlag = true;
                        board->check(square);
                        isChecked = true;
                        checked = square;
                    }
                    else
                    {
                        changeTurn();
                        beatingFlag = false;
                        // перевірка чи зміниться на дамку
                        if (square.first == BOARD_SIZE - 1)
                            changeToQueen(square);
                    }
                    return;
                }
            }
        }
    }
    // черга чорних
    else
    {
        if (beatingFlag)
        {
            if (beat(checked, square))
            {
                board->uncheck(checked);
                isChecked = false;
                // перевірка, чи можливе ще одне побиття
                if (beatingIsPossible(square))
                {
                    lastBeating = 20;
                    beatingFlag = true;
                    board->check(square);
                    isChecked = true;
                    checked = square;
                }
                else
                {
                    changeTurn();
                    beatingFlag = false;
                    // перевірка, чи стає дамкою
                    if (square.first == 0)
                        changeToQueen(square);
                }
                return;
            }
        }
        else
        {
            // ставимо прапорець якщо нічого не відмічено
            if (!isChecked && (fields[square.first][square.second] == BLACK || fields[square.first][square.second] == BLACK_QUEEN))
            {
                isChecked = true;
                checked = square;
                board->check(square);
            }
            // знімаємо прапорець
            else if (isChecked && checked == square)
            {
                isChecked = false;
                board->uncheck(square);
            }
            // ставимо інший прапорець
            else if (isChecked && (fields[square.first][square.second] == BLACK || fields[square.first][square.second] == BLACK_QUEEN))
            {
                board->uncheck(checked);
                checked = square;
                board->check(square);
            }
            // намагаємося походити
            else if (isChecked && fields[square.first][square.second] == EMPTY)
            {
                if (move(checked, square))
                {
                    board->uncheck(checked);
                    isChecked = false;
                    changeTurn();
                    if (square.first == 0)
                        changeToQueen(square);
                    return;
                }
                else if (beat(checked, square))
                {
                    lastBeating = 20;
                    board->uncheck(checked);
                    isChecked = false;
                    // перевірка, чи можливе ще одне побиття
                    if (beatingIsPossible(square))
                    {
                        beatingFlag = true;
                        board->check(square);
                        isChecked = true;
                        checked = square;
                    }
                    else
                    {
                        changeTurn();
                        beatingFlag = false;
                        if (square.first == 0)
                            changeToQueen(square);
                    }
                    return;
                }
            }
        }
    }
}

bool Game::moveIsPossible(std::pair<int, int> from)
{
    // перевіряємо ходи для білої фігури
    if (fields[from.first][from.second] == WHITE)
    {
        if ((from.first + 1 < BOARD_SIZE && from.second + 1 < BOARD_SIZE && fields[from.first + 1][from.second + 1] == EMPTY) ||
            (from.first + 1 < BOARD_SIZE && from.second - 1 >= 0 && fields[from.first + 1][from.second - 1] == EMPTY))
        {
            return true;
        }
    }
    // перевіряємо ходи для чорної фігури
    else if (fields[from.first][from.second] == BLACK)
    {
        if ((from.first - 1 >= 0 && from.second + 1 < BOARD_SIZE && fields[from.first - 1][from.second + 1] == EMPTY) ||
            (from.first - 1 >= 0 && from.second - 1 >= 0 && fields[from.first - 1][from.second - 1] == EMPTY))
        {
            return true;
        }
    }
    // дамки
    else if (fields[from.first][from.second] == WHITE_QUEEN || fields[from.first][from.second] == BLACK_QUEEN)
    {
        if ((from.first + 1 < BOARD_SIZE && from.second + 1 < BOARD_SIZE && fields[from.first + 1][from.second + 1] == EMPTY) ||
            (from.first - 1 >= 0 && from.second + 1 < BOARD_SIZE && fields[from.first - 1][from.second + 1] == EMPTY) ||
            (from.first + 1 < BOARD_SIZE && from.second - 1 >= 0 && fields[from.first + 1][from.second - 1] == EMPTY) ||
            (from.first - 1 >= 0 && from.second - 1 >= 0 && fields[from.first - 1][from.second - 1] == EMPTY))
        {
            return true;
        }
    }

    return false;
}

bool Game::beatingIsPossible(std::pair<int, int> from)
{
    // перевіряємо чи можна бити чорною фігурою
    if (fields[from.first][from.second] == BLACK)
    {
        if ((from.first < BOARD_SIZE - 2) && (from.second < BOARD_SIZE - 2))
        {
            if (fields[from.first + 1][from.second + 1] == WHITE && fields[from.first + 2][from.second + 2] == EMPTY)
                return true;
            else if (fields[from.first + 1][from.second + 1] == WHITE_QUEEN && fields[from.first + 2][from.second + 2] == EMPTY)
                return true;
        }
        if ((from.first < BOARD_SIZE - 2) && (from.second > 1))
        {
            if (fields[from.first + 1][from.second - 1] == WHITE && fields[from.first + 2][from.second - 2] == EMPTY)
                return true;
            else if (fields[from.first + 1][from.second - 1] == WHITE_QUEEN && fields[from.first + 2][from.second - 2] == EMPTY)
                return true;
        }
        if ((from.first > 1) && (from.second < BOARD_SIZE - 2))
        {
            if (fields[from.first - 1][from.second + 1] == WHITE && fields[from.first - 2][from.second + 2] == EMPTY)
                return true;
            else if (fields[from.first - 1][from.second + 1] == WHITE_QUEEN && fields[from.first - 2][from.second + 2] == EMPTY)
                return true;
        }
        if ((from.first > 1) && (from.second > 1))
        {
            if (fields[from.first - 1][from.second - 1] == WHITE && fields[from.first - 2][from.second - 2] == EMPTY)
                return true;
            else if (fields[from.first - 1][from.second - 1] == WHITE_QUEEN && fields[from.first - 2][from.second - 2] == EMPTY)
                return true;
        }
        return false;
    }
    // перевіряємо чи можна бити чорною фігурою
    else if (fields[from.first][from.second] == WHITE)
    {
        if ((from.first < BOARD_SIZE - 2) && (from.second < BOARD_SIZE - 2))
        {
            if (fields[from.first + 1][from.second + 1] == BLACK && fields[from.first + 2][from.second + 2] == EMPTY)
                return true;
            else if (fields[from.first + 1][from.second + 1] == BLACK_QUEEN && fields[from.first + 2][from.second + 2] == EMPTY)
                return true;
        }
        if ((from.first < BOARD_SIZE - 2) && (from.second > 1))
        {
            if (fields[from.first + 1][from.second - 1] == BLACK && fields[from.first + 2][from.second - 2] == EMPTY)
                return true;
            else if (fields[from.first + 1][from.second - 1] == BLACK_QUEEN && fields[from.first + 2][from.second - 2] == EMPTY)
                return true;
        }
        if ((from.first > 1) && (from.second < BOARD_SIZE - 2))
        {
            if (fields[from.first - 1][from.second + 1] == BLACK && fields[from.first - 2][from.second + 2] == EMPTY)
                return true;
            else if (fields[from.first - 1][from.second + 1] == BLACK_QUEEN && fields[from.first - 2][from.second + 2] == EMPTY)
                return true;
        }
        if ((from.first > 1) && (from.second > 1))
        {
            if (fields[from.first - 1][from.second - 1] == BLACK && fields[from.first - 2][from.second - 2] == EMPTY)
                return true;
            else if (fields[from.first - 1][from.second - 1] == BLACK_QUEEN && fields[from.first - 2][from.second - 2] == EMPTY)
                return true;
        }
        return false;
    }
    // перевірка для чорної дамки
    else if (fields[from.first][from.second] == BLACK_QUEEN)
    {
        // оглядаємо всі сторони
        int x1, y1;

        // починаючи з найближчих
        x1 = from.first + 1;
        y1 = from.second + 1;
        while (x1 < BOARD_SIZE && y1 < BOARD_SIZE && fields[x1][y1] == EMPTY)
        {
            ++x1;
            ++y1;
        }
        if (x1 < BOARD_SIZE - 1 && y1 < BOARD_SIZE - 1 &&
            (fields[x1][y1] == WHITE || fields[x1][y1] == WHITE_QUEEN) &&
            fields[x1 + 1][y1 + 1] == EMPTY)
        {
            return true;
        }

        // починаючи з дальших
        x1 = from.first - 1;
        y1 = from.second - 1;
        while (x1 >= 0 && y1 >= 0 && fields[x1][y1] == EMPTY)
        {
            --x1;
            --y1;
        }
        if (x1 > 0 && y1 > 0 &&
            (fields[x1][y1] == WHITE || fields[x1][y1] == WHITE_QUEEN) &&
            fields[x1 - 1][y1 - 1] == EMPTY)
        {
            return true;
        }

        // х збільшується, у зменшуються
        x1 = from.first + 1;
        y1 = from.second - 1;
        while (x1 < BOARD_SIZE && y1 >= 0 && fields[x1][y1] == EMPTY)
        {
            ++x1;
            --y1;
        }
        if (x1 < BOARD_SIZE - 1 && y1 > 0 &&
            (fields[x1][y1] == WHITE || fields[x1][y1] == WHITE_QUEEN) &&
            fields[x1 + 1][y1 - 1] == EMPTY)
        {
            return true;
        }

        // х зменшується, у збільшується
        x1 = from.first - 1;
        y1 = from.second + 1;
        while (x1 >= 0 && y1 < BOARD_SIZE && fields[x1][y1] == EMPTY)
        {
            --x1;
            ++y1;
        }
        if (x1 > 0 && y1 < BOARD_SIZE - 1 &&
            (fields[x1][y1] == WHITE || fields[x1][y1] == WHITE_QUEEN) &&
            fields[x1 - 1][y1 + 1] == EMPTY)
        {
            return true;
        }
    }
    // біла дамка
    else if (fields[from.first][from.second] == WHITE_QUEEN)
    {
        // оглядаємо всі сторони
        int x1, y1;

        // починаючи з найближчих
        x1 = from.first + 1;
        y1 = from.second + 1;
        while (x1 < BOARD_SIZE && y1 < BOARD_SIZE && fields[x1][y1] == EMPTY)
        {
            ++x1;
            ++y1;
        }
        if (x1 < BOARD_SIZE - 1 && y1 < BOARD_SIZE - 1 &&
            (fields[x1][y1] == BLACK || fields[x1][y1] == BLACK_QUEEN) &&
            fields[x1 + 1][y1 + 1] == EMPTY)
        {
            return true;
        }

        // починаючи з дальших
        x1 = from.first - 1;
        y1 = from.second - 1;
        while (x1 >= 0 && y1 >= 0 && fields[x1][y1] == EMPTY)
        {
            --x1;
            --y1;
        }
        if (x1 > 0 && y1 > 0 &&
            (fields[x1][y1] == BLACK || fields[x1][y1] == BLACK_QUEEN) &&
            fields[x1 - 1][y1 - 1] == EMPTY)
        {
            return true;
        }

        // х збільшується, у зменшуються
        x1 = from.first + 1;
        y1 = from.second - 1;
        while (x1 < BOARD_SIZE && y1 >= 0 && fields[x1][y1] == EMPTY)
        {
            ++x1;
            --y1;
        }
        if (x1 < BOARD_SIZE - 1 && y1 > 0 &&
            (fields[x1][y1] == BLACK || fields[x1][y1] == BLACK_QUEEN) &&
            fields[x1 + 1][y1 - 1] == EMPTY)
        {
            return true;
        }

        // х зменшується, у збільшується
        x1 = from.first - 1;
        y1 = from.second + 1;
        while (x1 >= 0 && y1 < BOARD_SIZE && fields[x1][y1] == EMPTY)
        {
            --x1;
            ++y1;
        }
        if (x1 > 0 && y1 < BOARD_SIZE - 1 &&
            (fields[x1][y1] == BLACK || fields[x1][y1] == BLACK_QUEEN) &&
            fields[x1 - 1][y1 + 1] == EMPTY)
        {
            return true;
        }
    }
    return false;
}

bool Game::move(std::pair<int, int> from, std::pair<int, int> to)
{
    if (fields[to.first][to.second] != EMPTY)
        return false;
    int x1 = from.first;
    int y1 = from.second;
    int x2 = to.first;
    int y2 = to.second;

    // ход фігури
    if (fields[x1][y1] == BLACK || fields[x1][y1] == WHITE)
    {
        if (abs(x1 - x2) == 1 && abs(y1 - y2) == 1)
        {
            // перевірка чи йде вперед
            if (fields[x1][y1] == BLACK && x1 < x2)
                return false;
            if (fields[x1][y1] == WHITE && x1 > x2)
                return false;
            fields[x2][y2] = fields[x1][y1];
            fields[x1][y1] = EMPTY;
            move(x1, y1, x2, y2);
            board->updateField(from.first, from.second);
            return true;
        }
    }
    // дамка
    else if (fields[x1][y1] == BLACK_QUEEN || fields[x1][y1] == WHITE_QUEEN)
    {
        // перевіра чи йде по діагоналі
        if (abs(x1 - x2) == abs(y1 - y2))
        {
            int xDirection;
            int yDirection;
            if (x1 > x2)
                xDirection = -1;
            else
                xDirection = 1;
            if (y1 > y2)
                yDirection = -1;
            else
                yDirection = 1;
            while (x1 != x2 && y1 != y2)
            {
                x1 += xDirection;
                y1 += yDirection;
                if (fields[x1][y1] != EMPTY)
                    return false;
            }
            x1 = from.first;
            y1 = from.second;
            fields[x2][y2] = fields[x1][y1];
            fields[x1][y1] = EMPTY;
            move(x1, y1, x2, y2);
            board->updateField(from.first, from.second);
            return true;
        }
    }

    return false;
}

bool Game::beat(std::pair<int, int> from, std::pair<int, int> to)
{
    if (fields[to.first][to.second] != EMPTY)
        return false;

    int x1 = from.first;
    int y1 = from.second;
    int x2;
    int y2;
    int x3 = to.first;
    int y3 = to.second;

    // чорна фігура
    if (fields[from.first][from.second] == BLACK)
    {
        x2 = (x1 + x3) / 2;
        y2 = (y1 + y3) / 2;

        if (abs(x1 - x3) == 2 && abs(y1 - y3) == 2 && (fields[x2][y2] == WHITE || fields[x2][y2] == WHITE_QUEEN))
        {
            fields[x3][y3] = fields[x1][y1];
            fields[x1][y1] = EMPTY;
            removePawn(x2, y2);
            move(x1, y1, x3, y3);

            return true;
        }
        return false;
    }
    // біла фігура
    else if (fields[from.first][from.second] == WHITE)
    {
        x2 = (x1 + x3) / 2;
        y2 = (y1 + y3) / 2;

        if (abs(x1 - x3) == 2 && abs(y1 - y3) == 2 && (fields[x2][y2] == BLACK || fields[x2][y2] == BLACK_QUEEN))
        {
            fields[x3][y3] = fields[x1][y1];
            fields[x1][y1] = EMPTY;
            removePawn(x2, y2);
            move(x1, y1, x3, y3);

            return true;
        }
        return false;
    }
    // чорна дамка
    else if (fields[from.first][from.second] == BLACK_QUEEN)
    {
        // чи рухається по діагоналі
        if (abs(x1 - x3) == abs(y1 - y3))
        {
            int xDirection;
            int yDirection;
            int numberOfPawns = 0;

            if (x1 > x3)
                xDirection = -1;
            else
                xDirection = 1;
            if (y1 > y3)
                yDirection = -1;
            else
                yDirection = 1;
            while (x1 != x3 && y1 != y3)
            {
                x1 += xDirection;
                y1 += yDirection;
                if (fields[x1][y1] == WHITE || fields[x1][y1] == WHITE_QUEEN)
                {
                    ++numberOfPawns;
                    x2 = x1;
                    y2 = y1;
                }
                else if (fields[x1][y1] != EMPTY)
                    return false;
            }
            if (numberOfPawns == 1)
            {
                x1 = from.first;
                y1 = from.second;
                fields[x3][y3] = fields[x1][y1];
                fields[x1][y1] = EMPTY;
                removePawn(x2, y2);
                move(x1, y1, x3, y3);

                return true;
            }
            return false;
        }
    }
    // біла дамка
    else if (fields[from.first][from.second] == WHITE_QUEEN)
    {
        // чи рухається по діагоналі
        if (abs(x1 - x3) == abs(y1 - y3))
        {
            int xDirection;
            int yDirection;
            int numberOfPawns = 0;

            if (x1 > x3)
                xDirection = -1;
            else
                xDirection = 1;
            if (y1 > y3)
                yDirection = -1;
            else
                yDirection = 1;
            while (x1 != x3 && y1 != y3)
            {
                x1 += xDirection;
                y1 += yDirection;
                if (fields[x1][y1] == BLACK || fields[x1][y1] == BLACK_QUEEN)
                {
                    ++numberOfPawns;
                    x2 = x1;
                    y2 = y1;
                }
                else if (fields[x1][y1] != EMPTY)
                    return false;
            }
            if (numberOfPawns == 1)
            {
                x1 = from.first;
                y1 = from.second;
                fields[x3][y3] = fields[x1][y1];
                fields[x1][y1] = EMPTY;
                removePawn(x2, y2);
                move(x1, y1, x3, y3);

                return true;
            }
            return false;
        }
    }
    return false;
}

void Game::move(int x1, int y1, int x2, int y2)
{
    int id = board->getPawnID(x1, y1);
    board->move(x2, y2, id);
}

void Game::addBlackPawn(int x, int y)
{
    board->addBlackPawn(x, y, maxID);
    pawns.push_back(maxID);
    ++maxID;
}

void Game::addWhitePawn(int x, int y)
{
    board->addWhitePawn(x, y, maxID);
    pawns.push_back(maxID);
    ++maxID;
}

void Game::changeToQueen(std::pair<int, int> p)
{
    board->changeToQueen(p.first, p.second);
    if (fields[p.first][p.second] == BLACK)
        fields[p.first][p.second] = BLACK_QUEEN;
    else
        fields[p.first][p.second] = WHITE_QUEEN;
}

void Game::removePawn(int x, int y)
{
    int id = board->getPawnID(x, y);
    for (std::vector<int>::iterator it = pawns.begin(); it != pawns.end(); ++it)
    {
        if (*it == id)
        {
            pawns.erase(it);
            if (fields[x][y] == BLACK || fields[x][y] == BLACK_QUEEN)
                --blackPawnsCounter;
            else if (fields[x][y] == WHITE || fields[x][y] == WHITE_QUEEN)
                --whitePawnsCounter;
            fields[x][y] = EMPTY;
            break;
        }
    }
    board->removePawn(id);
}

void Game::changeTurn()
{
    --lastMove;
    --lastBeating;
    whiteTurn = !whiteTurn;
    if (isGameOver())
    {
        gameOverFlag = true;
    }
}

bool Game::isGameOver()
{
    // перевірка чи сутація на дошці не являється патовою
    if (lastMove == 0)
    {
        return true;
    }
    if (lastBeating == 0)
        return true;
    if (whiteTurn)
    {
        if (whitePawnsCounter == 0)
            return true;
    }
    else
    {
        if (blackPawnsCounter == 0)
            return true;
    }

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (whiteTurn)
            {
                if (fields[i][j] == WHITE || fields[i][j] == WHITE_QUEEN)
                {
                    if (moveIsPossible(std::make_pair(i, j)))
                    {
                        return false;
                    }
                    else if (beatingIsPossible(std::make_pair(i, j)))
                        return false;
                }
            }
            else
            {
                if (fields[i][j] == BLACK || fields[i][j] == BLACK_QUEEN)
                {
                    if (moveIsPossible(std::make_pair(i, j)))
                        return false;
                    else if (beatingIsPossible(std::make_pair(i, j)))
                        return false;
                }
            }
        }
    return true;
}

bool Game::getGameOverFlag()
{
    return gameOverFlag;
}
