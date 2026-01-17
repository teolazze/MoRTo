# MoRTo

**MoRTo** is a C++ project that performs **exact (combinatorial) calculations** and **Monte Carlo simulations**
for a custom card game called **Morto**. It also provides **card-counting statistics** (two counting/tag systems)
to study how deck composition affects outcomes and expected value.

> **NB:** For now, do **not** trust uncertainty estimates from simulations.
> **Current version:** v1.0

---

## The Game: Morto (Rules Overview)

The game is **Player vs Dealer** and uses a standard **40-card deck** (or multiple decks).

### 1) Initial deal
- The Dealer gives **one face-up card** to the Player.
- The Dealer takes **one face-down card** for themselves.

### 2) Player decision
The Player chooses either:
- **Keep** the current face-up card, or
- **Swap** with the Dealer’s face-down card.

### 3) Dealer action (after the Player decision)

#### If the Player keeps the card
- The Dealer reveals their face-down card.
- If the Dealer’s card is **≥ 7**, the Dealer keeps it.
- Otherwise, the Dealer **draws a new card** from the deck (replacing it).

#### If the Player requests a swap
- **Special rule:** if the Dealer has a **10**, the swap **does not happen** (swap is blocked).
- Otherwise, the cards are swapped.
- Then the Dealer decides whether to keep the new card:
  - If the Dealer’s new card is **greater** than the card they gave to the Player, the Dealer **keeps it** (no further change).
  - Else, if the new card is **≥ 7**, the Dealer **keeps it**.
  - Otherwise, the Dealer **draws a new card** from the deck (replacing it).

### 4) Outcome / payout
- The **highest card wins**.
- If the **Dealer wins**, the Dealer takes the Player’s bet.
- If the **Player wins**, the Dealer pays the Player the bet (**even money**).
- If there is a **tie**, the Player gets the bet back (**push**).

### 5) Side bet: Ace tie
- If the outcome is a **tie of Aces**, a side bet is paid **4× the bet** (configurable in `input.txt`).

---

## Build and Run

From the project root:

```bash
cd MoRTo
make
./MoRTo.exe [output_results_file]
```

Example:

```bash
./MoRTo.exe results.txt
```

---

## Configuration (`input.txt`)

All main options are controlled via `input.txt`.

### Combinatorial Calculation Parameters

```txt
===============================
PARAMETRI CALCOLO COMBINATORIO:
===============================

mazzi: 4
vincita_asso: 4
rulesp: {true,true,true,true,true,false,false,false,false,false}
rulesd: {true,true,true,true,true,true,false,false,false,false}
```

#### Parameters

- **`mazzi`**  
  Number of decks used in the shoe (e.g., `4` means 4 standard 40-card decks).

- **`vincita_asso`**  
  Side-bet payout multiplier for an **Ace tie**.  
  Example: `4` means the side bet pays **4×** the stake when both end up with an Ace.

- **`rulesp`**  
  Player decision policy encoded as a boolean array for card values **1..10**  
  (Ace = 1, and `10` includes all ten-value cards).  
  Each entry tells the program whether the Player would request a swap / apply the **“change”** action for that value:
  - `true`  = apply the action for that value  
  - `false` = do not apply the action for that value  

- **`rulesd`**  
  Dealer decision policy encoded as a boolean array for card values **1..10**.  
  Same concept as `rulesp`, but applied to the Dealer’s strategy table.

> **Note:** The exact interpretation of `rulesp` / `rulesd` follows the strategy logic implemented in the codebase.  
> The array length must be **10** and corresponds to values **1..10**.

---

### Simulation Parameters

```txt
===============================
PARAMETRI SIMULAZIONE:
===============================

sabot: 10000
taglio: 14
sogliaTC1: 7
sogliaTC2: 7
```

#### Parameters

- **`sabot`**  
  Number of simulated games/shoes.  

- **`taglio`**  
  Cut-card position / penetration control: how many cards are left before reshuffling  
  (or before starting a new shoe).  
  Example: 14 means the cut card is placed 14 cards from the end of the shoe. When the cut card is reached during play, exactly one more round is played after the round in which it appears.

- **`sogliaTC1`**  
  Threshold for **True Count System 1**.  
  The simulation can compute statistics only for hands where `TC1 > sogliaTC1`.

- **`sogliaTC2`**  
  Threshold for **True Count System 2**.  
  The simulation can compute statistics only for hands where `TC2 > sogliaTC2`.

---

## Output

The program writes:
- **Combinatorial (exact) statistics** for the configured rules and number of decks.
- **Simulation results** across many games/shoes.
- **Card-counting tag outputs** (two systems) and filtered performance for hands above the configured true-count thresholds.

---

## Notes

MoRTo is intended for analysis and experimentation: you can modify strategies (`rulesp`, `rulesd`), shoe size (`mazzi`),
penetration (`taglio`), and counting thresholds (`sogliaTC1`, `sogliaTC2`) to explore expected value and variance.
