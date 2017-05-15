/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Dimasik
 *
 * Created on 17 февраля 2017 г., 22:43
 */

 
#include "myfuncs_h.h"


/*
 * 
 */


int main(int argc, char** argv) {

    sqlite3 *db;
    int err, clerk_id;
    struct clerk st_clerks_list[MAX_CLERCS];
//    struct clerk *st_clerks_list; // = calloc(MAX_CLERCS, sizeof(struct clerk)); // Работает, но сделаю чуть позже.
    int id_clerks_list[MAX_CLERCS] = {0};
    
    // Инициализируем массивы внутри структур.
    for (int i = 0; i < MAX_CLERCS; i++) {
        for (int k = 0; k < MAX_CLERCS; k++)
            st_clerks_list[i].depends[k] = 0;
    }
    
    err = sqlite3_open("example.db", &db);
    
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
        
    if (argc > 1 && !strcmp(argv[1], "--calc")) {
        clerk_id = atoi(argv[2]);
        get_all_clerks(clerk_id, id_clerks_list, db); 
        
        // Тут проходим по массиву и в соответствие с ним составляем
        // второй массив только с элементами в виде структур.
        for (int i = 0; id_clerks_list[i] != 0; i++) { 
            create_clerk(id_clerks_list[i], &st_clerks_list[i], db);
            
//            if ( !strcmp((char*)st_clerks_list[i].position, "Sales") ) {
//                get_all_clerks(id_clerks_list[i], st_clerks_list[i].depends, db);
//                lshift_arr(st_clerks_list[i].depends);
//            } 
        }
    }
    
    
    
    // DEBUG. Это так для проверки что всё зашло.
    printf("\n");
    for (int i = 0; id_clerks_list[i] != 0; i++) {
        printf( "%6d|%20s|%20s|%6d|%6d|%10s|%8d|%2d|->",
        st_clerks_list[i].id,
        st_clerks_list[i].surname,
        st_clerks_list[i].name,
        st_clerks_list[i].month,
        st_clerks_list[i].year,
        st_clerks_list[i].position,
        st_clerks_list[i].base_rate,
        st_clerks_list[i].boss_id);
        
        if ( strcmp((char*)st_clerks_list[i].position, "Employee") ) // Проверяем что это Sales.
            for (int k = 0; st_clerks_list[i].depends[k]; k++) { // 20 чтобы отбросить мусор неиницализированных элементов массива.
                printf("%3d", st_clerks_list[i].depends[k]); // Выводим список "с кого сдирать".
            }
    printf("\n");
    }
    // END_DEBUG.
    
    // DEBUG.
    printf("\n");
    printf("\n");
    int clerks[MAX_CLERCS] = {0};
    get_all_clerks(clerk_id, clerks, db);
    for (int i = 0; i < MAX_CLERCS; i++) {
        printf("%4d", clerks[i]);
    }
    printf("\n");
    // END_DEBUG.
    
    return (EXIT_SUCCESS);
}

