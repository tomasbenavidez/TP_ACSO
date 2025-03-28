.text

MOV X0, #5     
MOV X1, #10    
CMP X0, X1     
BGT greater    
BLT less       
BEQ equal      

greater:
    MOV X2, #1  
    B end

less:
    MOV X2, #2  
    B end

equal:
    MOV X2, #0  
    HLT 0

end:
    MOV X7, #1  
    HLT 0
