// New_student.c
#include <stdio.h>
#include <string.h>

void print_banner() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║   SecuriNets FST  Portal             ║\n");
    printf("║   Member Authentication System       ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
}

int main() {
    char password[100];

    char member_pass[] = {
    'U','2','V','j','d','X','J','p','b','m','V','0','c','1','9','m',
    'c','3','R','7','d','2','V','s','Y','z','B','t','Z','V','9','i',
    'N','G','N','r','X','2','1','l','b','W','J','l','c','n','0','=',
    '\0'
    };


    print_banner();
    printf("[*] Enter your password: ");
    scanf("%99s", password);

    if (strcmp(password, member_pass) == 0) {
        printf("\n[+] Access Granted! Welcome back, member!\n");
        printf("[+] Flag: %s\n", member_pass);
    } else {
        printf("\n[-] Access Denied! Wrong password.\n");
        printf("[-] Contact IT department at room 404.\n");
    }

    return 0;
}

