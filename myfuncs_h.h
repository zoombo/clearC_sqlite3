/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   myfuncs_h.h
 * Author: Dimasik
 *
 * Created on 22 марта 2017 г., 12:38
 */

#ifndef MYFUNCS_H_H
#define MYFUNCS_H_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* MYFUNCS_H_H */

////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite/sqlite3.h>

#define MAX_CLERCS 20

// Структура, описывающая сотрудника.
struct clerk {
    int id;
    int boss_id;
    int depends[MAX_CLERCS];        // Массив, хранящий ID-шники подчиненных.
    int month, year;                // Месяц и год принятия на работу.
    int base_rate;                  // Базовая ставка.
    int final_rate;                 // Итоговая ставка(зарплата).
    const unsigned char *position;  // Позиция(должность) в компании.

    const unsigned char *name;
    const unsigned char *surname;
    // struct date_ym date;         // Дата поступления.
};  

// Не нашел функций для очередей так что сделаю пока сам.
int queue(char *action, int value);

// Функция проверяет есть-ли число "num" в массиве "arr".
int inarr(int* arr, int num);

// Сдвигает массив "array" на один элемент влево.
void lshift_arr(int* array);

// Загружает в массив "clerks_arr" всех подчиненных "mainID"
// реализует алгоритм обхода графа в ширину.
void get_all_clerks(int mainID, int* clerks_arr, sqlite3 *db);

// Загружает в массив "clerks_arr" всех непосредственных подчиненных "mainID".
// Это для сотрудников типа "Manager".
void get_branch_clerks(int mainID, int* clerks_arr, sqlite3 *db);

// Заполняет структуру типа "clerk".
int create_clerk(int clerk_id, struct clerk *dest, sqlite3 *db);

// Возвращает количество пользователей в БД.
int count_of_users(sqlite3 *db);

// Считает часть ЗП зависящую от времени работы.
void time_salary(struct clerk *);

// Возвращает количество полных лет.
int full_years(int start_month, int start_year, int month, int year);


