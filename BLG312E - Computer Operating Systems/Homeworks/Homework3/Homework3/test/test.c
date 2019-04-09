
#include "base.h"

int main(int argc, const char * argv[]) {
    readfile(argv[1]);

    printf("Reg Time: %d\n", regtime);
    printf("Apl Count: %d\n", applicant_count);

    int i=0;
    for (i = 0; i < applicant_count; i++){
        printf("%d %d %d %d\n", apl_input[i].interview_time, apl_input[i].trial_time, apl_input[i].interview_result, apl_input[i].trial_result);
    }

    return 0;
}
