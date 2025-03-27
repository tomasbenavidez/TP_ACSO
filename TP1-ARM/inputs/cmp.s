.text

MOV R0, #5     
MOV R1, #10    
CMP R0, R1     
BGT greater    
BLT less       
BEQ equal      

greater:
    MOV R2, #1  
    B end

less:
    MOV R2, #2  
    B end

equal:
    MOV R2, #0  
    HLT 0

end:
    MOV R7, #1  
    HLT 0
