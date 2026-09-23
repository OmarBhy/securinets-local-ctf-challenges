#include <stdio.h>
#include <string.h>
#include <stdlib.h>


const char *REAL_FLAG =

"Q3liZXJRdWVzdHtTdHJpbmdzXzRyZV8zYXN5fQo=";

const char *FAKE_FLAGS[] = {
    "CyberQuest{this_is_fake}",
    "CyberQuest{keep_looking}",
    "CyberQuest{almost_there}",
};

/* 40 strings per block */
#define NOISE(ID) \
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA_" ID, \
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB_" ID, \
"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC_" ID, \
"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD_" ID, \
"Lorem_ipsum_dolor_sit_amet_" ID, \
"Error_invalid_opcode_" ID, \
"Segmentation_avoided_" ID, \
"Kernel_panic_not_syncing_" ID, \
"Stack_smashing_detected_" ID, \
"DEBUG_0_" ID, \
"DEBUG_1_" ID, \
"SUSPICIOUS_ACTIVITY_" ID, \
"deadbeefcafebabe_" ID, \
"0x7f454c4602010100_" ID, \
"THIS_IS_NOT_A_FLAG_" ID, \
"IGNORE_ME_" ID, \
"RANDOM_DATA_BLOCK_" ID, \
"QUFBQUFBQUFBQUFBQUFBQQ==_" ID, \
"U29tZVJhbmRvbUJhc2U2NA==_" ID, \
"RkxBR3t0aGlzX2lzX2Zha2V9_" ID, \
"U1RSSU5HU19BUkVfSEFSRA==_" ID, \
"VGhpcyBpcyBqdXN0IG5vaXNl_" ID, \
"QkFTRTY0X0RFQ09ZX0ZBS0U=_" ID, \
"QkFTRTY0X0RFQ09ZX0ZBS0Uy_" ID, \
"QkFTRTY0X0RFQ09ZX0ZBS0Uz_" ID, \
"QkFTRTY0X0RFQ09ZX0ZBS0U0_" ID, \
"QkFTRTY0X0RFQ09ZX0ZBS0Uy_" ID,\
"Urteetan zuhar magika norabide"\
"QkFTRTY0X0RFQ09ZX0ZBS0Uz_" ID,\
"desberdinetan gauatu da eta"\
"QkFTRTY0X0RFQ09ZX0ZBS0U0_" ID,\
"magia modu berriak garatu dira."\
"QkFTRTY0X0RFQ09ZX0ZBS0U1_" ID,\
"Sonrginkeriaren potentzia eta"\
"QkFTRTY0X0RFQ09ZX0ZBS0U2_" ID,\
"konulezutasuna hobetu ahala,"\
"QkFTRTY0X0RFQ09ZX0ZBS0U3_" ID,\
"beharrezkoa zen botere magikoan"\
"QkFTRTY0X0RFQ09ZX0ZBS0U4_" ID,\
"kopurua handitzen joan zen,"\
"QkFTRTY0X0RFQ09ZX0ZBS0U5_" ID,\
"eta sorginaren botere magikoa eta"\
"QkFTRTY0X0RFQ09ZX0ZBS0Ux_" ID,\
"mago gisa zuen gaitasuna handitzen joan"\
"QkFTRTY0X0RFQ09ZX0ZBS0Uy_" ID,\
"ziren."\
"QkFTRTY0X0RFQ09ZX0ZBS0U4_" ID


const char *STRING_OCEAN[] = {
    NOISE("000"), NOISE("001"), NOISE("002"), NOISE("003"), NOISE("004"),
    NOISE("005"), NOISE("006"), NOISE("007"), NOISE("008"), NOISE("009"),
    NOISE("010"), NOISE("011"), NOISE("012"), NOISE("013"), NOISE("014"),
    NOISE("015"), NOISE("016"), NOISE("017"), NOISE("018"), NOISE("019"),
    NOISE("020"), NOISE("021"), NOISE("022"), NOISE("023"), NOISE("024"),
    NOISE("025"), NOISE("026"), NOISE("027"), NOISE("028"), NOISE("029"),
    NOISE("030"), NOISE("031"), NOISE("032"), NOISE("033"), NOISE("034"),
    NOISE("035"), NOISE("036"), NOISE("037"), NOISE("038"), NOISE("039"),
    NOISE("040"), NOISE("041"), NOISE("042"), NOISE("043"), NOISE("044"),
    NOISE("045"), NOISE("046"), NOISE("047"), NOISE("048"), NOISE("049"),
    NOISE("050"), NOISE("051"), NOISE("052"), NOISE("053"), NOISE("054"),
    NOISE("055"), NOISE("056"), NOISE("057"), NOISE("058"), NOISE("059"),
    NOISE("060"), NOISE("061"), NOISE("062"), NOISE("063"), NOISE("064"),
    NOISE("065"), NOISE("066"), NOISE("067"), NOISE("068"), NOISE("069"),
    NOISE("070"), NOISE("071"), NOISE("072"), NOISE("073"), NOISE("074"),
    NOISE("075"), NOISE("076"), NOISE("077"), NOISE("078"), NOISE("079"),
    NOISE("080"), NOISE("081"), NOISE("082"), NOISE("083"), NOISE("084"),
    NOISE("085"), NOISE("086"), NOISE("087"), NOISE("088"), NOISE("089"),
    NOISE("090"), NOISE("091"), NOISE("092"), NOISE("093"), NOISE("094"),
    NOISE("095"), NOISE("096"), NOISE("097"), NOISE("098"), NOISE("099"),
};

volatile const char *keep_all(void) {
    volatile const char *sink = NULL;

    for (size_t i = 0; i < sizeof(STRING_OCEAN)/sizeof(*STRING_OCEAN); i++)
        sink = STRING_OCEAN[i];

    for (size_t i = 0; i < sizeof(FAKE_FLAGS)/sizeof(*FAKE_FLAGS); i++)
        sink = FAKE_FLAGS[i];

    sink = REAL_FLAG;
    return sink;
}


int main(void) {
    char buf[128];

    keep_all();

    printf("Enter password: ");
    fgets(buf, sizeof(buf), stdin);
    
    if (strcmp(buf,REAL_FLAG))
        puts("Correct!");
    else
        puts("Wrong!");

    return 0;
}
