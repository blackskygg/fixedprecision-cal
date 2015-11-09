# fixedprecision-cal
##Description
####1.this calculator only supports + - * /  

####2.this calculator doesn't support numbers that can't be represented in `long double`  

####3.this calculator supports following forms of numbers:  

  decimals: `123.234`, `1024` etc.  
  
  hexicals: `0xfe4f`, `0xfeef.fe112fe` etc.  
  
  binaries: `0101010.0101010111b`  
  
  octals: `012347`, `0123.741312` etc.  
    
  

##USAGE(I think a few examples will help)
###Interactive mode
`>>1+2+3+4*5*(2+3)`  

>= 106.000000  

`>>01010100.01001000100b +1.1010010010b`  

>= 85.925781  

`>>0xff`
>= 255.000000  

`>>1101.10b` 
>= 13.500000  

`>>0125`
>= 85.000000  

`>>oxff.5`
>tokenizer error: unknown token  
>near position oxff.5  

`>>0xff.5`
>= 255.312500  

`>>0125.1`
>= 85.125000  

`>>0x12.fe + -0101010.01110b + 0172717.71237 * 234 + (45.11 - 0x1123.123ff)`
>= 14720762.135086  

`>>exit`  
>(program exited)  
  
  
###one-line mode
`./cal   '1+2+3+4*5*(2+3)'  '0x12.fe + -0101010.01110b + 0172717.71237 * 234 + (45.11 - 0x1123.123ff)'`
>1+2+3+4*5*(2+3) = 106.000000  
>0x12.fe + -0101010.01110b + 0172717.71237 * 234 + (45.11 - 0x1123.123ff) = 14720762.135086

