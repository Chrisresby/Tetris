#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h> 

using namespace std;


wstring tetromino[7];
//Board Width
int nFieldWidth = 12;
//Board Height
int nFieldHeight = 18; 

unsigned char *pField = nullptr; 

//WINDOW
int nScreenWidth = 80;          //Console Screen Size x (Columns)
int nScreenHeight = 30;         //Console Screen Size y (Rows)

//Rotation Formula
int Rotate(int px, int py, int r)
{
    switch(r % 4)
    {
        case 0: return py * 4 + px;              //0 Degrees
        case 1: return 12 + py - (px * 4);       //90 Degrees
        case 2: return 15 - (py * 4) - px;       //180 Degrees
        case 3: return 3 - py + (px * 4);        //270 Degrees
    }
    return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for(int px = 0; px < 4; px++)
        for(int py = 0; py < 4; py++)
        {
            //Get index into piece
            int pi = Rotate(px, py, nRotation);
            
            //Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);
            
            if(nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if(nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if(tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                        return false; //fail on first hit
                }
            }
        }
    
    return true;
}

int main()
{
	//Create assets
     tetromino[0].append(L"..x.");
     tetromino[0].append(L"..x.");
     tetromino[0].append(L"..x.");
     tetromino[0].append(L"..x.");

     tetromino[1].append(L"..x.");
     tetromino[1].append(L".xx.");
     tetromino[1].append(L".x..");
     tetromino[1].append(L"....");
     
     tetromino[2].append(L".x..");
     tetromino[2].append(L".xx.");
     tetromino[2].append(L"..x.");
     tetromino[2].append(L"....");
     
     tetromino[3].append(L"....");
     tetromino[3].append(L".xx.");
     tetromino[3].append(L".xx.");
     tetromino[3].append(L"....");
     
     tetromino[4].append(L"..x.");
     tetromino[4].append(L"..x.");
     tetromino[4].append(L"..x.");
     tetromino[4].append(L"..x.");
     
     tetromino[5].append(L"..x.");
     tetromino[5].append(L".xx.");
     tetromino[5].append(L"..x.");
     tetromino[5].append(L"....");
     
     tetromino[6].append(L"....");
     tetromino[6].append(L".xx.");
     tetromino[6].append(L".x..");
     tetromino[6].append(L".x..");
     
     pField = new unsigned char[nFieldWidth * nFieldHeight]; //Create board
     for(int x = 0; x < nFieldWidth; x++) //Board Boundary
        for(int y = 0; y < nFieldHeight; y++)
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth -1 || y == nFieldHeight - 1) ? 9 : 0;
            
     
     wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
     for(int i = 0; i < nScreenWidth * nScreenHeight; i++)
         screen[i] = L' ';
     HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
     SetConsoleActiveScreenBuffer(hConsole);
     DWORD dwBytesWritten = 0; 
     
     //Game Loop
     bool bGameOver = false;
     
     int nCurrentPiece = 0;
     int nCurrentRotation = 0;
     int nCurrentX = nFieldWidth / 2;
     int nCurrentY = 0;
     
     bool bKey[4];
     bool bRotateHold = false;
     
     int nSpeed = 20;
     int nSpeedCounter = 0;
     bool bForceDown = false;
     int nPieceCounter = 0; 
     int nScore;
     
     vector<int> vLines;

     
     while(!bGameOver)
     {
         //GAME TIMING =====================================
         this_thread::sleep_for(50ms); // Game Tick
         nSpeedCounter++;
         bForceDown = (nSpeedCounter == nSpeed);
         
         
         // INPUT ==========================================
         for(int k = 0; k < 4; k++)
             bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) !=
         
         
         // GAME LOGIC =====================================
         //Pressing The Left Key
         nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece,nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
         
         //Pressing the Right Key
         nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece,nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
         
         //Pressing the Down Key
         nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece,nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;
         
        //Pressing the Z key to rotate
        if(bKey[3])
        {
            nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
            bRotateHold = true;
        }
        else
            bRotateHold = false;
            
            
        if(bForceDown)
        {
            if(DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                nCurrentY++;
            else
            {
                //Lock the currenct piece in the field
                for(int px = 0; px < 4; px++)
                    for(int py = 0; py < 4; py++)
                        if(tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1; 
                
                
                //Count Pieces
                nPieceCounter++;
                if(nPieceCounter % 10 == 0)
                    if(nSpeed >= 10)
                        nSpeed--;
                        
                        
                //Check have we got any lines
                for(int py = 0; py < 4; py++)
                    if(nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for(int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
                            
                        if(bLine)
                        {
                            //Remove Line, Set =
                            for(int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                                
                                vLines.push_back(nCurrentY + py);
                        }
                    }
                //Keeping Score
                nScore += 25;
                if(!vLines.empty())
                    nScore += (1 << vLines.size() * 100); 
                    
                    
                //Choose next piece
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = rand() % 7;
                
                
                //if piece does not fit
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
            
            nSpeedCounter = 0;
        }

         // RENDER OUTPUT ==================================
         
         
         //Draw Field
         for(int x = 0; x < nFieldWidth; x++)
             for(int y = 0; y < nFieldHeight; y++)
                 screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]]; 
                 
                 
        //Draw Current Piece
        for(int px = 0; px < 4; px++)
            for(int py = 0; py < 4; py++)
                if(tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;
                    
        
        //Display Score
        swprintf_l(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore); 
        
        if(!vLines.empty())
        {
            //Display Frame
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0,0}, &dwBytesWritten);
            this_thread::sleep_for(400ms); //Delay a bit
            
            for(auto &v : vLines)
                for(int px = 1; px < nFieldWidth - 1; px++)
                {
                    for(int py = v; py > 0; py--)
                        pField[py * nFieldWidth + px] = pField[(py -1) * nFieldWidth + px];
                        
                    pField[px] = 0;
                }
                
                //Remove lines
                vLines.clear();
        }
        //Display Frame
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0,0}, &dwBytesWritten);
     }
     
     //Game Over
    CloseHandle(hConsole);
    cout<< "Game Over!! Score: " << nScore << endl;
    system("pause");
     
     
     
     
     
     
	return 0;
}
