#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// set global_variables
char player1_name[11] , player2_name[11];
int current_player = 1 , redo_ = 0 , undo_= 0 , check_ = 0 , check_mate_ = 0 , promotion = 0 , computer = 0;
char player1_remove[16] , player2_remove[16];
FILE *scores , *games ;
int load1 = 0;

// all_functions
void initialize(char *pointer);
void initialize_global_variables();
// to set character to the grid
char get_char(char *pointer , int i , int j);
void set_char(char *pointer , int i , int j , char c);
void message(char *mes);
void welcome();
void take_name(int i);
void show_options_1(char *pointer);
void show_options_2();
int check_name_matches();
void take_names();
void load_files();
void close_files();
void load_and_game(char *pointer);
void set_grid(char *pointer);
void set_remove_arraies();
int load(char *pointer);
void copy_grid(char *p , char *pointer);
void copy_remove_arrays(char *remove1 , char *remove2);
int  check_in_files();
void new_game(char *pointer);
void play(char *pointer);
void print_grid(char *pointer);
void take_the_move(char *pointer);
int check_valid(char *string);
int validate(char *pointer,int i1,int j1,int i2,int j2);
void parse_input(char *string , int* i1 , int* j1 , int* i2 , int* j2);
int validate_pawn(char *pointer,int  i1,int j1, int i2, int j2);
int validate_rook(char *pointer,int  i1,int j1, int i2, int j2);
int validate_knight(char *pointer,int  i1,int j1, int i2, int j2);
int validate_bishop(char *pointer,int  i1,int j1, int i2, int j2);
int validate_queen(char *pointer,int  i1,int j1, int i2, int j2);
int validate_king(char *pointer,int  i1,int j1, int i2, int j2);
void make_move(char *pointer ,int i1 ,int j1,int i2,int j2);
int is_empty_vert(char *pointer , int i1 ,int j1 , int i2 , int j2);
int is_empty_horz(char *pointer , int i1 ,int j1 , int i2 , int j2);
int is_empty_diag(char *pointer , int i1 ,int j1 , int i2 , int j2);
void replace_cell(int c2);
void add_remove_piece(char c2,int id);
void remove_removed_piece(int id);
void save_step(char *pointer , int i1 , int j1 ,char c1 , int i2 , int j2 ,char c2 , int last_player);  // done
void add_node(char *pointer,int i1,int j1,char c1,int i2,int j2,char c2,int last_player); // done
struct step_node* create_new_node(char *pointer,int i1,int j1,char c1,int i2,int j2,char c2,int last_player); // done
void remove_nodes();
void save(char *pointer);
void Exit(char *pointer);
void redo(char *pointer);
void undo(char *pointer);
int check_valid_promotion_piece(char c,int player);
int check_for_status(char *pointer);
int get_int();
char* get_string();
int check_auto_save(char *pointer);
void remove_auto_file();
void update_auto_file(char *pointer);
char get_char2();
void set_new_nodes();
void remove_all_games();
//struct for moving parties
typedef struct move{
  int i, j;
  struct move* next;
} move;

int checkmate(char* pointer);
int check_king(char* pointer);
int check_stalemate(char* pointer);

//movement
void check_move(char*pointer,int i,int j ,move** list);
void check_move_p(char* pointer,int i, int j,move **list);
void check_move_q(char* pointer,int i, int j,move **list);
void check_move_r(char* pointer,int i, int j,move **list);
void check_move_k(char* pointer,int i, int j,move **list);
void check_move_b(char* pointer,int i, int j,move **list);
void check_move_n(char* pointer,int i, int j,move **list);
void list_assign(move** list,int i,int j);

void computer_play(char *pointer);



// here we initialize the node that is used to carry every step and add it  to a linked list
struct step_node{
    int i1 , j1 , i2 , j2 ,last_player ,prom;  // contain
    char c1 , c2;
    struct  step_node* next;
    struct step_node* pre;
};

// make head , current and last node to the linked list that contain steps
struct step_node *head;
struct step_node *current;
struct step_node *last;

int main(void)
{
    // initialize the board
    char grid[8][8];
    char *pointer = grid;

    initialize(pointer);
    play(pointer);

    return 0;
}


// inside each function write all functions

void initialize_global_variables(){

    current_player = 1 , redo_ = 0 , undo_= 0 , check_ = 0 , check_mate_ = 0;
}


void initialize(char *pointer){
    //  initialize_global_variables,welcome , take_names , load_game
   //initialize_global_variables();
   welcome();
   if(computer == 0) take_names();
   load_and_game(pointer);

}


void play(char *pointer){
    int check = 0;
     while(!check){
         load1 = 1;
         print_grid(pointer);
         show_options_2();

        if(current_player == 1)
            take_the_move(pointer);
        else{

            if(computer == 1)  computer_play(pointer);
            else take_the_move(pointer);
        }
     update_auto_file(pointer);
     current_player = (current_player == 1)?2:1;
     check = check_for_status(pointer);
    }
    remove_auto_file();
    remove_all_games();

    if(check == 1){
            print_grid(pointer);
          if(checkmate(pointer) == 1) {
                 if(computer == 1) printf("player 1 is checkmated , computer is win \n");
                 else printf("%s is checkmated , %s is win \n",player1_name,player2_name);
          }
          else {
            if(computer == 1) printf("computer is checkmated , player 1 is win \n");
            else  printf("%s is checkmated , %s is win \n",player2_name,player1_name);
        }

          return;
    }
  message("it is stalemate , Draw");  // stalemate
//print();
  return;
}

//check for the first time if it's load or start a new game


void load_and_game(char *pointer){
    // show options , set_new_nodes
    set_new_nodes();
    int check_auto_file = check_auto_save(pointer);
    if (check_auto_file == 0)show_options_1(pointer);
}

int check_auto_save(char *pointer){
      FILE *auto_file;
      auto_file = fopen("auto_file.txt","r");
      if(!auto_file) return 0;
      int size , i = 0 ;
      fseek(auto_file,0,SEEK_END);
      size = ftell(auto_file);
      if(size == 0) return 0;
      message("There is an Unsaved game Are you want to continue it :- (Y/N)");
      char x;
      x = get_char2();
      if(x == 'n' || x == 'N') return 0;
      fseek(auto_file,0,SEEK_SET);
      char arr[66];
      fgets(arr,66,auto_file);
      copy_grid(arr,pointer);
          if(arr[64] == '1') current_player = 2;
          else current_player = 1;
      fclose(auto_file);
      return 1;
}

char get_char2(){
  char x , c;
  int i = 0;
  x = getchar();
  while(x != '\n'){
    i++;
    c = x;
    x = getchar();
  }
  if(i != 1){
    message("Error enter valid (Y/N): - ");
    c = get_char2();
  }
  if(c == 'Y' || c== 'N' || c == 'y' || c == 'n') return c;
    message("Error enter valid (Y/N): - ");
    c = get_char2();
    return c;
}


// to update auto_file
void update_auto_file(char *pointer){
   FILE  *auto_file;
   int i,j;
   auto_file = fopen("auto_file.txt","w");
   for(i=0;i<8;i++){
    for(j=0;j<8;j++){
        fputc(get_char(pointer,i,j) , auto_file);
    }
   }
   fputc((char)('0'+current_player),auto_file);
   fclose(auto_file);
}

// to clear the auto save file after each save_choice from players
void remove_auto_file(){

    // just open a file with 'w' mode and close it immediately and every thing in it is removed
    FILE  *auto_file;
    auto_file = fopen("auto_file.txt","w");
    fclose(auto_file);
}
//set the grid to initial state
void set_grid(char *pointer)
{ // fares


     int i ,j ;
     set_char(pointer , 0 , 0 , 'R');
     set_char(pointer , 0 , 7 , 'R');
     set_char(pointer , 0 , 1 , 'N');
     set_char(pointer , 0 , 6 , 'N');
     set_char(pointer , 0 , 2 , 'B');
     set_char(pointer , 0 , 5 , 'B');
     set_char(pointer , 0 , 3 , 'Q');
     set_char(pointer , 0 , 4 , 'K');
     set_char(pointer , 7 , 0 , 'r');
     set_char(pointer , 7 , 7 , 'r');
     set_char(pointer , 7 , 1 , 'n');
     set_char(pointer , 7 , 6 , 'n');
     set_char(pointer , 7 , 2 , 'b');
     set_char(pointer , 7 , 5 , 'b');
     set_char(pointer , 7 , 3 , 'q');
     set_char(pointer , 7 , 4 , 'k');
     for(i=0;i<8;i++){
         set_char(pointer ,  1,i,'P');   ////   here stoooooop
         set_char(pointer , 6 , i, 'p');
    }
     for(i=2;i<6;i++){
        for(j=0;j<8;j++){
             set_char(pointer,i,j,'0');
        }
     }

}



char get_char(char *pointer , int i , int j){
   return *(pointer + i*8 + j);
}
void set_char(char *pointer , int i ,int j , char c) {
     *(pointer+ i*8 +j) = c;
}

// print_the grid
void print_grid(char *grid){
    system("cls");

    int i , j;
    char c;

   printf("\n\n\t\t");
   for(i=0;i<8;i++){
    printf("     %c",'A'+i);
   }
   printf("\t\tplayer_1 | player_2");
   printf("\n");
   printf("\t\t    ------------------------------------------------\n");
   for(i=0;i<8;i++){

        printf("\t\t %d |",8-i);
      for(j=0;j<8;j++){
          c = *(grid + i*8 + j);
        if( c == '0'){
            if(j%2 == 0){
                if(i%2 == 0) c = '.';
                else c = '-';
            }
            else{
                 if(i%2 == 0) c = '-';
                else c = '.';
            }
        }

         printf("  %c  |",c);
      }
      printf("  %d\t\t",8-i);
      printf("%c      %c | %c      %c\n",player1_remove[i*2],player1_remove[i*2+1],player2_remove[i*2],player2_remove[i*2+1]);
      printf("\t\t    ------------------------------------------------\n");
   }
  printf("\t\t");
   for(i=0;i<8;i++){
    printf("     %c",'A'+i);
   }
   printf("\n\n");
}



//to welcome the user in the first step of the game

void welcome(){
    // message
  message("Welcome in the chess game\n");
  message("1) player against player    2) player against computer \n \t\t Enter a choice :- ");
  int choice = get_int();
  if(choice == 2) computer  = 1;
}



//get every inf the the user typed and analyze it

void take_the_move(char *pointer){
      int i1,i2,j1,j2; promotion = 0;
      if(computer == 1) printf("player 1 is playing : ");
      else printf("%s is playing : ",(current_player == 1)?player1_name:player2_name);
      char *input = get_string();
      int length = strlen(input);
     // printf("%s %d\n",input,length);
      if(length != 1 && length != 4 && length != 5){
        message("Error your entered invalid choice Try again :  ");
        take_the_move(pointer);
      }
      else if(length == 1){
     char c = *(input);
      if(c == '1'){
          save(pointer);
      }
      else if(c == '2'){
            int x = load(pointer);
          if(x == 1 || x == 2 || x == 3)
            current_player = (current_player == 1)?2:1;
      }
      else if(redo_ && undo_){
         if(c == '3') redo(pointer);
         if(c == '4') undo(pointer);
      }
      else if(redo_ ==0 && undo_ == 1 && c == '3'){
         undo(pointer);
      }
      else if(undo_==0 && redo_ == 1 && c == '3'){
         redo(pointer);
      }
      else{
            message("Error you entered invalid indexes . try again : ");
            take_the_move(pointer);
      }
      }
      else if(length == 4){
           if(!check_valid(input)){
            message("Error you entered invalid indexes . try again : ");
            //printf("check_valid error\n");
            take_the_move(pointer);
           }
           else{ parse_input(input,&i1,&j1,&i2,&j2);
          // printf("%d %d %d %d\n",i1,j1,i2,j2);
           if(!validate(pointer,i1,j1,i2,j2)){
           //  printf("sjbivbu\n");
               take_the_move(pointer);

           }
           else{
                // if the player enter valid move so make this move

                make_move(pointer ,i1 ,j1,i2,j2);
           }
      }
      }
      else {
            // now if the player entered 5 characters so he wants to make promotion that he want to replace the
            // pawn when it reach the most north raw (according to each player) with other powerful piece of
            // the same kind of the player army
            // now what we do is for each player we make some steps of validation to do this promotion process
            // at first we check if the indexes that the player entered is valid on the board or not using check_valid() function
            // then if correct we check other four things and they are :-
            // 1) check if the player choose a pawn piece to move
            // 2) check if this move reach this pawn to the most north raw (according to each player)
            // 3) check if this move is valid or not (according to type of pawn)
            // 4) check if the player entered valid piece that is replaced with his pawn
        if(!(check_valid(input))){
             message("Error you entered invalid indexes . try again :");
             take_the_move(pointer);
        }
        else {
                parse_input(input,&i1,&j1,&i2,&j2);
           //  printf("%c %c %d  %d  %d \n\n",*(pointer + i1*8 + j1),
              //      *(input+4),i2,validate(pointer,i1,j1,i2,j2),check_valid_promotion_piece(*(input+4),current_player));
                    if((current_player == 2 && *(pointer + i1*8 + j1) == 'p' && i2 == 0 && validate(pointer,i1,j1,i2,j2) && check_valid_promotion_piece(*(input+4),current_player)) ||
                       (current_player == 1 && *(pointer + i1*8 + j1) == 'P' && i2 == 7 && validate(pointer,i1,j1,i2,j2) && check_valid_promotion_piece(*(input+4),current_player)) ){
                           // if all the checks are correct  so do the promotion process
                           // but first we must replace the pawn with the new piece
                           *(pointer + i1*8 + j1) =  *(input+4);
                             promotion = 1;
                             make_move(pointer,i1,j1,i2,j2);
                     }
                     // if the promotion is incorrect for any other reason
                     else{
                        promotion = 0;
                        message("Error you entered invalid indexes Or invalid promotion character . try again :");
                        // take new move
                        take_the_move(pointer);
                     }

        }
        // here must be promotion function
      }

}


int check_valid(char *string){
     return ((*(string) >= 'A' && *(string) <= 'H' )
           &&(*(string+2) >= 'A' && *(string+2) <= 'H')
           &&(*(string+1) >= '1' && *(string+1) <= '8')
           &&(*(string+3) >= '1' && *(string+3) <= '8'))? 1 : 0;
}

void parse_input(char *string , int* i1 , int* j1 , int* i2 , int* j2){
      *j1 =  *(string) - 'A';
      *i1 = '8' - *(string+1);
      *j2 = *(string+2) - 'A';
      *i2 = '8' - *(string+3);
}
int validate(char *pointer,int i1,int j1,int i2,int j2){
     // validate the first index
     if( (*(pointer + 8*i1 + j1) == '0' ) ||
     (*(pointer + 8*i1 + j1) >= 'a' && *(pointer + 8*i1 + j1) <= 'z' && current_player == 1) ||
     (*(pointer + 8*i1 + j1) >= 'A' && *(pointer + 8*i1 + j1) <= 'Z' && current_player == 2) ||
     (*(pointer + 8*i2 + j2) >= 'A' && *(pointer + 8*i2 + j2) <= 'Z' && current_player == 1) ||
     (*(pointer + 8*i2 + j2) >= 'a' && *(pointer + 8*i2 + j2) <= 'z' && current_player == 2)){
        printf("%c %c %d %d %d %d\n",*(pointer + 8*i1 + j1),*(pointer + 8*i2 + j2),i1,j1,i2,j2);
         printf("%c%c is invalid cell to play\n",'A'+j1 , '1'+i1);
         return 0;
     }
     int valid_type_result = 1;
    // printf("%c %c\n",*(pointer + 8*i1 + j1),*(pointer + 8*i2 + j2));
     char cell_char = *(pointer + i1*8 +j1);
     if(cell_char == 'p' || cell_char == 'P'){
            // validation pawn movement
        valid_type_result = validate_pawn(pointer,i1, j1,i2,j2);
     }else if(cell_char == 'r' || cell_char == 'R'){
           // validation rook movement
        valid_type_result = validate_rook(pointer,i1, j1,i2,j2);
     }
     else if(cell_char == 'n' || cell_char == 'N'){
         // validation knight movement
        valid_type_result = validate_knight(pointer,i1,j1,i2,j2);
     }
     else if(cell_char == 'b' || cell_char == 'B'){
         // validation bishop movement
        valid_type_result = validate_bishop(pointer,i1, j1,i2,j2);
     }
     else if(cell_char == 'q' || cell_char == 'Q'){
         // validation queen movement
        valid_type_result = validate_queen(pointer,i1, j1,i2,j2);
     }
     else if(cell_char == 'k' || cell_char == 'K'){
         // validation king movement
        valid_type_result = validate_king(pointer,i1, j1,i2,j2);
     }

     // return if the movement is valid or not
     return valid_type_result;
}

// validation of all movement of the chess pieces


int validate_pawn(char *pointer,int  i1,int j1, int i2, int j2){
    // check if the move will happen in the same raw
                                           // printf("%d %d %d %d\n",i1,j1,i2,j2);
      if(i1 == i2){
        message("Error pawn must n't move horizontally Try again : ");
        return 0;
      }
      // if first player move up
      if(current_player == 1 && i1>i2){
        message("Error you must move down Try again : ");
        return 0;
      }
      // if second player move down
      if(current_player == 2 && i1<i2){
        message("Error you must move up Try again : ");
        return 0;
      }

      char c = *(pointer + 8*i2 + j2);
                                                                                      // printf("%c\n",c);
      // here if the wanted cell is free so the only possible way is that the column
      //is the same and the difference between the two raws is 1 or 2
      //in the first move of the pawn
      if(c == '0'){
           if(j1 == j2){
               if(abs(i1-i2) == 1){
                                            //    printf("YES***************************\n");
               return 1;
               }

               else if(current_player == 1 && i1 == 1 && i2 == 3 && get_char(pointer,2,j1)=='0') return 1;
               else if(current_player == 2 && i1 == 6 && i2 == 4 && get_char(pointer,5,j1)=='0') return 1;
               else {
                    message("Error you can't move to this cell Try again : ");
                    return 0;
               }
           }
           else{
            message("Error you can't move to this cell Try again : \n");
            return 0;
           }
      }

              // contain other type of pieces

             // test if the piece of the second player is located in the next diagonally place in the grid
      if(abs(i2-i1) == 1 && abs(j1-j2) == 1){
            return 1;
            }

      else{
            message("Error you can't move to this cell Try again : \n");
            return 0;
    }
   return 0; // do nothing
}

// validate rook
int validate_rook(char *pointer,int  i1,int j1, int i2, int j2){

         if(i1 == i2  && j1!= j2){
                   if(!is_empty_horz(pointer , i1,j1,i2,j2)){
                      message("Error you can't move to this cell Try again : ");
                      return 0;
                   }
                   return 1;
         }

         if(j1 == j2 && i1 != i2){
                   if(!is_empty_vert(pointer , i1,j1,i2,j2)){
                      message("Error you can't move to this cell Try again : ");
                      return 0;
                   }
                   return 1;
         }
             message("Error you can't move to this cell Try again : ");
             return 0;
}

// validate knight
int validate_knight(char *pointer,int  i1,int j1, int i2, int j2){
    if(abs(i1-i2)== 2 && abs(j1-j2)==1){
        return 1;
    }
    if(abs(i1-i2) == 1 && abs(j2-j1)==2){
        return 1;
    }
    message("Error you can't move to this cell try again : ");
    return 0;
}

// validate bishops  move diagonally
int validate_bishop(char *pointer,int  i1,int j1, int i2, int j2){
      // check if index not diagonally
     if(abs(i1-i2) != abs(j1-j2)){
        message("Error you can't move to this cell (must diagonally )Try again : ");
        return 0;
     }
     if(!is_empty_diag(pointer,i1,j1,i2,j2)){
        message("Error you can't move to this cell (must diagonally )Try again : ");
        return 0;
     }
     return 1;
}

// validation move of queen [ vertical , horizontal , diagonal ]
int validate_queen(char *pointer,int  i1,int j1, int i2, int j2){
     // check for horizontal

     if(i1==i2){  // horizontal
          if(!is_empty_horz(pointer,i1,j1,i2,j2)){
                message("Error you can't move to this cell Try again : ");
                return 0;
          }
         return 1;
     }

     if(j1 == j2){  // vertical
         if(!is_empty_vert(pointer,i1,j1,i2,j2)){
                message("Error you can't move to this cell Try again : ");
                return 0;
         }
         // all is empty
         return 1;
     }

     // to check diagonally

         if(!is_empty_diag(pointer,i1,j1,i2,j2)){
                 message("Error you can't move to this cell Try again : ");
                return 0;
         }
     return 1;

}
int validate_king(char *pointer,int  i1,int j1, int i2, int j2){
      int raw_diff = abs(j1-j2) , col_diff = abs(i1-i2);
      if(abs(raw_diff - col_diff) <= 1) return 1;
      //invalid indexes (not one_place in move)
      message("Error you can't move to this cell Try again : ");
      return 0;

}

// make the move

void make_move(char *pointer ,int i1 ,int j1,int i2,int j2){
     int curr = 0;
      char c1 = *(pointer+ i1 * 8 + j1);
      char c2 = *(pointer + i2*8 + j2);
      *(pointer + i2*8 + j2) = c1;
      *(pointer + i1* 8 + j1) = '0';

      if(check_king(pointer) == current_player){
                     if(current_player == 1){
                         message("player 1 is checked\n");
                     }else{
                        message("player 2 is checked\n");
                     }
                     if(promotion == 1){
                         promotion = 0;
                         if(current_player == 1) c1 = 'P';
                         else c1 = 'p';
                     }
                              *(pointer + i2*8 + j2) = c2;
                              *(pointer + i1* 8 + j1) = c1;
                                take_the_move(pointer);

      }
      else{
                 // so any player play correct move (if he checked he must git from this check)
                 // here we know that the move is valid so the moved cell will be '0'
                 // and the wanted cell is replaced so we replace the wanted cell to check if it contain a piece of the other player army
                 replace_cell(c2);
                 // now when every play make_new move all the redo steps must be removed because anew game will be inserted
                 if(redo_ == 1){
                   remove_nodes();
                 }
                 // after we replace the cells and removed the other player's piece if existed we must save this step to validate (redo and undo)
                 save_step(pointer , i1 , j1 , c1 , i2 , j2 , c2 , current_player);
                 undo_ = 1;  // when we add new step to the linked list so we can do undo
                 redo_ = 0;  // when we add new step to the linked list so we can not do redo

      }
}


// replace the removed cell
void replace_cell(int c2){
    if(c2 != '0'){
        if(c2 >= 'A' && c2 <= 'Z'){
            add_remove_piece(c2,1);
        }
        else{
            add_remove_piece(c2,2);
        }
    }
}

// save the step in the linked list that carrying  the steps to make undo and redo
void save_step(char *pointer , int i1 , int j1 ,char c1 , int i2 , int j2 ,char c2 , int last_player){
        // when we save step we save it in auto_file
        add_node(pointer,i1,j1,c1,i2,j2,c2,last_player);  // will create new node and add it to the linked list
}
//check if the game - check - checkmate - stale





// save , exit , redo , undo
void save(char *pointer){
     // first remove auto_file
     remove_auto_file();

    //creat pointer to the file
    FILE* saveload;

    //Open the file for reading + add save -->> to appened the file
    saveload = fopen("saveload.txt","a");

    if(saveload == NULL)
    {
        return;
    }

        int i;
        int j;

        for(i=0;i<8;i++)
            for(j=0;j<8;j++)
                fputc(get_char(pointer,i,j),saveload);

        for(i=0;i<16;i++)
                fputc(player1_remove[i],saveload);

        for(i=0;i<16;i++)
                fputc(player2_remove[i],saveload);

        //player
        fputc(current_player,saveload);

        //keep the player
        current_player = (current_player == 1)?2:1;

    fclose(saveload);
    remove_auto_file();
}


void redo(char *pointer){

    if(redo_ == 0){
        printf("you can't redo \n");
        return;
    }
          // here we do the same thing that we did in undo process
          // first this function didn't called if the redo process can not be validated
          // and that by using redo_ variable
          // now we will also extract data from the next node after the current node
          // (here we use the next current node because this node carry the data that happened before we made undo process)
          // set the node next current
        struct step_node *node = current->next;
        // first extract the data from the current node and make update process
        int i1 = node->i1 , j1 = node->j1 , i2 = node->i2 , j2 = node->j2 , last_play = node->last_player , prom = node->prom;
        char c1 = node->c1 , c2 = node->c2 ;

        // update the grid
        *(pointer + 8* i1 + j1) = '0';
        *(pointer + 8* i2 + j2) = c1;

        // change the players
       /* if(current_player = 1) current_player = 2;
        else current_player = 1;*/

        // update remove_arrays
        if(c2 != '0'){
            if(c2 >= 'A' && c2 <= 'Z'){
                add_remove_piece(c2,1);
            }
            else{
                add_remove_piece(c2,2);
            }

        }

        // update linked_list
        current = node;
        if(current == last){
                printf("yes2\n\n");
             redo_ = 0; // can't make redo anymore
        }
        undo_ = 1; // can make undo to the last step
}


void undo(char *pointer){
         // here the player will make undo process so what happen is :-
         // first we will see if there is valid to make undo or not (or check if the players reach the new_game again)
         // then if there is valid undo , we know that we now stop at current node of the linked_list of the steps of  the game
         // and too every node contains the indexes and the data and the last_player of the last game
         // all we need to reset these last data and update the original tools (the grid , the player who must play ,
         //           , check the possibility of undo again , update the linked_list node by moving to the previous
         //            node if possible , update the remove_arrays of each player according to the kind of piece
         //            and this by removing the last element in the array of the player whose piece is removed)


         // first extract the data from the current node and make update process
        int i1 = current->i1 , j1 = current->j1 , i2 = current->i2 , j2 = current->j2 , last_play = current->last_player , prom = current->prom;
        char c1 = current->c1 , c2 = current->c2 ;
        // update the grid
        if(prom == 1){
            if(last_play == 1) c1 = 'P';
            else c1 = 'p';
        }
        *(pointer + 8* i1 + j1) = c1;
        *(pointer + 8* i2 + j2) = c2;

        // change the players
       /* if(current_player = 1) current_player = 2;
        else current_player = 1;*/

        // check to remove_arrays
        if(c2 != '0'){
        if(c2 >= 'A' && c2 <= 'Z'){
             remove_removed_piece(1);
        }
        else{
             remove_removed_piece(2);
        }
        }
         // now we will check if it is valid to make undo again or not
         if(current -> pre == head ){  // if we stop at the first , in other words we can do (if current == head ) we will have the same mean
                    undo_ = 0;
         }
       // here we set current node to the previous node
          current  = current->pre;
          redo_ = 1; // when we make undo so the redo is valid
}



//print message to the user
void message(char *mes){
    printf("%s",mes);
}

//Get string by the user and return pointer
char* get_string(){

      char *string = malloc(255); int  i = 0;
      char c = getchar();
      while(c != '\n' && i < 255){
        string[i] = c;
        c = getchar();
        i++;
      }
      while (c != '\n')
        c = getchar();

      string[i] = '\0';
      return string;
}

//Get and check the name from the user and assign it to user name
//Take the two users names

void take_names(){
    message("enter names (pls names don't exceed 10 characters and don't start with digits) \n");
    message("Player one : ");
    take_name(1);
    message("player two : ");
    take_name(2);
    while(check_name_matches()){
        message("Error the two names are the same ... try again .\n");
        message("Player one : ");
        take_name(1);
        message("player two : ");
        take_name(2);
    }
}


void take_name(int i){
   int count = 0 , flag = 0;
   char c = getchar(); char name[10];
   while( c != '\n'){
        if(count == 0 && c >= '0' && c<= '9'){
            flag = 1;
        }
        if(count < 10){
            name[count] = c;
        }
        count++;
        c = getchar();
   }
   if(c == '\n' && count == 0){
      message("Error you entered empty string ... enter again : ");
      take_name(i);
   }
   if(flag == 1){
     message("Error name  begin with a digit ...enter again : ");
     take_name(i);
   }
   if(flag == 0 && count > 10){
      message("Error name exceeds 10 letters ... enter again :  ");
      take_name(i);
   }
   if(flag == 0 && count <= 10 ){
        if(i == 1){
       for(i=0;i<count;i++){
             player1_name[i] = name[i];
       }
         player1_name[count] = '\0';
    }
         else if( i == 2 ){
            for(i=0;i<count;i++){
             player2_name[i] = name[i];
       }
             player2_name[count] = '\0';
         }
   }
}

//check for equality in names
int check_name_matches(){
    int i = 0 , j = 0;
    while(player1_name[i] != '\0'){
        if(player1_name[i] == player2_name[i]) j++;
        i++;
    }
    if(i==j && player2_name[i]=='\0') return 1;
    return 0;
}


void show_options_1(char *pointer){
    int choose  ;
    message("Main menu : \n");
    message("        1- New game    2- load game \n");
    message("        enter a choice : ");
    choose = get_int();
    while(choose != 1 && choose != 2){
        message("Error you must choose 1 or 2 \n");
        choose  = get_int();
    }
    // here after the player enter valid choice
    if(choose == 1) new_game(pointer);
    if(choose == 2)
        {
            int stat = load(pointer);
            if(stat == 1)
            {
                printf("No file Exist !");
                set_grid(pointer);
            }
            else if (stat == 2 || stat == 3)
            {
                printf("File has been corrupted !");
                set_grid(pointer);
            }
        }

}

//load game form file
int load(char *pointer){

    //creat pointer to the file
    FILE* saveload;

    //Open the file for reading + add save -->> to appened the file
    saveload = fopen("saveload.txt","r");

    if(saveload == NULL)
    {
        return 1;
    }

        // n = 65 -->> 64+1
        char buff[65];
        int chars = 0;
        int saving = 0;

        //Get how many chars are there ?
        while(fgetc(saveload) != EOF)
            {
                chars++;
                if(chars == 97)
                {
                    saving++;
                    chars -= 97;
                }
            }

        //check if the file in corrupted in more data
        if(chars > 0)
        {
            fclose(saveload);
            return 2;
        }


        //Make the user decide which one he want
        int choosen;

        do
        {
            printf("There is %i save in the file , \"1\" for the first and \"%i\" for the last: ",saving,saving);
            choosen = get_int();
        }while(choosen > saving || choosen < 1);

        //Fseek will change the pointer of the file to the wanted save
        //saveload -->> is the FILE*
        //(choosen-1)*64 -->> is the equation that calculate the start of the grid
        //SEEK_SET is a unique identify in fseek to the start of the file
        fseek(saveload,(choosen-1)*97,SEEK_SET);

        //it'll get us the wanted save
        fgets(buff, 65, saveload);
        int i;
        int j;

        for (i=0;i<64;i++)
            switch (buff[i])
            {
                case '0':
                case 'r':
                case 'n':
                case 'p':
                case 'b':
                case 'q':
                case 'k':
                case 'R':
                case 'N':
                case 'P':
                case 'B':
                case 'Q':
                case 'K':
                    break;
                default:
                    fclose(saveload);
                    return 3;
            }

        remove_all_games();
        undo_ = 0;
        redo_ = 0;

        //assign the data in the grid
        for(i=0;i<8;i++)
            for(j=0;j<8;j++)
                set_char(pointer,i,j,buff[i*8+j]);

        for(i=0;i<16;i++)
            player1_remove[i] = fgetc(saveload);


        for(i=0;i<16;i++)
            player2_remove[i] = fgetc(saveload);

            current_player = fgetc(saveload);

            if(load1 == 1) current_player = (current_player == 1)?2:1;
    //Close the file
    fclose(saveload);
}

// start new games
void new_game(char *pointer){
    // set_grid , set removes arrayes , set_new_nodes
     set_grid(pointer);
     set_remove_arraies();
}

void copy_grid(char *p ,char *pointer){
     //printf("%s\n",p);
      int i,j,k = 0;
       for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            *(pointer + i*8 + j) = *(p+k);
             k++;
         }
       }
     //  printf("\n\n%s\n",pointer);
}

int check_in_files(){

}

int get_int(){
  int i , error = 1;
  float f;
  char c;
  while(error){
        scanf("%f",&f);
        error = 1;
         if(f- (int)f != 0) error = 2;
         while(getchar() != '\n'){
              error = 3;
         }
         error = (error==1)?0:error;
         if(error == 2){
             message("error you can't enter double\n");
         }
         if(error == 3){
             message("error integer can't contain characters\n");
         }

  }
  return (int)f;
}

void show_options_2(){
    int i = 2;
   message("choose :-  \n 1- save 2- load ");
   if(computer == 0){
   if(redo_ && undo_){
      message("3- redo  4- undo\n");
   }
   if(redo_ && !undo_){
       message("3-redo\n");
   }
   if(!redo_ && undo_){
      message("3-undo\n");
   }
   }
   message("  Or enter the move :\n");
}

// to check is there is no_pieces between two pieces

// first to check vertically
int is_empty_vert(char *pointer , int i1 ,int j1 , int i2 , int j2){
     // we know that j1 == j2
     int i , min_i = (i1<i2)?i1:i2 , max_i = (i1>i2)?i1:i2;
     for(i = min_i+1 ; i< max_i; i++){
            // if there is a piece between these two pieces
         if( *(pointer+ i*8 + j1) != '0' ) return 0;
     }
     // there is all empty
     return 1;

}
// second to check horizontally
int is_empty_horz(char *pointer , int i1 ,int j1 , int i2 , int j2){
     // here i1 == i2
     int j , min_j = (j1<j2)?j1:j2 , max_j = (j1>j2)?j1:j2;
     for(j=min_j+1 ; j<max_j ; j++){
         if(*(pointer + i1*8 + j) != '0') return 0;
     }
     return 1;
}
// third to check diagonally
int is_empty_diag(char *pointer , int i1 ,int j1 , int i2 , int j2){
     // check for empty route
     int min_j = (j1<j2)?j1:j2  ,max_j = (j1<j2)?j2:j1 , i , j , k;
     // set the start of the diagonal to check
     if(j1<j2) {
            if(i1<i2){
                i = i1+1;
                k = 1;
            }
            else{
                i = i1-1;
                k = 0;
            }
     }
     if(j1>j2){
        if(i1<i2){
            i = i2-1;
            k = 0;
        }
        else{
            i = i2+1;
            k = 1;
        }
     }
     for(j=min_j+1 ; j<max_j ; j++){
         if(*(pointer + i*8 + j) != '0') return 0;
         if(k==1) i++;
         else i--;
     }
     return 1;
}

// this function is used in case of new play to set all removed pieces in the two arrays to '-'
void set_remove_arraies(){
    int i = 0;
     for(i;i<16;i++){
          player1_remove[i] = '-';
          player2_remove[i] = '-';
     }
}

// this function is used in case of load the game from the file in this case we must load the two arrays
// of the removed pieces to reuse in the current game
void copy_remove_arrays(char *remove1 , char *remove2){
    int i ;
    for(i=0;i,16;i++){
        player1_remove[i] = *(remove1+i);
        player2_remove[i] = *(remove2+i);
    }
}

// this function used in case of add removed piece of the player or in case of redo function when
// this redo game contained a remove piece in specific array but this removed piece was removed from
//its array during the undo process
void add_remove_piece(char c2,int id){
      int i;
      if(id == 1){
        for(i=0;i<16;i++){
            if(player1_remove[i] == '-'){
                player1_remove[i] = c2;
                return ;
            }
        }
      }
      else {
        for(i=0;i<16;i++){
            if(player2_remove[i] == '-'){
                player2_remove[i] = c2;
                return ;
            }
        }
      }
}

// this function will use in undo function to remove any removed pieces in the last game from the arrays  of the two players of removed pieces
void remove_removed_piece(int id){
     int i;
     if(id == 1){
        for(i=0;i<16;i++){
            if(player1_remove[i] == '-') break;
        }
        player1_remove[i-1] = '-';
     }
     if(id == 2){
        for(i=0;i<16;i++){
            if(player2_remove[i] == '-') break;
        }
        player2_remove[i-1] = '-';
     }
}
void set_new_nodes(){
    head = (struct step_node*)malloc(sizeof(struct step_node));
    head->pre = NULL;
    head->next= NULL;
    current = head;
    last = head;
}

void add_node(char *pointer,int i1,int j1,char c1,int i2,int j2,char c2,int last_player){
      struct  step_node *node = create_new_node(pointer,i1,j1,c1,i2,j2,c2,last_player);
        node->pre  =  last;
        last->next =  node;
        last = node;
        current = last;
}

struct step_node* create_new_node(char *pointer,int i1,int j1,char c1,int i2,int j2,char c2,int last_player){
     struct step_node* node  = (struct step_node*)malloc(sizeof(struct step_node));
      node->i1 = i1;
      node->j1 = j1;
      node->i2 = i2;
      node->j2 = j2;
      node->c1 = c1;
      node->c2 = c2;
      node->last_player = last_player;
      node->prom = promotion;
      node->next = NULL;
      node->pre = NULL;
      return node;
}

// this function is used if the player make undo then enter new move so all redo moves will be removed
void remove_nodes(){
     // now the undo stops in the current node
     // so we need to remove all node after current to last node
     // we will use free function to remove every node from the heap

    struct step_node *node = current->next;
    last = current;
    struct step_node *removed_node;
     while(node != NULL){
            removed_node = node;
            node = node->next;
            free(removed_node);
     }
    current = last;
    current->next = NULL;
}

// this function is used to check if the player  entered a valid character that
// represented the piece that he want to replace with his pawn
int check_valid_promotion_piece(char c,int player){
     if(player == 1){  // now current player is playing
        if(c == 'R' || c == 'N' || c == 'B' || c == 'Q') return 1;
        return 0;
     }
      // now player 2 is playing
         if(c == 'r' || c == 'n' || c == 'b' || c == 'q') return 1;
         return 0;
}


void remove_all_games(){
      struct step_node *node = head;
      struct step_node *remove;
      while(node != NULL){
        remove  = node;
        node = node->next;
        free(remove);
      }

}

//fares stuff

//check if the king is died ?
int checkmate(char* pointer){

        int flag1 = 0;
        int flag2 = 0;

    if(check_king(pointer) == 1 || check_king(pointer) == 3)
    {
    flag1 = 1;
    //Get I,J of the KING
    int i,j;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
        {
            if(get_char(pointer,i,j) == 'K')
                break;
        }

        int king_i=i;
        int king_j=j;
        for(i=0;i<8;i++)
        {
            for(j=0;j<8;j++)
            {
                if(get_char(pointer,i,j) >= 'A' && get_char(pointer,i,j) <= 'Z')
                {
                    move *list;
                    list = NULL;
                    check_move(pointer,i,j,&list);
                    move* curs = list;
                    while(curs != NULL)
                    {
                        char comefrom = get_char(pointer,i,j);
                        char goingto = get_char(pointer,curs->i,curs->j);

                        set_char(pointer,curs->i,curs->j,comefrom);
                        set_char(pointer,i,j,'0');

                        if(check_king(pointer) != 1 && check_king(pointer) != 3)
                        {
                            flag1 = 0;
                        }
                        set_char(pointer,i,j,comefrom);
                        set_char(pointer,curs->i,curs->j,goingto);
                        move* free1 = curs;
                        curs = curs->next;
                        free(free1);
                    }
                }
            }
        }
    }
    else if(check_king(pointer) == 2 || check_king(pointer) == 3)
    {
    flag2 = 1;
    //Get I,J of the KING
    int i,j;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
        {
            if(get_char(pointer,i,j) == 'k')
                break;
        }

        int king_i=i;
        int king_j=j;

        for(i=0;i<8;i++)
        {
            for(j=0;j<8;j++)
            {
                if(get_char(pointer,i,j) > 96 && get_char(pointer,i,j) < 123)
                {
                    move *list;
                    list = NULL;
                    check_move(pointer,i,j,&list);

                    move* curs = list;
                    while(curs != NULL)
                    {
                        char comefrom = get_char(pointer,i,j);
                        char goingto = get_char(pointer,curs->i,curs->j);

                        set_char(pointer,curs->i,curs->j,comefrom);
                        set_char(pointer,i,j,'0');

                        if(check_king(pointer) != 2 && check_king(pointer) != 3)
                        {
                            flag2 = 0;
                        }

                        set_char(pointer,i,j,comefrom);
                        set_char(pointer,curs->i,curs->j,goingto);
                        move* free1 = curs;
                        curs = curs->next;
                        free(free1);
                    }
                }
            }
        }
    }

    if(flag1)
        return 1;
    if(flag2)
        return 2;
    return 0;
}


int check_stalemate(char* pointer)
{
    int flag = 1;
    int i,j;
    if(current_player == 1)
    {
        for(i=0;i<8;i++)
        {
            for(j=0;j<8;j++)
            {
                if(get_char(pointer,i,j) >= 'A' && get_char(pointer,i,j) <= 'Z')
                {
                    move *list;
                    list = NULL;
                    check_move(pointer,i,j,&list);

                    move* curs = list;
                    while(curs != NULL)
                    {

                        char comefrom = get_char(pointer,i,j);
                        char goingto = get_char(pointer,curs->i,curs->j);

                        set_char(pointer,curs->i,curs->j,comefrom);
                        set_char(pointer,i,j,'0');

                        if(check_king(pointer) != 1 && check_king(pointer) != 3)
                        {
                            flag = 0;
                        }

                        set_char(pointer,i,j,comefrom);
                        set_char(pointer,curs->i,curs->j,goingto);
                        move* free1 = curs;
                        curs = curs->next;
                        free(free1);

                    }
                    if(flag == 0)
                        return 1;
                }
            }
        }
    }else
    {
        for(i=0;i<8;i++)
        {
            for(j=0;j<8;j++)
            {
                if(get_char(pointer,i,j) >= 'a' && get_char(pointer,i,j) <= 'z')
                {
                    move *list;
                    list = NULL;
                    check_move(pointer,i,j,&list);

                    move* curs = list;
                    while(curs != NULL)
                    {
                        char comefrom = get_char(pointer,i,j);
                        char goingto = get_char(pointer,curs->i,curs->j);

                        set_char(pointer,curs->i,curs->j,comefrom);
                        set_char(pointer,i,j,'0');

                        if(check_king(pointer) != 2 && check_king(pointer) != 3)
                        {
                            flag = 0;
                        }

                        set_char(pointer,i,j,comefrom);
                        set_char(pointer,curs->i,curs->j,goingto);
                        move* free1 = curs;
                        curs = curs->next;
                        free(free1);
                    }
                    if(flag == 0)
                        return 2;
                }
            }
        }
    }

    return 0;
}
//Check if the king is in danger or not ?
int check_king(char* pointer){

    int check1 = 0;
    int check2 = 0;

    int i,j;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
        {
            if(get_char(pointer,i,j) == 'K')
                if(check(pointer,i,j))
                    check1 = 1;
            if(get_char(pointer,i,j) == 'k')
                if(check(pointer,i,j))
                    check2 = 2;
        }

        if(check1 && check2)
            return 3;
        else if(check1)
            return 1;
        else if (check2)
            return 2;

        return 0;
}

//check item Ith and Jth is in danger or not
int check(char* pointer,int i,int j)
{
    int x = direction_x_check(pointer,i,j);
    int y = direction_y_check(pointer,i,j);
    int d = direction_diagonale_check(pointer,i,j);
    int h = horse_check(pointer,i,j);

    if(x || y || d || h)
        return 1;

    return 0;
}

//Check if the king is in stalemate
int check_stalemate_king(char* pointer)
{
    int i,j;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
        {

            //Stalemate for player one
            if(get_char(pointer,i,j) == 'K')
            {
                int result = 0;
                int count = 0;
                //check if in X direction there is danger
                if(i+1 < 8)
                    {
                        if(get_char(pointer,i+1,j) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i+1,j,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i+1,j);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i+1,j,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }
                if(i-1 > 0)
                    {
                        if(get_char(pointer,i-1,j) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i-1,j,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i-1,j);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i-1,j,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }
                //check if in y direction there is danger
                if(j+1 < 8)
                    {
                        if(get_char(pointer,i,j+1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i,j+1,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i,j+1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i,j+1,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }
                if(j-1 > 0)
                    {
                        if(get_char(pointer,i,j-1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i,j-1,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i,j-1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i,j-1,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }

                //check if in diaganol direction there is danger
                if(j+1 < 8 && i+1 < 8)
                    {
                        if(get_char(pointer,i+1,j+1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i+1,j+1,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i+1,j+1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i+1,j+1,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }
                if(j-1 > 0 && i-1 > 0)
                    {
                        if(get_char(pointer,i-1,j-1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i-1,j-1,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i-1,j-1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i-1,j-1,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }
                if(j-1 >1 && i+1 < 8)
                    {
                        if(get_char(pointer,i+1,j-1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i+1,j-1,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i+1,j-1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i+1,j-1,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }
                if(j+1 < 8 && i-1 > 0)
                    {
                        if(get_char(pointer,i-1,j+1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i-1,j+1,'K');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i-1,j+1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i-1,j+1,'0');
                            set_char(pointer,i,j,'K');
                        }
                    }

                //Check if it's stalemate
                if(count == result && count > 0)
                {
                    return 1;
                }
            }

            //Stalemate for player two
            if(get_char(pointer,i,j) == 'k')
            {
                int result = 0;
                int count = 0;

                //check if in X direction there is danger
                if(i+1 < 8)
                    {
                        if(get_char(pointer,i+1,j) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i+1,j,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i+1,j);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i+1,j,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }
                if(i-1 > 0)
                    {
                        if(get_char(pointer,i-1,j) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i-1,j,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i-1,j);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i-1,j,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }
                //check if in y direction there is danger
                if(j+1 < 8)
                    {
                        if(get_char(pointer,i,j+1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i,j+1,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i,j+1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i,j+1,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }
                if(j-1 > 0)
                    {
                        if(get_char(pointer,i,j-1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i,j-1,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i,j-1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i,j-1,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }

                //check if in diaganol direction there is danger
                if(j+1 < 8 && i+1 < 8)
                    {
                        if(get_char(pointer,i+1,j+1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i+1,j+1,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i+1,j+1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i+1,j+1,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }
                if(j-1 > 0 && i-1 > 0)
                    {
                        if(get_char(pointer,i-1,j-1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i-1,j-1,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i-1,j-1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i-1,j-1,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }

                if(j-1 >1 && i+1 < 8)
                    {
                        if(get_char(pointer,i+1,j-1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i+1,j-1,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i+1,j-1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i+1,j-1,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }

                if(j+1 < 8 && i-1 > 0)
                    {
                        if(get_char(pointer,i-1,j+1) == '0')
                        {
                            //change the place of the king
                            set_char(pointer,i-1,j+1,'k');
                            set_char(pointer,i,j,'0');
                            //check if he will die
                            result += check(pointer,i-1,j+1);
                            count++;
                            //re-change the place of the king
                            set_char(pointer,i-1,j+1,'0');
                            set_char(pointer,i,j,'k');
                        }
                    }

                //Check if it's stalemate

                if(count == result && count > 0)
                {
                    return 2;
                }
            }


        }
        return 0;
}


//Check if there is danger in X direction
int direction_x_check(char* pointer,int i , int j)
{
    //var for determine the piece player
    int player;

    //determine the range of the piece
    if(get_char(pointer,i,j) > 64 && get_char(pointer,i,j) < 91)
        player = 1;
    else if (get_char(pointer,i,j) > 96 && get_char(pointer,i,j) < 123)
        player = 2;
    else
        return 0;

    int m;
    int flag = 0;
    //check first if there is a first piece from the same kind in every direction so => it's safe
    int safe_positive = 0;
    int safe_negative = 0;

    //check negative direction
    for(m=j-1;m>-1;m--)
    {
        char c;
        if(get_char(pointer,i,m) != '0')
        {
            flag = 1;
            c = get_char(pointer,i,m);
            if(c > 64 && c < 91 && player == 1)
            {
                safe_negative = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_negative = 1;
                break;
            }
            break;
        }
    }

    //check negative
     if(!safe_negative && flag)
     {
        char c = get_char(pointer,i,m);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'r':
                    return 1;
                case 'q':
                    return 1;
                case 'k':
                    if(m == j-1 && player == 1)
                        return 1;
                    break;
                case 'R':
                    return 1;
                case 'Q':
                    return 1;
                case 'K':
                    if(m == j-1 && player == 2)
                        return 1;
                    break;
            }

    }

    flag = 0;
    //check positive direction
    for (m=j+1;m<8;m++)
    {
        char c;
        if(get_char(pointer,i,m) != '0')
        {
            flag = 1;
            c = get_char(pointer,i,m);
            if(c > 64 && c < 91 && player == 1)
            {
                safe_positive = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_positive = 1;
                break;
            }
            break;
        }
    }

    //check negative
     if(!safe_positive && flag)
     {
        char c = get_char(pointer,i,m);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'r':
                    return 1;
                case 'q':
                    return 1;
                case 'k':
                    if(m == j+1 && player == 1)
                        return 1;
                    break;
                case 'R':
                    return 1;
                case 'Q':
                    return 1;
                case 'K':
                    if(m == j+1  && player == 2)
                        return 1;
                    break;
            }

    }

    //else it's piece of enemy but it's safe !
    return 0;

}
//Check if there is danger in Y direction
int direction_y_check(char* pointer,int i , int j)
{
    //var for determine the piece player
    int player;

    //determine the range of the piece
    if(get_char(pointer,i,j) > 64 && get_char(pointer,i,j) < 91)
        player = 1;
    else if (get_char(pointer,i,j) > 96 && get_char(pointer,i,j) < 123)
        player = 2;
    else
        return 0;

    int m;
    int flag = 0;
    //check first if there is a first piece from the same kind in every direction so => it's safe
    int safe_positive = 0;
    int safe_negative = 0;

    //check positive direction
    for(m=i-1;m>-1;m--)
    {
        char c;
        if(get_char(pointer,m,j) != '0')
        {
            flag = 1;
            c = get_char(pointer,m,j);
            if(c > 64 && c < 91 && player == 1)
            {
                safe_positive = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_positive = 1;
                break;
            }
            break;
        }
    }

    //check positive
     if(!safe_positive && flag)
     {
        char c = get_char(pointer,m,j);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'r':
                    return 1;
                case 'q':
                    return 1;
                case 'k':
                    if(m == i-1 && player == 1)
                        return 1;
                    break;
                case 'R':
                    return 1;
                case 'Q':
                    return 1;
                case 'K':
                    if(m == i-1 && player == 2)
                        return 1;
                    break;
            }

    }

    flag = 0;
    //check negative direction
    for (m=i+1;m<8;m++)
    {
        char c;
        if(get_char(pointer,m,j) != '0')
        {
            flag = 1;
            c = get_char(pointer,m,j);
            if(c > 64 && c < 91 && player == 1)
            {
                safe_negative = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_negative = 1;
                break;
            }
            break;
        }
    }

    //check negative
     if(!safe_negative && flag)
     {
        char c = get_char(pointer,m,j);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'r':
                    return 1;
                case 'q':
                    return 1;
                case 'k':
                    if(m == i+1 && player == 1)
                        return 1;
                    break;
                case 'R':
                    return 1;
                case 'Q':
                    return 1;
                case 'K':
                    if(m == i+1 && player == 2)
                        return 1;
                    break;
            }

    }

    //else it's piece of enemy but it's safe !
    return 0;

}
//Check if there is danger in Diagnol direction
int direction_diagonale_check(char* pointer,int i , int j)
{
   //var for determine the piece player
    int player;

    //determine the range of the piece
    if(get_char(pointer,i,j) > 64 && get_char(pointer,i,j) < 91)
        player = 1;
    else if (get_char(pointer,i,j) > 96 && get_char(pointer,i,j) < 123)
        player = 2;
    else
        return 0;

    int m ;
    int flag;

    //check first if there is a first piece from the same kind in every direction so => it's safe
    int safe_left_above = 0;
    int safe_left_down = 0;

    int safe_right_above = 0;
    int safe_right_down = 0;

    m = 1;
    flag =0;
    //check left apove
    while(i-m >= 0 && j-m >= 0)
    {
        char c;
        if(get_char(pointer,i-m,j-m) != '0')
        {
            flag = 1;
            c = get_char(pointer,i-m,j-m);
            if(c > 64 && c < 91 && player == 1)
            {
                safe_left_above = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_left_above = 1;
                break;
            }
            break;
        }
        m++;
    }
    //check first enemy
     if(!safe_left_above && flag)
     {
        char c = get_char(pointer,i-m,j-m);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'q':
                case 'b':
                case 'Q':
                case 'B':
                    return 1;

                case 'k':
                case 'p':
                    if(m == 1 && player == 1)
                        return 1;
                    break;
                case 'P':
                case 'K':
                    if(m == 1 && player == 2)
                        return 1;
                    break;
            }
    }

    m = 1;
    flag =0;
    //check right down
    while(i+m <= 7 && j+m <= 7)
    {
        char c;
        if(get_char(pointer,i+m,j+m) != '0')
        {
            flag = 1;
            c = get_char(pointer,i+m,j+m);
            if(c > 64 && c < 91 && player == 1)
            {
                safe_right_down = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_right_down = 1;
                break;
            }
            break;
        }
        m++;
    }
    //check first enemy
     if(!safe_right_down && flag)
     {
        char c = get_char(pointer,i+m,j+m);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'q':
                case 'b':
                case 'Q':
                case 'B':
                    return 1;

                case 'k':
                case 'p':
                    if(m == 1 && player == 1)
                        return 1;
                    break;
                case 'P':
                case 'K':
                    if(m == 1 && player == 2)
                        return 1;
                    break;
            }
    }


    m = 1;
    flag =0;
    //check right above
    while(i-m >= 0 && j+m <= 7)
    {
        char c;
        if(get_char(pointer,i-m,j+m) != '0')
        {
            flag = 1;
            c = get_char(pointer,i-m,j+m);
            if(c > 64 && c < 91 && player == 1)
            {
                safe_right_above = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_right_above = 1;
                break;
            }
            break;
        }
        m++;
    }
    //check first enemy
     if(!safe_right_above && flag)
     {
        char c = get_char(pointer,i-m,j+m);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'q':
                case 'b':
                case 'Q':
                case 'B':
                    return 1;

                case 'k':
                case 'p':
                    if(m == 1 && player == 1)
                        return 1;
                    break;
                case 'P':
                case 'K':
                    if(m == 1 && player == 2)
                        return 1;
                    break;
            }
    }

    m = 1;
    flag =0;
    //check left down
    while(i+m <= 7 && j-m >= 0)
    {
        char c;
        if(get_char(pointer,i+m,j-m) != '0')
        {
            flag = 1;
            c = get_char(pointer,i+m,j-m);
            if(c > 64 && c < 91 &&player == 1)
            {
                safe_left_down = 1;
                break;
            }

            else if(c > 96 && c < 123 && player == 2)
            {
                safe_left_down = 1;
                break;
            }
            break;
        }
        m++;
    }
    //check first enemy
     if(!safe_left_down && flag)
     {
        char c = get_char(pointer,i+m,j-m);
            //check for a lose
            switch (c)
            {
                // he will die
                case 'q':
                case 'b':
                case 'Q':
                case 'B':
                    return 1;

                case 'k':
                case 'p':
                    if(m == 1 && player == 1)
                        return 1;
                    break;
                case 'P':
                case 'K':
                    if(m == 1 && player == 2)
                        return 1;
                    break;
            }

    }

    //else it's piece of enemy but it's safe !
    return 0;

}
//Check if there is danger by horse
int horse_check(char* pointer,int i , int j)
{
    //var for determine the piece player
    int player;
    char c;

    //determine the range of the piece
    if(get_char(pointer,i,j) > 64 && get_char(pointer,i,j) < 91)
        player = 1;
    else if (get_char(pointer,i,j) > 96 && get_char(pointer,i,j) < 123)
        player = 2;
    else
        return 0;

            //check the ABOVE section
            if(i-2 >= 0)
            {
                if(j-1 >= 0)
                {
                    c = get_char(pointer,i-2,j-1);
                    if(player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
                if(j+1 <=7)
                {
                    c = get_char(pointer,i-2,j+1);
                    if(player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
            }

            //check the UNDER section
            if(i+2 <= 7)
            {
                if(j-1 >= 0)
                {
                    c = get_char(pointer,i+2,j-1);
                    if(player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
                if(j+1 <=7)
                {
                    c = get_char(pointer,i+2,j+1);
                    if(c > 64 && c < 91 && player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
            }

            //check the RIGHT section
            if(j+2 <= 7)
            {
                if(i-1 >= 0)
                {
                    c = get_char(pointer,i-1,j+2);
                    if(player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
                if(i+1 <=7)
                {
                    c = get_char(pointer,i+1,j+2);
                    if(player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
            }

            //check the left section
            if(j-2 >= 0)
            {
                if(i-1 >= 0)
                {
                    c = get_char(pointer,i-1,j-2);
                    if(player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
                if(i+1 <=7)
                {
                    c = get_char(pointer,i+1,j-2);
                    if(player == 2 && c == 'N')
                    {
                        return 1;
                    }
                    if(player == 1 && c == 'n')
                    {
                        return 1;
                    }
                }
            }

    return 0;
}


//check for movement
void check_move(char* pointer,int i, int j,move **list)
{

    //var for determine the piece player
    int player;
    //determine the range of the piece
    if(get_char(pointer,i,j) > 64 && get_char(pointer,i,j) < 91)
        player = 1;
    else if (get_char(pointer,i,j) > 96 && get_char(pointer,i,j) < 123)
        player = 2;
    else
        return;


    char c = get_char(pointer,i,j);

    int m = 1,n = 0;
    switch (c)
    {
        case 'P':
        case 'p':
             (check_move_p(pointer,i,j,list));
             return;
        case 'Q':
        case 'q':
             (check_move_q(pointer,i,j,list));
             return;
        case 'R':
        case 'r':
             (check_move_r(pointer,i,j,list));
             return;
        case 'K':
        case 'k':
             (check_move_k(pointer,i,j,list));
             return;
        case 'B':
        case 'b':
             (check_move_b(pointer,i,j,list));
             return;
        case 'N':
        case 'n':
             (check_move_n(pointer,i,j,list));
             return;
    }
}

//SubCheck
//check for movement in p and P
void check_move_p(char* pointer,int i, int j,move **list)
{


    char c = get_char(pointer,i,j);

    int m = 1;
    switch (c)
    {
        case 'P':
            //check for one step ahead
            if((i+m) <= 7 && get_char(pointer,i+m,j) == '0')
                {
                    list_assign(list,i+m,j);
                }
            //check for two step ahead
            if(get_char(pointer,i+2,j) == '0' && get_char(pointer,i+m,j) == '0' && i == 1)
                {
                    list_assign(list,i+2,j);
                }
            //check for one step ahed-right
            if(i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+1) > 96 && get_char(pointer,i+m,j+1) < 123)
                {
                    list_assign(list,i+m,j+m);
                }
            //check for one step ahead left
            if(i+m <= 7 && j-1 >= 0 && get_char(pointer,i+m,j-1) > 96 && get_char(pointer,i+m,j-1) < 123)
                {
                    list_assign(list,i+m,j-m);
                }
return;
        case 'p':
            //check for two step ahead
            if(get_char(pointer,i-2,j) == '0' && i == 6 && get_char(pointer,i-m,j) == '0')
                {
                    list_assign(list,i-2,j);
                }
            //check for one step ahead
            if ((i-m) >= 0 && get_char(pointer,i-m,j) == '0')
            {
                    list_assign(list,i-m,j);
            }
            //check for one step ahed-right
            if(i-m >= 0 && j+1 <= 7 && get_char(pointer,i-m,j+1) > 64 && get_char(pointer,i-m,j+1) < 91)
                {
                    list_assign(list,i-m,j+1);
                }
            //check for one step ahead left
            if(i-m >= 0 && j-1 >= 0 && get_char(pointer,i-m,j-1) > 64 && get_char(pointer,i-1,j-1) < 91)
                {
                    list_assign(list,i-m,j-m);
                }

    }
}
//check for movement in q and Q
void check_move_q(char* pointer,int i, int j,move **list)
{


    char c = get_char(pointer,i,j);

    int m = 1;

    switch (c)
    {
        case 'Q':
            //check for move in right
                while (j+m <= 7 && get_char(pointer,i,j+m) == '0')
                {
                    list_assign(list,i,j+m);
                    m++;
                }
                //check if the last piece in right is enemy
                if(j+m <= 7 && get_char(pointer,i,j+m) > 96 && get_char(pointer,i,j+m) < 123)
                {
                    list_assign(list,i,j+m);
                }

            m = 1;
            //check for move in left
                while (j-m >= 0 && get_char(pointer,i,j-m) == '0')
                {
                    list_assign(list,i,j-m);
                    m++;
                }
                //check if the last piece in left is enemy
                if(j-m >= 0 && get_char(pointer,i,j-m) > 96 && get_char(pointer,i,j-m) < 123)
                {
                    list_assign(list,i,j-m);
                }

            m = 1;
            //check for move in upwords
                while (i-m >= 0 && get_char(pointer,i-m,j) == '0')
                {
                    list_assign(list,i-m,j);
                    m++;
                }
                //check if the last piece in upwords is enemy
                if(i-m >= 0 && get_char(pointer,i-m,j) > 96 && get_char(pointer,i-m,j) < 123)
                {
                    list_assign(list,i-m,j);
                }

            m = 1;
            //check for move in downwords
                while (i+m <= 7 && get_char(pointer,i+m,j) == '0')
                {
                    list_assign(list,i+m,j);
                    m++;
                }
                //check if the last piece in downwords is enemy
                if(i+m <= 7 && get_char(pointer,i+m,j) > 96 && get_char(pointer,i+m,j) < 123)
                {
                    list_assign(list,i+m,j);
                }

            m = 1;
            //check for move in left-above
                while (i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) == '0')
                {
                    list_assign(list,i-m,j-m);
                    m++;
                }
                //check if the last piece in left-above is enemy
                if(i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) > 96 && get_char(pointer,i-m,j-m) < 123)
                {
                    list_assign(list,i-m,j-m);
                }

            m = 1;
            //check for move in right-down
                while (i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) == '0')
                {
                    list_assign(list,i+m,j+m);
                    m++;
                }
                //check if the last piece in right-down is enemy
                if(i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) > 96 && get_char(pointer,i+m,j+m) < 123)
                {
                    list_assign(list,i+m,j+m);
                }

            m = 1;
            //check for move in right-above
                while (i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) == '0')
                {
                    list_assign(list,i-m,j+m);
                    m++;
                }
                //check if the last piece in right-above is enemy
                if(i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) > 96 && get_char(pointer,i-m,j+m) < 123)
                {
                    list_assign(list,i-m,j+m);
                }

            m = 1;
            //check for move in left-down
                while (i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) == '0')
                {
                    list_assign(list,i+m,j-m);
                    m++;
                }
                //check if the last piece in left-down is enemy
                if(i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) > 96 && get_char(pointer,i+m,j-m) < 123)
                {
                    list_assign(list,i+m,j-m);
                }

return;
        case 'q':

            //check for move in right
                while (j+m <= 7 && get_char(pointer,i,j+m) == '0')
                {
                    list_assign(list,i,j+m);
                    m++;
                }
                //check if the last piece in right is enemy
                if(j+m <= 7 && get_char(pointer,i,j+m) > 64 && get_char(pointer,i,j+m) < 91)
                {
                    list_assign(list,i,j+m);
                }

            m = 1;
            //check for move in left
                while (j-m >= 0 && get_char(pointer,i,j-m) == '0')
                {
                    list_assign(list,i,j-m);
                    m++;
                }
                //check if the last piece in left is enemy
                if(j-m >= 0 && get_char(pointer,i,j-m) > 64 && get_char(pointer,i,j-m) < 91)
                {
                    list_assign(list,i,j-m);
                }

            m = 1;
            //check for move in upwords
                while (i-m >= 0 && get_char(pointer,i-m,j) == '0')
                {
                    list_assign(list,i-m,j);
                    m++;
                }
                //check if the last piece in upwords is enemy
                if(i-m >= 0 && get_char(pointer,i-m,j) > 64 && get_char(pointer,i-m,j) < 91)
                {
                    list_assign(list,i-m,j);
                }

            m = 1;
            //check for move in downwords
                while (i+m <= 7 && get_char(pointer,i+m,j) == '0')
                {
                    list_assign(list,i+m,j);
                    m++;
                }
                //check if the last piece in downwords is enemy
                if(i+m <= 7 && get_char(pointer,i+m,j) > 64 && get_char(pointer,i+m,j) < 91)
                {
                    list_assign(list,i+m,j);
                }

            m = 1;
            //check for move in left-above
                while (i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) == '0')
                {
                    list_assign(list,i-m,j-m);
                    m++;
                }
                //check if the last piece in left-above is enemy
                if(i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) > 64 && get_char(pointer,i-m,j-m) < 91)
                {
                    list_assign(list,i-m,j-m);
                }

            m = 1;
            //check for move in right-down
                while (i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) == '0')
                {
                    list_assign(list,i+m,j+m);
                    m++;
                }
                //check if the last piece in right-down is enemy
                if(i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) > 64 && get_char(pointer,i+m,j+m) < 91)
                {
                    list_assign(list,i+m,j+m);
                }

            m = 1;
            //check for move in right-above
                while (i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) == '0')
                {
                    list_assign(list,i-m,j+m);
                    m++;
                }
                //check if the last piece in right-above is enemy
                if(i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) > 64 && get_char(pointer,i-m,j+m) < 91)
                {
                    list_assign(list,i-m,j+m);
                }

            m = 1;
            //check for move in left-down
                while (i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) == '0')
                {
                    list_assign(list,i+m,j-m);
                    m++;
                }
                //check if the last piece in left-down is enemy
                if(i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) > 64 && get_char(pointer,i+m,j-m) < 91)
                {
                    list_assign(list,i+m,j-m);
                }

    }
}
//check for movement in r and R
void check_move_r(char* pointer,int i, int j,move **list)
{

    char c = get_char(pointer,i,j);

    int m = 1;

    switch (c)
    {
        case 'R':
            //check for move in right
                while (j+m <= 7 && get_char(pointer,i,j+m) == '0')
                {
                    list_assign(list,i,j+m);
                    m++;
                }
                //check if the last piece in right is enemy
                if(j+m <= 7 && get_char(pointer,i,j+m) > 96 && get_char(pointer,i,j+m) < 123)
                {
                    list_assign(list,i,j+m);
                }

            m = 1;
            //check for move in left
                while (j-m >= 0 && get_char(pointer,i,j-m) == '0')
                {
                    list_assign(list,i,j-m);
                    m++;
                }
                //check if the last piece in left is enemy
                if(j-m >= 0 && get_char(pointer,i,j-m) > 96 && get_char(pointer,i,j-m) < 123)
                {
                    list_assign(list,i,j-m);
                }

            m = 1;
            //check for move in upwords
                while (i-m >= 0 && get_char(pointer,i-m,j) == '0')
                {
                    list_assign(list,i-m,j);
                    m++;
                }
                //check if the last piece in upwords is enemy
                if(i-m >= 0 && get_char(pointer,i-m,j) > 96 && get_char(pointer,i-m,j) < 123)
                {
                    list_assign(list,i-m,j);
                }

            m = 1;
            //check for move in downwords
                while (i+m <= 7 && get_char(pointer,i+m,j) == '0')
                {
                    list_assign(list,i+m,j);
                    m++;
                }
                //check if the last piece in downwords is enemy
                if(i+m <= 7 && get_char(pointer,i+m,j) > 96 && get_char(pointer,i+m,j) < 123)
                {
                    list_assign(list,i+m,j);
                }
return;
        case 'r':

            //check for move in right
                while (j+m <= 7 && get_char(pointer,i,j+m) == '0')
                {
                    list_assign(list,i,j+m);
                    m++;
                }
                //check if the last piece in right is enemy
                if(j+m <= 7 && get_char(pointer,i,j+m) > 64 && get_char(pointer,i,j+m) < 91)
                {
                    list_assign(list,i,j+m);
                }

            m = 1;
            //check for move in left
                while (j-m >= 0 && get_char(pointer,i,j-m) == '0')
                {
                    list_assign(list,i,j-m);
                    m++;
                }
                //check if the last piece in left is enemy
                if(j-m >= 0 && get_char(pointer,i,j-m) > 64 && get_char(pointer,i,j-m) < 91)
                {
                    list_assign(list,i,j-m);
                }

            m = 1;
            //check for move in upwords
                while (i-m >= 0 && get_char(pointer,i-m,j) == '0')
                {
                    list_assign(list,i-m,j);
                    m++;
                }
                //check if the last piece in upwords is enemy
                if(i-m >= 0 && get_char(pointer,i-m,j) > 64 && get_char(pointer,i-m,j) < 91)
                {
                    list_assign(list,i-m,j);
                }

            m = 1;
            //check for move in downwords
                while (i+m <= 7 && get_char(pointer,i+m,j) == '0')
                {
                    list_assign(list,i+m,j);
                    m++;
                }
                //check if the last piece in downwords is enemy
                if(i+m <= 7 && get_char(pointer,i+m,j) > 64 && get_char(pointer,i+m,j) < 91)
                {
                    list_assign(list,i+m,j);
                }
    }
}
//check for movement in k and K
void check_move_k(char* pointer,int i, int j,move **list){


    char c = get_char(pointer,i,j);

    int m = 1;

    switch (c)
    {
        case 'K':
            //check downside
            if(i+1 <=7)
            {
                if( get_char(pointer,i+1,j) == '0' || (get_char(pointer,i+1,j) > 96 && get_char(pointer,i+1,j) < 123))
                {
                    list_assign(list,i+1,j);
                }
                if(j+1 <= 7 && (get_char(pointer,i+1,j+1) == '0' || (get_char(pointer,i+1,j+1) > 96 && get_char(pointer,i+1,j+1) < 123)))
                {
                    list_assign(list,i+1,j+1);
                }
                if(j-1 >= 0 && (get_char(pointer,i+1,j-1) == '0' || (get_char(pointer,i+1,j-1) > 96 && get_char(pointer,i+1,j-1) < 123)))
                {
                    list_assign(list,i+1,j-1);
                }
            }
            //check upperside
            if(i-1 >= 0)
            {
                if( get_char(pointer,i-1,j) == '0' || (get_char(pointer,i-1,j) > 96 && get_char(pointer,i-1,j) < 123))
                {
                    list_assign(list,i-1,j);
                }
                if(j+1 <= 7 && (get_char(pointer,i-1,j+1) == '0' || (get_char(pointer,i-1,j+1) > 96 && get_char(pointer,i-1,j+1) < 123)))
                {
                    list_assign(list,i-1,j+1);
                }
                if(j-1 >= 0 && (get_char(pointer,i-1,j-1) == '0' || (get_char(pointer,i-1,j-1) > 96 && get_char(pointer,i-1,j-1) < 123)))
                {
                    list_assign(list,i-1,j-1);
                }
            }
            //check right
            if(j-1 >= 0)
            {
                if( get_char(pointer,i,j-1) == '0' || (get_char(pointer,i,j-1) > 96 && get_char(pointer,i,j-1) < 123))
                {
                    list_assign(list,i,j-1);
                }
            }
            //check left
            if(j+1 <= 7)
            {
                if( get_char(pointer,i,j+1) == '0' || (get_char(pointer,i,j+1) > 96 && get_char(pointer,i,j+1) < 123))
                {
                    list_assign(list,i,j+1);
                }
            }
return;
        case 'k':
            //check downside
            if(i+1 <=7)
            {
                if( get_char(pointer,i+1,j) == '0' || (get_char(pointer,i+1,j) > 64 && get_char(pointer,i+1,j) < 91))
                {
                    list_assign(list,i+1,j);
                }
                if(j+1 <= 7 && (get_char(pointer,i+1,j+1) == '0' || (get_char(pointer,i+1,j+1) > 64 && get_char(pointer,i+1,j+1) < 91)))
                {
                    list_assign(list,i+1,j+1);
                }
                if(j-1 >= 0 && (get_char(pointer,i+1,j-1) == '0' || (get_char(pointer,i+1,j-1) > 64 && get_char(pointer,i+1,j-1) < 91)))
                {
                    list_assign(list,i+1,j-1);
                }
            }
            //check upperside
            if(i-1 >= 0)
            {
                if( get_char(pointer,i-1,j) == '0' || (get_char(pointer,i-1,j) > 64 && get_char(pointer,i-1,j) < 91))
                {
                    list_assign(list,i-1,j);
                }
                if(j+1 <= 7 && (get_char(pointer,i-1,j+1) == '0' || (get_char(pointer,i-1,j+1) > 64 && get_char(pointer,i-1,j+1) < 91)))
                {
                    list_assign(list,i-1,j+1);
                }
                if(j-1 >= 0 && (get_char(pointer,i-1,j-1) == '0' || (get_char(pointer,i-1,j-1) > 64 && get_char(pointer,i-1,j-1) < 91)))
                {
                    list_assign(list,i-1,j-1);
                }
            }
            //check right
            if(j-1 >= 0)
            {
                if( get_char(pointer,i,j-1) == '0' || (get_char(pointer,i,j-1) > 64 && get_char(pointer,i,j-1) < 91))
                {
                    list_assign(list,i,j-1);
                }
            }
            //check left
            if(j+1 <= 7)
            {
                if( get_char(pointer,i,j+1) == '0' || (get_char(pointer,i,j+1) > 64 && get_char(pointer,i,j+1) < 91))
                {
                    list_assign(list,i,j+1);
                }
            }
    }
}
//check for movement in b and B
void check_move_b(char* pointer,int i, int j,move **list){

    char c = get_char(pointer,i,j);

    int m = 1;

    switch (c)
    {
        case 'B':
            m = 1;
            //check for move in left-above
                while (i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) == '0')
                {
                    list_assign(list,i-m,j-m);
                    m++;
                }
                //check if the last piece in left-above is enemy
                if(i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) > 96 && get_char(pointer,i-m,j-m) < 123)
                {
                    list_assign(list,i-m,j-m);
                }

            m = 1;
            //check for move in right-down
                while (i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) == '0')
                {
                    list_assign(list,i+m,j+m);
                    m++;
                }
                //check if the last piece in right-down is enemy
                if(i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) > 96 && get_char(pointer,i+m,j+m) < 123)
                {
                    list_assign(list,i+m,j+m);
                }

            m = 1;
            //check for move in right-above
                while (i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) == '0')
                {
                    list_assign(list,i-m,j+m);
                    m++;
                }
                //check if the last piece in right-above is enemy
                if(i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) > 96 && get_char(pointer,i-m,j+m) < 123)
                {
                    list_assign(list,i-m,j+m);
                }

            m = 1;
            //check for move in left-down
                while (i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) == '0')
                {
                    list_assign(list,i+m,j-m);
                    m++;
                }
                //check if the last piece in left-down is enemy
                if(i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) > 96 && get_char(pointer,i+m,j-m) < 123)
                {
                    list_assign(list,i+m,j-m);
                }
return;
        case 'b':

            m = 1;
            //check for move in left-above
                while (i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) == '0')
                {
                    list_assign(list,i-m,j-m);
                    m++;
                }
                //check if the last piece in left-above is enemy
                if(i-m >= 0 && j-m >= 0 && get_char(pointer,i-m,j-m) > 64 && get_char(pointer,i-m,j-m) < 91)
                {
                    list_assign(list,i-m,j-m);
                }

            m = 1;
            //check for move in right-down
                while (i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) == '0')
                {
                    list_assign(list,i+m,j+m);
                    m++;
                }
                //check if the last piece in right-down is enemy
                if(i+m <= 7 && j+m <= 7 && get_char(pointer,i+m,j+m) > 64 && get_char(pointer,i+m,j+m) < 91)
                {
                    list_assign(list,i+m,j+m);
                }

            m = 1;
            //check for move in right-above
                while (i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) == '0')
                {
                    list_assign(list,i-m,j+m);
                    m++;
                }
                //check if the last piece in right-above is enemy
                if(i-m >= 0 && j+m <= 7 && get_char(pointer,i-m,j+m) > 64 && get_char(pointer,i-m,j+m) < 91)
                {
                    list_assign(list,i-m,j+m);
                }

            m = 1;
            //check for move in left-down
                while (i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) == '0')
                {
                    list_assign(list,i+m,j-m);
                    m++;
                }
                //check if the last piece in left-down is enemy
                if(i+m <= 7 && j-m >= 0 && get_char(pointer,i+m,j-m) > 64 && get_char(pointer,i+m,j-m) < 91)
                {
                    list_assign(list,i+m,j-m);
                }

    }
}
//check for movement in n and N
void check_move_n(char* pointer,int i, int j,move **list){



    char c = get_char(pointer,i,j);

    int m = 1;

    switch (c)
    {
        case 'N':
            //check the ABOVE section
            if(i-2 >= 0)
            {
                if(j-1 >= 0)
                {
                    c = get_char(pointer,i-2,j-1);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i-2,j-1);
                    }
                }
                if(j+1 <=7)
                {
                    c = get_char(pointer,i-2,j+1);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i-2,j+1);
                    }
                }
            }

            //check the UNDER section
            if(i+2 <= 7)
            {
                if(j-1 >= 0)
                {
                    c = get_char(pointer,i+2,j-1);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i+2,j-1);
                    }
                }
                if(j+1 <=7)
                {
                    c = get_char(pointer,i+2,j+1);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i+2,j+1);
                    }
                }
            }

            //check the RIGHT section
            if(j+2 <= 7)
            {
                if(i-1 >= 0)
                {
                    c = get_char(pointer,i-1,j+2);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i-1,j+2);
                    }
                }
                if(i+1 <=7)
                {
                    c = get_char(pointer,i+1,j+2);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i+1,j+2);
                    }
                }
            }

            //check the RIGHT section
            if(j-2 >= 0)
            {
                if(i-1 >= 0)
                {
                    c = get_char(pointer,i-1,j-2);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i-1,j-2);
                    }
                }
                if(i+1 <=7)
                {
                    c = get_char(pointer,i+1,j-2);
                    if((c > 96 && c < 123) || c == '0')
                    {
                        list_assign(list,i+1,j-2);
                    }
                }
            }
            return;
        case 'n':
            //check the ABOVE section
            if(i-2 >= 0)
            {

                if(j-1 >= 0)
                {

                    c = get_char(pointer,i-2,j-1);
                    if((c > 64 && c < 91) || c == '0')
                    {
                        list_assign(list,i-2,j-1);
                    }
                }
                if(j+1 <=7)
                {
                    c = get_char(pointer,i-2,j+1);
                    if((c > 64 && c < 91) || c == '0')
                    {
                        list_assign(list,i-2,j+1);
                    }
                }
            }

            //check the UNDER section
            if(i+2 <= 7)
            {
                if(j-1 >= 0)
                {
                    c = get_char(pointer,i+2,j-1);
                    if((c > 64 && c < 91) || c == '0')
                    {
                        list_assign(list,i+2,j-1);
                    }
                }
                if(j+1 <=7)
                {
                    c = get_char(pointer,i+2,j+1);
                    if((c > 64 && c < 91) || c == '0')
                    {
                        list_assign(list,i+2,j+1);
                    }
                }
            }

            //check the RIGHT section
            if(j+2 <= 7)
            {
                if(i-1 >= 0)
                {
                    c = get_char(pointer,i-1,j+2);
                    if((c > 64 && c < 91) || c == '0')
                    {

                        list_assign(list,i-1,j+2);
                    }
                }
                if(i+1 <=7)
                {
                    c = get_char(pointer,i+1,j+2);
                    if((c > 64 && c < 91) || c == '0')
                    {
                        list_assign(list,i+1,j+2);
                    }
                }
            }

            //check the RIGHT section
            if(j-2 >= 0)
            {
                if(i-1 >= 0)
                {
                    c = get_char(pointer,i-1,j-2);
                    if((c > 64 && c < 91) || c == '0')
                    {
                        list_assign(list,i-1,j-2);
                    }
                }
                if(i+1 <=7)
                {
                    c = get_char(pointer,i+1,j-2);
                    if((c > 64 && c < 91) || c == '0')
                    {
                        list_assign(list,i+1,j-2);
                    }
                }
            }

    }
}
//assign to the list of movemement
void list_assign(move** list,int i,int j)
{
    //printf("N%i\n",(int)*list);
    move* node = malloc(sizeof(move));
    node->next = *list;
    *list = node;
    node->i=i;
    node->j=j;

}

int check_for_status(char *pointer){

    if(check_king(pointer)){
    {
    if(checkmate(pointer))
    return 1;
    }
}
    else
    {
        if(!check_stalemate(pointer))
            return 2;
    }

    return 0;
}



void computer_play(char* pointer)
{

    typedef struct computer{
        int i1, j1;
        int i2, j2;
        int result;
    } computer;


    //save the data in array
    computer comp[16*32*2];
    int counter = 0;

	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			if(get_char(pointer,i,j) >= 'a' && get_char(pointer,i,j) <= 'z')
			{
                    //get every move for the piece
                    move *list;
                    list = NULL;
                    check_move(pointer,i,j,&list);

                    move* curs = list;
                    while(curs != NULL)
                    {

                        //initial
                        comp[counter].result = 0;

                        //From
                        comp[counter].i1 = i;
                        comp[counter].j1 = j;

                        //To
                        comp[counter].i2 = curs->i;
                        comp[counter].j2 = curs->j;

                        //make the move
                        char comefrom = get_char(pointer,i,j);
                        char goingto = get_char(pointer,curs->i,curs->j);

                        //pluse for killing
                        if(goingto >= 'A' && goingto <= 'Z')
                            switch (goingto)
                            {
                                case 'K':
                                    comp[counter].result += 15*5*2;
                                    break;
                                case 'Q':
                                    comp[counter].result += 15*3*2;
                                    break;
                                case 'R':
                                case 'N':
                                case 'B':
                                    comp[counter].result += 15*2*2;
                                    break;
                                default:
                                    comp[counter].result += 15*2;
                            }



                        set_char(pointer,curs->i,curs->j,comefrom);
                        set_char(pointer,i,j,'0');


                        //check every piece else
                        int m,n;
                        for(m=0;m<8;m++)
                            for(n=0;n<8;n++)
                                if(get_char(pointer,m,n) != '0')
                                {
                                    int checky = check(pointer,m,n);

                                    switch (get_char(pointer,m,n))
                                    {
                                    case 'K':
                                        comp[counter].result += checky*5*2;
                                        break;
                                    case 'Q':
                                        comp[counter].result += checky*3*2;
                                        break;
                                    case 'R':
                                    case 'N':
                                    case 'B':
                                        comp[counter].result += checky*2*2;
                                        break;
                                    case 'P':
                                        comp[counter].result += checky*2;
                                        break;

                                    case 'k':
                                        comp[counter].result -= checky*5*2;
                                        break;
                                    case 'q':
                                        comp[counter].result -= checky*3*2;
                                        break;
                                    case 'r':
                                    case 'n':
                                    case 'b':
                                        comp[counter].result -= checky*2*2;
                                        break;
                                    case 'p':
                                        comp[counter].result -= checky*2;
                                        break;
                                    }
                                }

                                int checkking = check_king(pointer);
                                //check the kings
                                if(checkking == 1)
                                        comp[counter].result += 1000;
                                else if (checkking == 2)
                                        comp[counter].result -= 5000;


                        int counter1 = 0;
                        //check-one-play form user 1
                        for(m=0;m<8;m++)
                            for(n=0;n<8;n++)
                                if(get_char(pointer,m,n) >= 'A' && get_char(pointer,m,n) <= 'Z')
                                {
                                        //get every move for the piece
                                        move *list1;
                                        list1 = NULL;
                                        check_move(pointer,i,j,&list1);

                                        move* curs1 = list1;
                                        while(curs1 != NULL)
                                        {
                                            //make the move
                                            char comefrom1 = get_char(pointer,m,n);
                                            char goingto1 = get_char(pointer,curs1->i,curs1->j);

                                            set_char(pointer,curs1->i,curs1->j,comefrom1);
                                            set_char(pointer,m,n,'0');

                                    if(goingto1 >= 'a' && goingto1 <= 'z')
                                        switch (goingto1)
                                        {
                                            case 'k':
                                                comp[counter].result -= 15*5;
                                                break;
                                            case 'q':
                                                comp[counter].result -= 15*3;
                                                break;
                                            case 'r':
                                            case 'n':
                                            case 'b':
                                                comp[counter].result -= 15*2;
                                                break;
                                            default:
                                                comp[counter].result -= 15;
                                        }
                                        //check every piece else
                                        int d,f;
                                        for(d=0;d<8;d++)
                                            for(f=0;f<8;f++)
                                                if(get_char(pointer,d,f) != '0')
                                                {
                                                    int checky = check(pointer,d,f);
                                                    //check the pieces
                                                    switch (get_char(pointer,d,f))
                                                    {
                                                    case 'K':
                                                        comp[counter].result += checky*5;
                                                        break;
                                                    case 'Q':
                                                        comp[counter].result += checky*3;
                                                        break;
                                                    case 'R':
                                                    case 'N':
                                                    case 'B':
                                                        comp[counter].result += checky*2;
                                                        break;
                                                    case 'P':
                                                        comp[counter].result += checky;
                                                        break;

                                                    case 'k':
                                                        comp[counter].result -= checky*5;
                                                        break;
                                                    case 'q':
                                                        comp[counter].result -= checky*3;
                                                        break;
                                                    case 'r':
                                                    case 'n':
                                                    case 'b':
                                                        comp[counter].result -= checky*2;
                                                        break;
                                                    case 'p':
                                                        comp[counter].result -= checky
                                                        ;
                                                        break;
                                                    }
                                                }

                                                int checkking = check_king(pointer);
                                                //check the kings
                                                if(checkking == 1)
                                                        comp[counter].result += 1000;
                                                else if (checkking == 2)
                                                        comp[counter].result -= 1000;


                                            //unmake the move
                                            set_char(pointer,m,n,comefrom1);
                                            set_char(pointer,curs1->i,curs1->j,goingto1);

                                            move* free1 = curs;
                                            curs1 = curs1->next;
                                            free(free1);
                                            counter1++;
                                        }
                                }



                        //unmake the move
                        set_char(pointer,i,j,comefrom);
                        set_char(pointer,curs->i,curs->j,goingto);
                        move* free1 = curs;
                        curs = curs->next;
                        free(free1);
                        counter++;

                    }
			}

        int max = -1000000;
        int posi =0;

        for(i=0;i<counter;i++)
        {
            if(comp[i].result > max)
            {
                posi = i;
                max = comp[i].result;
            }
        }

        if(get_char(pointer,comp[posi].i2,comp[posi].j2) != '0')
            add_remove_piece(get_char(pointer,comp[posi].i2,comp[posi].j2),1);

        set_char(pointer,comp[posi].i2,comp[posi].j2,get_char(pointer,comp[posi].i1,comp[posi].j1));
        set_char(pointer,comp[posi].i1,comp[posi].j1,'0');

}
