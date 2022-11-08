REGISTER: zero(x0), sp, ra, a0~a4, t0~t4

Among the 40 instructions defined in the RV32I base instruction set, __snurisc__ supports the following 31 instructions:

* ALU instructions: `lui`, `auipc`, `addi`, `slti`, `sltiu`, `xori`, `ori`, `andi`, `slli`, `srli`, `srai`, `add`, `sub`, `sll`, `slt`, `sltu`, `xor`, `srl`, `sra`, `or`, `and`
* Memory access instructions: `lw`, `sw`
* Control transfer instructions: `jal`, `jalr`, `beq`, `bne`, `blt`, `bge`, `bltu`, `bgeu`

### Special Instruction

* `ebreak`: The `ebreak` instruction is used to return control to a debugging environment. In __snurisc__, we use the `ebreak` instruction to stop the execution of the simulator.

생각할 것..
- 패딩이 있음. 이건 w에 3을 곱한 것을 4로 나눠서 생각,,
- 굳이 2중 for문이 필요한가? 에 대해서도 생각해볼것,,
- a%4 -> 마지막 두 비트만 남기면 된다 3이랑 and치면 될듯
- 0~3을 순회하는 어떤 변수를 이용하면, 딱 한바이트 제외 나머지는 0으로 마스킹해서 깔끔하게 쓸 수 있을 것 같은데,,

array에서 n번째에 해당하는 byte 값에 접근하려면,  (4*k)ptr의 pth least byte로(p=0~3) 접근한다, 
Shift right을 8*p만큼 수행, 0x00000011과 or로 마스킹해서 1byte 값을 얻어낸다.
interface를 확실히 정하는게 좋을듯.. 함수 인자랑 behavior를 명확히 정하고 구현으로 들어가야 한다.

getAvgVal 로직: input의 [i][j] pixel과 RGB값에 대해, 평균값을 구한다!
i, j, BGR(0,1,2)에 대해 루프 돌려 -> getVal호출해서 더하고 나눈값 반환
getVal 로직: (inputIndex)imgptr 값을 땡겨와서 0x000000FF << inputByteOrder*8 와 and연산, 결과값을 >>inputByteOrder*8 후 리턴

write 로직: 1byte 값을 outputByte에 쓴다!
1. mask: 0x000000FF와 value을 각각 <<outputByteOrder*8 해준다.
2. mask에 not연산 때리고, (outputIndex)outptr 값과 and를 때려서 그 한 바이트만 0으로 초기화
3. 초기화한 그 값과 shift해둔 value를 or연산 때리기
4. 결과값을 outptr에 쓴다.