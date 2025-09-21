#   MIPS Assembly program that shows how to implement a struct    #
.data 
nameprompt: .asciiz "name:" 
ageprompt: .asciiz "age:" 
genderprompt: .asciiz "gender:" 
cityprompt: .asciiz "City:" 
mystruct: .word 0:129   

# name:   256 chars ASCII = 64 words   
# age:    1 byte = 1 word   
# gender: 1 char = 1 word   
# city:   256 chars ASCII = 64 words   
#--------------------------+   
#         130 words =  520 bytes 

.text  
li $v0, 4  
la $a0, nameprompt  
syscall  

la $a0, mystruct  
li $a1, 256  
li $v0, 8  
syscall   # read string. $a0 = string address, $a1 = max length  

li $v0, 4
la $a0, ageprompt  
syscall  

li $v0, 5  
syscall   # read int into $v1  

la $a0, mystruct  
addi $a0, $a0, 256  # we have to calculate where the age int is                    
#  in the struct  

sw $v0, 0($a0)  
li $v0, 4  
la $a0, genderprompt  
syscall  

la $a0, mystruct  
addi $a0, $a0, 260 # we have to calculate where the gender byte is                     
#  in the struct  

li $a1, 10 # it reads max 9 chars. Note: It may overwrite the             
#  adjoining city string!  

li $v0, 8   
syscall   # read string  

li $v0, 4  
la $a0, cityprompt  
syscall  

la $a0, mystruct  
addi $a0, $a0, 264 # we have to calculate where the city string is                     
#  in the struct  

li $a1, 256  
li $v0, 8  
syscall   # read string. $a0 = string address, $a1 = max length 

#terminate program  
li $v0, 10  
syscall