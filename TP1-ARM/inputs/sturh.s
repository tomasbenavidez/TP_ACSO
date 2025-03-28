.text
mov X1, 0x1000
lsl X1, X1, 16
mov X10, 0x1234
sturh W10, [X1, 0x0]
ldurh W13, [X1, 0x0]
HLT 0
