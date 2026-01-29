/*
 * Build:
 *   gcc -O0 -fno-stack-protector -no-pie -o Behaviour_portal Behaviour_portal.c
 *   strip --strip-all Behaviour_portal
 *
 * NOTE: Do NOT distribute flag.txt to players.
 * Put flag.txt only on the remote server/container.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int rate;
    int stock;
} Infraction;

static int points = 100;

/* Only confessable items (Portal Key is NOT here) */
#define ITEM_WORKSHOP 0
#define ITEM_TEST     1
#define ITEM_INSULT   2
#define NUM_ITEMS     3

static Infraction items[NUM_ITEMS] = {
    {"Workshop Absence", 30, 6},
    {"Test Absence",     45, 4},
    {"Member Insult",    25, 7}
};

/* Player ledger (confessed counts for sell/revoke) */
static int confessed[NUM_ITEMS] = {0, 0, 0};

/* Portal key (separate) */
#define KEY_COST 1000
static int key_available = 1;

// ─────────────────────────────────────────────────────────────
// FLAG
// ─────────────────────────────────────────────────────────────

static void print_flag(void) {
    char flag[128];
    FILE *f = fopen("flag.txt", "r");
    if (!f) {
        puts("Portal error.");
        exit(1);
    }

    if (!fgets(flag, sizeof(flag), f)) {
        fclose(f);
        puts("Portal error.");
        exit(1);
    }
    fclose(f);

    size_t len = strlen(flag);
    while (len && (flag[len - 1] == '\n' || flag[len - 1] == '\r')) {
        flag[--len] = '\0';
    }

    printf("\n");
    printf("+============================================================+\n");
    printf("|                                                            |\n");
    printf("|           *** PORTAL GATEWAY ACTIVATED! ***                |\n");
    printf("|                                                            |\n");
    printf("+============================================================+\n");
    printf("|                                                            |\n");
    printf("|   Flag:%-52s|\n", flag);
    printf("|                                                            |\n");
    printf("+============================================================+\n");
    printf("\n");
}

// ─────────────────────────────────────────────────────────────
// UI
// ─────────────────────────────────────────────────────────────

static void print_banner(void) {
    printf("\n");
    printf("+============================================================+\n");
    printf("|                                                            |\n");
    printf("|        TIME REVERSE UNIVERSE PORTAL ACCESS SYSTEM          |\n");
    printf("|             Securinets FST - Earth Division                |\n");
    printf("|                                                            |\n");
    printf("+============================================================+\n");
    printf("\n");
}

static void print_menu(void) {
    printf("\n");
    printf("=====================\n");
    printf("You have %d points\n", points);
    printf("\tInfraction\t\tRate\tAvailable\tConfessed\n");

    for (int i = 0; i < NUM_ITEMS; i++) {
        printf("(%d) %-25s\t%d\t%d\t\t%d\n",
               i, items[i].name, items[i].rate, items[i].stock, confessed[i]);
    }

    printf("(%d) Redeem Portal Access Key\t%d\t%d\n", NUM_ITEMS, KEY_COST, key_available);
    printf("(%d) Revoke a Confession\n", NUM_ITEMS + 1);
    printf("(%d) Exit\n", NUM_ITEMS + 2);
    printf("\n");
    printf("Choose an option: ");
}

// ─────────────────────────────────────────────────────────────
// INPUT
// ─────────────────────────────────────────────────────────────

static int get_number(void) {
    int num;
    if (scanf("%d", &num) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return -1;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    return num;
}

// ─────────────────────────────────────────────────────────────
// COMMON HELPERS (kept small so RE must inspect call sites)
// ─────────────────────────────────────────────────────────────

static int can_afford(int rate, int quantity) {
    /* Note: intentionally "simple" */
    return points >= rate * quantity;
}

static void apply_confession(int idx, int rate, int quantity) {
    points = points - (rate * quantity);
    items[idx].stock = items[idx].stock - quantity;
    if (quantity > 0) {
        confessed[idx] += quantity;
    }
}

// ─────────────────────────────────────────────────────────────
// CONFESSION FUNCTIONS (each item handled separately)
// ─────────────────────────────────────────────────────────────

static void confess_workshop_absence(void) {
    int quantity;
    printf("How many do you want to confess? ");
    quantity = get_number();

    /* Correct validation: reject negative */
    if (quantity < 0) {
        puts("Invalid amount!");
        return;
    }

    if (quantity > items[ITEM_WORKSHOP].stock) {
        puts("Not enough available!");
        return;
    }

    if (!can_afford(items[ITEM_WORKSHOP].rate, quantity)) {
        puts("Not enough points!");
        return;
    }

    apply_confession(ITEM_WORKSHOP, items[ITEM_WORKSHOP].rate, quantity);
    puts("Confession recorded!");
}

static void confess_test_absence(void) {
    int quantity;
    printf("How many do you want to confess? ");
    quantity = get_number();

    /* Correct validation: reject negative */
    if (quantity < 0) {
        puts("Invalid amount!");
        return;
    }

    if (quantity > items[ITEM_TEST].stock) {
        puts("Not enough available!");
        return;
    }

    if (!can_afford(items[ITEM_TEST].rate, quantity)) {
        puts("Not enough points!");
        return;
    }

    apply_confession(ITEM_TEST, items[ITEM_TEST].rate, quantity);
    puts("Confession recorded!");
}

/*
 * INTENDED VULNERABILITY (ONLY HERE):
 * Missing "quantity < 0" validation.
 * Negative quantity:
 *  - passes "quantity > stock" check
 *  - passes affordability check because rate*quantity is negative
 *  - points = points - (rate * negative) => points increase
 *  - stock increases
 */
static void confess_member_insult(void) {
    int quantity;
    printf("How many do you want to confess? ");
    quantity = get_number();

    // VULN: no check for negative quantity here

    if (quantity > items[ITEM_INSULT].stock) {
        puts("Not enough available!");
        return;
    }

    if (!can_afford(items[ITEM_INSULT].rate, quantity)) {
        puts("Not enough points!");
        return;
    }

    apply_confession(ITEM_INSULT, items[ITEM_INSULT].rate, quantity);
    puts("Confession recorded!");
}

// ─────────────────────────────────────────────────────────────
// DISTRACTION: REVOKE/SELL (safe)
// ─────────────────────────────────────────────────────────────

static void revoke_confession(void) {
    puts("Your ledger:");
    printf("\tInfraction\t\tRate\tConfessed\n");
    for (int i = 0; i < NUM_ITEMS; i++) {
        printf("(%d) %-25s\t%d\t%d\n",
               i, items[i].name, items[i].rate, confessed[i]);
    }

    printf("Choose infraction to revoke: ");
    int choice = get_number();
    if (choice < 0 || choice >= NUM_ITEMS) {
        puts("Invalid option!");
        return;
    }

    printf("How many do you want to revoke? ");
    int quantity = get_number();

    if (quantity < 0) {
        puts("Invalid amount!");
        return;
    }

    if (quantity > confessed[choice]) {
        puts("You haven't confessed that many!");
        return;
    }

    points = points + (items[choice].rate * quantity);
    confessed[choice] -= quantity;
    items[choice].stock += quantity;

    puts("Revocation recorded!");
}

// ─────────────────────────────────────────────────────────────
// PORTAL KEY (separate, cannot be confessed)
// ─────────────────────────────────────────────────────────────

static void redeem_key(void) {
    if (!key_available) {
        puts("Key unavailable!");
        return;
    }
    if (points < KEY_COST) {
        puts("Not enough points!");
        return;
    }

    key_available = 0;
    print_flag();
    exit(0);
}

// ─────────────────────────────────────────────────────────────
// DISPATCH
// ─────────────────────────────────────────────────────────────

static void handle_choice(int option) {
    if (option == ITEM_WORKSHOP) {
        confess_workshop_absence();
    } else if (option == ITEM_TEST) {
        confess_test_absence();
    } else if (option == ITEM_INSULT) {
        confess_member_insult();
    } else if (option == NUM_ITEMS) {
        redeem_key();
    } else if (option == NUM_ITEMS + 1) {
        revoke_confession();
    } else {
        puts("Invalid option!");
    }
}

// ─────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin,  NULL, _IONBF, 0);

    print_banner();

    while (1) {
        print_menu();
        int option = get_number();

        if (option == NUM_ITEMS + 2) {
            puts("Goodbye!");
            break;
        }

        handle_choice(option);
    }

    return 0;
}
