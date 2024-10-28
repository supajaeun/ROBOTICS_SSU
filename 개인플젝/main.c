#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct score{
    int game_played;
    int wins;
    int losses;

}Score;

typedef struct user{
    char id[30];
    char password[30];
}User;

void new_account(char *filename);
bool create_user (char *filename, char *id, char *password);
bool is_id_exists(char *filename, char *id);
void get_user(char *id, char *password);
bool login(char *filename, char *global_id);
bool is_user_exist(char *filename, char*id , char *password);
void print_board(char board[7][11], int idx_x[], int idx_y[], int idx_com, int idx_user);
bool boardgame(char *id);
void game_history(char *id);
void save_score(char *id, int game_played, int wins, int losses);
void rules();

// 메인함수

int main(){

    //1. 로그인 

    bool stop_running = false;
    bool stop_running2 = false;  // while문 반복 제어하는 변수
    char global_id[30];

    while (stop_running == false)
    {
        int choice;
        system("clear");
        printf("=============== 주사위 보드게임 =============== \n");
        printf(" 1. 회원가입 \n 2. 로그인 \n 0. 종료 \n");
        printf("=============================================== \n\n");
        printf(" 번호를 입력하세요 : ");
        
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
    
            new_account("/Users/jaeun/Desktop/숭실대/로보틱스/file2.txt");
            break;

        case 2:
  
            stop_running = login("/Users/jaeun/Desktop/숭실대/로보틱스/file2.txt", global_id);
            
            break;

        case 0:
            stop_running = true;
            break;

        default:

            printf("잘못된 선택입니다");
            break;
        }   
    }

    while(stop_running2== false){

        int choice;
        printf("=============================================== \n");
        printf(" 1. 게임 실행 \n 2. 게임 기록 \n 3. 게임 규칙 \n 0. 로그아웃 \n");
        printf("=============================================== \n");
        printf("\n");
        printf(" 번호를 입력하세요 : ");

        scanf("%d", &choice); 

        switch (choice)
        {
        case 1:
            boardgame(global_id);
            break;

        case 2:
            game_history(global_id);
            break;   

        case 3:
            rules();
            break; 

        case 0:
            stop_running2 = true;
            break;

        default:
          
            printf("잘못된 선택입니다");
            break;
        }  


    }

    

    //2. 게임 실행
    



    return 0;
}





// 로그인/ 회원가입 함수

// 1. new_account : 사용자 입력을 받아, create_account 함수를 호출하여 새 계정을 만드는 함수
void new_account(char *filename){
    system("clear");
    printf("================ 회원가입 ================ \n \n");
    char id[30], password[30];

    printf("id : ");
    scanf("%s", id);
    printf("\npassword : ");
    scanf("%s", password);
    printf("\n");
    
   if (create_user(filename, id, password)) // 
   //  사용자명이 존재하지 않으면 (create_user 함수의 반환값이 True면)
   {
    system("clear");
    printf(" 계정이 생성되었습니다. \n");
    sleep(1);
    printf(" 선택 화면으로 돌아갑니다. \n ");
    sleep(1);
   }
   else {
    system("clear");
    printf(" 계정 생성에 실패했습니다. \n 사용자명이 이미 존재합니다. \n") ;// 사용자명이 존재할 경우
    sleep(1);
    printf(" 선택 화면으로 돌아갑니다. \n ");
    sleep(1);
   }

} 

// 2. create_user : 파일에 정보를 write 하는 함수
// 저장되면 true를 반환
bool create_user (char *filename, char *id, char *password){
    FILE *fptr;
    User new_user;
    fptr = fopen(filename, "ab");

    if (fptr == NULL){ 
        return false;
    }

    // 새로운 사용자의 정보 복사
    strcpy (new_user.id, id);
    strcpy (new_user.password, password);

    // 중복확인
    if (is_id_exists(filename, id)) // 이미 존재하는 경우
    { 
        fclose(fptr); 
        return false;  // 파일을 닫고 false를 반환함
    }

    // 존재하지 않는 경우 파일에 사용자 정보를 저장함
    else{
        fwrite(&new_user, sizeof(User), 1, fptr); // fwrite함수를 사용하여 파일에 쓰기
        fclose(fptr); 
        return true; // 파일을 닫고 True를 반환함
    }
}


//3. is_username_exists : 사용자명이 존재하는지 확인하는 함수 
// 존재하면 True, 아니면 False 
bool is_id_exists(char *filename, char *id)
{
    FILE *fptr;
    User temp_user; // 파일에서 읽어온 사용자 정보를 임시로 저장할 구조체
    fptr = fopen(filename, "rb"); // 파일을 읽기모드로 불러옴

    while (fread(&temp_user, sizeof(User), 1, fptr) == 1) // User의 바이트만큼의 데이터를 읽고
    // temp_user에 저장. 데이터가 없을 때까지 반복함

    {
        if (strcmp(temp_user.id, id) == 0) // 현재 읽은 id와 비교해서 일치할 경우 
        {
            fclose(fptr);
            return true; // true를 반환
        }
    }
    fclose(fptr);
    return false;
}


//4. login 함수
bool login(char *filename, char *global_id){
    char id[20], password[20];

    system("clear");
    printf("================ 로그인 ================ \n \n");
    printf("id와 password를 입력하세요. \n\n");
    printf("id: ");
    scanf("%s", id);
    printf("password: ");
    scanf("%s", password);

    if (is_user_exist(filename, id,password))
    {   
        system("clear");
        printf("로그인 성공 \n");
        strcpy(global_id, id);
        sleep(1);
        system("clear");
        printf(" %s 님 반갑습니다.  \n\n", global_id);
        return true;
        
    }
    else{
    system("clear");
    printf(" 로그인에 실패했습니다. \n아이디 또는 비밀번호가 틀렸습니다. \n");
    sleep(1);
    printf(" 선택 화면으로 돌아갑니다. \n ");
    sleep(1);

    return false;
}
}


// 6. is_user_exist : id, password 확인 함수
bool is_user_exist(char *filename, char*id , char *password){
    FILE *fptr;
    User temp_user;
    fptr = fopen(filename, "rb");
    while(fread(&temp_user, sizeof(User), 1, fptr) == 1){
        if (strcmp(temp_user.id, id) ==0 && strcmp(temp_user.password, password)==0 )
            {
                fclose(fptr);
                return true;

            }
    }
    fclose(fptr);
    return false;
}


// 게임 함수

// 1. print_board : 보드를 출력하는 함수
void print_board(char board[7][11], int idx_x[], int idx_y[], int idx_com, int idx_user) {
    printf("\n");
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 11; j++) {
            if (idx_com < 21 && idx_user < 21 && idx_x[idx_com] == i && idx_y[idx_com] == j) {
                printf("𖠌");
            } else if (idx_com < 21 && idx_user < 21 && idx_x[idx_user] == i && idx_y[idx_user] == j) {
                printf("ဗီူ");
            } else {
                printf("%c", board[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// 2. boardgame 

bool boardgame(char *id) {

    system("clear");
    printf("=============== 게임을 시작합니다. =============== \n\n");
    sleep(1);    

    
    char board[7][11] = {
        {'o', ' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'},
        {'o', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'o'},
        {'o', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'o'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'o', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'o'},
        {'o', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'o'},
        {'o', ' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'}
    };
    int len_val = 21;
    int idx_x[21] = {6, 5, 4, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 4, 5, 6, 6, 6, 6, 6, 6};
    int idx_y[21] = {10, 10, 10, 10, 10, 10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 2, 4, 6, 8, 10};
    int idx_com = 0, idx_user = 0;
    int is_computer_turn;
    int user_choice, computer_choice;
    int r;
    int wins=0 , losses=0 , game_played =0;
    bool is_game_over = false;

    srand(time(NULL));  // 난수 초기화
    print_board(board, idx_x, idx_y, idx_com, idx_user);

    while (!is_game_over) {
        printf("가위바위보로 선공과 후공을 정하세요.\n");
        usleep(300000);
        printf("가위(0), 바위(1), 보(2)를 입력하세요: ");
        scanf("%d", &user_choice);

        sleep(1);

        computer_choice = rand() % 3;

        if (user_choice == computer_choice) {
            system("clear");
            printf("비겼습니다. 다시 가위바위보를 진행하세요.\n");
            sleep(1);
            continue;
        } else if ((user_choice == 0 && computer_choice == 2) || (user_choice == 1 && computer_choice == 0) || (user_choice == 2 && computer_choice == 1)) {

            printf("%s 님이 먼저 주사위를 던집니다.\n", id);
            is_computer_turn = 0;
        } else {
        
            printf("컴퓨터가 먼저 주사위를 던집니다.\n");
            is_computer_turn = 1;
        }
        sleep(1);

        while (!is_game_over) {
            if (is_computer_turn) {
                printf("컴퓨터가 주사위를 던집니다.\n");
                usleep(1500000);
                r = (rand() % 6) + 1;
                idx_com += r;
                sleep(1);
                system("clear");
                printf("컴퓨터가 주사위를 던져서 %d 이(가) 나왔습니다.\n", r);
                sleep(1);
                print_board(board, idx_x, idx_y, idx_com, idx_user);

                if (idx_com == idx_user) {
                    printf("컴퓨터가 %s 님의 말을 잡았습니다!\n", id);
                    usleep(300000);
                    printf("%s 님이 한 턴 쉽니다.\n",id);
                    sleep(1);
                    is_computer_turn = 1;
                } else {
                    is_computer_turn = !is_computer_turn;
                }
            } else {
                printf("주사위를 던지세요! 0을 입력하세요:\n");
                scanf("%d", &user_choice);  // 실제로는 사용자 입력값과 무관하게 난수를 생성합니다.
                r = (rand() % 6) + 1;
                idx_user += r;
                sleep(1);
                system("clear");
                printf("%s 님이 주사위를 던져서 %d 이(가) 나왔습니다.\n",id, r);
                sleep(1);
                print_board(board, idx_x, idx_y, idx_com, idx_user);
                sleep(1);

                if (idx_user == idx_com) {
                    printf("%s 님이 컴퓨터의 말을 잡았습니다!\n", id);
                    usleep(300000);
                    printf("컴퓨터가 한 턴 쉽니다.\n");
                    sleep(1);
                    is_computer_turn = 0;
                } else {
                    is_computer_turn = !is_computer_turn;
                }
            }

            if (idx_com >= len_val) {
                system("clear");
                printf(" 패배!\n\n");
                sleep(1);
                printf(" 선택 화면으로 돌아갑니다. \n ");
                sleep(1);
                losses += 1;
                wins = 0;
                game_played +=1;
                save_score(id, game_played, wins, losses);
                is_game_over = true; 

            } else if (idx_user >= len_val) {
                system("clear");
                printf(" 승리!\n\n");
                sleep(1);
                printf(" 선택 화면으로 돌아갑니다. \n ");
                sleep(1);
                
                losses = 0;
                wins += 1;
                game_played +=1;
                save_score(id, game_played, wins, losses);
                is_game_over = true; 
            }
        }
         return is_game_over;
    }
    
}

// 승률 계산 함수

//1. save_score : 파일에 게임 기록을 업데이트하는 함수

void save_score(char *id, int game_played, int wins, int losses) {
    FILE *fptr;
    Score score;
    char file_name[100];

    // 파일 이름 설정
    sprintf(file_name, "/Users/jaeun/Desktop/숭실대/로보틱스/%s", id);

    // 파일을 읽기 모드로 오픈
    fptr = fopen(file_name, "r+b");
    if (fptr != NULL) {
        // 파일이 존재하면 기존 데이터를 읽기
        if (fread(&score, sizeof(Score), 1, fptr) == 1) {
            // 기존 데이터에 새로운 결과 추가
            score.game_played += game_played;
            score.wins += wins;
            score.losses += losses;
            // 파일의 시작 위치로 이동
            fseek(fptr, 0, SEEK_SET);
        }
    } else {
        // 파일이 존재하지 않으면 w+b모드, 파일을 열 때 내용을 모두 지움
        fptr = fopen(file_name, "w+b");
        if (fptr == NULL) {
            printf("파일을 열 수 없습니다.\n");
            return;
        }
        // 새 데이터 설정
        score.game_played = game_played;
        score.wins = wins;
        score.losses = losses;
    }

    // 수정된 데이터를 파일에 쓰기
    fwrite(&score, sizeof(Score), 1, fptr);
    fclose(fptr);
}

//2. game_hystory : 게임 기록 출력함수
void game_history(char *id) {
    system("clear");
    printf("================== 게임 기록 ================== \n \n");
    
    FILE *fptr;
    Score score;
    char file_name[100];

    // 파일 이름 설정
    sprintf(file_name, "/Users/jaeun/Desktop/숭실대/로보틱스/%s", id);

    fptr = fopen(file_name, "rb");  // 읽기 모드로 오픈
    if (fptr == NULL) {
        printf("게임 기록을 찾을 수 없습니다.\n");
        sleep(2);
    } else {
        if (fread(&score, sizeof(Score), 1, fptr) == 1) {
            printf("%d전 %d승 %d패 \n\n", score.game_played, score.wins, score.losses);
            sleep(2);
        
        } else {
            printf("기록을 읽을 수 없습니다.\n");
            sleep(2);
        }
        fclose(fptr);
    }
}

void rules(){
    system("clear");
    printf("================== 게임 규칙 ================== \n \n");
    printf("사용자와 컴퓨터가 교대로 주사위를 던지면서 말을 이동시키고 \n");
    printf("보드판을 먼저 빠져나간 플레이어가 승리하는 게임입니다. \n\n");
    printf(" - 사용자와 컴퓨터가 가위바위보 게임을 하고 승자가 선공이 됩니다. \n");
    printf(" - 앞서 나가고 있는 말을 잡게 되면 잡힌 플레이어는 다음 턴을 쉽니다. \n\n");


    }
