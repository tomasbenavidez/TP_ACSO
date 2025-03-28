.text
mov X1, 0x2000
lsl X1, X1, 16
mov X11, 0x5678
stur X11, [X1, 0x8]
stur X11, [X1, 0x10]
ldur X16, [X1, 0x8]
ldur X17, [X1, 0x10]
HLT 0
