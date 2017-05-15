/*
Достает список подчиненных. Список подчиненных в виде списка их id.
*/
SELECT clerk_id FROM food_chain_t WHERE boss_id=(SELECT user_id FROM clerks_t 
INNER JOIN position_t USING(user_id) 
INNER JOIN baserate_t USING(position) 
WHERE user_surname='Vasiliev' AND startdate_year=2003);