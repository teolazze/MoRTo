# MoRTo

**MoRTo** is a C++ project that performs **exact (combinatorial) calculations** and **Monte Carlo simulations**
for a custom card game called **“Morto”**. It also provides **card-counting statistics** (two counting/tag
systems) to study how deck composition affects outcomes and expected value.

NB: Don't trust uncertainties for simulations now

---

## The Game: “Morto” (Rules Overview)

The game is **Player vs Dealer** and uses a standard 40 card deck (or multiple decks).

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

### 5) Side bet: “Ace tie”
- If the outcome is a **tie of Aces**, a side bet is paid **4× the bet** (configurable in `input.txt`).

---

## Build and Run

From the project root:

```bash
cd MoRTo

## Configuration (`input.txt`)

All main options are controlled via `input.txt`.

---

## Combinatorial Calculation Parameters

```txt
===============================
PARAMETRI CALCOLO COMBINATORIO:
===============================

mazzi: 4
vincita_asso: 4
rulesp: {true,true,true,true,true,false,false,false,false,false}
rulesd: {true,true,true,true,true,true,false,false,false,false}

make
./MoRTo.exe [output_results_file]
