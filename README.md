# Database
This is a database emulator written is C.

## Available commands:
- get-m - get the record from the master file 


**Usage**: get-m username 
- get-s - get the record from the slave file 


**Usage**: get-s username reaction 
- del-m - delete the record from the master file 


**Usage**: del-m username 
- del-s - delete the record from the slave file 


**Usage**: del-s username reaction 
- update-m - update the record in the master file 


**Usage**: update-m username email email@gmail.com 
- update-s - update the record in the slave file 


**Usage**: update-s username oldReaction type newReaction 
- insert-m - insert the record to the master file 


**Usage**: insert-m username email 
- insert-s - insert the record to the slave file 


**Usage**: insert-s username reaction 
- calc-m - show records count from the master file 


**Usage**: calc-m 
- calc-s - show records count from the slave file 


**Usage**: calc-s 
- ut-m - show all records from the master file 


**Usage**: ut-m 
- ut-s - show all records from the slave file 


**Usage**: ut-s username 
- exit - exit from database. 


**Usage**: exit 