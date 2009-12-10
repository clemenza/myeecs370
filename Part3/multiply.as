        lw      0       7       mcand  used to add   
        lw      0       2       mplier  
        lw      0       3       neg1    reg3 value is -1
        lw      0       4       repet   reg4 value is 32
        lw      0       5       one
start   nand    2       5       6       reg6 value is intermediate
        add     5       5       5
        nand    6       3       6       check reg6 value is 0
        beq     0       6       lable
        beq     0       0       rshift
rshift  add     1       7       1
lable   add     4       3       4       check if finished loop
        beq     4       0       done
        add     7       7       7
        beq     0       0       start
done    halt        
mcand   .fill   32766
mplier  .fill   10383
neg1    .fill   -1                      we use it to filter mcand
one     .fill   1                       also filter mcand cordination
repet   .fill   32                      we only cope with 32-bit integer
