#include "tns.h"
#include <stdio.h>

int main(int argc, char **argv){
    tnetstr * str;
    str = tns_parse("2:hi,");
    printf("%p\n%s\n", str, tns_get_payload(str).str);
    return 0;
}
