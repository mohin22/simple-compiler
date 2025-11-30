.text

ldi A 10
sta %var_a
ldi A 5
sta %var_b
lda %var_a
push A
lda %var_b
mov B A
pop A
add
sta %var_a
hlt

.data
var_a = 0
var_b = 0
var_sum = 0
var_diff = 0
var_x = 0
var_counter = 0
var_result = 0
var_temp = 0
