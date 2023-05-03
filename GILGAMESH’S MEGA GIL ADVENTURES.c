/***************************************************************************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts learned. I have constructed the functions 
and their respective algorithms and corresponding code by myself. The program was run, tested, and debugged by my own efforts. I further certify that I have not 
copied in part or whole or otherwise plagiarized the work of other students and/or persons.
<Rischa Francine C. Lastimosa> - <DLSU ID 12201875> - <S15B>
***************************************************************************************************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#include <time.h> 
#include <string.h>
#include <ctype.h>

#define NUMITEMS 8

// define the player's attributes and status
typedef struct {
    int cash;           // amount of money the player has
    int debt;           // amount of money the player owes
    int bank;           // amount of money in the player's bank account
	
    int day;            // current day of the game
    int inventory[8];   // number of items the player has for each type of item
} player_t;

// generates a random integer in the range [min, max]
int randRange(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

// does a probability check, returns true if it passes or false if it fails
// probability of true is 1/N: e.g. N = 5 is a 20% chance
bool probabilityCheck(int N) {
    return (rand() < (RAND_MAX + 1u) / N);
}

// clears the input buffer to prevent leftover characters from interfering with user input
void clearInputBuffer() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

// prints the shop name in between two lines of asterisks
void printShopName( char* name ) {
    size_t namelen = strlen(name);

    for ( int i = 0; i < namelen; i++ ) {
        putchar('*');
    }
    putchar('\n');

    puts(name);

    for ( int i = 0; i < namelen; i++ ) {
        putchar('*');
    }
    putchar('\n');
}

// simulates a player visiting a shop and purchasing/selling items
void visitShop( player_t* plr, int shopID, char* shopname ) {
      char* itemNames[] = {
        "Phoenix Down",     // 0
        "Elixir Essence",   // 1
        "Platinum Ingot",   // 2
        "Golden Materia",   // 3
        "Scarletite",       // 4
        "Adamantite",       // 5
        "Dark Matter",      // 6
        "Trapezohedron"     // 7
    };

    // generate prices
    int prices[] = {
        randRange(500, 1200),    // 0: Phoenix Down      500 – 1,200 Gil
        randRange(1500, 2100),   // 1: Elixir Essence    1,500 – 2,100 Gil
        randRange(5000, 7000),   // 2: Platinum Ingot    5,000 – 7,000 Gil
        randRange(3500, 5500),   // 3: Golden Materia    3,500 – 5,500 Gil
        randRange(8000, 12000),  // 4: Scarletite        8,000 – 12,000 Gil
        randRange(15000, 30000), // 5: Adamantite        5,000 – 30,000 Gil
        randRange(40000, 70000), // 6: Dark Matter       40,000 – 70,000 Gil
        randRange(60000, 90000)  // 7: Trapezohedron     60,000 – 90,000 Gil
    };
    // check for SOLD OUT
    for ( int i = 0; i < NUMITEMS; i++ ) {
        if ( probabilityCheck(5) ) {
            prices[i] = -1;
        }
    } 
    // generate shop-specific prices (overrides "sold out")
    switch ( shopID ) {
        // Tycoon's Meteor Minerals
        case 0: { 
            prices[7] = randRange(35000, 65000); // Trapezohedron    35,000 – 65,000 Gil
        } break;

        // Pulsian Restoratives
        case 1: { 
            prices[0] = randRange(300, 800);     // Phoenix Down     300 – 800 Gil
            prices[1] = randRange(1000, 1700);   // Elixir Essence   1,000 – 1,700 Gil
        } break;
        
        // Archadian Luxuries
        case 2: { 
            prices[2] = randRange(4500, 6000);   // Platinum Ingot   4,500 – 6,000 Gil
        } break;
        
        // Cid's Magical Escapades
        case 3: {
            prices[3] = randRange(3000, 5000);   // Golden Materia   3,000 – 5,000 Gil
        } break;
         
        // Gaian Gratitudes
        case 4: {
            prices[4] = randRange(5000, 10000);  // Scarletite       5,000 – 10,000 Gil
        } break;

        // Riches and Minerals of Spira
        case 5: {
            prices[5] = randRange(9500, 20000);  // Adamantite       9,500 – 20,000 Gil
            prices[6] = randRange(22000, 50000); // Dark Matter      22,000 – 50,000 Gil
        } break;
    }

    bool buymode = false; // list numbers next to items, for selection when buying or selling.
    bool buying = true; // true = buying, false = selling

    char* message = "Welcome! How may I be of service?";

    char choice;
    int amount;
    bool buyError = false;
    for (;;) {
        // print shop menu
        system("cls");
        printShopName( shopname );
        if ( buyError ) {
            printf("\nShopkeeper: %s %d %s!\n\n", message, amount, itemNames[choice]); // TODO: handle plural vs single item name grammar
            buyError = false;
        } else {
            printf("\nShopkeeper: %s\n\n", message);
        }
        printf( "%-21s%-13s%-13s\n\n", "Item", "On Hand", "Price" );
        for ( int i = 0; i < NUMITEMS; i++) {
            char priceTxt[] = "Sold Out";
            if ( prices[i] != -1 ) {
                sprintf( priceTxt, "%d G", prices[i] );
            }
            if ( buymode ) {
                printf( "[%d] %-17s%-13d%-15s", i+1, itemNames[i], plr->inventory[i], priceTxt);
            } else {
                printf( "%-21s%-13d%-15s", itemNames[i], plr->inventory[i], priceTxt);
            }

            switch (i) {
                case 1: {
                    printf("Day #%d\n", plr->day);
                } break;

                case 3: {
                    printf("Cash: %d G\n", plr->cash);
                } break;

                case 4: {
                    printf("Debt: %d G\n", plr->debt);
                } break;

                case 5: {
                    printf("Bank: %d G\n", plr->bank);
                } break;

                default: putchar('\n');
            }
        }

        if ( buymode ) {
            fputs("\nChoice: ", stdout);
        } else {
            fputs("\n[B]uy [S]ell [L]eave\n\nOption: ", stdout);
        }

        // handle user input
        choice = 0;
        scanf("%c", &choice);
        clearInputBuffer();
        putchar('\n');

        if ( buymode ) {
            buymode = false;
            if ( !isdigit(choice) ) { // if didn't enter a number
                message = "I'm sorry, I don't understand.";
                // would love to use `continue` here...
            } else {
                choice = choice - '1'; // TODO: modify this to support more than 9 items?
                if ( choice < 0 || choice >= NUMITEMS || prices[choice] == -1 ) { // if number is out of range
                    message = "I'm sorry, those are unavailable.";
                    // would love to use `continue` here...
                } else {

                    amount = 0;
                    fputs("Shopkeeper: How many?\n\nChoice: ", stdout);
                    scanf("%d", &amount);
                    clearInputBuffer();
                    putchar('\n');

                    message = "Thank you for your business!";
                    if ( buying ) {
                        if ( prices[choice]*amount <= plr->cash ) { // can afford to buy this many!
                            plr->cash -= prices[choice]*amount;
                            plr->inventory[choice] += amount;
                        } else {
                            message = "You can't afford";
                            buyError = true;
                        }
                    } else {
                        if ( amount <= plr->inventory[choice] ) { // have this many to sell!
                            plr->cash += prices[choice]*amount;
                            plr->inventory[choice] -= amount;
                        } else {
                            message = "You don't have";
                            buyError = true;
                        }
                    }
                }
            }
        } else {
            choice = tolower(choice); // switch to lowercase
            switch ( choice ) {
                case 'b': {
                    buymode = true;
                    buying = true;
                    message = "Buying an item? Which one?";
                } break;

                case 's': {
                    buymode = true;
                    buying = false;
                    message = "Selling an item? Which one?";
                } break;
                
                case 'l': {
                    fputs("Shopkeeper: Goodbye!\n\n", stdout);
										system("pause");
                    return;
                } break;

                default: message = "I'm sorry, I don't understand.";
            }
        }
    }
}

void merchant( player_t *plr ) {
  char* message = "Are you ready to pay your debt?";
  char choice;
  int amount;
  
  bool error = false;
  
  for (;;) {
  	system("cls");
  	printShopName("Merchant of the Rift");
		printf("\nMerchant of the Rift: %s\n\n", message);
    printf("%-35sDay #%d\n", "[1] Deposit Gil", plr->day);
    printf("%s\n", "[2] Withdraw Gil");
    printf("%-35sCash: %d G\n", "[3] Pay off Debt", plr->cash);
    printf("%-35sDebt: %d G\n", "[4] Ask for additional loan", plr->debt);
    printf("%-35sBank: %d G\n\n", "[5] Leave", plr->bank);
    
    fputs("Choice: ", stdout);
    scanf("%c", &choice);
    clearInputBuffer();
    choice = choice - '0';

    if ( choice == 5 ) { // Leave
      fputs("Merchant of the Rift: Goodbye!\n\n", stdout);
			system("pause");
      return;
      
    } else if ( choice <= 0 || choice > 4 ) {
      message = "I'm sorry, I don't understand.";
    } else if ( choice == 3 && plr->debt == 0 ) {
   		message = "What do you mean? You have no debt!";
    } else if ( choice == 2 && plr->bank == 0 ) {
      message = "I owe you nothing! Perhaps you wish to take out a loan?";
      
    } else {
      fputs("\nMerchant of the Rift: How much?\n\nChoice: ", stdout);
      scanf("%d", &amount);
      clearInputBuffer();
      if ( amount < 0 ) {
        message = "Negative Gil? Do you take me for a fool?";
      } else if ( amount == 0 ) {
        message = "Don't waste my time.";
      } else {
        switch ( choice ) {
          case 1: { // deposit
          	if ( amount <= plr->cash ) {
              if ( plr->debt > 0 ) {
              	message = "You should probably pay off your debt first...";
              } else if ( plr->bank == 0 ) {
	              message = "Ah, now I owe *you*, hmm?";
              } else { // if plr->debt == 0 && plr->bank > 0
                message = "Your gil continues to be in good hands.";
              }
							plr->cash -= amount;
              plr->bank += amount;
            } else { // if amount > plr->cash
              message = "You can't deposit more than you have!";
            }
          } break;
            
          case 2: { // withdraw
            if ( plr->bank < amount ) { // partial withdraw
              message = "Certainly, the rest of your gil remains in good hands.";
              plr->cash += amount;
              plr->bank -= amount;
            } else { // full withdraw
              if ( amount == plr->bank ) { // exact
                if ( plr->debt == 0 ) {
                  message = "Once again, we are square.";
                } else {
                  message = "Maybe you should put that towards your debt, hmm?";
                }
              } else { // asked for too much
	              message = "I'm afraid I don't owe you quite that much, but here's everything I *do* owe.";
              }
              plr->cash += plr->bank;
              plr->bank = 0;
            }
          } break;
            
          case 3: { // pay off loan
						if ( plr->cash < amount ) { // don't have enough
              message = "You don't have enough to make a payment of that size!";
            } else if ( amount < plr->debt ) { // partial payment
            	message = "I'll take that off of what you owe."; // TODO: tell how much they paid!
              plr->cash -= amount;
              plr->debt -= amount;
            } else { // full payment
              if ( amount == plr->debt ) { // exact
								message = "Good work! Your debt is paid in full.";
              } else { // overpaid
            		message = "You should be more careful! A less honest merchant would have stolen your change...";
              }
              plr->cash -= plr->debt;
              plr->debt = 0;
            }
          } break;
            
          case 4: { // get new loan
            if ( plr->debt == 0 ) {
              message = "You want another loan? Certainly...";
            } else {
	          	message = "I'll add that to what you owe."; // TODO: tell how much they asked for!
            }
            plr->cash += amount;
            plr->debt += amount;
          } break;
        }
      }
    }

  }	
}

void exitScreen( player_t *plr ) {
  system("cls");
  fputs("END OF THE GAME!\n\n", stdout);
  
  int total = plr->cash + plr->bank;
	int profit = total - plr->debt;
  printf("Total Gil Earned: %d G\n", total);
  printf("Debt Remaining: %d G\n", plr->debt);
  printf("Total Profit: %d G\n\n", profit);

  char* message;
	if ( profit < 0 ) {
    message = "REALLY? YOU COULDN’T EVEN EARN ENOUGH TO BUY AN EXCALIPOOR!";
  } else if ( profit == 0 ) {
		message = "You're right back where you started! Want to try again?";
  } else if ( profit < 10000 ) {
    message = "You need to try harder, that isn't enough for Excalibur!";
  } else { // if profit >= 10000
    message = "Excellent job! You'd make a fine merchant yourself, you know.";
  }
  printf("Merchant of the Rift: %s\n\n", message);
	
  if ( profit < 0 ) {
    fputs("\t\t\t\tYou Lose!\n\n", stdout);
  } else {
    fputs("\t\t\t\tYou Win!\n\n", stdout);
  }
}

void mainRift()
{
    char* shopNames[] = { // this could be a global
        "Tycoon Meteor's Minerals",
        "Pulsian Restoratives",
        "Archadian Luxuries",
        "Cid's Magical Escapades",
        "Gaian Gratitudes",
        "Riches and Minerals of Spira",
				"Merchant of the Rift"
    }; // TODO: enum for shop IDs?
  
  player_t plr = {.day = 1, .cash = 20000, .bank = 0, .debt = 50000, .inventory = {0}};
  
	for (;;) {
		if ( plr.day > 30 ) {
      exitScreen(&plr);
      return;
    }

		system("cls");
		printShopName("MAIN RIFT");
    fputs("\nGilgamesh: Where should my travels take me to?\n\n", stdout);
    for ( int i = 0; i < 7; i++ ) {
      printf("[%d] %-35s", i+1, shopNames[i]);
      
      switch (i) {
          case 0: {
              printf("Day #%d\n", plr.day);
          } break;

          case 2: {
              printf("Cash: %d G\n", plr.cash);
          } break;

          case 3: {
              printf("Debt: %d G\n", plr.debt);
          } break;

          case 4: {
              printf("Bank: %d G\n", plr.bank);
          } break;
          
        case 6: {
          fputs("[Q]uit\n", stdout);
        } break;

          default: putchar('\n');
      }
    }
    
    char choice = 0;
    fputs("\nChoice: ", stdout);
    scanf("%c", &choice);
		clearInputBuffer();
    choice = tolower(choice);
    if ( choice == 'q' ) {
    	exitScreen(&plr);
      return;
    } else {
      choice = choice - '1';
      if ( choice >= 0 && choice < 6 ) {
        // regular shop
        visitShop( &plr, choice, shopNames[choice] );
				plr.day++;
        plr.debt += (plr.debt*15)/100;
				plr.bank += (plr.bank*10)/100;
      } else if ( choice == 6 ) {
        merchant(&plr);
      } // else continue
    }
  }
}

int design()
{
	fputs("\t\t   ___ ___ _    ___   _   __  __ ___ ___ _  _   _\n", stdout);
  fputs("\t\t  / __|_ _| |  / __| /_\\ |  \\/  | __/ __| || | ( )\n", stdout);
  fputs("\t\t | (_ || || |_| (_ |/ _ \\| |\\/| | _|\\__ \\ __ | |/\n", stdout);
  fputs("\t\t  \\___|___|____\\___/_/ \\_\\_|  |_|___|___/_||_|\n", stdout);
  fputs("\t\t       __  __ ___ ___   _      ___ ___ _\n", stdout);
  fputs("\t\t      |  \\/  | __/ __| /_\\    / __|_ _| |\n", stdout);
  fputs("\t\t      | |\\/| | _| (_ |/ _ \\  | (_ || || |__\n", stdout);
  fputs("\t\t      |_|  |_|___\\___/_/ \\_\\  \\___|___|____|\n", stdout);
  fputs("\t\t    _   _____   _____ _  _ _____ _   _ ___ ___\n", stdout);
  fputs("\t\t   /_\\ |   \\ \\ / / __| \\| |_   _| | | | _ \\ __|\n", stdout);
  fputs("\t\t  / _ \\| |) \\ V /| _|| .` | | | | |_| |   / _|\n", stdout);
  fputs("\t\t /_/ \\_\\___/ \\_/ |___|_|\\_| |_|  \\___/|_|_\\___|\n", stdout);
  fputs("\n\n\n\n \t\t\t\t[S]tart a New Game\n\n\t\t\t\t[Q]uit Game\n\n", stdout);
}

int main() {
	
  srand( time(NULL) );
  system("COLOR 5F");
 
  char choice = 0;
	while ( choice != 's' ) {
    system("cls");
    design();
    
    fputs("Choice: ", stdout);
    scanf("%c", &choice);
    clearInputBuffer();
    
    choice = tolower(choice);
    if ( choice == 'q' ) {
      puts("Goodbye!");
      exit(0);
    }
  }
  
  for (;;) {
    // play game
    mainRift();
    // game has ended
		choice = 0;
    while ( choice != 'y' ) {
			fputs("Play Again? [Y]es [N]o\nChoice: ", stdout);
			scanf("%c", &choice);
      clearInputBuffer();
  		if ( choice == 'n' ) {
        puts("Goodbye!");
        exit(0);
      }
    }
  }
  
  return 0;
}
