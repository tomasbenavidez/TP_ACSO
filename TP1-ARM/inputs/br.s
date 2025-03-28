.text

adds X0, X10, #1       
mov X3, target         // Load the address of 'target' into X3
br X3                  // Branch to the address in X3
adds X1, X11, X0       

target:
    mov X2, #42        
    HLT 0