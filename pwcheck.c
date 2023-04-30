#include <stdio.h>
#include <stdlib.h>

struct Arguments_t{
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

/*nasledujici 3 funkce jsou logicke podminky*/
int isDigit(char temp){
    return ((temp >= '0') && (temp <= '9'));
}

int isUppercaseLetter(char temp){
    return ((temp >= 'A') && (temp <= 'Z'));
}

int isLowercaseLetter(char temp){
    return ((temp >= 'a') && (temp <= 'z'));
}



/*zjistuje delku textoveho retezce (bez znaku ukonceni radku)*/
int getLenght(char *buffer){
    int lenght = 0;
    while(buffer[lenght] != '\0' && buffer[lenght] != '\n'){
        lenght++;  
        /*CRLF/CR/LF podpora*/
        if(buffer[lenght] == '\r'){
            buffer[lenght] = '\n';
            buffer[lenght+1] = 0;
        }
    }
    return lenght;
}

/*vyhodnocuje hodnotu cisla ulozeneho do textoveho retezce*/
long getValue(char *buffer){
    int lenght = getLenght(buffer);
    unsigned long long value = 0;
    for(int i = 0; (i < lenght) && (i < 10); i++){
        //prevod je dokoncen pouze, pokud jsou v retezci jen cislice
        if(isDigit(buffer[i])){
            /*prida cislici prevedenou ze znaku na misto jednotek*/
            value = value * 10 + buffer[i] - '0'; 
        }
        /*v pripade jineho znaku nez je cislice*/
        else return -1;
    }
    return value;
}


/*kontrola bezpecnosti urovne 1*/
int level1check(char *password, int lenght){
    for(int i = 0; i < lenght; i++){
        /*kontrola, jestli heslo obsahuje velka pismena*/
        if(isUppercaseLetter(password[i])){ 
            /*pokud heslo obsahuje velke pismeno,*/
            /*provede se kontrola, jestli heslo obsahuje male pismeno*/
            for(int j = 0; j < lenght; j++){
                if (isLowercaseLetter(password[j])){
                    return 0;
                }               
            }           
        } 
    }
    /*v opacnem pripade*/
    return -1;
}

/*kontrola bezpecnosti urovne 2*/
int level2check(char *password, int parameter, int lenght){

    //kolik podminek z 2. urovne musi byt splneno 
    int needtopass;
   if(parameter > 4){
        needtopass = 4;
    }
    else{
        needtopass = parameter;
        } 

    /*pri splneni nutneho poctu podminek pokracuje kontrolou na urovni bezpecnosti 1*/

    /*chcecking for lowercase letters*/
    for(int i = 0; i < lenght; i++){
        /*hledani malych pismen*/
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
        /*hledani velkych pismen*/        
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
        /*hledani cislic*/
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
        /*hledani specialnich znaku*/
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

/*kontrola bezpecnosti urovne 3*/
int level3check(char *password, int parameter, int lenght){
    /*pocet stejnych znaku vedle sebe*/
    int sequence = 1; 

    for(int i = 0; i < lenght-1; i++){
        /*pri prochazeni textoveho retezce se zjistuje,*/
        /*kolik stejnych znaku je za sebou*/
        if(password[i] == password[i+1]){
            sequence++;
        }
        /*pokud se sekvence stejnych znaku prerusi,*/
        /*pocitadlo se vynuluje*/
        else sequence = 1;

        /*pokud sekvence odpovida parametru,*/
        /*heslo neuspelo v testu bezpecnosti urovne 3*/
        if(sequence == parameter){  
            return -1;
        }
    }
    /*v pripade uspechu hesla se pokracuje v kontrole bezpecnosti 2. rovne*/
    return level2check(password, parameter, lenght);
}

/*kontrola bezpecnosti urovne 4*/
int level4check(char *password, int parameter, int lenght){

    for(int i = 0; i < lenght; i++){
        for (int j = i+1; j < lenght; j++){
            /*hledani stejneho znaku v hesle,*/
            /*kterym by mohl zacinat totozny textovy podretezec*/
            if(password[i] == password[j]){ 
                /*pokud je takovy znak nalezen,*/
                /*nasledujici cyklus prochazi znaky v moznem podretezci*/
                for(int k = 0; k+j <= lenght; k++){
                    if(password[i+k] == password[j+k]){
                        /*pokud jsou znaky na nadchazejicich mistech shodne,*/
                        /*pokracuje se v porovnavani dalsich znaku*/
                        if(k+1 == parameter){
                            /*v pripade, kdy jsou nalezeny 2 podretezce o delky = parametru je heslo vyrazeno jako nevyhovujici*/
                            return -1;
                        }
                    }
                    else{
                        /*pokud se podretezce prerusi*/
                        break;
                    }
                }
            } 
        }
    }
    /*v pripade, kdy nejsou nalezeny 2 podretezce delky parametru, pokracuje se kontrolou urovne bezpecnosti 3*/
    return level3check(password, parameter, lenght);
}

/*prepinac mezi prvotnimy testy*/
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


/*kontrola, zda jsou arhumenty spravne zadany*/
/*v takovem pripade jsou argumenty ulozeny*/

/*BONUSOVE RESENI ZAHRNUTO*/
struct Arguments_t argsInputCheck(int argc, char *argv[]){
    /*deklarace a definice navratove promenne*/
    struct Arguments_t output = {0, 0, 0};

    /*pocitadlo v pripade pouziti bezneho zadani argumentu*/
    int outcount = 0;

    /*logicka hodnota urcujici pouziti volitelneho zadavani*/
    int optionalArgs = 0;

    for(int i = 1; i < argc; i++){
        /*byla snaha pouzit IF misto SWITCH, ale argumenty nasledne nebyly rozpoznany*/
        /*DON'T TOUCH THE RUNNING CODE!*/ 
        switch(argv[i][0]){
            /*implementace bonusoveho reseni*/
            /*na poradi argumentu nezalezi*/
            /*vsechny argumenty jsou volitelne*/
            case '-':
                switch(argv[i][1]){  
                    /*nastaveni urovne*/
                    case 'l':
                        if((output.level == 0) && (i+1 != argc) && isDigit(argv[i+1][0]) && (argv[i][2] == '\0')){
                            i++;
                            output.level = getValue(argv[i]);
                            optionalArgs = 1;
                        }
                        else{
                            output.level = -1;
                            return output;
                        }
                        break;
                    /*nastaveni parametru*/
                    case 'p': 
                        if((output.param == 0) && (i+1 != argc) && isDigit(argv[i+1][0]) && (argv[i][2] == '\0') && (i+1 != argc)){
                            i++;                        
                            output.param=getValue(argv[i]); 
                            optionalArgs = 1;                         
                        }
                        else{
                            output.param = -1;
                            return output;
                        }
                        break;
                    /*kontrola pouziti argumentu --stats*/
                    case '-': 
                        if(getLenght(argv[i])-2 != 5){
                            output.useStats = -1;
                        }
                        else{
                        char stats[] = {"stats"};
                            output.useStats = 1;
                            for(int j = 0; j <= 4; j++){
                                if(stats[j] != argv[i][j+2]){
                                    output.useStats = -1;
                                    return output;
                                }
                            }
                        }
                        break;
                    default:
                        output.level = -1;
                        return output;
                }
                break;
            /*klasicke zadavani*/
            default:
                if(isDigit(argv[i][0])){
                    if((outcount == 0) && (output.level == 0) && (getValue(argv[i]) > 0)){
                        output.level = getValue(argv[i]);
                        if(outcount == 0 && argc > i+1 && isDigit(argv[i+1][0])){
                            outcount++;
                        }
                        else{
                            outcount = 0;
                        }
                    }
                    else if((outcount == 1) && (output.param == 0) && (getValue(argv[i]) > 0)){
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

    if(output.level > 4){
        output.level = -1;
        return output;
    }

    /*pokud chybi parametr a zadani nebylo rozpoznano jako volitelne*/
    if((output.param == 0) && (output.level != 0) && (optionalArgs == 0)){
        output.param = -1;
    }
    /*v pripade nezadani jednoho volitelneho argumentu*/
    else{
        if(output.param == 0){
            output.param = 1;
        } 
        if(output.level == 0){
            output.level = 1;
        }
    }    

    //printf("%d %d %d\n\n", output.level, output.param, output.useStats); //troubleshooting
    return output;
}


/*pole pro ulozeni znaku, ktere byly alespon jednou pouzity*/
char charsused[120] = {0}; //velikost pole odpovida poctu moznych znaku dle ASCII + volne misto pro prpadne znaky mimo zakladni ASCII

/*ziskavani statistik*/                   
struct statistics_t statistics(struct statistics_t stats, char *buffer){
    /*ukladani poctu hesel*/
    stats.count++; 
    int lenght = getLenght(buffer);

    /*hledani nejkratsiho hesla*/
    if((stats.min == 0) || (stats.min > lenght)){ 
        stats.min = lenght;
    }

    /*prumerna delka hesel*/
    /*princip hledani prumeru: ((prumer * pocet zapoctenych hesel v prumeru) + delka noveho hesla) / pocet hesel celkem*/
    /*"hledani prumeru z prumeru a cisla"*/
    stats.avg = (stats.avg * (stats.count-1) + lenght) / stats.count; 


    /*hledani opakujich se znaku*/
    int isused = 0;
    for(int i = 0; i <= lenght; i++){  
        for(int j = 0; j < stats.nchars; j++){
            if((buffer[i]==charsused[j]) || (buffer[i] == '\n')|| (buffer[i] == '\r')|| (buffer[i] == '\0')){
                isused = 1;
                break;
            }            
        }
        /*pokud znak doposud nebyl pouzit,*/ 
        /*je pripocten a pridan do zaznamu*/
        if(isused == 0){
            charsused[stats.nchars] = buffer[i];
            stats.nchars++;
        }
        else{
            isused = 0;
        }
    }

    return stats;
}

int passwordProcess(struct Arguments_t inputArguments, struct statistics_t stats){

    char buffer[103] = {0}; 

    /*cteni hesel ze vstupu*/
    for(int line = 1; fgets(buffer, 103, stdin) != NULL; line++) //radek, ze ktereho se ctou hesla (vcetne prazdnych radku)
    {
        /*kontrola delky hesla*/
        /*v pripade prekroceni delky se program ukonci*/
        if(getLenght(buffer) > 100){ 
            fprintf(stderr, "Heslo na radku %d je prilis dlouhe! Kontrola hesel ukoncena!\n", line);
            return -1;
        }
        else if(buffer[0]!='\n'){
            //printf("%s\n",buffer);

            /*tisk hesel, ktere prosly testem*/
            if(check(buffer, inputArguments.level, inputArguments.param)==0){
                fprintf(stdout, "%s", buffer);
                if((buffer[getLenght(buffer)]!='\n')){
                    fprintf(stdout, "\n");
                }
            };
            if(inputArguments.useStats == 1){
                stats = statistics(stats, buffer);
            }
        }
    }
    /*tisk statistik, pokud jsou vyzadany*/
    if(inputArguments.useStats == 1){
        printf("Statistika:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %.1f\n", stats.nchars, stats.min, stats.avg);
    }
    return 0;
}

int main(int argc, char *argv[]){

    struct Arguments_t inputArguments;

    struct statistics_t stats = {0, 0, 0, 0};

    inputArguments = argsInputCheck(argc, argv);

    if(inputArguments.level == -1 || inputArguments.param == -1 || inputArguments.useStats == -1){
        fprintf(stderr, "ERROR: Vstupni argumenty jsou chybne zadany!\n");
        return -1;
    }

    return passwordProcess(inputArguments, stats);
}