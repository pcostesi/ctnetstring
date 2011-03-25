#include "tns.h"
#include <stdio.h>
#include "ht.h"
#include <string.h>

int main(int argc, char **argv){
    tnetstr * dict, * str, * str2;
    char c[300];
    size_t l;

    dict = tns_parse("14:2:hi,6:hellos,}");
    str = tns_get_dict(dict, "hi");
    printf("str is %p\n", str);
    if (str == NULL) return 1;
    l = tns_str(str, c, 300);
    printf("hi : %*s\n", (int) l, c);
    str2 = tns_parse("2:hi,");
    l = tns_str(str2, c, 300);
    printf("%s (%d of %d)\n", c, (int) strlen(c), (int) l);
    return 0;
}
