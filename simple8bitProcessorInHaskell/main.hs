import Debug.Trace (trace) --importado na fase de testes para printar o estado do processador a todo tempo
import Data.Bits ((.&.))  -- Importando a função bit a bit .&.
-- Definindo a memória
type MemoryUnit = (Int, Int)
type Memory = [MemoryUnit]
--(a=10) and (b=10)
--resp = A+B-2
-- 0 LOD 240
-- 2 ADD 241
-- 4 SUB 245
-- 6 STO 251
-- 8 HLT NOP
-- Função para inicializar a memória com valores predefinidos
initialMemoryOne :: Memory
initialMemoryOne = 
    [ (0,2)
    ,(1,240)
    ,(2,14)
    ,(3,241)
    ,(4,16)
    ,(5,245)
    ,(6,4)
    ,(7,251)
    ,(8,20)
    ,(9,18)
    ,(240,10)
    ,(241,10)
    ,(245,2)
    ,(251,0)
    ]
--Resp = A * B
-- Memória inicial para multiplicação (A = 10, B = 2, Resp = 0)
-- Baseado em somas sucessivas
-- 0 LOD 240 
-- 2 STO 245
-- 4 LOD 242
-- 6 CPE 241
-- 8 JMZ 24 
-- 10 LOD 240
-- 12 ADD 245
-- 14 STO 240
-- 16 LOD 242 
-- 18 add 246
-- 20 sto 242 
-- 22 JMP 4
-- 24 LOD 240 
-- 26 STO 251 
-- 28 HLT NOP
initialMemoryTwo :: Memory
initialMemoryTwo = 
        [ (0,2) -- carrega a no acumulador
        ,(1,240) 
        ,(2,4) -- guarda valor de a em 245
        ,(3,245)
        ,(4,2) -- carrega i no acumulador
        ,(5,242) 
        ,(6,10) -- compara i com b
        ,(7,241)
        ,(8,8) --se forem iguais pula para o fim do programa
        ,(9,24) 
        ,(10,2) -- carrge a no acumulador
        ,(11,240) 
        ,(12,14) --adiciona a com o valor de a que foi guardado como constante
        ,(13,245)
        ,(14,4) --guarda a na memoria novamente 
        ,(15,240)
        ,(16,2) -- carega i no acumulador
        ,(17,242)
        ,(18,14) --soma i com +=1
        ,(19,246)
        ,(20,4)
        ,(21,242)
        ,(22,6) --pula para endereço 4 
        ,(23,4)
        ,(24,2)
        ,(25,240)
        ,(26,4)
        ,(27,251)
        ,(28,20) -- fecha o programa
        ,(29,18)
        ,(240,10) -- a
        ,(241,2) -- b
        ,(242,1) -- i
        ,(245,0)
        ,(246,1) -- += 1
        ,(251,0)
        ]
      
-- A = 0; Resp = 1; while(A < 5) { A = A + 1; Resp = Resp + 2; } 
-- 0 ADD 241
-- 2 CPE 246 
-- 4 JMZ 20 
-- 6 LOD 241 
-- 8 ADD 245 
-- 10 STO 241 
-- 12 LOD 251 
-- 14 ADD 247 
-- 16 STO 251 
-- 18 JMP 0 
-- 20 HLT NOP
initialMemoryThree :: Memory
initialMemoryThree = 
            [ (0,2) --carrega a no accumulador
              ,(1,241)
              ,(2,10)    --compara a com a constante 5  a == 5
               ,(3,246)
               ,(4,8) --se for verdade pula para o fim do programa
               ,(5,20)
               ,(6,2) --carrega a no acumulador
               ,(7,241) 
               ,(8,14) --adiciona 1 ao acumuluador
               ,(9,245)
               ,(10,4) --armazena a de volta na memoria
               ,(11,241)
               ,(12,2) --carrega resp no acumulador
               ,(13,251)
               ,(14,14) --adiciona 5 ao acumulador
               ,(15,247)
               ,(16,4) --guarda acumulador de volta em resp
               ,(17,251)
               ,(18,6) --pula de volta para o inicio para reiniciar o loop
               ,(19,0)
               ,(20,20) --caso o porgrama chegue ao fim
               ,(21,18)
               ,(241,0)--armazena a
                ,(251,1)
                ,(245,1) --constante 1 para a soma
                ,(246,5)
                ,(247,2)
                ]
  
initiateProcessor:: Processor
initiateProcessor = Processor {
    ula = Ula {acc = 0, eqz = True},
    ic = 0,
    ri = (0,0)
}

-- Definindo a ULA
data Ula = Ula {
    acc :: Int, -- acumulador
    eqz :: Bool -- flag acumulador == 0
} deriving (Show)


-- Definindo o processador
data Processor = Processor {
    ula :: Ula, -- unidade aritmética
    ic :: Int, -- contador de instrução
    ri :: (Int, Int) -- registrador de instrução (opcode, endereço)
} deriving (Show)

-- Função para garantir que os valores estejam dentro dos limites de 8 bits
truncateTo8Bits :: Int -> Int
truncateTo8Bits x = x .&. 0xFF

-- Função load que modifica o valor do acc
lod :: Processor -> Memory -> Processor
lod processor memory =
    let (_, address) = ri processor  -- Extrai o endereço do registrador de instrução
        value = readMemory address memory  -- Lê o valor da memória no endereço especificado
        truncatedValue = truncateTo8Bits value
        newUla = (ula processor) { acc = truncatedValue, eqz = (truncatedValue == 0) }  -- Atualiza o valor de acc e eqz
    in processor { ula = newUla }
    
 -- Função add que modifica o valor do acc
add :: Processor -> Memory -> Processor
add processor memory =
    let (_, address) = ri processor  -- Extrai o endereço do registrador de instrução
        value = readMemory address memory  -- Lê o valor da memória no endereço especificado
        truncatedValue = truncateTo8Bits value
        newAcc = truncateTo8Bits (acc (ula processor) + truncatedValue)
        newUla = (ula processor) { acc = newAcc, eqz = newAcc == 0 }  -- Atualiza o valor de acc e eqz
    in processor { ula = newUla }
    
-- Função sub que modifica o valor do acc
sub :: Processor -> Memory -> Processor
sub processor memory =
    let (_, address) = ri processor  -- Extrai o endereço do registrador de instrução
        value = readMemory address memory  -- Lê o valor da memória no endereço especificado
        newUla = (ula processor) { acc = acc (ula processor) - value, eqz = value == 0 }  -- Atualiza o valor de acc e eqz
    in processor { ula = newUla }
    
-- Função STO que armazena o valor do acumulador na memória
sto :: Processor -> Memory -> Memory
sto processor memory =
    let (_, address) = ri processor  -- Extrai o endereço do registrador de instrução
        value = acc (ula processor)  -- Obtém o valor atual do acumulador
    in writeInMemory address value memory  -- Escreve o valor do acumulador na memória no endereço especificado

cpe:: Processor -> Memory -> Processor
cpe processor memory =
                let (_, address) = ri processor
                    value = readMemory address memory
                    accValue = acc (ula processor)
                    newAccVal = if value == accValue  then 0 else 1
                    newUla = (ula processor) { acc = newAccVal, eqz = newAccVal == 0 }  -- Atualiza o acumulador e a flag eqz
                in processor { ula = newUla }
                
jmp:: Processor -> Memory -> Processor
jmp processor memory = 
                let (_,address) = ri processor
                    
                in processor {ic = address}

jmz:: Processor -> Memory -> Processor
jmz processor memory = 
                let (_,address) = ri processor
                in if eqz (ula processor) then processor {ic = address} else processor 
    
-- Função para escrever na memória
writeInMemory :: Int -> Int -> Memory -> Memory
writeInMemory address value memory = (address, value) : filter ((/= address) . fst) memory

-- Função para ler da memória
readMemory :: Int -> Memory -> Int
readMemory address memory = maybe (error "Address not found.") id (lookup address memory)


-- Função para buscar a próxima instrução
fetchInstruction :: Processor -> Memory -> Processor
fetchInstruction processor memory = 
    let pc = ic processor
        opcode = readMemory pc memory
        address = readMemory (pc + 1) memory
    in processor { ri = (opcode, address), ic = pc + 2 }
    

 -- Função de loop principal com impressão do resultado
runProgram :: Processor -> Memory -> (Processor, Memory)
runProgram processor memory
        | opcode  == 20 = (processor, memory) -- Se HLT foi chamado foi ecnontrado
        | otherwise = runProgram newProcessor newMemory
        where
            (newProcessor, newMemory) = execute processor memory
            (opcode, _) = ri processor

        
-- Função principal para executar as instruções
execute :: Processor -> Memory -> (Processor, Memory)
execute processor memory =
    let updatedProcessor = fetchInstruction processor memory
        (opcode, _) = ri updatedProcessor
    in case opcode of
        2 -> (lod updatedProcessor memory, memory)  -- LOD
        14 -> (add updatedProcessor memory, memory)  -- ADD
        16 -> (sub updatedProcessor memory, memory)  -- ADD
        4 -> (updatedProcessor, sto updatedProcessor memory) -- STO
        10 -> (cpe updatedProcessor memory, memory)  -- CPE 
        6 -> (jmp updatedProcessor memory, memory) -- JMP
        8 -> (jmz updatedProcessor memory, memory) -- JMZ
        20 -> (updatedProcessor, memory) -- HLT
        18 -> (updatedProcessor, memory) -- NOP
        
