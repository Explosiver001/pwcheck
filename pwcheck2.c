#include <stdio.h>
#include <stdlib.h>

struct inputArguments_t{
    int level;
    int param;
    int useStats;
};

struct statistics_t{
    int nchars;
    int min;
    float avg;
    int count;
};

/*next 3 functions are used as logical conditions*/
int isDigit(char temp){
    return ((temp >= '0') && (temp <= '9'));
}

int isUppercaseLetter(char temp){
    return ((temp >= 'A') && (temp <= 'Z'));
}

int isLowercaseLetter(char temp){
    return ((temp >= 'a') && (temp <= 'z'));
}

/*gets lenght of word stored in character array*/
int getLenght(char *buffer){
    int lenght = 0;
    while(buffer[lenght] != '\0' && buffer[lenght] != '\n'){
        lenght++;  
    }
    return lenght;
}

/*evaluates numbers from string*/
long getValue(char *buffer){
    int lenght = getLenght(buffer);
    unsigned long long value = 0;
    for (int i = 0; (i < lenght) && (i<10); i++){
        //converts only if the string contains numbers
        if(((buffer[i] >= '0')&&(buffer[i] <= '9'))&&(value>=0)){
            value = value * 10 + buffer[i] - '0'; //puts evaluated digit to the end of the number  
        }
        else return -1;
    }
    return value;
}

/*checking security level 1*/
int level1check(char *password, int lenght){
    for(int i = 0; i < lenght; i++){
        /*chcecking, if the password contains uppercase letters*/
        if(isUppercaseLetter(password[i])){ 
            /*if the password contains uppercase letters*/
            /*chceck, if the password contains lowercase letters*/
            for (int j = 0; j < lenght; j++){
                if (isLowercaseLetter(password[j])){
                    return 0;
                }               
            }           
        } 
    }
    return -1;
}

/*checking security level 2*/
int level2check(char *password, int parameter, int lenght){

    int needtopass = parameter;
    /*if the password passes , the is sent for security level 1 chceck*/ 

    /*chcecking for lowercase letters*/
    for(int i = 0; i < lenght; i++){
        if(isLowercaseLetter(password[i])){
            needtopass--;
            if(needtopass == 0){
                return level1check(password, lenght);
            }
            break;
        }
    }

    /*chcecking for uppercase letters*/
    for(int i = 0; i < lenght; i++){
        if(isUppercaseLetter(password[i])){
            needtopass--;
            if(needtopass == 0){
                return level1check(password, lenght);
            }
            break;   
        }
    } 

    /*chcecking for digits*/
    for(int i = 0; i < lenght; i++){
        if(isDigit(password[i])){
            needtopass--;
            if(needtopass == 0){
                return level1check(password, lenght);
            }
            break; 
        }
    }

    /*chcecking for special characters*/
    for(int i = 0; i < lenght; i++){
        if((password[i] >= ' ') && (password[i] <= '~') && !isDigit(password[i]) && !isLowercaseLetter(password[i]) && !isUppercaseLetter(password[i])){
            needtopass--;
            if(needtopass == 0){
                return level1check(password, lenght);;
            }        
            break;  
        }
    } 

    return -1;
}

/*checking security level 3*/
int level3check(char *password, int parameter, int lenght){
    int sequence = 1; //how many characters are the same next to each other
    for (int i = 0; i<= lenght-parameter+2; i++){
        if(password[i]==password[i+1]){
            sequence++;
        }
        else sequence = 1;

        if (sequence == parameter){  
            return -1;
        }
    }
    /*if the password passes, the is sent for security level 2 chceck*/
    return level2check(password, parameter, lenght);
}

/*checking security level 4*/
int level4check(char *password, int parameter, int lenght){

    int same = 0; 

    for (int i = 0; i < lenght; i++){
        for (int j = i+1; j < lenght; j++){
            /*looking for the same character in the password*/
            if (password[i] == password[j]){ 
                /*if the password contains the same character,*/
                /*then code in this loop compares the "substrings" beginning with these characters*/
                for (int k = 0; k+j<= lenght; k++){
                    if (password[i+k] == password[j+k]){
                        same++;
                        if (same == parameter){
                            return -1;
                        }
                    }
                    else{
                        same = 0; 
                        break;
                    }
                }
            } 
        }
    }
    /*if the password passes, the is sent for security level 3 chceck*/
    return level3check(password, parameter, lenght);
}

/*checking if the arguments are correct*/
/*then saving those arguments*/

/*BONUS SOLUTION INCLUDED*/
struct inputArguments_t inputCheck(int argc, char *argv[], struct inputArguments_t output){

    int outcount = 0;

    for (int i = 1; i < argc; i++){
        /*I tried using if instead of switch, but the first argument haven't got recognized*/
        /*so in this case: DON'T TOUCH THE RUNNING CODE!*/ 
        switch(argv[i][0]){
            /*implementation of bonus solution*/
            /*the order of aruments doesn't matter*/
            /*all argumetns are optional*/
            case '-':
                switch(argv[i][1]){  
                    case 'l': //sets level
                        if (output.level==0 && isDigit(argv[i+1][0]) && argv[i][2] == '\0'){
                            i++;
                            output.level = getValue(argv[i]);
                        }
                        else{
                            output.level = -1;
                            return output;
                        }
                        break;
                    case 'p': //sets parameter
                        if (output.param==0 && isDigit(argv[i+1][0]) && argv[i][2] == '\0'){
                            i++;                        
                            output.param=getValue(argv[i]);                          
                        }
                        else{
                            output.param = -1;
                            return output;
                        }
                        break;
                    case '-': //checks for --stats for statistic to be used
                        if (getLenght(argv[i])-2 != 5){
                            output.useStats = -1;
                        }
                        else{
                        char stats[] = {"stats"};
                            output.useStats = 1;
                            for (int j = 0; j <= 4; j++){
                                if (stats[j] != argv[i][j+2]){
                                    output.useStats = -1;
                                }
                            }
                        }
                        break;
                    default:
                        output.level = -1;
                        return output;
                }
                break;
            default:
                if(isDigit(argv[i][0])){
                    if(outcount == 0 && output.level == 0 && argv[i][0] != '0'){
                        output.level = getValue(argv[i]);
                        if(outcount == 0 && argc > i+1 && isDigit(argv[i+1][0])){
                            outcount++;
                        }
                        else{
                            outcount = 0;
                        }
                    }
                    else if(outcount == 1 && output.param == 0 && argv[i][0] != '0'){
                        output.param = getValue(argv[i]);
                    }
                    else{
                        output.level = -1;
                        return output;
                    }
                }
                else{
                    output.level = -1;
                    return output;
                }
                break;
        }
    }

    if (output.level > 4){
        output.level = -1;
        return output;
    }
    if (output.param == 0 && output.level == 0){
        output.level = 1;
        output.param = 1;
    }
    else if(output.param == 0 && output.level != 0){
        output.param = -1;
    }


    //printf("%d %d %d\n\n", output.level, output.param, output.useStats);
    return output;
}

int check(char *password, int level, int parameter) 
{    
    int lenght = getLenght(password);

    switch(level){
        case 4:         
            return level4check(password, parameter, lenght);
            
        case 3:
            return level3check(password, parameter, lenght);         

        case 2:
            return level2check(password, parameter, lenght);
        
        case 1: 
            return level1check(password, lenght);

        default: 
            return -1;
    }
    return 0;
}

//array used to store all non-repetetive characters, that were used in passwords
char charsused[100] = {0}; //size is number of all usable characters + 1 
                   
struct statistics_t statistics(struct statistics_t stats, char *buffer){

    stats.count++; //number of passwords
    int lenght = getLenght(buffer);

    /*looking for shortest password*/
    if((stats.min == 0) || (stats.min > lenght)){ 
        stats.min = lenght;
    }

    /*average lenght of password*/
    /*principle: ((average * number of passwords acccounted in average) + lenght of new password to be involved in average) / number of passwords (including the new one) acccounted in average*/
    stats.avg = (stats.avg * (stats.count - 1) + lenght) / stats.count; 


    /*looking for repetetive characters*/
    int isused = 0;
    for (int i = 0; i <= lenght; i++){  
        for (int j = 0; j < stats.nchars; j++){
            if (buffer[i]==charsused[j] || buffer[i] == '\n'|| buffer[i] == '\r'){
                isused = 1;
                break;
            }            
        }
        if (isused == 0){
            charsused[(int)stats.nchars] = buffer[i];
            stats.nchars++;
        }
        else{
            isused = 0;
        }
    }

    return stats;
}

/*reads passwords from text line and sends them to proper functions*/
void passwordProcess(int *inputArguments, float *stats){

    char buffer[103] = {0}; 

    /*reading passwords from text file*/
    for (int line = 1; fgets(buffer, 103, stdin) != NULL; line++) //line = line of the password (empty lines not included)
    {
        /*password lenght check*/
        if(getLenght(buffer)>100){  //kontrola delky hesla
            fprintf(stderr, "Heslo na radku %d je prilis dlouhe! Maximalni delka hesla je 100 znaku. Toto heslo nebude zkontrolovano a zahrnuto ve statistikach!\n", line);
            /*finish reading line*/
            //printf("%s %d\n",buffer, getLenght(buffer));
            while(fgetc(stdin) != '\n');
        }
        else if(buffer[0]!='\n'){
            //printf("%s\n",buffer);

            /*printing passwords that passed check*/
            if(check(buffer, inputArguments[0], inputArguments[1])==0){
                printf("%s", buffer);
            };
            if (inputArguments[2] == 1){
                //statistics(stats, buffer);
            }
        }
    }
}

void passwordProcess1(struct inputArguments_t inputArguments, struct statistics_t stats){

    char buffer[103] = {0}; 

    /*reading passwords from text file*/
    
    for (int line = 1; fgets(buffer, 103, stdin) != NULL; line++) //line = line of the password (empty lines not included)
    {
        /*password lenght check*/
        if(getLenght(buffer)>100){  //kontrola delky hesla
            fprintf(stderr, "Heslo na radku %d je prilis dlouhe! Maximalni delka hesla je 100 znaku. Toto heslo nebude zkontrolovano a zahrnuto ve statistikach!\n", line);
            /*finish reading line*/
            scanf("\n");
        }
        else if(buffer[0]!='\n'){
            //printf("%s\n",buffer);

            /*printing passwords that passed check*/
            if(check(buffer, inputArguments.level, inputArguments.param)==0){
                fprintf(stdout, "%s", buffer);
                if(buffer[getLenght(buffer)]!='\n'){
                    fprintf(stdout, "\n");
                }
            };
            if (inputArguments.useStats == 1){
                stats = statistics(stats, buffer);
            }
        }
    }
    /*printing statistics if wanted*/
    if (inputArguments.useStats == 1){
        /*I wanted to write out NCHARS stats[0] but it was different number depending on OS used...*/
        printf("Statistika:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %.1f\n", getLenght(charsused), stats.min, stats.avg);
    }
}

int main(int argc, char *argv[]){

    struct inputArguments_t inputArguments;
    inputArguments.level = 0;
    inputArguments.param = 0;
    inputArguments.useStats = 0;

    struct statistics_t stats;
    stats.nchars = 0; 
    stats.min = 0;
    stats.avg = 0.0; 
    stats.count = 0; 

    inputArguments = inputCheck(argc, argv, inputArguments);

    if(inputArguments.level == -1 || inputArguments.param == -1 || inputArguments.useStats == -1){
        fprintf(stderr, "ERROR: Vstupni argumenty jsou chybne zadany!\n");
        return -1;
    }

    passwordProcess1(inputArguments, stats);
    return 0;
}