/*
Достает  только 'user_id'.
*/
SELECT user_id FROM clerks_t 
INNER JOIN position_t USING(user_id) 
INNER JOIN baserate_t USING(position) 
WHERE user_surname='Vasiliev' AND startdate_year=2003;