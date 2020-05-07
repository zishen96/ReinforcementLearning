#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdlib.h>
#include <functional>

//Grid base
const int row = 6;
const int col = 7;
std::string grid[row][col];                 //Grid to keep track of bounds/obstacles/valid grid spaces
std::string countGrid[row * 3][col * 3];    //N(s,a)
std::string qGrid[row * 3][col * 3];        //Q(s,a)
std::string spaceHolder = "    ";           //String spacing for display
std::string validSpace = "|";
std::string goalReward = "+100";
std::string trapReward = "-100";

int scaleGrid(int i)
{
    if (i == 0) return 1;
    else return i * 3 + 1;
}

void addObstacle(int row, int col)
{
    grid[row][col] = "####";
    countGrid[scaleGrid(row)][scaleGrid(col)] = "####";
    qGrid[scaleGrid(row)][scaleGrid(col)] = "####";
}

float roundTwoDec(float var)
{
    float val = (int)(var * 100 + 0.5);
    return (float)val / 100;
}

bool floatEquals(float a, float b)
{
    //Function to prevent floating point comparison using "==" to be inaccurate due to "fuzziness"
    float epsilon = 0.0000001;
    //For practical purposes, differences smaller than epsilon would be considered "equal"
    return std::abs(a - b) < epsilon;
}

void draw()
{
    std::cout << "N(s,a):\n";
    for (int i = 0; i < row * 3; i++)
    {
        for (int j = 0; j < col * 3; j++)
        {
            std::cout << std::setw(7) << countGrid[i][j];
        }
        std::cout << "\n\n";
    }
    std::cout << "\n\n";

    std::cout << "Q(s,a):\n";
    for (int i = 0; i < row * 3; i++)
    {
        for (int j = 0; j < col * 3; j++)
        {
            if (qGrid[i][j] != spaceHolder && qGrid[i][j] != validSpace && qGrid[i][j] != "####")
            {
                if (qGrid[i][j] == "+100" || qGrid[i][j] == "-100")
                {
                    std::cout << std::setw(7) << qGrid[i][j];
                }
                else
                {
                    std::cout << std::setw(7) << roundTwoDec(std::stof(qGrid[i][j]));
                }
            }
            else
            {
                std::cout << std::setw(7) << qGrid[i][j];
            }
        }
        std::cout << "\n\n";
    }
    std::cout << "\n\n";

    //Draw grid showing optimal actions selecting the max Q-val
    std::cout << "Optimal action:\n";
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (grid[i][j] != "####" && grid[i][j] != "+100" && grid[i][j] != "-100" && grid[i][j] != spaceHolder)
            {
                float maxQVal = 0.0;
                float wQVal = 0.0, nQVal = 0.0, eQVal = 0.0, sQVal = 0.0;
                //Check the maxQVal of valid grid space
                int r = scaleGrid(i);
                int s = scaleGrid(j);
                wQVal = std::stof(qGrid[r][s - 1]); //West Q
                nQVal = std::stof(qGrid[r - 1][s]); //North Q
                eQVal = std::stof(qGrid[r][s + 1]); //East Q
                sQVal = std::stof(qGrid[r + 1][s]); //South Q

                maxQVal = std::max(wQVal, std::max(nQVal, std::max(eQVal, sQVal)));

                if (floatEquals(maxQVal, wQVal)) std::cout << std::setw(7) << "<<<<";
                else if (floatEquals(maxQVal, nQVal)) std::cout << std::setw(7) << "^^^^";
                else if (floatEquals(maxQVal, eQVal)) std::cout << std::setw(7) << ">>>>";
                else if (floatEquals(maxQVal, sQVal)) std::cout << std::setw(7) << "vvvv";
            }
            else
            {
                std::cout << std::setw(7) << grid[i][j];
            }
        }
        std::cout << "\n\n";
    }
    std::cout << "\n\n";
}

void initGrids()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (grid[i][j] != "####")
            {
                countGrid[scaleGrid(i)][scaleGrid(j)] = validSpace;
                qGrid[scaleGrid(i)][scaleGrid(j)] = validSpace;

                //West: Col-1
                countGrid[scaleGrid(i)][scaleGrid(j)-1] = std::to_string(0);
                qGrid[scaleGrid(i)][scaleGrid(j)-1] = std::to_string(0);
                //North: Row-1
                countGrid[scaleGrid(i)-1][scaleGrid(j)] = std::to_string(0);
                qGrid[scaleGrid(i)-1][scaleGrid(j)] = std::to_string(0);
                //East: Col+1
                countGrid[scaleGrid(i)][scaleGrid(j)+1] = std::to_string(0);
                qGrid[scaleGrid(i)][scaleGrid(j)+1] = std::to_string(0);
                //South: Row+1
                countGrid[scaleGrid(i)+1][scaleGrid(j)] = std::to_string(0);
                qGrid[scaleGrid(i)+1][scaleGrid(j)] = std::to_string(0);
            }

            if (grid[i][j] == "####")
            {
                //West: Col-1
                countGrid[scaleGrid(i)][scaleGrid(j) - 1] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j) - 1] = spaceHolder;
                //North: Row-1
                countGrid[scaleGrid(i) - 1][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i) - 1][scaleGrid(j)] = spaceHolder;
                //East: Col+1
                countGrid[scaleGrid(i)][scaleGrid(j) + 1] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j) + 1] = spaceHolder;
                //South: Row+1
                countGrid[scaleGrid(i) + 1][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i) + 1][scaleGrid(j)] = spaceHolder;
            }
            
            //Empty strings into "   " spaces to align spacing for display
            //Diagonals are dead spaces
            //NorthWest: Row-1, Col-1
            countGrid[scaleGrid(i) - 1][scaleGrid(j) - 1] = spaceHolder;
            qGrid[scaleGrid(i) - 1][scaleGrid(j) - 1] = spaceHolder;
            //NorthEast: Row-1, Col+1
            countGrid[scaleGrid(i) - 1][scaleGrid(j) + 1] = spaceHolder;
            qGrid[scaleGrid(i) - 1][scaleGrid(j) + 1] = spaceHolder;
            //SouthEast: Row+1, Col+1
            countGrid[scaleGrid(i) + 1][scaleGrid(j) + 1] = spaceHolder;
            qGrid[scaleGrid(i) + 1][scaleGrid(j) + 1] = spaceHolder;
            //SouthWest: Row+1, Col-1
            countGrid[scaleGrid(i) + 1][scaleGrid(j) - 1] = spaceHolder;
            qGrid[scaleGrid(i) + 1][scaleGrid(j) - 1] = spaceHolder;

            //Goal and initial grids
            if (i == 0 && j == 6)
            {
                countGrid[scaleGrid(i)][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j)] = trapReward;
                grid[i][j] = trapReward;

                //West: Col-1
                countGrid[scaleGrid(i)][scaleGrid(j) - 1] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j) - 1] = spaceHolder;
                //North: Row-1
                countGrid[scaleGrid(i) - 1][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i) - 1][scaleGrid(j)] = spaceHolder;
                //East: Col+1
                countGrid[scaleGrid(i)][scaleGrid(j) + 1] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j) + 1] = spaceHolder;
                //South: Row+1
                countGrid[scaleGrid(i) + 1][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i) + 1][scaleGrid(j)] = spaceHolder;
            }
            if (i == 5 && j == 0)
            {
                countGrid[scaleGrid(i)][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j)] = goalReward;
                grid[i][j] = goalReward;

                //West: Col-1
                countGrid[scaleGrid(i)][scaleGrid(j) - 1] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j) - 1] = spaceHolder;
                //North: Row-1
                countGrid[scaleGrid(i) - 1][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i) - 1][scaleGrid(j)] = spaceHolder;
                //East: Col+1
                countGrid[scaleGrid(i)][scaleGrid(j) + 1] = spaceHolder;
                qGrid[scaleGrid(i)][scaleGrid(j) + 1] = spaceHolder;
                //South: Row+1
                countGrid[scaleGrid(i) + 1][scaleGrid(j)] = spaceHolder;
                qGrid[scaleGrid(i) + 1][scaleGrid(j)] = spaceHolder;
            }
        }
    }
}

int rng(int max)
{
    //Generate a number from 1 to max input
    return rand() % max + 1;
}

int maxQSelect(float west, float north, float east, float south)
{
    int action = 0;
    bool wAction = false, nAction = false, eAction = false, sAction = false;
    float maxQVal = 0;    
    int maxQCount = 0;  //If there are equal max Q, keep track of equally optimal choices
    maxQVal = std::max(west, std::max(north, std::max(east, south)));
    float qVal[4] = { west, north, east, south };

    for (int arrI = 0; arrI < 4; arrI++)
    {
        if (floatEquals(maxQVal, qVal[arrI]))
        {
            //Current direction is maxQVal
            maxQCount++;
            if (arrI == 0) wAction = true;
            else if (arrI == 1) nAction = true;
            else if (arrI == 2) eAction = true;
            else if (arrI == 3) sAction = true;
        }
    }

    //Action determination
    if (maxQCount == 1)
    {
        //Solo maxQVal action selected
        if (wAction == true) action = 1;
        else if (nAction == true) action = 2;
        else if (eAction == true) action = 3;
        else if (sAction == true) action = 4;
    }
    else if (maxQCount == 4)
    {
        action = rng(4);    //1 of 4 action selected
    }
    else
    {
        //from 2 or 3 specific actions to choose
        //Create rng buckets for each maxQVal action
        int bucket = rng(maxQCount * 100);
        //std::cout << "Bucket rng: " << bucket << " | ";
        
        //Each block of 100 numbers assigned to 1 action
        if (maxQCount == 2)
        {
            if (bucket <= 100)
            {
                if (wAction == true) action = 1;
                else if (wAction != true && nAction == true) action = 2;
                else if (wAction != true && nAction != true) action = 3;
            }
            else if (bucket > 100)
            {
                if (wAction == true)
                {
                    if (nAction == true) action = 2;
                    else if (eAction == true) action = 3;
                    else if (sAction == true) action = 4;
                }
                else if (nAction == true)
                {
                    if (eAction == true) action = 3;
                    else if (sAction == true) action = 4;
                }
                else
                {
                    action = 4;
                }
            }
        }
        else if (maxQCount == 3)
        {
            if (bucket <= 100)
            {
                if (wAction == true) action = 1;
                else if (wAction != true && nAction == true) action = 2;
            }
            else if (bucket > 100 && bucket <= 200)
            {
                if (wAction == true)
                {
                    if (nAction == true) action = 2;
                    else if (nAction != true && eAction == true) action = 3;
                }
                else if (wAction != true) action = 3;
            }
            else
            {
                if (eAction == true) action = 3;
                else if (sAction == true) action = 4;
            }
        }
    }
    //std::cout << "Action: " << action << "\n";
    return action;
}

int eGreedy(int x, int y, int epsilonPercent=5)
{
    //RNG check: 95% optimal action; 5% random action for epsilonPercent=5
    int choice = rng(100);
    int threshold = 100 - epsilonPercent;
    int action = 0;

    //Determine the action to be taken:
#pragma region Action determination
    //From 1-95: Optimal action
    if (choice <= threshold)
    {
        //Optimal action chosen
        //Check for single highest Q-Val and execute the action
        //If more than 1 highest Q-val, random between them
        float wQVal = 0.0;
        float nQVal = 0.0;
        float eQVal = 0.0;
        float sQVal = 0.0;
        //Check west q-val: Col-1
        wQVal = std::stof(qGrid[scaleGrid(x)][scaleGrid(y) - 1]);
        //Check north q-val: Row-1
        nQVal = std::stof(qGrid[scaleGrid(x) - 1][scaleGrid(y)]);
        //Check east q-val: Col+1
        eQVal = std::stof(qGrid[scaleGrid(x)][scaleGrid(y) + 1]);
        //Check south q-val: Row+1
        sQVal = std::stof(qGrid[scaleGrid(x) + 1][scaleGrid(y)]);

        std::cout << "e-Greedy: {Exploit}\n";
        //Action selected output: W/N/E/S
        action = maxQSelect(wQVal, nQVal, eQVal, sQVal);
    }
    //From 96-100: Random action
    else if (choice > threshold)
    {
        //Random action chosen (1 of 4 possible actions)
        int randChoice = rng(4);
        if (randChoice == 1) action = 1;        //West
        else if (randChoice == 2) action = 2;   //North
        else if (randChoice == 3) action = 3;   //East
        else if (randChoice == 4) action = 4;   //South
        std::cout << "e-Greedy: |Explore|\n";
    }
    std::cout << "Current position: " << x << "," << y <<"\n";
#pragma endregion Action determination
    
#pragma region Drift
    //Once maxQSelect or rng selects the desired direction, account for drift:
    //80% to go towards desired action
    //10% to drift to each side

    int driftRng = rng(100);
    //If 1-80: Desired direction | 81-90: Drift to sideA | 91-100: Drift to sideB
    bool desiredDirection = false, driftA = false, driftB = false;
    if (driftRng <= 80) desiredDirection = true;
    else if (driftRng > 80 && driftRng <= 90) driftA = true;
    else if (driftRng > 90) driftB = true;

    std::cout << "Desired Action: " << action;
    switch (action)
    {
    case 1://Desired direction is West
        if (desiredDirection == true) action = 1;   //West
        else if (driftA == true) action = 2;        //North
        else if (driftB == true) action = 4;        //South
        break;
    case 2://Desired direction is North
        if (desiredDirection == true) action = 2;   //North
        else if (driftA == true) action = 1;        //West
        else if (driftB == true) action = 3;        //East
        break;
    case 3://Desired direction is East
        if (desiredDirection == true) action = 3;   //East
        else if (driftA == true) action = 2;        //North
        else if (driftB == true) action = 4;        //South
        break;
    case 4://Desired direction is South
        if (desiredDirection == true) action = 4;   //South
        else if (driftA == true) action = 1;        //West
        else if (driftB == true) action = 3;        //East
        break;
    }
#pragma endregion Drift
    if (desiredDirection != true) std::cout << " | Drift occured | ";
    std::cout << "| Actual Action: " << action << "\n";
    //Final resulting action (takes into account maxQVal, rng, and drift)
    //The true position that will end up happening
    return action;
}

bool endState(std::pair<int, int> position)
{
    //Check to see if current position is terminal state
    if (grid[position.first][position.second] == "+100" || grid[position.first][position.second] == "-100")
    {
        return true;
    }
    else return false;
}

std::pair<int,int> qLearning(int x, int y, int epsilonPercent=5, int actionOverride=0)
{
    int oldNVal = 0, newNVal = 0;
    float oldQVal = 0.0, newQVal = 0.0;
    float nextQVal = 0.0, deltaA = 0.0, deltaB = 0.0;
    float discount = 0.9, actionReward = 0.0;
    int action = 0;
    if (actionOverride != 0)
    {
        action = actionOverride;
    }
    else
    {
        action = eGreedy(x, y, epsilonPercent);
    }
    
    int nextRow = 0, nextCol = 0;
    float nextWQ = 0.0, nextNQ = 0.0, nextEQ = 0.0, nextSQ = 0.0;
    int i = 0, j = 0;
    //Input the state (position), eGreedy selects the action
    switch (action)
    {
    case 1: //West  (Col-1) | R(s,a) = -2
        i = scaleGrid(x), j = scaleGrid(y) - 1;
        actionReward = -2.0;
        if (grid[x][y - 1] == "####" || y - 1 < 0) { nextCol = y; }
        else { nextCol = y - 1; }
        nextRow = x;
        break;
    case 2: //North (Row-1) | R(s,a) = -1
        i = scaleGrid(x) - 1, j = scaleGrid(y);
        actionReward = -1.0;
        if (grid[x - 1][y] == "####" || x - 1 < 0) { nextRow = x; }
        else { nextRow = x - 1; }
        nextCol = y;
        break;
    case 3: //East  (Col+1) | R(s,a) = -2
        i = scaleGrid(x), j = scaleGrid(y) + 1;
        actionReward = -2.0;
        if (grid[x][y + 1] == "####" || y + 1 >= col) { nextCol = y; }
        else { nextCol = y + 1; }
        nextRow = x;
        break;
    case 4: //South (Row+1) | R(s,a) = -3
        i = scaleGrid(x) + 1, j = scaleGrid(y);
        actionReward = -3.0;
        if (grid[x + 1][y] == "####" || x + 1 >= row) { nextRow = x; }
        else { nextRow = x + 1; }
        nextCol = y;
        break;
    }
    
    //Update the state-action
    std::pair<int, int> updatedPosition(nextRow, nextCol);

    oldNVal = std::stoi(countGrid[i][j]);
    newNVal = oldNVal + 1;
    
    oldQVal = std::stof(qGrid[i][j]);
    
    //Q-Learning update cases: Terminal or non-terminal states
    if (endState(updatedPosition))
    {
        //Goal
        if (grid[updatedPosition.first][updatedPosition.second] == "+100")
        {
            nextQVal = 100.0;
        }
        //Trap
        else if (grid[updatedPosition.first][updatedPosition.second] == "-100")
        {
            nextQVal = -100.0;
        }

    }
    else //Not terminal state
    {
        nextWQ = std::stof(qGrid[scaleGrid(nextRow)][scaleGrid(nextCol) - 1]);
        nextNQ = std::stof(qGrid[scaleGrid(nextRow) - 1][scaleGrid(nextCol)]);
        nextEQ = std::stof(qGrid[scaleGrid(nextRow)][scaleGrid(nextCol) + 1]);
        nextSQ = std::stof(qGrid[scaleGrid(nextRow) + 1][scaleGrid(nextCol)]);

        nextQVal = std::max(nextWQ, std::max(nextNQ, std::max(nextEQ, nextSQ)));
    }
    deltaA = 1.0 / (float)newNVal; // 1/N(s,a)
    deltaB = actionReward + (discount * nextQVal) - oldQVal;    //R(s,a) + (gamma * maxNextQVal) - oldQVal
    newQVal = oldQVal + (deltaA*deltaB);

    //Store iteration calculations
    countGrid[i][j] = std::to_string(newNVal);
    qGrid[i][j] = std::to_string(newQVal);

    //std::cout << "Curent (x,y): " << x << "," << y << "\nAction: " << action << "\nNext (x,y): " << nextRow << "," << nextCol <<"\n";
    return updatedPosition;
}

std::pair<int,int> initPosition()
{
    std::pair<int, int> position(0, 0);
    bool validPosition = false;
    while (validPosition == false)
    {
        position.first = rand()%row;    //From 0 ~ row-1
        position.second = rand()%col;   //From 0 ~ col-1
        
        //Check if initial position isn't an obstacle
        if (grid[position.first][position.second] != "####")
        {
            //Then check initial position isn't a terminal state
            if (grid[position.first][position.second] != "+100" && grid[position.first][position.second] != "-100")
            {
                validPosition = true;
            }
        }
    }
    return position;
}

void envSim(int maxTrial, int epsilonPercent=5)
{
    int i = 0;
    //Generate random valid grid space to start
    std::pair<int, int> position(0, 0);

    while (i != maxTrial)
    {
        //Generate random init position
        position = initPosition();
        while (endState(position) != true)
        {
            //While we are not at terminal state, continue
            position = qLearning(position.first, position.second, epsilonPercent);
            draw();
        }
        //Terminal state reached, start next trial
        i++;
    }
}

void envSeed()
{
    int epsilon = 5;
    for (int x = 0; x < row-1; x++)
    {
        for (int actions = 1; actions <= 4; actions++)
        {
            qLearning(4-x, 0, epsilon, actions);
        }
    }
    
    for (int y = 1; y < col; y++)
    {
        for (int actions = 1; actions <= 4; actions++)
        {
            qLearning(5, y, epsilon, actions);
        }
    }
    
    for (int y = 0; y < col - 1 ; y++)
    {
        for (int actions = 1; actions <= 4; actions++)
        {
            qLearning(0, y, epsilon, actions);
        }
    }
    
    for (int x = 0; x < row-1; x++)
    {
        for (int actions = 1; actions <= 4; actions++)
        {
            qLearning(5-x, 6, epsilon, actions);
        }
    }
}

int main()
{
    addObstacle(1, 2);
    addObstacle(1, 3);
    addObstacle(1, 4);
    addObstacle(1, 5);
    addObstacle(2, 2);
    addObstacle(2, 5);
    addObstacle(3, 1);
    addObstacle(3, 2);
    addObstacle(3, 5);
    addObstacle(4, 4);
    addObstacle(4, 5);
    initGrids();

    srand(111);
    //envSeed();
    draw();

    envSim(20000);

    std::cout << "End\n";
    draw();
}