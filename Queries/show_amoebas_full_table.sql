/*
Достает список подчиненных.
*/
SELECT user_id, user_surname, user_name, startdate_month, startdate_year, position, baserate 
FROM clerks_t 
INNER JOIN position_t USING(user_id) 
INNER JOIN baserate_t USING(position) 
INNER JOIN food_chain_t ON food_chain_t.clerk_id = clerks_t.user_id
WHERE boss_id=(SELECT user_id FROM clerks_t 
INNER JOIN position_t USING(user_id) 
INNER JOIN baserate_t USING(position) 
WHERE user_surname='Vasiliev' AND startdate_year=2003);