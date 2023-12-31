#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#define MEM_SIZE 1000
#define X = 1
#define O = 2

int gameMap[9] = {0,0,0,0,0,0,0,0,0,};
char printCell(int choose){
    if(choose == 0){
        return ' ';
    }
    if(choose == 1){
        return 'X';
    }
    if(choose == 2){
        return 'O';
    }
}
bool isYouWin(int side){
    return ((gameMap[0] == side )&&(gameMap[1]==side)&&(gameMap[2] == side))||((gameMap[3] == side )&&(gameMap[4]==side)&&(gameMap[5] == side))||
            ((gameMap[6] == side )&&(gameMap[7]==side)&&(gameMap[8] == side))||((gameMap[0] == side )&&(gameMap[3]==side)&&(gameMap[6] == side))||
            ((gameMap[1] == side )&&(gameMap[4]==side)&&(gameMap[7] == side))||((gameMap[2] == side )&&(gameMap[5]==side)&&(gameMap[8] == side))||
            ((gameMap[0] == side )&&(gameMap[4]==side)&&(gameMap[8] == side))||((gameMap[2] == side )&&(gameMap[4]==side)&&(gameMap[6] == side));
}
void showGameMap(){

    printf(" %c | %c | %c\n", printCell(gameMap[0]), printCell(gameMap[1]), printCell(gameMap[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", printCell(gameMap[3]), printCell(gameMap[4]), printCell(gameMap[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", printCell(gameMap[6]), printCell(gameMap[7]), printCell(gameMap[8]));
}
void showCoordinates(){
    printf("For hitting cell write number of cell\n");
    printf(" 1 | 2 | 3\n");
    printf("---+---+---\n");
    printf(" 4 | 5 | 6\n");
    printf("---+---+---\n");
    printf(" 7 | 8 | 9\n");
}
bool isDraw(){
    for(int i = 0; i<9;i++){
        if(gameMap[i] == 0){
            return false;
        }
    }
    return true;
}



int main(int argc, char ** argv) {
    if(argc != 2){
        fprintf(stderr, "Use %s  <game_name> \n",argv[0]);
        return 1;
    }

    HANDLE mapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            argv[1]
            );
    bool imCreator = false;
    if(mapFile == NULL){
        mapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,
                NULL,
                PAGE_READWRITE,
                0,
                MEM_SIZE,
                argv[1]
                );
        if(mapFile != NULL){
            printf("Game created! id: %s\n",argv[1]);
            imCreator = true;
        }else{
            fprintf(stderr,"Error during creation game!\n");
            return 1;
        }
    }else{
        printf("Successfully connected to game %s !\n",argv[1]);

    }
    LPVOID *buff = MapViewOfFile(
            mapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            MEM_SIZE
            );
    if(buff == NULL){
        fprintf(stderr,"Error during creation  map of game!\n");
        return 2;
    }
    startGame:
    if(strcmp(buff,"_IM_LEAVING_")==0){
        printf("Second player leaved game. Press <ENTER> to close game\n");
        getchar();
        goto endGame;

    }
    for(int i = 0; i<9 ;i++){
        gameMap[i] = 0;
    }
    if(imCreator){
        printf("Waiting for second player...\n");
        strcpy( buff, "_X_WAIT_");
        int imPlayedX  = 1;
        while(strcmp(buff, "_X_WAIT_") ==0){
            Sleep(10);
        }
        showCoordinates();

        while(!isYouWin(imPlayedX) && !isYouWin(2) && !isDraw()) {
            int myTarget;
            printf("Current map\n");
            showGameMap();
            printf("Your move:\n");
            do{
            scanf_s("%d",&myTarget);
            if(myTarget<1 || myTarget > 9 || gameMap[myTarget-1] !=0){
                printf("Wrong target\n");
            }
            }while(myTarget < 1 || myTarget>9 || gameMap[myTarget-1] !=0);

            gameMap[myTarget-1] = imPlayedX;
            if(isYouWin(imPlayedX) || isDraw()){
                break;
            }
            showGameMap();
            char hit[3];
            itoa(myTarget, (char *) hit, 10);
            hit[1] = 'X';
            hit[2] = '\0';
            strcpy( buff,hit );

            printf("Waiting for second player move...\n");

            while (strcmp(buff,hit)==0){
                Sleep(10);
            }

            strcpy(hit,buff);
            int target = atoi(hit);

            gameMap[target-1] = 2;
            if(strcmp(buff,"_I_WIN_")==0 ||strcmp(buff,"_I_LOST_")==0||strcmp(buff,"_DRAW_")==0){
                break;
            }


        }
        showGameMap();
        if(isYouWin(imPlayedX)){
            strcpy(buff,"_I_WIN_");
            printf("Congrats! You are winner!\n");
        }else if(isYouWin(2)){
            strcpy(buff,"_I_LOST_");
            printf("Good luck next time\n");
        }else if(strcmp(buff,"_I_WIN_")==0){
            printf("Good luck next time\n");
        }else if(strcmp(buff,"_I_LOST_") == 0){
            printf("Congrats! You are winner!\n");
        }else {
            strcpy(buff,"_DRAW_");
            printf("Draw!\n");
        }


    }else{
        printf("Press <Enter> to connect a game\n");
        getchar();
        printf("Connecting\n");
        strcpy( buff, "_IM_READY_");
        int imPlayedO  = 2;
        while(strcmp(buff, "_IM_READY_") ==0){
            Sleep(10);
        }
        showCoordinates();

        while(!isYouWin(imPlayedO) && !isYouWin(1) && !isDraw()) {
            char hit[3];

            strcpy(hit,buff);
            int target = atoi(hit);

            gameMap[target-1] = 1;
            if(strcmp(buff,"_I_WIN_")==0 ||strcmp(buff,"_I_LOST_")==0||strcmp(buff,"_DRAW_")==0){
                break;
            }
            int myTarget;
            printf("Current map\n");
            showGameMap();
            printf("Your move:\n");

            do{
                scanf_s("%d",&myTarget);
                if(myTarget<1 || myTarget > 9 || gameMap[myTarget-1] !=0){
                    printf("Wrong target\n");
                }
            }while(myTarget < 1 || myTarget>9 || gameMap[myTarget-1] !=0);

            gameMap[myTarget-1] = imPlayedO;

             itoa(myTarget, (char *) hit, 10);
            hit[1] = 'O';
            hit[2] = '\0';
            strcpy( buff,hit);
            if(isYouWin(imPlayedO) || isDraw() || isYouWin(1)){
                break;
            }
            printf("Waiting for second player move...\n");
            while (strcmp(buff,hit)==0){
                Sleep(10);
            };


        }
        showGameMap();
        if(isYouWin(imPlayedO)){
            strcpy(buff,"_I_WIN_");
            printf("Congrats! You are winner!\n");
        }else if(isYouWin(1)){
            strcpy(buff,"_I_LOST_");
            printf("Good luck next time\n");
        }else if(strcmp(buff,"_I_WIN_") == 0){
            printf("Good luck next time\n");
        }else if(strcmp(buff,"_I_LOST_") == 0){
            printf("Congrats! You are winner!\n");
        }
        else {
            strcpy(buff,"_DRAW_");
            printf("Draw!\n");
        }


    }
    printf("New Game?\n");
    int choose;
    do {
        printf("[1] - Yes / [2] - No ?\n");
        scanf_s("%d",&choose);
    } while ((choose != 1) && (choose !=2));
    if(choose == 1){
        goto startGame;
    }

    strcpy(buff,"_IM_LEAVING_");
    endGame:
    if(!UnmapViewOfFile(buff)) {
        fprintf(stderr, "Error during unmap view of file\n");
        CloseHandle(mapFile);
        return 3;
    }
    CloseHandle(mapFile);



    return 0;
}
