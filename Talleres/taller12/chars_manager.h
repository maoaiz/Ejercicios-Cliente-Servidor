/*
* chars_manager.h library was build by Mauricio Aizaga <mauricioaizaga.com>
* 
* Build with the code from:
*    * Kernighan & Ritchie's
*    * Mayank Bhola
*
* Available functions in chars_manager.h:
*    itoa: convert int to char,     return void
*    ftoa: convert floar to char,   return char *
*
**/





/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C":
**/
void strreverse(char* begin, char* end) {
    char aux;
    while(end>begin)
        aux=*end, *end--=*begin, *begin++=aux;
}

static void itoa(intptr_t value, char* str, int base) {
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char* wstr=str;
	intptr_t sign;
	div_t res;
	// Validate base
	if (base<2 || base>35){ *wstr='\0'; return; }
	// Take care of sign
	if ((sign=value) < 0) value = -value;
	// Conversion. Number is reversed.
	do {
		res = div(value, base);
		*wstr++ = num[res.rem];
	} while((value=res.quot));
	if (sign < 0) *wstr++='-';
	*wstr = '\0';
	// Reverse string
	strreverse(str, wstr - 1);
}


/*************************************************************************
*
* Converting float to string without using library function like sprintf
* (naive approach , may lead to anomalies)
*
* by Mayank Bhola
* Taked from: https://gist.github.com/psych0der/6319244
*
**************************************************************************/
#define precision 6  //precision for decimal digits
#define MAXDATASIZE 100 

char* ftoa(float f){
        
    float ff;
    ff = f;

    char *str = (char *)malloc(sizeof(char) * MAXDATASIZE);
    int a,b,c,k,l=0,m,i=0,j;
    
    // check for negetive float
    if(f<0.0)
    {
        str[i++]='-';
        f*=-1;
    }
    
    a=f;    // extracting whole number
    f-=a;   // extracting decimal part
    k = precision;
    
    // number of digits in whole number
    while(k>-1)
    {
        l = pow(10,k);
        m = a/l;
        if(m>0)
        {
            break;
        }
    k--;
    }

    // number of digits in whole number are k+1
    
    /*
    extracting most significant digit i.e. right most digit , and concatenating to string
    obtained as quotient by dividing number by 10^k where k = (number of digit -1)
    */
    
    for(l=k+1;l>0;l--)
    {
        b = pow(10,l-1);
        c = a/b;
        str[i++]=c+48;
        a%=b;
    }
    str[i++] = '.';
    
    /* extracting decimal digits till precision */

    for(l=0;l<precision;l++)
    {
        f*=10.0;
        b = f;
        str[i++]=b+48;
        f-=b;
    }
    str[i]='\0';
    return (str);
}