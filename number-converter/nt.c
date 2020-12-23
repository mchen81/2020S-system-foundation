#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_INT 2147483647
#define MIN_INT -2147483648
#define MAX_UNIT 4294967295
struct nt_info{
    unsigned int v;
    short width;
    short range_start;
    short range_end;
};

void int_to_str(int number, char* str);

int pow2(int pow){
    return 1<<pow;
}

/**
 * convert string to int
 * if input char is incorrect, it will end the program
 * */
int str_to_int(char* str){
    long long int pow10 = 1;
    int leng = (int) strlen(str);
    
    if(leng > 11){
        printf("Error: %s cannot fit into 32 bits\n", str);
        exit(1);
    }

    signed long long int number = 0;
    int neg = 0;
    if(str[0] == '-'){
        neg = 1;
        str[0] = '0';
    }

    for(int i=leng-1; i>=0;i--){
        char digit = str[i];
        if(digit < '0' || digit > '9'){
            printf("Error: %s is not a decimal number\n", str);
            exit(1);
        }
        number += (pow10 * (long long)(digit-'0'));
        pow10 *= 10;
    }

    if(neg ==0 && number >= MAX_INT && number <= MAX_UNIT){ // unsigned int -> negtive int
        neg = 1;
        number = number - MAX_UNIT - 1;
    }else if(number >  MAX_UNIT  || (neg>0 && (0 - number) < MIN_INT)){ // exceed 32 bit
        if(neg > 0){
            str[0] = '-';
        }
        printf("Error: %s cannot fit into 32 bits\n", str);
        exit(1);
    }else{
        if(neg > 0){
            number = 0- number;
        }
    }
    return (int)number;
}
/** return the number of digit of an integer*/
int num_digit(int num){
    if(num < 0){
        num = 0- num;
    }else if(num == 0){
        return 1;
    }
    int cnt = 0;
    while(num > 0){
        cnt++;
        num/=10;
    }
    return cnt;

}
/** unsigned int to string*/
void unit_to_str(int sit, int width, char* unit){
    if(sit >= 0){
        int_to_str(sit, unit);
    }else{
        if(width == 32){
            unsigned int un_int = sit;
            unsigned int ten = 10;
            char buf[10];
            for(int i=9;i>=0;i--){
                if(un_int == 0){
                    buf[i] = 0;
                }else{
                    buf[i] = (un_int % ten)+'0';
                    un_int /= ten;
                }
            }
            for(int i =0;i<10;i++){
                if(buf[i] != '0'){
                    strncpy(unit, &buf[i],10 );
                    break;
                }
            }
        }else{
            int_to_str(sit+pow2(width), unit);
        }
    }

}

void int_to_str(int number, char* str){
    int length = num_digit(number);
    if(number == MIN_INT){
        unsigned int unit = number;
        str[0] = '-';
        unit_to_str(unit, 32, &str[1]);
        return;
    }else if(number < 0){
        str[0] = '-';
        number = 0 - number;
        for(int i=length ;i>0 ;i--){
            str[i] = (number % 10) + '0';
            number/=10;
        }
        str[length+1] = '\0';
    }else if(number == 0){
        str[0] = '0';
        str[1] ='\0';
        return;
    }else{
        for(int i=length -1;i>=0;i--){
            str[i] = (number%10) + '0';
            number /= 10;
        }
        str[length] = '\0';
    }
}

void init_nt_info(struct nt_info* nti){
    nti->v = 0;
    nti->width = 32;
    nti->range_start = 0;
    nti->range_end = 31;
}

void print_nt_info(struct nt_info* nti){
    printf("v = %u\n", nti->v);
    printf("width = %d\n", nti->width);
    printf("range starts at %d\n", nti->range_start);
    printf("range   ends at %d\n", nti->range_end);
}

int bin_to_dec(char* bin){
    int leng = (int)strlen(bin);
    int dec = 0;
    int pow = 1;
    for(int i = leng-1; i>=0 ;i--){
        char digit = bin[i];
        if(digit != '1' && digit != '0'){
            printf("Error: Incorrect 2-Base Number\n");
            exit(1);
        }else if(digit == '1'){
            dec += pow;
        }
        pow *= 2;
    }
    return dec;
}

int hex_to_dec(char* hex){
    int leng = (int)strlen(hex);
    int dec = 0;
    int pow16 = 1;

    for(int i= leng-1; i>=0; i--){
        char digit = hex[i];
        
        if(digit >= '0' && digit <='9'){
            dec += pow16 * (digit-'0');
        }else if(digit >= 'A' && digit <= 'F'){
            dec += pow16 * (digit - 55);
        }else{
            printf("Error: Incorrect Hex Number\n");
            exit(1);
        }
        pow16 *= 16;
    }
    return dec;
}

void check_input_validation(char* number, int base){
    int length = (int)strlen(number);
    if((base == 2 && length > 32) || (base==16 && length > 8)){
        printf("Error: %s cannot fit into 32 bits\n", number);
        exit(1);
    }
}

void parse_input_num_to_v(char* number, struct nt_info* nti){

    if(number[1] == 'X' || number[1] == 'x'){
        check_input_validation(&number[2], 16);
        nti->v = hex_to_dec(&number[2]);
    }else if(number[1] == 'b' || number[1] == 'B'){
        check_input_validation(&number[2] , 2);
        nti->v =bin_to_dec(&number[2]);
    }else{
        unsigned int num = str_to_int(number);
        nti->v = num;
    }
}

void parse_args(int argc, char* argv[], struct nt_info* nti){
    for(int i =1;i < argc;i++){
        if(strcmp(argv[i] , "-b") == 0){
            int cust_bit = str_to_int(argv[i+1]);
            if(cust_bit < 4 || cust_bit > 32 || cust_bit % 4 != 0 ){
                printf("Only Support 4, 8, 12, 16, 20, 24, 28, 32 bit width\n");
                exit(0);
            }
            nti->width = cust_bit;
        }else if(strcmp(argv[i] , "-r") == 0){
            char* range = argv[i+1];
            int comma_index= -1;
            for(int r=0; r< strlen(range);r++){
                if(range[r] == ','){
                    comma_index = r;
                }
            }
            if(comma_index == -1){
                printf("Range Parsed Error\nMust follow the format: -r start,end\n");
                exit(0);
            }

            int range_start = 0;
            int cnt =1;
            for(int j = comma_index-1; j >=0; j--){
                 range_start += (range[j]-48) * cnt ;
                 cnt*=10;
            }
            cnt=1;
            int range_end = 0;
            
            for(int j= (int)strlen(range)-1 ; j > comma_index ;j--){
                range_end +=  (range[j]-48) * cnt;
                cnt*=10;
            }

            if( range_start >= range_end ||range_start < 0 || range_end < 0 || range_start > 31 || range_end > 31){
                printf("Range must be in from 0 to 31\n");
                exit(0);
            }
            nti->range_start = range_start;
            nti->range_end = range_end;
        }
    }
}
/** extract number from range*/
void nti_extract(struct nt_info* nti){
    int range_start = nti->range_start;
    int range_end = nti->range_end;
    if(range_start ==0 && range_end == 31){
        return;
    }
    int value = nti->v;
    value = value >> range_start;
    value = value & (pow2(range_end-range_start+1)-1);
    nti->v = value;
}
/** convert unsgined into binary */
void un_to_bin(unsigned int value, int width, char* binary_number){
    for(int i= width - 1; i>=0 ;i--){
        binary_number[i] = value%2 + '0';
        value /= 2;
    }
    binary_number[width] ='\0';
}
/** convert unsigned to hex*/
void un_to_hex(unsigned int value, int width, char* hex){
    hex[0] = '0';
    hex[1] = 'x';
    char c;
    int hex_width = width / 4 + 2;
    for(int i = hex_width-1 ; i>1 ; i--){ //end on hex[2]
        int rm = value % 16;
        if(rm > 9){
            c = rm + 55; // start from A
        }else{
            c = '0' + rm;
        }
        hex[i] = c;
        value /= 16;
    }
}
/** Generate string for human read*/
void out_hum_bin(char* bin, char* result){
    int length = (int)strlen(bin);
    int shift = 0;
    for(int i=0;i<length;i++){
        result[i+shift] = bin[i];
        if( (i+1) % 4 == 0){
            result[i+shift+1] = ' ';
            shift++;
        }
    }
    result[shift+length-1] = '\0';
}

int unsigned_to_signed(char* bin, int width){

    if(bin[0] == '0'){ // postive number
        return bin_to_dec(bin);
    } // else: engtive number
    
    char sit[width]; // sit: signed int
    int conv = 0; //  1 <-> 0 convertion flag
    for(int i=width-1;i>= 0;i--){
        if(conv != 0){ // convert 1 -> 0, 0 -> 1
            if(bin[i]=='0'){
                sit[i] = '1';
            }else{
                sit[i] = '0';
            }
        }else{ // nothing to do
            sit[i] = bin[i];
        }
        if(bin[i] == '1'){
            conv = 1;
        }
    }
    sit[width] = '\0';
    return 0 - bin_to_dec(sit);
}

int main(int argc, char *argv[]){

    struct nt_info nti;
    init_nt_info(&nti);
    if(argc == 1){
        printf("Input a signed int, unsigned int, hex number with 0x or binary number with 0b\n");
        printf("Assign bit width: -b ,only support 0,4,8,12,16,20,24,28,32. Default is 32\n");
        printf("Assign range from i-th to jth bit: -r i,j , i,j with no space. Default is from 0 to 31.\n");
        printf("Input Number must be on the last position\n");
        exit(0);
    }else if(argc > 2){
        parse_args(argc, argv, &nti);
    }
    
    parse_input_num_to_v(argv[argc-1], &nti);
    nti_extract(&nti);
    int v = nti.v;
  
    char bin[32];
    un_to_bin(v, nti.width, bin);

    char hum[40];
    out_hum_bin(bin, hum);

    char hex[10];
    un_to_hex(v, nti.width,hex);
    
    int sit = unsigned_to_signed(bin, nti.width);
    char sit_str[20];
    int_to_str(sit, sit_str);
    
    char unit_str[20];
    unit_to_str(sit, nti.width, unit_str);
    
    printf("%s (base 2)\n", hum);
    printf("0b%s (base 2)\n", bin);
    printf("%s (base 16)\n", hex);
    printf("%s (base 10 unsigned)\n", unit_str);
    printf("%s (base 10 signed)\n", sit_str);

    return 0;
}


