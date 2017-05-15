/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "myfuncs_h.h"

// Для удобочитаемости.///////////////////////////////////////
int iserror(int iserr) {                                    //
    if (iserr)                                              //
        printf("Error: %d\n", iserr);                       //
    return iserr;                                           //
}                                                           //
#define IF_ERROR(int_err) if (iserror(int_err)) return      //
//////////////////////////////////////////////////////////////

int queue(char *action, int value) {

    static int iqueue[MAX_CLERCS], position;

    if (!strcmp(action, "push")) {
        iqueue[++position] = value;
        return 0;
    }
    else if (!strcmp(action, "pop")) {
        if (position > 0)
            return iqueue[position--];
        else
            return -1;
    }
    else
        return 404;
}

int inarr(int* arr, int num) { 
    for (int i = 0; i < MAX_CLERCS; i++) 
        if (arr[i] == num)
            return 1;
    
    return 0;
}

void get_all_clerks(int mainID, int* clerks_arr, sqlite3 *db) {
    
    sqlite3_stmt *stmt;
    int err;
    char *query = {"SELECT clerk_id FROM food_chain_t WHERE boss_id=?"};
    
    //int nID, Gi;
    int nID;
    
    queue("push", mainID); // Помещаем первую вершину в очередь
    for (int i = 0; ( nID = queue("pop", 0) ) > 0; i++ ) { // Берем вершину из очереди, заодно проверяя что очередь не пуста.
        if ( !inarr(clerks_arr, nID) ) { // Проверяем что вершины нет в списке посещенных(онже список подчиненных).
           
            clerks_arr[i] = nID;    // Посещаем вершину т.е. добавляем в список посещенных.
            
            err = sqlite3_prepare(db, query, -1, &stmt, 0); // Запрос к базе.
            IF_ERROR(err);
            sqlite3_bind_int(stmt, 1, nID); // Вставляем в запрос ID текущей вершины.
            
            // Тут в условии цикла вызываем "sqlite3_step" 
            // которая, при каждом вызове переводит на следующую 
            // строку полученной таблицы.
            // Код возврата 100 значит что строка не NULL.
            while (sqlite3_step(stmt) == 100) { 
                int resp = sqlite3_column_int(stmt, 0);
                queue("push", resp);
            }
        }
    }
    //clerks_arr[Gi] = '/0'; // Детектор конца.
}

void get_branch_clerks(int mainID, int* clerks_arr, sqlite3 *db) {
    
    sqlite3_stmt *stmt;
    int err;
    char *query = {"SELECT clerk_id FROM food_chain_t WHERE boss_id=?"};
    
    err = sqlite3_prepare(db, query, -1, &stmt, 0); // Запрос к базе.
    IF_ERROR(err);
    sqlite3_bind_int(stmt, 1, mainID); // Вставляем в запрос ID текущей вершины.
    
    for (int i = 0; sqlite3_step(stmt) == 100; i++) {
        clerks_arr[i] = sqlite3_column_int(stmt, 0);
    }
}

int create_clerk(int clerk_id, struct clerk *dest, sqlite3 *db) {
    
    int rc;
    sqlite3_stmt *stmt;
    char *query = { "SELECT user_id, user_surname, user_name, startdate_month, "
                    "startdate_year, position, baserate, boss_id "
                    "FROM clerks_t "
                    "INNER JOIN position_t USING(user_id) "
                    "INNER JOIN baserate_t USING(position) "
                    "INNER JOIN food_chain_t ON clerks_t.user_id = food_chain_t.clerk_id "
                    "WHERE user_id=?"};
    
    // Запрос. 
    rc = sqlite3_prepare(db, query, -1, &stmt, 0);
    if (rc) {
        printf("Clerk not found or another error. Error number = %d.\n", rc);
        return rc;
    }  
    /* Тут прикрепляем число (clerk_id) к запросу.
     * Прикрепляется оно на место 1-го знака вопроса в строке запроса
     * т.е. "... user_id=?"  
     *                   ^
     * sqlite3_bind_int(stmt, 1, clerk_id) - 
     * stmt - хендл
     * 1 - позиция знака (по счету т.е. первый встретившийся.)
     * clerk_id - что вставлять(типа "int").      
     */ 
    sqlite3_bind_int(stmt, 1, clerk_id);
    sqlite3_step(stmt);
    
    // Заполняем данными структуру.
    dest->id = sqlite3_column_int(stmt, 0);
    dest->surname = sqlite3_column_text(stmt, 1);
    dest->name = sqlite3_column_text(stmt, 2);
    dest->month = sqlite3_column_int(stmt, 3);
    dest->year = sqlite3_column_int(stmt, 4);
    dest->position = sqlite3_column_text(stmt, 5);
    dest->base_rate = sqlite3_column_int(stmt, 6);
    dest->boss_id = sqlite3_column_int(stmt, 7);
    
    if ( !strcmp((char*)dest->position, "Sales") ) {
        get_all_clerks(dest->id, dest->depends, db);
        lshift_arr(dest->depends);
    }
    else if ( !strcmp((char*)dest->position, "Manager") ) {
        get_branch_clerks(dest->id, dest->depends, db);
    }
    
//    // DEBUG. Это так для проверки что всё зашло.
//    printf( "%6d|%20s|%20s|%6d|%6d|%10s|%8d|%2d\n",
//    dest->id,
//    dest->surname,
//    dest->name,
//    dest->month,
//    dest->year,
//    dest->position,
//    dest->base_rate,
//    dest->boss_id);
//    // END_DEBUG
    
    // sqlite3_reset и sqlite3_finalize тут делать ненадо иначе
    // развалятся текстовые поля в структурах.
//    sqlite3_reset(stmt);
//    sqlite3_finalize(stmt);
}

void lshift_arr(int* arr){
    for (int i = 0; i < MAX_CLERCS; i++) 
        arr[i] = arr[i+1];
    arr[MAX_CLERCS] = 0;
}

int count_of_users(sqlite3 *db) {
    char *query = {"SELECT COUNT(*) FROM clerks_t"};
    sqlite3_stmt *stmt;
    int ccount;
    
    sqlite3_prepare(db, query, -1, &stmt, 0);
    sqlite3_step(stmt);
    ccount = sqlite3_column_int(stmt, 0);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    return ccount;
}

void time_salary(struct clerk *pclerc) {
//    Заглушко.
}

int full_years(int start_month, int start_year, int month, int year) {
    int months_count;
    months_count = ((year * 12) + month) - ((start_year * 12) + start_month);
    return months_count % 12;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////_NOTES_//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Найти количество элементов массива - C (СИ)
//Если делать так, то все получается правильно. Выдает 5
//
//////_CODE_//////////////////////////
//#include <stdio.h>
//#include <conio.h>
// 
//int main(){
//    int u[]={1, 4, 1, 2, 8};
// 
//    printf("Kolichestvo elementov massiva %d\n", sizeof(u) / sizeof(u[0]));
// 
//    _getch();
//    return 0;
//}
//////_END_CODE_//////////////////////
//
//Но мне нужно задать функцию, т.е. пытаюсь сделать вот так
//////_CODE_//////////////////////////
//#include <stdio.h>
//#include <conio.h>
// 
//int degr(int a[]){
//    return sizeof(a) / sizeof(a[0]);
//  }
// 
//int main(){
//    int u[]={1, 4, 1, 2, 8};
// 
//    printf("Kolichestvo elementov massiva %d\n",degr(u));
// 
//    _getch();
//    return 0;
//}
//////_END_CODE_//////////////////////
//Получается 1.
//Помогите пожалуйста, задать функцию правильно.

//
//В функцию ты передаёшь указатель на первый элемент массива, а не весь массив, 
//поэтому sizeof(a)/sizeof(a[0]) ничто иное как sizeof(int*)/sizeof(int)=4/4=1
//

// http://www.cyberforum.ru/c-beginners/thread567265.html

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////_END_NOTES_//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
