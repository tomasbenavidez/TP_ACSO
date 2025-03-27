.text

adds X0, X10, #1       
b target             
adds X1, X11, X0       

target:
    mov X2, #42        
    HLT 0
    