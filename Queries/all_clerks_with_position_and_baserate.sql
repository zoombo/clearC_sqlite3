/*
Выводит всех сотрудников с должностью в компании и базовой ставкой.
*/
SELECT * FROM clerks_t INNER JOIN position_t USING(user_id) 
INNER JOIN baserate_t USING(position);