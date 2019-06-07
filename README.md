# bit-tac-toe

Fun implementation of the classic Tic Tac Toe game that utilizes bitwise operators.

## How it works

Each entry in the game board holds information that is encoded at the bit level. Assuming 32 bit unsigned integers, the squares can be broken up into 5 sections:

- Bit 31 (Active bit)
  - Boolean flag that determines whether or not a player has placed a symbol at the respective square
- Bit 30 (Symbol bit)
  - Boolean flag that determines whether the symbol placed is an "X" or an "O" (only checked if the Active Bit is active)
- Bits 29-20 (North bit)
  - Determines how many similar symbols lie directly North of the current square
- Bits 19-10 (North-West bit)
  - Determines how many similar symbols lie directly North-West (diagonally) of the current square
- Bits 9-0 (West bit)
  - Determines how many similar symbols lie directly West of the current square
