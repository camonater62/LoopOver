#include <format>

#include "olcPixelGameEngine.h"

class LoopOver : public olc::PixelGameEngine
{
public:
    LoopOver()
    {
        sAppName = "LoopOver";
    }

private:
    static constexpr int GRID_HEIGHT = 5;
    static constexpr int GRID_WIDTH = 5;
    char grid[GRID_HEIGHT][GRID_WIDTH];
    olc::vi2d lastMousePos;
    float timer = 0.0f;
    bool running = false;

public:
    bool OnUserCreate() override
    {
        char current = 'A';

        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            for (int x = 0; x < GRID_WIDTH; x++)
            {
                grid[y][x] = current;
                current++;
            }
        }

        lastMousePos = {-1, -1};

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetKey(olc::SPACE).bReleased) {
            Shuffle();
            running = false;
            timer = 0;
        }

        if (running && isComplete())
        {
            sAppName = std::format("Finished! {:2f}s", timer);
            running = false;
        }
        else if(running)
        {
            sAppName = std::format("{:2f}s", timer);
            timer += fElapsedTime;
        }

        if (GetMouse(olc::Mouse::LEFT).bPressed)
        {
            olc::vi2d currentPos = GetMousePos();
            currentPos *= olc::vi2d{GRID_WIDTH, GRID_HEIGHT};
            currentPos /= GetScreenSize();
            lastMousePos = currentPos;
        }

        if (GetMouse(olc::Mouse::LEFT).bHeld)
        {
            olc::vi2d currentPos = GetMousePos();
            currentPos *= olc::vi2d{GRID_WIDTH, GRID_HEIGHT};
            currentPos /= GetScreenSize();
            olc::vi2d delta = currentPos - lastMousePos;
            if (lastMousePos == olc::vi2d{-1, -1})
            {
                delta = {0, 0};
            }

            if (delta != olc::vi2d{0, 0})
            {
                // Start timer
                if (!running) {
                    running = true;
                    timer = 0;
                }
                
                if (delta.y < 0)
                {
                    RotateUp(currentPos.x);
                }
                if (delta.y > 0)
                {
                    RotateDown(currentPos.x);
                }
                if (delta.x < 0)
                {
                    RotateLeft(currentPos.y);
                }
                if (delta.x > 0)
                {
                    RotateRight(currentPos.y);
                }
            }

            lastMousePos = currentPos;
        }

        const olc::vf2d blockSize = {
            float(ScreenWidth()) / GRID_WIDTH,
            float(ScreenHeight()) / GRID_HEIGHT};
        const olc::vf2d halfBlockSize = blockSize / 2;

        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            for (int x = 0; x < GRID_WIDTH; x++)
            {
                olc::vf2d pos = {
                    x * blockSize.x,
                    y * blockSize.y};

                int cv = grid[y][x] - 'A';
                int cx = cv % GRID_WIDTH;
                int cy = cv / GRID_HEIGHT;

                float r = 255.0f * float(GRID_WIDTH - cx - 1) / GRID_WIDTH;
                float g = 255.0f * float(cy) / GRID_HEIGHT;
                float b = 255.0f * float(cx) / GRID_WIDTH;
                olc::Pixel color(r, g, b);

                FillRect(pos, blockSize, color);
                DrawString(pos + halfBlockSize, std::format("{}", grid[y][x]));
            }
        }

        return true;
    }

    bool OnUserDestroy() override
    {

        return true;
    }

private:
    void Shuffle()
    {
        for (int i = 0; i < 1000; i++)
        {
            int move = rand() % 4;
            int value = rand() % GRID_WIDTH;

            switch (move)
            {
            case 0:
                RotateUp(value);
            case 1:
                RotateDown(value);
            case 2:
                RotateLeft(value);
            case 3:
                RotateRight(value);

            default:
                break;
            }
        }
    }

    bool isComplete()
    {
        char c = 'A';

        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            for (int x = 0; x < GRID_WIDTH; x++)
            {
                if (grid[y][x] != c)
                    return false;
                c++;
            }
        }
        return true;
    }

    void RotateLeft(int row)
    {
        char end = grid[row][0];

        for (int i = 0; i < GRID_WIDTH - 1; i++)
        {
            grid[row][i] = grid[row][i + 1];
        }

        grid[row][GRID_WIDTH - 1] = end;
    }

    void RotateRight(int row)
    {
        char start = grid[row][GRID_WIDTH - 1];

        for (int i = GRID_WIDTH - 1; i > 0; i--)
        {
            grid[row][i] = grid[row][i - 1];
        }

        grid[row][0] = start;
    }

    void RotateDown(int col)
    {
        char start = grid[GRID_HEIGHT - 1][col];

        for (int i = GRID_HEIGHT - 1; i > 0; i--)
        {
            grid[i][col] = grid[i - 1][col];
        }

        grid[0][col] = start;
    }

    void RotateUp(int col)
    {
        char start = grid[0][col];

        for (int i = 0; i < GRID_HEIGHT - 1; i++)
        {
            grid[i][col] = grid[i + 1][col];
        }

        grid[GRID_HEIGHT - 1][col] = start;
    }
};

int main()
{
    srand(time(NULL));
    LoopOver game;
    if (game.Construct(240, 240, 2, 2, false, true))
    {
        game.Start();
    }

    return 0;
}